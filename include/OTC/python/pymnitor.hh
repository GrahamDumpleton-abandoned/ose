#ifndef OTC_PYTHON_PYMNITOR_HH
#define OTC_PYTHON_PYMNITOR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pymnitor.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pyagent.hh>
#include <OTC/dispatch/svbroker.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyMonitor : public OTC_PyAgent
{
  public:

  			OTC_PyMonitor(PyObject* theOuter);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_serviceAgents(PyObject*, PyObject*);

    static PyObject*	mfn_serviceGroups(PyObject*, PyObject*);

    static PyObject*	mfn_lookupServiceName(PyObject*, PyObject*);

    static PyObject*	mfn_lookupServiceAddress(PyObject*, PyObject*);

    static PyObject*	mfn_lookupServiceGroup(PyObject*, PyObject*);

    static PyObject*	mfn_unsubscribeAllRegistry(PyObject*, PyObject*);

    static PyObject*	mfn_subscribeRegistry(PyObject*, PyObject*);

    static PyObject*	mfn_unsubscribeRegistry(PyObject*, PyObject*);

    static PyObject*	mfn_subscribeServiceName(PyObject*, PyObject*);

    static PyObject*	mfn_unsubscribeServiceName(PyObject*, PyObject*);

    static PyObject*	mfn_subscribeServiceAddress(PyObject*, PyObject*);

    static PyObject*	mfn_unsubscribeServiceAddress(PyObject*, PyObject*);

    static PyObject*	mfn_subscribeServiceGroup(PyObject*, PyObject*);

    static PyObject*	mfn_unsubscribeServiceGroup(PyObject*, PyObject*);

  protected:

    void		handle(OTC_Event* theEvent);

  protected:

			~OTC_PyMonitor();

  private:

			OTC_PyMonitor(OTC_PyMonitor const&);

    OTC_PyMonitor&	operator=(OTC_PyMonitor const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYMNITOR_HH */
