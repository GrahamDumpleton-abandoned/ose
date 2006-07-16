/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     tcl/otctclqueue.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/tcl/tclqueue.hh"
#endif
#endif

#include <OTC/tcl/tclqueue.hh>

#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/alarm.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/ioevent.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/thread/mxreaper.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/logger.hh>

#include <stdlib.h>

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#define SOCKET_MESSAGE WM_USER+1

/* ------------------------------------------------------------------------- */
#ifdef OTCLIB_TRACE
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_TclJobQueue");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()
#endif

/* ------------------------------------------------------------------------- */
struct OTC_TclFileHandlerEvent
{
  Tcl_Event header;
  int fd;
  int mask;
};

/* ------------------------------------------------------------------------- */

class OTC_TclDummyJob : public OTC_Job
{
  public:

                        OTC_TclDummyJob() {}

                        ~OTC_TclDummyJob();

  protected:

    void                execute();
};

/* ------------------------------------------------------------------------- */
OTC_TclDummyJob::~OTC_TclDummyJob()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_TclDummyJob::execute()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_TclJobQueue* OTC_TclJobQueue::instance_ = 0;
#if TCL_MAJOR_VERSION >= 8
OTC_HMap<int,int> OTC_TclJobQueue::fileSubscriptions_;
OTC_Boolean OTC_TclJobQueue::signalFile_ = OTCLIB_FALSE;
OTC_Boolean OTC_TclJobQueue::wakeupFile_ = OTCLIB_FALSE;
#if !defined(SYS_UNIX)
HWND OTC_TclJobQueue::winHandle_;
WNDCLASS OTC_TclJobQueue::winClass_;
#endif
#endif
OTC_IOSubscription* OTC_TclJobQueue::pendingFdEvents_ = 0;

/* ------------------------------------------------------------------------- */
OTC_TclJobQueue::~OTC_TclJobQueue()
{
  // This should never get called anyway.

  instance_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_TclJobQueue::OTC_TclJobQueue()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_TclJobQueue::OTC_TclJobQueue()");

  OTCLIB_ENSURE_FN((instance_ == 0),
   "OTC_TclJobQueue::OTC_TclJobQueue()",
   "instance of TCL job queue already exists");

  idleRegistered_ = OTCLIB_FALSE;

  pendingFdEvents_ = new OTC_IOSubscription[
   OTCEV_IOEvent::maxSubscriptions()];
  OTCLIB_ASSERT_M(pendingFdEvents_ != 0);

#if TCL_MAJOR_VERSION >= 8
#if !defined(SYS_UNIX)
  // Create Win32 window for event callbacks.

  OSVERSIONINFO osInfo;

  winClass_.style = 0;
  winClass_.cbClsExtra = 0;
  winClass_.cbWndExtra = 0;
  winClass_.hbrBackground = NULL;
  winClass_.lpszMenuName = NULL;
  winClass_.lpszClassName = "OTC_TclJobQueue";
  winClass_.lpfnWndProc = fileCallbackWEP_;
  winClass_.hIcon = NULL;
  winClass_.hCursor = NULL;

  osInfo.dwOSVersionInfoSize = sizeof(osInfo);
  GetVersionEx(&osInfo);
  if (osInfo.dwPlatformId == VER_PLATFORM_WIN32s ||
   osInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
  {
    // 95 and non threaded platforms.

    winClass_.hInstance = GetModuleHandle(NULL);
  }
  else
  {
    // NT and hopefully everything following.
    winClass_.hInstance = (HINSTANCE)GetCurrentProcess();
  }

  if (RegisterClass(&winClass_))
  {
    winHandle_ = CreateWindow("OTC_TclJobQueue","OTC_TclJobQueue",
     WS_TILED,0,0,0,0,NULL,NULL,winClass_.hInstance,NULL);

    if (winHandle_ == 0)
    {
      OTC_LogStream theLogger;
      theLogger.setLevel(OTCLIB_LOG_WARNING);

      theLogger << "Operation failed" << EOL;
      theLogger << "Class: OTC_TclJobQueue" << EOL;
      theLogger << "Method: OTC_TclJobQueue()" << EOL;
      theLogger << "Description: can't create window handle" << EOL;
      theLogger << "Result: signal and i/o handling broken" << EOL;
      theLogger << flush;
    }
  }
  else
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTC_TclJobQueue" << EOL;
    theLogger << "Method: OTC_TclJobQueue()" << EOL;
    theLogger << "Description: can't register windows class" << EOL;
    theLogger << "Result: signal and i/o handling broken" << EOL;
    theLogger << flush;
  }
#endif
#endif

  instance_ = this;

  Tcl_CreateEventSource(setupCallback_,checkCallback_,0);
}

