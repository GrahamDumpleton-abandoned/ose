/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcepismesg.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/epismesg.hh"
#endif
#endif

#include <OTC/message/epismesg.hh>

#include <OTC/message/congest.hh>
#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/ioevent.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/vlink.hh>
#include <OTC/dispatch/action.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/dispatch/jobqueue.hh>
#include <OTC/thread/nrmutex.hh>
#include <OTC/system/sighndle.hh>

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

#if defined(HAVE_SYS_PARAM_H)
#include <sys/param.h>
#endif

#if defined(HAVE_MEMORY_H)
#include <memory.h>
#endif

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#if defined(SYS_UNIX)
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#if defined(shutdown)
#undef shutdown
#endif
#else
#if defined(HAVE_WINSOCK2)
#include <winsock2.h>
#endif
#include <windows.h>
#endif

#if defined(WSAEWOULDBLOCK)
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif
#if !defined(EWOULDBLOCK) && defined(EAGAIN)
#define EWOULDBLOCK EAGAIN
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_EPISMessage");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_EPISMessage::mutex_;
u_int OTC_EPISMessage::gBatch_ = 0;
char OTC_EPISMessage::gMagic_[4] = { '\252', '\252', '0', '3' };
u_int OTC_EPISMessage::gThreshold_ = 0;

/* ------------------------------------------------------------------------- */
OTC_EPISMessage::~OTC_EPISMessage()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISMessage::~OTC_EPISMessage()");

  OTCEV_Action::cancelAgent(id());
  OTCEV_IOEvent::unsubscribeAgent(id());

  // List only holds envelopes and doesn't
  // control deletion of them. Must therefore
  // ensure that they are destroyed.

  OTC_LinkIterator theItems = outgoing_->items();
  theItems.resetFirst();
  while (theItems.isLink())
  {
    OTC_VLink* theLink;
    theLink = (OTC_VLink*)theItems.link();

    if (theLink->item() != 0)
    {
      OTCEV_Envelope* theEnvelope;
      theEnvelope = (OTCEV_Envelope*)theLink->item();
      theEnvelope->destroy();
    }

    theItems.link()->kill();

    theItems.next();
  }

  outgoing_->unReference();
}

/* ------------------------------------------------------------------------- */
OTC_EPISMessage::OTC_EPISMessage(OTC_EndPoint* theEndPoint)
  : OTC_EPISocket(theEndPoint),
    state_(OTCLIB_EPISMESSAGE_INACTIVE),
    fd_(INVALID_SOCKET),
    nonblocking_(OTCLIB_FALSE),
    outgoingCount_(0),
    backlogCount_(0),
    waitingToWrite_(OTCLIB_FALSE),
    iphase_(0),
    ilength_(0),
    imessage_(OTC_Capacity(256)),
    olength_(0),
    omessage_(OTC_Capacity(256)),
    suspended_(OTCLIB_FALSE),
    stopping_(OTCLIB_FALSE)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISMessage::OTC_EPISMessage()");

  mutex_.lock();

  if (gBatch_ == 0)
  {
    char* var;
    var = getenv("OTCLIB_MESSAGEBATCH");
    if (var != 0)
    {
      int i = atoi(var);
      if (i > 0)
	gBatch_ = i;
    }

    if (gBatch_ == 0)
      gBatch_ = 32;
  }

  if (gThreshold_ == 0)
  {
    char* var;
    var = getenv("OTCLIB_MESSAGETHRESHOLD");
    if (var != 0)
    {
      int i = atoi(var);
      if (i > 0)
	gThreshold_ = i;
    }

    if (gThreshold_ == 0)
      gThreshold_ = 256;
  }

  mutex_.unlock();

  outgoing_ = new OTC_LinkList;
  outgoing_->reference();

  // Ignore instances of SIGPIPE.

#if defined(SIGPIPE)
  OTC_SignalHandler::install(SIGPIPE,(void (*)(int))SIG_IGN);
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_EPISMessage::attach(int theFd)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISMessage::attach(int)");

  OTCLIB_TRACER(MODULE) << "fd = " << theFd << endl;

  OTCLIB_ENSURE_FN((state_ == OTCLIB_EPISMESSAGE_INACTIVE),
   "OTC_EPISMessage::attach(int)",
   "active connection already exists");

  OTCLIB_ENSURE_FN((theFd >= 0),
   "OTC_EPISMessage::attach(int)",
   "invalid file descriptor");

  fd_ = theFd;

  // Determine if file descriptor is non blocking.

  // XXX Not relying on whether can be determined
  // that socket is non blocking any more.

  nonblocking_ = OTCLIB_FALSE;

#if defined(SYS_UNIX)
  int theFlags;
  theFlags = fcntl(fd_,F_GETFL,0);
#if defined(O_NONBLOCK)
  if (theFlags != -1 && theFlags & O_NONBLOCK)
    nonblocking_ = OTCLIB_TRUE;
#endif
#if defined(O_NDELAY)
  if (theFlags != -1 && theFlags & O_NDELAY)
    nonblocking_ = OTCLIB_TRUE;
#endif
#else
  // Assume Win32 is non blocking.

  nonblocking_ = OTCLIB_TRUE;
#endif

  OTCLIB_TRACER(MODULE) << "nonblocking = " << nonblocking_ << endl;

  // Go into start state.

  suspended_ = OTCLIB_FALSE;
  stopping_ = OTCLIB_FALSE;

  state_ = OTCLIB_EPISMESSAGE_STARTING;

  OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;

  // Send an action to ourselves so we send out
  // message to set remote address for other end
  // of the connection.

  OTCEV_Action::schedule(id(),OTCLIB_PRIORITY_JOB,"START");
}

/* ------------------------------------------------------------------------- */
void OTC_EPISMessage::suspendInput()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISMessage::suspendInput()");

  if (fd_ == INVALID_SOCKET)
    return;

  if (suspended_ != OTCLIB_FALSE)
    return;

  suspended_ = OTCLIB_TRUE;

  if (state_ == OTCLIB_EPISMESSAGE_CONNECTED)
    OTCEV_IOEvent::unsubscribe(id(),fd_,OTCLIB_POLLIN);
}

