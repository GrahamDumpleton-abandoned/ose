/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/_xmlrpcgw.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/internet/xmlrpcgw.hh>

#include <OTC/internet/httpdaem.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/request.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/types/time.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>
#include <signal.h>

/* ------------------------------------------------------------------------- */

class TST_EchoService : public OTC_EVAgent
{
  public:

    			TST_EchoService()
			  : broker_(this,"echo")
			  	{ broker_.joinGroup("public"); }

  protected:

    void		handle(OTC_Event* theEvent);

  private:

    OTC_SVBroker	broker_;
};

void TST_EchoService::handle(OTC_Event* theEvent)
{
  OTC_Tracer tracer("TST_EchoService::handle(OTC_Event*)");

  if (theEvent == 0)
    return;

  tracer() << *theEvent << endl;

  if (theEvent->type() == OTCEV_Request::typeId())
  {
    OTCEV_Request* theRequest;
    theRequest = (OTCEV_Request*)theEvent;

    theRequest->sendResponse(theRequest->content());
  }

  theEvent->destroy();
}

class TST_TimeService : public OTC_EVAgent
{
  public:

    			TST_TimeService()
			  : broker_(this,"time")
			  	{ broker_.joinGroup("public"); }

  protected:

    void		handle(OTC_Event* theEvent);

  private:

    OTC_SVBroker	broker_;
};

void TST_TimeService::handle(OTC_Event* theEvent)
{
  OTC_Tracer tracer("TST_TimeService::handle(OTC_Event*)");

  if (theEvent == 0)
    return;

  tracer() << *theEvent << endl;

  if (theEvent->type() == OTCEV_Request::typeId())
  {
    OTCEV_Request* theRequest;
    theRequest = (OTCEV_Request*)theEvent;

    OTC_SVPayload theObject;
    theObject <<= OTC_Time::currentTime();

    theRequest->sendResponse(theObject);
  }

  theEvent->destroy();
}

class TST_FailService : public OTC_EVAgent
{
  public:

    			TST_FailService()
			  : broker_(this,"fail")
			  	{ broker_.joinGroup("public"); }

  protected:

    void		handle(OTC_Event* theEvent);

  private:

    OTC_SVBroker	broker_;
};

void TST_FailService::handle(OTC_Event* theEvent)
{
  OTC_Tracer tracer("TST_FailService::handle(OTC_Event*)");

  if (theEvent == 0)
    return;

  tracer() << *theEvent << endl;

  if (theEvent->type() == OTCEV_Request::typeId())
  {
    OTCEV_Request* theRequest;
    theRequest = (OTCEV_Request*)theEvent;

    theRequest->sendFailure(0,"fail");
  }

  theEvent->destroy();
}

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  OTC_HttpDaemon theDaemon(8000);

  OTC_XmlRpcGateway theServer("public");
  theDaemon.attach("/service",&theServer);

  TST_EchoService theService1;
  TST_TimeService theService2;
  TST_FailService theService3;

  theDaemon.start();

  OTC_Dispatcher::run();
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1
};

/* ------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
  u_int const numTests = sizeof(tests)/sizeof(tests[0]);

  set_terminate(otclib_terminate_handler);

  if (argc != 2)
  {
    cout << numTests << endl;
    return 1;
  }
  else
  {
    int testNum = atoi(argv[1]);
    if (testNum > 0 && u_int(testNum) <= numTests)
    {
      tests[testNum-1]();
      return 0;
    }
    else
      return 1;
  }
}

/* ------------------------------------------------------------------------- */
