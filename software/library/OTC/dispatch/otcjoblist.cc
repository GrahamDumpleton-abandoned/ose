/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcjoblist.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1995-2006 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/joblist.hh"
#endif
#endif

#include <OTC/dispatch/joblist.hh>

#include <OTC/memory/mpobject.hh>

/* ------------------------------------------------------------------------- */
struct OSE_EXPORT OTC_JobListItem : public OTC_MPObject
{
  public:

                        OTC_JobListItem(OTC_Job* theJob)
                          : job(theJob), next(0) {}

    OTC_Job*            job;

    OTC_JobListItem*    next;
};

/* ------------------------------------------------------------------------- */
OTC_JobList::~OTC_JobList()
{
  OTC_Job* theJob;

  theJob = next();
  while (theJob)
  {
    theJob->destroy();

    theJob = next();
  }
}

/* ------------------------------------------------------------------------- */
void OTC_JobList::add(OTC_Job* theJob)
{
  OTCLIB_ENSURE_FN((theJob != 0),
   "OTC_JobList::add(OTC_Job*)",
   "invalid job");

  OTC_JobListItem* theItem;
  theItem = new OTC_JobListItem(theJob);
  OTCLIB_ASSERT_M(theItem != 0);

  if (start_ == 0)
  {
    start_ = theItem;
    end_ = theItem;
    count_ = 1;
  }
  else
  {
    end_->next = theItem;
    end_ = theItem;
    count_++;
  }
}

/* ------------------------------------------------------------------------- */
void OTC_JobList::transfer(OTC_JobList* theJobList)
{
  OTCLIB_ENSURE_FN((theJobList != 0),
   "OTC_JobList::transfer(OTC_JobList*)",
   "invalid job list");

  if (theJobList->start_ == 0)
    return;

  if (start_ == 0)
  {
    start_ = theJobList->start_;
    end_ = theJobList->end_;
    count_ = theJobList->count_;
  }
  else
  {
    end_->next = theJobList->start_;
    end_ = theJobList->end_;
    count_ += theJobList->count_;
  }

  theJobList->start_ = 0;
  theJobList->end_ = 0;
  theJobList->count_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_Job* OTC_JobList::next()
{
  if (count_ == 0)
    return 0;

  OTC_Job* theJob;
  OTC_JobListItem* theItem;

  theItem = start_;
  theJob = start_->job;

  start_ = start_->next;
  if (start_ == 0)
    end_ = 0;

  count_--;

  delete theItem;

  return theJob;
}

/* ------------------------------------------------------------------------- */
