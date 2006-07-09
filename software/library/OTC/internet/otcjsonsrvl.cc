/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/otcjsonsrvl.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2006 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/internet/jsonsrvl.hh"
#endif
#endif

#include <OTC/internet/jsonsrvl.hh>

#include <OTC/dispatch/response.hh>
#include <OTC/dispatch/failure.hh>
#include <OTC/dispatch/servannc.hh>
#include <OTC/text/osstream.hh>
#include <OTC/collctn/cbitset.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

#include <OTC/stdlib/strstream.h>

#include <string.h>
#include <ctype.h>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_JsonRpcServlet");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
static char const* const HEX_CHARACTERS = "0123456789abcdef";
#define HEX_DIGIT(x) (((x) <= '9') ? (x) - '0' : ((x) & 7) + 9)

/* ------------------------------------------------------------------------- */
OTC_JsonRpcServlet::~OTC_JsonRpcServlet()
{
  cleanup_();
}

/* ------------------------------------------------------------------------- */
OTC_JsonRpcServlet::OTC_JsonRpcServlet(
 OTC_HttpSession* theSession,
 OTC_ServiceBinding* theBinding
)
  : OTC_HttpServlet(theSession), binding_(0), broker_(0), requestId_("0")
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_JsonRpcServlet::OTC_JsonRpcServlet(...)");

  if (theBinding != 0)
    binding_ = theBinding->clone();
}

/* ------------------------------------------------------------------------- */
void OTC_JsonRpcServlet::processRequest()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_JsonRpcServlet::processRequest()");

  // Determine if we have a service to send
  // the request to.

  if (binding_ == 0)
  {
    sendError(404,"No such service available.");
 
    return;
  }

  // Eliminate requests we can't handle.

  if (requestMethod() != "POST")
  {
    sendError(405,"Only POST method supported by servlet.");

    return;
  }

  if (contentLength() == -1)
  {
    sendError(400,"Content length must be specified.");

    return;
  }

  // Presize buffer into which full content
  // of request is read. Limit this to a
  // maximum value in case an attempt is
  // being made to crash the system.

  int const MAXIMUM = 32768;

  OTCLIB_TRACER(MODULE) << "content-length = " << contentLength() << endl;

  if (contentLength() < MAXIMUM)
    content_.capacity(contentLength());
  else
    content_.capacity(MAXIMUM);

  // The remainder of the processing is now
  // dependent on receiving actual content.
}

/* ------------------------------------------------------------------------- */
void OTC_JsonRpcServlet::processContent(OTC_String const& theContent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_JsonRpcServlet::processContent()");

  // Ignore any extra content beyond that
  // specified by content length.

  if (int(content_.length()) >= contentLength())
    return;

  content_ += theContent;

  // Wait for more if don't yet have what
  // is specified by content length.

  if (int(content_.length()) < contentLength())
    return;
 
  // Attempt to decode request and send it
  // on its way if everything is okay.

  OTCLIB_TRACER(MODULE) << content_ << endl << endl;

  istrstream theStream((char*)content_.string(),content_.length());

  OTC_Capacity theCapacity1(63);
  OTC_String theMethod(theCapacity1);

  OTC_Capacity theCapacity2(content_.length());
  OTC_String theWorkarea(theCapacity2);

  OTC_SVPayload theObject;

  if (decodeRequest_(theStream,requestId_,theMethod,
   theObject,theWorkarea) == false)
  {
    OTCLIB_TRACER(MODULE) << "id = " << requestId_ << endl;
    OTCLIB_TRACER(MODULE) << "method = " << theMethod << endl << endl;
    OTCLIB_TRACER(MODULE) << theObject << endl;
    OTCLIB_TRACER(MODULE) << OTC_String::get(theStream,EOF) << endl;

    sendFailure_(54,"Request Decoding Error","netsvc.jsonrpc");

    return;
  }

  OTCLIB_TRACER(MODULE) << "id = " << requestId_ << endl;
  OTCLIB_TRACER(MODULE) << "method = " << theMethod << endl;
  OTCLIB_TRACER(MODULE) << theObject << endl;

  // Store meta information about request in
  // message payload.

  theObject.setProperty("REMOTE_ADDR",clientHost());
  theObject.setProperty("REMOTE_USER",remoteUser());

  // Bind ourselves as an anonymous service
  // and send the request.

  broker_ = new OTC_SVBroker(this,"","",OTCLIB_SERVICE_HIDDEN);
  OTCLIB_ASSERT_M(broker_ != 0);

  conversationId_ = broker_->sendRequest(binding_,
   theMethod,theObject);

  // Subscribe to existance of the specific
  // service in case it disappears before
  // we get a response back.

  OTC_SVBroker::registry()->subscribeServiceAddress(
   id(),binding_->serviceAddress());
}

