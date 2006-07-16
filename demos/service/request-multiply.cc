/*
// ============================================================================
//
// = FILENAME
//     request-multiply.cc
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
#include <OTC/dispatch/response.hh>
#include <OTC/dispatch/failure.hh>
#include <OTC/system/program.hh>
#include <OTC/types/time.hh>

#include <signal.h>

/* ------------------------------------------------------------------------- */

class Client2 : public OTC_EVAgent
{
  public:

    			Client2();

    void		handle(OTC_Event* theEvent);

  private:

    OTC_SVBroker	broker_;

    OTC_String		conversationId_;
};

Client2::Client2()
  : broker_(this)
{
  // The service broker is initialised above so
  // that this is an anonymous agent.

  // Start timer to trigger request.

  OTCEV_Timeout::start(id(),1000);
}

void Client2::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    // Lookup service with desired service name,
    // use the first which is found.

    OTC_SVRegistry* theRegistry;
    theRegistry = OTC_SVBroker::registry();

    OTC_Iterator<OTC_ServiceBinding*> theServices;
    theServices = theRegistry->lookupServiceName("SERVICE");

    if (theServices.isValid())
    {
      // Send a request to server to multiply two
      // values. For a request, use top level
      // array for arguments. In this case there
      // will be two arguments.

      OTC_SVPayload theObject;

      theObject[0] <<= (random() % 0xFF);
      theObject[1] <<= (random() % 0xFF);

      conversationId_ = broker_.sendRequest(
       theServices.item(),"multiply",theObject);
    }

    // Restart timer.

    OTCEV_Timeout::start(id(),1000);
  }
  else if (theEvent->type() == OTCEV_Response::typeId())
  {
    OTCEV_Response* theResponse;
    theResponse = (OTCEV_Response*)theEvent;

    if (theResponse->conversationId() == conversationId_)
    {
      // Retrieve the response data from the
      // response. Make sure that OTC_ROPayload is
      // used and not OTC_SVPayload as later will
      // result in an unecessary copy being made.

      OTC_ROPayload theObject;
      theObject = theResponse->content();

      // The status flag is used when decoding
      // to typed values. It should be set to
      // true to start with. If decoding of the
      // data is okay, it will still be true,
      // else it will be set to false.

      OTC_Boolean theStatus = OTCLIB_TRUE;

      // Decode the value. Value will come
      // back as "float" but we know we sent
      // "int". Read into an int.

      int theResult;

      (theObject,theStatus) >>= theResult;

      // Check the status flag to see if the value
      // was decoded.

      if (theStatus != OTCLIB_FALSE)
	cout << "result=" << theResult << endl;
    }
  }
  else if (theEvent->type() == OTCEV_Failure::typeId())
  {
    OTCEV_Failure* theFailure;
    theFailure = (OTCEV_Failure*)theEvent;

    cout << theFailure->conversationId() << " - ";
    cout << theFailure->description() << endl;
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

  // Create client.

  Client2 theClient2;

  // Run the dispatcher.

  OTC_Dispatcher::run();

  // Discard the client.

  theClient->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
