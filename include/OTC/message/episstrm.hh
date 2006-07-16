#ifndef OTC_MESSAGE_EPISSTRM_HH
#define OTC_MESSAGE_EPISSTRM_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     message/episstrm.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/episockt.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/episstrm.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_LinkList;

enum OTC_EPISStreamState
{
  OTCLIB_EPISSTREAM_INACTIVE,
  OTCLIB_EPISSTREAM_STARTING,
  OTCLIB_EPISSTREAM_CONNECTED
};

class OTC_EPISStream : public OTC_EPISocket
    // = TITLE
    //     Handles transmission of streamed data between processes.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_EPISStream> class handles the transmission of streamed
    //     data between processes over a socket connection. The class is used
    //     in pairs, one instance at either end of the socket connection.
    //     
    //     The class will send over the socket connection the content
    //     associated with any message events it is passed for which the
    //     sequence number is <0>, or for which the head of the path in the
    //     destination address matches the address created to represent the
    //     agent at the opposite end of the socket connection. Ie., matches
    //     the remote address.
    //
    //     If multiple messages get queued up due to the file descriptor
    //     being blocked, when the file descriptor once becomes available
    //     again, they will not all be written out in one go. Instead,
    //     a maximum number will be written out and the control returned
    //     to the dispatcher. An event will be scheduled so that control
    //     will come back to this class to deliver the remainder. This is
    //     done so as to allow other agents to run when there becomes a
    //     backlog of messages. By default the maximum delivered in one
    //     go is <32>. This can be overridden using the environment
    //     variable <OTCLIB_STREAMBATCH>.
    //
    // = NOTES
    //     If the file descriptor you provide is set for non blocking I/O,
    //     you MUST use the POSIX method for setting it to be non blocking.
    //     That is, you MUST use O_NONBLOCK. The behaviour of this class is
    //     undefined when a non blocking file descriptor is used and either
    //     the 4.2 BSD or SYS5 methods were used to define the file
    //     descriptor as being non blocking.
    //
    // = SEE ALSO
    //     <OTC_Event>, <OTCEV_Envelope>, <OTCEV_Message>, <OTC_EPISocket>
{
  public:

			~OTC_EPISStream();

    // = CONSTRUCTION

			OTC_EPISStream(
			 OTC_EndPoint* theEndPoint,
			 u_int theSize=0
			);
				// Initialises this end of the link. Note
				// that it is still necessary to provide a
				// file descriptor to be used for the
				// connection via the <attach()> member
				// function. The <attach()> member function
				// may be called from a derived class
				// constructor. Maximum amount of data
				// which will be read in one go from a
				// socket will be <theSize>. If this is <0>,
				// the default of <4096> will be used. The
				// default value of <4096> may be overridden
				// by setting the environment variable called
				// <OTCLIB_STREAMINPUT>.

    // = QUERY

    int			fd() const
				{ return fd_; }
				// Returns the file descriptor on which
				// messages are being sent/received. Will
				// return <-1> if there is connection.
				// Note that return a value not equal
				// to <-1> does not indicate an active
				// connection as an active connection
				// could still be getting negotiated.

    // = CONNECTION

    void		attach(int theFd);
				// Defines that <theFd> should be used
				// as the socket connection. Will raise
				// an exception if there is already an
				// active connection. This function will
				// generate an event such that the class
				// will be called back so as to be able to
				// deliver the initial address control
				// messages and any queued messages waiting
				// for delivery.

    int			detach();
				// Detaches this object from the file
				// descriptor in use for the current active
				// connection. Raises an exception if this
				// object wasn't currently attached to any
				// file descriptor. Use <fd()> to check that
				// file descriptor is not <-1> before calling
				// this function. Note that this function
				// doesn't call <close()> on the file
				// descriptor. The caller should close
				// the file descriptor either just before or
				// after calling this function. Returns the
				// file descriptor which was being used.

    void                suspendInput();
                                // Will suspend handling of input.

    void                resumeInput();
                                // Will resume handling of input.

    // = EVENT CALLBACK

    void		handle(OTC_Event* theEvent);
				// Sends messages from this process over
				// the socket connection and reads messages
				// coming from the socket connection and
				// distributes them locally if possible.
				// If a failure of the connection occurs, the
				// <shutdown()> function will be called.
				// Prior to calling <shutdown()> this function
				// will unsubscribe from the file descriptor,
				// set the held file descriptor to <-1>,
				// remove the association between the remote
				// address and this class, and put this class
				// into a sleep state. If the <shutdown()>
				// function returns a value greater than or
				// equal to <0>, the value will be
				// interpreted as being a new file descriptor
				// to use for the connection and this class
				// will be woken up again. If the returned
				// value is <-1>, this function will assume
				// that the file descriptor has at the least
				// been closed by calling the system
				// <close()> function. In the event of <-1>
				// being returned this function will not do
				// anything else in case the derived class
				// <shutdown()> function deleted the instance
				// of the class.

  private:

			OTC_EPISStream(OTC_EPISStream const&);
				// Do not define an implementation for this.

    OTC_EPISStream&	operator=(OTC_EPISStream const&);
				// Do not define an implementation for this.

    void		messageQueued_();
				// Called when a message is queued in order
				// to do processing for congestion management.

    void		messageSent_();
				// Called when a message is sent in order
				// to do processing for congestion management.

    static OTC_NRMutex  mutex_;
                                // Lock for threads.

    static u_int        gBatch_;
                                // Maximum number of messages to write
                                // out in one go without going back to
                                // the dispatcher.

    static u_int	gSize_;
    				// Default stream read size.

    static u_int	gThreshold_;
    				// Threshold count used in determining
				// if a backlog is arising.

    OTC_EPISStreamState	state_;
                                // The state of the tcp stream.

    int			fd_;
				// File descriptor for connection.

    OTC_Boolean		nonblocking_;
				// Set to <OTCLIB_TRUE> if file
				// descriptor is set for non blocking.

    OTC_String		remote_;
				// Remote address.

    OTC_LinkList*	outgoing_;
				// List of outgoing events waiting to
				// be delivered.

    u_int		outgoingCount_;
				// Count of how many outgoing messages
				// there currently are.

    u_int		backlogCount_;
				// Once a backlog has been detected, this
				// tracks how many messages sent and received
				// while backlogged.

    OTC_Boolean		waitingToWrite_;
				// Set to <OTCLIB_TRUE> if we are
				// waiting for notification that we
				// can write something out on the file
				// descriptor. Only used when the
				// file descriptor is non blocking.

    u_int		ilength_;
				// Length of input buffer.

    OTC_String		imessage_;
				// This is where the message is read in.

    int			olength_;
				// Count of how much has yet to be read
				// out of a message when dealing with
				// non blocking file descriptors.

    OTC_String		omessage_;
				// This is where the message to be
				// written out is held.

    OTC_Boolean		suspended_;
				// <OTCLIB_TRUE> if message input is
				// suspended.

    OTC_Boolean         stopping_;
                                // <OTCLIB_TRUE> if connection is to be
                                // stopped once the output queue is
                                // empty.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_EPISSTRM_HH */
