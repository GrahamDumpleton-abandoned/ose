/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpymnitor.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pymnitor.hh>
#include <OTC/python/pysrvbnd.hh>
#include <OTC/python/pydsptch.hh>
#include <OTC/dispatch/servannc.hh>
#include <OTC/dispatch/grpannc.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyMonitor::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Monitor",
  sizeof(OTC_PyMonitor),
  0,
  OTC_PyMonitor::sys_dealloc,
  0,
  OTC_PyMonitor::sys_getattr,
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
PyMethodDef OTC_PyMonitor::gMethods_[] = {
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
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyMonitor::~OTC_PyMonitor()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_PyMonitor::OTC_PyMonitor(PyObject* theOuter)
  : OTC_PyAgent(theOuter)
{
  ob_type = &gType_;

  // _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;

  if (!PyArg_ParseTuple(theArgs,"O",&theOuter))
  {
    return 0;
  }

  OTC_PyMonitor* theAgent;
  theAgent = new OTC_PyMonitor(theOuter);
  OTCLIB_ASSERT_M(theAgent != 0);

  return (PyObject*)theAgent;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyMonitor::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyMonitor*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyMonitor*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::mfn_serviceAgents(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  OTC_Iterator<OTC_ServiceBinding*> theBindings;
  theBindings = OTC_SVBroker::registry()->serviceAgents();

  PyObject* theList;
  theList = PyList_New(0);

  if (theList == 0)
    return 0;

  while (theBindings.isValid())
  {
    OTC_PyServiceBinding* theObject;
    theObject = new OTC_PyServiceBinding(theBindings.item());
    OTCLIB_ASSERT_M(theObject != 0);

    if (PyList_Append(theList,theObject) == -1)
    {
      Py_XDECREF(theObject);
      Py_XDECREF(theList);

      return 0;
    }

    Py_XDECREF(theObject);

    theBindings.next();
  }

  return theList;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::mfn_serviceGroups(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceAddress = 0;

  if (!PyArg_ParseTuple(theArgs,"|z",&theServiceAddress))
    return 0;

  OTC_Iterator<OTC_String> theGroups;
  theGroups = OTC_SVBroker::registry()->serviceGroups(theServiceAddress);

  PyObject* theList;
  theList = PyList_New(0);

  if (theList == 0)
    return 0;

  while (theGroups.isValid())
  {
    PyObject* theObject;
    theObject = Py_BuildValue("s",theGroups.item().string());

    if (PyList_Append(theList,theObject) == -1)
    {
      Py_XDECREF(theObject);
      Py_XDECREF(theList);

      return 0;
    }

    Py_XDECREF(theObject);

    theGroups.next();
  }

  return theList;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::mfn_lookupServiceName(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceName = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceName))
    return 0;

  OTC_Iterator<OTC_ServiceBinding*> theBindings;
  theBindings = OTC_SVBroker::registry()->lookupServiceName(theServiceName);

  PyObject* theList;
  theList = PyList_New(0);

  if (theList == 0)
    return 0;

  while (theBindings.isValid())
  {
    OTC_PyServiceBinding* theObject;
    theObject = new OTC_PyServiceBinding(theBindings.item());
    OTCLIB_ASSERT_M(theObject != 0);

    if (PyList_Append(theList,theObject) == -1)
    {
      Py_XDECREF(theObject);
      Py_XDECREF(theList);

      return 0;
    }

    Py_XDECREF(theObject);

    theBindings.next();
  }

  return theList;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::mfn_lookupServiceAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceAddress = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceAddress))
    return 0;

  OTC_Iterator<OTC_ServiceBinding*> theBindings;
  theBindings = OTC_SVBroker::registry()->lookupServiceAddress(
   theServiceAddress);

  if (!theBindings.isValid())
  {
    Py_XINCREF(Py_None);

    return Py_None;
  }

  OTC_PyServiceBinding* theObject;
  theObject = new OTC_PyServiceBinding(theBindings.item());
  OTCLIB_ASSERT_M(theObject != 0);

  return theObject;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::mfn_lookupServiceGroup(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceGroup = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceGroup))
    return 0;

  OTC_Iterator<OTC_ServiceBinding*> theBindings;
  theBindings = OTC_SVBroker::registry()->lookupServiceGroup(
   theServiceGroup);

  PyObject* theList;
  theList = PyList_New(0);

  if (theList == 0)
    return 0;

  while (theBindings.isValid())
  {
    OTC_PyServiceBinding* theObject;
    theObject = new OTC_PyServiceBinding(theBindings.item());
    OTCLIB_ASSERT_M(theObject != 0);

    if (PyList_Append(theList,theObject) == -1)
    {
      Py_XDECREF(theObject);
      Py_XDECREF(theList);

      return 0;
    }

    Py_XDECREF(theObject);

    theBindings.next();
  }

  return theList;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::mfn_unsubscribeAllRegistry(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_SVBroker::registry()->unsubscribeAgent(
   ((OTC_PyMonitor*)theInstance)->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::mfn_subscribeRegistry(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_SVBroker::registry()->subscribeRegistry(
   ((OTC_PyMonitor*)theInstance)->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::mfn_unsubscribeRegistry(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_SVBroker::registry()->unsubscribeRegistry(
   ((OTC_PyMonitor*)theInstance)->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::mfn_subscribeServiceName(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceName = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceName))
    return 0;

  OTC_SVBroker::registry()->subscribeServiceName(
   ((OTC_PyMonitor*)theInstance)->id(),theServiceName);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::mfn_unsubscribeServiceName(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceName = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceName))
    return 0;

  OTC_SVBroker::registry()->unsubscribeServiceName(
   ((OTC_PyMonitor*)theInstance)->id(),theServiceName);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::mfn_subscribeServiceAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceAddress = 0;
  char* theSubscriptionName = 0;

  if (!PyArg_ParseTuple(theArgs,"s|s",&theServiceAddress,&theSubscriptionName))
    return 0;

  OTC_ServiceStatus theStatus;

  theStatus = OTC_SVBroker::registry()->subscribeServiceAddress(
   ((OTC_PyMonitor*)theInstance)->id(),theServiceAddress,theSubscriptionName);

  return Py_BuildValue("i",theStatus);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::mfn_unsubscribeServiceAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceAddress = 0;
  char* theSubscriptionName = 0;

  if (!PyArg_ParseTuple(theArgs,"s|s",&theServiceAddress,&theSubscriptionName))
    return 0;

  OTC_SVBroker::registry()->unsubscribeServiceAddress(
   ((OTC_PyMonitor*)theInstance)->id(),theServiceAddress,theSubscriptionName);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::mfn_subscribeServiceGroup(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceGroup = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceGroup))
    return 0;

  OTC_SVBroker::registry()->subscribeServiceGroup(
   ((OTC_PyMonitor*)theInstance)->id(),theServiceGroup);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyMonitor::mfn_unsubscribeServiceGroup(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceGroup = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceGroup))
    return 0;

  OTC_SVBroker::registry()->unsubscribeServiceGroup(
   ((OTC_PyMonitor*)theInstance)->id(),theServiceGroup);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
void OTC_PyMonitor::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
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
    OTC_PyAgent::handle(theEvent);

    return;
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
