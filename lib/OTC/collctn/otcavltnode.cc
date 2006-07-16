/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcavltnode.cc
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
#pragma implementation "OTC/collctn/avltnode.hh"
#endif
#endif

#include <OTC/collctn/avltnode.hh>

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_AVLTreeNode::typespec()
{
  static os_typespec ts("OTC_AVLTreeNode");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_AVLTreeNode::~OTC_AVLTreeNode()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_AVLTreeNode::OTC_AVLTreeNode()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