/* ------------------------------------------------------------------------- */
OTC_Job* OTC_TclJobQueue::next(int, int)
{
  // Call TCL to dispatch one event.

  int theResult;

  theResult = Tcl_DoOneEvent(0);

  // If TCL found nothing to do, don't
  // return a job.

  if (theResult == 0)
    return 0;

  // Return a dummy job.

  OTC_Job* theJob;

  theJob = new OTC_TclDummyJob;
  OTCLIB_ASSERT_M(theJob != 0);

  return theJob;
}

/* ------------------------------------------------------------------------- */
void OTC_TclJobQueue::shutdown()
{
  Tcl_Exit(0);
}

/* ------------------------------------------------------------------------- */
void OTC_TclJobQueue::setupCallback_(ClientData, int)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_TclJobQueue::setupCallback_(...)");

  // Check to see if the dispatcher has been
  // stopped. This may have occurred from
  // some function not called as a result of
  // a job in the dispatcher but some other
  // code module bound into the application
  // or another thread.

  if (instance_->isStopped())
  {
    instance_->shutdown();

    OTCLIB_TRACER(MODULE) << "shutdown has occurred" << endl;

    return;
  }

  // Time structure. We use this in a number
  // of places. Default values will be zero
  // timeout.

  struct Tcl_Time tmpTime;
  tmpTime.sec = 0;
  tmpTime.usec = 0;


  // Clear signal/wakeup pipes now as we are
  // about to check for signals anyway.

  instance_->clearPipes();

  // Check for a pending signal and if there is
  // one queue a TCL event straight away and
  // exit. Put the event at the head of the
  // queue.

  if (OTCEV_Signal::isPending())
  {
    Tcl_SetMaxBlockTime(&tmpTime);

    OTCLIB_TRACER(MODULE) << "signal is pending" << endl;

    return;
  }

  // Look for executable priority jobs.

  if (!instance_->isPriorityJobsEmpty())
  {
    Tcl_SetMaxBlockTime(&tmpTime);

    OTCLIB_TRACER(MODULE) << "priority job is pending" << endl;

    return;
  }

  // Look for executable standard jobs.

  if (!instance_->isStandardJobsEmpty())
  {
    Tcl_SetMaxBlockTime(&tmpTime);

    OTCLIB_TRACER(MODULE) << "standard job is pending" << endl;

    return;
  }

  // Look for pending standard jobs.

  if (!instance_->isPendingStandardJobsEmpty())
  {
    Tcl_SetMaxBlockTime(&tmpTime);

    OTCLIB_TRACER(MODULE) << "standard jobs are queued" << endl;

    return;
  }

  // Look for executable idle jobs.

  if (!instance_->isIdleJobsEmpty())
  {
    Tcl_SetMaxBlockTime(&tmpTime);

    OTCLIB_TRACER(MODULE) << "idle jobs are queued" << endl;

    return;
  }

  // Register idle call back if we need to.

  if (instance_->idleRegistered_ == OTCLIB_FALSE
   && !instance_->isPendingIdleJobsEmpty())
  {
    Tcl_DoWhenIdle(idleCallback_,0);
    instance_->idleRegistered_ = OTCLIB_TRUE;

    OTCLIB_TRACER(MODULE) << "idle callback registered" << endl;
  }

  // Register file descriptor for watching of signals.

  if (instance_->signalPipe() != -1)
  {

#if TCL_MAJOR_VERSION >= 8
    if (signalFile_ == OTCLIB_FALSE)
    {
      OTCLIB_TRACER(MODULE) << "signal pipe subscribed = "
       << instance_->signalPipe() << endl;

      signalFile_ = OTCLIB_TRUE;
#if defined(SYS_UNIX)
      Tcl_CreateFileHandler(instance_->signalPipe(),TCL_READABLE,
       fileCallback_,(ClientData)instance_->signalPipe());
#else
      WSAAsyncSelect(instance_->signalPipe(),
       winHandle_,SOCKET_MESSAGE,FD_READ);
      fileSubscriptions_.add(instance_->signalPipe(),OTCLIB_POLLIN);
#endif
    }
#else
    Tcl_File file = Tcl_GetFile(
     (ClientData)instance_->signalPipe(),TCL_UNIX_FD);
    Tcl_WatchFile(file,TCL_READABLE);
#endif
  }

  if (instance_->wakeupPipe() != -1)
  {
#if TCL_MAJOR_VERSION >= 8
    if (wakeupFile_ == OTCLIB_FALSE)
    {
      OTCLIB_TRACER(MODULE) << "wakeup pipe subscribed = "
       << instance_->wakeupPipe() << endl;

      wakeupFile_ = OTCLIB_TRUE;
#if defined(SYS_UNIX)
      Tcl_CreateFileHandler(instance_->wakeupPipe(),TCL_READABLE,
       fileCallback_,(ClientData)instance_->wakeupPipe());
#else
      WSAAsyncSelect(instance_->wakeupPipe(),
       winHandle_,SOCKET_MESSAGE,FD_READ);
      fileSubscriptions_.add(instance_->wakeupPipe(),OTCLIB_POLLIN);
#endif
    }
#else
    Tcl_File file = Tcl_GetFile(
     (ClientData)instance_->wakeupPipe(),TCL_UNIX_FD);
    Tcl_WatchFile(file,TCL_READABLE);
#endif
  }

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

  tmpTime.sec = period / 1000;
  tmpTime.usec = (period % 1000) * 1000;

  OTCLIB_TRACER(MODULE) << "block period = " << period << endl;

  if (period != -1)
  {
    OTCLIB_TRACER(MODULE) << "set block period" << endl;

    Tcl_SetMaxBlockTime(&tmpTime);
  }

  // Register file descriptor events.

  int res;

