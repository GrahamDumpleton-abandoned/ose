/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyexchnc.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pyexchnc.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyExchangeAnnouncement::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "ExchangeAnnouncement",
  sizeof(OTC_PyExchangeAnnouncement),
  0,
  OTC_PyExchangeAnnouncement::sys_dealloc,
  0,
  OTC_PyExchangeAnnouncement::sys_getattr,
  0,
  OTC_PyExchangeAnnouncement::sys_compare,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

/* ------------------------------------------------------------------------- */
PyMethodDef OTC_PyExchangeAnnouncement::gMethods_[] = {
  { "group", OTC_PyExchangeAnnouncement::mfn_group, 1 },
  { "localAddress", OTC_PyExchangeAnnouncement::mfn_localAddress, 1 },
  { "remoteAddress", OTC_PyExchangeAnnouncement::mfn_remoteAddress, 1 },
  { "remoteProcess", OTC_PyExchangeAnnouncement::mfn_remoteProcess, 1 },
  { "state", OTC_PyExchangeAnnouncement::mfn_state, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyExchangeAnnouncement::~OTC_PyExchangeAnnouncement()
{
  announcement_->destroy();
}

/* ------------------------------------------------------------------------- */
OTC_PyExchangeAnnouncement::OTC_PyExchangeAnnouncement(
 OTCEV_ExchangeAnnouncement* theAnnouncement
)
  : announcement_(0)
{
  OTCLIB_ENSURE_FN((theAnnouncement != 0),
   "OTC_PyExchangeAnnouncement::OTC_PyExchangeAnnouncement(...)",
   "invalid exchange announcement supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  announcement_ = (OTCEV_ExchangeAnnouncement*)theAnnouncement->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyExchangeAnnouncement::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
int OTC_PyExchangeAnnouncement::obj_compare(
 OTC_PyExchangeAnnouncement const* theObject
) const
{
  return announcement_->remoteProcess().rank(
   ((OTC_PyExchangeAnnouncement*)theObject)->announcement_->remoteProcess());
}

/* ------------------------------------------------------------------------- */
void OTC_PyExchangeAnnouncement::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyExchangeAnnouncement*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyExchangeAnnouncement::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyExchangeAnnouncement*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
int OTC_PyExchangeAnnouncement::sys_compare(
 PyObject* theInstance,
 PyObject* theObject
)
{
  return ((OTC_PyExchangeAnnouncement*)theInstance)->obj_compare(
   (OTC_PyExchangeAnnouncement*)theObject);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyExchangeAnnouncement::mfn_group(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_ExchangeAnnouncement const* theAnnouncement;
  theAnnouncement = ((OTC_PyExchangeAnnouncement*)theInstance)->announcement_;

  return Py_BuildValue("s",theAnnouncement->group().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyExchangeAnnouncement::mfn_localAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_ExchangeAnnouncement const* theAnnouncement;
  theAnnouncement = ((OTC_PyExchangeAnnouncement*)theInstance)->announcement_;

  return Py_BuildValue("s",theAnnouncement->localAddress().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyExchangeAnnouncement::mfn_remoteAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_ExchangeAnnouncement const* theAnnouncement;
  theAnnouncement = ((OTC_PyExchangeAnnouncement*)theInstance)->announcement_;

  return Py_BuildValue("s",theAnnouncement->remoteAddress().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyExchangeAnnouncement::mfn_remoteProcess(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_ExchangeAnnouncement const* theAnnouncement;
  theAnnouncement = ((OTC_PyExchangeAnnouncement*)theInstance)->announcement_;

  return Py_BuildValue("s",theAnnouncement->remoteProcess().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyExchangeAnnouncement::mfn_state(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_ExchangeAnnouncement const* theAnnouncement;
  theAnnouncement = ((OTC_PyExchangeAnnouncement*)theInstance)->announcement_;

  return Py_BuildValue("i",theAnnouncement->state());
}

/* ------------------------------------------------------------------------- */
