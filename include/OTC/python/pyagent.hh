#ifndef OTC_PYTHON_PYAGENT_HH
#define OTC_PYTHON_PYAGENT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyagent.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/python/pydsptch.hh>

#include <OTC/dispatch/evhandle.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/thread/mutex.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyAgent : public PyObject, public OTC_EVAgent
{
  public:

  			OTC_PyAgent(PyObject* theOuter);

    static PyObject*	create(PyObject* theModule, PyObject* theArgs);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_acquireObject(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_releaseObject(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_activateThreading(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_suspendThreading(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_threadingEnabled(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_id(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_startTimer(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_cancelTimer(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_cancelAllTimers(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_setAlarm(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_cancelAlarm(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_cancelAllAlarms(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_scheduleAction(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_scheduleCronAction(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_cancelAction(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_cancelAllActions(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subscribeSignal(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_unsubscribeSignal(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_unsubscribeAllSignals(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_scheduleShutdown(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_suspendShutdown(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_resumeShutdown(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subscribeShutdown(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_unsubscribeShutdown(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subscribeSocket(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_unsubscribeSocket(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_unsubscribeAllSockets(PyObject*, PyObject* theArgs);

  protected:

    void		execute(OTC_Event* theEvent);

    void		filter(OTC_Event* theEvent);

    void		handle(OTC_Event* theEvent);

    void                enterPython()
                                {
#ifdef WITH_THREAD
                                  if (threadState_ != 0)
                                    PyEval_AcquireThread(threadState_);
                                  else
                                    OTC_PyDispatcher::enterPython();
#endif
                                  PyErr_Clear();
                                }

    void                leavePython()
                                {
#ifdef WITH_THREAD
                                  if (threadState_ != 0)
                                    PyEval_ReleaseThread(threadState_);
                                  else
                                    OTC_PyDispatcher::leavePython();
#endif
                                }

  protected:

			~OTC_PyAgent();

    PyObject*		outer_;

  private:

			OTC_PyAgent(OTC_PyAgent const&);

    OTC_PyAgent&	operator=(OTC_PyAgent const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTC_Mutex		mutex_;

    OTC_EVHandler*	threadPool_;

    OTC_Boolean		threadingSuspended_;

    PyInterpreterState* interpreterState_;

    PyThreadState*      threadState_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYAGENT_HH */
