#ifndef OTC_MEMORY_MPOBJECT_HH
#define OTC_MEMORY_MPOBJECT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/mpobject.hh
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

#include <OTC/OTC.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/memory/mpobject.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4291)
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_MPObject
    // = TITLE
    //     Mixin class for memory pool support.
    //
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     Mixin class to have small objects allocated from a common
    //     memory pool.
    //
    //     The operation of this class is disabled when Versant is being
    //     used. When the library interface to ObjectStore is being used, the
    //     common memory pool is only used for objects allocated in transient
    //     memory.
    //     
    // = SEE ALSO
    //     <OTC_CommonPool>
{
  public:

    virtual		~OTC_MPObject();

#if !defined(ENV_NOCOMMONPOOL)
    void*		operator new(size_t theSize);

    void		operator delete(void* theMemory, size_t theSize);
#endif

#if defined(ENV_OSTORE) && !defined(ENV_NOCOMMONPOOL)
    void*		operator new(
			 size_t theSize,
			 os_segment* theSegment,
			 os_typespec* theTypeSpec
			);

    void*		operator new(
			 size_t theSize,
			 os_database* theDatabase,
			 os_typespec* theTypeSpec
			);

#if OS_MAJOR_RELEASE < 6

    void*		operator new(
			 size_t theSize,
			 os_object_cluster* theCluster,
			 os_typespec* theTypeSpec
			);

#else

    void*		operator new(
			 size_t theSize,
			 os_cluster* theCluster,
			 os_typespec* theTypeSpec
			);

    void*		operator new(
			 size_t theSize,
			 os_cluster_with theHandle,
			 os_typespec* theTypeSpec
			);

#endif

    void*		operator new(
			 size_t theSize,
			 OTC_Locality const& theLocality,
			 os_typespec* theTypeSpec
			);
#endif

  protected:

			OTC_MPObject();

  private:

			OTC_MPObject(OTC_MPObject const&) {}
				// Do not define an implementation for this.

    OTC_MPObject&	operator=(OTC_MPObject const&);
				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_MPOBJECT_HH */
