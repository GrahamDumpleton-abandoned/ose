/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/otcpattern.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1996-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/pattern.hh"
#endif
#endif

#include <OTC/text/pattern.hh>

#include <string.h>

/* ------------------------------------------------------------------------- */
OTC_Pattern::OTC_Pattern(char const* thePattern)
{
  OTCLIB_ENSURE_FN((thePattern != 0),
   "OTC_Pattern::OTC_Pattern(char const*)",
   "invalid argument");

  pattern_ = new char[strlen(thePattern)+1];
  OTCLIB_ASSERT_M(pattern_ != 0);
  strcpy(pattern_,thePattern);
}

/* ------------------------------------------------------------------------- */
OTC_Pattern::~OTC_Pattern()
{
  delete [] pattern_;
}

/* ------------------------------------------------------------------------- */
OTC_Range OTC_Pattern::range(u_int theIndex) const
{
  return OTC_Range(start(theIndex),length(theIndex));
}

/* ------------------------------------------------------------------------- */
u_int OTC_Pattern::start(u_int) const
{
  return 0;
}

/* ------------------------------------------------------------------------- */
u_int OTC_Pattern::length(u_int) const
{
  return 0;
}

/* ------------------------------------------------------------------------- */
