#ifndef OTC_PYTHON_PYEXCHNC_HH
#define OTC_PYTHON_PYEXCHNC_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyexchnc.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/message/exchannc.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyExchangeAnnouncement : public PyObject
{
  public:

  			OTC_PyExchangeAnnouncement(
			 OTCEV_ExchangeAnnouncement* theAnnouncement
			);

    OTCEV_ExchangeAnnouncement*	announcement() const
    				{ return announcement_; }

    static int		isType(PyObject* theObject)
    				{ return theObject->ob_type == &gType_; }

    PyObject*		obj_getattr(char const* theName);

    int			obj_compare(OTC_PyExchangeAnnouncement const*) const;

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static int          sys_compare(PyObject*, PyObject*);

    static PyObject*	mfn_group(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_localAddress(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_remoteAddress(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_remoteProcess(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_state(PyObject*, PyObject* theArgs);

  protected:

			~OTC_PyExchangeAnnouncement();

  private:

			OTC_PyExchangeAnnouncement(
			 OTC_PyExchangeAnnouncement const&
			);

    OTC_PyExchangeAnnouncement&	operator=(OTC_PyExchangeAnnouncement const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_ExchangeAnnouncement*	announcement_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYEXCHNC_HH */
