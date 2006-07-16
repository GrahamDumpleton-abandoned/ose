/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     system/otcsigblock.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1996-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/system/sigblock.hh"
#endif
#endif

#include <OTC/system/sigblock.hh>

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif
#include <stdlib.h>
#if defined(CXX_ATT2_1) || defined(CXX_CL1_1)
#include <osfcn.h>
#endif

#if defined(HAVE_SIGPROCMASK)
#if !defined(__osf__)
#if defined(sun) || defined(__sun)
  // Sun platform.
#if defined(HAVE_SYS_RESOURCE_H) && !defined(HAVE_SYS_ELF_H)
  // SunOS 4.1.X
#if !defined(CXX_SUN) && !defined(CXX_CL2_1)
  // Non Sun / Non CenterLine 2.1 compiler.
  extern "C" int sigprocmask(int, sigset_t const*, sigset_t*);
#endif
#endif
#else
  // Non Sun platform.
#if !defined(M_XENIX) && !defined(__linux__)
  // Non SCO/LINUX platform.
  extern "C" int sigprocmask(int, sigset_t const*, sigset_t*);
#endif
#endif
#ifndef sigemptyset
extern "C" int sigemptyset(sigset_t*);
#endif
#ifndef sigfillset
extern "C" int sigfillset(sigset_t*);
#endif
#ifndef sigaddset
extern "C" int sigaddset(sigset_t*, int);
#endif
#ifndef sigdelset
extern "C" int sigdelset(sigset_t*, int);
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */
OTC_SignalBlock::OTC_SignalBlock(int theSignal)
  : signal_(theSignal)
{
  if (signal_ == 0)
    return;

#if defined(SYS_UNIX)
#if defined(HAVE_SIGPROCMASK)
  sigset_t theNewMask;
  if (theSignal == -1)
  {
    sigfillset(&theNewMask);
    sigdelset(&theNewMask,SIGABRT);
  }
  else
  {
    sigemptyset(&theNewMask);
    sigaddset(&theNewMask,theSignal);
  }

  sigprocmask(SIG_BLOCK,&theNewMask,&oldMask_);
#else
  int theNewMask = 0;
  if (theSignal == -1)
  {
    theNewMask = sigmask(SIGABRT);
    theNewMask ^= ~0;
  }
  else
    theNewMask = sigmask(theSignal);

  oldMask_ = sigblock(theNewMask);
#endif
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_SignalBlock::release()
{
  if (signal_ == 0)
    return;

#if defined(SYS_UNIX)
#if defined(HAVE_SIGPROCMASK)
  sigprocmask(SIG_SETMASK,&oldMask_,0);
#else
  sigsetmask(oldMask_);
#endif
#endif

  signal_ = 0;
}

/* ------------------------------------------------------------------------- */
