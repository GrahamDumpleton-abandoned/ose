/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otclinkcurs.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1995-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/linkcurs.hh"
#endif
#endif

#include <OTC/collctn/linkcurs.hh>
#include <OTC/collctn/linklist.hh>

/* ------------------------------------------------------------------------- */
OTC_LinkCursor::~OTC_LinkCursor()
{
  if (protection_ == OTCLIB_SAFE)
    list_->unReference();

  delete iter_;
}

/* ------------------------------------------------------------------------- */
OTC_LinkCursor::OTC_LinkCursor(
 OTC_LinkList* theList,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
  : list_(theList), iter_(0), direction_(theDirection),
    protection_(theProtection)
{
  OTCLIB_ASSERT(list_ != 0);

  OTC_LinkIterator const& tmpIter = list_->items(theProtection);

#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

#if defined(ENV_OSTORE)
  iter_ = new (theLocality,OTC_LinkIterator::get_os_typespec())
   OTC_LinkIterator(tmpIter);
#else
  iter_ = new OTC_LinkIterator(tmpIter);
#endif
  OTCLIB_ASSERT_M(iter_ != 0);

  if (protection_ == OTCLIB_SAFE)
    list_->reference();

  reset();
}

/* ------------------------------------------------------------------------- */
OTC_LinkCursor::OTC_LinkCursor(
 OTC_LinkList* theList,
 OTC_Linkable* theStart,
 OTC_Linkable* theEnd,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
  : list_(theList), iter_(0), direction_(theDirection),
    protection_(theProtection)
{
  OTCLIB_ASSERT(list_ != 0);

#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

#if defined(ENV_OSTORE)
  iter_ = new (theLocality,OTC_LinkIterator::get_os_typespec())
   OTC_LinkIterator(theStart,theEnd,theProtection);
#else
  iter_ = new OTC_LinkIterator(theStart,theEnd,theProtection);
#endif
  OTCLIB_ASSERT_M(iter_ != 0);

  if (protection_ == OTCLIB_SAFE)
    list_->reference();

  reset();
}

/* ------------------------------------------------------------------------- */
OTC_LinkCursor::OTC_LinkCursor(
 OTC_LinkCursor const& theIter
)
  : list_(theIter.list_), iter_(0), direction_(theIter.direction_),
    protection_(theIter.protection_)
{
  OTCLIB_ASSERT(list_ != 0);

#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

#if defined(ENV_OSTORE)
  iter_ = new (theLocality,OTC_LinkIterator::get_os_typespec())
   OTC_LinkIterator(*theIter.iter_);
#else
  iter_ = new OTC_LinkIterator(*theIter.iter_);
#endif
  OTCLIB_ASSERT_M(iter_ != 0);

  if (protection_ == OTCLIB_SAFE)
    list_->reference();
}

/* ------------------------------------------------------------------------- */