/* ------------------------------------------------------------------------- */
OTC_ServiceBinding* OTC_JsonRpcServlet::service(char const* theName)
{
  OTC_Iterator<OTC_ServiceBinding*> theBindings;
  theBindings = OTC_SVBroker::registry()->lookupServiceName(theName);

  if (theBindings.isValid())
    return theBindings.item()->clone();

  return 0;
}

/* ------------------------------------------------------------------------- */
void OTC_JsonRpcServlet::process(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_JsonRpcServlet::process(OTC_Event*)");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << *theEvent << endl;

  if (theEvent->type() == OTCEV_Response::typeId())
  {
    OTCEV_Response* theResponse;
    theResponse = (OTCEV_Response*)theEvent;

    if (theResponse->conversationId() == conversationId_)
    {
      OTC_ROPayload theObject;

      theObject = theResponse->content();

      if (theObject.isValid())
      {
	// Make a guess at the required capacity of
	// the string for the payload when formatted
	// as JSON-RPC.

	OTCEV_Message* tmpMessage;
	tmpMessage = theResponse->envelope()->message();

	size_t tmpCapacity = tmpMessage->contentLength();

	OTCLIB_TRACER(MODULE) << "content-length = " << tmpCapacity << endl;

	tmpCapacity = size_t(tmpCapacity);

	OTC_Capacity theCapacity(tmpCapacity);
	OTC_String theString(theCapacity);

	OTC_OSStream theStream(theString,OTCLIB_BUFFERED);

	if (encodeResponse_(theStream,requestId_,theObject) != false)
	{
	  theStream << flush;

	  sendResponse(200);

	  sendHeader("Content-Type","text/plain");
	  sendHeader("Content-Length",theString.length());

	  endHeaders();

	  sendContent(theString);

	  endContent();

	  cleanup_();
	}
	else
	  sendFailure_(52,"Response Encoding Error","netsvc.jsonrpc");
      }
      else
	sendFailure_(52,"Response Encoding Error","netsvc.jsonrpc");
    }
  }
  else if (theEvent->type() == OTCEV_Failure::typeId())
  {
    OTCEV_Failure* theFailure;
    theFailure = (OTCEV_Failure*)theEvent;

    if (theFailure->conversationId() == conversationId_)
    {
      sendFailure_(theFailure->error(),theFailure->description(),
       theFailure->origin(),theFailure->details());
    }
  }
  else if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* theAnnouncement;
    theAnnouncement = (OTCEV_ServiceAnnouncement*)theEvent;

    if (theAnnouncement->status() == OTCLIB_SERVICE_WITHDRAWN)
    {
      // Service has disappeared before we got back
      // a response.

      sendFailure_(57,"Application Error","netsvc.jsonrpc",
       "Service was withdrawn before response received.");
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_JsonRpcServlet::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  process(theEvent);

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
bool OTC_JsonRpcServlet::decodeRequest_(
 istream& theStream,
 OTC_String& theRequestId,
 OTC_String& theMethod,
 OTC_RWPayload& theObject,
 OTC_String& theWorkarea
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_JsonRpcServlet::decodeRequest_()");

  theMethod.truncate();

  // Ensure that payload starts out as
  // an empty array so that empty array
  // gets passed back if no params.

  theObject <<= OTC_ROPayload::nullArray();

  // Skip leading whitespace.

  theStream >> ws;

  // Verify we have a request and work
  // out what the method name is.

  OTC_Capacity theCapacity(63);
  OTC_String theString(theCapacity);

  if (theStream.peek() != '{')
    return false;

  if (!decodeValue_(theStream,theObject,theWorkarea))
    return false;

  OTC_RWPayload tmpPayload;

  tmpPayload = theObject.sibling("method");

  if (!tmpPayload.isValid())
    return false;

  if (tmpPayload.valueType() != "xsd:string")
    return false;

  theMethod.assign(tmpPayload.value().string(),
   tmpPayload.value().length());

  tmpPayload = theObject.sibling("id");

  if (!tmpPayload.isValid())
    return false;

  if (tmpPayload.nodeType() != "scalar")
    return false;

  if (tmpPayload.valueType() == "xsd:string")
  {
    // The jsolait package uses a string for the ID,
    // so need to encode it back into a quoted string.

    theRequestId.truncate();
    OTC_OSStream tmpStream(theRequestId,OTCLIB_BUFFERED);
    encodeString_(tmpStream,tmpPayload.value().string(),
     tmpPayload.value().length());
    tmpStream << flush;
  }
  else
  {
    theRequestId.assign(tmpPayload.value().string(),
     tmpPayload.value().length());

    // If request ID is empty, assume it was null and
    // explicitly set it to that so it will be returned.

    if (theRequestId.isEmpty())
      theRequestId = "null";
  }

  tmpPayload = theObject.sibling("params");

  if (!tmpPayload.isValid())
    return false;

  if (tmpPayload.nodeType() != "array")
    return false;

  theObject = tmpPayload;

  return true;
}

/* ------------------------------------------------------------------------- */
void OTC_JsonRpcServlet::encodeFailure_(
 ostream& theStream,
 OTC_String const& theRequestId,
 int theError,
 char const* theDescription,
 char const* theOrigin,
 char const* theDetails
)
{
  theStream << "{ \"result\": null";

  theStream << ", \"error\": ";
  theStream << "{";

  theStream << "\"error\": ";
  theStream << theError;

  theStream << ", \"description\": ";
  encodeString_(theStream,theDescription);
  theStream << ", ";

  theStream << "\"origin\": ";
  encodeString_(theStream,theOrigin);
  theStream << ", ";

  theStream << "\"details\": ";
  encodeString_(theStream,theDetails);
  theStream << " ";

  theStream << "}";
  
  theStream << ", \"id\": " << theRequestId;
  theStream << " }";

  theStream << flush;
}

/* ------------------------------------------------------------------------- */
void OTC_JsonRpcServlet::sendFailure_(
 int theError,
 char const* theDescription,
 char const* theOrigin,
 char const* theDetails
)
{
  OTC_Capacity theCapacity(1024);
  OTC_String theString(theCapacity);

  OTC_OSStream theStream(theString,OTCLIB_BUFFERED);

  encodeFailure_(theStream,requestId_,theError,
   theDescription,theOrigin,theDetails);

  theStream << flush;

  sendResponse(200);

  sendHeader("Content-Type","text/plain");
  sendHeader("Content-Length",theString.length());

  endHeaders();

  sendContent(theString);

  endContent();

  cleanup_();
}

/* ------------------------------------------------------------------------- */
bool OTC_JsonRpcServlet::encodeResponse_(
 ostream& theStream,
 OTC_String const& theRequestId,
 OTC_ROPayload const& theObject
)
{
  theStream << "{ \"result\": ";

  if (!encodeObject_(theStream,theObject))
    return false;

  theStream << ", \"error\": null";
  theStream << ", \"id\": " << theRequestId;
  theStream << "}";
  theStream << flush;

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_JsonRpcServlet::encodeObject_(
 ostream& theStream,
 OTC_ROPayload const& theObject
)
{
  if (theObject.nodeType() == "scalar")
  {
    if (theObject.property("type") == "xsd:integer" ||
     theObject.property("type") == "xsd:long" ||
     theObject.property("type") == "xsd:int" ||
     theObject.property("type") == "xsd:short" ||
     theObject.property("type") == "xsd:byte" ||
     theObject.property("type") == "xsd:unsignedLong" ||
     theObject.property("type") == "xsd:unsignedInt" ||
     theObject.property("type") == "xsd:unsignedShort" ||
     theObject.property("type") == "xsd:unsignedByte" ||
     theObject.property("type") == "xsd:double" ||
     theObject.property("type") == "xsd:float")
    {
      theStream.write(theObject.value().string(),
       theObject.value().length());
    }
    else if (theObject.property("type") == "xsd:boolean")
    {
      if (theObject.value() == "false" || theObject.value() == "0")
	theStream << "false";
      else if (theObject.value() == "true" || theObject.value() == "1")
	theStream << "true";
      else
	return false;
    }
    else if (theObject.property("type") == "")
    {
      theStream << "null";
    }
    else
    {
      encodeString_(theStream,theObject.value().string(),
       theObject.value().length());
    }
  }
  else if (theObject.nodeType() == "array")
  {
    theStream << "[";

    OTC_ROPayload theReader;
    theReader = theObject.firstChild();

    while (theReader.isValid())
    {
      if (!encodeObject_(theStream,theReader))
	return false;

      theReader = theReader.nextSibling();

      if (theReader.isValid())
        theStream << ",";
    }

    theStream << "]";
  }
  else if (theObject.nodeType() == "index")
  {
    theStream << "{";

    OTC_ROPayload theReader;
    theReader = theObject.firstChild();

    while (theReader.isValid())
    {
      encodeString_(theStream,
       theReader.nodeName().string(),
       theReader.nodeName().length());

      theStream << ":";

      if (!encodeObject_(theStream,theReader))
	return false;

      theReader = theReader.nextSibling();

      if (theReader.isValid())
        theStream << ",";
    }

    theStream << "}";
  }
  else
    return false;

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_JsonRpcServlet::decodeValue_(
 istream& theStream,
 OTC_RWPayload theObject,
 OTC_String& theWorkarea
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_JsonRpcServlet::decodeValue_()");

  OTC_Capacity theCapacity(63);

  theWorkarea.truncate();

  theStream >> ws;

  int theInput;

  theInput = theStream.peek();

  switch (theInput)
  {
    case '[':
    {
      theObject <<= OTC_ROPayload::nullArray();

      theStream.ignore(1);
      theStream >> ws;

      if (theStream.peek() != ']')
      {
        while (1)
        {
          if (!decodeValue_(theStream,theObject[-1],theWorkarea))
            return false;

          theStream >> ws;

          if (theStream.peek() == ']')
            break;
          else if (theStream.peek() != ',')
            return false;

          theStream.ignore(1);
        }
      }

      theStream.ignore(1);

      break;
    }

    case '{':
    {
      theObject <<= OTC_ROPayload::nullIndex();

      theStream.ignore(1);
      theStream >> ws;

      while (theStream.peek() != '}')
      {
        if (theStream.peek() != '"')
          return false;

        OTC_String theMember(theCapacity);

        if (!readString_(theStream,theMember))
          return false;

        theStream >> ws;

        if (theStream.get() != ':')
          return false;

        theStream >> ws;

        if (!decodeValue_(theStream,
         theObject.sibling(theMember),theWorkarea))
        {
          return false;
        }

        theStream >> ws;

        if (theStream.peek() != '}')
        {
          if (theStream.peek() != ',')
            return false;

          theStream.ignore(1);
          theStream >> ws;
        }
      }

      theStream.ignore(1);

      break;
    }

    case '"':
    {
      if (!readString_(theStream,theWorkarea))
        return false;

      theObject.assign(theWorkarea,theWorkarea.length());

      break;
    }

    case 'n':
    case 't':
    case 'f':
    {
      theWorkarea.truncate();

      theWorkarea += theStream.get();
      theInput = theStream.peek();

      while (isalpha(theInput))
      {
        theWorkarea += theStream.get();
        theInput = theStream.peek();
      }

      if (theWorkarea == "true")
      {
        theObject.assign("true");
        theObject.setProperty("type","xsd:boolean");
      }
      else if (theWorkarea == "false")
      {
        theObject.assign("false");
        theObject.setProperty("type","xsd:boolean");
      }
      else if (theWorkarea == "null")
      {
        theObject <<= OTC_ROPayload::nullValue();
      }
      else
        return false;

      break;
    }

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '-':
    {
      theWorkarea.truncate();

      theWorkarea += theStream.get();
      theInput = theStream.peek();

      OTC_Boolean theFloat = false;

      while (isdigit(theInput) || (tolower(theInput) == 'e') ||
       theInput == '.' || theInput == '-' || theInput == '+')
      {
        if (!isdigit(theInput))
          theFloat = true;

        theWorkarea += theStream.get();
        theInput = theStream.peek();
      }

      theObject.assign(theWorkarea,theWorkarea.length());

      if (theFloat)
        theObject.setProperty("type","xsd:double");
      else
        theObject.setProperty("type","xsd:int");

      break;
    }

    default:
    {
      return false;
    }
  }

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_JsonRpcServlet::readString_(
 istream& theStream,
 OTC_String& theResult
)
{
  theResult.truncate();

  if (theStream.get() != '"')
    return false;

  int theInput;

  theInput = theStream.peek();

  while (theInput != EOF && theInput != '"')
  {
    if (theInput == '\\')
    {
      theStream.ignore(1);

      theInput = theStream.get();

      if (theInput == EOF)
        return false;

      switch (theInput)
      {
        case 'b':
        {
          theResult += '\b';

          break;
        }

        case 'f':
        {
          theResult += '\f';

          break;
        }

        case 'n':
        {
          theResult += '\n';

          break;
        }

        case 'r':
        {
          theResult += '\r';

          break;
        }

        case 't':
        {
          theResult += '\t';

          break;
        }

        case 'u':
        {
          int d1, d2, d3, d4;

          d1 = theStream.get();
          d2 = theStream.get();
          d3 = theStream.get();
          d4 = theStream.get();

          if (d4 == EOF)
            return false;

          if (!isxdigit(d1))
            return false;
          if (!isxdigit(d2))
            return false;
          if (!isxdigit(d3))
            return false;
          if (!isxdigit(d4))
            return false;

          d1 = HEX_DIGIT(d1);
          d2 = HEX_DIGIT(d2);
          d3 = HEX_DIGIT(d3);
          d4 = HEX_DIGIT(d4);

          d4 += (d3 << 4);
          d4 += (d2 << 8);
          d4 += (d1 << 12);

          if (d4 < 0x80)
          {
            theResult += u_char(d4);
          }
          else if (d4 < 0x800)
          {
            theResult += u_char(0xc0 | (d4 >> 6));
            theResult += u_char(0x80 | (d4 & 0x3f));
          }
          else
          {
            theResult += u_char(0xe0 | (d4 >> 12));
            theResult += u_char(0x80 | ((d4 >> 6) & 0x3f));
            theResult += u_char(0x80 | (d4 & 0x3f));
          }

          break;
        }

        case '"':
        case '/':
        case '\\':
        {
          theResult += theInput;

          break;
        }

        default:
        {
          return false;
        }
      }
    }
    else
      theResult += theStream.get();

    theInput = theStream.peek();
  }

  if (theStream.get() != '"')
    return false;

  return true;
}

/* ------------------------------------------------------------------------- */
void OTC_JsonRpcServlet::encodeString_(
 ostream& theStream,
 char const* theString,
 size_t theLength
)
{
  char const* theStart;
  size_t theActive = 0;

  theStream << '"';

  theStart = theString;

  for (size_t i=0; i<theLength; i++)
  {
    char c = theString[i];

    switch (c)
    {
      case '\b':
      {
	theStream << "\\b";

        break;
      }

      case '\f':
      {
	theStream << "\\f";

        break;
      }

      case '\n':
      {
	theStream << "\\n";

        break;
      }

      case '\r':
      {
	theStream << "\\r";

        break;
      }

      case '\t':
      {
	theStream << "\\t";

        break;
      }

      case '"':
      case '\\':
      {
	theStream << "\\" << c;

        break;
      }

      default:
      {
        if (iscntrl(c))
        {
          theStream << "\\u00";
          theStream << HEX_CHARACTERS[u_char(c) >> 4];
          theStream << HEX_CHARACTERS[u_char(c) & 0xf];
        }
        else
          theStream << c;

        break;
      }
    }
  }

  if (theActive != 0)
    theStream.write(theStart,theActive);

  theStream << '"';
}

/* ------------------------------------------------------------------------- */
void OTC_JsonRpcServlet::cleanup_()
{
  if (binding_ != 0)
  {
    OTC_SVBroker::registry()->unsubscribeServiceAddress(
     id(),binding_->serviceAddress());

    binding_->destroy();

    binding_ = 0;
  }

  if (broker_ != 0)
  {
    delete broker_;

    broker_ = 0;
  }
}

/* ------------------------------------------------------------------------- */