#if TCL_MAJOR_VERSION >= 8
  OTC_PairIterator<int,int> thePairs;
  thePairs = fileSubscriptions_.pairs();
  while (thePairs.isValid())
  {
    if (thePairs.key() != instance_->signalPipe() &&
     thePairs.key() != instance_->wakeupPipe() &&
     OTCEV_IOEvent::events(thePairs.key()) != thePairs.item())
    {
      OTCLIB_TRACER(MODULE) << "socket unsubscribed = "
       << thePairs.key() << endl;

#if defined(SYS_UNIX)
      Tcl_DeleteFileHandler(thePairs.key());
#else
      WSAAsyncSelect(thePairs.key(),winHandle_,SOCKET_MESSAGE,0);
#endif
      fileSubscriptions_.remove(thePairs.key());
    }

    thePairs.next();
  }
#endif

  int fdCnt = 0;

  fdCnt = OTCEV_IOEvent::subscriptions(pendingFdEvents_);

  for (int i=0; i<fdCnt; i++)
  {
    res = pendingFdEvents_[i].events;

    if (res != 0)
    {
      int mask = 0;

      int fd = 0;
      fd = pendingFdEvents_[i].fd;

#if TCL_MAJOR_VERSION >= 8
      if (!fileSubscriptions_.contains(fd))
      {
	OTCLIB_TRACER(MODULE) << "socket subscribed = " << fd << endl;

	fileSubscriptions_.add(fd,res);
#if defined(SYS_UNIX)

	if (res & OTCLIB_POLLIN)
	  mask |= TCL_READABLE;
	if (res & OTCLIB_POLLOUT)
	  mask |= TCL_WRITABLE;
	if (res & OTCLIB_POLLPRI)
	  mask |= TCL_EXCEPTION;

	Tcl_CreateFileHandler(fd,mask,fileCallback_,(ClientData)fd);
#else
	if (res & OTCLIB_POLLIN)
	{
	  mask |= FD_CONNECT;
	  mask |= FD_ACCEPT;
	  mask |= FD_READ;
	  mask |= FD_CLOSE;
	}

	if (res & OTCLIB_POLLOUT)
	  mask |= FD_WRITE;
	if (res & OTCLIB_POLLPRI)
	  mask |= FD_OOB;

	WSAAsyncSelect(fd,winHandle_,SOCKET_MESSAGE,mask);
#endif
      }
#else
      if (res & OTCLIB_POLLIN)
	mask |= TCL_READABLE;
      if (res & OTCLIB_POLLOUT)
	mask |= TCL_WRITABLE;
      if (res & OTCLIB_POLLPRI)
	mask |= TCL_EXCEPTION;

      Tcl_File file = Tcl_GetFile((ClientData)fd,TCL_UNIX_FD);
      Tcl_WatchFile(file,mask);
#endif
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_TclJobQueue::checkCallback_(ClientData, int)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_TclJobQueue::checkCallback_(...)");

  // Generate events for file descriptors.
  // This is for TCL prior to 8.0 only.
  // It queues events to a event procedure
  // which maps to 8.0 file callback. We
  // do it now as when we return, the record
  // of what was ready will be wiped out.

#if TCL_MAJOR_VERSION < 8
  for (int i=0; i<=OTCEV_IOEvent::maxFd(); i++)
  {
    Tcl_File file = Tcl_GetFile((ClientData)i,TCL_UNIX_FD);

    int res = Tcl_FileReady(file,~0);

    if (res != 0)
    {
      OTC_TclFileHandlerEvent* theEvent;
      theEvent = (OTC_TclFileHandlerEvent*)malloc(
       sizeof(OTC_TclFileHandlerEvent));

      theEvent->header.proc = fileCallbackFEP_;
      theEvent->fd = i;
      theEvent->mask = res;

      Tcl_QueueEvent(&theEvent->header,TCL_QUEUE_TAIL);
    }
  }
#endif

  // Look for pending signals.

  if (OTCEV_Signal::isPending())
  {
    OTCLIB_TRACER(MODULE) << "signal is pending" << endl;

    Tcl_Event* theEvent;
    theEvent = (Tcl_Event*)malloc(sizeof(Tcl_Event));
    theEvent->proc = signalCallback_;
    Tcl_QueueEvent(theEvent,TCL_QUEUE_HEAD);

    return;
  }

  // Look for priority jobs.

  if (!instance_->isPriorityJobsEmpty())
  {
    OTCLIB_TRACER(MODULE) << "priority job pending" << endl;

    Tcl_Event* theEvent;
    theEvent = (Tcl_Event*)malloc(sizeof(Tcl_Event));
    theEvent->proc = priorityJobCallback_;
    Tcl_QueueEvent(theEvent,TCL_QUEUE_TAIL);

    return;
  }

  // Look for pending alarms.

  if (OTCEV_Alarm::isPending())
  {
    OTCLIB_TRACER(MODULE) << "alarm pending" << endl;

    Tcl_Event* theEvent;
    theEvent = (Tcl_Event*)malloc(sizeof(Tcl_Event));
    theEvent->proc = alarmCallback_;
    Tcl_QueueEvent(theEvent,TCL_QUEUE_TAIL);

    return;
  }

  // Look for pending timeout but only if there
  // are standard jobs to process.

  if (!instance_->isStandardJobsEmpty())
  {
    if (OTCEV_Timeout::isPending())
    {
      OTCLIB_TRACER(MODULE) << "timeout pending 1" << endl;

      Tcl_Event* theEvent;
      theEvent = (Tcl_Event*)malloc(sizeof(Tcl_Event));
      theEvent->proc = timeoutCallback_;
      Tcl_QueueEvent(theEvent,TCL_QUEUE_TAIL);

      return;
    }
  }

  // Look for standard jobs.

  if (!instance_->isStandardJobsEmpty())
  {
    OTCLIB_TRACER(MODULE) << "standard job pending" << endl;

    Tcl_Event* theEvent;
    theEvent = (Tcl_Event*)malloc(sizeof(Tcl_Event));
    theEvent->proc = standardJobCallback_;
    Tcl_QueueEvent(theEvent,TCL_QUEUE_TAIL);

    return;
  }

  // Look for idle jobs.

  if (!instance_->isIdleJobsEmpty())
  {
    OTCLIB_TRACER(MODULE) << "idle job pending" << endl;

    Tcl_Event* theEvent;
    theEvent = (Tcl_Event*)malloc(sizeof(Tcl_Event));
    theEvent->proc = idleJobCallback_;
    Tcl_QueueEvent(theEvent,TCL_QUEUE_TAIL);

    return;
  }

  // Look for pending timeout, but only if
  // we didn't have standard jobs to process.

  if (instance_->isStandardJobsEmpty())
  {
    if (OTCEV_Timeout::isPending())
    {
      OTCLIB_TRACER(MODULE) << "timeout pending 2" << endl;

      Tcl_Event* theEvent;
      theEvent = (Tcl_Event*)malloc(sizeof(Tcl_Event));
      theEvent->proc = timeoutCallback_;
      Tcl_QueueEvent(theEvent,TCL_QUEUE_TAIL);

      return;
    }
  }

  // Fill up standard jobs and look for
  // standard jobs again.

  OTCLIB_TRACER(MODULE) << "update standard jobs" << endl;

  instance_->updateStandardJobs();

  if (!instance_->isStandardJobsEmpty())
  {
    OTCLIB_TRACER(MODULE) << "standard job pending" << endl;

    Tcl_Event* theEvent;
    theEvent = (Tcl_Event*)malloc(sizeof(Tcl_Event));
    theEvent->proc = standardJobCallback_;
    Tcl_QueueEvent(theEvent,TCL_QUEUE_TAIL);

    return;
  }
}

/* ------------------------------------------------------------------------- */
void OTC_TclJobQueue::idleCallback_(ClientData)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_TclJobQueue::idleCallback_(...)");

  // Move jobs from idle queue to pending
  // queue if idle job registration had
  // been done.

  if (instance_->idleRegistered_ == OTCLIB_TRUE)
    instance_->updateIdleJobs();

  // Idle callback will have been deregistered,
  // remember this fact as we will need to
  // register again later when we know we have
  // idle jobs again.

  instance_->idleRegistered_ = OTCLIB_FALSE;

  // Execute first idle job.

  OTC_Job* theJob;

  theJob = instance_->nextIdleJob();

  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // Schedule a dummy event so TCL knows we did
  // at least do something in case it drops out
  // thinking we didn't.

  Tcl_Event* theEvent;
  theEvent = (Tcl_Event*)malloc(sizeof(Tcl_Event));
  theEvent->proc = dummyCallback_;
  Tcl_QueueEvent(theEvent,TCL_QUEUE_TAIL);

  // If dispatcher has been stopped, cause
  // TCL to exit.

  if (instance_->isStopped())
    instance_->shutdown();
}

