#ifndef OTC_DISPATCH_SHUTDOWN_HH
#define OTC_DISPATCH_SHUTDOWN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/shutdown.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/event.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/shutdown.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_ShutdownNotification
{
  OTCLIB_SHUTDOWN_PENDING=0,
  OTCLIB_SHUTDOWN_ARRIVED=1
};

class OTC_FNAgent;
class OTC_ShutdownSubscription;

class OSE_EXPORT OTCEV_Shutdown : public OTC_Event
    // = TITLE
    //     Event object to notify of program shutdown.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTCEV_Shutdown> is a derived version of <OTC_Event> specifically
    //     for notifying agents about program shutdown. The class also
    //     provides the interface for registration of interest in program
    //     shutdown by agents. Provision is provided for warning of a
    //     pending shutdown and that shutdown time has arrived. These
    //     two possibilities are identified by <OTCLIB_SHUTDOWN_PENDING>
    //     and <OTCLIB_SHUTDOWN_ARRIVED>.
    //     
    // = SEE ALSO
    //     <OTC_Event>, <OTC_EVAgent>
{
  public:

			~OTCEV_Shutdown();

    // = CONSTRUCTION

			OTCEV_Shutdown(OTC_ShutdownNotification theCategory)
			  : category_(theCategory)
				{}
				// Creates an event object notifying of a
				// pending shutdown of the application.
				// <theCategory> gives the type of shutdown
				// notifcation. This should be either
				// <OTCLIB_SHUTDOWN_PENDING> to indicate that
				// a shutdown is pending and
				// <OTCLIB_SHUTDOWN_ARRIVED> when the
				// shutdown is imminent.

    // = QUERY

    OTC_ShutdownNotification	category() const
				{ return category_; }
				// Returns the type of shutdown notification.

    // = IDENTIFICATION

    void*		type() const;
				// Returns a value that uniquely identifies
				// this type of event.

    static void*	typeId()
				{ return &gTypeId_; }
				// Returns a value that uniquely identifies
				// this type of event.

    // = DEBUGGING

    void		dump(ostream& outs) const;
				// Dumps info about the event onto <outs>.

    // = SUBSCRIPTION

    static void		subscribe(int theAgentId);
				// Registers that the agent identified by ID
				// <theAgentId> is interested in being
				// notified when a shutdown is to occur.
				// An agent will not be subscribed twice.

    static void		unsubscribe(int theAgentId);
				// Cancels any interest in the shutdown
				// notification by the agent identified by ID
				// <theAgentId>.

    // = SIGNALS

    static void		monitor(int theSignal);
				// Causes occurences of <theSignal> to be
				// monitored for and if it occurs, the
				// shutdown sequence will be initiated by
				// calling <schedule()>. If a signal occurs
				// while the shutdown sequence is in
				// progress, <exit()> will be called.
				// <exit()> is called rather than stopping
				// the dispatcher as simply stopping the
				// dispatcher may cause objects to be deleted
				// on program exit which still have threads
				// running in them. Once you indicate
				// <theSignal> should be monitored, you can't
				// turn off the monitoring, thus only use
				// this function if you want the affects to
				// be permanent.

    // = SCHEDULING

    static void		schedule(long theDelay=0);
				// Schedules the shutdown procedure. The
				// actions carried out are to first notify
				// of pending shutdown by calling <notify()>
				// with <OTCLIB_SHUTDOWN_PENDING>. A timeout
				// is then scheduled for <theDelay>
				// milliseconds. When this occurs, <notify()>
				// is called with <OTCLIB_SHUTDOWN_ARRIVED>.
				// A priority job is then scheduled which
				// when run will stop the dispatcher. If this
				// function is called when a shutdown is
				// already in progress, the second request
				// will be ignored. Messages are output via
				// the logger to indicate each phase of the
				// shutdown. The default value of <theDelay>
				// is <0>. This value indicates that an
				// inbuilt default of <1000> milliseconds.
				// The inbuilt default can be overridden
				// by the environment variable
				// <OTCLIB_SHUTDOWNDELAY>.

    static void		suspend();
				// Causes any shutdown to be delayed. The
				// function increments by one a count
				// indicating parties wanting shutdown to be
				// delayed. Delaying of shutdown is useful to
				// allow agents within the event system to
				// perform some required action first. This
				// function might be called in response to
				// a pending shutdown notification, but may
				// be called before a shutdown has even been
				// initiated. If notifications have already
				// been sent out for final shutdown, the
				// call to this function will not stop the
				// shutdown.

    static void		resume();
				// Decrements by one the number of parties
				// wanting shutdown to be delayed. If this
				// reaches <0> and a shutdown was supposed
				// to have already have occurred, it will
				// happen immediately. If the shutdown was
				// pending, it will happen at scheduled time.
				// If no shutdown has yet been scheduled,
				// nothing at all will happen.

    static OTC_Boolean	isScheduled();
				// Returns <OTCLIB_TRUE> if a shutdown is
				// scheduled.

    static OTC_Boolean	isSuspended();
				// Returns <OTCLIB_TRUE> if shutdown is
				// suspended.

    // = NOTIFICATION

    static void		notify(OTC_ShutdownNotification theCategory);
				// Schedules jobs notifying agents of pending
				// shutdown of the application. Note that the
				// jobs are scheduled with the dispatcher as
				// priority jobs. Because the dispatcher is
				// used to deliver the jobs, you can't have
				// stopped the dispatcher, else the jobs will
				// not be delivered. <theCategory> gives the
				// type of shutdown notifcation. This should
				// be either <OTCLIB_SHUTDOWN_PENDING> to
				// indicate that a shutdown is pending and
				// <OTCLIB_SHUTDOWN_ARRIVED> when the
				// shutdown is imminent.

    static void		notifyImmediately(OTC_ShutdownNotification theCategory);
				// Executes immediately, jobs notifying
				// agents of pending shutdown of the
				// application. <theCategory> gives the type
				// of shutdown notification. This should be
				// either <OTCLIB_SHUTDOWN_PENDING> to
				// indicate that a shutdown is pending and
				// <OTCLIB_SHUTDOWN_ARRIVED> when the
				// shutdown is imminent.

    // = DELIVERY

    static void		deliverImmediately(
			 int theAgentId,
			 OTC_ShutdownNotification theCategory
			);
    				// Immediately creates a shutdown event and
				// delivers it to the agent identified by
				// ID of <theAgentId>. This method would
				// normally only be used if you wanted to
				// forward on a shutdown notification to
				// an agent for which you are proxy and
				// which doesn't itself subscrie to any
				// shutdown events. <theCategory> gives the
				// type of shutdown notification. This should
				// be either <OTCLIB_SHUTDOWN_PENDING> to
				// indicate that a shutdown is pending and
				// <OTCLIB_SHUTDOWN_ARRIVED> when the
				// shutdown is imminent.

  protected:

    // = NON DELIVERY

    void		cancelSource(int theAgentId);
                                // Unsubscribes agent identified by ID
				// of <theAgentId>.

  private:

			OTCEV_Shutdown(OTCEV_Shutdown const&);
				// Do not define an implementation for this.

    OTCEV_Shutdown&	operator=(OTCEV_Shutdown const&);
				// Do not define an implementation for this.

    static void		fill_(
			 OTC_JobList* theJobList,
			 OTC_ShutdownNotification theCategory
			);
				// Fills <theJobList> with jobs to deliver
				// a shutdown event to current subscribed
				// agents.

    static void		signalCallback_(OTC_Event* theEvent, int theAgentId);
				// Callback function for events when
				// signal occurs.

    static void		shutdownCallback_(OTC_Event* theEvent, int theAgentId);
				// Callback function for events when
				// shutdown is being initiated.

    static OTC_NRMutex	mutex_;
				// Lock for threads.

    static OTC_NRMutex	amutex_;
				// Mutex to see who gets to create agents for
				// monitoring signals and shutdown. A
				// separate mutex is needed else we get dead
				// lock because agents created try to lock
				// above mutex.

    static OTC_ShutdownSubscription*	gSubscriptions_;
				// List of agents interested in this event.

    static int		gTypeId_;
				// Anchor for use as a type identifier.

    static OTC_FNAgent*	gSignalAgent_;
				// Pointer to signal agent when monitoring
				// for signals.

    static OTC_FNAgent*	gShutdownAgent_;
				// Pointer to callback agent when shutdown
				// in progress.

    static int		gDelayCount_;
				// Number of parties wanting shutdown to
				// be delayed.

    static OTC_Boolean	gShutdownArrived_;
				// Has value <OTCLIB_TRUE> if shutdown should
				// have occured. If parties had requested
				// shutdown be delayed, the final shutdown
				// may not yet have actually occured.

    OTC_ShutdownNotification	category_;
				// Type of shutdown.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_SHUTDOWN_HH */
