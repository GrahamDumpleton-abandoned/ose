#ifndef OTC_MEMORY_CMMNPOOL_HH
#define OTC_MEMORY_CMMNPOOL_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/cmmnpool.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/pool.hh>
#include <OTC/thread/nrmutex.hh>

#include <OTC/stdlib/ostream.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/memory/cmmnpool.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_CommonPool
    // = TITLE
    //     Common memory pool for small objects.
    //
    // = CLASS TYPE
    //     Static
    //
    // = DESCRIPTION
    //     The class <OTC_CommonPool> is a common memory pool for small
    //     objects.
    //
    //     Currently <8> pools are created, with the size of the pools
    //     varying by <sizeof(double)>. Thus, on <32> bit machines, there will
    //     be pools ranging from <8> bytes to <64> bytes, with a <8> byte
    //     difference in the size of each pool. If a piece of memory larger
    //     than <64> bytes is requested, the global <new> and <delete> are
    //     used.
    // 
    //     To define the blocksize to be used for a specific pool, you
    //     should define the <OTCLIB_POOLBLOCKSIZE_X> environment variable,
    //     where 'X' is replaced with the number of the pool. Pool numbering
    //     starts at '1' with the smallest pool. The value of the variable
    //     must be greater than '0'.
    //
    //     A pool may be made to preallocate memory from the operating
    //     system by specifying the <OTCLIB_POOLPREALLOCATE_X> environment
    //     variable, where 'X' is replaced with the number of the pool.
    //     The value of the environment variable should be the amount
    //     of memory to preallocate. If the value defined is not greater
    //     than the blocksize for that pool, the memory preallocated will
    //     be that define by the block size.
    //
    //     All the pools can be disabled and the global <new> and <delete>
    //     used for all memory, by defining the environment variable
    //     <OTCLIB_NOCOMMONPOOL>.
    //
    // = NOTES
    //     If a class overrides <operator new()> and <operator delete()>
    //     to use this class will be derived from. The class must have a
    //     virtual destructor.
    //
    // = SEE ALSO
    //     <OTC_Pool>
{
  public:

    friend class	OTC_CommonPoolDummy;
				// To shut up GNU C++.

    // = ALLOCATION/DEALLOCATION

    static void*	allocate(size_t theSize);
				// Allocates and returns a piece of memory
				// of sufficient size to hold an object of
				// <theSize>.

    static void		release(void* theMem, size_t theSize);
				// Releases <theMem>. <theSize> should be
				// the same as the size used to originally
				// allocate the memory. If the memory was
				// not originally allocated from this
				// pool, the result is undefined.

    // = MEMORY POOLS

    static u_int	numPools()
				{ return numPools_; }
				// Returns the number of memory pools.

    static OTC_Pool const*	pool(u_int theNum)
				{
				  return (!theNum || theNum > numPools_)
				   ? 0 : pools_[theNum-1];
				}
				// Returns a pointer to the pool at position
				// <theNum>. Pool positions start at <1>. The
				// largest pool number is that given by
				// calling <numPools()>.

    static void		usageSummary(ostream& theStream);
				// Dump memory pool usage summary to
				// <theStream>.

  private:

			OTC_CommonPool();
				// Do not define an implementation for this.

			OTC_CommonPool(OTC_CommonPool const&);
				// Do not define an implementation for this.

    OTC_CommonPool&	operator=(OTC_CommonPool const&);
				// Do not define an implementation for this.

    static void		initialise();
				// Initialises pool table and lookup index.

    static OTC_NRMutex	mutex_;
				// Lock for threads. Use non recursive mutex
				// as there shouldn't be any way for a thread
				// to come across the lock again while it is
				// locked.

    static OTC_Boolean	init_;
				// <OTCLIB_TRUE> once the class has been
				// initialised.

    static OTC_Boolean	disable_;
				// <OTCLIB_TRUE> if memory pools are
				// disabled.

    static OTC_Pool**	pools_;
				// Set of memory pools;

    static u_int	numPools_;
				// The number of pools.

    static size_t	increment_;
				// The size of the smallest pool and the
				// difference in size between the subsequent
				// pools.

    static size_t	largest_;
				// Size of the largest pool. When asked to
				// allocate memory larger than this we use
				// standard <new> and <delete>.

    static u_int*	index_;
				// Fast lookup index to avoid doing divisions
				// all the time.

    static size_t	allocated_;
				// The current number of allocated objects.

    static size_t	total_;
				// Total number of allocations ever made.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_CMMNPOOL_HH */
