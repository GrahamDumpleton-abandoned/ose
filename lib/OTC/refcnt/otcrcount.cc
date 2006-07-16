/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     refcnt/otcrcount.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/refcnt/rcount.hh"
#endif
#endif

#include <OTC/refcnt/rcount.hh>

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_RCount::typespec()
{
  static os_typespec ts("OTC_RCount");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_RCount::~OTC_RCount()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
