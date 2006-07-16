/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/otctracepat.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/tracepat.hh"
#endif
#endif

#include <OTC/text/tracepat.hh>
#include <OTC/text/regexp.hh>
#include <OTC/thread/mxreaper.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_TracePattern::mutex_;
OTC_Regexp* OTC_TracePattern::globRegexp_ = 0;
OTC_TracePattern* OTC_TracePattern::globHead_ = 0;

/* ------------------------------------------------------------------------- */
OTC_TracePattern::OTC_TracePattern(char const* theString)
  : state_(OTCLIB_FALSE), string_(theString)
{
  OTCLIB_ENSURE_FN((theString != 0),
   "OTC_TracePattern::OTC_TracePattern(char const*)",
   "invalid string");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  prev_ = 0;
  next_ = globHead_;
  if (globHead_ != 0)
    globHead_->prev_ = this;
  globHead_ = this;

  if (globRegexp_ == 0)
  {
    char const* thePattern = getenv("OTCLIB_TRACEPATTERN");
    if (thePattern == 0 || *thePattern == EOS)
      thePattern = "^$";

    globRegexp_ = new OTC_Regexp(thePattern);
    OTCLIB_ASSERT_M(globRegexp_ != 0);
  }

  if (globRegexp_->isValid() && globRegexp_->match(string_))
    state_ = OTCLIB_TRUE;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_TracePattern::~OTC_TracePattern()
{
  mutex_.lock();

  if (globHead_ == this)
    globHead_ = next_;
  if (prev_ != 0)
    prev_->next_ = next_;
  if (next_ != 0)
    next_->prev_ = prev_;

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_TracePattern::enabled() const
{
  return state_;
}

/* ------------------------------------------------------------------------- */
void OTC_TracePattern::set(char const* thePattern)
{
  OTCLIB_ENSURE_FN((thePattern != 0),
   "OTC_TracePattern::set(char const*)",
   "invalid pattern");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globRegexp_ != 0)
    delete globRegexp_;

  if (thePattern == 0 || *thePattern == EOS)
    thePattern = "^$";

  globRegexp_ = new OTC_Regexp(thePattern);
  OTCLIB_ASSERT_M(globRegexp_ != 0);

  OTC_TracePattern* theObject = globHead_;
  while (theObject != 0)
  {
    if (globRegexp_->isValid() && globRegexp_->match(theObject->string_))
      theObject->state_ = OTCLIB_TRUE;
    else
      theObject->state_ = OTCLIB_FALSE;

    theObject = theObject->next_;
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
