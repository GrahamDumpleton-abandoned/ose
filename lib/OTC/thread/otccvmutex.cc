/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/otccvmutex.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/thread/cvmutex.hh"
#endif
#endif

#include <OTC/thread/cvmutex.hh>

#if defined(HAVE_SOLARIS_THREADS)
/* ------------------------------------------------------------------------- */
OTC_CVMutex::OTC_CVMutex(OTC_NRMutex const& theMutex)
  : mutex_(theMutex)
{
  cond_init(&variable_,USYNC_THREAD,0);
  valid_ = 1;
}

/* ------------------------------------------------------------------------- */
OTC_CVMutex::~OTC_CVMutex()
{
  valid_ = 0;
  cond_destroy(&variable_);
}

/* ------------------------------------------------------------------------- */
void OTC_CVMutex::wait() const
{
  OTC_CVMutex* self = (OTC_CVMutex*)this;

  if (self->valid_ == 0)
    return;

  cond_wait(&self->variable_,(mutex_t*)&self->mutex_.mutex_);
}

/* ------------------------------------------------------------------------- */
void OTC_CVMutex::signal() const
{
  OTC_CVMutex* self = (OTC_CVMutex*)this;

  if (self->valid_ == 0)
    return;

  cond_signal(&self->variable_);
}

/* ------------------------------------------------------------------------- */
void OTC_CVMutex::broadcast() const
{
  OTC_CVMutex* self = (OTC_CVMutex*)this;

  if (self->valid_ == 0)
    return;

  cond_broadcast(&self->variable_);
}
#endif

#if defined(HAVE_POSIX_THREADS)
/* ------------------------------------------------------------------------- */
OTC_CVMutex::OTC_CVMutex(OTC_NRMutex const& theMutex)
  : mutex_(theMutex)
{
  pthread_cond_init(&variable_,0);
  valid_ = 1;
}

/* ------------------------------------------------------------------------- */
OTC_CVMutex::~OTC_CVMutex()
{
  valid_ = 0;
  pthread_cond_destroy(&variable_);
}

/* ------------------------------------------------------------------------- */
void OTC_CVMutex::wait() const
{
  OTC_CVMutex* self = (OTC_CVMutex*)this;

  if (self->valid_ == 0)
    return;

  pthread_cond_wait(&self->variable_,(pthread_mutex_t*)&self->mutex_.mutex_);
}

/* ------------------------------------------------------------------------- */
void OTC_CVMutex::signal() const
{
  OTC_CVMutex* self = (OTC_CVMutex*)this;

  if (self->valid_ == 0)
    return;

  pthread_cond_signal(&self->variable_);
}

/* ------------------------------------------------------------------------- */
void OTC_CVMutex::broadcast() const
{
  OTC_CVMutex* self = (OTC_CVMutex*)this;

  if (self->valid_ == 0)
    return;

  pthread_cond_broadcast(&self->variable_);
}
#endif

#if defined(HAVE_WIN32_THREADS)
/* ------------------------------------------------------------------------- */
OTC_CVMutex::OTC_CVMutex(OTC_NRMutex const& theMutex)
  : mutex_(theMutex), waiters_(0), wasBroadcast_(0)
{
  // XXX Someone tells me this was wrong, which may
  // explain why it didn't work very well. It probably
  // still has unfairness issues. POSIX threads is
  // mandatory in next major release.
  // semaphore_ = CreateSemaphore(0,1,0x7FFFFFF,NULL);

  semaphore_ = CreateSemaphore(0,0,0x7FFFFFF,NULL);
  waitersDone_ = CreateEvent(0,FALSE,FALSE,NULL);
  valid_ = 1;
}

/* ------------------------------------------------------------------------- */
OTC_CVMutex::~OTC_CVMutex()
{
  valid_ = 0;
  CloseHandle(waitersDone_);
  CloseHandle(semaphore_);
}

/* ------------------------------------------------------------------------- */
void OTC_CVMutex::wait() const
{
  OTC_CVMutex* self = (OTC_CVMutex*)this;

  if (self->valid_ == 0)
    return;

  self->waitersLock_.lock();
  self->waiters_++;
  self->waitersLock_.unlock();

  self->mutex_.unlock();

  WaitForSingleObject(self->semaphore_,INFINITE);

  self->waitersLock_.lock();
  self->waiters_--;

  int theLast = 0;

  if (self->wasBroadcast_ && (self->waiters_ == 0))
  {
    theLast = 1;

    self->wasBroadcast_ = 0;
  }

  self->waitersLock_.unlock();

  if (theLast != 0)
    SetEvent(self->waitersDone_);

  self->mutex_.lock();
}

/* ------------------------------------------------------------------------- */
void OTC_CVMutex::signal() const
{
  OTC_CVMutex* self = (OTC_CVMutex*)this;

  if (self->valid_ == 0)
    return;

  if (self->waiters_ > 0)
    ReleaseSemaphore(self->semaphore_,1,NULL);
}

/* ------------------------------------------------------------------------- */
void OTC_CVMutex::broadcast() const
{
  OTC_CVMutex* self = (OTC_CVMutex*)this;

  if (self->valid_ == 0)
    return;

  self->waitersLock_.lock();

  self->wasBroadcast_ = 1;

  int theWait = 0;

  if (self->waiters_ > 0)
    theWait = 1;

  ReleaseSemaphore(self->semaphore_,self->waiters_,NULL);

  self->waitersLock_.unlock();

  if (theWait != 0)
    WaitForSingleObject(self->waitersDone_,INFINITE);
}
#endif

/* ------------------------------------------------------------------------- */
