#ifndef OTC_COLLCTN_CURSOR_HH
#define OTC_COLLCTN_CURSOR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/cursor.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/refcnt/resource.hh>
#include <OTC/collctn/visitor.hh>
#include <OTC/collctn/worker.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if defined(EXPAND_TEMPLATES)
#pragma implementation "OTC/collctn/cursor.hh"
#endif
#pragma interface "OTC/collctn/cursor.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_Cursor : public OTC_Resource
    // = TITLE
    //	   Abstract base class for implementations of iterators.
    //
    // = CLASS TYPE
    //	   Abstract
    //
    // = DESCRIPTION
    //	   Abstract base class, for classes which implement iteration
    //	   over specific types of collections.
{
  private:

    virtual void	bogus();
				// Bogus virtual functions to get Sun
				// C++ 4.0 to work.

  public:

			~OTC_Cursor();

			OTC_Cursor();

    // = MOVEMENT
    //	   Following operations allow movement of the iterator over the
    //	   collection.

    virtual void	reset();
				// Resets the iterator to the start.

    virtual void	next();
				// Moves the iterator to the next item.

    // = RETRIEVAL
    //	   These operations provide access to the current item in the
    //	   collection.
    //
    //	   Although dependent on the derived iterator and how the collection
    //	   being iterated over is implemented, an exception will generally
    //	   result if an attempt is made to access an item, when the iterator
    //	   is not currently located at one.

    virtual T&		item();
				// Returns the item under the current
				// location of the iterator.

    virtual OTC_Boolean isValid() const;
				// Should return <OTCLIB_TRUE> while there is
				// a valid data item under the current
				// location of the iterator.

    // = CLONING

    virtual OTC_Cursor<T>*	clone();
				// Produces a new iterator at the same
				// location as this one, but which functions
				// independently.

    // = APPLICATORS

#if !defined(ENV_NOAPPLICATORS)

    static void		apply(
			 OTC_Cursor<T>* theCursor,
			 OTC_Visitor<T>& theApplicator
			);
				// Applies <theApplicator> to each item
				// accessible by <theCursor>.

    static void		apply(
			 OTC_Cursor<T>* theCursor,
			 OTC_Worker<T>& theApplicator
			);
				// Applies <theApplicator> to each item
				// accessible by <theCursor>.

#endif

    // = REFERENCE COUNTING

    static void		assign(
			 OTC_Cursor<T>*& theLeft,
			 OTC_Cursor<T>* theRight
			);
				// References <theRight> and then updates
				// <theLeft> with <theRight>. Handles
				// <theRight> being null.

    static void		replace(
			 OTC_Cursor<T>*& theLeft,
			 OTC_Cursor<T>* theRight
			);
				// References <theRight>, unreferences
				// <theLeft> and then updates the variable
				// <theLeft> with <theRight>. Handles
				// null pointers and pointers which are
				// equivalent.
};

/* ------------------------------------------------------------------------- */

#if defined(EXPAND_TEMPLATES)
#include <OTC/collctn/cursor.c>
#endif

#if (__GNUG__ == 2 && __GNUC_MINOR__ == 5)
template<class T>
inline OTC_Cursor<T>::~OTC_Cursor()
{
  // Nothing to do.
}
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_CURSOR_HH */
