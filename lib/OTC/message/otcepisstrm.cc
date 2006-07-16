/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     message/otcepisstrm.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/episstrm.hh"
#endif
#endif

#include <OTC/message/episstrm.hh>

#include <OTC/message/congest.hh>
#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/ioevent.hh>
#include <OTC/dispatch/envelope.hh>
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
#include <OTC/system/program.hh>

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

#if !defined(EWOULDBLOCK)
#if defined(WSAEWOULDBLOCK)
#define EWOULDBLOCK WSAEWOULDBLOCK
#else
#define EWOULDBLOCK EAGAIN
#endif
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
    TAG = new OTC_TraceTag("OTC_EPISStream");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_EPISStream::mutex_;
u_int OTC_EPISStream::gBatch_ = 0;
u_int OTC_EPISStream::gSize_ = 0;
u_int OTC_EPISStream::gThreshold_ = 0;

/* ------------------------------------------------------------------------- */
OTC_EPISStream::~OTC_EPISStream()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISStream::~OTC_EPISStream()");

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
OTC_EPISStream::OTC_EPISStream(OTC_EndPoint* theEndPoint, u_int theSize)
  : OTC_EPISocket(theEndPoint),
    state_(OTCLIB_EPISSTREAM_INACTIVE),
    fd_(INVALID_SOCKET),
    nonblocking_(OTCLIB_FALSE),
    outgoingCount_(0),
    backlogCount_(0),
    waitingToWrite_(OTCLIB_FALSE),
    ilength_(theSize),
    olength_(0),
    suspended_(OTCLIB_FALSE),
    stopping_(OTCLIB_FALSE)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISStream::OTC_EPISStream()");

  mutex_.lock();

  if (gBatch_ == 0)
  {
    char* var;
    var = getenv("OTCLIB_STREAMBATCH");
    if (var != 0)
    {
      int i = atoi(var);
      if (i > 0)
        gBatch_ = i;
    }

    if (gBatch_ == 0)
      gBatch_ = 32;
  }

  if (gSize_ == 0)
  {
    char* var;
    var = getenv("OTCLIB_STREAMINPUT");
    if (var != 0)
    {
      int i = atoi(var);
      if (i > 0)
        gSize_ = i;
    }

    if (gSize_ == 0)
      gSize_ = 4096;
  }


  if (gThreshold_ == 0)
  {
    char* var;
    var = getenv("OTCLIB_STREAMTHRESHOLD");
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

  if (ilength_ == 0)
    ilength_ = gSize_;

  // Ignore instances of SIGPIPE.

#if defined(SIGPIPE)
  OTC_SignalHandler::install(SIGPIPE,(void (*)(int))SIG_IGN);
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_EPISStream::attach(int theFd)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISStream::attach(int)");

  OTCLIB_TRACER(MODULE) << "fd = " << theFd << endl;

  OTCLIB_ENSURE_FN((state_ == OTCLIB_EPISSTREAM_INACTIVE),
   "OTC_EPISStream::attach(int)",
   "active connection already exists");

  OTCLIB_ENSURE_FN((theFd >= 0),
   "OTC_EPISStream::attach(int)",
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

  state_ = OTCLIB_EPISSTREAM_STARTING;

  OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;

  // Send an action to ourselves so we send out
  // message to set remote address for other end
  // of the connection.

  OTCEV_Action::schedule(id(),OTCLIB_PRIORITY_JOB,"START");
}

/* ------------------------------------------------------------------------- */
void OTC_EPISStream::suspendInput()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISStream::suspendInput()");

  if (fd_ == INVALID_SOCKET)
    return;

  if (suspended_ != OTCLIB_FALSE)
    return;

  suspended_ = OTCLIB_TRUE;

  if (state_ == OTCLIB_EPISSTREAM_CONNECTED)
    OTCEV_IOEvent::unsubscribe(id(),fd_,OTCLIB_POLLIN);
}

/* ------------------------------------------------------------------------- */
void OTC_EPISStream::resumeInput()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISStream::resumeInput()");

  if (fd_ == INVALID_SOCKET)
    return;

  if (suspended_ == OTCLIB_FALSE)
    return;

  suspended_ = OTCLIB_FALSE;

  if (state_ == OTCLIB_EPISSTREAM_CONNECTED)
    OTCEV_IOEvent::subscribe(id(),fd_,OTCLIB_POLLIN);
}

/* ------------------------------------------------------------------------- */
int OTC_EPISStream::detach()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISStream::detach()");

  OTCLIB_TRACER(MODULE) << "fd = " << fd_ << endl;

  OTCLIB_ENSURE_FN((fd_ != INVALID_SOCKET),
   "OTC_EPISStream::detach()",
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

  // Go into sleep state until given new file
  // descriptor for connection. It is assumed
  // that the caller closed the file
  // descriptor.

  suspended_ = OTCLIB_FALSE;
  stopping_ = OTCLIB_FALSE;

  state_ = OTCLIB_EPISSTREAM_INACTIVE;

  OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;

  // Wipe out remote address.

  remote_.truncate();

  unbindRemoteAddress();

  return theOldFd;
}

