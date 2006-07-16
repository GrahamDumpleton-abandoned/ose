/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/otcxmlrpcgw.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/internet/xmlrpcgw.hh"
#endif
#endif

#include <OTC/internet/xmlrpcgw.hh>
#include <OTC/internet/xmlrpcsl.hh>

#include <OTC/dispatch/svrgstry.hh>
#include <OTC/dispatch/grpannc.hh>
#include <OTC/dispatch/request.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_XmlRpcGateway");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_XmlRpcGateway::~OTC_XmlRpcGateway()
{
  OTC_SVBroker::registry()->unsubscribeAgent(id());
}

/* ------------------------------------------------------------------------- */
OTC_XmlRpcGateway::OTC_XmlRpcGateway(char const* theServiceGroup)
  : broker_(this,0,0), serviceGroup_(theServiceGroup)
{
  OTC_SVBroker::registry()->subscribeServiceGroup(id(),theServiceGroup);
}

/* ------------------------------------------------------------------------- */
OTC_HttpServlet* OTC_XmlRpcGateway::servlet(OTC_HttpSession* theSession)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_XmlRpcGateway::servlet()");

  OTC_String theServiceName;
  theServiceName = theSession->servletPath();

  OTC_HttpServlet* theServlet = 0;

#if 0
  if (theServiceName.isEmpty())
  {
    theServlet = new OTC_XmlRpcServlet(theSession,broker_.serviceBinding());
    OTCLIB_ASSERT_M(theServlet != 0);
  }
  else if (visible_.contains(theServiceName))
#endif

  OTCLIB_TRACER(MODULE) << "service = " << theServiceName << endl;

  if (visible_.contains(theServiceName))
  {
    OTC_ServiceBinding* theBinding;
    theBinding = visible_.item(theServiceName);

    OTCLIB_TRACER(MODULE) << "found" << endl;

    theServlet = new OTC_XmlRpcServlet(theSession,theBinding);
    OTCLIB_ASSERT_M(theServlet != 0);
  }
  else
  {
    OTCLIB_TRACER(MODULE) << "not found" << endl;

    theServlet = new OTC_XmlRpcServlet(theSession,0);
    OTCLIB_ASSERT_M(theServlet != 0);
  }

  return theServlet;
}

/* ------------------------------------------------------------------------- */
void OTC_XmlRpcGateway::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_GroupAnnouncement::typeId())
  {
    OTCEV_GroupAnnouncement* theAnnouncement;
    theAnnouncement = (OTCEV_GroupAnnouncement*)theEvent;

    if (theAnnouncement->serviceGroup() == serviceGroup_)
    {
      OTC_ServiceBinding* theBinding;
      theBinding = theAnnouncement->serviceBinding();

      if (theAnnouncement->status() == OTCLIB_SERVICE_AVAILABLE)
      {
	if (!available_.contains(theBinding->serviceName(),theBinding))
	{
	  // Add to available services.

	  available_.add(theBinding->serviceName(),theBinding);

	  // If no service visible with this service
	  // name, make this instance of the service
	  // visible.

	  if (!visible_.contains(theBinding->serviceName()))
	    visible_.add(theBinding->serviceName(),theBinding);
	}
      }
      else
      {
	if (available_.contains(theBinding->serviceName(),theBinding))
	{
	  // Remove from available services.

	  available_.remove(theBinding->serviceName(),theBinding);

	  // If the currently visible service with
	  // this name is the withdrawn service,
	  // remove it from visibility and see if
	  // there is another available service with
	  // the same service name which can be made
	  // visible instead.

	  OTC_ServiceBinding* tmpBinding;
	  tmpBinding = visible_.item(theBinding->serviceName());

	  if (tmpBinding->serviceAddress() == theBinding->serviceAddress())
	  {
	    visible_.remove(tmpBinding->serviceName());

	    OTC_Iterator<OTC_ServiceBinding*> theBindings;
	    theBindings = available_.items(theBinding->serviceName());

	    if (theBindings.isValid())
	      visible_.add(theBinding->serviceName(),theBindings.item());
	  }
        }
      }
    }
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
