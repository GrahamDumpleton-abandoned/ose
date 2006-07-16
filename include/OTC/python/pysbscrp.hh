#ifndef OTC_PYTHON_PYSBSCRP_HH
#define OTC_PYTHON_PYSBSCRP_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pysbscrp.hh
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

#include <OTC/dispatch/sbscrptn.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PySubscription : public PyObject
{
  public:

  			OTC_PySubscription(
			 OTCEV_Subscription* theSubscription
			);

    OTCEV_Subscription*	subscription() const
    				{ return subscription_; }

    static int		isType(PyObject* theObject)
    				{ return theObject->ob_type == &gType_; }

    PyObject*		obj_getattr(char const* theName);

    int			obj_compare(OTC_PySubscription const*) const;

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static int          sys_compare(PyObject*, PyObject*);

    static PyObject*	mfn_publisher(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subscriber(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_status(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subject(PyObject*, PyObject* theArgs);

  protected:

			~OTC_PySubscription();

  private:

			OTC_PySubscription(
			 OTC_PySubscription const&
			);

    OTC_PySubscription&	operator=(OTC_PySubscription const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_Subscription*	subscription_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYSBSCRP_HH */
