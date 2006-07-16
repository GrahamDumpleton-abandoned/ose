#ifndef OTC_COLLCTN_HINDEX_C
#define OTC_COLLCTN_HINDEX_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/hindex.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HIndex<T>::~OTC_HIndex()
{
  index_->unReference();
  list_->unReference();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HIndex<T>::OTC_HIndex(
 u_int theStartSize,
 u_int theFloorSize,
 u_int theLowThreshold,
 u_int theHighThreshold
)
  : rankFn_(0)
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
  index_ = new (theLocality,OTC_HashTable::get_os_typespec())
   OTC_HashTable(theStartSize,theFloorSize,theLowThreshold,theHighThreshold);
#else
  index_ = new OTC_HashTable(theStartSize,theFloorSize,
   theLowThreshold,theHighThreshold);
#endif
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HIndex<T>::OTC_HIndex(
 int (*theRankFn)(T const&,T const&),
 u_int theStartSize,
 u_int theFloorSize,
 u_int theLowThreshold,
 u_int theHighThreshold
)
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
  index_ = new (theLocality,OTC_HashTable::get_os_typespec())
   OTC_HashTable(theStartSize,theFloorSize,theLowThreshold,theHighThreshold);
#else
  index_ = new OTC_HashTable(theStartSize,theFloorSize,
   theLowThreshold,theHighThreshold);
#endif
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Boolean OTC_HIndex<T>::contains(T const& theKey) const
{
  OTC_HIndexEntry<T>* tmpIndexEntry;
  tmpIndexEntry = entry_(theKey);

  if (tmpIndexEntry != 0)
    return OTCLIB_TRUE;

  return OTCLIB_FALSE;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HIndexEntry<T>* OTC_HIndex<T>::insert(T const& theKey)
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  int tmpAvailableIndex = -1;

  OTC_HashEntry* tmpEntry = 0;
  OTC_HIndexEntry<T>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T>::hash(theKey);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0)
  {
    if (!tmpEntry->isOccupied())
    {
      if (tmpAvailableIndex == -1)
	tmpAvailableIndex = tmpEntryIndex;
    }

    if (tmpEntry->isEmpty())
      break;

    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_HIndexEntry<T>*)tmpEntry->item();

      tmpRank = rank(theKey,tmpIndexEntry->key());
      if (tmpRank == 0)
	return tmpIndexEntry;
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

#if defined(ENV_OSTORE)
  tmpIndexEntry = new (theLocality,OTC_HIndexEntry<T>::get_os_typespec())
   OTC_HIndexEntry<T>(theKey,index_,tmpHashValue);
#else
  tmpIndexEntry = new OTC_HIndexEntry<T>(theKey,
   index_,tmpHashValue);
#endif
  OTCLIB_ASSERT_M(tmpIndexEntry != 0);

  index_->add(tmpAvailableIndex,tmpIndexEntry,tmpHashValue);
  list_->addLast(tmpIndexEntry);

  return tmpIndexEntry;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_HIndex<T>::remove(T const& theKey)
{
  OTC_HIndexEntry<T>* tmpIndexEntry;
  tmpIndexEntry = entry_(theKey);

  if (tmpIndexEntry != 0)
    tmpIndexEntry->removeEntry();
}

/* ------------------------------------------------------------------------- */
template<class T>
int OTC_HIndex<T>::rank(T const& key1, T const& key2) const
{
  return (rankFn_ != 0) ? (*rankFn_)(key1,key2) :
   OTC_RankActions<T>::rank(key1,key2);
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HIndexEntry<T>* OTC_HIndex<T>::entry_(T const& theKey) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_HIndexEntry<T>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T>::hash(theKey);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_HIndexEntry<T>*)tmpEntry->item();
      tmpRank = rank(theKey,tmpIndexEntry->key());
      if (tmpRank == 0)
	return tmpIndexEntry;
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  return 0;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HINDEX_C */
