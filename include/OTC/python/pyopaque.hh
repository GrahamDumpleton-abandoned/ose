#ifndef OTC_PYTHON_PYOPAQUE_HH
#define OTC_PYTHON_PYOPAQUE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyopaque.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/dispatch/opaque.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyOpaque : public PyObject
{
  public:

  			OTC_PyOpaque(OTCEV_Opaque* theMessage);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_sender(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_conversationId(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_sequenceNumber(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_messageId(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_description(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_content(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_contentType(PyObject*, PyObject* theArgs);

  protected:

			~OTC_PyOpaque();

  private:

			OTC_PyOpaque(OTC_PyOpaque const&);

    OTC_PyOpaque&	operator=(OTC_PyOpaque const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_Opaque*	message_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYOPAQUE_HH */
