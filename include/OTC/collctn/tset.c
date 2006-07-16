#ifndef OTC_COLLCTN_TSET_C
#define OTC_COLLCTN_TSET_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/tset.c
// 
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/bucket.hh>
#include <OTC/collctn/icursor.hh>

/* ------------------------------------------------------------------------- */
template<class T>
OTC_TSet<T>::~OTC_TSet()
{
  index_->unReference();
  list_->unReference();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_TSet<T>::OTC_TSet(OTC_TSet<T> const& theSet)
  : rankFn_(theSet.rankFn_)
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

#if defined(ENV_OSTORE)
  list_ = new (theLocality,OTC_LinkList::get_os_typespec()) OTC_LinkList;
#else
  list_ = new OTC_LinkList;
#endif
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

#if defined(ENV_OSTORE)
  index_ = new (theLocality,OTC_AVLTree::get_os_typespec()) OTC_AVLTree;
#else
  index_ = new OTC_AVLTree;
#endif
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();

  OTC_Iterator<T> theItems = 0;
  theItems = theSet.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);
  for (theItems.reset(); theItems.isValid(); theItems.next())
    add(theItems.item());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_TSet<T>::OTC_TSet(int (*theRankFn)(T const&,T const&))
  : rankFn_(theRankFn)
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

#if defined(ENV_OSTORE)
  list_ = new (theLocality,OTC_LinkList::get_os_typespec()) OTC_LinkList;
#else
  list_ = new OTC_LinkList;
#endif
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

#if defined(ENV_OSTORE)
  index_ = new (theLocality,OTC_AVLTree::get_os_typespec())
OTC_AVLTree;
#else
  index_ = new OTC_AVLTree;
