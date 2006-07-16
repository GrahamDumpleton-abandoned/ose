#ifndef OTC_PYTHON_PYECHOSL_HH
#define OTC_PYTHON_PYECHOSL_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyechosl.hh
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

#include <OTC/internet/echosrvl.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyEchoServlet : public PyObject, public OTC_EchoServlet
{
  public:

  			OTC_PyEchoServlet(
			 PyObject* theOuter,
			 OTC_HttpSession* theSession
			);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_instance(PyObject*, PyObject*);

    void		destroy();

  protected:

			~OTC_PyEchoServlet();

    PyObject*		outer_;

  private:

			OTC_PyEchoServlet(OTC_PyEchoServlet const&);

    OTC_PyEchoServlet&	operator=(OTC_PyEchoServlet const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYECHOSL_HH */
