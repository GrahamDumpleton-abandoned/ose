/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcsilink.cc
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
#pragma implementation "OTC/text/silink.hh"
#endif
#endif

#include <OTC/text/silink.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/kcursor.hh>
#include <OTC/collctn/kicursor.hh>

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_SILink::typespec()
{
  static os_typespec ts("OTC_SILink");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_SILink::~OTC_SILink()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_SILink::OTC_SILink(OTC_String const& theKey, int theItem)
  : key_(theKey), item_(theItem)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_SILink::OTC_SILink(OTC_SILink const& theLink)
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

  OTC_Cursor<int>* theItemCursor;
  theItemCursor = new OTC_ItemCursor<int,OTC_SILink>(theList);
  delete theItemCursor;
  OTC_Cursor<OTC_String>* theKeyCursor;
  theKeyCursor = new OTC_KeyCursor<OTC_String,OTC_SILink>(theList);
  delete theKeyCursor;

  OTC_PairCursor<OTC_String,int>* thePairCursor;
  thePairCursor = new OTC_KeyItemCursor<OTC_String,int,OTC_SILink>(theList);
  delete thePairCursor;
}
#endif

/* ------------------------------------------------------------------------- */
