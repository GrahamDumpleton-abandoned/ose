/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpysrvbnd.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pysrvbnd.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyServiceBinding::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "ServiceBinding",
  sizeof(OTC_PyServiceBinding),
  0,
  OTC_PyServiceBinding::sys_dealloc,
  0,
  OTC_PyServiceBinding::sys_getattr,
  0,
  OTC_PyServiceBinding::sys_compare,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

/* ------------------------------------------------------------------------- */
PyMethodDef OTC_PyServiceBinding::gMethods_[] = {
  { "serviceName", OTC_PyServiceBinding::mfn_serviceName, 1 },
  { "serviceAudience", OTC_PyServiceBinding::mfn_serviceAudience, 1 },
  { "receiverName", OTC_PyServiceBinding::mfn_receiverName, 1 },
  { "agentIdentity", OTC_PyServiceBinding::mfn_agentIdentity, 1 },
  { "processAddress", OTC_PyServiceBinding::mfn_processAddress, 1 },
  { "processIdentity", OTC_PyServiceBinding::mfn_processIdentity, 1 },
  { "networkGroup", OTC_PyServiceBinding::mfn_networkGroup, 1 },
  { "agentAddress", OTC_PyServiceBinding::mfn_agentAddress, 1 },
  { "serviceAddress", OTC_PyServiceBinding::mfn_serviceAddress, 1 },
  { "serviceLocation", OTC_PyServiceBinding::mfn_serviceLocation, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyServiceBinding::~OTC_PyServiceBinding()
{
  serviceBinding_->destroy();
}

/* ------------------------------------------------------------------------- */
OTC_PyServiceBinding::OTC_PyServiceBinding(
 OTC_ServiceBinding* theServiceBinding
)
  : serviceBinding_(0)
{
  OTCLIB_ENSURE_FN((theServiceBinding != 0),
   "OTC_PyServiceBinding::OTC_PyServiceBinding(OTC_ServiceBinding*)",
   "invalid service binding supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  serviceBinding_ = theServiceBinding->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyServiceBinding::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
int OTC_PyServiceBinding::obj_compare(
 OTC_PyServiceBinding const* theObject
) const
{
  return serviceBinding_->serviceAddress().rank(
   ((OTC_PyServiceBinding*)theObject)->serviceBinding_->serviceAddress());
}

/* ------------------------------------------------------------------------- */
void OTC_PyServiceBinding::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyServiceBinding*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyServiceBinding::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyServiceBinding*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
int OTC_PyServiceBinding::sys_compare(
 PyObject* theInstance,
 PyObject* theObject
)
{
  return ((OTC_PyServiceBinding*)theInstance)->obj_compare(
   (OTC_PyServiceBinding*)theObject);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyServiceBinding::mfn_serviceName(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OTC_PyServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->serviceName().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyServiceBinding::mfn_serviceAudience(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OTC_PyServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->serviceAudience().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyServiceBinding::mfn_receiverName(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OTC_PyServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->receiverName().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyServiceBinding::mfn_agentIdentity(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OTC_PyServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->agentIdentity().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyServiceBinding::mfn_processAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OTC_PyServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->processAddress().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyServiceBinding::mfn_processIdentity(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OTC_PyServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->processIdentity().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyServiceBinding::mfn_networkGroup(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OTC_PyServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->networkGroup().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyServiceBinding::mfn_agentAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OTC_PyServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->agentAddress().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyServiceBinding::mfn_serviceAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OTC_PyServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->serviceAddress().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyServiceBinding::mfn_serviceLocation(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OTC_PyServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("i",theBinding->serviceLocation());
}

/* ------------------------------------------------------------------------- */
