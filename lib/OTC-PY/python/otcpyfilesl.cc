/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyfilesl.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef ANY
#undef ANY
#endif

#include <OTC/message/inetclnt.hh>
#include <OTC/message/inetlstn.hh>

#include <OTC/python/pyfilesl.hh>
#include <OTC/python/pydsptch.hh>

#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

/* ------------------------------------------------------------------------- */
#ifdef OTCLIB_TRACE
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_PyFileServlet");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()
#endif


/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyFileServlet::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "FileServlet",
  sizeof(OTC_PyFileServlet),
  0,
  OTC_PyFileServlet::sys_dealloc,
  0,
  OTC_PyFileServlet::sys_getattr,
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
PyMethodDef OTC_PyFileServlet::gMethods_[] = {
  { "instance", OTC_PyFileServlet::mfn_instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyFileServlet::~OTC_PyFileServlet()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_PyFileServlet::OTC_PyFileServlet(
 PyObject* theOuter,
 OTC_HttpSession* theSession,
 char const* theFileName,
 char const* theFileType
)
  : OTC_FileServlet(theSession,theFileName,theFileType),
    outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  Py_XINCREF(outer_);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFileServlet::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  PyObject* theObject = 0;
  char const* theFileName = 0;
  char const* theFileType = 0;

  if (!PyArg_ParseTuple(theArgs,"OOsz",&theOuter,
   &theObject,&theFileName,&theFileType))
    return 0;

  OTC_HttpSession* theSession;
  theSession = (OTC_HttpSession*)PyCObject_AsVoidPtr(theObject);

  OTC_PyFileServlet* theServlet;
  theServlet = new OTC_PyFileServlet(theOuter,
   theSession,theFileName,theFileType);
  OTCLIB_ASSERT_M(theServlet != 0);

  return (PyObject*)theServlet;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFileServlet::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyFileServlet::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyFileServlet*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFileServlet::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyFileServlet*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyFileServlet::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_FileServlet* theSelf;
  theSelf = (OTC_PyFileServlet*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
void OTC_PyFileServlet::destroy()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_PyFileServlet::destroy()");

  OTC_PyDispatcher::enterPython();

  PyObject* theOuter;
  theOuter = outer_;

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,"_destroyServlet","");

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();
  }
  else
    Py_XDECREF(theResult);

  Py_XDECREF(theOuter);

  OTC_PyDispatcher::leavePython();
}

/* ------------------------------------------------------------------------- */
