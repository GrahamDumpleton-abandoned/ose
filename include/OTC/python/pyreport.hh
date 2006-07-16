#ifndef OTC_PYTHON_PYREPORT_HH
#define OTC_PYTHON_PYREPORT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyreport.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/dispatch/report.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyReport : public PyObject
{
  public:

  			OTC_PyReport(OTCEV_Report* theReport);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_publisher(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subjectName(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subjectPattern(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_content(PyObject*, PyObject* theArgs);

  protected:

			~OTC_PyReport();

  private:

			OTC_PyReport(OTC_PyReport const&);

    OTC_PyReport&	operator=(OTC_PyReport const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_Report*	report_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYREPORT_HH */
