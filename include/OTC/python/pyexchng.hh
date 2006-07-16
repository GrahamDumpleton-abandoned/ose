#ifndef OTC_PYTHON_PYEXCHNG_HH
#define OTC_PYTHON_PYEXCHNG_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyexchng.hh
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

#include <OTC/message/exchange.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyExchange : public PyObject, public OTC_EVAgent
{
    friend class	OTC_PyExchangeListener;

  public:

  			OTC_PyExchange(
			 PyObject* theOuter,
			 OTC_ExchangeType theType,
			 char const* theGroup=0
			);

    static PyObject*	create(PyObject* theModule, PyObject* theArgs);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_connect(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_listen(PyObject*, PyObject* theArgs);

  protected:

    void		handle(OTC_Event* theEvent);

    OTC_Boolean		authorise(OTC_String const& theClientHost);

  protected:

			~OTC_PyExchange();

    PyObject*		outer_;

  private:

			OTC_PyExchange(OTC_PyExchange const&);

    OTC_PyExchange&	operator=(OTC_PyExchange const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTC_Exchange	exchange_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYEXCHNG_HH */
