/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyprogrm.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pyprogrm.hh>

#include <OTC/text/slogger.hh>

#include <OTC/stdlib/fstream.h>

/* ------------------------------------------------------------------------- */
OTC_PyProgram* OTC_PyProgram::gInstance_ = 0;

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyProgram::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Program",
  sizeof(OTC_PyProgram),
  0,
  OTC_PyProgram::sys_dealloc,
  0,
  OTC_PyProgram::sys_getattr,
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
PyMethodDef OTC_PyProgram::gMethods_[] = {
  { "removeAllConfig", OTC_PyProgram::mfn_removeAllConfig, 1 },
  { "removeConfig", OTC_PyProgram::mfn_removeConfig, 1 },
  { "loadConfig", OTC_PyProgram::mfn_loadConfig, 1 },
  { "mergeConfig", OTC_PyProgram::mfn_mergeConfig, 1 },
  { "lookupConfig", OTC_PyProgram::mfn_lookupConfig, 1 },
  { "saveConfig", OTC_PyProgram::mfn_saveConfig, 1 },
  { "lookupEnviron", OTC_PyProgram::mfn_lookupEnviron, 1 },
  { "mergeEnviron", OTC_PyProgram::mfn_mergeEnviron, 1 },
  { "expandEnviron", OTC_PyProgram::mfn_expandEnviron, 1 },
  { "processIdentity", OTC_PyProgram::mfn_processIdentity, 1 },
  { "uniqueId", OTC_PyProgram::mfn_uniqueId, 1 },
  { "threadingEnabled", OTC_PyProgram::mfn_threadingEnabled, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyProgram::~OTC_PyProgram()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_PyProgram::OTC_PyProgram()
{
  ob_type = &gType_;

  _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyProgram::instance(PyObject* theModule, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  if (gInstance_ == 0)
  {
    gInstance_ = new OTC_PyProgram;
    OTCLIB_ASSERT_M(gInstance_ != 0);
  }

  Py_XINCREF(gInstance_);

  return (PyObject*)gInstance_;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyProgram::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyProgram::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyProgram*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyProgram::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyProgram*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyProgram::mfn_removeAllConfig(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Program::removeAllConfig();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyProgram::mfn_removeConfig(PyObject*, PyObject* theArgs)
{
  char const* theKey;

  if (!PyArg_ParseTuple(theArgs,"s",&theKey))
    return 0;

  OTC_Program::removeConfig(theKey);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyProgram::mfn_loadConfig(PyObject*, PyObject* theArgs)
{
  char const* theFileName;
  char const* theChannel = "";

  if (!PyArg_ParseTuple(theArgs,"s|z",&theFileName,&theChannel))
    return 0;

  ifstream theStream(theFileName);

  if (theStream.fail())
  {
    PyErr_SetString(PyExc_IOError,
     "invalid configuration file");

    return 0;
  }

  OTC_String tmpChannel;
  tmpChannel = OTC_Program::uniqueId("$CID?",OTCLIB_ID_SHORT_FORMAT);

  OTC_SLogger tmpLogger(tmpChannel);

  OTC_Program::loadConfig(theStream,tmpChannel);

  OTC_String tmpMessages(tmpLogger.messages());

  if (theChannel != 0 && !tmpMessages.isEmpty())
  {
    OTC_Logger::notify(theChannel,OTCLIB_LOG_DEBUG,
     tmpMessages,tmpMessages.length());
  }

  if (!tmpMessages.isEmpty())
    return Py_BuildValue("s#",tmpMessages.string(),tmpMessages.length());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyProgram::mfn_mergeConfig(PyObject*, PyObject* theArgs)
{
  char const* theKey;
  char const* theValue;

  if (!PyArg_ParseTuple(theArgs,"ss",&theKey,&theValue))
    return 0;

  OTC_Program::mergeConfig(theKey,theValue);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyProgram::mfn_lookupConfig(PyObject*, PyObject* theArgs)
{
  char const* theKey;

  if (!PyArg_ParseTuple(theArgs,"s",&theKey))
    return 0;

  OTC_String theValue;
  theValue = OTC_Program::lookupConfig(theKey);

  if (theValue.isUndefined())
  {
    Py_XINCREF(Py_None);

    return Py_None;
  }

  return Py_BuildValue("s",theValue.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyProgram::mfn_saveConfig(PyObject*, PyObject* theArgs)
{
  char const* theFileName;

  if (!PyArg_ParseTuple(theArgs,"s",&theFileName))
    return 0;

  ofstream theStream(theFileName);

  OTC_Program::saveConfig(theStream);

  if (theStream.fail())
  {
    PyErr_SetString(PyExc_IOError,
     "error writing configuration file");

    return 0;
  }

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyProgram::mfn_lookupEnviron(PyObject*, PyObject* theArgs)
{
  char const* theKey;

  if (!PyArg_ParseTuple(theArgs,"s",&theKey))
    return 0;

  OTC_String theValue;
  theValue = OTC_Program::lookupEnviron(theKey);

  if (theValue.isUndefined())
  {
    Py_XINCREF(Py_None);

    return Py_None;
  }

  return Py_BuildValue("s",theValue.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyProgram::mfn_mergeEnviron(PyObject*, PyObject* theArgs)
{
  char const* theKey;
  char const* theValue;

  if (!PyArg_ParseTuple(theArgs,"ss",&theKey,&theValue))
    return 0;

  OTC_Program::mergeEnviron(theKey,theValue);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyProgram::mfn_expandEnviron(PyObject*, PyObject* theArgs)
{
  char const* theString;

  if (!PyArg_ParseTuple(theArgs,"s",&theString))
    return 0;

  OTC_String theValue;
  theValue = OTC_Program::expandEnviron(theString);

  return Py_BuildValue("s",theValue.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyProgram::mfn_processIdentity(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_String theResult;
  theResult = OTC_Program::processIdentity();

  return Py_BuildValue("s",theResult.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyProgram::mfn_uniqueId(PyObject*, PyObject* theArgs)
{
  int theType = 0;
  char const* thePrefix = 0;

  if (!PyArg_ParseTuple(theArgs,"i|z",&theType,&thePrefix))
    return 0;

  OTC_String theResult;
  theResult = OTC_Program::uniqueId(thePrefix,OTC_UniqueIdType(theType));

  return Py_BuildValue("s",theResult.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyProgram::mfn_threadingEnabled(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Boolean theResult;
  theResult = OTC_Program::threadingEnabled();

  return Py_BuildValue("i",theResult);
}

/* ------------------------------------------------------------------------- */
