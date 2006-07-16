/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcrngecurs.cc
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
#pragma implementation "OTC/collctn/rngecurs.hh"
#endif
#endif

#include <OTC/collctn/rngecurs.hh>

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_RangeCursor::typespec()
{
  static os_typespec ts("OTC_RangeCursor");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_RangeCursor::~OTC_RangeCursor()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_RangeCursor::OTC_RangeCursor(
 int theLower,
 u_int theLength,
 OTC_Direction theDirection
)
  : lower_(theLower), upper_(theLower+theLength), direction_(theDirection)
{
  reset();
}

/* ------------------------------------------------------------------------- */
void OTC_RangeCursor::reset()
{
  if (direction_ == OTCLIB_FORWARD)
    current_ = lower_;
  else
    current_ = upper_ - 1;
}

/* ------------------------------------------------------------------------- */
void OTC_RangeCursor::next()
{
  if (current_ >= lower_ && current_ < upper_)
  {
    if (direction_ == OTCLIB_FORWARD)
      current_++;
    else
      current_--;
  }
}

/* ------------------------------------------------------------------------- */
int& OTC_RangeCursor::item()
{
  OTCLIB_ENSURE_FN((isValid()),
   "OTC_RangeCursor::item()",
   "no item under cursor");

  return current_;
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_RangeCursor::isValid() const
{
  return current_ >= lower_ && current_ < upper_;
}

/* ------------------------------------------------------------------------- */
OTC_Cursor<int>* OTC_RangeCursor::clone()
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

  OTC_RangeCursor* theCursor;
#if defined(ENV_OSTORE)
  theCursor = new (theLocality,OTC_RangeCursor::get_os_typespec())
   OTC_RangeCursor(*this);
#else
  theCursor = new OTC_RangeCursor(*this);
#endif
  OTCLIB_ASSERT_M(theCursor != 0);
  return theCursor;
}

/* ------------------------------------------------------------------------- */
OTC_RangeCursor::OTC_RangeCursor(OTC_RangeCursor const& theCursor)
  : lower_(theCursor.lower_),
    upper_(theCursor.upper_),
    direction_(theCursor.direction_),
    current_(theCursor.current_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
