/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     python/otcpymodule.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 2000-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef ANY
#undef ANY
#endif

#include <OTC/message/inetclnt.hh>
#include <OTC/message/inetlstn.hh>

#include <OTC/python/pymodule.hh>
#include <OTC/python/pydsptch.hh>
#include <OTC/python/pyprogrm.hh>
#include <OTC/python/pylogger.hh>
#include <OTC/python/pyagent.hh>
#include <OTC/python/pyservce.hh>
#include <OTC/python/pyexchng.hh>
#include <OTC/python/pyrpcgwy.hh>
#include <OTC/python/pyfilesr.hh>
#include <OTC/python/pyhttpdm.hh>
#include <OTC/python/pyhttpsr.hh>
#include <OTC/python/pyhttpsl.hh>
#include <OTC/python/pymnitor.hh>
#include <OTC/python/pyechosl.hh>
#include <OTC/python/pyfilesl.hh>
#include <OTC/python/pyrpcslt.hh>
#include <OTC/python/pyxmlrsl.hh>
#include <OTC/python/pyjob.hh>

/* ------------------------------------------------------------------------- */
PyObject* OTCLIB_PYTHON_ERROR_OBJECT;

/* ------------------------------------------------------------------------- */
static struct PyMethodDef otclib_module_methods[] = {
  { "init_JobCallback", OTC_PyJob::init_JobCallback, 1 },
  { "new_Logger", OTC_PyLogger::create, 1 },
  { "new_Agent", OTC_PyAgent::create, 1 },
  { "new_Service", OTC_PyService::create, 1 },
  { "new_Monitor", OTC_PyMonitor::create, 1 },
  { "new_Exchange", OTC_PyExchange::create, 1 },
  { "new_HttpDaemon", OTC_PyHttpDaemon::create, 1 },
  { "new_HttpServer", OTC_PyHttpServer::create, 1 },
  { "new_HttpServlet", OTC_PyHttpServlet::create, 1 },
  { "new_EchoServlet", OTC_PyEchoServlet::create, 1 },
  { "new_FileServlet", OTC_PyFileServlet::create, 1 },
  { "new_RpcServlet", OTC_PyRpcServlet::create, 1 },
  { "new_XmlRpcServlet", OTC_PyXmlRpcServlet::create, 1 },
  { "new_FileServer", OTC_PyFileServer::create, 1 },
  { "new_RpcGateway", OTC_PyRpcGateway::create, 1 },
  { "Dispatcher", OTC_PyDispatcher::instance, 1 },
  { "Program", OTC_PyProgram::instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OSE_EXPORT void otclib_init_python()
{
  PyObject* theModule;

  // Enable display of exceptions.

  OTC_Exception::enableLogging();


  // Create the module and add the functions.

  theModule = Py_InitModule("_ose",otclib_module_methods);

  // Check for errors.

  if (PyErr_Occurred())
    Py_FatalError("can't initialise module _ose");
}

/* ------------------------------------------------------------------------- */
