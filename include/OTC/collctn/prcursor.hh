#ifndef OTC_COLLCTN_PRCURSOR_HH
#define OTC_COLLCTN_PRCURSOR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/prcursor.hh
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

#include <OTC/refcnt/resource.hh>
#include <OTC/collctn/prvisitr.hh>
#include <OTC/collctn/prworker.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if defined(EXPAND_TEMPLATES)
#pragma implementation "OTC/collctn/prcursor.hh"
#endif
#pragma interface "OTC/collctn/prcursor.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class OTC_PairCursor : public OTC_Resource
    // = TITLE
    //	   Abstract base class for cursors over a key/item pair.
    //
    // = CLASS TYPE
    //	   Abstract
    //
    // = DESCRIPTION
    //	   Abstract base class for cursors providing a means of iteration
    //	   over collections containing key/item pairs.
{
  public:

			OTC_PairCursor();

			~OTC_PairCursor();

    // = MOVEMENT
    //	   Following operations allow movement of the iterator over the
    //	   collection.

    virtual void	reset() = 0;
				// Resets the iterator to the start.

    virtual void	next() = 0;
				// Moves the iterator to the next
				// item.

    // = RETRIEVAL
    //	   These operations provide access to the current item in the
    //	   collection.
    //
    //	   Although dependent on the derived iterator and how the collection
    //	   being iterated over is implemented, an exception will generally
    //	   result if an attempt is made to access an item, when the iterator
    //	   is not currently located at one.

    virtual T1&		key() = 0;
				// Returns the key under the current
				// location of the iterator.

    virtual T2&		item() = 0;
				// Returns the item under the current
				// location of the iterator.

    virtual OTC_Boolean isValid() const = 0;
				// Should return <OTCLIB_TRUE> while there is
				// a valid data item under the current
				// location of the iterator.

    // = CLONING

    virtual OTC_PairCursor<T1,T2>*	clone() = 0;
				// Produces a new iterator at the same
				// location as this one, but which functions
				// independently.

    // = APPLICATORS

#if !defined(ENV_NOAPPLICATORS)

    static void		apply(
			 OTC_PairCursor<T1,T2>* theCursor,
			 OTC_PairVisitor<T1,T2>& theApplicator
			);
				// Applies <theApplicator> to each item
				// accessible by <theCursor>.

    static void		apply(
			 OTC_PairCursor<T1,T2>* theCursor,
			 OTC_PairWorker<T1,T2>& theApplicator
			);
				// Applies <theApplicator> to each item
				// accessible by <theCursor>.

#endif

    // = REFERENCE COUNTING

    static void		assign(
			 OTC_PairCursor<T1,T2>*& theLeft,
			 OTC_PairCursor<T1,T2>* theRight
			);
				// References <theRight> and then updates
				// <theLeft> with <theRight>. Handles
				// <theRight> being null.

    static void		replace(
			 OTC_PairCursor<T1,T2>*& theLeft,
			 OTC_PairCursor<T1,T2>* theRight
			);
				// References <theRight>, unreferences
				// <theLeft> and then updates the variable
				// <theLeft> with <theRight>. Handles
				// null pointers and pointers which are
				// equivalent.
};

/* ------------------------------------------------------------------------- */

#if defined(EXPAND_TEMPLATES)
#include <OTC/collctn/prcursor.c>
#endif

#if (__GNUG__ == 2 && __GNUC_MINOR__ == 5)
template<class T1, class T2>
inline OTC_PairCursor<T1,T2>::~OTC_PairCursor()
{
  // Nothing to do.
}
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_PRCURSOR_HH */
