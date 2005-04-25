#ifndef OTC_MESSAGE_INETSRVR_HH
#define OTC_MESSAGE_INETSRVR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/inetsrvr.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/episockt.hh>
#include <OTC/message/epserver.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/inetsrvr.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_InetListener;

class OSE_EXPORT OTC_InetServer : public OTC_EPServer
    // = TITLE
    //	   Server end of internet style socket connection.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   The <OTC_InetServer> class implements the server end of an
    //	   internet style socket connection. In the event of the connection
    //	   failing, this class will close the file descriptor and delete
    //	   itself.
{
  public:

			~OTC_InetServer();

    // = CONSTRUCTION
    //	   Note that the constructor doesn't attach the interface object to
    //	   the file descriptor, you must call <start()> to do this.

			OTC_InetServer(
			 char const* theInterface,
			 OTC_EPListener* theListener,
			 int theFd
			);
				// <theFd> is the file descriptor to be used
				// for the message connection. <theListener>
				// is the listener which spawned this server.
				// This server will be added to the list of
				// servers created by <theListener>.
				// The interface object which controls how
				// data is written over the socket is
				// specified by <theInterface>. The two
				// inbuilt types are "MESSAGE" and "STREAM".
				// The "MESSAGE" type implements a structured
				// messaging abstraction including message
				// routing etc. The "STREAM" type provides
				// the ability for direct control over
				// what is sent over the socket. That is,
				// all that will be sent is the contents
				// of your message.

    // = ADDRESSING

    OTC_String		localTransport() const;
				// Returns the local address corresponding to
				// the transport layer address.

    OTC_String		remoteTransport() const;
				// Returns the remote address corresponding
				// to the transport layer address.

    // = CONNECTION

    void		start();
				// Causes the interface handler to be
				// attached to the file descriptor.

    void		suspendInput();
				// Will suspend handling of input.

    void		resumeInput();
				// Will resume handling of input.


    // = SOCKET

    int			fd() const
				{ return fd_; }
				// Returns the file descriptor for the
				// socket used for the connection.

  protected:

    // = SHUTDOWN

    void		close();
				// Calls <shutdown()> too effect closure
				// of current open connection.

    void*		shutdown(void* theHandle, int theError);
				// Overrides the <shutdown()> method for the
				// <OTC_EndPoint> class. When called the file
				// descriptor pointed to by <theHandle> will
				// be closed and this class will cause itself
				// to be deleted if no longer in use.

  private:

			OTC_InetServer(OTC_InetServer const&);
				// Do not provide an implementation for this.

    OTC_InetServer&	operator=(OTC_InetServer const&);
				// Do not provide an implementation for this.

    OTC_EPISocket*	handler_;
				// Protocol handler for sockets.

    int			fd_;
				// The file descriptor.

    OTC_String		localTransport_;
				// Local socket name.

    OTC_String		remoteTransport_;
				// Remote socket name.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_INETSRVR_HH */
