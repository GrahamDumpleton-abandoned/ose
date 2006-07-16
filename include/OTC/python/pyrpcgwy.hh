#ifndef OTC_PYTHON_PYRPCGWY_HH
#define OTC_PYTHON_PYRPCGWY_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyrpcgwy.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/internet/rpcgtway.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyRpcGateway : public PyObject, public OTC_RpcGateway
{
  public:

  			OTC_PyRpcGateway(
			 PyObject* theOuter,
			 char const* theServiceGroup
			);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_instance(PyObject*, PyObject*);

    OTC_Boolean		authorise(
			 char const* theLogin,
			 char const* thePassword
			);

  protected:

			~OTC_PyRpcGateway();

    PyObject*		outer_;

  private:

			OTC_PyRpcGateway(OTC_PyRpcGateway const&);

    OTC_PyRpcGateway&	operator=(OTC_PyRpcGateway const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYRPCGWY_HH */
