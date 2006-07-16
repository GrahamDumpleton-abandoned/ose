/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyhttpsl.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef ANY
#undef ANY
#endif

#include <OTC/message/inetclnt.hh>
#include <OTC/message/inetlstn.hh>
#include <OTC/message/endpoint.hh>
#include <OTC/message/congest.hh>

#include <OTC/python/pyhttpsl.hh>
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
    TAG = new OTC_TraceTag("OTC_PyHttpServlet");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()
#endif


/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyHttpServlet::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "HttpServlet",
  sizeof(OTC_PyHttpServlet),
  0,
  OTC_PyHttpServlet::sys_dealloc,
  0,
  OTC_PyHttpServlet::sys_getattr,
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
PyMethodDef OTC_PyHttpServlet::gMethods_[] = {
  { "instance", OTC_PyHttpServlet::mfn_instance, 1 },
  { "contentLength", OTC_PyHttpServlet::mfn_contentLength, 1 },
  { "contentType", OTC_PyHttpServlet::mfn_contentType, 1 },
  { "requestMethod", OTC_PyHttpServlet::mfn_requestMethod, 1 },
  { "url", OTC_PyHttpServlet::mfn_url, 1 },
  { "protocolVersion", OTC_PyHttpServlet::mfn_protocolVersion, 1 },
  { "serverHost", OTC_PyHttpServlet::mfn_serverHost, 1 },
  { "serverPort", OTC_PyHttpServlet::mfn_serverPort, 1 },
  { "requestPath", OTC_PyHttpServlet::mfn_requestPath, 1 },
  { "queryString", OTC_PyHttpServlet::mfn_queryString, 1 },
  { "clientHost", OTC_PyHttpServlet::mfn_clientHost, 1 },
  { "clientPort", OTC_PyHttpServlet::mfn_clientPort, 1 },
  { "remoteUser", OTC_PyHttpServlet::mfn_remoteUser, 1 },
  { "headers", OTC_PyHttpServlet::mfn_headers, 1 },
  { "containsHeader", OTC_PyHttpServlet::mfn_containsHeader, 1 },
  { "header", OTC_PyHttpServlet::mfn_header, 1 },
  { "serverRoot", OTC_PyHttpServlet::mfn_serverRoot, 1 },
  { "serverPath", OTC_PyHttpServlet::mfn_serverPath, 1 },
  { "servletPath", OTC_PyHttpServlet::mfn_servletPath, 1 },
  { "servletInfo", OTC_PyHttpServlet::mfn_servletInfo, 1 },
  { "sessionId", OTC_PyHttpServlet::mfn_sessionId, 1 },
  { "sessionContext", OTC_PyHttpServlet::mfn_sessionContext, 1 },
  { "sendResponse", OTC_PyHttpServlet::mfn_sendResponse, 1 },
  { "sendHeader", OTC_PyHttpServlet::mfn_sendHeader, 1 },
  { "endHeaders", OTC_PyHttpServlet::mfn_endHeaders, 1 },
  { "sendContent", OTC_PyHttpServlet::mfn_sendContent, 1 },
  { "endContent", OTC_PyHttpServlet::mfn_endContent, 1 },
  { "sendError", OTC_PyHttpServlet::mfn_sendError, 1 },
  { "sendComplete", OTC_PyHttpServlet::mfn_sendComplete, 1 },
  { "shutdown", OTC_PyHttpServlet::mfn_shutdown, 1 },
  { "statusText", OTC_PyHttpServlet::mfn_statusText, 1 },
  { "subscribeCongestion", OTC_PyHttpServlet::mfn_subscribeCongestion, 1 },
  { "unsubscribeCongestion", OTC_PyHttpServlet::mfn_unsubscribeCongestion, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyHttpServlet::~OTC_PyHttpServlet()
{
  outer_ = 0;

  endPoint()->removeObserver(id());
}

/* ------------------------------------------------------------------------- */
OTC_PyHttpServlet::OTC_PyHttpServlet(
 PyObject* theOuter,
 OTC_HttpSession* theSession
)
  : OTC_HttpServlet(theSession), outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  Py_XINCREF(outer_);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  PyObject* theObject = 0;

  if (!PyArg_ParseTuple(theArgs,"OO",&theOuter,&theObject))
    return 0;

  OTC_HttpSession* theSession;
  theSession = (OTC_HttpSession*)PyCObject_AsVoidPtr(theObject);

  OTC_PyHttpServlet* theServlet;
  theServlet = new OTC_PyHttpServlet(theOuter,theSession);
  OTCLIB_ASSERT_M(theServlet != 0);

  return (PyObject*)theServlet;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyHttpServlet::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyHttpServlet*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyHttpServlet*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_contentLength(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("i",theSelf->contentLength());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_contentType(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->contentType().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_requestMethod(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->requestMethod().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_url(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->url().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_protocolVersion(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->protocolVersion().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_serverHost(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->serverHost().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_serverPort(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->serverPort().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_requestPath(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->requestPath().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_queryString(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->queryString().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_clientHost(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->clientHost().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_clientPort(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->clientPort().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_remoteUser(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->remoteUser().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_headers(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  PyObject* theDictionary;
  theDictionary = PyDict_New();

  if (theDictionary == 0)
    return 0;

  OTC_PairIterator<OTC_String,OTC_String> theItems;
  theItems = theSelf->headers();
  while (theItems.isValid())
  {
    PyObject* theKey = 0;
    PyObject* theValue = 0;

    theKey = PyString_FromString(theItems.key().string());
    theValue = PyString_FromString(theItems.item().string());

    if (theKey == 0 || theValue == 0)
    {
      Py_XDECREF(theKey);
      Py_XDECREF(theValue);
      Py_XDECREF(theDictionary);

      return 0;
    }

    if (PyDict_SetItem(theDictionary,theKey,theValue) == -1)
    {
      Py_XDECREF(theKey);
      Py_XDECREF(theValue);
      Py_XDECREF(theDictionary);

      return 0;
    }

    Py_XDECREF(theKey);
    Py_XDECREF(theValue);

    theItems.next();
  }

  return theDictionary;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_containsHeader(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theHeader;

  if (!PyArg_ParseTuple(theArgs,"s",&theHeader))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  if (theSelf->containsHeader(theHeader))
    return Py_BuildValue("i",1);

  return Py_BuildValue("i",0);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_header(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theHeader;

  if (!PyArg_ParseTuple(theArgs,"s",&theHeader))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  if (!theSelf->containsHeader(theHeader))
  {
    // PyErr_SetString(PyExc_KeyError,
    //  "no such header present in request");
    PyErr_SetString(PyExc_KeyError,theHeader);

    return 0;
  }

  return Py_BuildValue("s",theSelf->header(theHeader).string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_serverRoot(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->serverRoot().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_serverPath(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->serverPath().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_servletPath(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->servletPath().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_servletInfo(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->servletInfo().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_sessionId(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->sessionId().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_sessionContext(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->sessionContext().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_sendResponse(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theStatus;
  char const* theMessage=0;

  if (!PyArg_ParseTuple(theArgs,"i|z",&theStatus,&theMessage))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->sendResponse(u_int(theStatus),theMessage);

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_sendHeader(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theKeyWord=0;
  char const* theValue=0;

  if (!PyArg_ParseTuple(theArgs,"ss",&theKeyWord,&theValue))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->sendHeader(theKeyWord,theValue);

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_endHeaders(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->endHeaders();

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_sendContent(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theContent=0;
  int theLength;

  if (!PyArg_ParseTuple(theArgs,"s#",&theContent,&theLength))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->sendContent(OTC_String(theContent,theLength));

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_endContent(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyHttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->endContent();

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_sendError(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theStatus;
  char const* theContent=0;

  if (!PyArg_ParseTuple(theArgs,"i|z",&theStatus,&theContent))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->sendError(u_int(theStatus),theContent);

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_sendComplete(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theResponse=0;
  int theLength;

  if (!PyArg_ParseTuple(theArgs,"s#",&theResponse,&theLength))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->sendComplete(OTC_String(theResponse,theLength));

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_shutdown(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theDelay = -1;

  if (!PyArg_ParseTuple(theArgs,"|i",&theDelay))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->shutdown(theDelay);

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_statusText(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theStatus;

  if (!PyArg_ParseTuple(theArgs,"i",&theStatus))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->statusText(theStatus));
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_subscribeCongestion(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyHttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  theSelf->endPoint()->addObserver(theSelf->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpServlet::mfn_unsubscribeCongestion(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyHttpServlet* theSelf;
  theSelf = (OTC_PyHttpServlet*)theInstance;

  theSelf->endPoint()->removeObserver(theSelf->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
void OTC_PyHttpServlet::processRequest()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_PyHttpServlet::processRequest()");

  OTC_PyDispatcher::enterPython();

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,"_processRequest","");

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();
  }
  else
    Py_XDECREF(theResult);

  OTC_PyDispatcher::leavePython();
}

/* ------------------------------------------------------------------------- */
void OTC_PyHttpServlet::processContent(OTC_String const& theContent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_PyHttpServlet::processContent()");

  OTC_PyDispatcher::enterPython();

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,
   "_processContent","s#",theContent.string(),theContent.length());

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();
  }
  else
    Py_XDECREF(theResult);

  OTC_PyDispatcher::leavePython();
}

/* ------------------------------------------------------------------------- */
void OTC_PyHttpServlet::destroy()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_PyHttpServlet::destroy()");

  endPoint()->removeObserver(id());

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
void OTC_PyHttpServlet::handle(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_PyHttpServlet::handle()");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << *theEvent << endl;

  if (theEvent->type() == OTCEV_Congestion::typeId())
  {
    OTCEV_Congestion* theCongestion;
    theCongestion = (OTCEV_Congestion*)theEvent;

    OTC_PyDispatcher::enterPython();

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleCongestion","ii",theCongestion->status(),
     theCongestion->pending());

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    OTC_PyDispatcher::leavePython();
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
