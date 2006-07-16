/*
// ============================================================================
//
// = FILENAME
//     publish-subscribe.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/dispatch/svbroker.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/report.hh>
#include <OTC/types/time.hh>

#include <signal.h>

#if !defined(SYS_UNIX)
#include <windows.h>
#include <winsock.h>
#endif

/* ------------------------------------------------------------------------- */

class Publisher : public OTC_EVAgent
{
  public:

    			Publisher();

    void		handle(OTC_Event* theEvent);

  private:

    OTC_SVBroker	broker_;
};

Publisher::Publisher()
  : broker_(this,"SEED")
{
  // The service broker is initialised above so
  // that this agent has service name "SEED".

  // Start timer to trigger first publication.

  OTCEV_Timeout::start(id(),1000);
}

void Publisher::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    OTC_SVPayload theObject;

    // Build a struct with two members to contain
    // the data. The time will be stored as a
    // "date-time" and the long as an "int".

    theObject["time"] <<= OTC_Time::currentTime();
    theObject["value"] <<= random();

    // Publish the data against the subject "next".

    broker_.publishReport("next",theObject);

    // Start time to trigger next publication.

    OTCEV_Timeout::start(id(),1000);
  }

  theEvent->destroy();
}

class Subscriber : public OTC_EVAgent
{
  public:

    			Subscriber();

    void		handle(OTC_Event* theEvent);

  private:

    OTC_SVBroker	broker_;
};

Subscriber::Subscriber()
  : broker_(this)
{
  // The service broker is initialised above so
  // that this is an anonymous agent. This is done
  // as nothing needs to subscribe to this agent.

  // Subscribe to all reports from the service
  // with name "SEED" published against the
  // subject name "next".

  broker_.subscribeReports("SEED","next");
}

void Subscriber::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Report::typeId())
  {
    OTCEV_Report* theReport;
    theReport = (OTCEV_Report*)theEvent;

    // Retrieve the published data from the
    // report. Make sure that OTC_ROPayload is
    // used and not OTC_SVPayload as later will
    // result in an unecessary copy being made.

    OTC_ROPayload theObject;
    theObject = theReport->content();

    // The status flag is used when decoding
    // to typed values. It should be set to
    // true to start with. If decoding of the
    // data is okay, it will still be true,
    // else it will be set to false.

    OTC_Boolean theStatus = OTCLIB_TRUE;

    // Retrieve the members from the struct.
    // The values will be read direct into
    // typed values.

    OTC_Time theTime;
    long theValue = 0;

    (theObject["time"],theStatus) >>= theTime;
    (theObject["value"],theStatus) >>= theValue;

    // Check the status flag to see if the struct
    // members existed, were of the correct type
    // and that the values could be decoded.

    if (theStatus != OTCLIB_FALSE)
      cout << theTime << " - " << theValue << endl;
  }

  theEvent->destroy();
}

int main(int argc, char* argv[])
{
  // Initialise dispatcher.

  OTC_Dispatcher::initialise();

  // Shutdown gracefully if we get CTRL-C sent
  // to the process.

  OTCEV_Shutdown::monitor(SIGINT);

  // Create subscriber.

  Subscriber theSubscriber;

  // Create publisher.

  Publisher thePublisher;

  // Start the dispatcher.

  OTC_Dispatcher::run();

  return 0;
}

/* ------------------------------------------------------------------------- */
