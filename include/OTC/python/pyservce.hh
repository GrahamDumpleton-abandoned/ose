#ifndef OTC_PYTHON_PYSERVCE_HH
#define OTC_PYTHON_PYSERVCE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyservce.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/python/pymnitor.hh>
#include <OTC/dispatch/svbroker.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyService : public OTC_PyMonitor
{
  public:

  			OTC_PyService(
			 PyObject* theOuter,
			 char const* theServiceName,
			 char const* theServiceAudience="*"
			);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_serviceBinding(PyObject*, PyObject*);

    static PyObject*	mfn_joinGroup(PyObject*, PyObject*);

    static PyObject*	mfn_leaveGroup(PyObject*, PyObject*);

    static PyObject*	mfn_sendRequest(PyObject*, PyObject*);

    static PyObject*	mfn_sendMessage(PyObject*, PyObject*);

    static PyObject*	mfn_subscribeReports(PyObject*, PyObject*);

    static PyObject*	mfn_unsubscribeReports(PyObject*, PyObject*);

    static PyObject*	mfn_publishReport(PyObject*, PyObject*);

    static PyObject*	mfn_sendReport(PyObject*, PyObject*);

    static PyObject*	mfn_expireReports(PyObject*, PyObject*);

    static PyObject*	mfn_expireAllReports(PyObject*, PyObject*);

    static PyObject*	mfn_purgeReports(PyObject*, PyObject*);

    static PyObject*	mfn_matchSubject(PyObject*, PyObject*);

  public:

    static PyObject*	payloadToObject(
			 OTC_ROPayload const& thePayload,
			 PyObject* theCallback
			);

    static OTC_Boolean	objectToPayload(
			 PyObject* theObject,
			 OTC_RWPayload thePayload,
			 PyObject* theCallback
			);

  protected:

    void		handle(OTC_Event* theEvent);

  protected:

			~OTC_PyService();

  private:

			OTC_PyService(OTC_PyService const&);

    OTC_PyService&	operator=(OTC_PyService const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTC_SVBroker	broker_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYSERVCE_HH */
