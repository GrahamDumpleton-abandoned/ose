/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyrspnse.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pyrspnse.hh>
#include <OTC/python/pysrvbnd.hh>
#include <OTC/python/pyservce.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyResponse::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Response",
  sizeof(OTC_PyResponse),
  0,
  OTC_PyResponse::sys_dealloc,
  0,
  OTC_PyResponse::sys_getattr,
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
PyMethodDef OTC_PyResponse::gMethods_[] = {
  { "sender", OTC_PyResponse::mfn_sender, 1 },
  { "conversationId", OTC_PyResponse::mfn_conversationId, 1 },
  { "sequenceNumber", OTC_PyResponse::mfn_sequenceNumber, 1 },
  { "content", OTC_PyResponse::mfn_content, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyResponse::~OTC_PyResponse()
{
  response_->destroy();
}

/* ------------------------------------------------------------------------- */
OTC_PyResponse::OTC_PyResponse(OTCEV_Response* theResponse)
  : response_(0)
{
  OTCLIB_ENSURE_FN((theResponse != 0),
   "OTC_PyResponse::OTC_PyResponse(OTCEV_Response*)",
   "invalid response supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  response_ = (OTCEV_Response*)theResponse->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyResponse::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyResponse::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyResponse*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyResponse::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyResponse*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyResponse::mfn_sender(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Response* theResponse;
  theResponse = ((OTC_PyResponse*)theInstance)->response_;

  return Py_BuildValue("s",
   theResponse->sender().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyResponse::mfn_conversationId(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Response* theResponse;
  theResponse = ((OTC_PyResponse*)theInstance)->response_;

  return Py_BuildValue("s",
   theResponse->conversationId().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyResponse::mfn_sequenceNumber(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Response* theResponse;
  theResponse = ((OTC_PyResponse*)theInstance)->response_;

  return Py_BuildValue("i",
   theResponse->sequenceNumber());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyResponse::mfn_content(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theDecoder = 0;

  if (!PyArg_ParseTuple(theArgs,"O",&theDecoder))
    return 0;

  OTCEV_Response* theResponse;
  theResponse = ((OTC_PyResponse*)theInstance)->response_;

  return OTC_PyService::payloadToObject(theResponse->content(),theDecoder);
}

/* ------------------------------------------------------------------------- */
