#ifndef OTC_QT_QTQUEUE_HH
#define OTC_QT_QTQUEUE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     qt/qtqueue.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/jobqueue.hh>

#include <OTC/collctn/hmap.hh>
#include <OTC/dispatch/joblist.hh>

#include <qapplication.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/qt/qtqueue.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_QtJobQueue : public QObject, public OTC_JobQueue
    // = TITLE
    //	   Version of job queue which uses Qt to do OSE dispatching.
    //	   
    // = DESCRIPTION
    //     The <OTC_QtJobQueue> class uses hooks within Qt in order to have
    //     Qt implement the dispatch loop. The dispatcher should be
    //     initialised with this class but the Qt main loop used.
    //
    //     Note that with <OTC_QtJobQueue> there is no way for a user
    //     supplied setup function to be called before each attempt to find a
    //     job to execute. This code will only work on UNIX.
{
    Q_OBJECT

  public:

			~OTC_QtJobQueue();

    // = INITIALISATION

			OTC_QtJobQueue(QApplication* theApplication);
				// <theApplication> should be the top
				// level Qt application object.

    // = QUEUEING

    OTC_Job*		next(int theActions, int theOptions);
				// Calls Qt to dispatch a single event/job.
				// This may be a Qt or OSE event/job.

    // = SHUTDOWN

    virtual void	shutdown();
				// Called when the dispatcher has been
				// stopped. By default this function
				// will cause the Qt application to exit.

    // = WAKEUP

    void		wakeup(int theType);
				// Derived version of <wakeup()> function to
				// do special processing when signal occurs.

  private slots:

    void		handleQuit();
    				// Stops the dispatcher when the Qt
				// application is being quit.

    void		handleSignal();
    				// Deals with a signal.

    void		handleWakeup();
    				// Deals with wakeup notification from
				// secondary thread.

    void		handleTimeout();
    				// Deals with a timeout.

    void		handlePriorityJob();
    				// Deals with a priority job.

    void		handlePendingStandardJob();
    				// Deals with a pending standard job.

    void		handleStandardJob();
    				// Deals with a standard job.

    void		handlePendingIdleJob();
    				// Deals with a pending idle job.

    void		handleIdleJob();
    				// Deals with an idle job.

    void		handleSocketRead(int theSocket);
    				// Deals with read on a socket.

    void		handleSocketWrite(int theSocket);
    				// Deals with write on a socket.

    void		handleSocketException(int theSocket);
    				// Deals with exception on a socket.

  private:

			OTC_QtJobQueue(OTC_QtJobQueue const&);
				// Do not define an implementation for this.

    OTC_QtJobQueue&	operator=(OTC_QtJobQueue const&);
				// Do not define an implementation for this.

    static OTC_QtJobQueue* instance_;
				// There should only ever be one instance
				// of this class, and this is it.

    QApplication*	application_;
    				// The Qt application object.

    QObject*		signalNotifier_;
    				// Special notifier for handling signals.

    QObject*		wakeupNotifier_;
    				// Special notifier for handling wakeup
				// from secondary threads.

    OTC_HMap<int,QObject*>	fileNotifiersRead_;
				// Record of file notifiers.

    OTC_HMap<int,QObject*>	fileNotifiersWrite_;
				// Record of file notifiers.

    OTC_HMap<int,QObject*>	fileNotifiersException_;
				// Record of file notifiers.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_QT_QTQUEUE_HH */
