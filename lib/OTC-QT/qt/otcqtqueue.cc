/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     qt/otcqtqueue.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/qt/qtqueue.hh"
#endif
#endif

#include <OTC/qt/qtqueue.hh>

#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/alarm.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/ioevent.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/thread/mxreaper.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/debug/tracer.hh>

#include <qtimer.h>
#include <qsocketnotifier.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */
#ifdef OTCLIB_TRACE
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_QtJobQueue");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()
#endif

/* ------------------------------------------------------------------------- */

class OTC_QtDummyJob : public OTC_Job
{
  public:

                        OTC_QtDummyJob() {}

                        ~OTC_QtDummyJob();

  protected:

    void                execute();

    void                destroy();
};

/* ------------------------------------------------------------------------- */
OTC_QtDummyJob::~OTC_QtDummyJob()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_QtDummyJob::execute()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_QtDummyJob::destroy()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_QtJobQueue* OTC_QtJobQueue::instance_ = 0;

/* ------------------------------------------------------------------------- */
OTC_QtJobQueue::~OTC_QtJobQueue()
{
  // This should never get called anyway.

  if (signalNotifier_ != 0)
    delete signalNotifier_;
  if (wakeupNotifier_ != 0)
    delete wakeupNotifier_;

  instance_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_QtJobQueue::OTC_QtJobQueue(QApplication* theApplication)
  : application_(theApplication),
    signalNotifier_(0),
    wakeupNotifier_(0)
{
  OTCLIB_ENSURE_FN((instance_ == 0),
   "OTC_QtJobQueue::OTC_QtJobQueue()",
   "instance of Qt job queue already exists");
  OTCLIB_ENSURE_FN((theApplication != 0),
   "OTC_QtJobQueue::OTC_QtJobQueue()",
   "invalid Qt application object");

  instance_ = this;

  // Register file descriptor for watching of signals.

  if (signalPipe() != -1)
  {
    signalNotifier_ = new QSocketNotifier(
     signalPipe(),QSocketNotifier::Read,0);
    OTCLIB_ASSERT_M(signalNotifier_ != 0);

    QObject::connect(signalNotifier_,SIGNAL(activated(int)),
     this,SLOT(handleSignal()));
  }

  if (wakeupPipe() != -1)
  {
    wakeupNotifier_ = new QSocketNotifier(
     wakeupPipe(),QSocketNotifier::Read,0);
    OTCLIB_ASSERT_M(wakeupNotifier_ != 0);

    QObject::connect(wakeupNotifier_,SIGNAL(activated(int)),
     this,SLOT(handleWakeup()));
  }

  // Register interest in application quiting.

#if QT_VERSION >= 200
  QObject::connect(application_,SIGNAL(aboutToQuit()),
   this,SLOT(handleQuit()));
#endif

  // Cause registration of current event
  // sources.

  wakeup(0);
}

/* ------------------------------------------------------------------------- */
OTC_Job* OTC_QtJobQueue::next(int, int)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_QtJobQueue::next()");

  // Call Qt to dispatch one event.

  application_->processOneEvent();

  // Check to see if the dispatcher has been
  // stopped. This may have occurred from
  // some function not called as a result of
  // a job in the dispatcher but some other
  // code module bound into the application
  // or another thread.

  if (isStopped())
  {
    shutdown();

    return 0;
  }

  // Return a dummy job.

  OTC_Job* theJob;

  theJob = new OTC_QtDummyJob;
  OTCLIB_ASSERT_M(theJob != 0);

  return theJob;
}

/* ------------------------------------------------------------------------- */
void OTC_QtJobQueue::shutdown()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_QtJobQueue::shutdown()");

  application_->exit_loop();
}

