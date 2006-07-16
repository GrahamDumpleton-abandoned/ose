/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyfilesr.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef ANY
#undef ANY
#endif

#include <OTC/message/inetclnt.hh>
#include <OTC/message/inetlstn.hh>

#include <OTC/python/pyfilesr.hh>
#include <OTC/python/pydsptch.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyFileServer::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "FileServer",
  sizeof(OTC_PyFileServer),
  0,
  OTC_PyFileServer::sys_dealloc,
  0,
  OTC_PyFileServer::sys_getattr,
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
PyMethodDef OTC_PyFileServer::gMethods_[] = {
  { "instance", OTC_PyFileServer::mfn_instance, 1 },
  { "directory", OTC_PyFileServer::mfn_directory, 1 },
  { "map", OTC_PyFileServer::mfn_map, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyFileServer::~OTC_PyFileServer()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_PyFileServer::OTC_PyFileServer(
 PyObject* theOuter,
 char const* theDirectory
)
  : OTC_FileServer(theDirectory), outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFileServer::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  char const* theDirectory;

  if (!PyArg_ParseTuple(theArgs,"Os",&theOuter,&theDirectory))
    return 0;

  OTC_PyFileServer* theServer;
  theServer = new OTC_PyFileServer(theOuter,theDirectory);
  OTCLIB_ASSERT_M(theServer != 0);

  return (PyObject*)theServer;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFileServer::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyFileServer::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyFileServer*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFileServer::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyFileServer*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFileServer::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServer* theSelf;
  theSelf = (OTC_PyFileServer*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFileServer::mfn_directory(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_FileServer* theSelf;
  theSelf = (OTC_PyFileServer*)theInstance;

  return Py_BuildValue("s",theSelf->directory().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFileServer::mfn_map(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theExtension;
  char const* theFileType;

  if (!PyArg_ParseTuple(theArgs,"ss",&theExtension,&theFileType))
    return 0;

  OTC_PyFileServer* theSelf;
  theSelf = (OTC_PyFileServer*)theInstance;

  theSelf->map(theExtension,theFileType);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_PyFileServer::authorise(
 char const* theLogin,
 char const* thePassword
)
{
  OTC_PyDispatcher::enterPython();

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,
   "_authorise","ss",theLogin,thePassword);

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();

    OTC_PyDispatcher::leavePython();

    return OTCLIB_FALSE;
  }

  OTC_Boolean theAnswer;
  theAnswer = OTCLIB_TRUE;

  if (theResult == Py_None)
    theAnswer = OTCLIB_FALSE;

  Py_XDECREF(theResult);

  OTC_PyDispatcher::leavePython();

  return theAnswer;
}

/* ------------------------------------------------------------------------- */
