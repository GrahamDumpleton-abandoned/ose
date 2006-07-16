/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyhttpdm.cc
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

#include <OTC/python/pyhttpdm.hh>
#include <OTC/python/pydsptch.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyHttpDaemon::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "HttpDaemon",
  sizeof(OTC_PyHttpDaemon),
  0,
  OTC_PyHttpDaemon::sys_dealloc,
  0,
  OTC_PyHttpDaemon::sys_getattr,
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
PyMethodDef OTC_PyHttpDaemon::gMethods_[] = {
  { "port", OTC_PyHttpDaemon::mfn_port, 1 },
  { "start", OTC_PyHttpDaemon::mfn_start, 1 },
  { "stop", OTC_PyHttpDaemon::mfn_stop, 1 },
  { "attach", OTC_PyHttpDaemon::mfn_attach, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyHttpDaemon::~OTC_PyHttpDaemon()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_PyHttpDaemon::OTC_PyHttpDaemon(PyObject* theOuter, int thePort)
  : OTC_HttpDaemon(thePort), outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpDaemon::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  int thePort;

  if (!PyArg_ParseTuple(theArgs,"Oi",&theOuter,&thePort))
    return 0;

  OTC_PyHttpDaemon* theDaemon;
  theDaemon = new OTC_PyHttpDaemon(theOuter,thePort);
  OTCLIB_ASSERT_M(theDaemon != 0);

  return (PyObject*)theDaemon;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpDaemon::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyHttpDaemon::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyHttpDaemon*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpDaemon::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyHttpDaemon*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpDaemon::mfn_port(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpDaemon* theSelf;
  theSelf = (OTC_PyHttpDaemon*)theInstance;

  return Py_BuildValue("i",theSelf->port());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpDaemon::mfn_start(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpDaemon* theSelf;
  theSelf = (OTC_PyHttpDaemon*)theInstance;

  theSelf->start();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpDaemon::mfn_stop(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpDaemon* theSelf;
  theSelf = (OTC_PyHttpDaemon*)theInstance;

  theSelf->stop();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpDaemon::mfn_attach(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theServerPath;
  PyObject* theObject;

  if (!PyArg_ParseTuple(theArgs,"sO",&theServerPath,&theObject))
    return 0;

  OTC_HttpDaemon* theSelf;
  theSelf = (OTC_PyHttpDaemon*)theInstance;

  OTC_HttpServer* theServer;
  theServer = (OTC_HttpServer*)PyCObject_AsVoidPtr(theObject);

  theSelf->attach(theServerPath,theServer);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_PyHttpDaemon::authorise(
 OTC_String const& theClientHost
)
{
  OTC_PyDispatcher::enterPython();

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,
   "_authorise","s",theClientHost.string());

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
