/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/_iserver.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/inetlstn.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/action.hh>
#include <OTC/dispatch/fnagent.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/program/program.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/logger.hh>

#include <signal.h>

/* ------------------------------------------------------------------------- */

OTC_InetListener* theListener;

void service_callback(OTC_Event* theEvent, int)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCEV_Envelope* theEnvelope;
    theEnvelope = (OTCEV_Envelope*)theEvent;

    if (theEnvelope->subject() == "BLIP")
    {
      OTCEV_Message* theMessage;
      theMessage = theEnvelope->message();

      cout << theEnvelope->conversationId();
      cout << " ";
      cout << theMessage->content();
      cout << endl;
    }
  }

  theEvent->destroy();
}

void usage()
{
  cerr << "Usage: " << OTC_Program::basename();
  cerr << " -listen port" << endl;
}

int main(int argc, char* argv[])
{
  set_terminate(otclib_terminate_function);

  OTC_Program::initialise(argc,argv);

  OTC_String thePortString;

  while (OTC_Program::numOptions() != 0)
  {
    if (OTC_Program::option(1) == "-listen")
    {
      OTC_Program::shift(1);
      if (OTC_Program::numOptions() == 0)
        break;

      thePortString = OTC_Program::option(1);
    }

    OTC_Program::shift(1);
  }
  
  if (thePortString.isEmpty())
  {
    usage();
    return 1;
  }

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  int thePort;

  if (sscanf(thePortString,"%d",&thePort) != 1)
  {
    cerr << OTC_Program::basename() << ": invalid port number" << endl;
    return 1;
  }

  theListener = new OTC_InetListener("MESSAGE",thePort);
  theListener->reference();

  theListener->start();

  if (theListener->fd() == -1)
  {
    cerr << OTC_Program::basename() << ": listener failed" << endl;
    return 1;
  }

  OTC_FNAgent theService(service_callback);
  OTC_Receiver theServiceInBox(&theService,"HEARTBEAT");

  OTC_Dispatcher::run();

  theListener->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
