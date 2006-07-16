#ifndef OTC_MEMORY_MCOBJECT_HH
#define OTC_MEMORY_MCOBJECT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/mcobject.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/cmmnpool.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/memory/mcobject.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4291)
#endif

/* ------------------------------------------------------------------------- */

class OTC_Cluster;

class OSE_EXPORT OTC_ClusterAllocation
    // = TITLE
    //     Class to keep track of cluster allocation.
{
  public:

    void*		operator new(size_t theSize)
				{ return OTC_CommonPool::allocate(theSize); }

    void		operator delete(void* theMemory, size_t theSize)
				{ OTC_CommonPool::release(theMemory,theSize); }

    			OTC_ClusterAllocation() {}

    			~OTC_ClusterAllocation() {}

    OTC_Cluster*	cluster;

    void*		memory;

    size_t		size;

    OTC_ClusterAllocation*	next;
};

class OSE_EXPORT OTC_MCObject
    // = TITLE
    //     Mixin class for memory cluster objects.
    //
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     The <OTC_MCObject> class is a mixin class which gives to a derived
    //     object the ability to be allocated within a memory cluster.
    //     Allocation within a cluster is achieved through an overloaded
    //     version of <operator new()> which takes an instance of
    //     <OTC_Cluster> as argument. Objects do not have to be deleted
    //     explicitly, all objects in the cluster being automatically
    //     destroyed when the cluster is destroyed. Destructors for derived
    //     classes will be called.
    //     
    // = NOTES
    //     A derived class destructor should have <protected> access to
    //     prevent creation of the class in any context except that of a
    //     cluster.
    //     
    // = EXAMPLE
    // = BEGIN<CODE>
    //     class Foo : public OTC_MCObject { protected: ~Foo(); };
    //
    //     OTC_Cluster cluster;
    //     Foo* foo1 = new (cluster) Foo;
    //     Foo* foo2 = new (*OTC_Cluster::of(foo1)) Foo;
    // = END<CODE>
    //
    // = SEE ALSO
    //     <OTC_Cluster>
{
    friend class	OTC_Cluster;

  public:

    // = ALLOCATION

    void*		operator new(size_t theSize, OTC_Cluster& theCluster);
				// Returns memory from <theCluster> of
				// <theSize>.

  protected:

    void*		operator new(size_t);
				// Should never be called. If called,
				// generates an exception.

    // = INITIALISATION

			OTC_MCObject();
				// Adds the object into the cluster which
				// its memory was allocated from. Also
				// records in the object which cluster
				// it came from.

    // = DELETION

    void		operator delete(void*) {}
				// This doesn't do anything as memory is
				// released when the arena held by the
				// <OTC_Cluster> is deleted. This should only
				// be called by <OTC_Cluster> and not by
				// the user of a derived class.

  protected:

    virtual		~OTC_MCObject();
				// Private so that only the cluster can
				// destroy objects.

  private:

			OTC_MCObject(OTC_MCObject const&) {}
				// Do not define an implementation for this.

    OTC_MCObject&	operator=(OTC_MCObject const&);
				// Do not define an implementation for this.

    OTC_Cluster*	cluster_;
    				// The cluster this object is in.

    OTC_MCObject*	next_;
				// Pointer to next object in the same
				// cluster.

    static OTC_NRMutex	mutex_;
    				// Lock for threads.

    static OTC_ClusterAllocation	gAllocations_;
				// A chain of information about allocations
				// in progress. Need this as multiple calls
				// to <operator new()> can occur before
				// constructor for first instances is called.
				// Different threads may also allocate
				// classes at the same times as well.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_MCOBJECT_HH */
