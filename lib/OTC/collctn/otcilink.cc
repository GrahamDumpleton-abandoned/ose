/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcilink.cc
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
#pragma implementation "OTC/collctn/ilink.hh"
#endif
#endif

#include <OTC/collctn/ilink.hh>

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_ILink::typespec()
{
  static os_typespec ts("OTC_ILink");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_ILink::~OTC_ILink()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_ILink::OTC_ILink(int theItem)
  : item_(theItem)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_ILink::OTC_ILink(OTC_ILink const& theLink)
  : item_(theLink.item_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
