/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     SCHEMA.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ostore/ostore.hh>
#include <ostore/manschem.hh>

#include <OTC/memory/arena.hh>
#include <OTC/collctn/avllnode.hh>
#include <OTC/collctn/avlnode.hh>
#include <OTC/collctn/avltree.hh>
#include <OTC/collctn/anchor.hh>
#include <OTC/collctn/bucket.hh>
#include <OTC/refcnt/count.hh>
#include <OTC/collctn/cursor.hh>
#include <OTC/collctn/hashntry.hh>
#include <OTC/collctn/hashtble.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/kcursor.hh>
#include <OTC/collctn/kicursor.hh>
#include <OTC/collctn/iilink.hh>
#include <OTC/dispatch/job.hh>
#include <OTC/collctn/linkiter.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/linkable.hh>
#include <OTC/memory/mpobject.hh>
#include <OTC/collctn/prbucket.hh>
#include <OTC/refcnt/rcount.hh>
#include <OTC/collctn/rngecurs.hh>
#include <OTC/refcnt/resource.hh>
#include <OTC/text/regexp.hh>
#include <OTC/text/string.hh>
#include <OTC/collctn/hindex.hh>
#include <OTC/message/exchange.hh>

/* ------------------------------------------------------------------------- */

void dummy()
{
#if OS_MAJOR_RELEASE < 6

  OS_MARK_SCHEMA_TYPE((OTC_AVLLinkNode));
  OS_MARK_SCHEMA_TYPE((OTC_AVLNode));
  OS_MARK_SCHEMA_TYPE((OTC_AVLTree));
  OS_MARK_SCHEMA_TYPE((OTC_Arena));
  OS_MARK_SCHEMA_TYPE((OTC_ArenaBlock));
  OS_MARK_SCHEMA_TYPE((OTC_Anchor));
  OS_MARK_SCHEMA_TYPE((OTC_Count));
  OS_MARK_SCHEMA_TYPE((OTC_HashEntry));
  OS_MARK_SCHEMA_TYPE((OTC_HashTable));
  OS_MARK_SCHEMA_TYPE((OTC_Job));
  OS_MARK_SCHEMA_TYPE((OTC_Link));
  OS_MARK_SCHEMA_TYPE((OTC_LinkCursor));
  OS_MARK_SCHEMA_TYPE((OTC_LinkIterator));
  OS_MARK_SCHEMA_TYPE((OTC_LinkList));
  OS_MARK_SCHEMA_TYPE((OTC_Linkable));
  OS_MARK_SCHEMA_TYPE((OTC_MPObject));
  OS_MARK_SCHEMA_TYPE((OTC_RCount));
  OS_MARK_SCHEMA_TYPE((OTC_RString));
  OS_MARK_SCHEMA_TYPE((OTC_RangeCursor));
  OS_MARK_SCHEMA_TYPE((OTC_RegexpStruct));
  OS_MARK_SCHEMA_TYPE((OTC_Resource));
  OS_MARK_SCHEMA_TYPE((OTC_String));
  OS_MARK_SCHEMA_TYPE((OTC_StringData));

  OS_MARK_SCHEMA_TYPE((OTC_Bucket<OTC_EPListener*>));
  OS_MARK_SCHEMA_TYPE((OTC_Bucket<OTC_ServiceBinding*>));
  OS_MARK_SCHEMA_TYPE((OTC_Bucket<OTC_String>));
  OS_MARK_SCHEMA_TYPE((OTC_Cursor<int>));
  OS_MARK_SCHEMA_TYPE((OTC_HIndexEntry<OTC_String>));
  OS_MARK_SCHEMA_TYPE((OTC_ItemCursor<int,OTC_IILink>));
  OS_MARK_SCHEMA_TYPE((OTC_KeyCursor<int,OTC_IILink>));
  OS_MARK_SCHEMA_TYPE((OTC_KeyItemCursor<int,int,OTC_IILink>));
  OS_MARK_SCHEMA_TYPE((OTC_PairBucket<OTC_String,int>));
  OS_MARK_SCHEMA_TYPE((OTC_PairBucket<OTC_String,OTC_Exchange*>));
  OS_MARK_SCHEMA_TYPE((OTC_PairBucket<OTC_String,OTC_EndPoint*>));
  OS_MARK_SCHEMA_TYPE((OTC_PairBucket<OTC_String,OTC_EPRegistry*>));
  OS_MARK_SCHEMA_TYPE((OTC_PairBucket<OTC_String,OTC_EPServer*>));
  OS_MARK_SCHEMA_TYPE((OTC_PairBucket<OTC_String,OTC_Receiver*>));
  OS_MARK_SCHEMA_TYPE((OTC_PairBucket<OTC_String,OTC_ServiceBinding*>));
  OS_MARK_SCHEMA_TYPE((OTC_PairBucket<OTC_String,OTC_String>));
  OS_MARK_SCHEMA_TYPE((OTC_PairCursor<int,int>));

#else

  OS_MARK_SCHEMA_TYPE(OTC_AVLLinkNode);
  OS_MARK_SCHEMA_TYPE(OTC_AVLNode);
  OS_MARK_SCHEMA_TYPE(OTC_AVLTree);
  OS_MARK_SCHEMA_TYPE(OTC_Arena);
  OS_MARK_SCHEMA_TYPE(OTC_ArenaBlock);
  OS_MARK_SCHEMA_TYPE(OTC_Anchor);
  OS_MARK_SCHEMA_TYPE(OTC_Count);
  OS_MARK_SCHEMA_TYPE(OTC_HashEntry);
  OS_MARK_SCHEMA_TYPE(OTC_HashTable);
  OS_MARK_SCHEMA_TYPE(OTC_Job);
  OS_MARK_SCHEMA_TYPE(OTC_Link);
  OS_MARK_SCHEMA_TYPE(OTC_LinkCursor);
  OS_MARK_SCHEMA_TYPE(OTC_LinkIterator);
  OS_MARK_SCHEMA_TYPE(OTC_LinkList);
  OS_MARK_SCHEMA_TYPE(OTC_Linkable);
  OS_MARK_SCHEMA_TYPE(OTC_MPObject);
  OS_MARK_SCHEMA_TYPE(OTC_RCount);
  OS_MARK_SCHEMA_TYPE(OTC_RString);
  OS_MARK_SCHEMA_TYPE(OTC_RangeCursor);
  OS_MARK_SCHEMA_TYPE(OTC_RegexpStruct);
  OS_MARK_SCHEMA_TYPE(OTC_Resource);
  OS_MARK_SCHEMA_TYPE(OTC_String);
  OS_MARK_SCHEMA_TYPE(OTC_StringData);

  OS_MARK_SCHEMA_TYPESPEC((OTC_Bucket<OTC_EPListener*>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_Bucket<OTC_ServiceBinding*>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_Bucket<OTC_String>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_Cursor<int>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_HIndexEntry<OTC_String>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_ItemCursor<int,OTC_IILink>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_KeyCursor<int,OTC_IILink>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_KeyItemCursor<int,int,OTC_IILink>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_PairBucket<OTC_String,int>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_PairBucket<OTC_String,OTC_Exchange*>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_PairBucket<OTC_String,OTC_EndPoint*>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_PairBucket<OTC_String,OTC_EPRegistry*>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_PairBucket<OTC_String,OTC_EPServer*>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_PairBucket<OTC_String,OTC_Receiver*>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_PairBucket<OTC_String,OTC_ServiceBinding*>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_PairBucket<OTC_String,OTC_String>));
  OS_MARK_SCHEMA_TYPESPEC((OTC_PairCursor<int,int>));

#endif
}

/* ------------------------------------------------------------------------- */
