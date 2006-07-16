/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyagent.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pyagent.hh>

#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/alarm.hh>
#include <OTC/dispatch/action.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/dispatch/ioevent.hh>
#include <OTC/dispatch/event.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
#ifdef OTCLIB_TRACE
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_PyAgent");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()
#endif

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyAgent::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Agent",
  sizeof(OTC_PyAgent),
  0,
  OTC_PyAgent::sys_dealloc,
  0,
  OTC_PyAgent::sys_getattr,
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
PyMethodDef OTC_PyAgent::gMethods_[] = {
  { "acquireObject", OTC_PyAgent::mfn_acquireObject, 1 },
  { "releaseObject", OTC_PyAgent::mfn_releaseObject, 1 },
  { "activateThreading", OTC_PyAgent::mfn_activateThreading, 1 },
  { "suspendThreading", OTC_PyAgent::mfn_suspendThreading, 1 },
  { "threadingEnabled", OTC_PyAgent::mfn_threadingEnabled, 1 },
  { "id", OTC_PyAgent::mfn_id, 1 },
  { "startTimer", OTC_PyAgent::mfn_startTimer, 1 },
  { "cancelTimer", OTC_PyAgent::mfn_cancelTimer, 1 },
  { "cancelAllTimers", OTC_PyAgent::mfn_cancelAllTimers, 1 },
  { "setAlarm", OTC_PyAgent::mfn_setAlarm, 1 },
  { "cancelAlarm", OTC_PyAgent::mfn_cancelAlarm, 1 },
  { "cancelAllAlarms", OTC_PyAgent::mfn_cancelAllAlarms, 1 },
  { "scheduleAction", OTC_PyAgent::mfn_scheduleAction, 1 },
  { "scheduleCronAction", OTC_PyAgent::mfn_scheduleCronAction, 1 },
  { "cancelAction", OTC_PyAgent::mfn_cancelAction, 1 },
  { "cancelAllActions", OTC_PyAgent::mfn_cancelAllActions, 1 },
  { "subscribeSignal", OTC_PyAgent::mfn_subscribeSignal, 1 },
  { "unsubscribeSignal", OTC_PyAgent::mfn_unsubscribeSignal, 1 },
  { "unsubscribeAllSignals", OTC_PyAgent::mfn_unsubscribeAllSignals, 1 },
  { "scheduleShutdown", OTC_PyAgent::mfn_scheduleShutdown, 1 },
  { "suspendShutdown", OTC_PyAgent::mfn_suspendShutdown, 1 },
  { "resumeShutdown", OTC_PyAgent::mfn_resumeShutdown, 1 },
  { "subscribeShutdown", OTC_PyAgent::mfn_subscribeShutdown, 1 },
  { "unsubscribeShutdown", OTC_PyAgent::mfn_unsubscribeShutdown, 1 },
  { "subscribeSocket", OTC_PyAgent::mfn_subscribeSocket, 1 },
  { "unsubscribeSocket", OTC_PyAgent::mfn_unsubscribeSocket, 1 },
  { "unsubscribeAllSockets", OTC_PyAgent::mfn_unsubscribeAllSockets, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyAgent::~OTC_PyAgent()
{
  if (threadState_ != 0)
    PyThreadState_Delete(threadState_);

  if (threadPool_ != 0)
    delete threadPool_;

  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_PyAgent::OTC_PyAgent(PyObject* theOuter)
  : outer_(theOuter), threadPool_(0), threadingSuspended_(OTCLIB_FALSE),
    interpreterState_(0), threadState_(0)
{
  ob_type = &gType_;

  _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;

  if (!PyArg_ParseTuple(theArgs,"O",&theOuter))
    return 0;

  OTC_PyAgent* theAgent;
  theAgent = new OTC_PyAgent(theOuter);
  OTCLIB_ASSERT_M(theAgent != 0);

  return (PyObject*)theAgent;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyAgent::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyAgent*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyAgent*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_acquireObject(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->mutex_.lock();

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_releaseObject(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->mutex_.unlock();

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_activateThreading(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theQueueSize = 0;

  if (!PyArg_ParseTuple(theArgs,"|i",&theQueueSize))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->mutex_.lock();

  if (theSelf->threadPool_ == 0)
  {
    theSelf->threadPool_ = new OTC_EVHandler(theSelf,1,theQueueSize);
    OTCLIB_ASSERT_M(theSelf->threadPool_ != 0);
  }

  theSelf->mutex_.unlock();

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_suspendThreading(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  if (theSelf->threadPool_ != 0)
    theSelf->threadPool_->suspend();

  theSelf->threadingSuspended_ = OTCLIB_TRUE;

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_threadingEnabled(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  int theResult = 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  theResult = (theSelf->threadPool_ != 0);

  return Py_BuildValue("i",theResult);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_id(PyObject* theInstance, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  return Py_BuildValue("i",theSelf->id());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_startTimer(PyObject* theInstance, PyObject* theArgs)
{
  double theDelay = 0;
  char* theDescription = 0;

  if (!PyArg_ParseTuple(theArgs,"ds",&theDelay,&theDescription,-1))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  int theTimerId;

  theTimerId = OTCEV_Timeout::start(
   theSelf->id(),int(1000*theDelay),theDescription);

  return Py_BuildValue("i",theTimerId);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_cancelTimer(PyObject* theInstance, PyObject* theArgs)
{
  char* theDescription = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theDescription))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_Timeout::cancelAgent(theSelf->id(),theDescription,-1);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_cancelAllTimers(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_Timeout::cancelAgent(theSelf->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_setAlarm(PyObject* theInstance, PyObject* theArgs)
{
  int theTime = 0;
  char* theDescription = 0;

  if (!PyArg_ParseTuple(theArgs,"is",&theTime,&theDescription,-1))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  int theAlarmId;

  theAlarmId = OTCEV_Alarm::set(theSelf->id(),theTime,theDescription);

  return Py_BuildValue("i",theAlarmId);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_cancelAlarm(PyObject* theInstance, PyObject* theArgs)
{
  char* theDescription = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theDescription))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_Alarm::cancelAgent(theSelf->id(),theDescription,-1);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_cancelAllAlarms(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_Alarm::cancelAgent(theSelf->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_scheduleAction(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theType = 0;
  char* theDescription = 0;

  theType = OTCLIB_STANDARD_JOB;

  if (!PyArg_ParseTuple(theArgs,"is",&theType,&theDescription))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  int theActionId;

  theActionId = OTCEV_Action::schedule(theSelf->id(),
   theType,theDescription,-1);

  return Py_BuildValue("i",theActionId);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_scheduleCronAction(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theSpecification = 0;
  char* theDescription = 0;

  if (!PyArg_ParseTuple(theArgs,"ss",&theSpecification,&theDescription))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  int theActionId;

  theActionId = OTCEV_Action::schedule(theSelf->id(),
   theSpecification,theDescription,-1);

  return Py_BuildValue("i",theActionId);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_cancelAction(PyObject* theInstance, PyObject* theArgs)
{
  char* theDescription = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theDescription))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_Action::cancelAgent(theSelf->id(),theDescription,-1);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_cancelAllActions(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_Action::cancelAgent(theSelf->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_subscribeSignal(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theSignal = 0;

  if (!PyArg_ParseTuple(theArgs,"i",&theSignal))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_Signal::subscribe(theSelf->id(),theSignal);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_unsubscribeSignal(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theSignal = 0;

  if (!PyArg_ParseTuple(theArgs,"i",&theSignal))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_Signal::unsubscribe(theSelf->id(),theSignal);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_unsubscribeAllSignals(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_Signal::unsubscribeAgent(theSelf->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_scheduleShutdown(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  double theDelay= 0;

  if (!PyArg_ParseTuple(theArgs,"d",&theDelay))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_Shutdown::schedule(int(1000*theDelay));

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_suspendShutdown(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_Shutdown::suspend();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_resumeShutdown(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_Shutdown::resume();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_subscribeShutdown(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_Shutdown::subscribe(theSelf->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_unsubscribeShutdown(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_Shutdown::unsubscribe(theSelf->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_subscribeSocket(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theSocket = 0;
  int theEvents = 0;

  if (!PyArg_ParseTuple(theArgs,"ii",&theSocket,&theEvents))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_IOEvent::subscribe(theSelf->id(),theSocket,theEvents);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_unsubscribeSocket(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theSocket = 0;
  int theEvents = 0;

  if (!PyArg_ParseTuple(theArgs,"ii",&theSocket,&theEvents))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_IOEvent::unsubscribe(theSelf->id(),theSocket,theEvents);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyAgent::mfn_unsubscribeAllSockets(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_PyAgent* theSelf;
  theSelf = (OTC_PyAgent*)theInstance;

  OTCEV_IOEvent::unsubscribeAgent(theSelf->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
void OTC_PyAgent::execute(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_PyAgent::execute()");

  OTC_PyDispatcher::enterPython();

  Py_XINCREF(this);

  OTC_PyDispatcher::leavePython();

  // XXX How safe is this???

  if (threadPool_ == 0)
    OTC_EVAgent::execute(theEvent);
  else
    threadPool_->process(theEvent);

  OTC_PyDispatcher::enterPython();

  Py_XDECREF(this);

  OTC_PyDispatcher::leavePython();
}

/* ------------------------------------------------------------------------- */
void OTC_PyAgent::filter(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_PyAgent::filter()");

#ifdef WITH_THREAD
  if (threadPool_ != 0 && threadState_ == 0)
  {
    PyEval_InitThreads();
    threadState_ = PyThreadState_New(interpreterState_);
  }
#endif

  enterPython();

  Py_XINCREF(this);

  leavePython();

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  // If threading has already been suspended
  // because of pending destruction of the
  // object, all we can do is throw the event
  // away.

  if (threadingSuspended_ == OTCLIB_FALSE)
    handle(theEvent);

  xxxMutex.release();

  mutex_.unlock();

  enterPython();

  Py_XDECREF(this);

  leavePython();
}

/* ------------------------------------------------------------------------- */
void OTC_PyAgent::handle(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_PyAgent::handle()");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << "event = " << *theEvent << endl;

  if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    OTCEV_Timeout* theTimeout;
    theTimeout = (OTCEV_Timeout*)theEvent;

    enterPython();

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleTimer","s",theTimeout->description().string());

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Alarm::typeId())
  {
    OTCEV_Alarm* theAlarm;
    theAlarm = (OTCEV_Alarm*)theEvent;

    enterPython();

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleAlarm","s",theAlarm->description().string());

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Action::typeId())
  {
    OTCEV_Action* theAction;
    theAction = (OTCEV_Action*)theEvent;

    enterPython();

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleAction","s",theAction->description().string());

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Signal::typeId())
  {
    OTCEV_Signal* theSignal;
    theSignal = (OTCEV_Signal*)theEvent;

    enterPython();

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleSignal","i",theSignal->signal());

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Shutdown::typeId())
  {
    OTCEV_Shutdown* theShutdown;
    theShutdown = (OTCEV_Shutdown*)theEvent;

    enterPython();

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleShutdown","i",theShutdown->category());

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_IOEvent::typeId())
  {
    OTCLIB_TRACER(MODULE) << "i/o event" << endl;

    OTCEV_IOEvent* theIOEvent;
    theIOEvent = (OTCEV_IOEvent*)theEvent;

    // Note that this code expects only one event
    // ever sent back at a time. This is okay for
    // now as only allow subscription to input
    // being ready on the socket.

    OTCLIB_TRACER(MODULE) << "call method" << endl;

    enterPython();

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleSocket","ii",theIOEvent->fd(),theIOEvent->events());

    OTCLIB_TRACER(MODULE) << "result = " << (void*)theResult << endl;

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    leavePython();
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
