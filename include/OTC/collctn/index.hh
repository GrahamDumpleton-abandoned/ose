#ifndef OTC_COLLCTN_INDEX_HH
#define OTC_COLLCTN_INDEX_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/index.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/tindex.hh>

/* ------------------------------------------------------------------------- */

#define OTC_Index OTC_TIndex
#define OTC_IndexNode OTC_TIndexEntry

// [OTC_Index]
//
// = TITLE
//     Class implementing an index.
//
// = SYNOPSIS
//     #define OTC_Index OTC_TIndex
//     #define OTC_IndexNode OTC_TIndexEntry
//
// = DESCRIPTION
//     The <OTC_Index> class is now obsolete. You should instead use
//     the <OTC_TIndex> class. A macro is provided for <OTC_Index> so
//     existing code will automatically start using the <OTC_TIndex>
//     class. Note that the names of some functions changed in
//     <OTC_Index>. The old names are kept for backward compatability
//     but a change to new names and the new way of deleting nodes should
//     be made.
//
// = SEE ALSO
//     <OTC_TIndex>, <OTC_TIndexEntry>

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_INDEX_HH */
