/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/otcrstring.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1993-1994 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/rstring.hh"
#endif
#endif

#include <OTC/text/rstring.hh>
#include <OTC/collctn/hashtble.hh>
#include <OTC/collctn/strgactn.hh>
#include <OTC/memory/cmmnpool.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/thread/mxreaper.hh>

#include <OTC/stdlib/iostream.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_RString::mutex_;
OTC_NRMutex OTC_RString::rcmutex_;
OTC_NRMutex OTC_RString::stmutex_;
OTC_Boolean OTC_RString::initialised_ = OTCLIB_FALSE;
OTC_BufferingStrategy OTC_RString::bufType_ = OTCLIB_BUFFERING_UNKNOWN;
OTC_HashTable* OTC_RString::index_ = 0;

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_StringData::typespec()
{
  static os_typespec ts("OTC_StringData");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_StringData::~OTC_StringData()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_RString::~OTC_RString()
{
  if (data_->symbol != OTCLIB_FALSE)
  {
    // Treat symbols as a special case as they
    // had the reference count bumped up by one
    // initially so that the copy in the symbol
    // table couldn't be modified. Thus when it
    // reaches a count of 1 rather than 0 we need
    // to delete it, remembering to also remove
    // the index entry from the symbol table.

    if (otclib_atomic_dec(data_->count,rcmutex_) < 2)
    {
      stmutex_.lock();

      OTC_MutexReaper<OTC_NRMutex> xxxMutex;
      xxxMutex.grab(stmutex_);

      index_->remove(data_,OTC_Hash::hash(data_->data,data_->length));

      xxxMutex.release();

      stmutex_.unlock();

      release_(data_->data,data_->capacity);

      delete data_;
    }
  }
  else
  {
    // Need to delete it when count reaches 0.

    if (otclib_atomic_dec(data_->count,rcmutex_) < 1)
    {
      release_(data_->data,data_->capacity);

      delete data_;
    }
  }
}

/* ------------------------------------------------------------------------- */
OTC_RString::OTC_RString(
 u_int theLength,
 OTC_BufferingFlag theType
)
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

#if defined(ENV_OSTORE)
  data_ = new (theLocality,OTC_StringData::get_os_typespec()) OTC_StringData;
#else
  data_ = new OTC_StringData;
#endif
  OTCLIB_ASSERT_M(data_ != 0);

  data_->count = 1;
  data_->length = theLength;
  data_->symbol = OTCLIB_FALSE;

  data_->capacity = capacity_(data_->length,theType)+1;
  OTCLIB_ASSERT(data_->capacity >= data_->length+1);

#if defined(ENV_OSTORE)
  data_->data = new (theLocality,os_typespec::get_char(),
   data_->capacity) char[data_->capacity];
#else
  data_->data = allocate_(data_->capacity);
#endif
  OTCLIB_ASSERT_M(data_->data != 0);

  data_->data[data_->length] = EOS;
}

/* ------------------------------------------------------------------------- */
OTC_RString::OTC_RString(
 u_int theLength,
 void const* theObject,
 OTC_BufferingFlag theType
)
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality;
  if (theObject != 0)
    theLocality = OTC_Locality::of(theObject);
  else
    theLocality = OTC_Locality::of(this);
#endif

#if defined(ENV_OSTORE)
  data_ = new (theLocality,OTC_StringData::get_os_typespec()) OTC_StringData;
#else
  data_ = new OTC_StringData;
#endif
  OTCLIB_ASSERT_M(data_ != 0);

  data_->count = 1;
  data_->length = theLength;
  data_->symbol = OTCLIB_FALSE;

  data_->capacity = capacity_(data_->length,theType)+1;
  OTCLIB_ASSERT(data_->capacity >= data_->length+1);

#if defined(ENV_OSTORE)
  data_->data = new (theLocality,os_typespec::get_char(),
   data_->capacity) char[data_->capacity];