/* ------------------------------------------------------------------------- */
void OTC_EPISStream::handle(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISStream::handle(OTC_Event*)");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << "event = " << *theEvent << endl;

  if (theEvent->type() == OTCEV_Action::typeId())
  {
    OTCEV_Action* theAction;
    theAction = (OTCEV_Action*)theEvent;

    if (theAction->description() == "START")
    {
      // Clear any old messages from prior
      // connection.

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

      outgoingCount_ = 0;
      backlogCount_ = 0;

      // Move to connected state.

      state_ = OTCLIB_EPISSTREAM_CONNECTED;

      // Set the remote address. Use a random value.
      // User has to be subscribe as observer to
      // know when connection becomes available
      // and thus now what this address is.

      bindRemoteAddress(OTC_Program::uniqueId(
       "$EP-PROXY?",OTCLIB_ID_SHORT_FORMAT));

      // Subscribe to file descriptor so we know
      // when we receive address setup message from
      // client.

      OTCEV_IOEvent::subscribe(id(),fd_,OTCLIB_POLLIN);
    }
    else if (theAction->description() == "CLOSE")
    {
      if (state_ != OTCLIB_EPISSTREAM_INACTIVE)
      {
	if (outgoingCount_ == 0)
	{
          state_ = OTCLIB_EPISSTREAM_INACTIVE;

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

            state_ = OTCLIB_EPISSTREAM_STARTING;

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

    if (state_ != OTCLIB_EPISSTREAM_INACTIVE &&
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
	      theLogger << "OTC_EPISStream::handle() - ";
	      theLogger << "cannot route event" << EOL;
	      theLogger << "Event " << *theEnvelope << EOL;
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
	    theLogger << "OTC_EPISStream::handle() - ";
	    theLogger << "cannot deflate event" << EOL;
	    theLogger << "Event " << *theEnvelope << EOL;
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

	  omessage_ = theEnvelope->message()->content();

	  olength_ = omessage_.length();
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
	// message contents, shut the connection down,
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

	  state_ = OTCLIB_EPISSTREAM_INACTIVE;

	  OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;
	}
      }

      if (outgoingCount_ == 0 && stopping_ != OTCLIB_FALSE)
      {
	state_ = OTCLIB_EPISSTREAM_INACTIVE;

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

	  state_ = OTCLIB_EPISSTREAM_STARTING;

	  OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;

	  OTCEV_Action::schedule(id(),OTCLIB_PRIORITY_JOB,"START");
	}
      }
    }

    if (state_ != OTCLIB_EPISSTREAM_INACTIVE &&
     (theIOEvent->events() & OTCLIB_POLLIN))
    {
      imessage_.length(ilength_);

      int theResult = 0;

      // Read in what we can up to limit set
      // by input buffer size.

      theResult = SOCKET_ERROR;

      while (theResult == SOCKET_ERROR)
      {
	theResult = recv(fd_,imessage_.buffer(),imessage_.length(),0);

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
      {
	imessage_.length(theResult);

	OTCLIB_TRACER(MODULE)
	 << "length of data read = " << theResult << endl;

	// Construct the envelope and message and
	// work out what we should do with it.

	OTCEV_Message* theMessage;
	theMessage = new OTCEV_Message(
	 OTC_CString(imessage_.buffer(),imessage_.length()));
	OTCLIB_ASSERT_M(theMessage != 0);

	OTCEV_Envelope* theEnvelope;
	theEnvelope = new OTCEV_Envelope(
	 theMessage,localAddress(),remoteAddress());
	OTCLIB_ASSERT_M(theEnvelope != 0);

	// endPoint()->notifyObservers(theEnvelope);

	theEnvelope->queue(OTCLIB_STANDARD_JOB);
      }
      else if (theResult == SOCKET_ERROR)
      {
	state_ = OTCLIB_EPISSTREAM_INACTIVE;

	OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;
      }
    }

    // Now check for error conditions.

    if (state_ != OTCLIB_EPISSTREAM_INACTIVE &&
     theIOEvent->events() & OTCLIB_POLLERR)
    {
      // Do not really now if I expect to get this
      // sort of error message back. Take it as being
      // an I/O error and close connection down.

      theError = EIO;

      state_ = OTCLIB_EPISSTREAM_INACTIVE;

      OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;
    }

    if (state_ != OTCLIB_EPISSTREAM_INACTIVE &&
     theIOEvent->events() & OTCLIB_POLLNVAL)
    {
      // An invalid file descriptor.

      theError = EBADF;

      state_ = OTCLIB_EPISSTREAM_INACTIVE;

      OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;
    }

    if (state_ != OTCLIB_EPISSTREAM_INACTIVE &&
     theIOEvent->events() & OTCLIB_POLLHUP)
    {
      // Received hangup for connection. Most likely
      // had a POLLIN at the same time which has already
      // realised that connection has closed, however
      // still check here just in case.

      theError = 0;

      state_ = OTCLIB_EPISSTREAM_INACTIVE;

      OTCLIB_TRACER(MODULE) << "state = " << (int)state_ << endl;
    }

    if (state_ == OTCLIB_EPISSTREAM_INACTIVE)
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

	  state_ = OTCLIB_EPISSTREAM_STARTING;

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

    if (state_ == OTCLIB_EPISSTREAM_CONNECTED)
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
void OTC_EPISStream::messageQueued_()
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
void OTC_EPISStream::messageSent_()
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