#ifndef OTC_COLLCTN_PRBUCKET_HH
#define OTC_COLLCTN_PRBUCKET_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/prbucket.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/bucket.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if defined(EXPAND_TEMPLATES)
#pragma implementation "OTC/collctn/prbucket.hh"
#endif
#pragma interface "OTC/collctn/prbucket.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class OTC_PairBucket : public OTC_Bucket<T2>
    // = TITLE
    //     Bucket containing a lookup key as well as an item.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_PairBucket> derives from <OTC_Bucket> in order that both a
    //     key and item can be encapsulated into a single bucket. To manage
    //     resources, when parameterised over a pointer type, it is possible
    //     to define an explicit version of <OTC_BaseActions>. This allows
    //     specific actions to be performed at the point that the key and
    //     item are added to the bucket, and when the bucket is deleted.
    //     
    // = SEE ALSO
    //     <OTC_Bucket>, <OTC_BaseActions>
{
  public:

    // = CONSTRUCTION

			OTC_PairBucket(OTC_PairBucket<T1,T2> const& theBucket);
				// Constructs a bucket holding the key an
				// item held in <theBucket>. The appropriate
				// <OTC_BaseActions\<T\>::add()> function is
				// called on both the key and item.

			OTC_PairBucket(T1 const& theKey, T2 const& theItem);
				// Constructs a bucket holding <theKey> and
				// <theItem>. The appropriate
				// <OTC_BaseActions\<T\>::add()> function is
				// called on both the key and item.

    // = DESTRUCTION

			~OTC_PairBucket();
				// The appropriate
				// <OTC_BaseActions\<T\>::remove()> function
				// is called on both the key and item.

    // = ACCESS

    T1&			key()
				{ return key_; }
				// Returns the key being held by this bucket.

  public:

#if defined(ENV_OSTORE)
    static os_typespec*	get_os_typespec();
#endif

  private:

    OTC_PairBucket<T1,T2>&	operator=(OTC_PairBucket<T1,T2> const&)
				{ terminate(); return *this; }
				// Do not define an implementation for this.

    T1			key_;
				// The key being contained in this bucket.
};

/* ------------------------------------------------------------------------- */

#if defined(EXPAND_TEMPLATES)
#include <OTC/collctn/prbucket.c>
#endif

#if (__GNUG__ == 2 && __GNUC_MINOR__ == 5)
template<class T1, class T2>
inline OTC_PairBucket<T1,T2>::~OTC_PairBucket()
{
  OTC_BaseActions<T1>::remove(key_);
}
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_PRBUCKET_HH */