#else
  data_->data = allocate_(data_->capacity);
#endif
  OTCLIB_ASSERT_M(data_->data != 0);

  data_->data[data_->length] = EOS;
}

/* ------------------------------------------------------------------------- */
OTC_RString::OTC_RString(char const* theString, u_int theLength)
{
  OTCLIB_ENSURE_FN((theString != 0 || theLength == 0),
   "OTC_RString::OTC_RString(char const*, u_int)",
   "Invalid string.");

  if (theLength == 0)
    theString = "";

  stmutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex2;
  xxxMutex2.grab(stmutex_);

  if (index_ == 0)
  {
    index_ = new OTC_HashTable;
    OTCLIB_ASSERT_M(index_ != 0);
  }

  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  int tmpAvailableIndex = -1;

  OTC_HashEntry* tmpEntry = 0;
  OTC_StringData* tmpIndexEntry = 0;

  tmpHashValue = OTC_Hash::hash(theString,theLength);
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
      tmpIndexEntry = (OTC_StringData*)tmpEntry->item();

      tmpRank = OTC_Rank::rank(theString,theLength,
       tmpIndexEntry->data,tmpIndexEntry->length);

      if (tmpRank != 0)
	tmpIndexEntry = 0;
      else
	break;
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  if (tmpIndexEntry == 0)
  {
    tmpIndexEntry = new OTC_StringData;
    OTCLIB_ASSERT_M(tmpIndexEntry != 0);

    tmpIndexEntry->count = 1;
    tmpIndexEntry->length = theLength;
    tmpIndexEntry->capacity = capacity_(theLength,OTCLIB_UNBUFFERED)+1;
    tmpIndexEntry->symbol = OTCLIB_TRUE;

    tmpIndexEntry->data = allocate_(tmpIndexEntry->capacity);
    OTCLIB_ASSERT_M(tmpIndexEntry->data != 0);

    OTC_Copy::copy(tmpIndexEntry->data,theString,theLength);
    tmpIndexEntry->data[theLength] = EOS;

    index_->add(tmpAvailableIndex,tmpIndexEntry,tmpHashValue);
  }

  xxxMutex2.release();

  stmutex_.unlock();

  // Note that initial reference count will be 2 and not 1. This
  // is so that delayed copy will always occur if something tries
  // to modify the string.

  data_ = tmpIndexEntry;
  data_->count++;
}

/* ------------------------------------------------------------------------- */
void OTC_RString::reallocate(u_int theLength, OTC_BufferingFlag theType)
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality;
  OTC_Boolean theLocalityIsSet = OTCLIB_FALSE;
