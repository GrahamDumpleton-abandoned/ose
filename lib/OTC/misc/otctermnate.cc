/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     misc/otctermnate.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/misc/termnate.hh>

#include <stdlib.h>

#if defined(NEED_TERMINATE)

/* ------------------------------------------------------------------------- */
static PFV otclib_terminateFunc = 0;

/* ------------------------------------------------------------------------- */
void terminate()
{
  static int loop = 0;
  if (loop != 0)
    abort();
  loop++;

  if (otclib_terminateFunc)
    otclib_terminateFunc();

  // Terminate function shouldn't return, if it does then abort anyway.

  abort();
}

/* ------------------------------------------------------------------------- */
PFV set_terminate(PFV theFunc)
{
  PFV oldFunc = otclib_terminateFunc;
  otclib_terminateFunc = theFunc;
  return oldFunc;
}

/* ------------------------------------------------------------------------- */

#endif
