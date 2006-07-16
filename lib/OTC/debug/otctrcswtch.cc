/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otctrcswtch.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1997-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/debug/trcswtch.hh"
#endif
#endif

#include <OTC/debug/trcswtch.hh>

/* ------------------------------------------------------------------------- */
OTC_TraceSwitch::~OTC_TraceSwitch()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_TraceSwitch::enabled() const
{
  return state_;
}

/* ------------------------------------------------------------------------- */
OTC_TraceSwitch::operator int() const
{
  return enabled() ? 0 : -1;
}

/* ------------------------------------------------------------------------- */
OTC_TraceSwitch OTC_TraceSwitch::operator||(
 OTC_TraceSwitch const& theSwitch
) const
{
  return (enabled() || theSwitch.enabled()) ? OTCLIB_TRUE : OTCLIB_FALSE;
}

/* ------------------------------------------------------------------------- */
OTC_TraceSwitch OTC_TraceSwitch::operator&&(
 OTC_TraceSwitch const& theSwitch
) const
{
  return (enabled() && theSwitch.enabled()) ? OTCLIB_TRUE : OTCLIB_FALSE;
}

/* ------------------------------------------------------------------------- */
OTC_TraceSwitch OTC_TraceSwitch::operator!() const
{
  return enabled() ? OTCLIB_FALSE : OTCLIB_TRUE;
}

/* ------------------------------------------------------------------------- */