#endif

  // If memory is shared, always have to
  // create a new memory area.

  if (otclib_atomic_get(data_->count,rcmutex_) > 1)
  {
    OTC_StringData* tmpData;
#if defined(ENV_OSTORE)
    if (theLocalityIsSet == OTCLIB_FALSE)
    {
      theLocality = OTC_Locality::of(this);
      theLocalityIsSet = OTCLIB_TRUE;
    }
    tmpData = new (theLocality,OTC_StringData::get_os_typespec())
     OTC_StringData;
#else
    tmpData = new OTC_StringData;
#endif
    OTCLIB_ASSERT_M(tmpData != 0);

    tmpData->count = 1;
    tmpData->length = theLength;
    tmpData->symbol = OTCLIB_FALSE;

    tmpData->capacity = capacity_(theLength,theType)+1;
    OTCLIB_ASSERT(tmpData->capacity >= theLength+1);

#if defined(ENV_OSTORE)
    tmpData->data = new (theLocality,os_typespec::get_char(),
     tmpData->capacity) char[tmpData->capacity];
#else
    tmpData->data = allocate_(tmpData->capacity);
#endif
    OTCLIB_ASSERT_M(tmpData->data != 0);

    tmpData->data[tmpData->length] = EOS;

    if (tmpData->length >= data_->length)
      OTC_Copy::copy(tmpData->data,data_->data,data_->length);
    else
      OTC_Copy::copy(tmpData->data,data_->data,tmpData->length);

    if (otclib_atomic_dec(data_->count,rcmutex_) < 1)
    {
      // Since another thread may have cut
      // the delayed copy, must still make sure
      // that we clean up if count reached 0.

      release_(data_->data,data_->capacity);

      delete data_;
    }

    data_ = tmpData;
  }
  else
  {
    // If length has not changed, don't bother
    // doing anything.

    if (theLength == data_->length)
      return;

    // If length is reduced, add in a new null
    // terminator and adjust length.

    if (theLength <= data_->length)
    {
      data_->data[theLength] = EOS;
      data_->length = theLength;

      return;
    }

    // If length has increased, but have
    // sufficient capacity, add in a new null
    // terminator and adjust length.

    if (theLength > data_->length && theLength < data_->capacity)
    {
      data_->data[theLength] = EOS;
      data_->length = theLength;

      return;
    }

    // If we get here, it means we have to
    // allocate some new memory.

    u_int newCapacity = capacity_(theLength,theType)+1;
    OTCLIB_ASSERT(newCapacity >= theLength+1);

    char* tmpString;
#if defined(ENV_OSTORE)
    if (theLocalityIsSet == OTCLIB_FALSE)
    {
      theLocality = OTC_Locality::of(this);
      theLocalityIsSet = OTCLIB_TRUE;
    }
    tmpString = new (theLocality,os_typespec::get_char(),
     newCapacity) char[newCapacity];
#else
    tmpString = allocate_(newCapacity);
#endif
    OTCLIB_ASSERT_M(tmpString != 0);
    tmpString[theLength] = EOS;

    OTC_Copy::copy(tmpString,data_->data,data_->length);

    release_(data_->data,data_->capacity);

    data_->data = tmpString;
    data_->length = theLength;
    data_->capacity = newCapacity;
  }
}

