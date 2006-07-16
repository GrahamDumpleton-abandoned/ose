/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpydsptch.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pydsptch.hh>
#include <OTC/python/pyjob.hh>
#include <OTC/python/pymodule.hh>

#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

/* ------------------------------------------------------------------------- */
#ifdef OTCLIB_TRACE
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_PyDispatcher");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()
#endif

/* ------------------------------------------------------------------------- */
OTC_PyDispatcher* OTC_PyDispatcher::gInstance_ = 0;
PyInterpreterState* OTC_PyDispatcher::gInterpreterState_ = 0;
PyThreadState* OTC_PyDispatcher::gThreadState_ = 0;

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyDispatcher::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Dispatcher",
  sizeof(OTC_PyDispatcher),
  0,
  OTC_PyDispatcher::sys_dealloc,
  0,
  OTC_PyDispatcher::sys_getattr,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

/* ------------------------------------------------------------------------- */
PyMethodDef OTC_PyDispatcher::gMethods_[] = {
  { "run", OTC_PyDispatcher::mfn_run, 1 },
  { "stop", OTC_PyDispatcher::mfn_stop, 1 },
  { "schedule", OTC_PyDispatcher::mfn_schedule, 1 },
  { "monitor", OTC_PyDispatcher::mfn_monitor, 1 },
  { "shutdown", OTC_PyDispatcher::mfn_shutdown, 1 },
  { "reset", OTC_PyDispatcher::mfn_reset, 1 },
  { "wakeup", OTC_PyDispatcher::mfn_wakeup, 1 },
  { "isWaiting", OTC_PyDispatcher::mfn_isWaiting, 1 },
  { "isRunning", OTC_PyDispatcher::mfn_isRunning, 1 },
  { "isStopped", OTC_PyDispatcher::mfn_isStopped, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyDispatcher::~OTC_PyDispatcher()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_PyDispatcher::OTC_PyDispatcher()
{
  ob_type = &gType_;

  _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyDispatcher::instance(PyObject* theModule, PyObject* theArgs)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_PyDispatcher::instance()");

  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  if (gInstance_ == 0)
  {
    OTC_Dispatcher::initialise();

    gInstance_ = new OTC_PyDispatcher;
    OTCLIB_ASSERT_M(gInstance_ != 0);

#ifdef WITH_THREAD
    PyEval_InitThreads();
    gInterpreterState_ = PyThreadState_Get()->interp;
    gThreadState_ = PyThreadState_New(gInterpreterState_);
#endif
  }

  Py_XINCREF(gInstance_);

  return (PyObject*)gInstance_;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyDispatcher::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyDispatcher::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyDispatcher*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyDispatcher::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyDispatcher*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyDispatcher::mfn_run(PyObject*, PyObject*)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_PyDispatcher::run()");

  Py_BEGIN_ALLOW_THREADS

  OTC_Dispatcher::run();

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyDispatcher::mfn_stop(PyObject*, PyObject*)
{
  OTC_Dispatcher::stop();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyDispatcher::mfn_schedule(PyObject*, PyObject* theArgs)
{
  PyObject* theCallback;
  int theOptions = OTCLIB_STANDARD_JOB;

  if (!PyArg_ParseTuple(theArgs,"O|i",&theCallback,&theOptions))
    return 0;

  OTC_Job* theJob;
  theJob = new OTC_PyJob(theCallback);
  OTCLIB_ASSERT(theJob != 0);

  OTC_Dispatcher::schedule(theJob,theOptions);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyDispatcher::mfn_wakeup(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Dispatcher::wakeup();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyDispatcher::mfn_monitor(PyObject*, PyObject* theArgs)
{
  int theSignal;

  if (!PyArg_ParseTuple(theArgs,"i",&theSignal))
    return 0;

  OTCEV_Shutdown::monitor(theSignal);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyDispatcher::mfn_shutdown(PyObject*, PyObject* theArgs)
{
  double theDelay = 0;

  if (!PyArg_ParseTuple(theArgs,"|d",&theDelay))
    return 0;

  OTCEV_Shutdown::schedule(int(theDelay*1000));

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyDispatcher::mfn_reset(PyObject*, PyObject* theArgs)
{
  OTC_Dispatcher::reset();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyDispatcher::mfn_isWaiting(PyObject*, PyObject* theArgs)
{
  OTC_Boolean theResult;

  theResult = OTC_Dispatcher::isWaiting();

  return Py_BuildValue("i",int(theResult));
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyDispatcher::mfn_isRunning(PyObject*, PyObject* theArgs)
{
  OTC_Boolean theResult;

  theResult = OTC_Dispatcher::isRunning();

  return Py_BuildValue("i",int(theResult));
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyDispatcher::mfn_isStopped(PyObject*, PyObject* theArgs)
{
  OTC_Boolean theResult;

  theResult = OTC_Dispatcher::isStopped();

  return Py_BuildValue("i",int(theResult));
}

/* ------------------------------------------------------------------------- */
