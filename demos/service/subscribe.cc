/*
// ============================================================================
//
// = FILENAME
//     subscribe.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/exchange.hh>
#include <OTC/message/inetclnt.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/dispatch/svbroker.hh>
#include <OTC/dispatch/report.hh>
#include <OTC/system/program.hh>
#include <OTC/types/time.hh>

#include <signal.h>

/* ------------------------------------------------------------------------- */

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

void usage()
{
  cerr << "Usage: " << OTC_Program::basename();
  cerr << " -connect server [ -domain name ]" << endl;
}

int main(int argc, char* argv[])
{
  set_terminate(otclib_terminate_function);

  // Save away program arguments.

  OTC_Program::initialise(argc,argv);

  // Decode the arguments.

  OTC_String theServerString;
  OTC_String theDomainName;

  while (OTC_Program::numOptions() != 0)
  {
    if (OTC_Program::option(1) == "-connect")
    {
      OTC_Program::shift(1);
      if (OTC_Program::numOptions() == 0)
	break;

      theServerString = OTC_Program::option(1);
    }
    else if (OTC_Program::option(1) == "-domain")
    {
      OTC_Program::shift(1);
      if (OTC_Program::numOptions() == 0)
	break;

      theDomainName = OTC_Program::option(1);
    }
    else
    {
      usage();
      return 1;
    }

    OTC_Program::shift(1);
  }

  if (theServerString.isEmpty())
  {
    usage();
    return 1;
  }

  // Initialise the dispatcher.

  OTC_Dispatcher::initialise();

  // Monitor signals for shutdown.

  OTCEV_Shutdown::monitor(SIGINT);

  // Connect to the exchange process.

  OTC_String theHostname;
  int thePort = 0;

  OTC_Exchange theExchange(OTCLIB_EXCHANGE_CLIENT,theDomainName);

  char const* theHostPort;
  theHostPort = theServerString.string();

  while (*theHostPort != EOS && *theHostPort != ':')
    theHostname += *theHostPort++;

  if (*theHostPort == ':')
    theHostPort++;

  if (sscanf(theHostPort,"%d",&thePort) != 1)
    thePort = 0;

  if (theHostname.isEmpty() || thePort == 0)
  {
    cerr << OTC_Program::basename() << ": invalid server" << endl;
    return 1;
  }

  OTC_InetClient* theClient;
  theClient = new OTC_InetClient("MESSAGE",theHostname,thePort,1000);
  theClient->reference();

  theExchange.connect(theClient);

  // Create subscriber.

  Subscriber theSubscriber;

  // Run the dispatcher.

  OTC_Dispatcher::run();

  // Discard the client.

  theClient->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
