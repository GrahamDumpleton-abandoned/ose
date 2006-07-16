/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyhttpsr.cc
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

#include <OTC/python/pyhttpsr.hh>
#include <OTC/python/pyhttpsn.hh>
#include <OTC/python/pydsptch.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyHttpServer::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "HttpServer",
  sizeof(OTC_PyHttpServer),
  0,
  OTC_PyHttpServer::sys_dealloc,
  0,
  OTC_PyHttpServer::sys_getattr,
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
PyMethodDef OTC_PyHttpServer::gMethods_[] = {
  { "instance", OTC_PyHttpServer::mfn_instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyHttpServer::~OTC_PyHttpServer()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_PyHttpServer::OTC_PyHttpServer(PyObject* theOuter)
  : outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServer::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;

  if (!PyArg_ParseTuple(theArgs,"O",&theOuter))
    return 0;

  OTC_PyHttpServer* theDaemon;
  theDaemon = new OTC_PyHttpServer(theOuter);
  OTCLIB_ASSERT_M(theDaemon != 0);

  return (PyObject*)theDaemon;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServer::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyHttpServer::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyHttpServer*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServer::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyHttpServer*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServer::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServer* theSelf;
  theSelf = (OTC_PyHttpServer*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_PyHttpServer::authorise(
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
OTC_HttpServlet* OTC_PyHttpServer::servlet(OTC_HttpSession* theSession)
{
  OTC_PyDispatcher::enterPython();

  PyObject* theObject;
  theObject = new OTC_PyHttpSession(theSession);
  OTCLIB_ASSERT_M(theObject != 0);

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,"_servlet","O",theObject);

  Py_XDECREF(theObject);

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();

    OTC_PyDispatcher::leavePython();

    return 0;
  }

  OTC_HttpServlet* theServlet = 0;

  if (theResult != Py_None)
    theServlet = (OTC_HttpServlet*)PyCObject_AsVoidPtr(theResult);

  Py_XDECREF(theResult);

  OTC_PyDispatcher::leavePython();

  return theServlet;
}

/* ------------------------------------------------------------------------- */
