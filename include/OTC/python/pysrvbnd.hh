#ifndef OTC_PYTHON_PYSRVBND_HH
#define OTC_PYTHON_PYSRVBND_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pysrvbnd.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/dispatch/servbind.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyServiceBinding : public PyObject
{
  public:

  			OTC_PyServiceBinding(
			 OTC_ServiceBinding* theServiceBinding
			);

    OTC_ServiceBinding*	binding() const
    				{ return serviceBinding_; }

    static int		isType(PyObject* theObject)
    				{ return theObject->ob_type == &gType_; }

    PyObject*		obj_getattr(char const* theName);

    int			obj_compare(OTC_PyServiceBinding const*) const;

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static int          sys_compare(PyObject*, PyObject*);

    static PyObject*	mfn_serviceName(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_serviceAudience(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_receiverName(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_agentIdentity(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_processAddress(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_processIdentity(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_networkGroup(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_agentAddress(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_serviceAddress(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_serviceLocation(PyObject*, PyObject* theArgs);

  protected:

			~OTC_PyServiceBinding();

  private:

			OTC_PyServiceBinding(OTC_PyServiceBinding const&);

    OTC_PyServiceBinding&	operator=(OTC_PyServiceBinding const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTC_ServiceBinding*	serviceBinding_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYSRVBND_HH */
