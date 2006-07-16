#ifndef OTC_COLLCTN_PAIR_HH
#define OTC_COLLCTN_PAIR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/pair.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1995-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/mpobject.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if defined(EXPAND_TEMPLATES)
#pragma implementation "OTC/collctn/pair.hh"
#pragma interface "OTC/collctn/pair.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class OTC_Pair : public OTC_MPObject
    // = TITLE
    //     Template class for encapsulating a pair of values.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_Pair> class can be used to encapsulate a pair of simple
    //     values. This facilitates returning of a pair of values from a
    //     function. The class also allows allocation of a pair of objects
    //     in the common memory pool.
    //
    // = SEE ALSO
    //     <OTC_Single>, <OTC_Triple>
{
  public:

    // = INITIALISATION

			OTC_Pair(T1 const& theFirst, T2 const& theSecond)
			  : first_(theFirst), second_(theSecond)
				{}
				// Sets the values of this pair to <theFirst>
				// and <theSecond>.

			OTC_Pair(OTC_Pair<T1,T2> const& thePair)
			  : first_(thePair.first_), second_(thePair.second_)
				{}
				// Sets the values of this pair to that
				// of <thePair>.

    // = ASSIGNMENT

    OTC_Pair<T1,T2>&	operator=(OTC_Pair<T1,T2> const& thePair)
				{
				  first_ = thePair.first_;
				  second_ = thePair.second_;
				  return *this;
				}
				// Sets the values of this pair to that
				// of <thePair>.

    // = ACCESS

    T1 const&		first() const
				{ return first_; }
				// Returns the first item in the pair.

    T1&			first()
				{ return first_; }
				// Returns the first item in the pair.

    T2 const&		second() const
				{ return second_; }
				// Returns the second item in the pair.

    T2&			second()
				{ return second_; }
				// Returns the second item in the pair.

  public:

#if defined(ENV_OSTORE)
    static os_typespec* get_os_typespec();
#endif

  private:

    T1			first_;
				// First item in the pair.

    T2			second_;
				// Second item in the pair.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_PAIR_HH */
