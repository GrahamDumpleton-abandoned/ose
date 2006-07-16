#ifndef OTC_COLLCTN_KCURSOR_C
#define OTC_COLLCTN_KCURSOR_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/kcursor.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1994-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
template<class T, class L>
os_typespec* OTC_KeyCursor<T,L>::typespec()
{
  static os_typespec ts("OTC_KeyCursor<int,OTC_IILink>");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
#if !(__GNUG__ == 2 && __GNUC_MINOR__ == 5)
template<class T, class L>
OTC_KeyCursor<T,L>::~OTC_KeyCursor()
{
  // Nothing to do.
}
#endif

/* ------------------------------------------------------------------------- */
template<class T, class L>
OTC_KeyCursor<T,L>::OTC_KeyCursor(
 OTC_LinkList* aList,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
 : OTC_LinkCursor(aList,theDirection,theProtection)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
OTC_KeyCursor<T,L>::OTC_KeyCursor(
 OTC_LinkList* aList,
 OTC_Linkable* theStart,
 OTC_Linkable* theEnd,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
 : OTC_LinkCursor(aList,theStart,theEnd,theDirection,theProtection)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
OTC_KeyCursor<T,L>::OTC_KeyCursor(OTC_KeyCursor<T,L> const& theIter)
 : OTC_LinkCursor(theIter)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
void OTC_KeyCursor<T,L>::reset()
{
  OTC_LinkCursor::reset();
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
T& OTC_KeyCursor<T,L>::item()
{
  OTCLIB_ENSURE_FN((OTC_LinkCursor::isValid()),
   "OTC_KeyCursor<T,L>::item()",
   "no item under cursor");

  // return ((L*)OTC_LinkCursor::link())->key();

  OTC_Link* theLink;
  theLink = OTC_LinkCursor::link();

  L* theTLink;
  theTLink = (L*)theLink;

  T* theKey = (T*)&theTLink->key();

  return *theKey;
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
void OTC_KeyCursor<T,L>::next()
{
  OTC_LinkCursor::next();
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
OTC_Boolean OTC_KeyCursor<T,L>::isValid() const
{
  return OTC_LinkCursor::isValid();
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
OTC_Cursor<T>* OTC_KeyCursor<T,L>::clone()
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

  OTC_KeyCursor<T,L>* theIter;
#if defined(ENV_OSTORE)
  theIter = new (theLocality,OTC_KeyCursor<T,L>::get_os_typespec())
   OTC_KeyCursor<T,L>(*this);
#else
  theIter = new OTC_KeyCursor<T,L>(*this);
#endif
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_KCURSOR_C */
