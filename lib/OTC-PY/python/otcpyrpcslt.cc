/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyrpcslt.cc
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

#include <OTC/python/pyrpcslt.hh>
#include <OTC/python/pysrvbnd.hh>
#include <OTC/python/pydsptch.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyRpcServlet::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "RpcServlet",
  sizeof(OTC_PyRpcServlet),
  0,
  OTC_PyRpcServlet::sys_dealloc,
  0,
  OTC_PyRpcServlet::sys_getattr,
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
PyMethodDef OTC_PyRpcServlet::gMethods_[] = {
  { "instance", OTC_PyRpcServlet::mfn_instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyRpcServlet::~OTC_PyRpcServlet()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_PyRpcServlet::OTC_PyRpcServlet(
 PyObject* theOuter,
 OTC_HttpSession* theSession,
 OTC_ServiceBinding* theBinding
)
  : OTC_RpcServlet(theSession,theBinding), outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  Py_XINCREF(outer_);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRpcServlet::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  PyObject* theObject = 0;
  PyObject* theBindingObject = 0;

  if (!PyArg_ParseTuple(theArgs,"OOO",&theOuter,
   &theObject,&theBindingObject))
    return 0;

  OTC_HttpSession* theSession;
  theSession = (OTC_HttpSession*)PyCObject_AsVoidPtr(theObject);

  OTC_ServiceBinding* theBinding;
  theBinding = ((OTC_PyServiceBinding*)theBindingObject)->binding();

  OTC_PyRpcServlet* theServlet;
  theServlet = new OTC_PyRpcServlet(theOuter,theSession,theBinding);
  OTCLIB_ASSERT_M(theServlet != 0);

  return (PyObject*)theServlet;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRpcServlet::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyRpcServlet::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyRpcServlet*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRpcServlet::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyRpcServlet*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRpcServlet::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_RpcServlet* theSelf;
  theSelf = (OTC_PyRpcServlet*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
void OTC_PyRpcServlet::destroy()
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
