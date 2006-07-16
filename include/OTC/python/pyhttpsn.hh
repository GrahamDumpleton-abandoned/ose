#ifndef OTC_PYTHON_PYHTTPSN_HH
#define OTC_PYTHON_PYHTTPSN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     python/pyhttpsn.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/dispatch/evagent.hh>
#include <OTC/internet/httpsssn.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_PyHttpSession : public PyObject
{
  public:

  			OTC_PyHttpSession(OTC_HttpSession* theSession);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_instance(PyObject*, PyObject*);

    static PyObject*	mfn_contentLength(PyObject*, PyObject*);

    static PyObject*	mfn_contentType(PyObject*, PyObject*);

    static PyObject*	mfn_requestMethod(PyObject*, PyObject*);

    static PyObject*	mfn_url(PyObject*, PyObject*);

    static PyObject*	mfn_protocolVersion(PyObject*, PyObject*);

    static PyObject*	mfn_serverHost(PyObject*, PyObject*);

    static PyObject*	mfn_serverPort(PyObject*, PyObject*);

    static PyObject*	mfn_requestPath(PyObject*, PyObject*);

    static PyObject*	mfn_queryString(PyObject*, PyObject*);

    static PyObject*	mfn_clientHost(PyObject*, PyObject*);

    static PyObject*	mfn_clientPort(PyObject*, PyObject*);

    static PyObject*	mfn_remoteUser(PyObject*, PyObject*);

    static PyObject*	mfn_headers(PyObject*, PyObject*);

    static PyObject*	mfn_containsHeader(PyObject*, PyObject*);

    static PyObject*	mfn_header(PyObject*, PyObject*);

    static PyObject*	mfn_serverRoot(PyObject*, PyObject*);

    static PyObject*	mfn_serverPath(PyObject*, PyObject*);

    static PyObject*	mfn_servletPath(PyObject*, PyObject*);

    static PyObject*	mfn_servletInfo(PyObject*, PyObject*);

    static PyObject*	mfn_bindServlet(PyObject*, PyObject*);

    static PyObject*	mfn_sessionId(PyObject*, PyObject*);

    static PyObject*	mfn_sessionContext(PyObject*, PyObject*);

    static PyObject*	mfn_bindSession(PyObject*, PyObject*);

  protected:

    void		handle(OTC_Event* theEvent);

			~OTC_PyHttpSession();

    OTC_HttpSession*	session_;

  private:

			OTC_PyHttpSession(OTC_PyHttpSession const&);

    OTC_PyHttpSession&	operator=(OTC_PyHttpSession const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_PYTHON_PYHTTPSN_HH */
