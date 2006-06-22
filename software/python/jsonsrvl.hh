#ifndef OPY_JSONSRVL_HH
#define OPY_JSONSRVL_HH
/*
// ============================================================================
//
// = FILENAME
//     jsonsrvl.hh
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

#include <OTC/internet/jsonsrvl.hh>

/* ------------------------------------------------------------------------- */

class OPY_JsonRpcServlet : public PyObject, public OTC_JsonRpcServlet
{
  public:

  			OPY_JsonRpcServlet(
			 PyObject* theOuter,
			 OTC_HttpSession* theSession,
			 OTC_ServiceBinding* theBinding=0
			);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_instance(PyObject*, PyObject*);

    void		destroy();

  protected:

			~OPY_JsonRpcServlet();

    PyObject*		outer_;

  private:

			OPY_JsonRpcServlet(OPY_JsonRpcServlet const&);

    OPY_JsonRpcServlet&	operator=(OPY_JsonRpcServlet const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    PyInterpreterState*	interpreterState_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_JSONSRVL_HH */
