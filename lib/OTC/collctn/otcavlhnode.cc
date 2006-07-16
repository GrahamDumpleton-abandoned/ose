/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcavlhnode.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/avlhnode.hh"
#endif
#endif

#include <OTC/collctn/avlhnode.hh>

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_AVLHashTableNode::typespec()
{
  static os_typespec ts("OTC_AVLHashTableNode");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_AVLHashTableNode::~OTC_AVLHashTableNode()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_AVLHashTableNode::OTC_AVLHashTableNode(
 u_int theStartSize,
 u_int theFloorSize,
 u_int theLowThreshold,
 u_int theHighThreshold
)
  : index_(theStartSize,theFloorSize,theLowThreshold,theHighThreshold)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
