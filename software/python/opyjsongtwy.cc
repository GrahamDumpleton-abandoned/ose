/*
// ============================================================================
//
// = FILENAME
//     opyjsongtwy.cc
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

#include "jsongtwy.hh"
#include "dispatch.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_JsonRpcGateway::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "RpcGateway",
  sizeof(OPY_JsonRpcGateway),
  0,
  OPY_JsonRpcGateway::sys_dealloc,
  0,
  OPY_JsonRpcGateway::sys_getattr,
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
PyMethodDef OPY_JsonRpcGateway::gMethods_[] = {
  { "instance", OPY_JsonRpcGateway::mfn_instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_JsonRpcGateway::~OPY_JsonRpcGateway()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OPY_JsonRpcGateway::OPY_JsonRpcGateway(
 PyObject* theOuter,
 char const* theSelection,
 OTC_ServiceSubscription theType
)
  : OTC_JsonRpcGateway(theSelection,theType), outer_(theOuter),
    interpreterState_(0)
{
  ob_type = &gType_;

  _Py_NewReference(this);

#ifdef WITH_THREAD
  PyEval_InitThreads();
  interpreterState_ = PyThreadState_Get()->interp;
#endif
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_JsonRpcGateway::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  char const* theSelection;
  int theType;

  if (!PyArg_ParseTuple(theArgs,"Osi",&theOuter,&theSelection,&theType))
    return 0;

  OPY_JsonRpcGateway* theServer;
  theServer = new OPY_JsonRpcGateway(theOuter,
   theSelection,OTC_ServiceSubscription(theType));
  OTCLIB_ASSERT_M(theServer != 0);

  return (PyObject*)theServer;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_JsonRpcGateway::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_JsonRpcGateway::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_JsonRpcGateway*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_JsonRpcGateway::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_JsonRpcGateway*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_JsonRpcGateway::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServer* theSelf;
  theSelf = (OPY_JsonRpcGateway*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
bool OPY_JsonRpcGateway::authorise(
 char const* theLogin,
 char const* thePassword
)
{
  OPY_Dispatcher::enterPython(interpreterState_);

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,
   "_authorise","ss",theLogin,thePassword);

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();

    return false;
  }

  bool theAnswer;
  theAnswer = true;

  if (theResult == Py_None)
    theAnswer = false;

  Py_XDECREF(theResult);

  OPY_Dispatcher::leavePython(interpreterState_);

  return theAnswer;
}

/* ------------------------------------------------------------------------- */
