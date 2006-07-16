#ifndef OTC_REFCNT_RESOURCE_HH
#define OTC_REFCNT_RESOURCE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     refcnt/resource.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/refcnt/count.hh>
#include <OTC/thread/nrmutex.hh>
#include <OTC/thread/atomic.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/refcnt/resource.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Resource : public OTC_MPObject
    // = TITLE
    //     Base class for objects requiring reference counting.
    //     
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     The class <OTC_Resource> contains support for reference counting.
    //     The reference count allows an object to know how many other
    //     objects hold a reference to it. This allows the object to know
    //     when it is no longer required, so delete itself.
    //     
    //     For this to work, users of objects derived from <OTC_Resource>
    //     must obey a strict protocol. The protocol is that if the user of
    //     the object wishes to hold a reference to the object, the method
    //     <reference()> must be called. When the user of the object no
    //     longer requires the object, the method <unReference()> must be
    //     called. After the <unReference()> method has been called, the
    //     user must not attempt to access the object. If the object is
    //     accessed after <unReference()> has been called, the result is
    //     undefined. In addition to the above, once the <reference()>
    //     method has been called, no user of the object should destroy the
    //     object using <operator delete()>. If an attempt is made to destroy
    //     the object in this way, an exception will be raised.
    //     
    //     Objects derived from <OTC_Resource> should not be created on the
    //     stack, as members of other classes or arrays. The objects should
    //     always be created by using <operator new()>. If an object is not
    //     created using <operator new()>, the methods <reference()> and
    //     <unReference()> must not be used. If either of the methods are
    //     called, the result is undefined. As this could quite easily occur,
    //     the <OTC_Resource> class contains code to flag this as an error
    //     and display a warning. To enable the generation of these warning
    //     messages via the log facility, the environment variable
    //     <OTCLIB_RESOURCEWARNINGS> should be set. Note though, that the
    //     check is disabled when the library is being used in conjunction
    //     with any object oriented database such as ObjectStore or Versant,
    //     and when thread support is enabled at compilation time.
    //     
    // = NOTES
    //     To support the checks for referencing an object that was not
    //     created using <operator new()>, the method <doInOperatorNew()> is
    //     invoked from <operator new()>. If defining an <operator new()> in
    //     a derived class, you must include a call to the method
    //     <doInOperatorNew()>. If the call is not included, an exception
    //     will be incorrectly raised when you call <reference()> on an
    //     instance of the derived class.
    //     
    // = EXAMPLE
    //     A code example adhering to the protocol described above.
    //
    // = BEGIN<CODE>
    //     class EX_Foo : public virtual OTC_Resource { };
    //     EX_Foo* foo = new EX_Foo;
    //     foo->reference();
    //     // ...
    //     foo->unReference(); // Item deleted here automatically.
    // = END<CODE>
{
  public:

    // = REFERENCE PROTOCOL

    u_int		reference() const;
				// Increments by one, the number of objects
				// which reference this object.

    u_int		unReference() const;
				// Decrements by one, the number of objects
				// which reference this object. If the count
				// reaches <0> as a result, this object will
				// delete itself by invoking <delete this>.
				// If there are no references to the object,
				// an exception is raised.

    // = REFERENCED OBJECTS

    u_int		numRefs() const;
				// Returns a count of the number of references
				// to this object.

  protected:

    // = CONSTRUCTION

			OTC_Resource();
				// Initialises the reference count to <0>.

  public:

    // = DESTRUCTION

    virtual		~OTC_Resource();
				// Checks that the number of references to
				// this object is <0>. If the count is not
				// <0>, the reference/unreference protocol
				// has been violated and an exception will
				// be raised.

    // = REFERENCE CHECKS

    static void		doInOperatorNew()
				{ globAllocatedUsingNew_ = OTCLIB_TRUE; }
				// Invoked by the <operator new()> function
				// of this class. If a derived class contains
				// an <operator new()>, it must include a
				// call to this function. If
				// <doInOperatorNew()> is not called, and
				// <reference()> is called in a derived
				// class, an incorrect warning regarding
				// referencing of an object not allocated
				// using <operator new()> may be generated.

#if !defined(ENV_OSTORE) && !defined(ENV_THREADS)

    void*		operator new(size_t theSize);

    void		operator delete(void* theMem, size_t theSz);

#endif

  private:

			OTC_Resource(OTC_Resource const&)
			  : OTC_MPObject() {}
				// Do not define an implementation for this.

    OTC_Resource&	operator=(OTC_Resource const&);
				// Do not define an implementation for this.

    OTC_Atomic		refCount_;
				// The count of the number of objects which
				// reference this object.

    OTC_Boolean		allocatedUsingNew_;
				// <OTCLIB_TRUE> if this object was allocated
				// using <operator new()>.

    static OTC_NRMutex	mutex_;
    				// Mutex to protect manipulation of reference
				// count in threaded application.

    static OTC_Boolean	globAllocatedUsingNew_;
				// Used to transfer information from global
				// scope in <operator new()>, to class scope
				// in the constructor.

    static OTC_Boolean	globInitialised_;
				// <OTCLIB_TRUE> if the environment variable
				// <OTCLIB_RESOURCEWARNINGS> has been checked.

    static OTC_Boolean	globResourceWarnings_;
				// <OTCLIB_TRUE> if the environment variable
				// <OTCLIB_RESOURCEWARNINGS> was set.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_REFCNT_RESOURCE_HH */
