/*
// ============================================================================
//
// = FILENAME
//     reply.cc
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
#include <OTC/dispatch/request.hh>
#include <OTC/dispatch/response.hh>
#include <OTC/system/program.hh>
#include <OTC/types/time.hh>

#include <signal.h>

/* ------------------------------------------------------------------------- */

class Service : public OTC_EVAgent
{
  public:

    			Service();

    void		handle(OTC_Event* theEvent);

  private:

    OTC_SVBroker	broker_;

    OTC_Time		startTime_;
};

Service::Service()
  : broker_(this,"SERVICE")
{
  // The service broker is initialised above so
  // that this agent has service name "SERVICE".

  // Now join "PUBLIC" service group. This is
  // only so that RPC gateway can export this
  // service. Any group name could be used as
  // long as the RPC gateway is using the same.

  broker_.joinGroup("PUBLIC");
}

void Service::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Request::typeId())
  {
    OTCEV_Request* theRequest;
    theRequest = (OTCEV_Request*)theEvent;

    if (theRequest->methodName() == "uptime")
    {
      OTC_SVPayload theResult;

      // Return a value with how long service
      // has been active. Ignore any arguments.

      OTC_Duration theDuration;
      theDuration = OTC_Time::currentTime() - startTime_;

      theResult <<= theDuration;

      // Return the result.

      theRequest->sendResponse(theResult);
    }
    else if (theRequest->methodName() == "multiply")
    {
      // Retrieve the request data from the
      // request. Make sure that OTC_ROPayload is
      // used and not OTC_SVPayload as later will
      // result in an unecessary copy being made.

      OTC_ROPayload theObject;
      theObject = theRequest->content();

      // The status flag is used when decoding
      // to typed values. It should be set to
      // true to start with. If decoding of the
      // data is okay, it will still be true,
      // else it will be set to false.

      OTC_Boolean theStatus = OTCLIB_TRUE;

      // Expect two arguments. They may be of type
      // "int" or "float". Read into double so can
      // handle both.

      double theArgument1;
      double theArgument2;

      (theObject[0],theStatus) >>= theArgument1;
      (theObject[1],theStatus) >>= theArgument2;

      // Check the status flag to see if the value
      // was decoded.

      if (theStatus != OTCLIB_FALSE)
      {
	OTC_SVPayload theResult;

	theResult <<= (theArgument1 * theArgument2);

	theRequest->sendResponse(theResult);
      }
      else
	theRequest->sendFailure(1,"unable to decode arguments");
    }
    else
      theRequest->sendFailure(1,"method not supported");
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

  // Create server.

  Service theService;

  // Run the dispatcher.

  OTC_Dispatcher::run();

  // Discard the client.

  theClient->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
