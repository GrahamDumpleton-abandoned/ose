/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyhttpsn.cc
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

#include <OTC/python/pyhttpsn.hh>
#include <OTC/python/pydsptch.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyHttpSession::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "HttpSession",
  sizeof(OTC_PyHttpSession),
  0,
  OTC_PyHttpSession::sys_dealloc,
  0,
  OTC_PyHttpSession::sys_getattr,
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
PyMethodDef OTC_PyHttpSession::gMethods_[] = {
  { "instance", OTC_PyHttpSession::mfn_instance, 1 },
  { "contentLength", OTC_PyHttpSession::mfn_contentLength, 1 },
  { "contentType", OTC_PyHttpSession::mfn_contentType, 1 },
  { "requestMethod", OTC_PyHttpSession::mfn_requestMethod, 1 },
  { "url", OTC_PyHttpSession::mfn_url, 1 },
  { "protocolVersion", OTC_PyHttpSession::mfn_protocolVersion, 1 },
  { "serverHost", OTC_PyHttpSession::mfn_serverHost, 1 },
  { "serverPort", OTC_PyHttpSession::mfn_serverPort, 1 },
  { "requestPath", OTC_PyHttpSession::mfn_requestPath, 1 },
  { "queryString", OTC_PyHttpSession::mfn_queryString, 1 },
  { "clientHost", OTC_PyHttpSession::mfn_clientHost, 1 },
  { "clientPort", OTC_PyHttpSession::mfn_clientPort, 1 },
  { "remoteUser", OTC_PyHttpSession::mfn_remoteUser, 1 },
  { "headers", OTC_PyHttpSession::mfn_headers, 1 },
  { "containsHeader", OTC_PyHttpSession::mfn_containsHeader, 1 },
  { "header", OTC_PyHttpSession::mfn_header, 1 },
  { "serverRoot", OTC_PyHttpSession::mfn_serverRoot, 1 },
  { "serverPath", OTC_PyHttpSession::mfn_serverPath, 1 },
  { "servletPath", OTC_PyHttpSession::mfn_servletPath, 1 },
  { "servletInfo", OTC_PyHttpSession::mfn_servletInfo, 1 },
  { "bindServlet", OTC_PyHttpSession::mfn_bindServlet, 1 },
  { "sessionId", OTC_PyHttpSession::mfn_sessionId, 1 },
  { "sessionContext", OTC_PyHttpSession::mfn_sessionContext, 1 },
  { "bindSession", OTC_PyHttpSession::mfn_bindSession, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyHttpSession::~OTC_PyHttpSession()
{
  session_->unReference();
}

/* ------------------------------------------------------------------------- */
OTC_PyHttpSession::OTC_PyHttpSession(OTC_HttpSession* theSession)
  : session_(theSession)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  session_->reference();
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyHttpSession::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyHttpSession*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyHttpSession*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_contentLength(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("i",theSelf->contentLength());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_contentType(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->contentType().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_requestMethod(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->requestMethod().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_url(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->url().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_protocolVersion(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->protocolVersion().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_serverHost(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->serverHost().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_serverPort(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->serverPort().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_requestPath(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->requestPath().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_queryString(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->queryString().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_clientHost(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->clientHost().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_clientPort(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->clientPort().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_remoteUser(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->remoteUser().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_headers(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

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
PyObject* OTC_PyHttpSession::mfn_containsHeader(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theHeader;

  if (!PyArg_ParseTuple(theArgs,"s",&theHeader))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  if (theSelf->containsHeader(theHeader))
    return Py_BuildValue("i",1);

  return Py_BuildValue("i",0);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_header(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theHeader;

  if (!PyArg_ParseTuple(theArgs,"s",&theHeader))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

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
PyObject* OTC_PyHttpSession::mfn_serverRoot(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->serverRoot().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_serverPath(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->serverPath().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_servletPath(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->servletPath().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_servletInfo(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->servletInfo().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_bindServlet(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theServletPath;
  char const* theServletInfo;

  if (!PyArg_ParseTuple(theArgs,"ss",&theServletPath,&theServletInfo))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  theSelf->bindServlet(theServletPath,theServletInfo);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_sessionId(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->sessionId().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_sessionContext(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->sessionContext().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyHttpSession::mfn_bindSession(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theSessionId;
  char const* theSessionContext = 0;

  if (!PyArg_ParseTuple(theArgs,"s|z",&theSessionId,&theSessionContext))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OTC_PyHttpSession*)theInstance)->session_;

  theSelf->bindSession(theSessionId,theSessionContext);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
