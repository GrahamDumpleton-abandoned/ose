/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyjob.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pyjob.hh>
#include <OTC/python/pydsptch.hh>

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyJob::gJobCallback_ = 0;

/* ------------------------------------------------------------------------- */
OTC_PyJob::~OTC_PyJob()
{
  OTC_PyDispatcher::enterPython();

  Py_XDECREF(callback_);

  OTC_PyDispatcher::leavePython();
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyJob::init_JobCallback(
 PyObject* theModule,
 PyObject* theArgs
)
{
  PyObject* theCallback = 0;

  if (!PyArg_ParseTuple(theArgs,"O",&theCallback))
    return 0;

  Py_XINCREF(theCallback);

  gJobCallback_ = theCallback;

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
OTC_PyJob::OTC_PyJob(PyObject* theCallback)
  : callback_(theCallback)
{
  Py_XINCREF(callback_);
}

/* ------------------------------------------------------------------------- */
void OTC_PyJob::execute()
{
  OTC_PyDispatcher::enterPython();

  PyObject* theArgs;
  PyObject* theResult;

  // theArgs = Py_BuildValue("()");
  // theResult = PyEval_CallObject(callback_,theArgs);

  theArgs = Py_BuildValue("(O,())",callback_);
  theResult = PyEval_CallObject(gJobCallback_,theArgs);

  Py_XDECREF(theArgs);

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();

    OTC_PyDispatcher::leavePython();

    return;
  }

  Py_XDECREF(theResult);

  OTC_PyDispatcher::leavePython();
}

/* ------------------------------------------------------------------------- */
