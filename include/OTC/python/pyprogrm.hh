#ifndef OTC_PYTHON_PYPROGRM_HH
#define OTC_PYTHON_PYPROGRM_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyprogrm.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/system/program.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyProgram : public PyObject
{
  public:

			~OTC_PyProgram();

  			OTC_PyProgram();

    static PyObject*	instance(PyObject* theModule, PyObject* theArgs);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_removeAllConfig(PyObject*, PyObject*);

    static PyObject*	mfn_removeConfig(PyObject*, PyObject*);

    static PyObject*	mfn_loadConfig(PyObject*, PyObject*);

    static PyObject*	mfn_mergeConfig(PyObject*, PyObject*);

    static PyObject*	mfn_lookupConfig(PyObject*, PyObject*);

    static PyObject*	mfn_saveConfig(PyObject*, PyObject*);

    static PyObject*	mfn_lookupEnviron(PyObject*, PyObject*);

    static PyObject*	mfn_mergeEnviron(PyObject*, PyObject*);

    static PyObject*	mfn_expandEnviron(PyObject*, PyObject*);

    static PyObject*	mfn_processIdentity(PyObject*, PyObject*);

    static PyObject*	mfn_uniqueId(PyObject*, PyObject*);

    static PyObject*	mfn_threadingEnabled(PyObject*, PyObject*);

  private:

			OTC_PyProgram(OTC_PyProgram const&);

    OTC_PyProgram&	operator=(OTC_PyProgram const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    static OTC_PyProgram*	gInstance_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYPROGRM_HH */
