#ifndef OTC_PYTHON_PYFILESR_HH
#define OTC_PYTHON_PYFILESR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyfilesr.hh
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

#include <OTC/internet/filesrvr.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyFileServer : public PyObject, public OTC_FileServer
{
  public:

  			OTC_PyFileServer(
			 PyObject* theOuter,
			 char const* theDirectory
			);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_instance(PyObject*, PyObject*);

    static PyObject*	mfn_directory(PyObject*, PyObject*);

    static PyObject*	mfn_map(PyObject*, PyObject*);

    OTC_Boolean		authorise(
			 char const* theLogin,
			 char const* thePassword
			);

  protected:

			~OTC_PyFileServer();

    PyObject*		outer_;

  private:

			OTC_PyFileServer(OTC_PyFileServer const&);

    OTC_PyFileServer&	operator=(OTC_PyFileServer const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYFILESR_HH */
