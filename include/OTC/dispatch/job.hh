#ifndef OTC_DISPATCH_JOB_HH
#define OTC_DISPATCH_JOB_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/job.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/cmmnpool.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/job.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Job
    // = TITLE
    //     Base class for jobs to be executed by the dispatcher.
    //
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     <OTC_Job> is the base class for any jobs to be executed by the
    //     dispatcher. Derived classes must override the <execute()> function
    //     to perform the actual work. The dispatcher will call <destroy()>
    //     once the job has executed. The default action of <destroy()> is to
    //     delete the object; this can be overridden in a derived class if
    //     necessary.
    //     
    //     Space for any derived classes is allocated from <OTC_CommonPool>.
    //     
    // = SEE ALSO
    //     <OTC_JobQueue>, <OTC_Dispatcher>, <OTC_CommonPool>
{
  public:

    virtual		~OTC_Job();

#if !defined(ENV_NOCOMMONPOOL)
    void*		operator new(size_t theSize)
				{ return OTC_CommonPool::allocate(theSize); }

    void		operator delete(void* theMemory, size_t theSize)
				{ OTC_CommonPool::release(theMemory,theSize); }
#endif

    // = EXECUTION

    virtual void	execute() = 0;
				// Must be redefined in a derived class to
				// perform the actual work associated with
				// the job.

    // = DESTRUCTION

    virtual void	destroy();
				// Called by the dispatcher once the job has
				// been executed. The default action is to
				// delete the object. This can be overridden
				// in a derived class if necessary.

    // = IDENTIFICATION

    virtual void*       type() const;
                                // Could be redefined in a derived class
                                // to return a <void*> which uniquely
                                // identifies the job type. Normally,
                                // a pointer to a static member variable
                                // of a derived class would be used as the
                                // return value. By default, the job type
				// will be that for this class, ie.,
				// <OTC_Job>. You need only override this
				// function if you need to identify a new
				// class of jobs.

    static void*	typeId()
				{ return &globTypeId_; }
				// Returns a value which uniquely identifies
				// this type of event.

  protected:

			OTC_Job()
				{}

  private:

			OTC_Job(OTC_Job const&) {}
				// Do not define an implementation for this.

    OTC_Job&		operator=(OTC_Job const&);
				// Do not define an implementation for this.

    static int		globTypeId_;
				// Tag for mini rtti.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_JOB_HH */