/* ------------------------------------------------------------------------- */
int OTC_TclJobQueue::signalCallback_(Tcl_Event*, int)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_TclJobQueue::signalCallback_(...)");

  OTC_Job* theJob;

  // Execute a signal event if we have one.

  theJob = OTCEV_Signal::pending();
  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // TCL to exit.

  if (instance_->isStopped())
    instance_->shutdown();

  return 1;
}

/* ------------------------------------------------------------------------- */ int OTC_TclJobQueue::alarmCallback_(Tcl_Event*, int)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_TclJobQueue::alarmCallback_(...)");

  OTC_Job* theJob;

  // Execute an alarm event if we have one.

  theJob = OTCEV_Alarm::pending();
  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // TCL to exit.

  if (instance_->isStopped())
    instance_->shutdown();

  return 1;
}

/* ------------------------------------------------------------------------- */
int OTC_TclJobQueue::timeoutCallback_(Tcl_Event*, int)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_TclJobQueue::timeoutCallback_(...)");

  OTC_Job* theJob;

  // Execute a timeout event if we have one.

  theJob = OTCEV_Timeout::pending();
  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // TCL to exit.

  if (instance_->isStopped())
    instance_->shutdown();

  return 1;
}

/* ------------------------------------------------------------------------- */
int OTC_TclJobQueue::priorityJobCallback_(Tcl_Event*, int)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_TclJobQueue::priorityJobCallback_(...)");

  OTC_Job* theJob;

  // Execute a priority job if we have one.

  theJob = instance_->nextPriorityJob();
  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // TCL to exit.

  if (instance_->isStopped())
    instance_->shutdown();

  return 1;
}