#endif
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_TSet<T>::OTC_TSet(OTC_TSet<T> const& theSet, OTC_ShallowCopy)
  : rankFn_(theSet.rankFn_)
{
  list_ = theSet.list_;
  list_->reference();

  index_ = theSet.index_;
  index_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_TSet<T>::removeAll()
{
  index_->removeAll();
  list_->removeAll();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_TSet<T>& OTC_TSet<T>::operator=(OTC_TSet<T> const& theSet)
{
  if (&theSet != this)
  {
    removeAll();

    rankFn_ = theSet.rankFn_;
    OTC_Iterator<T> theItems = 0;
    theItems = theSet.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);
    for (theItems.reset(); theItems.isValid(); theItems.next())
      add(theItems.item());
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_TSet<T>::add(T const& theItem, OTC_Replacement theFlag)
{
  int theRank;
  OTC_AVLLinkNode* theLinkNode;

  theLinkNode = (OTC_AVLLinkNode*)index_->root();
  theRank = searchIndex_(theLinkNode,theItem,theLinkNode);

  OTCLIB_ENSURE_FN((theRank != 0 || theFlag == OTCLIB_REPLACE),
   "OTC_TSet<T>::add(T const&, OTC_Replacement)",
   "item already exists");

#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

  OTC_Bucket<T>* theLink;
#if defined(ENV_OSTORE)
  theLink = new (theLocality,OTC_Bucket<T>::get_os_typespec())
   OTC_Bucket<T>(theItem);
#else
  theLink = new OTC_Bucket<T>(theItem);
#endif
  OTCLIB_ASSERT_M(theLink != 0);

  if (theLinkNode != 0 && theRank == 0)
  {
    theLinkNode->link()->addBefore(theLink);
    theLinkNode->link()->kill();
    theLinkNode->setLink(theLink);
  }
  else
  {
    OTC_AVLLinkNode* theNode;
#if defined(ENV_OSTORE)
    theNode = new (theLocality,OTC_AVLLinkNode::get_os_typespec())
     OTC_AVLLinkNode(theLink);
#else
    theNode = new OTC_AVLLinkNode(theLink);
#endif
    OTCLIB_ASSERT_M(theNode != 0);

    if (theLinkNode == 0)
    {
      index_->addRoot(theNode);
      list_->addLast(theLink);
    }
    else if (theRank < 0)
    {
      theLinkNode->addBefore(theNode);
      theLinkNode->link()->addBefore(theLink);
    }
    else
    {
      theLinkNode->addAfter(theNode);
      theLinkNode->link()->addAfter(theLink);
    }
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_TSet<T>::remove(T const& theItem, OTC_Protection theFlag)
{
  int theRank;
  OTC_AVLLinkNode* theNode;

  theNode = (OTC_AVLLinkNode*)index_->root();
  theRank = searchIndex_(theNode,theItem,theNode);

  OTCLIB_ENSURE_FN((theFlag == OTCLIB_UNSAFE || theRank == 0),
   "OTC_TSet<T>::remove(T const&, OTC_Protection)",
   "item doesn't exist");

  if (theRank != 0)
    return;

  OTC_Bucket<T>* theLink;
  theLink = (OTC_Bucket<T>*)theNode->link();

  theNode->unlink();
  delete theNode;

  theLink->kill();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Boolean OTC_TSet<T>::contains(T const& theItem) const
{
  OTC_AVLLinkNode* theNode;
  theNode = (OTC_AVLLinkNode*)index_->root();

  return searchIndex_(theNode,theItem,theNode) == 0;
}

/* ------------------------------------------------------------------------- */
template<class T>
T const& OTC_TSet<T>::item(T const& theItem) const
{
  OTC_AVLLinkNode* theNode;
  theNode = (OTC_AVLLinkNode*)index_->root();

  int theResult = searchIndex_(theNode,theItem,theNode);

  OTCLIB_ENSURE_FN((theResult == 0),
   "OTC_TSet<T>::item(T const&)",
   "invalid item");

  return ((OTC_Bucket<T>*)theNode->link())->item();
}

/* ------------------------------------------------------------------------- */
template<class T>
T const* OTC_TSet<T>::pItem(T const& theItem) const
{
  OTC_AVLLinkNode* theNode;
  theNode = (OTC_AVLLinkNode*)index_->root();

  int theResult = searchIndex_(theNode,theItem,theNode);
  if (theResult != 0)
    return 0;

  return &((OTC_Bucket<T>*)theNode->link())->item();
}

/* ------------------------------------------------------------------------- */
template<class T>
T const& OTC_TSet<T>::pickItem() const
{
  OTCLIB_ENSURE_FN((index_->population() != 0),
   "OTC_TSet<T>::pickItem()",
   "collection must not be empty");

  return ((OTC_Bucket<T>*)list_->first())->item();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Iterator<T> OTC_TSet<T>::items(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
  if (!theLocality.isTransientSegment())
  {
    os_transaction* theTXN = os_transaction::get_current();
    if (theTXN != 0 && theTXN->get_type() == os_transaction::read_only)
      theProtection = OTCLIB_UNSAFE;

    if (theProtection == OTCLIB_UNSAFE)
      theLocality = os_segment::get_transient_segment();
  }
#endif

  OTC_Cursor<T>* theIter;
#if defined(ENV_OSTORE)
  theIter = new (theLocality,
   OTC_ItemCursor< T,OTC_Bucket<T> >::get_os_typespec())
   OTC_ItemCursor< T,OTC_Bucket<T> >(list_,theDirection,theProtection);
#else
  theIter = new OTC_ItemCursor< T,OTC_Bucket<T> >(list_,theDirection,
   theProtection);
#endif
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Iterator<T> OTC_TSet<T>::items(
 T const& theItem,
 int (*theRankFn)(T const&,T const&),
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  int theRank;
  OTC_AVLLinkNode* tmpNode;
  OTC_AVLLinkNode* theNode;

  tmpNode = (OTC_AVLLinkNode*)index_->root();

  while (tmpNode != 0)
  {
    OTC_Bucket<T>* tmpBucket;
    tmpBucket = (OTC_Bucket<T>*)tmpNode->link();

    if (theRankFn != 0)
      theRank = (*theRankFn)(theItem,tmpBucket->item());
    else
      theRank = rank(theItem,tmpBucket->item());

    if (theRank == 0)
      break;
    else if (theRank < 0)
      tmpNode = (OTC_AVLLinkNode*)tmpNode->left();
    else
      tmpNode = (OTC_AVLLinkNode*)tmpNode->right();
  }

  if (tmpNode == 0)
    return 0;

  theNode = tmpNode;

  OTC_AVLLinkNode* tmpStartNode;
  OTC_AVLLinkNode* tmpEndNode;

  tmpStartNode = theNode;
  tmpNode = (OTC_AVLLinkNode*)theNode->prev();

  while (tmpNode != 0)
  {
    OTC_Bucket<T>* tmpBucket;
    tmpBucket = (OTC_Bucket<T>*)((OTC_AVLLinkNode*)tmpNode)->link();

    if (theRankFn != 0)
      theRank = (*theRankFn)(theItem,tmpBucket->item());
    else
      theRank = rank(theItem,tmpBucket->item());

    if (theRank != 0)
      break;

    tmpStartNode = tmpNode;
    tmpNode = (OTC_AVLLinkNode*)tmpNode->prev();
  }

  tmpEndNode = theNode;
  tmpNode = (OTC_AVLLinkNode*)theNode->next();

  while (tmpNode != 0)
  {
    OTC_Bucket<T>* tmpBucket;
    tmpBucket = (OTC_Bucket<T>*)((OTC_AVLLinkNode*)tmpNode)->link();

    if (theRankFn != 0)
      theRank = (*theRankFn)(theItem,tmpBucket->item());
    else
      theRank = rank(theItem,tmpBucket->item());

    if (theRank != 0)
      break;

    tmpEndNode = tmpNode;
    tmpNode = (OTC_AVLLinkNode*)tmpNode->next();
  }

#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
  if (!theLocality.isTransientSegment())
  {
    os_transaction* theTXN = os_transaction::get_current();
    if (theTXN != 0 && theTXN->get_type() == os_transaction::read_only)
      theProtection = OTCLIB_UNSAFE;

    if (theProtection == OTCLIB_UNSAFE)
      theLocality = os_segment::get_transient_segment();
  }
#endif

  OTC_Anchor* theStartAnchor = 0;
  OTC_Anchor* theEndAnchor = 0;

  if (theProtection == OTCLIB_SAFE)
  {
#if defined(ENV_OSTORE)
    theStartAnchor = new (theLocality,OTC_Anchor::get_os_typespec())
OTC_Anchor;
    OTCLIB_ASSERT_M(theStartAnchor != 0);
    theEndAnchor = new (theLocality,OTC_Anchor::get_os_typespec()) OTC_Anchor;
    OTCLIB_ASSERT_M(theEndAnchor != 0);
#else
    theStartAnchor = new OTC_Anchor;
    OTCLIB_ASSERT_M(theStartAnchor != 0);
    theEndAnchor = new OTC_Anchor;
    OTCLIB_ASSERT_M(theEndAnchor != 0);
#endif
  }

  OTC_Linkable* theStartLink;
  OTC_Linkable* theEndLink;

  theStartLink = tmpStartNode->link();
  theEndLink = tmpEndNode->link();

  if (theStartAnchor != 0)
  {
    theStartLink->addBefore(theStartAnchor);
    theStartLink = theStartAnchor;
  }
  else
  {
    theStartLink = theStartLink->prev();
  }

  if (theEndAnchor != 0)
  {
    theEndLink->addAfter(theEndAnchor);
    theEndLink = theEndAnchor;
  }
  else
  {
    theEndLink = theEndLink->next();
  }

  OTC_Cursor<T>* theIter;
#if defined(ENV_OSTORE)
  theIter = new (theLocality,
   OTC_ItemCursor< T,OTC_Bucket<T> >::get_os_typespec())
   OTC_ItemCursor< T,OTC_Bucket<T> >(list_,theStartLink,theEndLink,
   theDirection,theProtection);
#else
  theIter = new OTC_ItemCursor< T,OTC_Bucket<T> >(list_,theStartLink,
   theEndLink,theDirection,theProtection);
#endif
  OTCLIB_ASSERT_M(theIter != 0);

  if (theStartAnchor != 0)
    theStartAnchor->kill();
  if (theEndAnchor != 0)
    theEndAnchor->kill();

  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T>
int OTC_TSet<T>::rank(T const& item1, T const& item2) const
{
  return (rankFn_ != 0) ? (*rankFn_)(item1,item2) :
   OTC_RankActions<T>::rank(item1,item2);
}

/* ------------------------------------------------------------------------- */
template<class T>
int OTC_TSet<T>::searchIndex_(
 OTC_AVLLinkNode* theRoot,
 T const& theItem,
 OTC_AVLLinkNode*& theNode
) const
{
  theNode = 0;

  OTC_AVLNode* tmpNode = 0;
  OTC_AVLNode* tmpParent = 0;
  int theRank = 0;

  if (theRoot == 0)
    return 1;

  tmpNode = theRoot;

  while (tmpNode != 0)
  {
    tmpParent = tmpNode;

    OTC_Bucket<T>* tmpBucket;
    tmpBucket = (OTC_Bucket<T>*)((OTC_AVLLinkNode*)tmpNode)->link();

    theRank = rank(theItem,tmpBucket->item());
    if (theRank == 0)
      break;
    else if (theRank < 0)
      tmpNode = tmpNode->left();
    else
      tmpNode = tmpNode->right();
  }

  if (tmpNode != 0)
  {
    theNode = (OTC_AVLLinkNode*)tmpNode;
    return 0;
  }

  theNode = (OTC_AVLLinkNode*)tmpParent;
  return theRank;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_TSET_C */
