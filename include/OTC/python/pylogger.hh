#ifndef OTC_PYTHON_PYLOGGER_HH
#define OTC_PYTHON_PYLOGGER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pylogger.hh
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

#include <OTC/debug/logger.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyLogger : public PyObject, public OTC_Logger
{
  public:

			~OTC_PyLogger();

  			OTC_PyLogger(PyObject* theCallback);

    static PyObject*	create(PyObject* theModule, PyObject* theArgs);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_notify(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_notifyChannel(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_logFile(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_setLogFile(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_stderrOutputEnabled(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_enableStderrOutput(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_disableStderrOutput(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_longFormatEnabled(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_enableLongFormat(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_disableLongFormat(PyObject*, PyObject* theArgs);

  protected:

    void		log(
			 char const* theChannel,
			 OTC_LogLevel theLevel,
			 char const* theMessage,
			 u_int theLength
			);

  private:

			OTC_PyLogger(OTC_PyLogger const&);

    OTC_PyLogger&	operator=(OTC_PyLogger const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    PyObject*		callback_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYLOGGER_HH */
