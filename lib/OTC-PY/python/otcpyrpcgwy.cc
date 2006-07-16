/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyrpcgwy.cc
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

#include <OTC/python/pyrpcgwy.hh>
#include <OTC/python/pydsptch.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyRpcGateway::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "RpcGateway",
  sizeof(OTC_PyRpcGateway),
  0,
  OTC_PyRpcGateway::sys_dealloc,
  0,
  OTC_PyRpcGateway::sys_getattr,
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
PyMethodDef OTC_PyRpcGateway::gMethods_[] = {
  { "instance", OTC_PyRpcGateway::mfn_instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyRpcGateway::~OTC_PyRpcGateway()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_PyRpcGateway::OTC_PyRpcGateway(
 PyObject* theOuter,
 char const* theServiceGroup
)
  : OTC_RpcGateway(theServiceGroup), outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRpcGateway::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  char const* theServiceGroup;

  if (!PyArg_ParseTuple(theArgs,"Os",&theOuter,&theServiceGroup))
    return 0;

  OTC_PyRpcGateway* theServer;
  theServer = new OTC_PyRpcGateway(theOuter,theServiceGroup);
  OTCLIB_ASSERT_M(theServer != 0);

  return (PyObject*)theServer;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRpcGateway::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyRpcGateway::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyRpcGateway*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRpcGateway::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyRpcGateway*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRpcGateway::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServer* theSelf;
  theSelf = (OTC_PyRpcGateway*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_PyRpcGateway::authorise(
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
