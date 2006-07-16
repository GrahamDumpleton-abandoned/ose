/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyopaque.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pyopaque.hh>
#include <OTC/python/pysrvbnd.hh>
#include <OTC/python/pyservce.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyOpaque::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Opaque",
  sizeof(OTC_PyOpaque),
  0,
  OTC_PyOpaque::sys_dealloc,
  0,
  OTC_PyOpaque::sys_getattr,
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
PyMethodDef OTC_PyOpaque::gMethods_[] = {
  { "sender", OTC_PyOpaque::mfn_sender, 1 },
  { "conversationId", OTC_PyOpaque::mfn_conversationId, 1 },
  { "sequenceNumber", OTC_PyOpaque::mfn_sequenceNumber, 1 },
  { "messageId", OTC_PyOpaque::mfn_messageId, 1 },
  { "description", OTC_PyOpaque::mfn_description, 1 },
  { "content", OTC_PyOpaque::mfn_content, 1 },
  { "contentType", OTC_PyOpaque::mfn_contentType, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyOpaque::~OTC_PyOpaque()
{
  message_->destroy();
}

/* ------------------------------------------------------------------------- */
OTC_PyOpaque::OTC_PyOpaque(OTCEV_Opaque* theMessage)
  : message_(0)
{
  OTCLIB_ENSURE_FN((theMessage != 0),
   "OTC_PyOpaque::OTC_PyOpaque(OTCEV_Opaque*)",
   "invalid request supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  message_ = (OTCEV_Opaque*)theMessage->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyOpaque::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyOpaque::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyOpaque*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyOpaque::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyOpaque*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyOpaque::mfn_sender(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Opaque* theMessage;
  theMessage = ((OTC_PyOpaque*)theInstance)->message_;

  return Py_BuildValue("s",
   theMessage->sender().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyOpaque::mfn_conversationId(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Opaque* theMessage;
  theMessage = ((OTC_PyOpaque*)theInstance)->message_;

  return Py_BuildValue("s",
   theMessage->conversationId().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyOpaque::mfn_sequenceNumber(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Opaque* theMessage;
  theMessage = ((OTC_PyOpaque*)theInstance)->message_;

  return Py_BuildValue("i",
   theMessage->sequenceNumber());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyOpaque::mfn_messageId(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Opaque* theMessage;
  theMessage = ((OTC_PyOpaque*)theInstance)->message_;

  return Py_BuildValue("s",
   theMessage->messageId().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyOpaque::mfn_description(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Opaque* theMessage;
  theMessage = ((OTC_PyOpaque*)theInstance)->message_;

  return Py_BuildValue("s",
   theMessage->description().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyOpaque::mfn_content(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Opaque* theMessage;
  theMessage = ((OTC_PyOpaque*)theInstance)->message_;

  return Py_BuildValue("s#",
   theMessage->content().string(),theMessage->content().length());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyOpaque::mfn_contentType(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Opaque* theMessage;
  theMessage = ((OTC_PyOpaque*)theInstance)->message_;

  return Py_BuildValue("s",
   theMessage->contentType().string());
}

/* ------------------------------------------------------------------------- */
