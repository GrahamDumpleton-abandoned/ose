/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyrquest.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pyrquest.hh>
#include <OTC/python/pysrvbnd.hh>
#include <OTC/python/pyservce.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyRequest::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Request",
  sizeof(OTC_PyRequest),
  0,
  OTC_PyRequest::sys_dealloc,
  0,
  OTC_PyRequest::sys_getattr,
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
PyMethodDef OTC_PyRequest::gMethods_[] = {
  { "sender", OTC_PyRequest::mfn_sender, 1 },
  { "conversationId", OTC_PyRequest::mfn_conversationId, 1 },
  { "sequenceNumber", OTC_PyRequest::mfn_sequenceNumber, 1 },
  { "methodName", OTC_PyRequest::mfn_methodName, 1 },
  { "content", OTC_PyRequest::mfn_content, 1 },
  { "bounceRequest", OTC_PyRequest::mfn_bounceRequest, 1 },
  { "sendResponse", OTC_PyRequest::mfn_sendResponse, 1 },
  { "sendFailure", OTC_PyRequest::mfn_sendFailure, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyRequest::~OTC_PyRequest()
{
  request_->destroy();
}

/* ------------------------------------------------------------------------- */
OTC_PyRequest::OTC_PyRequest(OTCEV_Request* theRequest)
  : request_(0)
{
  OTCLIB_ENSURE_FN((theRequest != 0),
   "OTC_PyRequest::OTC_PyRequest(OTCEV_Request*)",
   "invalid request supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  request_ = (OTCEV_Request*)theRequest->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRequest::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyRequest::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyRequest*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRequest::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyRequest*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRequest::mfn_sender(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Request* theRequest;
  theRequest = ((OTC_PyRequest*)theInstance)->request_;

  return Py_BuildValue("s",
   theRequest->sender().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRequest::mfn_conversationId(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Request* theRequest;
  theRequest = ((OTC_PyRequest*)theInstance)->request_;

  return Py_BuildValue("s",
   theRequest->conversationId().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRequest::mfn_sequenceNumber(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Request* theRequest;
  theRequest = ((OTC_PyRequest*)theInstance)->request_;

  return Py_BuildValue("i",
   theRequest->sequenceNumber());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRequest::mfn_methodName(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Request* theRequest;
  theRequest = ((OTC_PyRequest*)theInstance)->request_;

  return Py_BuildValue("s",
   theRequest->methodName().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRequest::mfn_content(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theDecoder = 0;

  if (!PyArg_ParseTuple(theArgs,"O",&theDecoder))
    return 0;

  OTCEV_Request* theRequest;
  theRequest = ((OTC_PyRequest*)theInstance)->request_;

  return OTC_PyService::payloadToObject(theRequest->content(),theDecoder);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRequest::mfn_bounceRequest(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theBindingObject = 0;
  char const* theMethodName = 0;

  if (!PyArg_ParseTuple(theArgs,"O|z",&theBindingObject,&theMethodName))
    return 0;

  if (!OTC_PyServiceBinding::isType(theBindingObject))
  {
    PyErr_SetString(PyExc_TypeError,
     "agent description not a service binding object");

    return 0;
  }

  OTC_ServiceBinding* theBinding;
  theBinding = ((OTC_PyServiceBinding*)theBindingObject)->binding();

  OTCEV_Request* theRequest;
  theRequest = ((OTC_PyRequest*)theInstance)->request_;

  theRequest->bounceRequest(theBinding,theMethodName);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRequest::mfn_sendResponse(
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
  theRequest = ((OTC_PyRequest*)theInstance)->request_;

  OTC_SVPayload thePayload;

  if (!OTC_PyService::objectToPayload(thePayloadObject,thePayload,theEncoder))
  {
    return 0;
  }

  theRequest->sendResponse(thePayload);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyRequest::mfn_sendFailure(
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
  theRequest = ((OTC_PyRequest*)theInstance)->request_;

  theRequest->sendFailure(theError,theDescription,theOrigin,theDetails);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
