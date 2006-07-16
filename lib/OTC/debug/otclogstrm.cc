/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otclogstrm.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/debug/logstrm.hh"
#endif
#endif

#include <OTC/debug/logstrm.hh>

#if !defined(CXX_GNU2_5)

#include <new.h>

// #if defined(__BORLANDC__)
// extern void *operator new(size_t, void*);
// #endif

#endif

/* ------------------------------------------------------------------------- */
static char otclib_logbuf[2048];
static OTC_LogStream* otclib_logstrm = 0;
static double otclib_logstrm_mem[(sizeof(OTC_LogStream)/sizeof(double))+1];

/* ------------------------------------------------------------------------- */
OTC_LogStream::OTC_LogStream(char* theBuffer, int theSize)
  : OTC_LogBufWrapper(theBuffer,theSize),
    ostream(OTC_LogBufWrapper::streambuf()) 
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_LogStream::~OTC_LogStream()
{
  OTC_LogBufWrapper::streambuf()->pubsync();
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT OTC_LogStream& OTCLIB_LOGGER(OTC_LogLevel theLevel)
{
  if (otclib_logstrm == 0)
  {
#if defined(__BORLANDC__) || defined(_MSC_VER) || defined(CXX_GNU2_5)
    otclib_logstrm = new OTC_LogStream(otclib_logbuf,sizeof(otclib_logbuf));
#else
    otclib_logstrm = new (otclib_logstrm_mem) OTC_LogStream(
     otclib_logbuf,sizeof(otclib_logbuf)
    );
#endif
  }
  otclib_logstrm->setLevel(theLevel);
  otclib_logstrm->clear();
  return *otclib_logstrm;
}

/* ------------------------------------------------------------------------- */
