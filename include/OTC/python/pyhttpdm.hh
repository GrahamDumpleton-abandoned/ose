#ifndef OTC_PYTHON_PYHTTPDM_HH
#define OTC_PYTHON_PYHTTPDM_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyhttpdm.hh
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

#include <OTC/internet/httpdaem.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyHttpDaemon : public PyObject, public OTC_HttpDaemon
{
  public:

  			OTC_PyHttpDaemon(PyObject* theOuter, int thePort);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_port(PyObject*, PyObject*);

    static PyObject*	mfn_start(PyObject*, PyObject*);

    static PyObject*	mfn_stop(PyObject*, PyObject*);

    static PyObject*	mfn_attach(PyObject*, PyObject*);

    OTC_Boolean		authorise(OTC_String const& theClientHost);

  protected:

			~OTC_PyHttpDaemon();

    PyObject*		outer_;

  private:

			OTC_PyHttpDaemon(OTC_PyHttpDaemon const&);

    OTC_PyHttpDaemon&	operator=(OTC_PyHttpDaemon const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYHTTPDM_HH */
