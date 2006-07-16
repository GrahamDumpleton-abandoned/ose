#ifndef OUX_DISPATCH_JOBQUEUE_HH
#define OUX_DISPATCH_JOBQUEUE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OUX
// 
// = FILENAME
//     dispatch/jobqueue.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/jobqueue.hh>

/* ------------------------------------------------------------------------- */

// The OUX_JobQueue class has been moved into the OTC library.
// The class name has been merged into the OTC_JobQueue class.

typedef OTC_JobQueue OUX_JobQueue;
typedef OTC_JobType OUX_JobType;

#define OUXLIB_DONT_WAIT OTCLIB_DONT_WAIT
#define OUXLIB_SIGNAL_JOBS OTCLIB_SIGNAL_JOBS
#define OUXLIB_ALARM_JOBS OTCLIB_ALARM_JOBS
#define OUXLIB_TIMEOUT_JOBS OTCLIB_TIMEOUT_JOBS
#define OUXLIB_IO_JOBS OTCLIB_IO_JOBS
#define OUXLIB_IDLE_JOBS OTCLIB_IDLE_JOBS
#define OUXLIB_PRIORITY_JOBS OTCLIB_PRIORITY_JOBS
#define OUXLIB_ALL_JOBS OTCLIB_ALL_JOBS

#define OUXLIB_IDLE_JOB OTCLIB_IDLE_JOB
#define OUXLIB_PRIORITY_JOB OTCLIB_PRIORITY_JOB

/* ------------------------------------------------------------------------- */

#endif /* OUX_DISPATCH_JOBQUEUE_HH */