/* ------------------------------------------------------------------------- */
void OTC_QtJobQueue::wakeup(int theType)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_QtJobQueue::wakeup()");

  OTCLIB_TRACER(MODULE) << "type = " << theType << endl;

  // Call base class wakeup() function.

  OTC_JobQueue::wakeup(theType);

  // Check for signal wakeup. Need to return
  // straight away as don't want to do anything
  // in signal handler.

  if (theType & OTCLIB_WAKEUP_SIGNAL)
    return;

  // Register file descriptor events.

  if (theType & OTCLIB_WAKEUP_IOEVENT)
  {
    int theEvents;

    OTC_PairIterator<int,QObject*> theSockets;

    theSockets = fileNotifiersRead_.pairs();
    while (theSockets.isValid())
    {
      theEvents = OTCEV_IOEvent::events(theSockets.key());

      if ((theEvents & OTCLIB_POLLIN) == 0)
      {
	delete theSockets.item();
	fileNotifiersRead_.remove(theSockets.key());
      }

      theSockets.next();
    }

    theSockets = fileNotifiersWrite_.pairs();
    while (theSockets.isValid())
    {
      theEvents = OTCEV_IOEvent::events(theSockets.key());

      if ((theEvents & OTCLIB_POLLOUT) == 0)
      {
	delete theSockets.item();
	fileNotifiersWrite_.remove(theSockets.key());
      }

      theSockets.next();
    }

    theSockets = fileNotifiersException_.pairs();
    while (theSockets.isValid())
    {
      theEvents = OTCEV_IOEvent::events(theSockets.key());

      if ((theEvents & OTCLIB_POLLPRI) == 0)
      {
	delete theSockets.item();
	fileNotifiersException_.remove(theSockets.key());
      }

      theSockets.next();
    }

    for (int i=0; i<=OTCEV_IOEvent::maxFd(); i++)
    {
      theEvents = OTCEV_IOEvent::events(i);

      if ((theEvents & OTCLIB_POLLIN) != 0)
      {
	if (!fileNotifiersRead_.contains(i))
	{
	  QSocketNotifier* notifier;
	  notifier = new QSocketNotifier(i,QSocketNotifier::Read,0);
	  OTCLIB_ASSERT_M(notifier);

	  QObject::connect(notifier,SIGNAL(activated(int)),
	   this,SLOT(handleSocketRead(int)));

	  fileNotifiersRead_.add(i,notifier);
	}
      }

      if ((theEvents & OTCLIB_POLLOUT) != 0)
      {
	if (!fileNotifiersWrite_.contains(i))
	{
	  QSocketNotifier* notifier;
	  notifier = new QSocketNotifier(i,QSocketNotifier::Write,0);
	  OTCLIB_ASSERT_M(notifier);

	  QObject::connect(notifier,SIGNAL(activated(int)),
	   this,SLOT(handleSocketWrite(int)));

	  fileNotifiersWrite_.add(i,notifier);
	}
      }

      if ((theEvents & OTCLIB_POLLPRI) != 0)
      {
	if (!fileNotifiersException_.contains(i))
	{
	  QSocketNotifier* notifier;
	  notifier = new QSocketNotifier(i,QSocketNotifier::Exception,0);
	  OTCLIB_ASSERT_M(notifier);

	  QObject::connect(notifier,SIGNAL(activated(int)),
	   this,SLOT(handleSocketException(int)));

	  fileNotifiersException_.add(i,notifier);
	}
      }
    }
  }

  // Set maximum time to block. This
  // is derived from pending timeouts
  // and alarms.

  if (theType & OTCLIB_WAKEUP_ALARM || theType & OTCLIB_WAKEUP_TIMEOUT)
  {
    long alarmPeriod = -1;
    long timeoutPeriod = -1;
    long period = -1;

    alarmPeriod = 1000 * OTCEV_Alarm::period();
    timeoutPeriod = OTCEV_Timeout::period();

    if (alarmPeriod < 0 && timeoutPeriod >= 0)
      period = timeoutPeriod;
    else if (timeoutPeriod < 0 && alarmPeriod >= 0)
      period = alarmPeriod;
    else if (alarmPeriod >= 0 && timeoutPeriod >= 0)
      period = (timeoutPeriod < alarmPeriod) ? timeoutPeriod : alarmPeriod;

    if (period != -1)
      QTimer::singleShot(period,this,SLOT(handleTimeout()));
  }

  // Check to see if the dispatcher has been
  // stopped. This may have occurred from
  // some function not called as a result of
  // a job in the dispatcher but some other
  // code module bound into the application
  // or another thread.

  if (isStopped())
  {
    shutdown();

    return;
  }

  // If wakeup type is set to 0, check all
  // possibilities.

  if (theType == 0)
    theType = ~0;

  // Look for executable priority jobs.

  if (theType & OTCLIB_WAKEUP_PRIORITY_JOB ||
   theType & OTCLIB_WAKEUP_IDLE_JOB)
  {
    if (!isPriorityJobsEmpty())
    {
      QTimer::singleShot(0,this,SLOT(handlePriorityJob()));

      return;
    }
  }

  // Look for executable standard jobs.

  if (theType & OTCLIB_WAKEUP_STANDARD_JOB ||
   theType & OTCLIB_WAKEUP_IDLE_JOB)
  {
    if (!isStandardJobsEmpty())
    {
      QTimer::singleShot(0,this,SLOT(handleStandardJob()));

      return;
    }
  }

  // Look for executable idle jobs.

  if (theType & OTCLIB_WAKEUP_IDLE_JOB)
  {
    if (!isIdleJobsEmpty())
    {
      QTimer::singleShot(0,this,SLOT(handleIdleJob()));

      return;
    }
  }

  // Look for pending standard jobs.

  if (theType & OTCLIB_WAKEUP_STANDARD_JOB ||
   theType & OTCLIB_WAKEUP_IDLE_JOB)
  {
    if (!isPendingStandardJobsEmpty())
    {
      QTimer::singleShot(0,this,SLOT(handlePendingStandardJob()));

      return;
    }
  }

  // Look for pending idle jobs.

  if (theType & OTCLIB_WAKEUP_IDLE_JOB)
  {
    if (!isPendingIdleJobsEmpty())
    {
      QTimer::singleShot(0,this,SLOT(handlePendingIdleJob()));

      return;
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_QtJobQueue::handleQuit()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_QtJobQueue::handleQuit()");

  OTC_Dispatcher::stop();
}

/* ------------------------------------------------------------------------- */
void OTC_QtJobQueue::handleSignal()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_QtJobQueue::handleSignal()");

  OTC_Job* theJob;

  // Clear signal/wakeup pipes now as we are
  // about to check for signals anyway.

  clearPipes();

  // Execute a signal event if we have one.

  theJob = OTCEV_Signal::pending();
  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // Qt to exit.

  if (isStopped())
    shutdown();
}

