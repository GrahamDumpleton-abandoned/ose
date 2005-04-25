/*
// ============================================================================
//
// = FILENAME
//     opyrequest.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "request.hh"
#include "servbind.hh"
#include "service.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_Request::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Request",
  sizeof(OPY_Request),
  0,
  OPY_Request::sys_dealloc,
  0,
  OPY_Request::sys_getattr,
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
PyMethodDef OPY_Request::gMethods_[] = {
  { "sender", OPY_Request::mfn_sender, 1 },
  { "conversationId", OPY_Request::mfn_conversationId, 1 },
  { "sequenceNumber", OPY_Request::mfn_sequenceNumber, 1 },
  { "methodName", OPY_Request::mfn_methodName, 1 },
  { "content", OPY_Request::mfn_content, 1 },
  { "bounceRequest", OPY_Request::mfn_bounceRequest, 1 },
  { "sendResponse", OPY_Request::mfn_sendResponse, 1 },
  { "sendFailure", OPY_Request::mfn_sendFailure, 1 },
  { "properties", OPY_Request::mfn_properties, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_Request::~OPY_Request()
{
  if (properties_ != 0)
    Py_XDECREF(properties_);

  request_->destroy();
}

/* ------------------------------------------------------------------------- */
OPY_Request::OPY_Request(OTCEV_Request* theRequest)
  : request_(0), properties_(0)
{
  OTCLIB_ENSURE_FN((theRequest != 0),
   "OPY_Request::OPY_Request(OTCEV_Request*)",
   "invalid request supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  request_ = (OTCEV_Request*)theRequest->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Request::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_Request::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_Request*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Request::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_Request*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Request::mfn_sender(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Request* theRequest;
  theRequest = ((OPY_Request*)theInstance)->request_;

  return Py_BuildValue("s",
   theRequest->sender().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Request::mfn_conversationId(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Request* theRequest;
  theRequest = ((OPY_Request*)theInstance)->request_;

  return Py_BuildValue("s",
   theRequest->conversationId().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Request::mfn_sequenceNumber(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Request* theRequest;
  theRequest = ((OPY_Request*)theInstance)->request_;

  return Py_BuildValue("i",
   theRequest->sequenceNumber());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Request::mfn_methodName(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Request* theRequest;
  theRequest = ((OPY_Request*)theInstance)->request_;

  return Py_BuildValue("s",
   theRequest->methodName().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Request::mfn_content(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theDecoder = 0;

  if (!PyArg_ParseTuple(theArgs,"O",&theDecoder))
    return 0;

  OTCEV_Request* theRequest;
  theRequest = ((OPY_Request*)theInstance)->request_;

  return OPY_Service::payloadToObject(theRequest->content(),theDecoder);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Request::mfn_bounceRequest(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theBindingObject = 0;
  char const* theMethodName = 0;

  if (!PyArg_ParseTuple(theArgs,"O|z",&theBindingObject,&theMethodName))
    return 0;

  if (!OPY_ServiceBinding::isType(theBindingObject))
  {
    PyErr_SetString(PyExc_TypeError,
     "agent description not a service binding object");

    return 0;
  }

  OTC_ServiceBinding* theBinding;
  theBinding = ((OPY_ServiceBinding*)theBindingObject)->binding();

  OTCEV_Request* theRequest;
  theRequest = ((OPY_Request*)theInstance)->request_;

  theRequest->bounceRequest(theBinding,theMethodName);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Request::mfn_sendResponse(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* thePayloadObject = 0;
  PyObject* theEncoder = 0;

  if (!PyArg_ParseTuple(theArgs,"OO",&thePayloadObject,&theEncoder))
  {
    return 0;
  }

  OTCEV_Request* theRequest;
  theRequest = ((OPY_Request*)theInstance)->request_;

  OTC_SVPayload thePayload;

  if (!OPY_Service::objectToPayload(thePayloadObject,thePayload,theEncoder))
  {
    return 0;
  }

  theRequest->sendResponse(thePayload);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Request::mfn_sendFailure(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theError;
  char const* theDescription = 0;
  char const* theOrigin = 0;
  char const* theDetails = 0;

  if (!PyArg_ParseTuple(theArgs,"i|zzz",&theError,
   &theDescription,&theOrigin,&theDetails))
  {
    return 0;
  }

  OTCEV_Request* theRequest;
  theRequest = ((OPY_Request*)theInstance)->request_;

  theRequest->sendFailure(theError,theDescription,theOrigin,theDetails);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Request::mfn_properties(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OPY_Request* theSelf;
  theSelf = (OPY_Request*)theInstance;

  if (theSelf->properties_ != 0)
  {
    Py_XINCREF(theSelf->properties_);

    return theSelf->properties_;
  }

  theSelf->properties_ = PyDict_New();

  if (theSelf->properties_ == 0)
    return 0;

  OTC_DOMProperty const* theProperty;
  theProperty = theSelf->request_->content().properties();

  while (theProperty != 0)
  {
    PyObject* theKey = 0;
    PyObject* theValue = 0;

    theKey = PyString_FromStringAndSize(
     theProperty->name().string(),theProperty->name().length());
    theValue = PyString_FromStringAndSize(
     theProperty->value().string(),theProperty->value().length());

    if (PyDict_SetItem(theSelf->properties_,theKey,theValue) == -1)
    {
      Py_XDECREF(theKey);
      Py_XDECREF(theValue);
      Py_XDECREF(theSelf->properties_);

      theSelf->properties_ = 0;

      return 0;
    }

    Py_XDECREF(theKey);
    Py_XDECREF(theValue);

    theProperty = theProperty->next();
  }

  Py_XINCREF(theSelf->properties_);

  return theSelf->properties_;
}

/* ------------------------------------------------------------------------- */
