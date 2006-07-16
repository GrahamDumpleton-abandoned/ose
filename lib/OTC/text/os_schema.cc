/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/os_schema.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1995-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ostore/ostore.hh>
#include <ostore/manschem.hh>

#include <OTC/collctn/bucket.hh>
#include <OTC/collctn/icursor.hh>

#include <OTC/text/string.hh>
#include <OTC/text/symbol.hh>
#include <OTC/collctn/iilink.hh>

/* ------------------------------------------------------------------------- */

void dummy()
{
  OS_MARK_SCHEMA_TYPE((OTC_Bucket<OTC_String>));
  OS_MARK_SCHEMA_TYPE((OTC_Bucket<OTC_Symbol>));
  OS_MARK_SCHEMA_TYPE((OTC_ItemCursor<int,OTC_IILink>));
}

/* ------------------------------------------------------------------------- */
