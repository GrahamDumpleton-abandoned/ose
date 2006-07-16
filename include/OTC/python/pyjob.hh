#ifndef OTC_PYTHON_PYJOB_HH
#define OTC_PYTHON_PYJOB_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyjob.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/dispatch/job.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyJob : public OTC_Job
{
  public:

			~OTC_PyJob();

			OTC_PyJob(PyObject* theCallback);

    void		execute();

    static PyObject*	init_JobCallback(PyObject*, PyObject*);

  private:

			OTC_PyJob(OTC_PyJob const&);

    OTC_PyJob&		operator=(OTC_PyJob const&);

    PyObject*		callback_;

    static PyObject*	gJobCallback_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYJOB_HH */
