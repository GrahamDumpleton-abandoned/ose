/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/os_schema.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ostore/ostore.hh>
#include <ostore/manschem.hh>

#include "xxxitem1.hh"
#include "xxxitem2.hh"

#include <OTC/collctn/bucket.hh>
#include <OTC/collctn/holder.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/kcursor.hh>
#include <OTC/collctn/kicursor.hh>
#include <OTC/collctn/prbucket.hh>
#include <OTC/collctn/hashntry.hh>
#include <OTC/collctn/iilink.hh>

/* ------------------------------------------------------------------------- */

void dummy()
{
  OS_MARK_SCHEMA_TYPE((OTC_HashEntry));

  OS_MARK_SCHEMA_TYPE((OTC_Bucket<int>));
  OS_MARK_SCHEMA_TYPE((OTC_Bucket<TST_Item1>));
  OS_MARK_SCHEMA_TYPE((OTC_Bucket<TST_Item2*>));
  OS_MARK_SCHEMA_TYPE((OTC_Holder<int>));
  OS_MARK_SCHEMA_TYPE((OTC_Holder<TST_Item1>));
  OS_MARK_SCHEMA_TYPE((OTC_ItemCursor<int,OTC_IILink>));
  OS_MARK_SCHEMA_TYPE((OTC_KeyCursor<int,OTC_IILink>));
  OS_MARK_SCHEMA_TYPE((OTC_KeyItemCursor<int,int,OTC_IILink>));
  OS_MARK_SCHEMA_TYPE((OTC_PairBucket<int,double>));
  OS_MARK_SCHEMA_TYPE((OTC_PairBucket<int,int>));
}

/* ------------------------------------------------------------------------- */
