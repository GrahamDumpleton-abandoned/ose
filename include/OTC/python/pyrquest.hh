#ifndef OTC_PYTHON_PYRQUEST_HH
#define OTC_PYTHON_PYRQUEST_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyrquest.hh
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

#include <OTC/dispatch/request.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyRequest : public PyObject
{
  public:

  			OTC_PyRequest(OTCEV_Request* theRequest);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_sender(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_conversationId(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_sequenceNumber(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_methodName(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_content(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_bounceRequest(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_sendResponse(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_sendFailure(PyObject*, PyObject* theArgs);

  protected:

			~OTC_PyRequest();

  private:

			OTC_PyRequest(OTC_PyRequest const&);

    OTC_PyRequest&	operator=(OTC_PyRequest const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_Request*	request_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYRQUEST_HH */
