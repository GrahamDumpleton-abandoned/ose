/*
// ============================================================================
//
// = FILENAME
//     simple-threading.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1999 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/action.hh>
#include <OTC/dispatch/evcllbck.hh>
#include <OTC/dispatch/evthread.hh>
#include <OTC/thread/nrmutex.hh>
#include <OTC/thread/cvmutex.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/logstrm.hh>

#include <OTC/stdlib/iostream.h>

/* ------------------------------------------------------------------------- */

class MyFnCallback : public OTC_EVCallback
{
  public:

			~MyFnCallback();

			MyFnCallback(void (*theCallback)(OTC_Event*))
			  : callback_(theCallback) {}
			  	// Creates callback object for normal
				// function.

    void		execute(OTC_Event* theEvent);
    				// Will call function with the event.

  private:

  			MyFnCallback(MyFnCallback const&);

    MyFnCallback&	operator=(MyFnCallback const&);

    void		(*callback_)(OTC_Event*);
    				// The function to be called.
};

MyFnCallback::~MyFnCallback() {}

void MyFnCallback::execute(OTC_Event* theEvent)
{
  // Calls the function with the event.

  if (callback_ != 0)
    callback_(theEvent);
}

static u_int COUNT;

static OTC_NRMutex MUTEX;
static OTC_CVMutex CVMUTEX(MUTEX);

void process(OTC_Event* theEvent)
{
  OTC_Tracer tracer("process()");

  if (theEvent == 0)
    return;

  // Counter being used to know when all
  // threads are finished must be protected
  // by a mutex. When counter gets to 0
  // used condition variable mutex to
  // notify main thread that all threads
  // have finished.

  if (theEvent->type() == OTCEV_Action::typeId())
  {
    OTCEV_Action* theAction;
    theAction = (OTCEV_Action*)theEvent;

    MUTEX.lock();

    OTC_LogStream theLogger;
    theLogger << "EXECUTE " << theAction->action() << endl;

    COUNT--;

    if (COUNT == 0)
      CVMUTEX.broadcast();

    MUTEX.unlock();
  }

  theEvent->destroy();
}

int main()
{
  OTC_Logger::enableStderrOutput();
  OTC_Logger::enableLongFormat();

  OTC_Tracer::enableLogTrace();
  OTC_Tracer::disableTraceIndent();

  // Create a thread pool with 5 threads.
  // Set the maxmimum queue size to 10.
  // This will cause main thread to block
  // at times while threads catch up
  // with the work they have been given.

  u_int theNumThreads = 5;
  u_int theQueueSize = 10;

  OTC_EVThreadPool theThreadPool(theNumThreads,theQueueSize);

  // Start the threads.

  OTC_LogStream theLogger;
  theLogger << "STARTUP" << endl;

  theThreadPool.startup();

  // Schedule a whole of jobs. This should
  // block at times since number of jobs is
  // greater than queue size.

  u_int theNumJobs = 100;

  COUNT = theNumJobs;

  while (theNumJobs != 0)
  {
    OTCEV_Action* theAction;
    theAction = new OTCEV_Action(theNumJobs);

    MyFnCallback* theCallback;
    theCallback = new MyFnCallback(process);

    theLogger << "SCHEDULE " << theNumJobs << endl;

    theThreadPool.process(theAction,theCallback);

    theNumJobs--;
  }

  // All jobs scheduled, so wait until
  // all the threads have finished.

  theLogger << "WAIT" << endl;

  MUTEX.lock();

  while (COUNT != 0)
    CVMUTEX.wait();

  MUTEX.unlock();

  // Shutdown the thread pool, causing
  // the threads to exit.

  theLogger << "FINISHED" << endl;

  theThreadPool.shutdown();

  return 0;
}