/* ------------------------------------------------------------------------- */
void OTC_QtJobQueue::handleWakeup()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_QtJobQueue::handleWakuep()");

  // Nothing to do here as wakeup()
  // should also have been called.
}

/* ------------------------------------------------------------------------- */ 
void OTC_QtJobQueue::handleTimeout()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_QtJobQueue::handleTimeout()");

  OTC_Job* theJob;

  // Execute an alarm event if we have one.

  theJob = OTCEV_Alarm::pending();
  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();

    if (isStopped())
      shutdown();

    // Set maximum time to block. This
    // is derived from pending timeouts
    // and alarms.

    long alarmPeriod = -1;
    long timeoutPeriod = -1;
    long period = -1;

    alarmPeriod = 1000 * OTCEV_Alarm::period();
    timeoutPeriod = OTCEV_Timeout::period();

    if (alarmPeriod < 0 && timeoutPeriod >= 0)
      period = timeoutPeriod;
    else if (timeoutPeriod < 0 && alarmPeriod >= 0)
      period = alarmPeriod;
    else if (alarmPeriod >= 0 && timeoutPeriod >= 0)
      period = (timeoutPeriod < alarmPeriod) ? timeoutPeriod : alarmPeriod;

    if (period != -1)
      QTimer::singleShot(period,this,SLOT(handleTimeout()));

    return;
  }

  // Execute a timeout event if we have one.

  theJob = OTCEV_Timeout::pending();
  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();

    if (isStopped())
      shutdown();

    // Set maximum time to block. This
    // is derived from pending timeouts
    // and alarms.

    long alarmPeriod = -1;
    long timeoutPeriod = -1;
    long period = -1;

    alarmPeriod = 1000 * OTCEV_Alarm::period();
    timeoutPeriod = OTCEV_Timeout::period();

    if (alarmPeriod < 0 && timeoutPeriod >= 0)
      period = timeoutPeriod;
    else if (timeoutPeriod < 0 && alarmPeriod >= 0)
      period = alarmPeriod;
    else if (alarmPeriod >= 0 && timeoutPeriod >= 0)
      period = (timeoutPeriod < alarmPeriod) ? timeoutPeriod : alarmPeriod;

    if (period != -1)
      QTimer::singleShot(period,this,SLOT(handleTimeout()));

    return;
  }

  // If dispatcher has been stopped, cause
  // Qt to exit.

  if (isStopped())
    shutdown();
}

