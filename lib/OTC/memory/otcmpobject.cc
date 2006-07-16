/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/otcmpobject.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/memory/mpobject.hh"
#endif
#endif

#include <OTC/memory/mpobject.hh>
#include <OTC/memory/cmmnpool.hh>

/* ------------------------------------------------------------------------- */
OTC_MPObject::~OTC_MPObject()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_MPObject::OTC_MPObject()
{
  // Nothing to do.
}

#if !defined(ENV_NOCOMMONPOOL)
/* ------------------------------------------------------------------------- */
void* OTC_MPObject::operator new(size_t theSize)
{
  return OTC_CommonPool::allocate(theSize);
}

/* ------------------------------------------------------------------------- */
void OTC_MPObject::operator delete(void* theMemory, size_t theSize)
{
#if defined(ENV_OSTORE)
  if (os_segment::of(theMemory) != os_segment::get_transient_segment())
  {
    ::operator delete(theMemory);
    return;
  }
#endif

  OTC_CommonPool::release(theMemory,theSize);
}
#endif

#if defined(ENV_OSTORE) && !defined(ENV_NOCOMMONPOOL)
/* ------------------------------------------------------------------------- */
void* OTC_MPObject::operator new(
 size_t theSize,
 os_segment* theSegment,
 os_typespec* theTypeSpec
)
{
  if (theSegment == os_segment::get_transient_segment())
    return OTC_CommonPool::allocate(theSize);

  return ::operator new(theSize,theSegment,theTypeSpec);
}

/* ------------------------------------------------------------------------- */
void* OTC_MPObject::operator new(
 size_t theSize,
 os_database* theDatabase,
 os_typespec* theTypeSpec
)
{
  return ::operator new(theSize,theDatabase,theTypeSpec);
}

#if OS_MAJOR_RELEASE < 6
/* ------------------------------------------------------------------------- */
void* OTC_MPObject::operator new(
 size_t theSize,
 os_object_cluster* theCluster,
 os_typespec* theTypeSpec
)
{
  return ::operator new(theSize,theCluster,theTypeSpec);
}
#else
/* ------------------------------------------------------------------------- */
void* OTC_MPObject::operator new(
 size_t theSize,
 os_cluster* theCluster,
 os_typespec* theTypeSpec
)
{
  return ::operator new(theSize,theCluster,theTypeSpec);
}

/* ------------------------------------------------------------------------- */
void* OTC_MPObject::operator new(
 size_t theSize,
 os_cluster_with theHandle,
 os_typespec* theTypeSpec
)
{
  return ::operator new(theSize,theHandle,theTypeSpec);
}
#endif

/* ------------------------------------------------------------------------- */
void* OTC_MPObject::operator new(
 size_t theSize,
 OTC_Locality const& theLocality,
 os_typespec* theTypeSpec
)
{
  if (theLocality.isTransientSegment())
    return OTC_CommonPool::allocate(theSize);

  return ::operator new(theSize,theLocality,theTypeSpec);
}
#endif

/* ------------------------------------------------------------------------- */
