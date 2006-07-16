/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/otcpyexchng.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pyexchng.hh>
#include <OTC/python/pyexchnc.hh>

#ifdef ANY
#undef ANY
#endif

#include <OTC/python/pydsptch.hh>

#include <OTC/message/inetclnt.hh>
#include <OTC/message/inetlstn.hh>
#include <OTC/message/exchannc.hh>

/* ------------------------------------------------------------------------- */
class OTC_PyExchangeListener : public OTC_InetListener
{
  public:
    			~OTC_PyExchangeListener();
    			OTC_PyExchangeListener(
			 char const* theInterface,
			 int thePort,
			 OTC_PyExchange* theExchange
			)
			  : OTC_InetListener(theInterface,thePort),
			    exchange_(theExchange) {}
  protected:
    OTC_Boolean		authorise(OTC_String const& theClientHost);
  private:
    OTC_PyExchange*	exchange_;
};

/* ------------------------------------------------------------------------- */
OTC_PyExchangeListener::~OTC_PyExchangeListener() { }

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_PyExchangeListener::authorise(OTC_String const& theClientHost)
{
  return exchange_->authorise(theClientHost);
}

/* ------------------------------------------------------------------------- */
PyTypeObject OTC_PyExchange::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Exchange",
  sizeof(OTC_PyExchange),
  0,
  OTC_PyExchange::sys_dealloc,
  0,
  OTC_PyExchange::sys_getattr,
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
PyMethodDef OTC_PyExchange::gMethods_[] = {
  { "connect", OTC_PyExchange::mfn_connect, 1 },
  { "listen", OTC_PyExchange::mfn_listen, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OTC_PyExchange::~OTC_PyExchange()
{
  exchange_.registry()->unsubscribe(id());

  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_PyExchange::OTC_PyExchange(
 PyObject* theOuter,
 OTC_ExchangeType theType,
 char const* theGroup
)
  : outer_(theOuter), exchange_(theType,theGroup)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  exchange_.registry()->subscribe(id());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyExchange::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  OTC_ExchangeType theType = OTCLIB_EXCHANGE_CLIENT;
  char* theGroup = 0;

  if (!PyArg_ParseTuple(theArgs,"Ois",
   &theOuter,&theType,&theGroup))
  {
    return 0;
  }

  OTC_PyExchange* theExchange;
  theExchange = new OTC_PyExchange(theOuter,theType,theGroup);
  OTCLIB_ASSERT_M(theExchange != 0);

  return (PyObject*)theExchange;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyExchange::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OTC_PyExchange::sys_dealloc(PyObject* theInstance)
{
  delete (OTC_PyExchange*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyExchange::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OTC_PyExchange*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyExchange::mfn_connect(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theHostName = 0;
  int thePort = 0;
  double theRetryDelay = -1;

  if (!PyArg_ParseTuple(theArgs,"sid",
   &theHostName,&thePort,&theRetryDelay))
  {
    return 0;
  }

  if (theRetryDelay > 0)
    theRetryDelay = 1000*theRetryDelay;

  OTC_PyExchange* theSelf;
  theSelf = (OTC_PyExchange*)theInstance;

  OTC_InetClient* theClient;
  theClient = new OTC_InetClient("MESSAGE",
   theHostName,thePort,int(theRetryDelay));
  OTCLIB_ASSERT_M(theClient != 0);

  OTC_String theLocalAddress;
  theLocalAddress = theClient->localAddress();

  theSelf->exchange_.connect(theClient);

  return Py_BuildValue("s",theLocalAddress.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OTC_PyExchange::mfn_listen(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int thePort = 0;

  if (!PyArg_ParseTuple(theArgs,"i",&thePort))
    return 0;

  OTC_PyExchange* theSelf;
  theSelf = (OTC_PyExchange*)theInstance;

  OTC_InetListener* theListener;
  theListener = new OTC_PyExchangeListener("MESSAGE",thePort,theSelf);
  OTCLIB_ASSERT_M(theListener != 0);

  theSelf->exchange_.listen(theListener);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
void OTC_PyExchange::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_ExchangeAnnouncement::typeId())
  {
    OTCEV_ExchangeAnnouncement* theAnnouncement;
    theAnnouncement = (OTCEV_ExchangeAnnouncement*)theEvent;

    OTC_PyDispatcher::enterPython();

    OTC_PyExchangeAnnouncement* theObject;
    theObject = new OTC_PyExchangeAnnouncement(theAnnouncement);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleConnection","O",theObject);

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
OTC_Boolean OTC_PyExchange::authorise(OTC_String const& theClientHost)
{
  OTC_Boolean theBoolean;
  theBoolean = OTCLIB_FALSE;

  OTC_PyDispatcher::enterPython();

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,
   "_authorise","s",theClientHost.string());

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();
  }
  else
  {
    if (theResult != Py_None)
      theBoolean = OTCLIB_TRUE;

    Py_XDECREF(theResult);
  }

  OTC_PyDispatcher::leavePython();

  return theBoolean;
}

/* ------------------------------------------------------------------------- */
