/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcdispatch.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/dispatch.hh"
#endif
#endif

#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/thread/mxreaper.hh>

#if defined(SYS_UNIX)
#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif
#else
#include <windows.h>
#include <process.h>
#if !defined(__BORLANDC__)
#ifndef getpid
#define getpid _getpid
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_Dispatcher::mutex_;
OTC_JobQueue* OTC_Dispatcher::globJobQueue_ = 0;
long OTC_Dispatcher::processId_ = 0;

/* ------------------------------------------------------------------------- */
void OTC_Dispatcher::initialise(
 void (*theFunc)()
)
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = new OTC_JobQueue(theFunc);
  OTCLIB_ASSERT_M(theJobQueue != 0);

  initialise(theJobQueue);
}

/* ------------------------------------------------------------------------- */
void OTC_Dispatcher::initialise(OTC_JobQueue* theJobQueue)
{
  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::initialise(OTC_JobQueue*, void (*)())",
   "invalid jobqueue");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCLIB_ENSURE_FN((globJobQueue_ == 0),
   "OTC_Dispatcher::initialise(OTC_JobQueue*, void (*)())",
   "dispatcher already initialised");

  globJobQueue_ = theJobQueue;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_JobQueue* OTC_Dispatcher::queue()
{
  OTC_JobQueue* theJobQueue;

  mutex_.lock();

  theJobQueue = globJobQueue_;

  mutex_.unlock();

  return theJobQueue;
}

/* ------------------------------------------------------------------------- */
int OTC_Dispatcher::dispatch(int theActions, int theOptions)
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::dispatch(int,int)",
   "dispatcher not initialised");

  return theJobQueue->dispatch(theActions,theOptions);
}

/* ------------------------------------------------------------------------- */
int OTC_Dispatcher::run()
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::run()",
   "dispatcher not initialised");

  processId_ = getpid();

  return theJobQueue->run();
}

/* ------------------------------------------------------------------------- */
int OTC_Dispatcher::run(OTC_Condition* theCondition)
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::run()",
   "dispatcher not initialised");

  processId_ = getpid();

  return theJobQueue->run(theCondition);
}

/* ------------------------------------------------------------------------- */
void OTC_Dispatcher::stop()
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::stop()",
   "dispatcher not initialised");

  theJobQueue->stop();
}

/* ------------------------------------------------------------------------- */
void OTC_Dispatcher::reset()
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::reset()",
   "dispatcher not initialised");

  theJobQueue->reset();
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_Dispatcher::isWaiting()
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::isWaiting()",
   "dispatcher not initialised");

  return theJobQueue->isWaiting();
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_Dispatcher::isRunning()
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::isRunning()",
   "dispatcher not initialised");

  return theJobQueue->isRunning();
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_Dispatcher::isStopped()
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::isStopped()",
   "dispatcher not initialised");

  return theJobQueue->isStopped();
}

/* ------------------------------------------------------------------------- */
void OTC_Dispatcher::wakeup(int theType)
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::isStopped()",
   "dispatcher not initialised");

  theJobQueue->wakeup(theType);
}

/* ------------------------------------------------------------------------- */
void OTC_Dispatcher::schedule(OTC_Job* theJob, int theType)
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::isStopped()",
   "dispatcher not initialised");

  theJobQueue->add(theJob,theType);
}

/* ------------------------------------------------------------------------- */
