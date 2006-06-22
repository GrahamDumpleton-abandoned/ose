/*
// ============================================================================
//
// = FILENAME
//     opyjsonsrvl.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2006 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef ANY
#undef ANY
#endif

#include <OTC/message/inetclnt.hh>
#include <OTC/message/inetlstn.hh>

#include "jsonsrvl.hh"
#include "servbind.hh"
#include "dispatch.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_JsonRpcServlet::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "JsonRpcServlet",
  sizeof(OPY_JsonRpcServlet),
  0,
  OPY_JsonRpcServlet::sys_dealloc,
  0,
  OPY_JsonRpcServlet::sys_getattr,
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
PyMethodDef OPY_JsonRpcServlet::gMethods_[] = {
  { "instance", OPY_JsonRpcServlet::mfn_instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_JsonRpcServlet::~OPY_JsonRpcServlet()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OPY_JsonRpcServlet::OPY_JsonRpcServlet(
 PyObject* theOuter,
 OTC_HttpSession* theSession,
 OTC_ServiceBinding* theBinding
)
  : OTC_JsonRpcServlet(theSession,theBinding), outer_(theOuter),
    interpreterState_(0)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  Py_XINCREF(outer_);

#ifdef WITH_THREAD
  PyEval_InitThreads();
  interpreterState_ = PyThreadState_Get()->interp;
#endif
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_JsonRpcServlet::create(PyObject* theModule, PyObject* theArgs)
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
  theBinding = ((OPY_ServiceBinding*)theBindingObject)->binding();

  OPY_JsonRpcServlet* theServlet;
  theServlet = new OPY_JsonRpcServlet(theOuter,theSession,theBinding);
  OTCLIB_ASSERT_M(theServlet != 0);

  return (PyObject*)theServlet;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_JsonRpcServlet::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_JsonRpcServlet::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_JsonRpcServlet*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_JsonRpcServlet::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_JsonRpcServlet*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_JsonRpcServlet::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_JsonRpcServlet* theSelf;
  theSelf = (OPY_JsonRpcServlet*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
void OPY_JsonRpcServlet::destroy()
{
  OPY_Dispatcher::enterPython(interpreterState_);

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

  OPY_Dispatcher::leavePython(interpreterState_);
}

/* ------------------------------------------------------------------------- */
