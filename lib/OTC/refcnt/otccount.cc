/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     refcnt/otccount.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/refcnt/count.hh"
#endif
#endif

#include <OTC/refcnt/count.hh>

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_Count::typespec()
{
  static os_typespec ts("OTC_Count");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_Count::~OTC_Count()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
