/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     ostore/otclocality.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/ostore/locality.hh>

#if defined(ENV_OSTORE)

#if OS_MAJOR_RELEASE < 6
/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_Locality::clustersDisabled_ = OTCLIB_FALSE;

/* ------------------------------------------------------------------------- */
OTC_Locality OTC_Locality::of(void const* theMem)
{
  // Check if memory is persistent first.
  // This check is apparently quick and
  // since in most cases memory will not be
  // persistent, better than doing explicit
  // database checks first.

  if (!objectstore::is_persistent(theMem))
    return OTC_Locality(os_segment::get_transient_segment());

  // Check storage managers.

  OTC_Storage* theStorage;
  theStorage = OTC_Storage::first();
  while (theStorage != 0)
  {
    if (theStorage->manages(theMem))
      return theStorage;
    theStorage = theStorage->next();
  }

  // Checks clusters if enabled.

  if (clustersDisabled_ == OTCLIB_FALSE)
  {
    os_object_cluster* theCluster;
    theCluster = os_object_cluster::of(theMem);
    if (theCluster != 0)
      return theCluster;
  }

  // Check segment.

  return OTC_Locality(os_segment::of(theMem));
}

/* ------------------------------------------------------------------------- */
void* operator new(
 size_t theSize,
 OTC_Locality const& theLocality,
 os_typespec* theTypespec
)
{
  if (theLocality.isStorage())
    return theLocality.storage()->allocate(theSize,theTypespec);

  if (theLocality.isCluster())
    return ::operator new (theSize,theLocality.cluster(),theTypespec);

  return ::operator new (theSize,theLocality.segment(),theTypespec);
}

/* ------------------------------------------------------------------------- */
void* operator new _OS_VECTOR(
 size_t theSize,
 OTC_Locality const& theLocality,
 os_typespec* theTypespec,
 os_int32 theNum
)
{
  if (theLocality.isStorage())
    return theLocality.storage()->allocate(theSize,theTypespec,theNum);

  if (theLocality.isCluster())
  {
    return ::operator new _OS_VECTOR(theSize,
     theLocality.cluster(),theTypespec,theNum);
  }

  return ::operator new _OS_VECTOR(theSize,
   theLocality.segment(),theTypespec,theNum);
}
#else

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_Locality::operator==(OTC_Locality const& theLocality) const
{
  os_segment* lhs;
  os_segment* rhs;

  lhs = os_segment::of(object_);
  rhs = os_segment::of(theLocality.object_);

  OTC_Boolean theResult;
  theResult = (rhs == lhs);

  lhs->release_pointer();
  rhs->release_pointer();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_Locality::isTransientSegment() const
{
  os_segment* seg;
  seg = os_segment::of(object_);
 
  OTC_Boolean theResult;
  theResult = seg->is_transient_segment();
   
  seg->release_pointer();
   
  return theResult;
}

/* ------------------------------------------------------------------------- */
void* operator new(
 size_t theSize,
 OTC_Locality const& theLocality,
 os_typespec* theTypespec
)
{
  return ::operator new (theSize,
   os_cluster::with(theLocality.object()),theTypespec);
}

/* ------------------------------------------------------------------------- */
void* operator new _OS_VECTOR(
 size_t theSize,
 OTC_Locality const& theLocality,
 os_typespec* theTypespec,
 os_int32 theNum
)
{
  return ::operator new _OS_VECTOR(theSize,
   os_cluster::with(theLocality.object()),theTypespec,theNum);
}
#endif

/* ------------------------------------------------------------------------- */

#endif
