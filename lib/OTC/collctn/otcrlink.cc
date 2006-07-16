/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcrlink.cc
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
#pragma implementation "OTC/collctn/rlink.hh"
#endif
#endif

#include <OTC/collctn/rlink.hh>

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_RLink::typespec()
{
  static os_typespec ts("OTC_RLink");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_RLink::~OTC_RLink()
{
  if (item_ != 0)
    item_->unReference();
}

/* ------------------------------------------------------------------------- */
OTC_RLink::OTC_RLink(OTC_Resource* theItem)
  : item_(theItem)
{
  if (item_ != 0)
    item_->reference();
}

/* ------------------------------------------------------------------------- */
OTC_RLink::OTC_RLink(OTC_RLink const& theLink)
  : item_(theLink.item_)
{
  if (item_ != 0)
    item_->reference();
}

/* ------------------------------------------------------------------------- */
