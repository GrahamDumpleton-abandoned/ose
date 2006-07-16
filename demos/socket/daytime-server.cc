/*
// ============================================================================
//
// = FILENAME
//     daytime-server.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/inetlstn.hh>
#include <OTC/message/connect.hh>
#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/fnagent.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/types/time.hh>

#include <signal.h>

#if !defined(SYS_UNIX)
#include <windows.h>
#include <winsock.h>
#endif

/* ------------------------------------------------------------------------- */

void event_callback(OTC_Event* theEvent, int theAgentId)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Connection::typeId())
  {
    OTCEV_Connection* theConnection;
    theConnection = (OTCEV_Connection*)theEvent;

    // Received notification about state of
    // connection.

    if (theConnection->state() == OTCLIB_CONNECTION_ACTIVE)
    {
      // Send date/time to remote end of the
      // connection.

      OTC_String theContent;
      theContent += OTC_Time::currentTime().asString();
      theContent += EOL;

      OTCEV_Message* theMessage;

      theMessage = new OTCEV_Message(theContent);
      theMessage->queue(theConnection->remoteAddress());

      // Close down connection.

      OTC_EndPoint* theEndPoint;
      theEndPoint = OTC_EndPoint::remote(theConnection->remoteAddress());

      if (theEndPoint != 0)
      {
	// Supply -1 as argument to stop() so that
	// connection is only closed once any queued
	// messages are sent.

	theEndPoint->stop(-1);
      }
    }
  }

  theEvent->destroy();
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    cerr << "Usage: daytime-server port" << endl;
    return 1;
  }

  // Initialise dispatcher.

  OTC_Dispatcher::initialise();

  // Shutdown gracefully if we get CTRL-C sent
  // to the process.

  OTCEV_Shutdown::monitor(SIGINT);

  // Create callback agent.

  OTC_FNAgent theAgent(event_callback);

  // Create listener. Request "STREAM" as
  // opposed to "MESSAGE" as we want to be able
  // to read/write raw data.

  OTC_InetListener theListener("STREAM",atoi(argv[1]));

  // Add callback as observer of the listener
  // so we know about new connections.

  theListener.addObserver(theAgent.id());

  // Start the listener.

  theListener.start();

  // Start the dispatcher.

  OTC_Dispatcher::run();

  return 0;
}

/* ------------------------------------------------------------------------- */