/* ------------------------------------------------------------------------- */
int OTC_TclJobQueue::standardJobCallback_(Tcl_Event*, int)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_TclJobQueue::standardJobCallback_(...)");

  OTC_Job* theJob;

  // Execute a standard job if we have one.

  theJob = instance_->nextStandardJob();
  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // TCL to exit.

  if (instance_->isStopped())
    instance_->shutdown();

  return 1;
}

/* ------------------------------------------------------------------------- */
int OTC_TclJobQueue::idleJobCallback_(Tcl_Event*, int)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_TclJobQueue::idleJobCallback_(...)");

  OTC_Job* theJob;

  // Execute a idle job if we have one.

  theJob = instance_->nextIdleJob();
  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // TCL to exit.

  if (instance_->isStopped())
    instance_->shutdown();

  return 1;
}

/* ------------------------------------------------------------------------- */
int OTC_TclJobQueue::dummyCallback_(Tcl_Event*, int)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_TclJobQueue::dummyCallback_(...)");

  return 1;
}

/* ------------------------------------------------------------------------- */
int OTC_TclJobQueue::fileCallbackFEP_(Tcl_Event* theEvent, int)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_TclJobQueue::fileCallbackFEP_(...)");

  // Massage event into a call to
  // <fileCallback_> function. We have this
  // function for TCL prior to 8.0.

  OTC_TclFileHandlerEvent* theFileHandlerEvent;
  theFileHandlerEvent = (OTC_TclFileHandlerEvent*)theEvent;

  fileCallback_(
   (ClientData)theFileHandlerEvent->fd,
   theFileHandlerEvent->mask);

  return 1;
}

