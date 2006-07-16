#ifndef OTC_PYTHON_PYFILESL_HH
#define OTC_PYTHON_PYFILESL_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyfilesl.hh
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

#include <OTC/internet/filesrvl.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyFileServlet : public PyObject, public OTC_FileServlet
{
  public:

  			OTC_PyFileServlet(
			 PyObject* theOuter,
			 OTC_HttpSession* theSession,
			 char const* theFileName,
			 char const* theFileType
			);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_instance(PyObject*, PyObject*);

    void		destroy();

  protected:

			~OTC_PyFileServlet();

    PyObject*		outer_;

  private:

			OTC_PyFileServlet(OTC_PyFileServlet const&);

    OTC_PyFileServlet&	operator=(OTC_PyFileServlet const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYFILESL_HH */
