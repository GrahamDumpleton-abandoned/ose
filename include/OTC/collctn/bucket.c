#ifndef OTC_COLLCTN_BUCKET_C
#define OTC_COLLCTN_BUCKET_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/bucket.c
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

/* ------------------------------------------------------------------------- */
#if !(__GNUG__ == 2 && __GNUC_MINOR__ == 5)
template<class T>
OTC_Bucket<T>::~OTC_Bucket()
{
  OTC_BaseActions<T>::remove(item_);
}
#endif

#if !defined(ENV_OSTORE) || !defined(SCHEMA_GENERATION)
/* ------------------------------------------------------------------------- */
template<class T>
OTC_Bucket<T>::OTC_Bucket(OTC_Bucket<T> const& theBucket)
 : item_(OTC_BaseActions<T>::add(theBucket.item_))
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Bucket<T>::OTC_Bucket(T const& theItem)
 : item_(OTC_BaseActions<T>::add(theItem))
{
  // Nothing to do.
}
#endif

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Bucket<T>::bogus()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_BUCKET_C */
