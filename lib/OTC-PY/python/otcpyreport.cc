/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyreport.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pyreport.hh>
#include <OTC/python/pysrvbnd.hh>
#include <OTC/python/pyservce.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyReport::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Report",
  sizeof(OTC_PyReport),
  0,
  OTC_PyReport::sys_dealloc,
  0,
  OTC_PyReport::sys_getattr,
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
PyMethodDef OTC_PyReport::gMethods_[] = {
  { "publisher", OTC_PyReport::mfn_publisher, 1 },
  { "subjectName", OTC_PyReport::mfn_subjectName, 1 },
  { "subjectPattern", OTC_PyReport::mfn_subjectPattern, 1 },
  { "content", OTC_PyReport::mfn_content, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyReport::~OTC_PyReport()
{
  report_->destroy();
}

/* ------------------------------------------------------------------------- */
OTC_PyReport::OTC_PyReport(OTCEV_Report* theReport)
  : report_(0)
{
  OTCLIB_ENSURE_FN((theReport != 0),
   "OTC_PyReport::OTC_PyReport(OTCEV_Report*)",
   "invalid report supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  report_ = (OTCEV_Report*)theReport->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyReport::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyReport::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyReport*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyReport::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyReport*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyReport::mfn_publisher(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding* thePublisher;
  thePublisher = ((OTC_PyReport*)theInstance)->report_->publisher();

  OTC_PyServiceBinding* theBinding;
  theBinding = new OTC_PyServiceBinding(thePublisher);
  OTCLIB_ASSERT_M(theBinding != 0);

  return theBinding;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyReport::mfn_subjectName(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Report* theReport;
  theReport = ((OTC_PyReport*)theInstance)->report_;

  return Py_BuildValue("s",
   theReport->subjectName().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyReport::mfn_subjectPattern(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Report* theReport;
  theReport = ((OTC_PyReport*)theInstance)->report_;

  return Py_BuildValue("s",
   theReport->subjectPattern().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyReport::mfn_content(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theDecoder = 0;

  if (!PyArg_ParseTuple(theArgs,"O",&theDecoder))
    return 0;

  OTCEV_Report* theReport;
  theReport = ((OTC_PyReport*)theInstance)->report_;

  return OTC_PyService::payloadToObject(theReport->content(),theDecoder);
}

/* ------------------------------------------------------------------------- */