/* ------------------------------------------------------------------------- */
OTC_RString const& OTC_RString::operator=(OTC_RString const& theString)
{
  if (&theString == this)
    return *this;

  if (theString.data_ == data_)
    return *this;

#if defined(ENV_OSTORE)
  // Make sure segment check is against <theString.data_> as constructor
  // accepting <void*> for locality means that data could be in different
  // class than what is holding it. If localities are different, we will
  // always have to make a copy of the string. Reuse the string data object
  // for this class if we can.

  OTC_Locality theLocality = OTC_Locality::of(this);
  OTC_Locality theRHSLocality = OTC_Locality::of(theString.data_);

  if (theLocality != theRHSLocality)
  {
    if (data_ != 0)
    {
      if (data_->symbol != OTCLIB_FALSE)
      {
	// Treat symbols as a special case as they
	// had the reference count bumped up by one
	// initially so that the copy in the symbol
	// table couldn't be modified. Thus when it
	// reaches a count of 1 rather than 0 we need
	// to delete it, remembering to also remove
	// the index entry from the symbol table.

	if (otclib_atomic_dec(data_->count,rcmutex_) < 2)
	{
	  stmutex_.lock();

	  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
	  xxxMutex.grab(stmutex_);

	  index_->remove(data_,OTC_Hash::hash(data_->data,data_->length));

	  xxxMutex.release();

	  stmutex_.unlock();

	  release_(data_->data,data_->capacity);
	}
	else
	{
	  data_ = new (theLocality,OTC_StringData::get_os_typespec())
	   OTC_StringData;
	  OTCLIB_ASSERT_M(data_ != 0);
	}
      }
      else
      {
	// Need to delete it when count reaches 0.

	if (otclib_atomic_dec(data_->count,rcmutex_) < 1)
	{
	  release_(data_->data,data_->capacity);
	}
	else
	{
	  data_ = new (theLocality,OTC_StringData::get_os_typespec())
	   OTC_StringData;
	  OTCLIB_ASSERT_M(data_ != 0);
	}
      }
    }
    else
    {
      data_ = new (theLocality,OTC_StringData::get_os_typespec())
       OTC_StringData;
      OTCLIB_ASSERT_M(data_ != 0);
    }

    // Fix up string data object and copy source string across.

    data_->count = 1;
    data_->length = theString.data_->length;
    data_->symbol = OTCLIB_FALSE;

    data_->capacity = capacity_(data_->length,OTCLIB_BUFFERED)+1;
    OTCLIB_ASSERT(data_->capacity >= data_->length+1);

    data_->data = new (theLocality,os_typespec::get_char(),
     data_->capacity) char[data_->capacity];
    OTCLIB_ASSERT_M(data_->data != 0);

    OTC_Copy::copy(data_->data,theString.data_->data,data_->length);

    data_->data[data_->length] = EOS;

    return *this;
  }
#endif

  otclib_atomic_inc(theString.data_->count,rcmutex_);

  if (data_ != 0)
  {
    if (data_->symbol != OTCLIB_FALSE)
    {
      // Treat symbols as a special case as they
      // had the reference count bumped up by one
      // initially so that the copy in the symbol
      // table couldn't be modified. Thus when it
      // reaches a count of 1 rather than 0 we need
      // to delete it, remembering to also remove
      // the index entry from the symbol table.

      if (otclib_atomic_dec(data_->count,rcmutex_) < 2)
      {
	stmutex_.lock();

	OTC_MutexReaper<OTC_NRMutex> xxxMutex;
	xxxMutex.grab(stmutex_);

	index_->remove(data_,OTC_Hash::hash(data_->data,data_->length));

	xxxMutex.release();

	stmutex_.unlock();

	release_(data_->data,data_->capacity);

	delete data_;
      }
    }
    else
    {
      // Need to delete it when count reaches 0.

      if (otclib_atomic_dec(data_->count,rcmutex_) < 1)
      {
	release_(data_->data,data_->capacity);

	delete data_;
      }
    }
  }

  data_ = theString.data_;

  return *this;
}

