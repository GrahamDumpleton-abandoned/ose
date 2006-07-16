/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyfailre.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pyfailre.hh>
#include <OTC/python/pysrvbnd.hh>
#include <OTC/python/pyservce.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyFailure::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Failure",
  sizeof(OTC_PyFailure),
  0,
  OTC_PyFailure::sys_dealloc,
  0,
  OTC_PyFailure::sys_getattr,
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
PyMethodDef OTC_PyFailure::gMethods_[] = {
  { "conversationId", OTC_PyFailure::mfn_conversationId, 1 },
  { "error", OTC_PyFailure::mfn_error, 1 },
  { "description", OTC_PyFailure::mfn_description, 1 },
  { "origin", OTC_PyFailure::mfn_origin, 1 },
  { "details", OTC_PyFailure::mfn_details, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyFailure::~OTC_PyFailure()
{
  failure_->destroy();
}

/* ------------------------------------------------------------------------- */
OTC_PyFailure::OTC_PyFailure(OTCEV_Failure* theFailure)
  : failure_(0)
{
  OTCLIB_ENSURE_FN((theFailure != 0),
   "OTC_PyFailure::OTC_PyFailure(OTCEV_Failure*)",
   "invalid failure supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  failure_ = (OTCEV_Failure*)theFailure->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFailure::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyFailure::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyFailure*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFailure::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyFailure*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFailure::mfn_conversationId(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Failure* theFailure;
  theFailure = ((OTC_PyFailure*)theInstance)->failure_;

  return Py_BuildValue("s",
   theFailure->conversationId().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFailure::mfn_error(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Failure* theFailure;
  theFailure = ((OTC_PyFailure*)theInstance)->failure_;

  return Py_BuildValue("i",theFailure->error());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFailure::mfn_description(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Failure* theFailure;
  theFailure = ((OTC_PyFailure*)theInstance)->failure_;

  return Py_BuildValue("s",theFailure->description().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFailure::mfn_origin(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Failure* theFailure;
  theFailure = ((OTC_PyFailure*)theInstance)->failure_;

  return Py_BuildValue("s",theFailure->origin().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFailure::mfn_details(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Failure* theFailure;
  theFailure = ((OTC_PyFailure*)theInstance)->failure_;

  return Py_BuildValue("s",theFailure->details().string());
}

/* ------------------------------------------------------------------------- */
