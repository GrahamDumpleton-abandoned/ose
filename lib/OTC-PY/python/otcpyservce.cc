/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyservce.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pyservce.hh>
#include <OTC/python/pysrvbnd.hh>
#include <OTC/python/pyreport.hh>
#include <OTC/python/pyrquest.hh>
#include <OTC/python/pyrspnse.hh>
#include <OTC/python/pysbscrp.hh>
#include <OTC/python/pypbntfn.hh>
#include <OTC/python/pyfailre.hh>
#include <OTC/python/pyopaque.hh>
#include <OTC/python/pydsptch.hh>
#include <OTC/dispatch/servannc.hh>
#include <OTC/dispatch/grpannc.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyService::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Service",
  sizeof(OTC_PyService),
  0,
  OTC_PyService::sys_dealloc,
  0,
  OTC_PyService::sys_getattr,
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
PyMethodDef OTC_PyService::gMethods_[] = {
  { "acquireObject",
    OTC_PyAgent::mfn_acquireObject, 1 },
  { "releaseObject",
    OTC_PyAgent::mfn_releaseObject, 1 },
  { "activateThreading",
    OTC_PyAgent::mfn_activateThreading, 1 },
  { "suspendThreading",
    OTC_PyAgent::mfn_suspendThreading, 1 },
  { "threadingEnabled",
    OTC_PyAgent::mfn_threadingEnabled, 1 },
  { "id",
    OTC_PyAgent::mfn_id, 1 },
  { "startTimer",
    OTC_PyAgent::mfn_startTimer, 1 },
  { "cancelTimer",
    OTC_PyAgent::mfn_cancelTimer, 1 },
  { "cancelAllTimers",
    OTC_PyAgent::mfn_cancelAllTimers, 1 },
  { "setAlarm",
    OTC_PyAgent::mfn_setAlarm, 1 },
  { "cancelAlarm",
    OTC_PyAgent::mfn_cancelAlarm, 1 },
  { "cancelAllAlarms",
    OTC_PyAgent::mfn_cancelAllAlarms, 1 },
  { "scheduleAction",
    OTC_PyAgent::mfn_scheduleAction, 1 },
  { "scheduleCronAction",
    OTC_PyAgent::mfn_scheduleCronAction, 1 },
  { "cancelAction",
    OTC_PyAgent::mfn_cancelAction, 1 },
  { "cancelAllActions",
    OTC_PyAgent::mfn_cancelAllActions, 1 },
  { "subscribeSignal",
    OTC_PyAgent::mfn_subscribeSignal, 1 },
  { "unsubscribeSignal",
    OTC_PyAgent::mfn_unsubscribeSignal, 1 },
  { "unsubscribeAllSignals",
    OTC_PyAgent::mfn_unsubscribeAllSignals, 1 },
  { "scheduleShutdown",
    OTC_PyAgent::mfn_scheduleShutdown, 1 },
  { "suspendShutdown",
    OTC_PyAgent::mfn_suspendShutdown, 1 },
  { "resumeShutdown",
    OTC_PyAgent::mfn_resumeShutdown, 1 },
  { "subscribeShutdown",
    OTC_PyAgent::mfn_subscribeShutdown, 1 },
  { "unsubscribeShutdown",
    OTC_PyAgent::mfn_unsubscribeShutdown, 1 },
  { "subscribeSocket",
    OTC_PyAgent::mfn_subscribeSocket, 1 },
  { "unsubscribeSocket",
    OTC_PyAgent::mfn_unsubscribeSocket, 1 },
  { "unsubscribeAllSockets",
    OTC_PyAgent::mfn_unsubscribeAllSockets, 1 },
  { "serviceAgents",
    OTC_PyMonitor::mfn_serviceAgents, 1 },
  { "serviceGroups",
    OTC_PyMonitor::mfn_serviceGroups, 1 },
  { "lookupServiceName",
    OTC_PyMonitor::mfn_lookupServiceName, 1 },
  { "lookupServiceAddress",
    OTC_PyMonitor::mfn_lookupServiceAddress, 1 },
  { "lookupServiceGroup",
    OTC_PyMonitor::mfn_lookupServiceGroup, 1 },
  { "unsubscribeAllRegistry",
    OTC_PyMonitor::mfn_unsubscribeAllRegistry, 1 },
  { "subscribeRegistry",
    OTC_PyMonitor::mfn_subscribeRegistry, 1 },
  { "unsubscribeRegistry",
    OTC_PyMonitor::mfn_unsubscribeRegistry, 1 },
  { "subscribeServiceName",
    OTC_PyMonitor::mfn_subscribeServiceName, 1 },
  { "unsubscribeServiceName",
    OTC_PyMonitor::mfn_unsubscribeServiceName, 1 },
  { "subscribeServiceAddress",
    OTC_PyMonitor::mfn_subscribeServiceAddress, 1 },
  { "unsubscribeServiceAddress",
    OTC_PyMonitor::mfn_unsubscribeServiceAddress, 1 },
  { "subscribeServiceGroup",
    OTC_PyMonitor::mfn_subscribeServiceGroup, 1 },
  { "unsubscribeServiceGroup",
    OTC_PyMonitor::mfn_unsubscribeServiceGroup, 1 },
  { "serviceBinding",
    OTC_PyService::mfn_serviceBinding, 1 },
  { "joinGroup",
    OTC_PyService::mfn_joinGroup, 1 },
  { "leaveGroup",
    OTC_PyService::mfn_leaveGroup, 1 },
  { "sendRequest",
    OTC_PyService::mfn_sendRequest, 1 },
  { "sendMessage",
    OTC_PyService::mfn_sendMessage, 1 },
  { "subscribeReports",
    OTC_PyService::mfn_subscribeReports, 1 },
  { "unsubscribeReports",
    OTC_PyService::mfn_unsubscribeReports, 1 },
  { "publishReport",
    OTC_PyService::mfn_publishReport, 1 },
  { "sendReport",
    OTC_PyService::mfn_sendReport, 1 },
  { "expireReports",
    OTC_PyService::mfn_expireReports, 1 },
  { "expireAllReports",
    OTC_PyService::mfn_expireAllReports, 1 },
  { "purgeReports",
    OTC_PyService::mfn_purgeReports, 1 },
  { "matchSubject",
    OTC_PyService::mfn_matchSubject, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyService::~OTC_PyService()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_PyService::OTC_PyService(
 PyObject* theOuter,
 char const* theServiceName,
 char const* theServiceAudience
)
  : OTC_PyMonitor(theOuter),
    broker_(this,theServiceName,theServiceAudience)
{
  ob_type = &gType_;

  // _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  char* theServiceName = 0;
  char* theServiceAudience = 0;

  if (!PyArg_ParseTuple(theArgs,"Oss",&theOuter,
   &theServiceName,&theServiceAudience))
  {
    return 0;
  }

  OTC_PyService* theAgent;
  theAgent = new OTC_PyService(theOuter,theServiceName,theServiceAudience);
  OTCLIB_ASSERT_M(theAgent != 0);

  return (PyObject*)theAgent;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyService::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyService*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyService*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::mfn_serviceBinding(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyService* theSelf;
  theSelf = (OTC_PyService*)theInstance;

  OTC_PyServiceBinding* theBinding;
  theBinding = new OTC_PyServiceBinding(
   theSelf->broker_.serviceBinding());
  OTCLIB_ASSERT_M(theBinding != 0);

  return theBinding;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::mfn_joinGroup(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceGroup = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceGroup))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OTC_PyService*)theInstance)->broker_;

  theBroker->joinGroup(theServiceGroup);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::mfn_leaveGroup(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceGroup = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceGroup))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OTC_PyService*)theInstance)->broker_;

  theBroker->leaveGroup(theServiceGroup);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::mfn_sendRequest(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theBindingObject = 0;
  char* theMethodName = 0;
  PyObject* thePayloadObject = 0;
  PyObject* theEncoder = 0;

  if (!PyArg_ParseTuple(theArgs,"OsOO",
   &theBindingObject,&theMethodName,&thePayloadObject,&theEncoder))
  {
    return 0;
  }

  if (!OTC_PyServiceBinding::isType(theBindingObject))
  {
    PyErr_SetString(PyExc_TypeError,
     "agent description not a service binding object");

    return 0;
  }

  OTC_SVBroker* theBroker;
  theBroker = &((OTC_PyService*)theInstance)->broker_;

  OTC_ServiceBinding* theBinding;
  theBinding = ((OTC_PyServiceBinding*)theBindingObject)->binding();

  OTC_SVPayload thePayload;

  if (!objectToPayload(thePayloadObject,thePayload,theEncoder))
    return 0;

  OTC_String theConversationId;

  theConversationId = theBroker->sendRequest(theBinding,
   theMethodName,thePayload);

  return Py_BuildValue("s",theConversationId.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::mfn_sendMessage(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theBindingObject = 0;
  char* theMessageId = 0;
  char* theDescription = 0;
  char* theContent = 0;
  int theContentLength = 0;
  char* theContentType = 0;
  int theSequenceNumber = 0;

  if (!PyArg_ParseTuple(theArgs,"Osss#si",
   &theBindingObject,&theMessageId,&theDescription,&theContent,
   &theContentLength,&theContentType,&theSequenceNumber))
  {
    return 0;
  }

  if (!OTC_PyServiceBinding::isType(theBindingObject))
  {
    PyErr_SetString(PyExc_TypeError,
     "agent description not a service binding object");

    return 0;
  }

  OTC_SVBroker* theBroker;
  theBroker = &((OTC_PyService*)theInstance)->broker_;

  OTC_ServiceBinding* theBinding;
  theBinding = ((OTC_PyServiceBinding*)theBindingObject)->binding();

  OTC_String theConversationId;

  theConversationId = theBroker->sendMessage(theBinding,theMessageId,
   theDescription,theContent,theContentLength,theContentType,
   theSequenceNumber);

  return Py_BuildValue("s",theConversationId.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::mfn_subscribeReports(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theBindingObject = 0;

  char* theServiceName = 0;
  char* theSubjectPattern = 0;

#if 0
  if (!PyArg_ParseTuple(theArgs,"ss",&theServiceName,&theSubjectPattern))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OTC_PyService*)theInstance)->broker_;

  theBroker->subscribeReports(theServiceName,theSubjectPattern);
#endif

  if (PyArg_ParseTuple(theArgs,"ss",&theServiceName,&theSubjectPattern))
  {
    OTC_SVBroker* theBroker;
    theBroker = &((OTC_PyService*)theInstance)->broker_;

    theBroker->subscribeReports(theServiceName,theSubjectPattern);
  }
  else if (PyArg_ParseTuple(theArgs,"Os",&theBindingObject,
   &theSubjectPattern) && OTC_PyServiceBinding::isType(theBindingObject))
  {
    PyErr_Clear();

    OTC_SVBroker* theBroker;
    theBroker = &((OTC_PyService*)theInstance)->broker_;

    OTC_ServiceBinding* theBinding;
    theBinding = ((OTC_PyServiceBinding*)theBindingObject)->binding();

    theBroker->subscribeReports(theBinding,theSubjectPattern);
  }
  else
    return 0;

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::mfn_unsubscribeReports(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theBindingObject = 0;

  char* theServiceName = 0;
  char* theSubjectPattern = 0;

#if 0
  if (!PyArg_ParseTuple(theArgs,"ss",&theServiceName,&theSubjectPattern))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OTC_PyService*)theInstance)->broker_;

  theBroker->unsubscribeReports(theServiceName,theSubjectPattern);
#endif

  if (PyArg_ParseTuple(theArgs,"ss",&theServiceName,&theSubjectPattern))
  {
    OTC_SVBroker* theBroker;
    theBroker = &((OTC_PyService*)theInstance)->broker_;

    theBroker->unsubscribeReports(theServiceName,theSubjectPattern);
  }
  else if (PyArg_ParseTuple(theArgs,"Os",&theBindingObject,
   &theSubjectPattern) && OTC_PyServiceBinding::isType(theBindingObject))
  {
    PyErr_Clear();

    OTC_SVBroker* theBroker;
    theBroker = &((OTC_PyService*)theInstance)->broker_;

    OTC_ServiceBinding* theBinding;
    theBinding = ((OTC_PyServiceBinding*)theBindingObject)->binding();

    theBroker->unsubscribeReports(theBinding,theSubjectPattern);
  }
  else
    return 0;


  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::mfn_publishReport(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theSubjectName = 0;
  PyObject* thePayloadObject = 0;
  int theLifeTime = 0;
  PyObject* theEncoder = 0;

  if (!PyArg_ParseTuple(theArgs,"sOiO",
   &theSubjectName,&thePayloadObject,&theLifeTime,&theEncoder))
  {
    return 0;
  }

  OTC_SVBroker* theBroker;
  theBroker = &((OTC_PyService*)theInstance)->broker_;

  OTC_SVPayload thePayload;

  if (!objectToPayload(thePayloadObject,thePayload,theEncoder))
    return 0;

  theBroker->publishReport(theSubjectName,
   thePayload,theLifeTime);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::mfn_sendReport(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theBindingObject = 0;
  char* theSubjectName = 0;
  PyObject* thePayloadObject = 0;
  PyObject* theEncoder = 0;

  if (!PyArg_ParseTuple(theArgs,"OsOO",
   &theBindingObject,&theSubjectName,&thePayloadObject,&theEncoder))
  {
    return 0;
  }

  if (!OTC_PyServiceBinding::isType(theBindingObject))
  {
    PyErr_SetString(PyExc_TypeError,
     "agent description not a service binding object");

    return 0;
  }

  OTC_SVBroker* theBroker;
  theBroker = &((OTC_PyService*)theInstance)->broker_;

  OTC_ServiceBinding* theBinding;
  theBinding = ((OTC_PyServiceBinding*)theBindingObject)->binding();

  OTC_SVPayload thePayload;

  if (!objectToPayload(thePayloadObject,thePayload,theEncoder))
    return 0;

  OTC_String theConversationId;

  theBroker->sendReport(theBinding,theSubjectName,thePayload);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::mfn_expireReports(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theSubjectPattern = 0;
  int theAge = 0;

  if (!PyArg_ParseTuple(theArgs,"si",&theSubjectPattern,&theAge))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OTC_PyService*)theInstance)->broker_;

  theBroker->expireReports(theSubjectPattern,theAge);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::mfn_expireAllReports(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theAge = 0;

  if (!PyArg_ParseTuple(theArgs,"i",&theAge))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OTC_PyService*)theInstance)->broker_;

  theBroker->expireReports(theAge);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::mfn_purgeReports(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OTC_PyService*)theInstance)->broker_;

  theBroker->purgeReports();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::mfn_matchSubject(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theSubjectPattern = 0;
  char* theSubjectName = 0;

  if (!PyArg_ParseTuple(theArgs,"ss",&theSubjectPattern,&theSubjectName))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OTC_PyService*)theInstance)->broker_;

  OTC_Boolean theResult;
  theResult = theBroker->matchSubject(theSubjectPattern,theSubjectName);

  return Py_BuildValue("i",int(theResult));
}

