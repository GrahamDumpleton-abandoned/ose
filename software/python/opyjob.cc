/*
// ============================================================================
//
// = FILENAME
//     opyjob.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2005 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "job.hh"
#include "dispatch.hh"

/* ------------------------------------------------------------------------- */
OTC_HMap<PyInterpreterState*,PyObject*>* OPY_Job::gJobCallbacks_ = 0;

/* ------------------------------------------------------------------------- */
OPY_Job::~OPY_Job()
{
  OPY_Dispatcher::enterPython(interpreterState_);

  Py_XDECREF(callback_);

  OPY_Dispatcher::leavePython(interpreterState_);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Job::init_JobCallback(
 PyObject* theModule,
 PyObject* theArgs
)
{
  PyObject* theCallback = 0;

  if (!PyArg_ParseTuple(theArgs,"O",&theCallback))
    return 0;

  PyInterpreterState* theInterpreterState = 0;

#ifdef WITH_THREAD
  PyEval_InitThreads();
  theInterpreterState = PyThreadState_Get()->interp;
#endif

  if (gJobCallbacks_ == 0)
    gJobCallbacks_ = new OTC_HMap<PyInterpreterState*,PyObject*>();

  if (gJobCallbacks_->contains(theInterpreterState))
  {
    Py_XDECREF(gJobCallbacks_->item(theInterpreterState));
  }

  Py_XINCREF(theCallback);

  gJobCallbacks_->add(theInterpreterState,theCallback,OTCLIB_REPLACE);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
OPY_Job::OPY_Job(PyObject* theCallback)
  : callback_(theCallback), interpreterState_(0)
{
  Py_XINCREF(callback_);

#ifdef WITH_THREAD
  PyEval_InitThreads();
  interpreterState_ = PyThreadState_Get()->interp;
#endif
}

/* ------------------------------------------------------------------------- */
void OPY_Job::execute()
{
  OPY_Dispatcher::enterPython(interpreterState_);

  PyObject* theHook = 0;

  if (!gJobCallbacks_->contains(interpreterState_))
    Py_FatalError("OPY_Job::execute - callback hook unavailable");

  theHook = gJobCallbacks_->item(interpreterState_);

  PyObject* theArgs;
  PyObject* theResult;

  theArgs = Py_BuildValue("(O,())",callback_);
  theResult = PyEval_CallObject(theHook,theArgs);

  Py_XDECREF(theArgs);

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();

    OPY_Dispatcher::leavePython(interpreterState_);

    return;
  }

  Py_XDECREF(theResult);

  OPY_Dispatcher::leavePython(interpreterState_);
}

/* ------------------------------------------------------------------------- */
