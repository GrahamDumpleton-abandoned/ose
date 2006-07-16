#ifndef OTC_INTERNET_XMLRPCGW_HH
#define OTC_INTERNET_XMLRPCGW_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/xmlrpcgw.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/internet/httpsrvr.hh>
#include <OTC/dispatch/svbroker.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/collctn/hmap.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/internet/xmlrpcgw.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_XmlRpcGateway : public OTC_HttpServer, public OTC_EVAgent
    // = TITLE
    //     Factory for creation of HTTP servlets to handle RPC requests.
    //
    // = DESCRIPTION
    //     The <OTC_XmlRpcGateway> class is a HTTP server object factory for
    //     creating HTTP servlets to handle RPC requests. An instance
    //     of this class monitors a specified service group and if it receives
    //     an RPC request where the servlet path corresponds to the name
    //     of the service, it will forward the RPC request to that service
    //     and subsequently send back any response. At this time only an
    //     XML-RPC style request is implemented.
    //     
    // = SEE ALSO
    //     <OTC_HttpDaemon>, <OTC_RpcServlet>, <OTC_HttpServer>
{
  public:

    virtual		~OTC_XmlRpcGateway();

    // = INITIALISATION

			OTC_XmlRpcGateway(char const* theServiceGroup);
				// Creates a HTTP server object which
				// handles passing RPC requests to
				// services in <theServiceGroup>.

    // = SERVICE GROUP

    OTC_String const&	serviceGroup() const
      				{ return serviceGroup_; }
    				// Returns the service group in which a
				// target service must reside.

  protected:

    // = SERVLET CREATION

    OTC_HttpServlet*	servlet(OTC_HttpSession* theSession);
				// Returns a HTTP servlet to process the
				// current request being handled by
				// <theSession> assuming it is an RPC
				// request.

    // = EVENT HANDLING

    void		handle(OTC_Event* theEvent);
    				// Handles <theEvent>, responding to
				// announcements about servics joining
				// and leaving service groups.

  private:

			OTC_XmlRpcGateway(OTC_XmlRpcGateway const&);
				// Do not define an implementation for this.

    OTC_XmlRpcGateway&	operator=(OTC_XmlRpcGateway const&);
				// Do not define an implementation for this.

    OTC_SVBroker	broker_;
    				// The service broker.

    OTC_String		serviceGroup_;
    				// Service group to monitor.

    OTC_HMap<OTC_String,OTC_ServiceBinding*>	visible_;
    				// Currently visible services.

    OTC_TManyMap<OTC_String,OTC_ServiceBinding*>	available_;
    				// Services available.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_INTERNET_XMLRPCGW_HH */
