#ifndef OTC_COLLCTN_PRIQUEUE_C
#define OTC_COLLCTN_PRIQUEUE_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/priqueue.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1995-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/avltree.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/avllnode.hh>
#include <OTC/collctn/holder.hh>

/* ------------------------------------------------------------------------- */
template<class T>
OTC_PriorityQueue<T>::~OTC_PriorityQueue()
{
  delete tree_;
  list_->unReference();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_PriorityQueue<T>::OTC_PriorityQueue(OTC_QueueType theType)
  : type_(theType), rankFn_(0)
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

#if defined(ENV_OSTORE)
  tree_ = new (theLocality,OTC_AVLTree::get_os_typespec()) OTC_AVLTree;
#else
  tree_ = new OTC_AVLTree;
#endif
  OTCLIB_ASSERT_M(tree_ != 0);

#if defined(ENV_OSTORE)
  list_ = new (theLocality,OTC_LinkList::get_os_typespec()) OTC_LinkList;
#else
  list_ = new OTC_LinkList;
#endif
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_PriorityQueue<T>::OTC_PriorityQueue(
 int (*theRankFn)(T const&,T const&),
 OTC_QueueType theType
)
  : type_(theType), rankFn_(theRankFn)
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

#if defined(ENV_OSTORE)
  tree_ = new (theLocality,OTC_AVLTree::get_os_typespec()) OTC_AVLTree;
#else
  tree_ = new OTC_AVLTree;
#endif
  OTCLIB_ASSERT_M(tree_ != 0);

#if defined(ENV_OSTORE)
  list_ = new (theLocality,OTC_LinkList::get_os_typespec()) OTC_LinkList;
#else
  list_ = new OTC_LinkList;
#endif
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_PriorityQueue<T>::OTC_PriorityQueue(OTC_PriorityQueue<T> const& theQueue)
  : type_(theQueue.type_), rankFn_(theQueue.rankFn_)
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

#if defined(ENV_OSTORE)
  tree_ = new (theLocality,OTC_AVLTree::get_os_typespec()) OTC_AVLTree;
#else
  tree_ = new OTC_AVLTree;
#endif
  OTCLIB_ASSERT_M(tree_ != 0);

#if defined(ENV_OSTORE)
  list_ = new (theLocality,OTC_LinkList::get_os_typespec()) OTC_LinkList;
#else
  list_ = new OTC_LinkList;
#endif
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  OTC_LinkIterator iter = theQueue.list_->items(OTCLIB_UNSAFE);
  for (iter.resetFirst(); iter.isLink(); iter.next())
    add(((OTC_Holder<T>*)iter.link())->item());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Boolean OTC_PriorityQueue<T>::isEmpty() const
{
  return tree_->isEmpty();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_PriorityQueue<T>::add(T const& theItem)
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

  OTC_Holder<T>* theHolder;
#if defined(ENV_OSTORE)
  theHolder = new (theLocality,OTC_Holder<T>::get_os_typespec())
   OTC_Holder<T>(theItem);
#else
  theHolder = new OTC_Holder<T>(theItem);
#endif
  OTCLIB_ASSERT_M(theHolder != 0);

  OTC_AVLLinkNode* theLinkNode;
#if defined(ENV_OSTORE)
  theLinkNode = new (theLocality,OTC_AVLLinkNode::get_os_typespec())
   OTC_AVLLinkNode(theHolder);
#else
  theLinkNode = new OTC_AVLLinkNode(theHolder);
#endif
  OTCLIB_ASSERT_M(theLinkNode);

  OTC_AVLNode* theNode = tree_->root();
  if (theNode == 0)
  {
    list_->addFirst(theHolder);
    tree_->addRoot(theLinkNode);
  }
  else
  {
    OTC_AVLNode* theParent = theNode;
    while (theNode != 0)
    {
      T& anItem = ((OTC_Holder<T>*)((OTC_AVLLinkNode*)theNode)->link())->item();
      int theRank = rank(theItem,anItem);
      theParent = theNode;
      if (theRank < 0)
      {
        theNode = theNode->left();
        if (theNode == 0)
        {
          ((OTC_AVLLinkNode*)theParent)->link()->addBefore(theHolder);
          theParent->addBefore(theLinkNode);
          break;
        }
      }
      else
      {
        theNode = theNode->right();
        if (theNode == 0)
        {
          ((OTC_AVLLinkNode*)theParent)->link()->addAfter(theHolder);
          theParent->addAfter(theLinkNode);
          break;
        }
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
T OTC_PriorityQueue<T>::remove()
{
  OTCLIB_ENSURE_FN((!isEmpty()),
   "OTC_PriorityQueue<T>::remove()",
   "queue empty");

  OTC_AVLNode* theNode = tree_->root();
  OTC_AVLNode* theParent = 0;
  while (theNode != 0)
  {
    theParent = theNode;
    if (type_ == OTCLIB_ASCENDING)
      theNode = theNode->left();
    else
      theNode = theNode->right();
  }
  theParent->unlink();
  OTC_AVLLinkNode* theLinkNode = (OTC_AVLLinkNode*)theParent;
  T anItem = ((OTC_Holder<T>*)theLinkNode->link())->item();
  theLinkNode->link()->kill();
  delete theParent;
  return anItem;
}

/* ------------------------------------------------------------------------- */
template<class T>
T const& OTC_PriorityQueue<T>::head() const
{
  OTCLIB_ENSURE_FN((!isEmpty()),
   "OTC_PriorityQueue<T>::head()",
   "queue empty");

  OTC_Holder<T>* aNode = 0;
  if (type_ == OTCLIB_ASCENDING)
    aNode = (OTC_Holder<T>*)list_->first();
  else
    aNode = (OTC_Holder<T>*)list_->last();

  return aNode->item();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_PriorityQueue<T>::clear()
{
  tree_->removeAll();
  list_->removeAll();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_PriorityQueue<T>::discard(u_int theCount)
{
  OTCLIB_ENSURE_FN((theCount <= tree_->population()),
   "OTC_PriorityQueue<T>::discard(u_int)",
   "insufficient items");

  while (theCount > 0)
  {
    OTC_AVLNode* theNode = tree_->root();
    OTC_AVLNode* theParent = 0;
    while (theNode != 0)
    {
      theParent = theNode;
      if (type_ == OTCLIB_ASCENDING)
	theNode = theNode->left();
      else
	theNode = theNode->right();
    }
    theParent->unlink();
    OTC_AVLLinkNode* theLinkNode = (OTC_AVLLinkNode*)theParent;
    theLinkNode->link()->kill();
    delete theParent;
    theCount--;
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_PriorityQueue<T>& OTC_PriorityQueue<T>::operator=(
 OTC_PriorityQueue<T> const& theQueue
)
{
  OTCLIB_ENSURE_FN((type_ == theQueue.type_),
   "OTC_PriorityQueue<T>::operator=(OTC_PriorityQueue<T> const&)",
   "queues of different types");

  if (&theQueue == this)
    return *this;

  clear();

  OTC_LinkIterator iter = theQueue.list_->items(OTCLIB_UNSAFE);
  for (iter.resetFirst(); iter.isLink(); iter.next())
    add(((OTC_Holder<T>*)iter.link())->item());

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
int OTC_PriorityQueue<T>::rank(T const& item1, T const& item2) const
{
  return (rankFn_ != 0) ? (*rankFn_)(item1,item2) :
   OTC_RankActions<T>::rank(item1,item2);
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_PRIQUEUE_C */
