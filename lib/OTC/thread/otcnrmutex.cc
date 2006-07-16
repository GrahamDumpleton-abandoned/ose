/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/otcnrmutex.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/thread/nrmutex.hh"
#endif
#endif

#include <OTC/thread/nrmutex.hh>

#if defined(HAVE_SOLARIS_THREADS)
/* ------------------------------------------------------------------------- */
OTC_NRMutex::OTC_NRMutex()
{
  mutex_init(&mutex_,USYNC_THREAD,0);
  valid_ = 1;
}

/* ------------------------------------------------------------------------- */
OTC_NRMutex::~OTC_NRMutex()
{
  valid_ = 0;
  mutex_destroy(&mutex_);
}

/* ------------------------------------------------------------------------- */
void OTC_NRMutex::lock() const
{
  OTC_NRMutex* self = (OTC_NRMutex*)this;

  if (self->valid_ == 0)
    return;

  mutex_lock(&self->mutex_);
}

/* ------------------------------------------------------------------------- */
void OTC_NRMutex::unlock() const
{
  OTC_NRMutex* self = (OTC_NRMutex*)this;

  if (self->valid_ == 0)
    return;

  mutex_unlock(&self->mutex_);
}
#endif

#if defined(HAVE_POSIX_THREADS)
/* ------------------------------------------------------------------------- */
OTC_NRMutex::OTC_NRMutex()
{
#if defined(MUTEX_RECURSIVE_NP)
  pthread_mutex_init(&mutex_,pthread_mutexattr_default);
#else
  pthread_mutex_init(&mutex_,0);
#endif
  valid_ = 1;
}

/* ------------------------------------------------------------------------- */
OTC_NRMutex::~OTC_NRMutex()
{
  valid_ = 0;
  pthread_mutex_destroy(&mutex_);
}

/* ------------------------------------------------------------------------- */
void OTC_NRMutex::lock() const
{
  OTC_NRMutex* self = (OTC_NRMutex*)this;

  if (self->valid_ == 0)
    return;

  pthread_mutex_lock(&self->mutex_);
}

/* ------------------------------------------------------------------------- */
void OTC_NRMutex::unlock() const
{
  OTC_NRMutex* self = (OTC_NRMutex*)this;

  if (self->valid_ == 0)
    return;

  pthread_mutex_unlock(&self->mutex_);
}
#endif

#if defined(HAVE_WIN32_THREADS)
/* ------------------------------------------------------------------------- */
OTC_NRMutex::OTC_NRMutex()
{
  mutex_ = CreateMutex(0,FALSE,0);
  valid_ = 1;
}

/* ------------------------------------------------------------------------- */
OTC_NRMutex::~OTC_NRMutex()
{
  valid_ = 0;
  CloseHandle(mutex_);
}

/* ------------------------------------------------------------------------- */
void OTC_NRMutex::lock() const
{
  OTC_NRMutex* self = (OTC_NRMutex*)this;

  if (self->valid_ == 0)
    return;

  WaitForSingleObject(self->mutex_,INFINITE);
}

/* ------------------------------------------------------------------------- */
void OTC_NRMutex::unlock() const
{
  OTC_NRMutex* self = (OTC_NRMutex*)this;

  if (self->valid_ == 0)
    return;

  ReleaseMutex(self->mutex_);
}
#endif

/* ------------------------------------------------------------------------- */
