/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/_httpdaem.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2006 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/internet/httpdaem.hh>

#include <OTC/message/connect.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/system/program.hh>
#include <OTC/debug/tracer.hh>

#include <signal.h>
#include <stdlib.h>

/* ------------------------------------------------------------------------- */

class TST_Throttle : public OTC_EVAgent
{
  public:

                        TST_Throttle(
                         OTC_HttpDaemon* theDaemon,
                         u_int theSuspendThreshold,
                         u_int theResumeThreshold
                        );

  protected:

    void                handle(OTC_Event* theEvent);

  private:

    OTC_HttpDaemon*     daemon_;

    u_int               suspendThreshold_;

    u_int               resumeThreshold_;

    bool                isSuspended_;
};

TST_Throttle::TST_Throttle(
 OTC_HttpDaemon* theDaemon,
 u_int theSuspendThreshold,
 u_int theResumeThreshold
)
  : daemon_(theDaemon), suspendThreshold_(theSuspendThreshold),
    resumeThreshold_(theResumeThreshold), isSuspended_(false)
{
  daemon_->addObserver(id());
}

void TST_Throttle::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  cerr << daemon_->numClients() << endl;

  if (theEvent->type() == OTCEV_Connection::typeId())
  {
    OTCEV_Connection* theConnection;
    theConnection = (OTCEV_Connection*)theEvent;

    if (theConnection->state() == OTCLIB_CONNECTION_ACTIVE)
    {
      if (daemon_->numClients() >= suspendThreshold_)
      {
        if (!isSuspended_)
        {
          daemon_->suspendListening();
          isSuspended_ = true;

          cerr << "suspend" << endl;
        }
      }
    }
    else
    {
      if (isSuspended_)
      {
        if (daemon_->numClients() < resumeThreshold_)
        {
          daemon_->resumeListening();
          isSuspended_ = false;

          cerr << "resume" << endl;
        }
      }
    }
  }

  theEvent->destroy();
}

void test1()
{
  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  OTC_HttpDaemon theDaemon(8000);

  OTC_HttpServer theServer;
  theDaemon.attach("/",&theServer);
  theDaemon.attach("/1",&theServer);
  theDaemon.attach("/1/2",&theServer);
  theDaemon.attach("/1/2/3",&theServer);

  TST_Throttle theThrottle(&theDaemon,5,2);

  theDaemon.start();

  OTC_Dispatcher::run();
}

/* ------------------------------------------------------------------------- */

typedef void (*testFunc)();

testFunc tests[] =
{
  test1
};

/* ------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
  u_int const numTests = sizeof(tests)/sizeof(tests[0]);

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
