/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyechosl.cc
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

#include <OTC/python/pyechosl.hh>
#include <OTC/python/pydsptch.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyEchoServlet::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "EchoServlet",
  sizeof(OTC_PyEchoServlet),
  0,
  OTC_PyEchoServlet::sys_dealloc,
  0,
  OTC_PyEchoServlet::sys_getattr,
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
PyMethodDef OTC_PyEchoServlet::gMethods_[] = {
  { "instance", OTC_PyEchoServlet::mfn_instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyEchoServlet::~OTC_PyEchoServlet()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_PyEchoServlet::OTC_PyEchoServlet(
 PyObject* theOuter,
 OTC_HttpSession* theSession
)
  : OTC_EchoServlet(theSession),
    outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  Py_XINCREF(outer_);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyEchoServlet::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  PyObject* theObject = 0;

  if (!PyArg_ParseTuple(theArgs,"OO",&theOuter,&theObject))
    return 0;

  OTC_HttpSession* theSession;
  theSession = (OTC_HttpSession*)PyCObject_AsVoidPtr(theObject);

  OTC_PyEchoServlet* theServlet;
  theServlet = new OTC_PyEchoServlet(theOuter,theSession);
  OTCLIB_ASSERT_M(theServlet != 0);

  return (PyObject*)theServlet;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyEchoServlet::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyEchoServlet::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyEchoServlet*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyEchoServlet::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyEchoServlet*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyEchoServlet::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_EchoServlet* theSelf;
  theSelf = (OTC_PyEchoServlet*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
void OTC_PyEchoServlet::destroy()
{
  OTC_PyDispatcher::enterPython();

  PyObject* theOuter;
  theOuter = outer_;

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,"_destroyServlet","");

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();
  }
  else
    Py_XDECREF(theResult);

  Py_XDECREF(theOuter);

  OTC_PyDispatcher::leavePython();
}

/* ------------------------------------------------------------------------- */
