/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     system/otcsighndle.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/system/sighndle.hh"
#endif
#endif

#include <OTC/system/sighndle.hh>

#include <stdlib.h>
#include <string.h>

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#include <signal.h>
#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif
#if defined(CXX_ATT2_1) || defined(CXX_CL1_1)
#include <osfcn.h>
#endif

#ifdef HAVE_SIGACTION
#if !defined(__osf__)
#if !defined(CXX_CL) && !defined(M_XENIX) && !defined(CXX_ATT2_1) && \
    !defined(CXX_OS) && !defined(__linux__) && \
    !((defined(CXX_SUN) || defined(CXX_LC)) && !defined(HAVE_SYS_ELF_H))
extern "C" int sigaction(int, struct sigaction const*, struct sigaction*);
#endif
#if (defined(CXX_CL) && (defined(CXX_CL1_0) || defined(CXX_CL1_1) || \
    defined(CXX_CL2_0_0) || defined(CXX_CL2_0_1) || \
    defined(CXX_CL2_0_2) || defined(CXX_CL2_0_3)))
extern "C" int sigaction(int, struct sigaction const*, struct sigaction*);
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
OTC_SignalFunction OTC_SignalHandler::install(
 int theSignal,
 OTC_SignalFunction theFunc
)
{
  // <0> usually indicates default signal handler, but do this
  // just to make sure.

  if (theFunc == 0)
    theFunc = (OTC_SignalFunction)SIG_DFL;

#ifdef HAVE_SIGACTION
  struct sigaction act;
  struct sigaction oact;
  memset(&act,0,sizeof(struct sigaction));
  memset(&oact,0,sizeof(struct sigaction));

#if defined(CXX_SUN)
#if defined(HAVE_SYS_RESOURCE_H) && !defined(HAVE_SYS_ELF_H)
  // Sun C++ - SunOS 4.1.X / Sun C++ 2.1 / Sun C++ 3.0.X
#if defined(CXX_SUN2_1) || defined(CXX_SUN3_0)
  act.sa_handler = (void (*)(DOTDOTDOT))theFunc;
#else
  act.sa_handler = (void (*)(int,DOTDOTDOT))theFunc;
#endif
#else
  // Sun C++ - SunOS 5.X
#if (_POSIX_VERSION - 0 >= 199506L)
  act.sa_handler = theFunc;
#else
  act.sa_handler = (void (*)())theFunc;
#endif
#endif
#else
#if defined(M_XENIX)
  // SCO UNIX
#ifdef __STDC__
  act.sa_handler = theFunc;
#else
  act.sa_handler = __SIG_FTN theFunc;
#endif
#else
#if defined(__nonstopux)
  // Tandem Non Stop UNIX
  act.sa_handler = (void (*)_EXTERN_ARGS((int)))theFunc;
#else
#if defined(hpux) || defined(__hpux)
  act.sa_handler = (void (*)(__harg))theFunc;
#else
#if defined(CXX_LC)
  act.sa_handler = (void (*)_LUCID_SIGARGLIST)theFunc;
#else
#if defined(__linux__) || defined(_AIX) || defined(__osf__)
  act.sa_handler = theFunc;
#else
#if defined(__sgi)
#if defined(sigargs_)
  act.sa_handler = (__sigret_t (*)(sigargs_))theFunc;
#else
  act.sa_handler = (__sigret_t (*)(__sigargs))theFunc;
#endif
#else
#if defined(CXX_CL) && \
    !defined(CXX_CL1_0) && !defined(CXX_CL1_1) && \
    !defined(CXX_CL2_0_0) && !defined(CXX_CL2_0_1) && \
    !defined(CXX_CL2_0_2) && !defined(CXX_CL2_0_3)
#if defined(HAVE_SYS_RESOURCE_H) && !defined(HAVE_SYS_ELF_H)
  act.sa_handler = (SIG_PF)theFunc;
#else
#if defined(sun) || defined(__sun)
#if (_POSIX_VERSION - 0 >= 199506L)
  act.sa_handler = theFunc;
#else
  act.sa_handler = (void (*)())theFunc;
#endif
#else
  act.sa_handler = (void (*)())theFunc;
#endif
#endif
#else
#ifdef ENV_VXWORKS
  act.sa_handler = theFunc;
#else
#if defined(sun) || defined(__sun)
#if defined(HAVE_SYS_RESOURCE_H) && !defined(HAVE_SYS_ELF_H)
  act.sa_handler = (void (*)())theFunc;
#else
#if (_POSIX_VERSION - 0 >= 199506L)
  act.sa_handler = theFunc;
#else
  act.sa_handler = (void (*)())theFunc;
#endif
#endif
#else
#if defined(__CYGWIN32__) || defined(__bsdi__)
  act.sa_handler = (void (*)(int))theFunc;
#else
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__APPLE__)
  // Prior to 2.2.2, first prototype used.
  // Will have to be changed manually, if
  // you are using older version of FreeBSD.
  // act.sa_handler = (void (*)())theFunc;
  act.sa_handler = (void (*)(int))theFunc;
#else
  act.sa_handler = (void (*)())theFunc;
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(theSignal,(struct sigaction*)&act,(struct sigaction*)&oact);
  return (OTC_SignalFunction)oact.sa_handler;
#else
#if defined(SIG_PF)
  return (OTC_SignalFunction)::signal(theSignal,(SIG_PF)theFunc);
#else
  return (OTC_SignalFunction)::signal(theSignal,theFunc);
#endif
#endif
}

/* ------------------------------------------------------------------------- */
