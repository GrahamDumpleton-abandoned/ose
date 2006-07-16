/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpylogger.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pylogger.hh>

#include <OTC/python/pydsptch.hh>
#include <OTC/debug/logger.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyLogger::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Logger",
  sizeof(OTC_PyLogger),
  0,
  OTC_PyLogger::sys_dealloc,
  0,
  OTC_PyLogger::sys_getattr,
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
PyMethodDef OTC_PyLogger::gMethods_[] = {
  { "notify", OTC_PyLogger::mfn_notify, 1 },
  { "notifyChannel", OTC_PyLogger::mfn_notifyChannel, 1 },
  { "logFile", OTC_PyLogger::mfn_logFile, 1 },
  { "setLogFile", OTC_PyLogger::mfn_setLogFile, 1 },
  { "stderrOutputEnabled", OTC_PyLogger::mfn_stderrOutputEnabled, 1 },
  { "enableStderrOutput", OTC_PyLogger::mfn_enableStderrOutput, 1 },
  { "disableStderrOutput", OTC_PyLogger::mfn_disableStderrOutput, 1 },
  { "longFormatEnabled", OTC_PyLogger::mfn_longFormatEnabled, 1 },
  { "enableLongFormat", OTC_PyLogger::mfn_enableLongFormat, 1 },
  { "disableLongFormat", OTC_PyLogger::mfn_disableLongFormat, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyLogger::~OTC_PyLogger()
{
  Py_XDECREF(callback_);
}

/* ------------------------------------------------------------------------- */
OTC_PyLogger::OTC_PyLogger(PyObject* theCallback)
  : callback_(theCallback)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  Py_XINCREF(callback_);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyLogger::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theCallback;

  if (!PyArg_ParseTuple(theArgs,"O",&theCallback))
    return 0;

  OTC_PyLogger* theObject;
  theObject = new OTC_PyLogger(theCallback);
  OTCLIB_ASSERT_M(theObject != 0);

  return theObject;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyLogger::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyLogger::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyLogger*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyLogger::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyLogger*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyLogger::mfn_notify(PyObject*, PyObject* theArgs)
{
  OTC_LogLevel theLevel;
  char* theMessage = 0;

  theLevel = OTCLIB_LOG_WARNING;

  if (!PyArg_ParseTuple(theArgs,"is",&theLevel,&theMessage))
    return 0;

  Py_BEGIN_ALLOW_THREADS

  OTC_Logger::notify(theLevel,theMessage);

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyLogger::mfn_notifyChannel(PyObject*, PyObject* theArgs)
{
  OTC_LogLevel theLevel;
  char* theChannel = 0;
  char* theMessage = 0;

  theLevel = OTCLIB_LOG_WARNING;

  if (!PyArg_ParseTuple(theArgs,"sis",&theChannel,&theLevel,&theMessage))
    return 0;

  Py_BEGIN_ALLOW_THREADS

  OTC_Logger::notify(theChannel,theLevel,theMessage);

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyLogger::mfn_logFile(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  return Py_BuildValue("s",OTC_Logger::logFile());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyLogger::mfn_setLogFile(PyObject*, PyObject* theArgs)
{
  char* theLogFile = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theLogFile))
    return 0;

  OTC_Logger::setLogFile(theLogFile);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyLogger::mfn_stderrOutputEnabled(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  return Py_BuildValue("i",OTC_Logger::stderrOutputEnabled());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyLogger::mfn_enableStderrOutput(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Logger::enableStderrOutput();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyLogger::mfn_disableStderrOutput(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Logger::disableStderrOutput();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyLogger::mfn_longFormatEnabled(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  return Py_BuildValue("i",OTC_Logger::longFormatEnabled());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyLogger::mfn_enableLongFormat(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Logger::enableLongFormat();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyLogger::mfn_disableLongFormat(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Logger::disableLongFormat();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
void OTC_PyLogger::log(
 char const* theChannel,
 OTC_LogLevel theLevel,
 char const* theMessage,
 u_int theLength
)
{
  if (theChannel == 0 || *theChannel == EOS)
    return;

  OTC_PyDispatcher::enterPython();

  PyObject* theArgs;
  PyObject* theResult;

  theArgs = Py_BuildValue("(sis#)",
   theChannel,int(theLevel),theMessage,theLength);

  if (theArgs == 0)
  {
    PyErr_Clear();

    OTC_PyDispatcher::leavePython();

    return;
  }

  theResult = PyEval_CallObject(callback_,theArgs);

  Py_XDECREF(theArgs);

  if (theResult == 0)
  {
    PyErr_Clear();

    OTC_PyDispatcher::leavePython();

    return;
  }

  Py_XDECREF(theResult);

  OTC_PyDispatcher::leavePython();
}

/* ------------------------------------------------------------------------- */