/* ------------------------------------------------------------------------- */
void OTC_PyService::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_PublisherNotification::typeId())
  {
    OTCEV_PublisherNotification* theNotification;
    theNotification = (OTCEV_PublisherNotification*)theEvent;

    enterPython();

    OTC_PyPublisherNotification* theObject;
    theObject = new OTC_PyPublisherNotification(theNotification);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handlePublisherNotification","O",theObject);

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    Py_XDECREF(theObject);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Subscription::typeId())
  {
    OTCEV_Subscription* theSubscription;
    theSubscription = (OTCEV_Subscription*)theEvent;

    enterPython();

    OTC_PySubscription* theObject;
    theObject = new OTC_PySubscription(theSubscription);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleSubscription","O",theObject);

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    Py_XDECREF(theObject);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Report::typeId())
  {
    OTCEV_Report* theReport;
    theReport = (OTCEV_Report*)theEvent;

    enterPython();

    OTC_PyReport* theObject;
    theObject = new OTC_PyReport(theReport);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleReport","O",theObject);

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    Py_XDECREF(theObject);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Opaque::typeId())
  {
    OTCEV_Opaque* theMessage;
    theMessage = (OTCEV_Opaque*)theEvent;

    enterPython();

    OTC_PyOpaque* theObject;
    theObject = new OTC_PyOpaque(theMessage);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleMessage","O",theObject);

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    Py_XDECREF(theObject);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Request::typeId())
  {
    OTCEV_Request* theRequest;
    theRequest = (OTCEV_Request*)theEvent;

    enterPython();

    OTC_PyRequest* theObject;
    theObject = new OTC_PyRequest(theRequest);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleRequest","O",theObject);

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    Py_XDECREF(theObject);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Response::typeId())
  {
    OTCEV_Response* theResponse;
    theResponse = (OTCEV_Response*)theEvent;

    enterPython();

    OTC_PyResponse* theObject;
    theObject = new OTC_PyResponse(theResponse);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleResponse","O",theObject);

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    Py_XDECREF(theObject);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Failure::typeId())
  {
    OTCEV_Failure* theFailure;
    theFailure = (OTCEV_Failure*)theEvent;

    enterPython();

    OTC_PyFailure* theObject;
    theObject = new OTC_PyFailure(theFailure);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleFailure","O",theObject);

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    Py_XDECREF(theObject);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* theAnnouncement;
    theAnnouncement = (OTCEV_ServiceAnnouncement*)theEvent;

    enterPython();

    if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_REGISTRY)
    {
      OTC_PyServiceBinding* theObject;
      theObject = new OTC_PyServiceBinding(
       theAnnouncement->serviceBinding());
      OTCLIB_ASSERT_M(theObject != 0);

#if 0
      PyObject* theGroup = Py_None;
      Py_XINCREF(theGroup);
#endif

      PyObject* theResult;
      theResult = PyObject_CallMethod(outer_,
       // "_handleRegistry","OOi",theObject,theGroup,
       "_handleRegistry","OOi",theObject,Py_None,
       theAnnouncement->status());

      if (theResult == 0)
      {
	PyErr_Print();
	PyErr_Clear();
      }
      else
	Py_XDECREF(theResult);

      Py_XDECREF(theObject);
    }
    else if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_NAME)
    {
      OTC_PyServiceBinding* theObject;
      theObject = new OTC_PyServiceBinding(
       theAnnouncement->serviceBinding());
      OTCLIB_ASSERT_M(theObject != 0);

      PyObject* theResult;
      theResult = PyObject_CallMethod(outer_,
       "_handleServiceName","Oi",theObject,
       theAnnouncement->status());

      if (theResult == 0)
      {
	PyErr_Print();
	PyErr_Clear();
      }
      else
	Py_XDECREF(theResult);

      Py_XDECREF(theObject);
    }
    else if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_ADDRESS)
    {
      OTC_PyServiceBinding* theObject;
      theObject = new OTC_PyServiceBinding(
       theAnnouncement->serviceBinding());
      OTCLIB_ASSERT_M(theObject != 0);

#if 0
      Py_XINCREF(Py_None);
#endif

      PyObject* theResult;
      theResult = PyObject_CallMethod(outer_,
       "_handleServiceAddress","OOis",theObject,Py_None,
       theAnnouncement->status(),
       theAnnouncement->subscriptionName().string());

      if (theResult == 0)
      {
	PyErr_Print();
	PyErr_Clear();
      }
      else
	Py_XDECREF(theResult);

      Py_XDECREF(theObject);
    }

    leavePython();
  }
  else if (theEvent->type() == OTCEV_GroupAnnouncement::typeId())
  {
    OTCEV_GroupAnnouncement* theAnnouncement;
    theAnnouncement = (OTCEV_GroupAnnouncement*)theEvent;

    enterPython();

    if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_REGISTRY)
    {
      OTC_PyServiceBinding* theObject;
      theObject = new OTC_PyServiceBinding(
       theAnnouncement->serviceBinding());
      OTCLIB_ASSERT_M(theObject != 0);

      PyObject* theResult;
      theResult = PyObject_CallMethod(outer_,
       "_handleRegistry","Osi",theObject,
       theAnnouncement->serviceGroup().string(),
       theAnnouncement->status());

      if (theResult == 0)
      {
	PyErr_Print();
	PyErr_Clear();
      }
      else
	Py_XDECREF(theResult);

      Py_XDECREF(theObject);
    }
    else if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_ADDRESS)
    {
      OTC_PyServiceBinding* theObject;
      theObject = new OTC_PyServiceBinding(
       theAnnouncement->serviceBinding());
      OTCLIB_ASSERT_M(theObject != 0);

      PyObject* theResult;
      theResult = PyObject_CallMethod(outer_,
       "_handleServiceAddress","Osis",theObject,
       theAnnouncement->serviceGroup().string(),
       theAnnouncement->status(),
       theAnnouncement->subscriptionName().string());

      if (theResult == 0)
      {
	PyErr_Print();
	PyErr_Clear();
      }
      else
	Py_XDECREF(theResult);

      Py_XDECREF(theObject);
    }
    else if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_GROUP)
    {
      OTC_PyServiceBinding* theObject;
      theObject = new OTC_PyServiceBinding(
       theAnnouncement->serviceBinding());
      OTCLIB_ASSERT_M(theObject != 0);

      PyObject* theResult;
      theResult = PyObject_CallMethod(outer_,
       "_handleServiceGroup","Osi",theObject,
       theAnnouncement->serviceGroup().string(),
       theAnnouncement->status());

      if (theResult == 0)
      {
	PyErr_Print();
	PyErr_Clear();
      }
      else
	Py_XDECREF(theResult);

      Py_XDECREF(theObject);
    }

    leavePython();
  }
  else
  {
    OTC_PyMonitor::handle(theEvent);

    return;
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyService::payloadToObject(
 OTC_ROPayload const& thePayload,
 PyObject* theCallback
)
{
  if (thePayload.nodeType() == "invalid")
  {
    PyErr_SetString(PyExc_TypeError,"invalid payload");

    return 0;
  }
  else if (thePayload.nodeType() == "scalar")
  {
    if (thePayload.valueType() == "xsd:string")
    {
      return Py_BuildValue("s#",
       thePayload.value().string(),
       thePayload.value().length());
    }
    else if (thePayload.valueType() == "xsd:integer" ||
     thePayload.valueType() == "xsd:long" ||
     thePayload.valueType() == "xsd:int" ||
     thePayload.valueType() == "xsd:short" ||
     thePayload.valueType() == "xsd:byte" ||
     thePayload.valueType() == "xsd:unsignedLong" ||
     thePayload.valueType() == "xsd:unsignedInt" ||
     thePayload.valueType() == "xsd:unsignedShort" ||
     thePayload.valueType() == "xsd:unsignedByte")
    {
      OTC_Boolean theStatus;
      theStatus = OTCLIB_TRUE;

      int theValue = 0;

      (thePayload,theStatus) >>= theValue;

      if (theStatus == OTCLIB_FALSE)
      {
	PyObject* theObject;
	char* theNext = 0;

	theObject = PyLong_FromString(
	 (char*)thePayload.value().string(),&theNext,10);

	if (theObject == 0)
	  return 0;

	int theLength;
	theLength = theNext - thePayload.value().string();

	if (theNext == 0 || *theNext != EOS ||
	 theLength != int(thePayload.value().length()))
	{
	  PyErr_SetString(PyExc_TypeError,"int conversion failed");

	  return 0;
	}

	return theObject;
      }

      return Py_BuildValue("i",theValue);
    }
    else if (thePayload.valueType() == "xsd:double" ||
     thePayload.valueType() == "xsd:float")
    {
      OTC_Boolean theStatus;
      theStatus = OTCLIB_TRUE;

      double theValue = 0;

      (thePayload,theStatus) >>= theValue;

      if (theStatus == OTCLIB_FALSE)
      {
	PyErr_SetString(PyExc_TypeError,"float conversion failed");

	return 0;
      }

      return Py_BuildValue("d",theValue);
    }
    else
    {
      PyObject* theArgs;
      theArgs = Py_BuildValue("(s,s#)",
       thePayload.valueType().string(),
       thePayload.value().string(),
       thePayload.value().length());

      PyObject* theResult;
      theResult = PyEval_CallObject(theCallback,theArgs);

      Py_XDECREF(theArgs);

      if (theResult == 0)
	return 0;

      return theResult;
    }
  }
  else if (thePayload.nodeType() == "index")
  {
    PyObject* theDictionary;
    theDictionary = PyDict_New();

    if (theDictionary == 0)
      return 0;

    OTC_ROPayload theElement;
    theElement = thePayload.firstChild();

    while (theElement.isValid())
    {
      PyObject* theKey = 0;
      PyObject* theValue = 0;

      theKey = PyString_FromStringAndSize(
       theElement.nodeName().string(),theElement.nodeName().length());
      theValue = payloadToObject(theElement,theCallback);

      if (theValue == 0)
      {
	Py_XDECREF(theKey);
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

      theElement = theElement.nextSibling();
    }

    return theDictionary;
  }
  else if (thePayload.nodeType() == "array")
  {
    PyObject* theList;
    theList = PyList_New(0);

    if (theList == 0)
      return 0;

    OTC_ROPayload theElement;
    theElement = thePayload.firstChild();

    while (theElement.isValid())
    {
      PyObject* theValue = 0;

      theValue = payloadToObject(theElement,theCallback);

      if (theValue == 0)
      {
	Py_XDECREF(theList);

	return 0;
      }

      if (PyList_Append(theList,theValue) == -1)
      {
	Py_XDECREF(theValue);
	Py_XDECREF(theList);

	return 0;
      }

      Py_XDECREF(theValue);

      theElement = theElement.nextSibling();
    }

    return theList;
  }

  PyErr_SetString(PyExc_TypeError,"invalid payload element");

  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_PyService::objectToPayload(
 PyObject* theObject,
 OTC_RWPayload thePayload,
 PyObject* theCallback
)
{
  if (PyString_Check(theObject))
  {
    char* theString = 0;
    int theLength = 0;

    if (!PyArg_Parse(theObject,"s#",&theString,&theLength))
      return OTCLIB_FALSE;

    thePayload.assign(theString,theLength);

    return OTCLIB_TRUE;
  }
#if defined(PyBool_Check)
  else if (PyInt_Check(theObject) && !PyBool_Check(theObject))
#else
  else if (PyInt_Check(theObject))
#endif
  {
    int theValue = 0;

    if (!PyArg_Parse(theObject,"i",&theValue))
      return OTCLIB_FALSE;

    thePayload <<= theValue;

    return OTCLIB_TRUE;
  }
  else if (PyFloat_Check(theObject))
  {
    double theValue = 0;

    if (!PyArg_Parse(theObject,"d",&theValue))
      return OTCLIB_FALSE;

    thePayload <<= theValue;

    return OTCLIB_TRUE;
  }
  else if (theObject == Py_None)
  {
    thePayload <<= OTC_ROPayload::nullValue();

    return OTCLIB_TRUE;
  }
  else if (PyDict_Check(theObject))
  {
    thePayload <<= OTC_ROPayload::nullIndex();

    PyObject* theItems;
    theItems = PyDict_Items(theObject);

    int theSize = PyList_Size(theItems);

    for (int i=0; i<theSize; i++)
    {
      PyObject* theKey;
      PyObject* theValue;
      PyObject* theItem;

      theItem = PyList_GET_ITEM(theItems,i);

      theKey = PyTuple_GET_ITEM(theItem,0);
      theValue = PyTuple_GET_ITEM(theItem,1);

      char const* theKeyString = 0;
      u_int theKeyLength = 0;

      if (!PyArg_Parse(theKey,"s#",&theKeyString,&theKeyLength))
      {
	Py_XDECREF(theItems);

	return OTCLIB_FALSE;
      }

      if (!objectToPayload(theValue,
       thePayload.sibling(theKeyString,theKeyLength),theCallback))
      {
	Py_XDECREF(theItems);

	return OTCLIB_FALSE;
      }
    }

    Py_XDECREF(theItems);

    return OTCLIB_TRUE;
  }
  else if (PyList_Check(theObject))
  {
    thePayload <<= OTC_ROPayload::nullArray();

    int theSize = PyList_Size(theObject);

    for (int i=0; i<theSize; i++)
    {
      PyObject* theItem;
      theItem = PyList_GET_ITEM(theObject,i);
      if (!objectToPayload(theItem,thePayload[-1],theCallback))
	return OTCLIB_FALSE;
    }

    return OTCLIB_TRUE;
  }
  else if (PyTuple_Check(theObject))
  {
    thePayload <<= OTC_ROPayload::nullArray();

    int theSize = PyTuple_Size(theObject);

    for (int i=0; i<theSize; i++)
    {
      PyObject* theItem;
      theItem = PyTuple_GET_ITEM(theObject,i);
      if (!objectToPayload(theItem,thePayload[-1],theCallback))
	return OTCLIB_FALSE;
    }

    return OTCLIB_TRUE;
  }
  else if (PyLong_Check(theObject))
  {
    PyObject* theString;
    theString = PyObject_Str(theObject);

    if (theString == 0)
      return OTCLIB_FALSE;

    char* tmpString = 0;
    tmpString = PyString_AsString(theString);

    if (tmpString == 0 || *tmpString == EOS)
    {
      Py_XDECREF(theString);

      return OTCLIB_FALSE;
    }

    thePayload.assign(tmpString,"xsd:long");

    Py_XDECREF(theString);

    return OTCLIB_TRUE;
  }
  else
  {
    PyObject* theArgs;
    theArgs = Py_BuildValue("(O)",theObject);

    PyObject* theResult;
    theResult = PyEval_CallObject(theCallback,theArgs);

    Py_XDECREF(theArgs);

    if (theResult == 0)
      return OTCLIB_FALSE;

    if (!PyTuple_Check(theResult) || PyTuple_Size(theResult) != 2)
    {
      Py_XDECREF(theResult);

      PyErr_SetString(PyExc_RuntimeError,"invalid result from encoder");

      return OTCLIB_FALSE;
    }

    PyObject* theItem;
    theItem = PyTuple_GET_ITEM(theResult,0);

    if (theItem == Py_None)
    {
      theItem = PyTuple_GET_ITEM(theResult,1);

      if (theItem != theObject)
      {
	// Data has been translated so need to
	// represent it to the encoder.

	OTC_Boolean tmpResult;
	tmpResult = objectToPayload(theItem,thePayload,theCallback);

	Py_XDECREF(theResult);

	return tmpResult;
      }
      else
      {
	Py_XDECREF(theResult);

	PyErr_SetString(PyExc_RuntimeError,"invalid result from encoder");

	return OTCLIB_FALSE;
      }
    }

    char* theType = 0;
    char* theString = 0;
    int theLength = 0;

    if (!PyArg_ParseTuple(theResult,"ss#",&theType,&theString,&theLength))
    {
      Py_XDECREF(theResult);

      PyErr_SetString(PyExc_RuntimeError,"invalid result from encoder");

      return OTCLIB_FALSE;
    }

    thePayload.assign(theString,theLength,theType);

    Py_XDECREF(theResult);

    return OTCLIB_TRUE;
  }

  PyErr_SetString(PyExc_TypeError,"object type not supported");

  return OTCLIB_FALSE;
}

/* ------------------------------------------------------------------------- */
