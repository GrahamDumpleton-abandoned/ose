/*
// ============================================================================
//
// = FILENAME
//     request-reply.cc
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
#include <OTC/dispatch/request.hh>
#include <OTC/dispatch/response.hh>
#include <OTC/dispatch/failure.hh>
#include <OTC/types/time.hh>

#include <signal.h>

/* ------------------------------------------------------------------------- */

class Server : public OTC_EVAgent
{
  public:

    			Server();

    void		handle(OTC_Event* theEvent);

  private:

    OTC_SVBroker	broker_;

    OTC_Time		startTime_;
};

Server::Server()
  : broker_(this,"SERVICE")
{
  // The service broker is initialised above so
  // that this agent has service name "SERVICE".
}

void Server::handle(OTC_Event* theEvent)
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

class Client1 : public OTC_EVAgent
{
  public:

    			Client1();

    void		handle(OTC_Event* theEvent);

  private:

    OTC_SVBroker	broker_;

    OTC_String		conversationId_;
};

Client1::Client1()
  : broker_(this)
{
  // The service broker is initialised above so
  // that this is an anonymous agent.

  // Start timer to trigger request.

  OTCEV_Timeout::start(id(),1000);
}

void Client1::handle(OTC_Event* theEvent)
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
      // Send a request to server to get the
      // uptime. For a request, use top level
      // array for arguments. In this case though,
      // there are no arguments so use null array.

      conversationId_ = broker_.sendRequest(theServices.item(),
       "uptime",OTC_ROPayload::nullArray());
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

      // Decode the value.

      OTC_Duration theDuration;

      (theObject,theStatus) >>= theDuration;

      // Check the status flag to see if the value
      // was decoded.

      if (theStatus != OTCLIB_FALSE)
	cout << "uptime=" << theDuration << endl;
    }
  }

  theEvent->destroy();
}

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

int main(int argc, char* argv[])
{
  // Initialise dispatcher.

  OTC_Dispatcher::initialise();

  // Shutdown gracefully if we get CTRL-C sent
  // to the process.

  OTCEV_Shutdown::monitor(SIGINT);

  // Create clients.

  Client1 theClient1;
  Client2 theClient2;

  // Create server.

  Server theServer;

  // Start the dispatcher.

  OTC_Dispatcher::run();

  return 0;
}

/* ------------------------------------------------------------------------- */
