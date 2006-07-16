#ifndef OTC_TEXT_PATTERN_HH
#define OTC_TEXT_PATTERN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/pattern.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/range.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/pattern.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Pattern
    // = TITLE
    //	   Abstract base class for patterns which will be matched against
    //	   strings.
    //
    // = CLASS TYPE
    //	   Abstract
    //
    // = DESCRIPTION
    //	   This class is used as the base class for various pattern matching
    //	   methods. This allows certain matching routines to be simply
    //	   expressed in terms of a pattern and not a specific type of
    //	   pattern.
{
  public:

    virtual		~OTC_Pattern();

    // = INITIALISATION

			OTC_Pattern(char const* thePattern);
				// <thePattern> is the pattern. This class
				// makes its own copy of <thePattern>.

    // = QUERY

    char const*		pattern() const
				{ return pattern_; }
				// Returns the pattern.

    // = PATTERN OPERATIONS

    virtual OTC_Boolean	match(char const* theString) = 0;
				// Should be redefined in a derived class to
				// return <OTCLIB_TRUE> if the pattern matches
				// <theString>.

    virtual u_int	start(u_int theIndex=0) const = 0;
				// Returns the index into the string where
				// the match began for the tagged portion
				// indicated by <theIndex>.

    virtual u_int	length(u_int theIndex=0) const = 0;
				// Returns the length of the match for the
				// tagged portion indicated by <theIndex>.

    virtual OTC_Range	range(u_int theIndex=0) const;
				// Returns a range object representing the
				// portion of the string that matched.

    virtual OTC_Boolean	isValid() const = 0;
				// Should be redefined in a derived class to
				// return <OTCLIB_TRUE> if the pattern was
				// valid.

  private:

			OTC_Pattern(OTC_Pattern const&);
				// Do not define an implementation for this.

    OTC_Pattern&	operator=(OTC_Pattern const&);
				// Do not define an implementation for this.

    char*		pattern_;
				// The pattern.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_PATTERN_HH */
