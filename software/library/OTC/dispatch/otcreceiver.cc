/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcreceiver.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2006 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/receiver.hh"
#endif
#endif

#include <OTC/dispatch/receiver.hh>
#include <OTC/dispatch/event.hh>
#include <OTC/dispatch/eventjob.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/system/program.hh>
#include <OTC/text/symbol.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_Receiver::mutex_;
OTC_NRMutex OTC_Receiver::receiverMutex_;
bool OTC_Receiver::gIndexInitialised_ = false;
OTC_HIndex<char const*>* OTC_Receiver::gIndex_ = 0;
bool OTC_Receiver::gReceiversIntialised_ = false;
OTC_Receiver* OTC_Receiver::gLocalReceiverInBox_;
OTC_Receiver* OTC_Receiver::gReceiverInBox_;
OTC_Receiver* OTC_Receiver::gAgentInBox_;
OTC_Receiver* OTC_Receiver::gRelayInBox_;

/* ------------------------------------------------------------------------- */
struct OSE_EXPORT OTC_ReceiverTarget
{
  public:

                        OTC_ReceiverTarget()
                          : callback(0), agentId(0), next(0) {}

    void                (*callback)(OTC_Event*);
    int                 agentId;
    OTC_ReceiverTarget* next;
};

/* ------------------------------------------------------------------------- */
class OSE_EXPORT OTC_ReceiverJob : public OTC_Job
{
  public:

			~OTC_ReceiverJob();

			OTC_ReceiverJob(
                         char const* theName,
                         OTC_Event* theEvent
                        )
			  : name_(theName), event_(theEvent) {}

    void		execute();

  private:

    OTC_String          name_;

    OTC_Event*          event_;
};

OTC_ReceiverJob::~OTC_ReceiverJob()
{
  if (event_ != 0)
    event_->destroy();
}

