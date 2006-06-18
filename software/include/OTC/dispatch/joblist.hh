#ifndef OTC_DISPATCH_JOBLIST_HH
#define OTC_DISPATCH_JOBLIST_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/joblist.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-2006 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/job.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/joblist.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_JobListItem;

class OSE_EXPORT OTC_JobList
    // = TITLE
    //     Queue for holding jobs.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_JobList> holds a list of jobs in a queue format. The class
    //     adds new jobs to the end of the queue and returns jobs from the
    //     front of the queue.
    //     
    // = SEE ALSO
    //     <OTC_Job>
{
  public:

			OTC_JobList()
                          : start_(0), end_(0), count_(0) {}

    // = DESTRUCTION

                        ~OTC_JobList();
				// Goes through the job queue, destroying
				// any jobs.

    // = QUEUEING

    u_int               count() const
                                { return count_; }
                                // Returns the number of jobs in the list.

    bool		isEmpty() const
                                { return start_ == 0; }
				// Returns <true> if there are no jobs in the
				// queue.

    void		add(OTC_Job* theJob);
				// Adds <theJob> to the end of the queue.
				// Generates an exception if <theJob>
				// is <0>.

    void                transfer(OTC_JobList* theJobList);
                                // Transfers jobs from <theJobList> to
                                // the end of this list.

    OTC_Job*		next();
				// Returns the job at the head of the queue.

  private:

			OTC_JobList(OTC_JobList const&);
				// Do not define an implementation for this.

    OTC_JobList&	operator=(OTC_JobList const&);
				// Do not define an implementation for this.

    OTC_JobListItem*    start_;
                                // Start of the list of job items.

    OTC_JobListItem*    end_;
                                // End of the list of job items.

    u_int               count_;
                                // Number of jobs in list.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_JOBLIST_HH */
