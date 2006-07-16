#ifndef OTC_PYTHON_PYDSPTCH_HH
#define OTC_PYTHON_PYDSPTCH_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pydsptch.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/OTC.h>

/* ------------------------------------------------------------------------- */

class OTC_PyJobQueue;

class OSE_EXPORT OTC_PyDispatcher : public PyObject
{
  public:

			~OTC_PyDispatcher();

  			OTC_PyDispatcher();

    static PyObject*	instance(PyObject* theModule, PyObject* theArgs);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_run(PyObject*, PyObject*);

    static PyObject*	mfn_stop(PyObject*, PyObject*);

    static PyObject*	mfn_schedule(PyObject*, PyObject*);

    static PyObject*	mfn_wakeup(PyObject*, PyObject*);

    static PyObject*	mfn_monitor(PyObject*, PyObject*);

    static PyObject*	mfn_shutdown(PyObject*, PyObject*);

    static PyObject*	mfn_reset(PyObject*, PyObject*);

    static PyObject*	mfn_isWaiting(PyObject*, PyObject*);

    static PyObject*	mfn_isRunning(PyObject*, PyObject*);

    static PyObject*	mfn_isStopped(PyObject*, PyObject*);

    static void		enterPython()
				{
#ifdef WITH_THREAD
				  PyEval_AcquireThread(gThreadState_);
#endif
				  PyErr_Clear();
				}

    static void		leavePython()
				{
#ifdef WITH_THREAD
				  PyEval_ReleaseThread(gThreadState_);
#endif
				}

    static PyInterpreterState*  interpreterState()
                                { return gInterpreterState_; }

  private:

			OTC_PyDispatcher(OTC_PyDispatcher const&);

    OTC_PyDispatcher&	operator=(OTC_PyDispatcher const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    static OTC_PyDispatcher*	gInstance_;

    // static OTC_PyJobQueue*	gJobQueue_;

    static PyInterpreterState*  gInterpreterState_;

    static PyThreadState*	gThreadState_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYDSPTCH_HH */
