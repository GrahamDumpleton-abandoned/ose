#ifndef OTC_TCL_TCLQUEUE_HH
#define OTC_TCL_TCLQUEUE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     tcl/tclqueue.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/jobqueue.hh>

#ifndef TCLHEADER
#define TCLHEADER <tcl.h>
#endif

#include TCLHEADER

#if TCL_MAJOR_VERSION >= 8
#include <OTC/collctn/hmap.hh>
#endif

#include <OTC/dispatch/joblist.hh>
#include <OTC/dispatch/ioevent.hh>

#if !defined(SYS_UNIX)
#if defined(HAVE_WINSOCK2)
#include <winsock2.h>
#endif
#include <windows.h>
#endif

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/tcl/tclqueue.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_TclJobQueue : public OTC_JobQueue
    // = TITLE
    //	   Version of job queue which uses TCL to do OSE dispatching.
    //	   
    // = DESCRIPTION
    //     The <OTC_TclJobQueue> class uses hooks within TCL in order to have
    //     TCL implement the dispatch loop. This class only works for version
    //     7.5 or later of TCL.
    //
    //     Note that this class will ignore the flags supplied by TCL about
    //     what event sources should be checked and will always check all
    //     event sources. If doing single event dispatching any flags about
    //     what event sources to check are similarly ignored and will not be
    //     passed onto TCL with TCL always being told to check all event
    //     sources.
    //     
    //     If using this class, you can call <OTC_Dispatcher::run()> or the
    //     TCL main loop. By default both ways will result in <Tcl_Exit()>
    //     being called if the dispatcher is stopped. This means that if you
    //     call <OTC_Dispatcher::run()> and the dispatcher is stopped, it
    //     will not return. Thus, don't rely on any code being executed after
    //     the call to <OTC_Dispatcher::run()>. If you do need to change this
    //     behaviour, you should create a derived version of this class and
    //     redefine the <shutdown()> method.
    //
    //     Note that with <OTC_TclJobQueue> there is no way for a user
    //     supplied setup function to be called before each attempt to find a
    //     job to execute.
{
  public:

			~OTC_TclJobQueue();

    // = INITIALISATION

			OTC_TclJobQueue();
				// Registers this class as an event source
				// with TCL.

    // = QUEUEING

    OTC_Job*		next(int theActions, int theOptions);
				// Calls TCL to dispatch a single event/job.
				// This may be a TCL or OSE event/job.

    // = SHUTDOWN

    virtual void	shutdown();
				// Called when the dispatcher has been
				// stopped. By default this function
				// will call <Tcl_Exit()> resulting in
				// the application exiting immediately.
				// You can override this function in a
				// derived class to do something else.

  private:

			OTC_TclJobQueue(OTC_TclJobQueue const&);
				// Do not define an implementation for this.

    OTC_TclJobQueue&	operator=(OTC_TclJobQueue const&);
				// Do not define an implementation for this.

    static void 	setupCallback_(ClientData theClientData, int theFlags);
				// Setup callback registered with TCL for
				// event source.

    static void 	checkCallback_(ClientData theClientData, int theFlags);
				// Check callback registered with TCL for
				// event source.

    static void 	idleCallback_(ClientData theClientData);
				// Idle callback registered with TCL.

    static int		signalCallback_(Tcl_Event*, int);
				// Callback for when it is detected that
				// a signal has occurred.

    static int		alarmCallback_(Tcl_Event*, int);
				// Callback for when it is detected that
				// an alarm has occurred.

    static int		timeoutCallback_(Tcl_Event*, int);
				// Callback for when it is detected that
				// a timeout has occurred.

    static int		priorityJobCallback_(Tcl_Event*, int);
				// Callback for when it is detected that
				// a priority job is available.

    static int		standardJobCallback_(Tcl_Event*, int);
				// Callback for when it is detected that
				// a standard job is available.

    static int		idleJobCallback_(Tcl_Event*, int);
				// Callback for when it is detected that
				// an idle job is available.

    static int		dummyCallback_(Tcl_Event*, int);
				// Dummy callback to make it look like
				// something was done.

    static int		fileCallbackFEP_(Tcl_Event* theEvent, int);
				// File event procedure. This is used
				// for TCL prior to 8.0 and forwards call
				// to <fileCallback_()>.

#if !defined(SYS_UNIX)

    static LRESULT CALLBACK	fileCallbackWEP_(
    				 HWND hwnd,
				 UINT message,
				 WPARAM wParam,
				 LPARAM lParam
				);
				// Windows event procedure. This is used
				// on Windows for TCL 8.0 or later and
				// forwards call to <fileCallback_()>.

#endif

    static void		fileCallback_(ClientData theClientData, int theMask);
				// File callback registered with TCL.

    static OTC_TclJobQueue* instance_;
				// There should only ever be one instance
				// of this class, and this is it.

    OTC_Boolean		idleRegistered_;
				// Has value <OTCLIB_TRUE> when idle callback
				// registered. Needed as idle callback is
				// automatically removed after being called.
#if TCL_MAJOR_VERSION >= 8
    static OTC_HMap<int,int>	fileSubscriptions_;
				// Record of what events on what files a
				// file handler has been created.

    static OTC_Boolean	signalFile_;
				// Flag remembering if file handler has
				// been created for signal file descriptor.

    static OTC_Boolean	wakeupFile_;
				// Flag remembering if file handler has
				// been created for wakeup file descriptor.

#if !defined(SYS_UNIX)

    static HWND		winHandle_;
				// Handle to Win32 window for event
				// callbacks.

    static WNDCLASS	winClass_;
    				// Class of window for Win32 callback
				// events.

#endif
#endif

    static OTC_IOSubscription*	pendingFdEvents_;
    				// Set of subscriptions to I/O events.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TCL_TCLQUEUE_HH */
