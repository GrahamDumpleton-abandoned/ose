/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/otcmutex.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2005 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/thread/mutex.hh"
#endif
#endif

#include <OTC/thread/mutex.hh>

#include <memory.h>

#if defined(HAVE_SOLARIS_THREADS)
/* ------------------------------------------------------------------------- */
OTC_Mutex::OTC_Mutex()
  : nest_(0), thread_(0)
{
  mutex_init(&mutex_,USYNC_THREAD,0);
  valid_ = 1;
}

/* ------------------------------------------------------------------------- */
OTC_Mutex::~OTC_Mutex()
{
  valid_ = 0;
  mutex_destroy(&mutex_);
}

/* ------------------------------------------------------------------------- */
void OTC_Mutex::lock() const
{
  OTC_Mutex* self = (OTC_Mutex*)this;

  if (self->valid_ == 0)
    return;

  thread_t theThread;

  theThread = thr_self();

  if (theThread == self->thread_)
  {
    self->nest_++;
  }
  else
  {
    if (mutex_lock(&self->mutex_) == 0)
    {
      self->thread_ = theThread;
      self->nest_ = 0;
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Mutex::unlock() const
{
  OTC_Mutex* self = (OTC_Mutex*)this;

  if (self->valid_ == 0)
    return;

  if (self->nest_ > 0)
  {
    self->nest_--;
  }
  else
  {
    self->thread_ = 0;
    mutex_unlock(&self->mutex_);
  }
}
#endif

#if defined(HAVE_POSIX_THREADS)
/* ------------------------------------------------------------------------- */
OTC_Mutex::OTC_Mutex()
{
#if defined(MUTEX_RECURSIVE_NP)
  pthread_mutexattr_create(&attr_);
  pthread_mutexattr_setkind_np(&attr_,MUTEX_RECURSIVE_NP);
  pthread_mutex_init(&mutex_,attr_);
#else
  pthread_mutex_init(&mutex_,0);
  memset(&thread_,0,sizeof(thread_));
  nest_ = 0;
#endif

  valid_ = 1;
}

/* ------------------------------------------------------------------------- */
OTC_Mutex::~OTC_Mutex()
{
  valid_ = 0;
  pthread_mutex_destroy(&mutex_);

#if defined(MUTEX_RECURSIVE_NP)
  pthread_mutexattr_delete(&attr_);
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_Mutex::lock() const
{
  OTC_Mutex* self = (OTC_Mutex*)this;

  if (self->valid_ == 0)
    return;

#if defined(MUTEX_RECURSIVE_NP)
  pthread_mutex_lock(&self->mutex_);
#else
  pthread_t theThread;
 
  theThread = pthread_self();
 
  if (pthread_equal(theThread,self->thread_))
  {
    self->nest_++;
  }
  else
  {
    if (pthread_mutex_lock(&self->mutex_) == 0)
    {
      self->thread_ = theThread;
      self->nest_ = 0;
    }
  }
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_Mutex::unlock() const
{
  OTC_Mutex* self = (OTC_Mutex*)this;

  if (self->valid_ == 0)
    return;

#if defined(MUTEX_RECURSIVE_NP)
  pthread_mutex_unlock(&self->mutex_);
#else
  if (self->nest_ > 0)
  {
    self->nest_--;
  }
  else
  {
    memset(&self->thread_,0,sizeof(self->thread_));
    pthread_mutex_unlock(&self->mutex_);
  }
#endif
}
#endif

#if defined(HAVE_WIN32_THREADS)
/* ------------------------------------------------------------------------- */
OTC_Mutex::OTC_Mutex()
{
  InitializeCriticalSection(&mutex_);
  valid_ = 1;
}

/* ------------------------------------------------------------------------- */
OTC_Mutex::~OTC_Mutex()
{
  valid_ = 0;
  DeleteCriticalSection(&mutex_);
}

/* ------------------------------------------------------------------------- */
void OTC_Mutex::lock() const
{
  OTC_Mutex* self = (OTC_Mutex*)this;

  if (self->valid_ == 0)
    return;

  EnterCriticalSection(&self->mutex_);
}

/* ------------------------------------------------------------------------- */
void OTC_Mutex::unlock() const
{
  OTC_Mutex* self = (OTC_Mutex*)this;

  if (self->valid_ == 0)
    return;

  LeaveCriticalSection(&self->mutex_);
}
#endif

/* ------------------------------------------------------------------------- */
