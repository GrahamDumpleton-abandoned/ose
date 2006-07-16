/*
// ============================================================================
//
// = FILENAME
//     publish.cc
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
#include <OTC/dispatch/timeout.hh>
#include <OTC/system/program.hh>
#include <OTC/types/time.hh>

#include <signal.h>

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

  // Create publisher.

  Publisher thePublisher;

  // Run the dispatcher.

  OTC_Dispatcher::run();

  // Discard the client.

  theClient->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
