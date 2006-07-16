/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otciilink.cc
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
#pragma implementation "OTC/collctn/iilink.hh"
#endif
#endif

#include <OTC/collctn/iilink.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/kcursor.hh>
#include <OTC/collctn/kicursor.hh>

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_IILink::typespec()
{
  static os_typespec ts("OTC_IILink");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_IILink::~OTC_IILink()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_IILink::OTC_IILink(int theKey, int theItem)
  : key_(theKey), item_(theItem)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_IILink::OTC_IILink(OTC_IILink const& theLink)
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

  OTC_Cursor<int>* theCursor;
  theCursor = new OTC_ItemCursor<int,OTC_IILink>(theList);
  delete theCursor;
  theCursor = new OTC_KeyCursor<int,OTC_IILink>(theList);
  delete theCursor;

  OTC_PairCursor<int,int>* thePairCursor;
  thePairCursor = new OTC_KeyItemCursor<int,int,OTC_IILink>(theList);
  delete thePairCursor;
}
#endif

/* ------------------------------------------------------------------------- */
