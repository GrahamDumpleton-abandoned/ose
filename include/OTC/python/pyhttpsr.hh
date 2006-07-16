#ifndef OTC_PYTHON_PYHTTPSR_HH
#define OTC_PYTHON_PYHTTPSR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyhttpsr.hh
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

#include <OTC/internet/httpsrvr.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyHttpServer : public PyObject, public OTC_HttpServer
{
  public:

  			OTC_PyHttpServer(PyObject* theOuter);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_instance(PyObject*, PyObject*);

    OTC_Boolean		authorise(
			 char const* theLogin,
			 char const* thePassword
			);

    OTC_HttpServlet*	servlet(OTC_HttpSession* theSession);

  protected:

			~OTC_PyHttpServer();

    PyObject*		outer_;

  private:

			OTC_PyHttpServer(OTC_PyHttpServer const&);

    OTC_PyHttpServer&	operator=(OTC_PyHttpServer const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYHTTPSR_HH */