void OTC_ReceiverJob::execute()
{
  if (event_ != 0)
    OTC_Receiver::deliver(name_,event_);

  event_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_Receiver::~OTC_Receiver()
{
  unbind();
}

/* ------------------------------------------------------------------------- */
OTC_Receiver::OTC_Receiver()
  : agentId_(0), callback_(0), prev_(0), next_(0), count_(1)
{
  if (!gIndexInitialised_)
    initIndex_();
}

/* ------------------------------------------------------------------------- */
OTC_Receiver::OTC_Receiver(OTC_EVAgent* theAgent, char const* theName)
  : agentId_(0), callback_(0), prev_(0), next_(0), count_(1)
{
  if (!gIndexInitialised_)
    initIndex_();

  bind(theAgent,theName);
}

/* ------------------------------------------------------------------------- */
OTC_Receiver::OTC_Receiver(OTC_EVAgent* theAgent)
  : agentId_(0), callback_(0), prev_(0), next_(0), count_(1)
{
  if (!gIndexInitialised_)
    initIndex_();

  bind(theAgent);
}

/* ------------------------------------------------------------------------- */
OTC_Receiver::OTC_Receiver(void (*theCallback)(OTC_Event*), char const* theName)
  : agentId_(0), callback_(0), prev_(0), next_(0), count_(1)
{
  if (!gIndexInitialised_)
    initIndex_();

  bind(theCallback,theName);
}

/* ------------------------------------------------------------------------- */
OTC_Receiver::OTC_Receiver(void (*theCallback)(OTC_Event*))
  : agentId_(0), callback_(0), prev_(0), next_(0), count_(1)
{
  if (!gIndexInitialised_)
    initIndex_();

  bind(theCallback);
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::unbind()
{
  mutex_.lock();

  if (agentId_ != 0 || callback_ != 0)
  {
    // Unlink ourselves from the chain. If we are
    // first in the list we need to update entry
    // in named index or if we are also the last
    // in the chain delete the entry from the
    // index.

    if (prev_ == 0)
    {
      OTC_HIndexEntry<char const*>* theNode;
      theNode = gIndex_->entry(name_);
      OTCLIB_ASSERT(theNode != 0);

      if (next_ != 0)
      {
        next_->count_ = count_ - 1;
	theNode->setItem((void*)next_);
      }
      else
	theNode->removeEntry();
    }

    if (prev_ != 0)
      prev_->next_ = next_;
    if (next_ != 0)
      next_->prev_ = prev_;

    agentId_ = 0;
    callback_ = 0;
    name_ = "";
    prev_ = 0;
    next_ = 0;
  }

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::bind(OTC_EVAgent* theAgent, char const* theName)
{
  OTCLIB_ENSURE_FN((theAgent != 0 && theName != 0 && *theName != EOS),
   "OTC_Receiver::bind(OTC_EVAgent*, char const*)",
   "invalid agent/name");

  unbind();

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Symbol tmpName;
  tmpName = theName;

  agentId_ = theAgent->id();
  callback_ = 0;
  name_ = tmpName;

  OTC_HIndexEntry<char const*>* theNode;
  theNode = gIndex_->insert(name_);

  next_ = (OTC_Receiver*)theNode->item();
  if (next_ != 0)
  {
    next_->prev_ = this;
    count_ = next_->count_ + 1;
  }

  theNode->setItem((void*)this);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::bind(OTC_EVAgent* theAgent)
{
  OTCLIB_ENSURE_FN((theAgent != 0),
   "OTC_Receiver::bind(OTC_EVAgent*)",
   "invalid agent");

  unbind();

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  // Have a while loop here just in case
  // the ID we use is in use already, in
  // which case we try with another one
  // until we find one that isn't in use.

  while (1)
  {
    name_ = OTC_Symbol(OTC_Program::uniqueId("$O?",OTCLIB_ID_SHORT_FORMAT));

    agentId_ = theAgent->id();
    callback_ = 0;

    OTC_HIndexEntry<char const*>* theNode;
    theNode = gIndex_->insert(name_);

    next_ = (OTC_Receiver*)theNode->item();

    if (next_ == 0)
    {
      theNode->setItem((void*)this);

      break;
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::bind(void (*theCallback)(OTC_Event*), char const* theName)
{
  OTCLIB_ENSURE_FN((theCallback != 0 && theName != 0 && *theName != EOS),
   "OTC_Receiver::bind(void (*)(OTC_Event*), char const*)",
   "invalid callback/name");

  unbind();

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Symbol tmpName;
  tmpName = theName;

  agentId_ = 0;
  callback_ = theCallback;
  name_ = tmpName;

  OTC_HIndexEntry<char const*>* theNode;
  theNode = gIndex_->insert(name_);

  next_ = (OTC_Receiver*)theNode->item();
  if (next_ != 0)
  {
    next_->prev_ = this;
    count_ = next_->count_ + 1;
  }

  theNode->setItem((void*)this);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::bind(void (*theCallback)(OTC_Event*))
{
  OTCLIB_ENSURE_FN((theCallback != 0),
   "OTC_Receiver::bind(void (*)(OTC_Event*))",
   "invalid callback");

  unbind();

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  while (1)
  {
    OTC_String theName;
    theName = OTC_Program::uniqueId("$O?",OTCLIB_ID_SHORT_FORMAT);

    OTC_Symbol tmpName;
    tmpName = theName;

    agentId_ = 0;
    callback_ = theCallback;
    name_ = tmpName;

    OTC_HIndexEntry<char const*>* theNode;
    theNode = gIndex_->insert(name_);

    next_ = (OTC_Receiver*)theNode->item();

    if (next_ == 0)
    {
      theNode->setItem((void*)this);

      break;
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::deliver(char const* theName, OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theName == 0 || *theName == EOS)
  {
    theEvent->destroy();

    return;
  }

  u_int theCount;
  OTC_ReceiverTarget theFirstRecipient;

  theCount = retrieveTargets_(&theFirstRecipient,theName);

  if (theCount == 1)
  {
    if (theFirstRecipient.callback != 0)
      theEvent->deliver(theFirstRecipient.callback);
    else
      theEvent->deliver(theFirstRecipient.agentId);
  }
  else if (theCount > 1)
  {
    if (theFirstRecipient.callback != 0)
      theEvent->clone()->deliver(theFirstRecipient.callback);
    else
      theEvent->clone()->deliver(theFirstRecipient.agentId);

    OTC_ReceiverTarget* theNextRecipient;
    OTC_ReceiverTarget* theLastRecipient;

    theNextRecipient = theFirstRecipient.next;

    while (theNextRecipient != 0)
    {
      if (theNextRecipient->callback != 0)
        theEvent->clone()->deliver(theNextRecipient->callback);
      else
        theEvent->clone()->deliver(theNextRecipient->agentId);

      theLastRecipient = theNextRecipient;
      theNextRecipient = theNextRecipient->next;
      delete theLastRecipient;
    }

    theEvent->destroy();
  }
  else
    theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::queue(char const* theName, OTC_Event* theEvent, int theType)
{
  if (theEvent == 0)
    return;

  if (theName == 0 || *theName == EOS)
  {
    theEvent->destroy();

    return;
  }

  OTC_ReceiverJob* theJob;
  theJob = new OTC_ReceiverJob(theName,theEvent);

  OTC_Dispatcher::schedule(theJob,theType);
}

/* ------------------------------------------------------------------------- */
u_int OTC_Receiver::retrieveTargets_(
 OTC_ReceiverTarget* theRecipients,
 char const* theName
)
{
  if (!gReceiversIntialised_)
    initReceivers_();

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Receiver const* theReceiver = 0;
  u_int theCount = 0;

  if (gIndex_ != 0)
  {
    OTC_HIndexEntry<char const*>* theNode;
    theNode = gIndex_->entry(theName);

    if (theNode != 0)
    {
      theReceiver = (OTC_Receiver*)theNode->item();
      theCount = theReceiver->count_;
    }
  }

  if (theReceiver != 0)
  {
    // The first target object should be on the
    // parent stack so just populate it.
    // Additional target objects need to be
    // created on the free store.

    theRecipients->agentId = theReceiver->agentId_;
    theRecipients->callback = theReceiver->callback_;

    // Only create additional target objects if
    // more receivers. The additional target objects
    // are created on the heap and must be deleted
    // by the caller.

    if (theCount > 1)
    {
      OTC_Receiver* theNextReceiver = theReceiver->next_;
      OTC_ReceiverTarget* theRecipient = theRecipients;

      while (theNextReceiver != 0)
      {
        theRecipient->next = new OTC_ReceiverTarget;
        OTCLIB_ASSERT_M(theRecipient->next != 0);

        theRecipient = theRecipient->next;

        theRecipient->agentId = theReceiver->agentId_;
        theRecipient->callback = theReceiver->callback_;

        theNextReceiver = theNextReceiver->next_;
      }
    }
  }

  xxxMutex.release();

  mutex_.unlock();

  return theCount;
}

/* ------------------------------------------------------------------------- */
OTC_Receiver const* OTC_Receiver::lookup(char const* theName)
{
  if (theName == 0 || *theName == EOS)
    return 0;

  if (!gReceiversIntialised_)
    initReceivers_();

  OTC_Receiver* theResult;
  theResult = 0;

  mutex_.lock();

  if (gIndex_ != 0)
  {
    OTC_HIndexEntry<char const*>* theNode;
    theNode = gIndex_->entry(theName);

    if (theNode != 0)
      theResult = (OTC_Receiver*)theNode->item();
  }

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::initIndex_()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gIndex_ == 0)
  {
    gIndex_ = new OTC_HIndex<char const*>;
    OTCLIB_ASSERT_M(gIndex_ != 0);
  }

  gIndexInitialised_ = true;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::initReceivers_()
{
  receiverMutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(receiverMutex_);

  if (gLocalReceiverInBox_ == 0)
  {
    gLocalReceiverInBox_ = new OTC_Receiver;
    OTCLIB_ASSERT_M(gLocalReceiverInBox_ != 0);

    gLocalReceiverInBox_->bind(builtinReceiver_,"$LOCAL-RECEIVER");
  }

  if (gReceiverInBox_ == 0)
  {
    gReceiverInBox_ = new OTC_Receiver;
    OTCLIB_ASSERT_M(gReceiverInBox_ != 0);

    gReceiverInBox_->bind(builtinReceiver_,"$RECEIVER");
  }

  if (gAgentInBox_ == 0)
  {
    gAgentInBox_ = new OTC_Receiver;
    OTCLIB_ASSERT_M(gAgentInBox_ != 0);

    gAgentInBox_->bind(builtinReceiver_,"$AGENT");
  }

  if (gRelayInBox_ == 0)
  {
    gRelayInBox_ = new OTC_Receiver;
    OTCLIB_ASSERT_M(gRelayInBox_ != 0);

    gRelayInBox_->bind(builtinReceiver_,"$RELAY");
  }

  gReceiversIntialised_ = true;

  xxxMutex.release();

  receiverMutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::builtinReceiver_(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCEV_Envelope* theEnvelope;
    theEnvelope = (OTCEV_Envelope*)theEvent;

    if (theEnvelope->currentReceiver() == "$RELAY")
    {
      theEnvelope->forward(OTCLIB_STANDARD_JOB);
    }
    else if (theEnvelope->currentReceiver() == "$RECEIVER" ||
     theEnvelope->currentReceiver() == "$LOCAL-RECEIVER")
    {
      OTCEV_Envelope* tmpEnvelope;
      tmpEnvelope = new OTCEV_Envelope(theEnvelope,
       theEnvelope->nextDestination(),theEnvelope->from());
      OTCLIB_ASSERT_M(tmpEnvelope);

      tmpEnvelope->deliver();
    }
    else if (theEnvelope->currentReceiver() == "$AGENT")
    {
      int theAgentId = 0;

      theAgentId = atoi(theEnvelope->nextDestination());

      if (theAgentId != 0)
	theEvent->clone()->deliver(theAgentId);
    }
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