/* ------------------------------------------------------------------------- */
void OTC_EPISMessage::resumeInput()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISMessage::resumeInput()");

  if (fd_ == INVALID_SOCKET)
    return;

  if (suspended_ == OTCLIB_FALSE)
    return;

  suspended_ = OTCLIB_FALSE;

  if (state_ == OTCLIB_EPISMESSAGE_CONNECTED)
    OTCEV_IOEvent::subscribe(id(),fd_,OTCLIB_POLLIN);
}

/* ------------------------------------------------------------------------- */
int OTC_EPISMessage::detach()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISMessage::detach()");

  OTCLIB_TRACER(MODULE) << "fd = " << fd_ << endl;

  OTCLIB_ENSURE_FN((fd_ != INVALID_SOCKET),
   "OTC_EPISMessage::detach()",
   "no active connection exists");

  // Unsubscribe from file descriptor.

  OTCEV_IOEvent::unsubscribe(id(),fd_);

  // Wipe out file descriptor.

  int theOldFd;
  
  theOldFd = fd_;
  fd_ = INVALID_SOCKET;

  // Forget whether we were waiting to write out
  // anything.

  waitingToWrite_ = OTCLIB_FALSE;

  // Drop out of any read/write operations we are in.

  iphase_ = 0;

  imessage_.length(0);

  // Go into sleep state until given new file
  // descriptor for connection. It is assumed
  // that the caller closed the file
  // descriptor.

  suspended_ = OTCLIB_FALSE;
  stopping_ = OTCLIB_FALSE;

  state_ = OTCLIB_EPISMESSAGE_INACTIVE;

  OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;

  // Wipe out remote address.

  remote_.truncate();

  unbindRemoteAddress();

  return theOldFd;
}

