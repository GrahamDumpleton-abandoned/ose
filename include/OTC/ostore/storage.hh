#ifndef OTC_OSTORE_STORAGE_HH
#define OTC_OSTORE_STORAGE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     ostore/storage.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OTC_OSTORE_OSTORE_HH
#include <OTC/ostore/ostore.hh>
#endif

#if defined(ENV_OSTORE)

#ifndef OTC_OTC_H
#include <OTC/OTC.h>
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Storage
    // = TITLE
    //     Base class for storage managers for ObjectStore.
    //
    // = DESCRIPTION
    //     By deriving from this class, it is possible to construct
    //     alternate means of grouping objects in ObjectStore besides
    //     a single cluster. Through a derived class, you could allow
    //     related objects to be stored across a number of fixed size
    //     clusters.
{
  public:

    virtual		~OTC_Storage();

    // = QUERY

    virtual OTC_Boolean	manages(void const* theObject) const = 0;
				// Should <OTCLIB_TRUE> if this storage
				// manager manages <theObject>.

    static OTC_Storage*	first()
				{ return first_; }
				// Returns the first storage manager in
				// the chain of storage managers.

    OTC_Storage*	prev()
				{ return prev_; }
				// Returns the previous storage manager in
				// the chain of storage managers.

    OTC_Storage*	next()
				{ return next_; }
				// Returns the next storage manager in
				// the chain of storage managers.

    // = ALLOCATION

    virtual void*	allocate(
			 size_t theSize,
			 os_typespec* theTypeSpec
			) = 0;
				// Should return a block of memory suitable
				// for containing an object of type as defined
				// by <theTypeSpec> where that object has
				// size <theSize>.

    virtual void*	allocate(
			 size_t theSize,
			 os_typespec* theTypeSpec,
			 os_int32 theNum
			) = 0;
				// Should return a block of memory suitable
				// for containing an array of objects of
				// length <theNum> where the object is of
				// type as defined by <theTypeSpec> and where
				// the size of the array will be <theSize>.

    // = DESTRUCTION

    virtual void	destroy() = 0;
				// Should be redefined in a derived class
				// to provide a means of destroying all
				// segments/clusters associated with this
				// locality of storage. This function is
				// independent of the destructor of this
				// class. The destructor doesn't take any
				// action to destroy segments/clusters
				// associated with this locality of storage
				// as this class will usually server purely
				// as a transient marker for storage space.

  protected:

			OTC_Storage();

  private:

			OTC_Storage(OTC_Storage const&);
				// Do not define an implementation for this.

    OTC_Storage&	operator=(OTC_Storage const&);
				// Do not define an implementation for this.

    static OTC_Storage*	first_;
				// Pointer to first storage manger in the
				// chain of storage managers.

    OTC_Storage*	prev_;
				// Pointer to the previous storage manager in
				// the chain of storage managers.

    OTC_Storage*	next_;
				// Pointer to the next storage manager in
				// the chain of storage managers.
};

/* ------------------------------------------------------------------------- */

#endif

#endif /* OTC_OSTORE_STORAGE_HH */
