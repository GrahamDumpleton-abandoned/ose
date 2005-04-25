#ifndef OPY_JOB_HH
#define OPY_JOB_HH
/*
// ============================================================================
//
// = FILENAME
//     job.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2005 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/collctn/hmap.hh>
#include <OTC/dispatch/job.hh>

/* ------------------------------------------------------------------------- */

class OPY_Job : public OTC_Job
{
  public:

			~OPY_Job();

			OPY_Job(PyObject* theCallback);

    void		execute();

    static PyObject*	init_JobCallback(PyObject*, PyObject*);

  private:

			OPY_Job(OPY_Job const&);

    OPY_Job&		operator=(OPY_Job const&);

    PyObject*		callback_;
    
    PyInterpreterState*	interpreterState_;

    static OTC_HMap<PyInterpreterState*,PyObject*>*	gJobCallbacks_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_JOB_HH */