/* ------------------------------------------------------------------------- */
void OTC_EPISMessage::handle(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISMessage::handle(OTC_Event*)");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << "event = " << *theEvent << endl;

  if (theEvent->type() == OTCEV_Action::typeId())
  {
    OTCEV_Action* theAction;
    theAction = (OTCEV_Action*)theEvent;

    // Request to perform address setup.

    if (theAction->description() == "START")
    {
      if ((type() == OTCLIB_ENDPOINT_CLIENT
       && state_ == OTCLIB_EPISMESSAGE_STARTING)
       || (type() == OTCLIB_ENDPOINT_SERVER
       && state_ == OTCLIB_EPISMESSAGE_WAITING)) 
      {
	// About to try and negotiate new connection.
	// Make sure we go through queued messages
	// and delete any control messages pending
	// from old connection.

	OTC_LinkIterator theItems = outgoing_->items();
	theItems.resetFirst();
	while (theItems.isLink())
	{
	  OTC_VLink* theLink;
	  theLink = (OTC_VLink*)theItems.link();

	  if (theLink->item() != 0)
	  {
	    OTCEV_Envelope* theEnvelope;
	    theEnvelope = (OTCEV_Envelope*)theLink->item();

	    if (theEnvelope->currentReceiver().isEmpty())
	    {
	      theEnvelope->destroy();
	      theItems.link()->kill();

	      OTCLIB_ASSERT(outgoingCount_ > 0);
	      outgoingCount_--;

	      messageSent_();
	    }
	  }

	  theItems.next();
	}

	// Create an envelope with empty message and
	// empty destination. An empty destination
	// will be interpreted at the other end as
	// being a control message. Subject of
	// message is "$EP::ANNOUNCE" to let other end
	// know we are here.

	OTCEV_Message* theMessage;
	theMessage = new OTCEV_Message("");

	OTCEV_Envelope* theEnvelope;
	theEnvelope = new OTCEV_Envelope(theMessage,"",
	 localAddress(),"$EP::ANNOUNCE","",0,0);

	pack_(theEnvelope);

	theEnvelope->destroy();

	// Write out the message. Keep trying until
	// all of its is successfully written or an
	// error is encountered. Presumably this isn't
	// needed as the connection has only just been
	// created and there should be sufficient space
	// in the system buffers. However, if this is
	// non blocking file descriptor, anything is
	// possible. Although the signal model use by
	// the OSE library shouldn't allow a return
	// from send() when an interupt occurs, we also
	// check that just in case.

	int theResult = SOCKET_ERROR;
	int theError = 0;
	int theMessageLength;
	char const* theMessageBuffer;

	theMessageLength = omessage_.length();
	theMessageBuffer = omessage_.buffer();

	while (theMessageLength != 0)
	{
	  theResult = send(fd_,(char*)theMessageBuffer,theMessageLength,0);

#if defined(SYS_UNIX)
	  int theLastError = errno;
#else
	  int theLastError = WSAGetLastError();
#endif

	  if (theResult == SOCKET_ERROR && theLastError != EAGAIN &&
	   theLastError != EWOULDBLOCK && theLastError != EINTR)
	  {
	    theError = theLastError;
	    break;
	  }

	  if (theResult > 0)
	  {
	    theMessageLength -= theResult;
	    theMessageBuffer += theResult;
	  }
	}

	omessage_.length(0);

	if (theResult == SOCKET_ERROR)
	{
	  state_ = OTCLIB_EPISMESSAGE_INACTIVE;

	  OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;

	  // Unsubscribe to file descriptor in case
	  // we have subscribed to it already.

	  OTCEV_IOEvent::unsubscribe(id(),fd_);

	  // Wipe out remote address. There shouldn't
	  // be one at this point. We do this though
	  // so that a OTCEV_Connection event is still
	  // generated with an empty remote address
	  // indicating that the socket connection
	  // attempt faild.

	  remote_.truncate();

	  unbindRemoteAddress();

	  // Call derived class to close file
	  // descriptor and optionally provide a new
	  // file descriptor. If we do not receive a
	  // new file descriptor from shutdown()
	  // function, this class may have been deleted
	  // so we cannot touch any member variables of
	  // the class if that is the case.

	  int theOldFd;
	  int theNewFd = INVALID_SOCKET;

	  theOldFd = fd_;
	  fd_ = INVALID_SOCKET;

	  void* theNewFdP = 0;
	  theNewFdP = shutdown((void*)&theOldFd,theError);

	  if (theNewFdP != 0)
	    theNewFd = *(int*)theNewFdP;

	  // Received a new file descriptor so start up
	  // connection once more.

	  if (theNewFd >= 0)
	  {
	    fd_ = theNewFd;

	    suspended_ = OTCLIB_FALSE;
	    stopping_ = OTCLIB_FALSE;

	    state_ = OTCLIB_EPISMESSAGE_STARTING;

	    OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;

	    OTCEV_Action::schedule(id(),OTCLIB_PRIORITY_JOB,"START");
	  }
	}
	else
	{
	  if (type() == OTCLIB_ENDPOINT_CLIENT)
	  {
	    // Subscribe to file descriptor so we know
	    // when we have got a message from the other
	    // end with its address.

	    OTCEV_IOEvent::subscribe(id(),fd_,OTCLIB_POLLIN);

	    // Wait for remote address message from other
	    // end. Cannot flush what is in the message
	    // queue until we get the remote address for
	    // the other end.

	    state_ = OTCLIB_EPISMESSAGE_WAITING;

	    OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;
	  }
	  else
	  {
	    // Move into connected state.

	    state_ = OTCLIB_EPISMESSAGE_CONNECTED;

	    OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;

	    bindRemoteAddress(remote_);

	    // See if we have been suspended already.

	    if (suspended_ != OTCLIB_FALSE)
	      OTCEV_IOEvent::unsubscribe(id(),fd_,OTCLIB_POLLIN);

	    // If messages already queued, make an attempt to
	    // flush them out.

	    if (!outgoing_->isEmpty())
	    {
	      OTC_Event* theIOEvent;
	      theIOEvent = new OTCEV_IOEvent(fd_,OTCLIB_POLLOUT);
	      theIOEvent->queue(id(),OTCLIB_STANDARD_JOB);
	    }
	  }
	}
      }
      else if (type() == OTCLIB_ENDPOINT_SERVER
       && state_ == OTCLIB_EPISMESSAGE_STARTING)
      {
	// Subscribe to file descriptor so we know
	// when we receive address setup message from
	// client.

	OTCEV_IOEvent::subscribe(id(),fd_,OTCLIB_POLLIN);
      }
    }
    else if (theAction->description() == "CLOSE")
    {
      if (state_ != OTCLIB_EPISMESSAGE_INACTIVE)
      {
	if (outgoingCount_ == 0)
	{
	  state_ = OTCLIB_EPISMESSAGE_INACTIVE;

	  OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;

	  // Unsubscribe to file descriptor in case
	  // we have subscribed to it already.

	  OTCEV_IOEvent::unsubscribe(id(),fd_);

	  // Wipe out remote address. There shouldn't
	  // be one at this point. We do this though
	  // so that a OTCEV_Connection event is still
	  // generated with an empty remote address
	  // indicating that the socket connection
	  // attempt faild.

	  remote_.truncate();

	  unbindRemoteAddress();

	  // Call derived class to close file
	  // descriptor and optionally provide a new
	  // file descriptor. If we do not receive a
	  // new file descriptor from shutdown()
	  // function, this class may have been deleted
	  // so we cannot touch any member variables of
	  // the class if that is the case.

	  int theOldFd;
	  int theNewFd = INVALID_SOCKET;

	  theOldFd = fd_;
	  fd_ = INVALID_SOCKET;

	  void* theNewFdP = 0;
	  theNewFdP = shutdown((void*)&theOldFd,0);

	  if (theNewFdP != 0)
	    theNewFd = *(int*)theNewFdP;

	  // Received a new file descriptor so start up
	  // connection once more.

	  if (theNewFd >= 0)
	  {
	    fd_ = theNewFd;

	    suspended_ = OTCLIB_FALSE;
	    stopping_ = OTCLIB_FALSE;

	    state_ = OTCLIB_EPISMESSAGE_STARTING;

	    OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;

	    OTCEV_Action::schedule(id(),OTCLIB_PRIORITY_JOB,"START");
	  }
	}
	else
	{
	  stopping_ = OTCLIB_TRUE;
	}
      }
    }

    // Destroy the event we received.

    theEvent->destroy();
  }
  else if (theEvent->type() == OTCEV_IOEvent::typeId())
  {
    OTCEV_IOEvent* theIOEvent;
    theIOEvent = (OTCEV_IOEvent*)theEvent;

    int theError = 0;

    if (state_ != OTCLIB_EPISMESSAGE_INACTIVE &&
     (theIOEvent->events() & OTCLIB_POLLOUT))
    {
      // If we were waiting to be able to write to
      // the file descriptor, cancel our
      // subscription so we do not keep getting
      // notified. We may resubscribe later on if
      // we cannot write everything we have
      // waiting.

      if (waitingToWrite_ != OTCLIB_FALSE)
      {
	OTCLIB_TRACER(MODULE) << "connection no longer blocked" << endl;

	OTCEV_IOEvent::unsubscribe(id(),fd_,OTCLIB_POLLOUT);

	waitingToWrite_ = OTCLIB_FALSE;

	OTCEV_Congestion* theCongestion;
	theCongestion = new OTCEV_Congestion(localAddress(),
	 remoteAddress(),OTCLIB_CONNECTION_CLEARED,outgoingCount_);
	OTCLIB_ASSERT_M(theCongestion != 0);

	endPoint()->notifyObservers(theCongestion);
      }

      // Send out messages until we can send no more.

      OTC_Boolean theFinish;
      theFinish = OTCLIB_FALSE;

      u_int theMessagesSent = 0;

      while (theFinish == OTCLIB_FALSE)
      {
        // If we do not have part of a message still to write out
	// we need to format the next message to send.

	OTC_VLink* theLink = 0;

	theLink = (OTC_VLink*)outgoing_->first();

	if (theLink == 0)
	{
	  theFinish = OTCLIB_TRUE;
	  continue;
	}

	if (olength_ == 0)
	{
	  // If length is zero but outgoing message string is
	  // not zero length, it means that the last time through
	  // we had written out the last of the message. In this
	  // case, the first message on the queue is what was
	  // in the formatted string and we need to delete it.
	  // Note that this probably shouldn't occur, as the complete
	  // writing of a message should be detected down further
	  // and the message deleted there. Do this here just in
	  // case though.

	  if (!omessage_.isEmpty())
	  {
	    omessage_.length(0);

	    ((OTCEV_Envelope*)theLink->item())->destroy();
	    theLink->kill();

	    OTCLIB_ASSERT(outgoingCount_ > 0);
	    outgoingCount_--;

	    messageSent_();

	    theLink = (OTC_VLink*)outgoing_->first();

	    if (theLink == 0)
	    {
	      theFinish = OTCLIB_TRUE;
	      continue;
	    }
	  }

	  // If next link in queue doesn't actually contain a
	  // message jump back to beginning of loop and get next
	  // one.

	  OTCLIB_ASSERT(theLink->item() != 0);

	  if (theLink->item() == 0)
	  {
	    theLink->kill();

	    OTCLIB_ASSERT(outgoingCount_ > 0);
	    outgoingCount_--;

	    messageSent_();

	    continue;
	  }

	  // First message in the queue is the next message to send.
	  // First filter out messages we shouldn't send.

	  OTCEV_Envelope* theEnvelope;
	  theEnvelope = (OTCEV_Envelope*)theLink->item();

	  if (theEnvelope->sequenceNumber() != 0)
	  {
	    OTC_String theInBoxName;
	    theInBoxName = theEnvelope->currentReceiver();

	    if (!theInBoxName.isEmpty() && theInBoxName != remoteAddress())
	    {
	      OTC_LogStream theLogger;
	      theLogger.setLevel(OTCLIB_LOG_WARNING);

	      theLogger << "Operation failed" << EOL;
	      theLogger << "Class: OTC_EPISMessage" << EOL;
	      theLogger << "Method: handle()" << EOL;
	      theLogger << "Description: cannot route message" << EOL;
	      theLogger << "Result: message will be discarded" << EOL;
	      theLogger << "Comment: connection may have been lost" << EOL;
	      theLogger << "Event: " << *theEnvelope << EOL;
	      theLogger << flush;

	      // Drop back to beginning of loop and get next one.

	      ((OTCEV_Envelope*)theLink->item())->destroy();
	      theLink->kill();

	      OTCLIB_ASSERT(outgoingCount_ > 0);
	      outgoingCount_--;

	      messageSent_();

	      continue;
	    }
	  }

	  // Make sure we have a message to send. We
	  // might not if the event cannot be deflated.

	  if (theEnvelope->message() == 0)
	  {
	    OTC_LogStream theLogger;
	    theLogger.setLevel(OTCLIB_LOG_WARNING);

	    theLogger << "Operation failed" << EOL;
	    theLogger << "Class: OTC_EPISMessage" << EOL;
	    theLogger << "Method: handle()" << EOL;
	    theLogger << "Description: cannot deflate event" << EOL;
	    theLogger << "Result: message will be discarded" << EOL;
	    theLogger << "Comment: no deflate method for event" << EOL;
	    theLogger << "Event: " << *theEnvelope << EOL;
	    theLogger << flush;

	    // Drop back to beginning of loop and get next one.

	    ((OTCEV_Envelope*)theLink->item())->destroy();
	    theLink->kill();

	    OTCLIB_ASSERT(outgoingCount_ > 0);
	    outgoingCount_--;

	    messageSent_();

	    continue;
	  }

	  // First message in queue is now the one we want
	  // to format into the outgoing message string.
	  // Do not need to check if message is empty as
	  // have verified above that event could be deflated.

	  pack_(theEnvelope);

	  olength_ = omessage_.length();

	  OTCLIB_ASSERT(olength_ != 0);
	}

	int theResult = 0;
	char const* theMessageBuffer;

	theMessageBuffer = omessage_.buffer() + omessage_.length();
	theMessageBuffer -= olength_;

	theError = 0;

	while (olength_ != 0)
	{
	  theResult = send(fd_,(char*)theMessageBuffer,olength_,0);

	  if (theResult == 0 || theResult == SOCKET_ERROR)
	  {
#if defined(SYS_UNIX)
	    int theLastError = errno;
#else
	    int theLastError = WSAGetLastError();
#endif

	    // XXX Don't rely on nonblocking being set.
	    // if (nonblocking_ != OTCLIB_FALSE && (theResult == 0 ||
	    //  theLastError == EAGAIN || theLastError == EWOULDBLOCK))
	    if (theResult == SOCKET_ERROR &&
	     (theLastError == EAGAIN || theLastError == EWOULDBLOCK))
	    {
	      OTCLIB_TRACER(MODULE) << "connection blocked" << endl;

	      OTCLIB_ASSERT(waitingToWrite_ == OTCLIB_FALSE);

	      waitingToWrite_ = OTCLIB_TRUE;

	      OTCEV_IOEvent::subscribe(id(),fd_,OTCLIB_POLLOUT);

	      OTCEV_Congestion* theCongestion;
	      theCongestion = new OTCEV_Congestion(localAddress(),
	       remoteAddress(),OTCLIB_CONNECTION_BLOCKED,outgoingCount_);
	      OTCLIB_ASSERT_M(theCongestion != 0);

	      endPoint()->notifyObservers(theCongestion);

	      // Force backlog events.

	      backlogCount_ = gThreshold_;

	      theFinish = OTCLIB_TRUE;

	      break;
	    }
	    else if (theResult == SOCKET_ERROR && theLastError != EAGAIN &&
	     theLastError != EWOULDBLOCK && theLastError != EINTR)
	    {
	      theError = theLastError;

	      break;
	    }
	  }

	  if (theResult > 0)
	  {
	    olength_ -= theResult;
	    theMessageBuffer += theResult;
	  }
	}

	// If we succesfully wrote the message out,
	// remove the corresponding message from the
	// queue and truncate the outgoing message string.

	if (olength_ == 0)
	{
	  omessage_.length(0);

	  ((OTCEV_Envelope*)theLink->item())->destroy();
	  theLink->kill();

	  OTCLIB_ASSERT(outgoingCount_ > 0);
	  outgoingCount_--;

	  messageSent_();

	  theMessagesSent++;
	  if (theMessagesSent >= gBatch_)
	  {
	    OTCLIB_TRACER(MODULE) << "batch = " << theMessagesSent << endl;

	    theFinish = OTCLIB_TRUE;

	    if (outgoingCount_ > 0)
	    {
	      OTC_Event* tmpIOEvent;
	      tmpIOEvent = new OTCEV_IOEvent(fd_,OTCLIB_POLLOUT);
	      tmpIOEvent->queue(id(),OTCLIB_STANDARD_JOB);
	    }

	    continue;
	  }
	}

	// If there was an error in writing out the
	// header or body, shut the connection down,
	// but leave the envelope in the queue in
	// case the connection is restarted. Have to
	// wipe out the message in the output string
	// as we need to re-evaulate as to whether it
	// should be sent on any new connection.

	if (theError != 0)
	{
	  theFinish = OTCLIB_TRUE;

	  olength_ = 0;
	  omessage_.length(0);

	  state_ = OTCLIB_EPISMESSAGE_INACTIVE;

	  OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;
	}
      }

      if (outgoingCount_ == 0 && stopping_ != OTCLIB_FALSE)
      {
	state_ = OTCLIB_EPISMESSAGE_INACTIVE;

	OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;

	// Unsubscribe to file descriptor in case
	// we have subscribed to it already.

	OTCEV_IOEvent::unsubscribe(id(),fd_);

	// Wipe out remote address. There shouldn't
	// be one at this point. We do this though
	// so that a OTCEV_Connection event is still
	// generated with an empty remote address
	// indicating that the socket connection
	// attempt faild.

	remote_.truncate();

	unbindRemoteAddress();

	// Call derived class to close file
	// descriptor and optionally provide a new
	// file descriptor. If we do not receive a
	// new file descriptor from shutdown()
	// function, this class may have been deleted
	// so we cannot touch any member variables of
	// the class if that is the case.

	int theOldFd;
	int theNewFd = INVALID_SOCKET;

	theOldFd = fd_;
	fd_ = INVALID_SOCKET;

	void* theNewFdP = 0;
	theNewFdP = shutdown((void*)&theOldFd,0);

	if (theNewFdP != 0)
	  theNewFd = *(int*)theNewFdP;

	// Received a new file descriptor so start up
	// connection once more.

	if (theNewFd >= 0)
	{
	  fd_ = theNewFd;

	  suspended_ = OTCLIB_FALSE;
	  stopping_ = OTCLIB_FALSE;

	  state_ = OTCLIB_EPISMESSAGE_STARTING;

	  OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;

	  OTCEV_Action::schedule(id(),OTCLIB_PRIORITY_JOB,"START");
	}
      }
    }

    if (state_ != OTCLIB_EPISMESSAGE_INACTIVE &&
     (theIOEvent->events() & OTCLIB_POLLIN))
    {
      OTCLIB_TRACER(MODULE) << "phase = " << iphase_ << endl;

      int theResult = 0;
      int theMessageLength;
      char* theMessageBuffer;

      int const SZINT = sizeof(u_int);

      // If we were waiting for a message, read
      // in the magic header and the length of
      // the message.

      if (iphase_ == 0)
      {
	OTCLIB_MARKBLOCK(MODULE,"PHASE 0");

        // XXX

#if 0
	theMessageLength = sizeof(gMagic_) + SZINT;

	imessage_.length(theMessageLength);

	theMessageBuffer = imessage_.buffer();
#endif

        // XXX

	int const SZHDR = sizeof(gMagic_) + SZINT;

        if (imessage_.length() == 0)
	{
	  theMessageLength = SZHDR;

	  imessage_.length(theMessageLength);

	  theMessageBuffer = imessage_.buffer();
	}
	else
	{
	  // If we come through here, it means that got
	  // EWOULDBLOCK previous time while trying to
	  // read this first part of the message but
	  // that part of the message had actually been
	  // read.

	  theMessageBuffer = imessage_.buffer();
	  theMessageBuffer += imessage_.length();

	  theMessageLength = SZHDR - imessage_.length();

	  imessage_.length(SZHDR);
	}

	// XXX

	while (theMessageLength != 0)
	{
	  theResult = recv(fd_,theMessageBuffer,theMessageLength,0);

#if defined(SYS_UNIX)
	  int theLastError = errno;
#else
	  int theLastError = WSAGetLastError();
#endif

	  if (theResult == SOCKET_ERROR && theLastError != EINTR)
	  {
	    theError = theLastError;
	    break;
	  }

	  if (theResult == 0)
	  {
	    theResult = SOCKET_ERROR;
	    theError = 0;
	    break;
	  }

	  if (theResult > 0)
	  {
	    theMessageLength -= theResult;
	    theMessageBuffer += theResult;
	  }
	}

	if (theResult != SOCKET_ERROR)
        {
	  // Verify magic header.

	  if (!imessage_.compare(0,gMagic_,sizeof(gMagic_)))
	  {
	    theResult = SOCKET_ERROR;
	    theError = -1;
	  }
	}

	if (theResult != SOCKET_ERROR)
	{
          u_int theHLength;
          u_int theNLength;

	  // Extract the length of the message.

	  memcpy((char*)&theNLength,imessage_.buffer()+sizeof(gMagic_),SZINT);
	  theHLength = ntohl(theNLength);

	  // Resize the length of the message and
	  // remember how much is left to be read in.

	  imessage_.length(theHLength+sizeof(gMagic_)+SZINT);
	  ilength_ = theHLength;

	  OTCLIB_TRACER(MODULE)
	   << "message length to read = " << ilength_ << endl;

	  // Move into phase 1.

	  iphase_ = 1;
	}

	// XXX

	if (theResult == SOCKET_ERROR &&
	 (theError == EWOULDBLOCK || theError == EAGAIN))
	{
	  OTCLIB_TRACER(MODULE) << "recv() blocked" << endl;

	  imessage_.length(SZHDR-theMessageLength);

	  theResult = 0;
	}

	// XXX
      }

      if (iphase_ == 1)
      {
	OTCLIB_MARKBLOCK(MODULE,"PHASE 1");

	// Read in what we can of the message.

	theMessageBuffer = imessage_.buffer();
	theMessageBuffer += (imessage_.length() - ilength_);

	theResult = SOCKET_ERROR;

	while (theResult == SOCKET_ERROR)
	{
	  theResult = recv(fd_,theMessageBuffer,ilength_,0);

#if defined(SYS_UNIX)
	  int theLastError = errno;
#else
	  int theLastError = WSAGetLastError();
#endif

	  if (theResult == SOCKET_ERROR && theLastError != EINTR)
	  {
	    theError = theLastError;
	    break;
	  }

	  if (theResult == 0)
	  {
	    theResult = SOCKET_ERROR;
	    theError = 0;
	    break;
	  }
	}

	if (theResult > 0)
	  ilength_ -= theResult;

	OTCLIB_TRACER(MODULE)
	 << "message length to read = " << ilength_ << endl;

	if (ilength_ == 0)
	{
	  iphase_ = 0;

	  // We have the complete message.
	  // Construct the envelope and message and
	  // work out what we should do with it.

	  OTCEV_Envelope* theEnvelope;
	  theEnvelope = unpack_();

	  imessage_.length(0);

	  if (theEnvelope == 0)
	  {
	    OTC_LogStream theLogger;
	    theLogger.setLevel(OTCLIB_LOG_WARNING);

	    theLogger << "Operation failed" << EOL;
	    theLogger << "Class: OTC_EPISMessage" << EOL;
	    theLogger << "Method: handle()" << EOL;
	    theLogger << "Description: cannot inflate header/body" << EOL;
	    theLogger << "Result: message will be discarded" << EOL;
	    theLogger << "Comment: possibly an implementation problem" << EOL;
	    theLogger << flush;

	    state_ = OTCLIB_EPISMESSAGE_INACTIVE;

	    OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;
	  }
	  else
	  {
	    if (theEnvelope->to().isEmpty())
	    {
	      // Control message from remote end.

	      OTCLIB_TRACER(MODULE) << "event = " << *theEnvelope << endl;

	      if (theEnvelope->event()->type() == OTCEV_Message::typeId())
	      {
		if (theEnvelope->subject() == "$EP::ANNOUNCE")
		{
		  if ((type() == OTCLIB_ENDPOINT_CLIENT
		   && state_ == OTCLIB_EPISMESSAGE_WAITING)
		   || (type() == OTCLIB_ENDPOINT_SERVER
		   && state_ == OTCLIB_EPISMESSAGE_STARTING)) 
		  {
		    // Other end is letting us know it is there.

		    remote_ = theEnvelope->from();

		    OTCLIB_TRACER(MODULE) << "remote = " << remote_ << endl;

		    if (type() == OTCLIB_ENDPOINT_SERVER)
		    {
		      state_ = OTCLIB_EPISMESSAGE_WAITING;

		      OTCLIB_TRACER(MODULE) << "state = "
		       << (int)state_ << endl;

		      // Send of address setup message to client.

		      OTCEV_Action::schedule(id(),OTCLIB_PRIORITY_JOB,"START");
		    }
		    else
		    {
		      state_ = OTCLIB_EPISMESSAGE_CONNECTED;

		      OTCLIB_TRACER(MODULE) << "state = "
		       << (int)state_ << endl;

		      bindRemoteAddress(remote_);

		      // See if we have been suspended already.

		      if (suspended_ != OTCLIB_FALSE)
			OTCEV_IOEvent::unsubscribe(id(),fd_,OTCLIB_POLLIN);

		      // If messages already queued, make an attempt to
		      // flush them out.

		      if (!outgoing_->isEmpty())
		      {
			OTC_Event* tmpIOEvent;
			tmpIOEvent = new OTCEV_IOEvent(fd_,OTCLIB_POLLOUT);
			tmpIOEvent->queue(id(),OTCLIB_STANDARD_JOB);
		      }
		    }
		  }
		}
	      }

	      theEnvelope->destroy();
	    }
	    else
	    {
	      theEnvelope->queue(OTCLIB_STANDARD_JOB);
	    }
	  }
	}

	// XXX

	if (theResult == SOCKET_ERROR &&
	 (theError == EWOULDBLOCK || theError == EAGAIN))
	{
	  OTCLIB_TRACER(MODULE) << "recv() blocked" << endl;

	  theResult = 0;
	}

	// XXX
      }

      if (theResult == SOCKET_ERROR)
      {
	iphase_ = 0;

	imessage_.length(0);

	state_ = OTCLIB_EPISMESSAGE_INACTIVE;

	OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;
      }
    }

    // Now check for error conditions.

    if (state_ != OTCLIB_EPISMESSAGE_INACTIVE &&
     theIOEvent->events() & OTCLIB_POLLERR)
    {
      // Do not really now if I expect to get this
      // sort of error message back. Take it as being
      // an I/O error and close connection down.

      theError = EIO;

      state_ = OTCLIB_EPISMESSAGE_INACTIVE;

      OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;
    }

    if (state_ != OTCLIB_EPISMESSAGE_INACTIVE &&
     theIOEvent->events() & OTCLIB_POLLNVAL)
    {
      // An invalid file descriptor.

      theError = EBADF;

      state_ = OTCLIB_EPISMESSAGE_INACTIVE;

      OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;
    }

    if (state_ != OTCLIB_EPISMESSAGE_INACTIVE &&
     theIOEvent->events() & OTCLIB_POLLHUP)
    {
      // Received hangup for connection. Most likely
      // had a POLLIN at the same time which has already
      // realised that connection has closed, however
      // still check here just in case.

      theError = 0;

      state_ = OTCLIB_EPISMESSAGE_INACTIVE;

      OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;
    }

    if (state_ == OTCLIB_EPISMESSAGE_INACTIVE)
    {
      // Guard against coming in here when we are attached
      // to no file descriptor. Can happen if connection is
      // closed due to a write when notice that there is
      // something to read was cached in the dispatcher.

      if (fd_ != INVALID_SOCKET)
      {
	// Unsubscribe from file descriptor.

	OTCEV_IOEvent::unsubscribe(id(),fd_);

	// Forget whether we were waiting to write out
	// anything.

	waitingToWrite_ = OTCLIB_FALSE;

	// Drop out of any read/write operations we are in.

	iphase_ = 0;

	imessage_.length(0);

	// Wipe out file descriptor.

	int theOldFd;
	theOldFd = fd_;
	fd_ = INVALID_SOCKET;

	// Wipe out remote address.

	remote_.truncate();

	unbindRemoteAddress();

	// Call derived class to close file
	// descriptor and optionally provide a new
	// file descriptor. If we do not receive a
	// new file descriptor from shutdown()
	// function, this class may have been deleted
	// so we cannot touch any member variables of
	// the class if that is the case.

	int theNewFd = INVALID_SOCKET;

	void* theNewFdP = 0;
	theNewFdP = shutdown((void*)&theOldFd,theError);

	if (theNewFdP != 0)
	  theNewFd = *(int*)theNewFdP;

	// Received a new file descriptor so start up
	// connection once more.

	if (theNewFd >= 0)
	{
	  fd_ = theNewFd;

	  suspended_ = OTCLIB_FALSE;
	  stopping_ = OTCLIB_FALSE;

	  state_ = OTCLIB_EPISMESSAGE_STARTING;

	  OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;

	  OTCEV_Action::schedule(id(),OTCLIB_PRIORITY_JOB,"START");
	}
      }
    }

    // Destroy the event we received.

    theEvent->destroy();
  }
  else if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCEV_Envelope* theEnvelope;
    theEnvelope = (OTCEV_Envelope*)theEvent;

    // Add the message to the queue.

    OTC_Link* theLink;
    theLink = new OTC_VLink((void*)theEnvelope);

    outgoing_->addLast(theLink);
    outgoingCount_++;

    messageQueued_();

    // If in a connected state, and not already
    // waiting for the ability to write to the
    // file descriptor, create a dummy event
    // saying we can write to the file descriptor
    // and send it to ourselves in order to try
    // and flush out what is in the queue. Do
    // not do anything after the event is delivered
    // as the connection could have been shutdown
    // and the instance of this class deleted.

    if (state_ == OTCLIB_EPISMESSAGE_CONNECTED)
    {
      if (waitingToWrite_ == OTCLIB_FALSE)
      {
	OTCLIB_TRACER(MODULE) << "force output of messages" << endl;

	OTC_Event* theIOEvent;
	theIOEvent = new OTCEV_IOEvent(fd_,OTCLIB_POLLOUT);
	theIOEvent->deliver(this);
      }
      else
      {
	OTCLIB_TRACER(MODULE) << "queue message" << endl;
      }
    }
  }
  else
  {
    // Unexpected event, destroy it.

    theEvent->destroy();
  }
}

