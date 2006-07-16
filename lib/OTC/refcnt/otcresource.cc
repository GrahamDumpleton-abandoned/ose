/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     refcnt/otcresource.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/refcnt/resource.hh"
#endif
#endif

#include <OTC/refcnt/resource.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_Resource::mutex_;
OTC_Boolean OTC_Resource::globAllocatedUsingNew_ = OTCLIB_FALSE;
OTC_Boolean OTC_Resource::globInitialised_ = OTCLIB_FALSE;
OTC_Boolean OTC_Resource::globResourceWarnings_ = OTCLIB_FALSE;

/* ------------------------------------------------------------------------- */
OTC_Resource::OTC_Resource()
  : refCount_(0)
{
#if !defined(ENV_OSTORE) && !defined(ENV_THREADS)

  if (globInitialised_ == OTCLIB_FALSE)
  {
    globInitialised_ = OTCLIB_TRUE;
    char* env = getenv("OTCLIB_RESOURCEWARNINGS");
    if (env != 0)
      globResourceWarnings_ = OTCLIB_TRUE;
  }

  allocatedUsingNew_ = globAllocatedUsingNew_;
  globAllocatedUsingNew_ = OTCLIB_FALSE;
#endif
}

/* ------------------------------------------------------------------------- */
OTC_Resource::~OTC_Resource()
{
  OTCLIB_ENSURE_FN((numRefs() == 0),
   "OTC_Resource::~OTC_Resource()",
   "object still referenced");
}

/* ------------------------------------------------------------------------- */
u_int OTC_Resource::numRefs() const
{
  OTC_Resource* theSelf;
  theSelf = (OTC_Resource*)this;

  return otclib_atomic_get(theSelf->refCount_,mutex_);
}

/* ------------------------------------------------------------------------- */
u_int OTC_Resource::reference() const
{
#if !defined(ENV_OSTORE) && !defined(ENV_THREADS)
  if (globResourceWarnings_ != OTCLIB_FALSE)
  {
    OTCLIB_ENSURE_FN_W((allocatedUsingNew_ != OTCLIB_FALSE),
     "OTC_Resource::reference()",
     "object not allocated using new");
  }
#endif

  OTC_Resource* theSelf;
  theSelf = (OTC_Resource*)this;

  return otclib_atomic_inc(theSelf->refCount_,mutex_);
}

/* ------------------------------------------------------------------------- */
u_int OTC_Resource::unReference() const
{
#if !defined(ENV_OSTORE) && !defined(ENV_THREADS)
  if (globResourceWarnings_ != OTCLIB_FALSE)
  {
    if (allocatedUsingNew_ == OTCLIB_FALSE)
    {
      OTCLIB_ENSURE_FN_W((allocatedUsingNew_ != OTCLIB_FALSE),
       "OTC_Resource::unReference()",
       "object not allocated using new");
    }
  }
#endif

  OTCLIB_ENSURE_FN((numRefs() != 0),
   "OTC_Resource::unReference()",
   "object not referenced");

  u_int theResult;

  OTC_Resource* theSelf;
  theSelf = (OTC_Resource*)this;

  theResult = otclib_atomic_dec(theSelf->refCount_,mutex_);

  if (theResult == 0)
    delete (OTC_Resource*)this;

  return theResult;
}

#if !defined(ENV_OSTORE) && !defined(ENV_THREADS)
/* ------------------------------------------------------------------------- */
void* OTC_Resource::operator new(size_t theSize)
{
  doInOperatorNew();

#if defined(ENV_NOCOMMONPOOL)
  return ::operator new(theSize);
#else
  return OTC_MPObject::operator new(theSize);
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_Resource::operator delete(void* theMem, size_t theSz)
{
#if defined(ENV_NOCOMMONPOOL)
  ::operator delete(theMem);
#else
  OTC_MPObject::operator delete(theMem,theSz);
#endif
}
#endif

/* ------------------------------------------------------------------------- */
