#ifndef OPY_JSONGTWY_HH
#define OPY_JSONGTWY_HH
/*
// ============================================================================
//
// = FILENAME
//     jsongtwy.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2006 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/internet/jsongtwy.hh>

/* ------------------------------------------------------------------------- */

class OPY_JsonRpcGateway : public PyObject, public OTC_JsonRpcGateway
{
  public:

  			OPY_JsonRpcGateway(
			 PyObject* theOuter,
			 char const* theSelection,
			 OTC_ServiceSubscription theType
			);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_instance(PyObject*, PyObject*);

    bool		authorise(
			 char const* theLogin,
			 char const* thePassword
			);

  protected:

			~OPY_JsonRpcGateway();

    PyObject*		outer_;

  private:

			OPY_JsonRpcGateway(OPY_JsonRpcGateway const&);

    OPY_JsonRpcGateway&	operator=(OPY_JsonRpcGateway const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    PyInterpreterState*	interpreterState_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_JSONGTWY_HH */