/* ------------------------------------------------------------------------- */
OTCEV_Envelope* OTC_EPISMessage::unpack_()
{
  int const SZINT = sizeof(u_int);

  // Make sure message was at least long
  // enough to hold magic header and the
  // length field.

  if (imessage_.length() < (sizeof(gMagic_) + SZINT))
    return 0;

  // Verify length field.

  u_int theValue;
  u_int theLength;

  memcpy((char*)&theValue,imessage_.buffer()+sizeof(gMagic_),SZINT);
  theLength = ntohl(theValue);

  if (theLength != imessage_.length()-sizeof(gMagic_)-SZINT)
    return 0;

  // Extract fields.

  u_int theProtocol;

  OTC_String theToAddress;
  OTC_String theFromAddress;
  OTC_String theSubject;
  OTC_String theHeader;

  OTC_String theConversationId;
  int theSequenceNumber = 0;

  OTC_String theContentType;
  OTC_String theEventType;

  u_int theOffset = sizeof(gMagic_) + SZINT;

  OTC_String theBuffer;

  // The protocol.

  memcpy((char*)&theValue,imessage_.buffer()+theOffset,SZINT);
  theProtocol = ntohl(theValue);

  if (theProtocol != 0)
    return 0;

  theOffset += SZINT;

  // The destination.

  theToAddress.assign(imessage_.buffer() + theOffset);
  theOffset += theToAddress.length() + 1;

  if (theOffset >= imessage_.length())
    return 0;

  // The source.

  theFromAddress.assign(imessage_.buffer() + theOffset);
  theOffset += theFromAddress.length() + 1;

  if (theOffset >= imessage_.length())
    return 0;

  // The subject.

  theSubject = imessage_.buffer() + theOffset;
  theOffset += theSubject.length() + 1;

  // The header.

  theHeader = imessage_.buffer() + theOffset;
  theOffset += theHeader.length() + 1;

  if (theOffset >= imessage_.length())
    return 0;

  // The conversation id.

  theConversationId = imessage_.buffer() + theOffset;
  theOffset += theConversationId.length() + 1;

  if (theOffset >= imessage_.length())
    return 0;

  // The sequence number.

  theBuffer = imessage_.buffer() + theOffset;
  theOffset += theBuffer.length() + 1;

  sscanf(theBuffer.buffer(),"%d",&theSequenceNumber);

  if (theOffset >= imessage_.length())
    return 0;

  // The content type.

  theContentType = imessage_.buffer() + theOffset;
  theOffset += theContentType.length() + 1;

  if (theOffset >= imessage_.length())
    return 0;

  // The event type.

  theEventType = imessage_.buffer() + theOffset;
  theOffset += theEventType.length() + 1;

  if (theOffset > imessage_.length())
    return 0;

  // Now construct message and envelope.

  imessage_.remove(0,theOffset);

  OTCEV_Message* theEvent;
  theEvent = new OTCEV_Message(imessage_,theContentType,theEventType);
  OTCLIB_ASSERT_M(theEvent != 0);

  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(theEvent,theToAddress,theFromAddress,
   theSubject,theHeader,theConversationId,theSequenceNumber);
  OTCLIB_ASSERT_M(theEnvelope != 0);

  return theEnvelope;
}

