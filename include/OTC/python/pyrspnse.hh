#ifndef OTC_PYTHON_PYRSPNSE_HH
#define OTC_PYTHON_PYRSPNSE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyrspnse.hh
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

#include <OTC/dispatch/response.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyResponse : public PyObject
{
  public:

  			OTC_PyResponse(OTCEV_Response* theRequest);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_sender(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_conversationId(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_sequenceNumber(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_content(PyObject*, PyObject* theArgs);

  protected:

			~OTC_PyResponse();

  private:

			OTC_PyResponse(OTC_PyResponse const&);

    OTC_PyResponse&	operator=(OTC_PyResponse const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_Response*	response_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYRSPNSE_HH */
