/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcanchor.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1996-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/anchor.hh"
#endif
#endif

#include <OTC/collctn/anchor.hh>

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_Anchor::typespec()
{
  static os_typespec ts("OTC_Anchor");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_Anchor::~OTC_Anchor()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