/* ------------------------------------------------------------------------- */
void OTC_EPISMessage::pack_(OTCEV_Envelope* theEnvelope)
{
  omessage_.length(0);

  OTCEV_Message* theMessage;
  theMessage = theEnvelope->message();

  if (theMessage == 0)
    return;

  char theBuffer[32];

  int const SZINT = sizeof(u_int);

  // Adjust destination and return address
  // if required.

  OTC_String theToAddress;
  OTC_String theFromAddress;

  OTC_String theInBoxName;

  if (!theEnvelope->to().isEmpty())
  {
    theInBoxName = theEnvelope->currentReceiver();

    if (theInBoxName == remoteAddress())
    {
      theToAddress = theEnvelope->nextDestination();

      if (!theEnvelope->from().isEmpty())
	theFromAddress = localAddress() + "!" + theEnvelope->from();
    }
    else
    {
      theToAddress = theEnvelope->to();
      if (!theEnvelope->from().isEmpty())
	theFromAddress = localAddress() + "!" + theEnvelope->from();
    }
  }
  else
  {
    theFromAddress = theEnvelope->from();
  }

  // Add in magic 4 byte header header as a
  // sanity check.

  omessage_.append(gMagic_,sizeof(gMagic_));

  // Leave space for message length.
  // Note that 32 bit systems say you use
  // u_long however on 64 bit systems you use
  // u_int. Assume that we never have to deal
  // with a 16 bit system and always use u_int
  // which is 32 bits on both 32 and 64 bit
  // systems which have so far been
  // encountered.

  omessage_.append(EOS,SZINT);

  // Leave space for protocol field. We reserve
  // all zeros for the protocol field. Other
  // values should be used for the protocol
  // field for different header formats.

  omessage_.append(EOS,SZINT);

  // Append destination address.

  omessage_.append(theToAddress);
  omessage_.append(EOS);

  // Append return address.

  omessage_.append(theFromAddress);
  omessage_.append(EOS);

  // Append subject.

  omessage_.append(theEnvelope->subject());
  omessage_.append(EOS);

  // Append header.

  omessage_.append(theEnvelope->header());
  omessage_.append(EOS);

  // Append conversation id.

  omessage_.append(theEnvelope->conversationId());
  omessage_.append(EOS);

  // Append sequence number.

  sprintf(theBuffer,"%d",theEnvelope->sequenceNumber());
  omessage_.append(theBuffer);
  omessage_.append(EOS);

  // Append content type.

  omessage_.append(theMessage->contentType());
  omessage_.append(EOS);

  // Append event type.

  omessage_.append(theMessage->eventType());
  omessage_.append(EOS);

  // Set message length field.

  u_int theHLength;
  u_int theNLength;

  theHLength =
   omessage_.length() + theMessage->contentLength() - sizeof(gMagic_) - SZINT;
  theNLength = htonl(theHLength);

  omessage_.replace(sizeof(gMagic_),SZINT,(char*)&theNLength,SZINT);

  omessage_.append(theMessage->content(),theMessage->contentLength());
}