/* ------------------------------------------------------------------------- */
void OTC_QtJobQueue::handlePriorityJob()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_QtJobQueue::handlePriorityJob()");

  OTC_Job* theJob;

  // Execute a priority job if we have one.

  theJob = nextPriorityJob();
  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // Qt to exit.

  if (isStopped())
    shutdown();
}

/* ------------------------------------------------------------------------- */
void OTC_QtJobQueue::handlePendingStandardJob()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_QtJobQueue::handlePendingStandardJob()");

  OTC_Job* theJob;

  // Fill up standard jobs and look for
  // standard jobs again.

  updateStandardJobs();

  // Execute a standard job if we have one.

  theJob = nextStandardJob();
  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // Qt to exit.

  if (isStopped())
    shutdown();
}

/* ------------------------------------------------------------------------- */
void OTC_QtJobQueue::handlePendingIdleJob()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_QtJobQueue::handlePendingIdleJob()");

  OTC_Job* theJob;

  // Fill up standard jobs and look for
  // standard jobs again.

  updateIdleJobs();

  // Execute a standard job if we have one.

  theJob = nextIdleJob();
  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // Qt to exit.

  if (isStopped())
    shutdown();
}

/* ------------------------------------------------------------------------- */
void OTC_QtJobQueue::handleStandardJob()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_QtJobQueue::handleStandardJob()");

  OTC_Job* theJob;

  // Execute a standard job if we have one.

  theJob = nextStandardJob();
  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // Qt to exit.

  if (isStopped())
    shutdown();
}

/* ------------------------------------------------------------------------- */
void OTC_QtJobQueue::handleIdleJob()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_QtJobQueue::handleIdleJob()");

  OTC_Job* theJob;

  // Execute a idle job if we have one.

  theJob = nextIdleJob();
  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // TCL to exit.

  if (isStopped())
    shutdown();
}

/* ------------------------------------------------------------------------- */
void OTC_QtJobQueue::handleSocketRead(int theSocket)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_QtJobQueue::handleSocketRead()");

  OTC_Job* theJob;

  theJob = OTCEV_IOEvent::job(theSocket,OTCLIB_POLLIN);

  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // Qt to exit.

  if (isStopped())
    shutdown();
}

/* ------------------------------------------------------------------------- */
void OTC_QtJobQueue::handleSocketWrite(int theSocket)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_QtJobQueue::handleSocketWrite()");

  OTC_Job* theJob;

  theJob = OTCEV_IOEvent::job(theSocket,OTCLIB_POLLOUT);

  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // Qt to exit.

  if (isStopped())
    shutdown();
}

/* ------------------------------------------------------------------------- */
void OTC_QtJobQueue::handleSocketException(int theSocket)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_QtJobQueue::handleSocketException()");

  OTC_Job* theJob;

  theJob = OTCEV_IOEvent::job(theSocket,OTCLIB_POLLPRI);

  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // Qt to exit.

  if (isStopped())
    shutdown();
}

/* ------------------------------------------------------------------------- */
