/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcivlink.cc
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
#pragma implementation "OTC/collctn/ivlink.hh"
#endif
#endif

#include <OTC/collctn/ivlink.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/kcursor.hh>
#include <OTC/collctn/kicursor.hh>

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_IVLink::typespec()
{
  static os_typespec ts("OTC_IVLink");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_IVLink::~OTC_IVLink()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_IVLink::OTC_IVLink(int theKey, void* theItem)
  : key_(theKey), item_(theItem)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_IVLink::OTC_IVLink(OTC_IVLink const& theLink)
  : key_(theLink.key_), item_(theLink.item_)
{
  // Nothing to do.
}

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
static void bogus()
{
  // This function exists so as to trick the Sun C++ CAFE compiler
  // and Micro$oft compilers into generating templates for the listed
  // template cursor classes.

  OTC_LinkList* theList;

  OTC_Cursor<void*>* theItemCursor;
  theItemCursor = new OTC_ItemCursor<void*,OTC_IVLink>(theList);
  delete theItemCursor;
  OTC_Cursor<int>* theKeyCursor;
  theKeyCursor = new OTC_KeyCursor<int,OTC_IVLink>(theList);
  delete theKeyCursor;

  OTC_PairCursor<int,void*>* thePairCursor;
  thePairCursor = new OTC_KeyItemCursor<int,void*,OTC_IVLink>(theList);
  delete thePairCursor;
}
#endif

/* ------------------------------------------------------------------------- */