/* ------------------------------------------------------------------------- */
void OTC_EPISMessage::messageQueued_()
{
  if (backlogCount_ == 0)
  {
    // No previous backlog.

    if (outgoingCount_ == gThreshold_)
    {
      // Have reached backlog threshold
      // for the first time this time
      // round.

      OTCEV_Congestion* theCongestion;
      theCongestion = new OTCEV_Congestion(localAddress(),
       remoteAddress(),OTCLIB_CONNECTION_BACKLOG,outgoingCount_);
      OTCLIB_ASSERT_M(theCongestion != 0);

      endPoint()->notifyObservers(theCongestion);

      backlogCount_ = outgoingCount_;
    }
  }
  else
  {
    // Already in backlog mode.

    backlogCount_++;

    if ((backlogCount_ % gThreshold_) == 0)
    {
      // Reached next threshold multiple
      // based on messages sent and received.

      if (outgoingCount_ >= gThreshold_)
      {
	OTCEV_Congestion* theCongestion;
	theCongestion = new OTCEV_Congestion(localAddress(),
	 remoteAddress(),OTCLIB_CONNECTION_BACKLOG,outgoingCount_);
	OTCLIB_ASSERT_M(theCongestion != 0);

	endPoint()->notifyObservers(theCongestion);
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_EPISMessage::messageSent_()
{
  if (backlogCount_ != 0)
  {
    // Already in backlog mode.

    backlogCount_++;

    if (outgoingCount_ == 0)
    {
      // Have cleared all the backlog.

      OTCEV_Congestion* theCongestion;
      theCongestion = new OTCEV_Congestion(localAddress(),
       remoteAddress(),OTCLIB_CONNECTION_BACKLOG,outgoingCount_);
      OTCLIB_ASSERT_M(theCongestion != 0);

      endPoint()->notifyObservers(theCongestion);

      backlogCount_ = 0;
    }
    else if ((backlogCount_ % gThreshold_) == 0)
    {
      // Reached next threshold multiple
      // based on messages sent and received.

      if (outgoingCount_ >= gThreshold_)
      {
	OTCEV_Congestion* theCongestion;
	theCongestion = new OTCEV_Congestion(localAddress(),
	 remoteAddress(),OTCLIB_CONNECTION_BACKLOG,outgoingCount_);
	OTCLIB_ASSERT_M(theCongestion != 0);

	endPoint()->notifyObservers(theCongestion);
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
