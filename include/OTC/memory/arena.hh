#ifndef OTC_MEMORY_ARENA_HH
#define OTC_MEMORY_ARENA_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/arena.hh
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

#include <OTC/memory/mpobject.hh>
#include <OTC/memory/align.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/memory/arena.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_ArenaBlock : public OTC_MPObject
    // = TITLE
    //     Class to keep track of arena block information.
{
  public:

#if defined(ENV_OSTORE) && !defined(SCHEMA_GENERATION)
    static os_typespec* typespec();
    static os_typespec* get_os_typespec() { return typespec(); }
#endif

			OTC_ArenaBlock() {}

			~OTC_ArenaBlock();

    char*		block;

    char*		free;

    size_t		size;

    size_t		capacity;

    OTC_ArenaBlock*	next;
};

class OSE_EXPORT OTC_Arena
    // = TITLE
    //     A memory allocator which concatenates requests into blocks.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_Arena> is a memory allocator which obtains blocks of memory
    //     using <operator new()> and then parcels the memory out in pieces.
    //     All memory allocated must be released at the same time. One can
    //     release memory for reuse through the same arena without actually
    //     destroying the whole arena.
    //     
    // = NOTES
    //     If a request for memory greater than the block size is requested,
    //     it will be allocated directly from the free store.
    //     
    //     The default block size is <2040>. A weird value calculated by
    //     looking at size of blocks allocated by GNU malloc and BSD malloc.
    //     Sun malloc used first fit, so the size doesn't matter too much
    //     when using it.
{
  public:

#if defined(ENV_OSTORE) && !defined(SCHEMA_GENERATION)
    static os_typespec* typespec();
    static os_typespec* get_os_typespec() { return typespec(); }
#endif

    // = CONSTRUCTION

			OTC_Arena(size_t theAlign=OTC_Alignment::ofDouble());
				// Initialises the class. The block size used
				// for allocating memory will be <2040> bytes
				// unless overridden by the environment
				// variable <OTCLIB_ARENABLOCKSIZE>. When the
				// amount of free space in a block falls
				// below <16> bytes, the class will stop
				// trying to allocate memory from that block.
				// The slop value can be overridden by
				// setting the environment variable
				// <OTCLIB_ARENASLOPSIZE>. Memory returned
				// will be aligned according to <theAlign>.
				// The default is to align memory to that
				// required by the type <double>. If
				// <theAlign> is <0> an exception is raised.

			OTC_Arena(
			 size_t theBlockSize,
			 size_t theSlop,
			 size_t theAlign=OTC_Alignment::ofDouble()
			);
				// Initialises the class. <theBlockSize>
				// should be the minimum amount of memory
				// allocated from the free store. When the
				// amount of free space in a block decreases
				// below <theSlop>, the class stops trying to
				// allocate from that block. Memory returned
				// will be aligned according to <theAlign>.
				// The default is to align memory to that
				// required by the type <double>.
				// If <theAlign> is <0> an exception is
				// raised.

    // = DESTRUCTION

			~OTC_Arena();
				// Returns all memory to the free store.

    // = QUERY

    size_t		blockSize() const
      				{ return blockSize_; }
    				// Returns the size of the blocks being
				// allocated.

    size_t		slop() const
      				{ return slop_; }
    				// Returns the threshold which determines
				// when a block is full. That is, when a
				// block has less than this number of bytes
				// available, it is marked as full.


    size_t		align() const
      				{ return align_; }
				// Returns the alignment constraint with
				// respect to the start of any memory
				// returned.

    // = BLOCKS
    //     The following are calculated on request as it isn't anticipated
    //     that they would be used for anything other than debugging or
    //     performance analysis.

    u_int		freeBlocks() const;
    				// Returns the number of blocks allocated
				// which still have free space in them.

    u_int		fullBlocks() const;
    				// Returns the number of blocks allocated
				// which have been marked as full.

    size_t		allocated() const
				{ return allocated_; }
				// Returns the total number of bytes which
				// are in use across all blocks. Ie., sum
				// of the sizes of blocks returned by the
				// <allocate()> member function.

    // = ALLOCATION

    void*		allocate(size_t theSize);
				// Returns a piece of memory of size
				// <theSize>.

    void		release(u_int theBlocks=1);
				// Indicates you are finished with the memory
				// that has been allocated by the arena, but
				// that you don't want to destroy the arena
				// just yet. <theBlocks> is the maximum
				// number of blocks you want the arena to
				// keep hold of. That is, this number of
				// blocks will not actually be freed but
				// will be available for reuse by the arena.
				// Blocks which were greater in size than the
				// specified block size will not be kept.

  private:

			OTC_Arena(OTC_Arena const&);
				// Do not define an implementation for this.

    OTC_Arena&		operator=(OTC_Arena const&);
				// Do not define an implementation for this.

    void*		allocateNewBlock(size_t theSize);
				// Allocates a new block of memory and
				// allocate <theSize> memory out of it.

    size_t		blockSize_;
				// Minimum block size in which allocations
				// of free store memory should be made.

    size_t		slop_;
				// When the amount of free space in a block
				// decreases below this amount we stop
				// trying to allocate more strings from it.

    size_t		align_;
				// Memory alignment.

    OTC_ArenaBlock*	full_;
				// The blocks of memory which are full.

    OTC_ArenaBlock*	free_;
				// The blocks of memory which still hold
				// some free space.

    size_t		allocated_;
				// Number of bytes which have been allocated.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_ARENA_HH */
