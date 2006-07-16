#ifndef OTC_PYTHON_PYMODULE_HH
#define OTC_PYTHON_PYMODULE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pymodule.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/OTC.h>

/* ------------------------------------------------------------------------- */

extern "C" OSE_EXPORT void otclib_init_python();

extern OSE_EXPORT PyObject* OTCLIB_PYTHON_ERROR_OBJECT;

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYMODULE_HH */
