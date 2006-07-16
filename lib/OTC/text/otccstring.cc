/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/otccstring.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1997-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/cstring.hh"
#endif
#endif

#include <OTC/text/cstring.hh>
#include <OTC/collctn/copyactn.hh>

#include <string.h>

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_CString::typespec()
{
  static os_typespec ts("OTC_CString");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_CString::~OTC_CString()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_CString::OTC_CString(char const* theString)
  : data_((theString ? ::strlen(theString) : 0),OTCLIB_UNBUFFERED)
{
  if (theString != 0)
    OTC_CopyActions<char>::copy(data_.string(),theString,data_.length());
}

/* ------------------------------------------------------------------------- */
OTC_CString::OTC_CString(char theChar, u_int theNum)
  : data_(theNum,OTCLIB_UNBUFFERED)
{
  memset(data_.string(),theChar,data_.length());
}

/* ------------------------------------------------------------------------- */
OTC_CString::OTC_CString(char const* theString, u_int theNum)
  : data_(theNum,OTCLIB_UNBUFFERED)
{
  OTCLIB_ENSURE_FN((theNum == 0 || theString != 0),
   "OTC_CString(char const*, u_int)",
   "null string but non zero length");

  if (theString != 0)
    OTC_CopyActions<char>::copy(data_.string(),theString,theNum);
}

/* ------------------------------------------------------------------------- */
