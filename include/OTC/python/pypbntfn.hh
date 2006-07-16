#ifndef OTC_PYTHON_PYPBNTFN_HH
#define OTC_PYTHON_PYPBNTFN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pypbntfn.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/dispatch/pbntfctn.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyPublisherNotification : public PyObject
{
  public:

  			OTC_PyPublisherNotification(
			 OTCEV_PublisherNotification* theNotification
			);

    OTCEV_PublisherNotification*	notification() const
    				{ return notification_; }

    static int		isType(PyObject* theObject)
    				{ return theObject->ob_type == &gType_; }

    PyObject*		obj_getattr(char const* theName);

    int			obj_compare(OTC_PyPublisherNotification const*) const;

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static int          sys_compare(PyObject*, PyObject*);

    static PyObject*	mfn_publisher(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subscriber(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_status(PyObject*, PyObject* theArgs);

  protected:

			~OTC_PyPublisherNotification();

  private:

			OTC_PyPublisherNotification(
			 OTC_PyPublisherNotification const&
			);

    OTC_PyPublisherNotification&	operator=(
			 OTC_PyPublisherNotification const&
			);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_PublisherNotification*	notification_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYPBNTFN_HH */
