/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     debug/otcerrors.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/debug/errors.h>
#include <OTC/debug/excption.hh>
#include <OTC/debug/throwerr.hh>
#include <OTC/misc/termnate.hh>

#include <stdlib.h>
#include <stdio.h>

/* ------------------------------------------------------------------------- */
class OTC_PleaseLetMeDie { public: OTC_PleaseLetMeDie() {} };

/* ------------------------------------------------------------------------- */
OSE_EXPORT void OTCLIB_TERMINATE()
{
#if defined(HAVE_EXCEPTIONS)
  OTC_PleaseLetMeDie tmpException;
  throw tmpException;
#else
  terminate();
#endif
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT void otclib_exception(
 char const* theFile,
 int theLine,
 char const* theMessage
)
{
  OTC_Exception exception(theMessage,theFile,theLine);
  OTCLIB_THROW(exception);
}

/* ------------------------------------------------------------------------- */
