/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcrange.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/range.hh"
#endif
#endif

#include <OTC/collctn/range.hh>
#include <OTC/collctn/rngecurs.hh>

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_Range::typespec()
{
  static os_typespec ts("OTC_Range");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_Range::isSubRange(OTC_Range const& theRange) const
{
  if (&theRange == this || isNullRange())
    return OTCLIB_TRUE;
  
  if (theRange.isNullRange())
    return OTCLIB_FALSE;

  if (
   lower() >= theRange.lower()
   &&
   lower() < theRange.upper()
   &&
   upper() > theRange.lower()
   &&
   upper() <= theRange.upper()
  )
  {
    return OTCLIB_TRUE;
  }

  return OTCLIB_FALSE;
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<int> OTC_Range::indices(OTC_Direction theDirection) const
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
  if (!theLocality.isTransientSegment())
  {
    os_transaction* theTXN = os_transaction::get_current();
    if (theTXN != 0 && theTXN->get_type() == os_transaction::read_only)
      theLocality = os_segment::get_transient_segment();
  }
#endif

  OTC_RangeCursor* theCursor;
#if defined(ENV_OSTORE)
  theCursor = new (theLocality,OTC_RangeCursor::get_os_typespec())
   OTC_RangeCursor(lower_,length_,theDirection);
#else
  theCursor = new OTC_RangeCursor(lower_,length_,theDirection);
#endif
  OTCLIB_ASSERT_M(theCursor != 0);
  return theCursor;
}

/* ------------------------------------------------------------------------- */
