/*
// ============================================================================
//
// = FILENAME
//     agent-threading.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1999-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/event.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/dispatch/evhandle.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/dispatch/action.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/fnagent.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/tracer.hh>

#include <OTC/stdlib/iostream.h>

#include <signal.h>

/* ------------------------------------------------------------------------- */

class MyAgent : public OTC_EVAgent
{
  public:

			MyAgent()
			  : threadPool_(this,2) {}
			  	// Creates a thread pool with 2 threads.

			~MyAgent() { threadPool_.shutdown(); }

  protected:

    void		accept(OTC_Event* theEvent);
    				// Intercepts events and hands them off
				// to the thread pool.

    void		handle(OTC_Event* theEvent);
    				// Events handled here as per normal
				// except that this method will be
				// called in context of a thread.

  private:

    OTC_EVHandler	threadPool_;
    				// The thread pool.
};

void MyAgent::accept(OTC_Event* theEvent)
{
  OTC_Tracer tracer("MyAgent::accept()");

  // Intercepts event and hands it off
  // to the thread pool.

  threadPool_.process(theEvent);
}

void MyAgent::handle(OTC_Event* theEvent)
{
  OTC_Tracer tracer("MyAgent::handle()");

  if (theEvent == 0)
    return;

  // Normal event handling except that
  // this method will be called in the
  // context of a thread. Important to
  // use mutex locks on shared data if
  // this method were accessing any.

  if (theEvent->type() == OTCEV_Action::typeId())
  {
    OTCEV_Action* theAction;
    theAction = (OTCEV_Action*)theEvent;

    OTC_LogStream theLogger;
    theLogger << "EXECUTE " << id();
    theLogger << "/" << theAction->action() << endl;
  }

  theEvent->destroy();
}

int main()
{
  OTC_Logger::enableStderrOutput();
  OTC_Logger::enableLongFormat();

  OTC_Tracer::enableLogTrace();
  OTC_Tracer::disableTraceIndent();

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  // Create agents. Each will have 2
  // threads for handling of events.

  MyAgent theAgent1;
  MyAgent theAgent2;
  MyAgent theAgent3;

  // Schedule some events.

  OTC_LogStream theLogger;
  theLogger << "SCHEDULE" << endl;

  OTCEV_Action::schedule(theAgent1.id(),OTCLIB_STANDARD_JOB);
  OTCEV_Action::schedule(theAgent1.id(),OTCLIB_STANDARD_JOB);
  OTCEV_Action::schedule(theAgent1.id(),OTCLIB_STANDARD_JOB);

  OTCEV_Action::schedule(theAgent2.id(),OTCLIB_STANDARD_JOB);
  OTCEV_Action::schedule(theAgent2.id(),OTCLIB_STANDARD_JOB);
  OTCEV_Action::schedule(theAgent2.id(),OTCLIB_STANDARD_JOB);

  OTCEV_Action::schedule(theAgent3.id(),OTCLIB_STANDARD_JOB);
  OTCEV_Action::schedule(theAgent3.id(),OTCLIB_STANDARD_JOB);
  OTCEV_Action::schedule(theAgent3.id(),OTCLIB_STANDARD_JOB);

  // Run the dispatcher.

  theLogger << "RUN" << endl;

  OTC_Dispatcher::run();

  // Get here when SIGINT sent to the
  // process.

  theLogger << "EXIT" << endl;

  return 0;
}

