#ifndef OTC_PYTHON_PYRPCSLT_HH
#define OTC_PYTHON_PYRPCSLT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyrpcslt.hh
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

#include <OTC/internet/rpcsrvlt.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyRpcServlet : public PyObject, public OTC_RpcServlet
{
  public:

  			OTC_PyRpcServlet(
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

			~OTC_PyRpcServlet();

    PyObject*		outer_;

  private:

			OTC_PyRpcServlet(OTC_PyRpcServlet const&);

    OTC_PyRpcServlet&	operator=(OTC_PyRpcServlet const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYRPCSLT_HH */
