#ifndef OTC_INTERNET_JSONSRVL_HH
#define OTC_INTERNET_JSONSRVL_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/jsonsrvl.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2006 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/internet/httpsrvl.hh>
#include <OTC/dispatch/svbroker.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/internet/jsonsrvl.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_JsonRpcServlet
 : public OTC_HttpServlet, public OTC_EVAgent
    // = TITLE
    //      Servlet translating JSON-RPC requests into service requests.
    //
    // = CLASS TYPE
    //      Concrete
    //
    // = DESCRIPTION
    //     The <OTC_JsonRpcServlet> class is a HTTP servlet which acts
    //     as a gateway, translating JSON-RPC requests into service
    //     requests which are then delivered to a service. Any response
    //     form the service is subsequently translated back into the
    //     appropriate response for the JSON-RPC and returned to the
    //     HTTP client.
{
  public:

    			~OTC_JsonRpcServlet();

    // = INITIALISATION

    			OTC_JsonRpcServlet(
			 OTC_HttpSession* theSession,
			 OTC_ServiceBinding* theBinding=0
			);
				// Creates a HTTP servlet to handle an
				// RPC request. <theSession> is the HTTP
				// session object which has parsed the HTTP
				// request. This must be supplied by the
				// factory method contained in the HTTP
				// server object. <theBinding> is the service
				// binding of the service to which the
				// RPC request should be passed. If
				// <theBinding> is <0>, a HTTP response will
				// instead be sent back indicating the
				// target URL was invalid.

  protected:

    // = REQUEST PROCESSING

    void		processRequest();
    				// Decodes the request and either flags an
				// error.

    void		processContent(OTC_String const& theContent);
    				// Accumulates the content of the request
				// and when complete forwards request
				// onto the appropriate service.

    // = TARGET SERVICE

    virtual OTC_ServiceBinding*	service(char const* theName);
				// This function is be called by the method
				// <processRequest()> with <theName> equal to
				// the servlet path supplied to the
				// constructor. This method should analyse
				// <theName> to determine what service the
				// request is being targeted at. By default
				// the method simply looks up the service
				// registry and uses the first service having
				// <theName> that it finds. You can overload
				// this method in a derived class to use
				// some other means to identify which service
				// to use. This would be appropriate given
				// that not all services may be able to
				// respond to requests conforming to what
				// can be represented in an RPC. The instance
				// of <OTC_ServiceBinding> returned by a
				// derived class must have no owner, or if it
				// has, the derived method must call <clone()>
				// on it. This is because <processRequest()>
				// will call <destroy()> on the returned
				// object when it has finished with it.

    // = EVENT HANDLING

    void		process(OTC_Event* theEvent);
    				// Deals with the response received from a
				// service, translating it back into the
				// appropriate RPC format and returning it.

    void		handle(OTC_Event* theEvent);
    				// Calls <process()> then destroys <theEvent>.

  private:

    static bool		decodeRequest_(
			 istream& theStream,
			 OTC_String& theRequestId,
			 OTC_String& theMethod,
			 OTC_RWPayload& theObject,
			 OTC_String& theWorkarea
			);
    				// Decodes the actual request converting
				// int into a service request. <theWorkarea>
				// should be a preallocated working buffer
				// estimated to be large enough to hold the
				// content of any element. If it isn't large
				// enough it will be increased in size as
				// necessary but by having it large enough to
				// start with can avoid reallocations and
				// memory copying.

    static void		encodeFailure_(
			 ostream& theStream,
                         OTC_String const& theRequestId,
			 int theError,
			 char const* theDescription="",
			 char const* theOrigin="",
			 char const* theDetails=""
			);
    				// Encodes an JSON-RPC failure response.

    void		sendFailure_(
			 int theError,
			 char const* theDescription="",
			 char const* theOrigin="",
			 char const* theDetails=""
			);
    				// Sends back an JSON-RPC failure response.

    static bool		encodeResponse_(
			 ostream& theStream,
                         OTC_String const& theRequestId,
			 OTC_ROPayload const& theObject
			);
    				// Encodes an JSON-RPC response.

    static bool		encodeObject_(
			 ostream& theStream,
			 OTC_ROPayload const& theObject
			);
    				// Converts a service response into an
				// JSON value.

  private:

    static bool		decodeValue_(
			 istream& theStream,
			 OTC_RWPayload theObject,
			 OTC_String& theWorkarea
			);
				// Decodes an JSON value. <theWorkarea>
				// should be a preallocated working buffer
				// estimated to be large enough to hold the
				// content of any element. If it isn't large
				// enough it will be increased in size as
				// necessary but by having it large enough to
				// start with can avoid reallocations and
				// memory copying.

    static bool		readString_(
			 istream& theStream,
			 OTC_String& theResult
			);
				// Reads characters between double quotes
                                // from <theStream> undoing any JSON escapes.
				// The result is assigned to <theResult> with
                                // the function returning <true> if the
				// conversion was successful.

    static void		encodeString_(
			 ostream& theStream,
			 char const* theString,
			 size_t theLength
			);
				// Outputs the first <theLength> characters
				// of <theString> onto <theStream>. The
				// characters are escaped as per JSON
				// requirements for characters and enclosed
                                // in double quotes.

    static void		encodeString_(
			 ostream& theStream,
			 char const* theString
			)
      				{
				  encodeString_(theStream,theString,
				   theString?strlen(theString):0);
				}
				// Outputs the <theString> onto <theStream>.
                                // The characters are escaped as per JSON
				// requirements for characters and enclosed
                                // in double quotes.

  private:

    			OTC_JsonRpcServlet(OTC_JsonRpcServlet const&);
				// Do not define an implementation for this.

    OTC_JsonRpcServlet&	operator=(OTC_JsonRpcServlet const&);
				// Do not define an implementation for this.

    void		cleanup_();
				// Removes any subscriptions on service
				// registry and destroys the service broker.

    OTC_ServiceBinding*	binding_;
    				// Service binding of target service.

    OTC_SVBroker*	broker_;
    				// Broker through which requests are sent
				// and subsequently received.

    OTC_String		content_;
    				// Accumulated content of the request.

    OTC_String          requestId_;
                                // Inbound JSON request ID.

    OTC_String		conversationId_;
    				// Conversation ID of outbound request.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_INTERNET_JSONSRVL_HH */
