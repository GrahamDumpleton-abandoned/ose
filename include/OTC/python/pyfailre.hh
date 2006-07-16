#ifndef OTC_PYTHON_PYFAILRE_HH
#define OTC_PYTHON_PYFAILRE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyfailre.hh
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

#include <OTC/dispatch/failure.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyFailure : public PyObject
{
  public:

  			OTC_PyFailure(OTCEV_Failure* theRequest);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_conversationId(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_error(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_description(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_origin(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_details(PyObject*, PyObject* theArgs);

  protected:

			~OTC_PyFailure();

  private:

			OTC_PyFailure(OTC_PyFailure const&);

    OTC_PyFailure&	operator=(OTC_PyFailure const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_Failure*	failure_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYFAILRE_HH */
