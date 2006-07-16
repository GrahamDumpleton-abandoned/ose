#ifndef OTC_COLLCTN_HOLDER_HH
#define OTC_COLLCTN_HOLDER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/holder.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/link.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if defined(EXPAND_TEMPLATES)
#pragma implementation "OTC/collctn/holder.hh"
#endif
#pragma interface "OTC/collctn/holder.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_Holder : public OTC_Link
    // = TITLE
    //	   Holder to hold an instance of type <T>.
    //	   
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   This class is a holder for an item of type <T> within a linked
    //	   list. Note that when parameterised over a pointer type, the object
    //	   held is not deleted when the holder is destroyed.
    //	   
    // = SEE ALSO
    //	   <OTC_Link>, <OTC_Bucket>
{
  private:

    virtual void	bogus();
				// Bogus virtual function to get Sun
				// C++ 4.0 to work.

  public:

    // = DESTRUCTION

			~OTC_Holder();

    // = INITIALISATION

			OTC_Holder(OTC_Holder<T> const& theHolder);
				// Creates a bucket containing the item
				// held by <theHolder>.

			OTC_Holder(T const& theItem);
				// Creates a bucket containing <theItem>.

    // = RETRIEVAL

    T&			item()
				{ return item_; }
				// Returns the item being held by this
				// bucket.

  public:

#if defined(ENV_OSTORE)
    static os_typespec* get_os_typespec();
#endif

  private:

    OTC_Holder<T>&	operator=(OTC_Holder<T> const&)
				{ terminate(); return *this; }
				// Do not define an implementation for this.

    T			item_;
				// The item being contained in this bucket.
};

/* ------------------------------------------------------------------------- */

#if defined(EXPAND_TEMPLATES)
#include <OTC/collctn/holder.c>
#endif

#if (__GNUG__ == 2 && __GNUC_MINOR__ == 5)
template<class T>
inline OTC_Holder<T>::~OTC_Holder()
{
  // Nothing to do.
}
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HOLDER_HH */
