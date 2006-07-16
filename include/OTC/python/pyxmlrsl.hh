#ifndef OTC_PYTHON_PYXMLRSL_HH
#define OTC_PYTHON_PYXMLRSL_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyxmlrsl.hh
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

#include <OTC/internet/xmlrpcsl.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyXmlRpcServlet : public PyObject, public OTC_XmlRpcServlet
{
  public:

  			OTC_PyXmlRpcServlet(
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

			~OTC_PyXmlRpcServlet();

    PyObject*		outer_;

  private:

			OTC_PyXmlRpcServlet(OTC_PyXmlRpcServlet const&);

    OTC_PyXmlRpcServlet&	operator=(OTC_PyXmlRpcServlet const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYXMLRSL_HH */
