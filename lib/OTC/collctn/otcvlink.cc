/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcvlink.cc
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
#pragma implementation "OTC/collctn/vlink.hh"
#endif
#endif

#include <OTC/collctn/vlink.hh>

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_VLink::typespec()
{
  static os_typespec ts("OTC_VLink");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_VLink::~OTC_VLink()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_VLink::OTC_VLink(void* theItem)
  : item_(theItem)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_VLink::OTC_VLink(OTC_VLink const& theLink)
  : item_(theLink.item_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
