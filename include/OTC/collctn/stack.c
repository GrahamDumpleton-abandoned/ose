#ifndef OTC_COLLCTN_STACK_C
#define OTC_COLLCTN_STACK_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/stack.c
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

#include <OTC/collctn/holder.hh>

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Stack<T>::OTC_Stack()
  : count_(0)
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
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Stack<T>::OTC_Stack(OTC_Stack<T> const& theStack)
  : count_(0)
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

  OTC_LinkIterator iter = theStack.list_->items(OTCLIB_UNSAFE);
  for (iter.resetLast(); iter.isLink(); iter.prev())
    push(((OTC_Holder<T>*)iter.link())->item());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Stack<T>& OTC_Stack<T>::operator=(OTC_Stack<T> const& theStack)
{
  if (&theStack == this)
    return *this;

  clear();

  OTC_LinkIterator iter = theStack.list_->items(OTCLIB_UNSAFE);
  for (iter.resetLast(); iter.isLink(); iter.prev())
    push(((OTC_Holder<T>*)iter.link())->item());

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Stack<T>::push(T const& theItem)
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

  OTC_Holder<T>* theNode;
#if defined(ENV_OSTORE)
  theNode = new (theLocality,OTC_Holder<T>::get_os_typespec())
   OTC_Holder<T>(theItem);
#else
  theNode = new OTC_Holder<T>(theItem);
#endif
  OTCLIB_ASSERT_M(theNode != 0);
  list_->addFirst(theNode);
  count_++;
}

/* ------------------------------------------------------------------------- */
template<class T>
T OTC_Stack<T>::pop()
{
  OTCLIB_ENSURE_FN((!isEmpty()),
   "OTC_Stack<T>::pop()",
   "stack empty");

  OTC_Holder<T>* theNode = (OTC_Holder<T>*)list_->first();
  T theItem = theNode->item();
  theNode->kill();
  count_--;
  return theItem;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Stack<T>::discard(u_int theCount)
{
  OTCLIB_ENSURE_FN((theCount <= count_),
   "OTC_Stack<T>::discard(u_int)",
   "insufficient items");

  while (theCount > 0)
  {
    list_->first()->kill();
    theCount--;
    count_--;
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
T& OTC_Stack<T>::top_() const
{
  OTCLIB_ENSURE_FN((!isEmpty()),
   "OTC_Stack<T>::top_()",
   "stack empty");

  OTC_Holder<T>* theNode = (OTC_Holder<T>*)list_->first();
  return theNode->item();
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_STACK_C */
