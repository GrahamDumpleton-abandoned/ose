#ifndef OTC_OSTORE_LOCALITY_HH
#define OTC_OSTORE_LOCALITY_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     ostore/locality.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OTC_OSTORE_OSTORE_HH
#include <OTC/ostore/ostore.hh>
#endif

#if defined(ENV_OSTORE)

#include <OTC/ostore/storage.hh>

/* ------------------------------------------------------------------------- */

#if OS_MAJOR_RELEASE < 6

enum OTC_LocalityType
{
  OTCLIB_SEGMENT,
  OTCLIB_CLUSTER,
  OTCLIB_STORAGE
};

class OTC_Storage;

union OSE_EXPORT OTC_LocalityUnion
{
  void* void_;
  os_segment* segment_;
  os_object_cluster* cluster_;
  OTC_Storage* storage_;
};

#endif

class OSE_EXPORT OTC_Locality
    // = TITLE
    //     Pointer object which can hold either a segment or cluster pointer.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     Bundles up ObjectStore segment and cluster pointers into one
    //     object so as to simplify code which needs to work for both segment
    //     and cluster levels of storage locality. A global <operator new()>
    //     is provided which accepts an instance of <OTC_Locality>.
{
  public:

#if OS_MAJOR_RELEASE < 6

			~OTC_Locality() {}

    // = INITIALISATION

			OTC_Locality()
			  : type_(OTCLIB_SEGMENT)
				{
				  ptr_.segment_ =
				   os_segment::get_transient_segment();
				}
				// Initialises this class to refer the
				// transient segment.

			OTC_Locality(OTC_Locality const& theLocality)
			  : type_(theLocality.type_)
				{ ptr_.void_ = theLocality.ptr_.void_; }
				// Initialises this class to refer to the
				// same locality as <theLocality>.

			OTC_Locality(os_segment* theSegment)
			  : type_(OTCLIB_SEGMENT)
				{ ptr_.segment_ = theSegment; }
				// Initialises this class to refer to
				// <theSegment>.

			OTC_Locality(os_object_cluster* theCluster)
			  : type_(OTCLIB_CLUSTER)
				{ ptr_.cluster_ = theCluster; }
				// Initialises this class to refer to
				// <theCluster>.

			OTC_Locality(OTC_Storage* theStorage)
			  : type_(OTCLIB_STORAGE)
				{ ptr_.storage_ = theStorage; }
				// Initialises this class to refer to
				// <theStorage>.

    OTC_Locality&	operator=(OTC_Locality const& theLocality)
				{
				  type_ = theLocality.type_;
				  ptr_.void_ = theLocality.ptr_.void_;
				  return *this;
				}
				// Sets this class to refer to the same
				// locality as <theLocality>.

    OTC_Locality&	operator=(os_segment* theSegment)
				{
				  type_ = OTCLIB_SEGMENT;
				  ptr_.segment_ = theSegment;
				  return *this;
				}
				// Sets this class to refer to <theSegment>.

    OTC_Locality&	operator=(os_object_cluster* theCluster)
				{
				  type_ = OTCLIB_CLUSTER;
				  ptr_.cluster_ = theCluster;
				  return *this;
				}
				// Sets this class to refer to <theCluster>.

    OTC_Locality&	operator=(OTC_Storage* theStorage)
				{
				  type_ = OTCLIB_STORAGE;
				  ptr_.storage_ = theStorage;
				  return *this;
				}
				// Sets this class to refer to <theStorage>.

    // = QUERY

    OTC_Boolean		isSegment() const
				{ return type_ == OTCLIB_SEGMENT; }
				// Returns <OTCLIB_TRUE> if the locality
				// refers to a segment.

    OTC_Boolean		isTransientSegment() const
				{
				  return ptr_.segment_ ==
				   os_segment::get_transient_segment();
				 }
				// Returns <OTCLIB_TRUE> if the locality
				// refers to the transient segment.

    OTC_Boolean		isCluster() const
				{ return type_ == OTCLIB_CLUSTER; }
				// Returns <OTCLIB_TRUE> if the locality
				// refers to a cluster.

    OTC_Boolean		isStorage() const
				{ return type_ == OTCLIB_STORAGE; }
				// Returns <OTCLIB_TRUE> if the locality
				// refers to a storage manager.

    os_segment*		segment() const
				{ return isSegment() ? ptr_.segment_ : 0; }
				// Returns the segment pointer. Will
				// be zero if this class doesn't refer to
				// a segment.

    os_object_cluster*	cluster() const
				{ return isCluster() ? ptr_.cluster_ : 0; }
				// Returns the cluster pointer. Will
				// be zero if this class doesn't refer to
				// a cluster.

    OTC_Storage*	storage() const
				{ return isStorage() ? ptr_.storage_ : 0; }
				// Returns the storage manager pointer. Will
				// be zero if this class doesn't refer to
				// a storage manager.

    OTC_Boolean		operator==(OTC_Locality const& theLocality) const
				{ return ptr_.void_ == theLocality.ptr_.void_; }
				// Returns <OTCLIB_TRUE> if <theLocality>
				// is the same as this locality.

    OTC_Boolean		operator!=(OTC_Locality const& theLocality) const
				{ return ptr_.void_ != theLocality.ptr_.void_; }
				// Returns <OTCLIB_TRUE> if <theLocality>
				// is not the same as this locality.

    // = LOCALITY

    static OTC_Locality	of(void const* theMem);
				// Returns the locality in which the
				// address <theMem> is located.

    // With respect to the following functions, the default for the class is
    // that checking for clusters will be done. If you don't use clusters at
    // all, or only rely on them in specific sections of code, you can
    // disable the checks for clusters for the whole program or around
    // specific sections of code with a noticeable improvement in
    // performance.

    static void		disableClusterCheck()
				{ clustersDisabled_ = OTCLIB_TRUE; }
				// Disable checks for locality being within
				// a cluster. Ie., only detect locality as
				// being a segment.

    static void		enableClusterCheck()
				{ clustersDisabled_ = OTCLIB_FALSE; }
				// Enable checks for locality being within
				// a cluster. Ie., check for locality being
				// in a cluster is done before check for
				// locality within a segment.

  private:

    OTC_LocalityUnion	ptr_;
				// Union containing pointer to the
				// locality.

    OTC_LocalityType	type_;
				// The type of the locality.


    static OTC_Boolean  clustersDisabled_;
                                // Flag indicating whether check for use of
                                // clusters should be carried out. Ie., just
                                // check segments when use of clusters is
                                // disabled.

#else

  public:

			~OTC_Locality() {}

			OTC_Locality() : object_(0) {}

			OTC_Locality(void const* theObject)
			  : object_(theObject) {}

    void const*		object() const { return object_; }

    static OTC_Locality	of(void const* theObject)
      				{ return OTC_Locality(theObject); }

    OTC_Boolean		operator==(OTC_Locality const& theLocality) const;

    OTC_Boolean		operator!=(OTC_Locality const& theLocality) const
      				{ return !operator==(theLocality); }

    OTC_Boolean		isTransientSegment() const;

  private:

    void const*		object_;

#endif
};

#if !defined(_OS_VECTOR)
#define _OS_VECTOR
#endif

extern OSE_EXPORT void* operator new(
 size_t, OTC_Locality const&, os_typespec*);
extern OSE_EXPORT void* operator new _OS_VECTOR(
 size_t, OTC_Locality const&, os_typespec*, os_int32);

/* ------------------------------------------------------------------------- */

#endif

#endif /* OTC_OSTORE_LOCALITY_HH */
