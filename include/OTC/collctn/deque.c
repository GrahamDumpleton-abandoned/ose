#ifndef OTC_COLLCTN_DEQUE_C
#define OTC_COLLCTN_DEQUE_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/deque.c
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

#include <OTC/collctn/icursor.hh>

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Deque<T>::OTC_Deque(OTC_Deque<T> const& theDeque)
{
  OTC_Iterator<T> iter = 0;
  iter = theDeque.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);
  for (iter.reset(); iter.isValid(); iter.next())
    addLast(iter.item());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Deque<T>::OTC_Deque(OTC_Deque<T>& theDeque, OTC_ShallowCopy)
  : list_(theDeque.list_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Deque<T>& OTC_Deque<T>::operator=(OTC_Deque<T> const& theDeque)
{
  if (&theDeque != this)
  {
    removeAll();
    OTC_Iterator<T> iter = 0;
    iter = theDeque.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);
    for (iter.reset(); iter.isValid(); iter.next())
      addLast(iter.item());
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Cursor<T>* OTC_Deque<T>::items_(
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
   OTC_ItemCursor< T,OTC_Bucket<T> >(list_.list(),theDirection,
   theProtection);
#else
  theIter = new OTC_ItemCursor< T,OTC_Bucket<T> >(list_.list(),theDirection,
   theProtection);
#endif
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Link* OTC_Deque<T>::link_(T const& theItem)
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

  OTC_Bucket<T>* theBucket;
#if defined(ENV_OSTORE)
  theBucket = new (theLocality,OTC_Bucket<T>::get_os_typespec())
   OTC_Bucket<T>(theItem);
#else
  theBucket = new OTC_Bucket<T>(theItem);
#endif
  OTCLIB_ASSERT_M(theBucket != 0);

  return theBucket;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_DEQUE_C */
