/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     validate.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/internet/rpcgtway.hh>

#include <OTC/internet/httpdaem.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/request.hh>
#include <OTC/dispatch/shutdown.hh>

#include <stdlib.h>
#include <signal.h>

/* ------------------------------------------------------------------------- */

class TST_Validator : public OTC_EVAgent
{
  public:

    			TST_Validator()
			  : broker_(this,"validator")
				{ broker_.joinGroup("test"); }

  protected:

    void		handle(OTC_Event* theEvent);

  private:

    OTC_SVBroker	broker_;
};

void TST_Validator::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Request::typeId())
  {
    OTCEV_Request* theRequest;
    theRequest = (OTCEV_Request*)theEvent;

    if (theRequest->methodName() == "validator1.arrayOfStructsTest")
    {
      OTC_ROPayload thePayload;
      OTC_ROPayload theArgument;
      OTC_ROPayload theElement;
      OTC_Boolean theStatus;
      int theValue;

      thePayload = theRequest->content();
      theArgument = thePayload[0];

      if (theArgument.nodeType() == "array")
      {
	int theTotal = 0;

	theElement = theArgument[0];

	theStatus = OTCLIB_TRUE;

	while(theElement.isValid())
	{
	  (theElement["curly"],theStatus) >>= theValue;

	  if (theStatus == OTCLIB_FALSE)
	    break;

	  theTotal += theValue;

	  theElement = theElement.nextSibling();
	}

	if (theStatus != OTCLIB_FALSE)
	{
	  OTC_SVPayload theResult;

	  theResult <<= theTotal;

	  theRequest->sendResponse(theResult);
	}
	else
	  theRequest->sendFailure(3,"Decode of argument failed.");
      }
      else
	theRequest->sendFailure(3,"Unexpected argument type.");
    }
    else if (theRequest->methodName() == "validator1.countTheEntities")
    {
      OTC_ROPayload thePayload;
      OTC_ROPayload theArgument;
      OTC_Boolean theStatus;
      OTC_String theValue;

      thePayload = theRequest->content();
      theArgument = thePayload[0];

      theStatus = OTCLIB_TRUE;

      (theArgument,theStatus) >>= theValue;

      if (theStatus != OTCLIB_FALSE)
      {
	int ctLeftAngleBrackets = 0;
	int ctRightAngleBrackets = 0;
	int ctAmpersands = 0;
	int ctApostrophes = 0;
	int ctQuotes = 0;

	u_int theIndex;

	for (theIndex=0; theIndex<theValue.length(); theIndex++)
	{
	  switch (theValue[theIndex])
	  {
	    case '<':
	    {
	      ctLeftAngleBrackets++;
	      break;
	    }

	    case '>':
	    {
	      ctRightAngleBrackets++;
	      break;
	    }

	    case '&':
	    {
	      ctAmpersands++;
	      break;
	    }

	    case '\'':
	    {
	      ctApostrophes++;
	      break;
	    }

	    case '"':
	    {
	      ctQuotes++;
	      break;
	    }
	  }
	}

	OTC_SVPayload theResult;

	theResult["ctLeftAngleBrackets"] <<= ctLeftAngleBrackets;
	theResult["ctRightAngleBrackets"] <<= ctRightAngleBrackets;
	theResult["ctAmpersands"] <<= ctAmpersands;
	theResult["ctApostrophes"] <<= ctApostrophes;
	theResult["ctQuotes"] <<= ctQuotes;

	theRequest->sendResponse(theResult);
      }
      else
	theRequest->sendFailure(3,"Decode of argument failed.");
    }
    else if (theRequest->methodName() == "validator1.easyStructTest")
    {
      OTC_ROPayload thePayload;
      OTC_ROPayload theArgument;
      OTC_Boolean theStatus;

      int theValue1;
      int theValue2;
      int theValue3;

      thePayload = theRequest->content();
      theArgument = thePayload[0];

      theStatus = OTCLIB_TRUE;

      (theArgument["moe"],theStatus) >>= theValue1;
      (theArgument["larry"],theStatus) >>= theValue2;
      (theArgument["curly"],theStatus) >>= theValue3;

      if (theStatus != OTCLIB_FALSE)
      {
	int theTotal = 0;

	theTotal += theValue1;
	theTotal += theValue2;
	theTotal += theValue3;

	OTC_SVPayload theResult;

	theResult <<= theTotal;

	theRequest->sendResponse(theResult);
      }
      else
	theRequest->sendFailure(3,"Decode of argument failed.");
    }
    else if (theRequest->methodName() == "validator1.echoStructTest")
    {
      OTC_ROPayload thePayload;
      OTC_ROPayload theArgument;

      thePayload = theRequest->content();
      theArgument = thePayload[0];

      if (theArgument.nodeType() == "index")
	theRequest->sendResponse(theArgument);
      else
	theRequest->sendFailure(3,"Unexpected argument type.");
    }
    else if (theRequest->methodName() == "validator1.manyTypesTest")
    {
      OTC_ROPayload thePayload;
      OTC_ROPayload theArgument;
      OTC_Boolean theStatus;

      int theNumber;
      OTC_Boolean theBoolean;
      OTC_String theString;
      double theDouble;
      OTC_Time theDateTime;
      OTC_String theBinary;

      thePayload = theRequest->content();

      theStatus = OTCLIB_TRUE;

      (thePayload[0],theStatus) >>= theNumber;
      (thePayload[1],theStatus) >>= theBoolean;
      (thePayload[2],theStatus) >>= theString;
      (thePayload[3],theStatus) >>= theDouble;
      (thePayload[4],theStatus) >>= theDateTime;
      (thePayload[5],theStatus) >>= theBinary;

      OTC_SVPayload theResult;

      if (theStatus != OTCLIB_FALSE)
      {
	theResult[-1] <<= theNumber;
	theResult[-1] <<= theBoolean;
	theResult[-1] <<= theString;
	theResult[-1] <<= theDouble;
	theResult[-1] <<= theDateTime;

	theResult[-1].assign(theBinary,theBinary.length(),"xsd:base64Binary");

	theRequest->sendResponse(theResult);
      }
      else
	theRequest->sendFailure(3,"Decode of argument failed.");
    }
    else if (theRequest->methodName() == "validator1.moderateSizeArrayCheck")
    {
      OTC_ROPayload thePayload;
      OTC_ROPayload theArgument;
      OTC_Boolean theStatus;

      OTC_String theString;

      OTC_Deque<OTC_String> theItems;

      thePayload = theRequest->content();
      theArgument = thePayload[0];

      theStatus = OTCLIB_TRUE;

      if (theArgument.nodeType() == "array")
      {
	(theArgument,theStatus) >>= theItems;

	if (theStatus != OTCLIB_FALSE)
	{
	  if (theItems.population() == 1)
	    theString = theItems.first();
	  else
	    theString = theItems.first() + theItems.last();

	  OTC_SVPayload theResult;

	  theResult <<= theString;

	  theRequest->sendResponse(theResult);
	}
	else
	  theRequest->sendFailure(3,"Decode of argument failed.");
      }
      else
	theRequest->sendFailure(3,"Unexpected argument type.");
    }
    else if (theRequest->methodName() == "validator1.nestedStructTest")
    {
      OTC_ROPayload thePayload;
      OTC_ROPayload theArgument;
      OTC_Boolean theStatus;

      int theValue1;
      int theValue2;
      int theValue3;

      thePayload = theRequest->content();
      theArgument = thePayload[0];

      theStatus = OTCLIB_TRUE;

      theArgument = theArgument["2000.04.01"];

      (theArgument["moe"],theStatus) >>= theValue1;
      (theArgument["curly"],theStatus) >>= theValue2;
      (theArgument["larry"],theStatus) >>= theValue3;

      if (theStatus != OTCLIB_FALSE)
      {
	theValue1 += theValue2;
	theValue1 += theValue3;

	OTC_SVPayload theResult;

	theResult <<= theValue1;

	theRequest->sendResponse(theResult);
      }
      else
	theRequest->sendFailure(3,"Decode of argument failed.");
    }
    else if (theRequest->methodName() == "validator1.simpleStructReturnTest")
    {
      OTC_ROPayload thePayload;
      OTC_ROPayload theArgument;
      OTC_Boolean theStatus;

      int theValue1;
      int theValue2;
      int theValue3;

      thePayload = theRequest->content();
      theArgument = thePayload[0];

      theStatus = OTCLIB_TRUE;

      (theArgument,theStatus) >>= theValue1;

      if (theStatus != OTCLIB_FALSE)
      {
	theValue3 = theValue1 * 1000;
	theValue2 = theValue1 * 100;
	theValue1 = theValue1 * 10;

	OTC_SVPayload theResult;

	theResult["times10"] <<= theValue1;
	theResult["times100"] <<= theValue2;
	theResult["times1000"] <<= theValue3;

	theRequest->sendResponse(theResult);
      }
      else
	theRequest->sendFailure(3,"Decode of argument failed.");
    }
    else
      theRequest->sendFailure(1,"Unknown method requested.");
  }

  theEvent->destroy();
}

int main()
{
  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  OTC_HttpDaemon theDaemon(8000);

  TST_Validator theValidator;

  OTC_RpcGateway theServer("test");
  theDaemon.attach("/netrpc",&theServer);

  theDaemon.start();

  OTC_Dispatcher::run();

  return 0;
}
