/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpypbntfn.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pypbntfn.hh>
#include <OTC/python/pysrvbnd.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyPublisherNotification::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "PublisherNotification",
  sizeof(OTC_PyPublisherNotification),
  0,
  OTC_PyPublisherNotification::sys_dealloc,
  0,
  OTC_PyPublisherNotification::sys_getattr,
  0,
  OTC_PyPublisherNotification::sys_compare,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

/* ------------------------------------------------------------------------- */
PyMethodDef OTC_PyPublisherNotification::gMethods_[] = {
  { "publisher", OTC_PyPublisherNotification::mfn_publisher, 1 },
  { "subscriber", OTC_PyPublisherNotification::mfn_subscriber, 1 },
  { "status", OTC_PyPublisherNotification::mfn_status, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyPublisherNotification::~OTC_PyPublisherNotification()
{
  notification_->destroy();
}

/* ------------------------------------------------------------------------- */
OTC_PyPublisherNotification::OTC_PyPublisherNotification(
 OTCEV_PublisherNotification* theNotification
)
  : notification_(0)
{
  OTCLIB_ENSURE_FN((theNotification != 0),
   "OTC_PyPublisherNotification::OTC_PyPublisherNotification(...)",
   "invalid publisher notification event supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  notification_ = (OTCEV_PublisherNotification*)theNotification->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyPublisherNotification::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
int OTC_PyPublisherNotification::obj_compare(
 OTC_PyPublisherNotification const* theObject
) const
{
  return notification_->publisher()->serviceAddress().rank(
   ((OTC_PyPublisherNotification*)theObject)->notification_->publisher()->serviceAddress());
}

/* ------------------------------------------------------------------------- */
void OTC_PyPublisherNotification::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyPublisherNotification*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyPublisherNotification::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyPublisherNotification*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
int OTC_PyPublisherNotification::sys_compare(
 PyObject* theInstance,
 PyObject* theObject
)
{
  return ((OTC_PyPublisherNotification*)theInstance)->obj_compare(
   (OTC_PyPublisherNotification*)theObject);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyPublisherNotification::mfn_publisher(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_PublisherNotification const* theNotification;
  theNotification = ((OTC_PyPublisherNotification*)theInstance)->notification_;

  OTC_PyServiceBinding* theObject;
  theObject = new OTC_PyServiceBinding(theNotification->publisher());
  OTCLIB_ASSERT_M(theObject != 0);

  return theObject;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyPublisherNotification::mfn_subscriber(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_PublisherNotification const* theNotification;
  theNotification = ((OTC_PyPublisherNotification*)theInstance)->notification_;

  OTC_PyServiceBinding* theObject;
  theObject = new OTC_PyServiceBinding(theNotification->subscriber());
  OTCLIB_ASSERT_M(theObject != 0);

  return theObject;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyPublisherNotification::mfn_status(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_PublisherNotification const* theNotification;
  theNotification = ((OTC_PyPublisherNotification*)theInstance)->notification_;

  return Py_BuildValue("i",theNotification->status());
}

/* ------------------------------------------------------------------------- */
