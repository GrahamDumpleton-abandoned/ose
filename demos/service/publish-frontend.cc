/*
// ============================================================================
//
// = FILENAME
//     publish-frontend.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/exchange.hh>
#include <OTC/message/inetlstn.hh>
#include <OTC/message/inetclnt.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/dispatch/report.hh>
#include <OTC/program/program.hh>

#include <signal.h>

#if defined(SYS_UNIX)
#include <netdb.h>
#include <arpa/inet.h>
#else
#include <windows.h>
#include <winsock.h>
#endif

#include <OTC/stdlib/strstream.h>

/* ------------------------------------------------------------------------- */

class Proxy : public OTC_EVAgent
{
  public:

    			Proxy();

    void		handle(OTC_Event* theEvent);

  private:

    OTC_SVBroker	broker_;

    OTC_Time		last_;
};

Proxy::Proxy()
  : broker_(this,"SEED")
{
  // The service broker is initialised above so
  // that this agent has service name "SEED".

  // Subscribe to subject name "next" of the
  // backend publishers

  broker_.subscribeReports("BACKEND:SEED","next");
}

void Proxy::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Report::typeId())
  {
    OTCEV_Report* theReport;
    theReport = (OTCEV_Report*)theEvent;

    OTC_ROPayload theObject;
    theObject = theReport->content();

    OTC_Boolean theStatus = OTCLIB_TRUE;

    OTC_Time theTime;
    long theValue = 0;

    (theObject["time"],theStatus) >>= theTime;
    (theObject["value"],theStatus) >>= theValue;

    if (theStatus != OTCLIB_FALSE)
    {
      // Only republish the report if it
      // is newer than the last we published.

      if (theTime > last_)
      {
	last_ = theTime;

	broker_.publishReport("next",theObject);
      }
    }
  }

  theEvent->destroy();
}

void usage()
{
  cerr << "Usage: " << OTC_Program::basename();
  cerr << " -listen port -connect server [ -domain name ]" << endl;
}

int main(int argc, char* argv[])
{
  set_terminate(otclib_terminate_function);

  // Save away program arguments.

  OTC_Program::initialise(argc,argv);

  // Decode arguments.

  OTC_String thePortString;
  OTC_String theServerString;
  OTC_String theDomainName;

  while (OTC_Program::numOptions() != 0)
  {
    if (OTC_Program::option(1) == "-listen")
    {
      OTC_Program::shift(1);
      if (OTC_Program::numOptions() == 0)
        break;

      thePortString = OTC_Program::option(1);
    }
    else if (OTC_Program::option(1) == "-connect")
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

    OTC_Program::shift(1);
  }
  
  if (thePortString.isEmpty() || theServerString.isEmpty())
  {
    usage();
    return 1;
  }

  if (theDomainName == "BACKEND")
  {
    cerr << OTC_Program::basename();
    cerr << ": Invalid group name." << endl;

    return 1;
  }

  // Initialise dispatcher.

  OTC_Dispatcher::initialise();

  // Monitor signals for shutdown.

  OTCEV_Shutdown::monitor(SIGINT);

  // Create backend server exchange.

  OTC_Exchange theBackendExchange(OTCLIB_EXCHANGE_SERVER,"BACKEND");

  // Create client exchange.

  OTC_Exchange theClientExchange(OTCLIB_EXCHANGE_CLIENT,theDomainName);

  // Create the listener for the
  // backend exchange.

  int thePort;

  if (sscanf(thePortString,"%d",&thePort) != 1)
  {
    cerr << OTC_Program::basename() << ": invalid port number" << endl;
    return 1;
  }

  OTC_InetListener* theListener;
  theListener = new OTC_InetListener("MESSAGE",thePort);
  theListener->reference();

  theBackendExchange.listen(theListener);

  if (theListener->fd() == -1)
  {
    cerr << OTC_Program::basename() << ": listener failed" << endl;
    return 1;
  }

  // Create client for client exchange.

  OTC_String theHostname;

  char const* theHostPort;
  theHostPort = theServerString;

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

  theClientExchange.connect(theClient);

  // Create proxy service.

  Proxy theProxy;

  // Run the dispatcher.

  OTC_Dispatcher::run();

  // Discard the listener.

  theListener->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