/* ------------------------------------------------------------------------- */
u_int OTC_RString::capacity_(
 u_int theLength,
 OTC_BufferingFlag theType
) const
{
  if (initialised_ == OTCLIB_FALSE)
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex1;
    xxxMutex1.grab(mutex_);

    if (bufType_ == OTCLIB_BUFFERING_UNKNOWN)
    {
      bufType_ = OTCLIB_BUFFERING_SCHEME2;

      char const* env = 0;

      env = getenv("OTCLIB_NOSTRINGBUFFERING");
      if (env != 0)
      {
	bufType_ = OTCLIB_BUFFERING_SCHEME0;
      }
      else
      {
	env = getenv("OTCLIB_STRINGBUFFERING1");
	if (env != 0)
	  bufType_ = OTCLIB_BUFFERING_SCHEME1;
      }
    }

    initialised_ = OTCLIB_TRUE;

    xxxMutex1.release();

    mutex_.unlock();
  }

  if (theType == OTCLIB_UNBUFFERED)
    return theLength;

  switch (bufType_)
  {
    case OTCLIB_BUFFERING_SCHEME1:
    {
      if ((theLength % 16) != 0)
	theLength = (theLength/16 + 1) * 16;

      break;
    }

    case OTCLIB_BUFFERING_SCHEME2:
    {
      // These values are magic. For those less
      // than 64 bytes, we only take off 1 byte to
      // allow for EOS marker. For those above 64
      // bytes, we take off 1 byte for EOS marker,
      // plus 8 bytes in case memory allocator uses
      // power of 2 blocks, with 8 bytes for its
      // internal memory chain. Above 1015 bytes,
      // just allow for some slop.

      if (theLength > 15)
      {
	if (theLength > 63)
	{
	  if (theLength > 247)
	  {
	    if (theLength > 1015)
	    {
	      if (theLength > 4096)
		theLength += (theLength >> 2);
	      else
		theLength += (theLength >> 1);
	    }
	    else
	      theLength = 1015;
	  }
	  else
	    theLength = 247;
	}
	else
	  theLength = 63;
      }
      else
	theLength = 15;

      break;
    }

    default:
    {
      break;
    }
  }

  return theLength;
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_RString::symbolExists(char const* theString, u_int theLength)
{
  OTCLIB_ENSURE_FN((theString != 0 || theLength == 0),
   "OTC_RString::symbolExists(char const*, u_int)",
   "null pointer but non zero count");

  OTC_Boolean theResult;
  theResult = OTCLIB_FALSE;

  stmutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex2;
  xxxMutex2.grab(stmutex_);

  if (index_ == 0)
  {
    index_ = new OTC_HashTable;
    OTCLIB_ASSERT_M(index_ != 0);
  }
  else if (index_->population() != 0)
  {
    int tmpRank;
    int tmpHashValue;
    u_int tmpIteration = 0;
    u_int tmpStartIndex = 0;
    u_int tmpEntryIndex = 0;

    OTC_HashEntry* tmpEntry = 0;
    OTC_StringData* tmpIndexEntry = 0;

    tmpHashValue = OTC_Hash::hash(theString,theLength);
    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);

    while (tmpEntry != 0 && !tmpEntry->isEmpty())
    {
      if (tmpEntry->isOccupied())
      {
	tmpIndexEntry = (OTC_StringData*)tmpEntry->item();
	tmpRank = OTC_Rank::rank(theString,theLength,
	 tmpIndexEntry->data,tmpIndexEntry->length);
	if (tmpRank == 0)
	{
	  theResult = OTCLIB_TRUE;
	  break;
	}
      }

      tmpEntry = index_->searchHash(tmpHashValue,
       tmpIteration,tmpStartIndex,tmpEntryIndex);
    }
  }

  xxxMutex2.release();

  stmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT ostream& operator<<(ostream& outs, OTC_RString const& theString)
{
  if (!outs.good())
    return outs;

  // Special streams prefix operations.

#if 0
  if (!outs.opfx())
    return outs;
#endif

  // If number of characters in string greater than width defined in
  // stream then just write it out. If not we must do some formatting.

  u_int theWidth = outs.width();
  u_int theLength = theString.length();

  if (theLength >= theWidth)
  {
    outs.write(theString.string(),theLength);
  }
  else
  {
    if (!(outs.flags() & ios::left))
    {
      for (int i=0; i<int(theWidth-theLength); i++)
	outs.put((char)outs.fill());
    }
    outs.write(theString.string(),theLength);
    if (outs.flags() & ios::left)
    {
      for (int i=0; i<int(theWidth-theLength); i++)
	outs.put((char)outs.fill());
    }
  }

  // Reset the stream width setting.

  outs.width(0);

  // Sepcial streams suffix operations.

#if 0
  outs.osfx();
#endif

  return outs;
}

/* ------------------------------------------------------------------------- */
char* OTC_RString::allocate_(u_int theSize) const
{
  char* theMemory = 0;

  if (bufType_ == OTCLIB_BUFFERING_SCHEME2)
    theMemory = (char*)OTC_CommonPool::allocate(theSize);
  else
    theMemory = new char[theSize];

  return theMemory;
}

/* ------------------------------------------------------------------------- */
void OTC_RString::release_(char* theMemory, u_int theSize) const
{
#if defined(ENV_OSTORE)
  delete [] theMemory;
#else
  if (bufType_ == OTCLIB_BUFFERING_SCHEME2)
    OTC_CommonPool::release(theMemory,theSize);
  else
    delete [] theMemory;
#endif
}

/* ------------------------------------------------------------------------- */
