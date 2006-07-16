#ifndef OTC_COLLCTN_RANGE_HH
#define OTC_COLLCTN_RANGE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/range.hh
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

#include <OTC/collctn/iterator.hh>
#include <OTC/collctn/dirction.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/range.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Range
    // = TITLE
    //     Class to represent a range of integers.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class is for representing a range of integers. That set of
    //     integers is any integer <i> such that the condition <lower() \<=
    //     i \< upper()> is satisfied. If <lower()> is equal to <upper()>
    //     then it is a null range. In defining the range, the lower bound
    //     and a length is expected. The property which relates the bounds
    //     and the length is <upper() == lower() + length()>.
{
  public:

#if defined(ENV_OSTORE) && !defined(SCHEMA_GENERATION)
    static os_typespec* typespec();
    static os_typespec* get_os_typespec() { return typespec(); }
#endif

    // = CONSTRUCTION

			OTC_Range(u_int theLength=0)
			  : lower_(0), length_(theLength) {}
				// Initialises the class to represent
				// a range with a length <theLength> and
				// lower bound of <0>.

			OTC_Range(int theLower, u_int theLength)
			  : lower_(theLower), length_(theLength) {}
				// Initialises the class to represent a
				// range with a lower bound <theLower> and
				// length <theLength>.

			OTC_Range(OTC_Range const& theRange)
			  : lower_(theRange.lower_),
			    length_(theRange.length_) {}
				// Initialises the class to represent
				// the same range as <theRange>.

    // = ASSIGNMENT

    OTC_Range&		operator=(OTC_Range const& theRange)
				{
				  lower_ = theRange.lower_;
				  length_ = theRange.length_;
				  return *this;
				}
				// Changes this class to represent the
				// same range as <theRange>.

    // = COMPARISON

    OTC_Boolean		operator==(OTC_Range const& theRange) const
				{
				  return lower_ == theRange.lower_ &&
				   length_ == theRange.length_;
				}
				// Returns <OTCLIB_FALSE> if this range
				// isn't equivalent to <theRange>.

    OTC_Boolean		operator!=(OTC_Range const& theRange) const
				{ return !(*this == theRange); }
				// Returns <OTCLIB_FALSE> if this range
				// is equivalent to <theRange>.

    // = QUERY

    int			lower() const
				{ return lower_; }
				// Returns the integer which is the lower
				// bound of the range. Note that this value
				// is regarded as being in the range.

    int			upper() const
				{ return lower_ + length_; }
				// Returns the integer which is the upper
				// bound of the range. Note that this value
				// is not regarded as being in the range, but
				// is one more than the highest value in the
				// range.

    u_int		length() const
				{ return length_; }
				// Returns the length of this range, ie., the
				// number of integers in the range.

    OTC_Boolean		contains(int theValue) const
				{
				  return lower() <= theValue &&
				   theValue < upper();
				}
				// Returns <OTCLIB_FALSE> if <theValue> is
				// not contained in this range.

    OTC_Boolean		isNullRange() const
				{ return length_ == 0; }
				// Returns <OTCLIB_FALSE> if this is not a
				// null range.

    OTC_Boolean		isSubRange(OTC_Range const& theRange) const;
				// Returns <OTCLIB_FALSE> if this range is not
				// contained within <theRange>. If this
				// range is a null range then <OTCLIB_TRUE>
				// will always be returned.

    OTC_Iterator<int>	indices(
			 OTC_Direction theDirection=OTCLIB_FORWARD
			) const;
				// Returns an iterator for the indices in the
				// range. <theDirection> indicates the
				// direction of traversal, namely, if
				// <theDirection> is <OTCLIB_FORWARD>,
				// traversal will be in the direction of
				// lower to upper. For <OTCLIB_BACKWARD> it
				// will be the reverse.

  private:

    int			lower_;
				// Lower bound of the range.

    u_int		length_;
				// The length of the range.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_RANGE_HH */