#if !defined(SYS_UNIX)
/* ------------------------------------------------------------------------- */
LRESULT CALLBACK OTC_TclJobQueue::fileCallbackWEP_(
 HWND hwnd,
 UINT message,
 WPARAM wParam,
 LPARAM lParam
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_TclJobQueue::fileCallbackWEP_(...)");

  int event;
  int error;
  SOCKET socket;

  OTCLIB_TRACER(MODULE) << "message = " << message << endl;

  if (message != SOCKET_MESSAGE)
    return DefWindowProc(hwnd,message,wParam,lParam);

  event = WSAGETSELECTEVENT(lParam);
  error = WSAGETSELECTERROR(lParam);
  socket = (SOCKET)wParam;

  OTCLIB_TRACER(MODULE) << "event = " << event << endl;
  OTCLIB_TRACER(MODULE) << "socket = " << socket << endl;
  OTCLIB_TRACER(MODULE) << "error = " << error << endl;

#if 0
  // XXX Don't know if I should do something
  // special with this or not.

  if (error != ERROR_SUCCESS)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTC_TclJobQueue" << EOL;
    theLogger << "Method: fileCallbackWEP_()" << EOL;
    theLogger << "Description: error notification received" << EOL;
    theLogger << "Error: " << error << EOL;
    theLogger << flush;
  }
