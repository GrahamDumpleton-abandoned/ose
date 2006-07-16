/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyxmlrsl.cc
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

#include <OTC/python/pyxmlrsl.hh>
#include <OTC/python/pysrvbnd.hh>
#include <OTC/python/pydsptch.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyXmlRpcServlet::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "XmlRpcServlet",
  sizeof(OTC_PyXmlRpcServlet),
  0,
  OTC_PyXmlRpcServlet::sys_dealloc,
  0,
  OTC_PyXmlRpcServlet::sys_getattr,
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
PyMethodDef OTC_PyXmlRpcServlet::gMethods_[] = {
  { "instance", OTC_PyXmlRpcServlet::mfn_instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyXmlRpcServlet::~OTC_PyXmlRpcServlet()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_PyXmlRpcServlet::OTC_PyXmlRpcServlet(
 PyObject* theOuter,
 OTC_HttpSession* theSession,
 OTC_ServiceBinding* theBinding
)
  : OTC_XmlRpcServlet(theSession,theBinding), outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  Py_XINCREF(outer_);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyXmlRpcServlet::create(PyObject* theModule, PyObject* theArgs)
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

  OTC_PyXmlRpcServlet* theServlet;
  theServlet = new OTC_PyXmlRpcServlet(theOuter,theSession,theBinding);
  OTCLIB_ASSERT_M(theServlet != 0);

  return (PyObject*)theServlet;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyXmlRpcServlet::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyXmlRpcServlet::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyXmlRpcServlet*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyXmlRpcServlet::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyXmlRpcServlet*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyXmlRpcServlet::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_XmlRpcServlet* theSelf;
  theSelf = (OTC_PyXmlRpcServlet*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
void OTC_PyXmlRpcServlet::destroy()
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
