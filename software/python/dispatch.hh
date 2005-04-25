#ifndef OPY_DISPATCH_HH
#define OPY_DISPATCH_HH
/*
// ============================================================================
//
// = FILENAME
//     dispatch.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2005 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/collctn/hmap.hh>

/* ------------------------------------------------------------------------- */

class OPY_JobQueue;

class OPY_Dispatcher : public PyObject
{
  public:

			~OPY_Dispatcher();

  			OPY_Dispatcher();

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

    static PyObject*	mfn_task_start(PyObject*, PyObject*);

    static PyObject*	mfn_task_stop(PyObject*, PyObject*);

    static PyObject*	mfn_task_wait(PyObject*, PyObject*);

    static PyObject*	mfn_warningsEnabled(PyObject*, PyObject*);

    static PyObject*	mfn_enableWarnings(PyObject*, PyObject*);

    static PyObject*	mfn_disableWarnings(PyObject*, PyObject*);

  public:

    static void		enterPython(PyInterpreterState* theInterpreterState);

    static void		leavePython(PyInterpreterState* theInterpreterState);

  private:

			OPY_Dispatcher(OPY_Dispatcher const&);

    OPY_Dispatcher&	operator=(OPY_Dispatcher const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    static bool		gInitialised_;

    static OTC_HMap<PyInterpreterState*,OPY_Dispatcher*>*	gInstances_;

    static OTC_HMap<PyInterpreterState*,PyThreadState*>*	gStates_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_DISPATCH_HH */