#endif

  if (!fileSubscriptions_.contains(socket))
  {
    // If we don't have something as a valid
    // subscription kill any callback. This is to
    // cater for the case that a socket created
    // by accept() inherits the WinSock socket
    // subscriptions of the listener socket.

    WSAAsyncSelect(socket,hwnd,SOCKET_MESSAGE,0);

    return 0;
  }
  
  int theMask = 0;

  if (event & FD_CONNECT)
    theMask |= TCL_READABLE;
  if (event & FD_ACCEPT)
    theMask |= TCL_READABLE;
  if (event & FD_READ)
    theMask |= TCL_READABLE;
  if (event & FD_CLOSE)
    theMask |= TCL_READABLE;
  if (event & FD_WRITE)
    theMask |= TCL_WRITABLE;
  if (event & FD_OOB)
    theMask |= TCL_EXCEPTION;

  fileCallback_((ClientData)socket,theMask);

  return 0;
}
#endif

/* ------------------------------------------------------------------------- */
void OTC_TclJobQueue::fileCallback_(ClientData theFd, int theMask)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_TclJobQueue::fileCallback_(...)");

  int tmpFd = (int)theFd;

  // Check to see if this is the signal pipe.

  OTCLIB_TRACER(MODULE) << "check signal pipe" << endl;

  if (tmpFd == instance_->signalPipe())
  {
    // Clear wakeup/signal pipe.

    OTCLIB_TRACER(MODULE) << "clear signal pipe" << endl;

    instance_->clearPipes();

    // Queue an event to handle the signal.

    OTCLIB_TRACER(MODULE) << "queue event for signal" << endl;

    Tcl_Event* theEvent;
    theEvent = (Tcl_Event*)malloc(sizeof(Tcl_Event));
    theEvent->proc = signalCallback_;
    Tcl_QueueEvent(theEvent,TCL_QUEUE_HEAD);
  }

  // Check to see if this is the wakeup pipe. 

  OTCLIB_TRACER(MODULE) << "check wakeup pipe" << endl;

  if (tmpFd == instance_->wakeupPipe())
  {
    // Clear wakeup/signal pipe.

    OTCLIB_TRACER(MODULE) << "clear wakeup pipe" << endl;

    instance_->clearPipes();
  }

  // Now handle normal descriptors. Note that
  // we need to convert TCL mask bits to OSE
  // mask bits.

  OTCLIB_TRACER(MODULE) << "build mask" << endl;

  int tmpMask = 0;

  if (theMask & TCL_READABLE)
    tmpMask |= OTCLIB_POLLIN;
  if (theMask & TCL_WRITABLE)
    tmpMask |= OTCLIB_POLLOUT;
  if (theMask & TCL_EXCEPTION)
    tmpMask |= OTCLIB_POLLPRI;

  OTCLIB_TRACER(MODULE) << "mask = " << tmpMask << endl;

  OTC_Job* theJob;

  OTCLIB_TRACER(MODULE) << "look for job" << endl;

  theJob = OTCEV_IOEvent::job(tmpFd,tmpMask);

  if (theJob != 0)
  {
    OTCLIB_TRACER(MODULE) << "execute job" << endl;

    theJob->execute();
    theJob->destroy();
  }

  // If dispatcher has been stopped, cause
  // TCL to exit.

  if (instance_->isStopped())
  {
    OTCLIB_TRACER(MODULE) << "stop dispatcher" << endl;

    instance_->shutdown();
  }
}

/* ------------------------------------------------------------------------- */
