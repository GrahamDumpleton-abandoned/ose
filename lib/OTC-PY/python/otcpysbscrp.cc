/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpysbscrp.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pysbscrp.hh>
#include <OTC/python/pysrvbnd.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PySubscription::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Subscription",
  sizeof(OTC_PySubscription),
  0,
  OTC_PySubscription::sys_dealloc,
  0,
  OTC_PySubscription::sys_getattr,
  0,
  OTC_PySubscription::sys_compare,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

/* ------------------------------------------------------------------------- */
PyMethodDef OTC_PySubscription::gMethods_[] = {
  { "publisher", OTC_PySubscription::mfn_publisher, 1 },
  { "subscriber", OTC_PySubscription::mfn_subscriber, 1 },
  { "status", OTC_PySubscription::mfn_status, 1 },
  { "subject", OTC_PySubscription::mfn_subject, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PySubscription::~OTC_PySubscription()
{
  subscription_->destroy();
}

/* ------------------------------------------------------------------------- */
OTC_PySubscription::OTC_PySubscription(
 OTCEV_Subscription* theSubscription
)
  : subscription_(0)
{
  OTCLIB_ENSURE_FN((theSubscription != 0),
   "OTC_PySubscription::OTC_PySubscription(...)",
   "invalid subscription event supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  subscription_ = (OTCEV_Subscription*)theSubscription->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PySubscription::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
int OTC_PySubscription::obj_compare(
 OTC_PySubscription const* theObject
) const
{
  return subscription_->subject().rank(
   ((OTC_PySubscription*)theObject)->subscription_->subject());
}

/* ------------------------------------------------------------------------- */
void OTC_PySubscription::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PySubscription*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PySubscription::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PySubscription*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
int OTC_PySubscription::sys_compare(
 PyObject* theInstance,
 PyObject* theObject
)
{
  return ((OTC_PySubscription*)theInstance)->obj_compare(
   (OTC_PySubscription*)theObject);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PySubscription::mfn_publisher(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Subscription const* theSubscription;
  theSubscription = ((OTC_PySubscription*)theInstance)->subscription_;

  OTC_PyServiceBinding* theObject;
  theObject = new OTC_PyServiceBinding(theSubscription->publisher());
  OTCLIB_ASSERT_M(theObject != 0);

  return theObject;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PySubscription::mfn_subscriber(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Subscription const* theSubscription;
  theSubscription = ((OTC_PySubscription*)theInstance)->subscription_;

  OTC_PyServiceBinding* theObject;
  theObject = new OTC_PyServiceBinding(theSubscription->subscriber());
  OTCLIB_ASSERT_M(theObject != 0);

  return theObject;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PySubscription::mfn_status(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Subscription const* theSubscription;
  theSubscription = ((OTC_PySubscription*)theInstance)->subscription_;

  return Py_BuildValue("i",theSubscription->status());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PySubscription::mfn_subject(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Subscription const* theSubscription;
  theSubscription = ((OTC_PySubscription*)theInstance)->subscription_;

  return Py_BuildValue("s",theSubscription->subject().string());
}

/* ------------------------------------------------------------------------- */
