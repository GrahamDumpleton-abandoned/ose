#ifndef OTC_THREAD_THREAD_HH
#define OTC_THREAD_THREAD_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/thread.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OSE_OSE_H
#include <OSE/OSE.h>
#endif

/* ------------------------------------------------------------------------- */

#if defined(HAVE_SOLARIS_THREADS)
#include <thread.h>
#include <synch.h>
#define ENV_THREADS 1
#endif

#if defined(HAVE_POSIX_THREADS)
#include <pthread.h>
#define ENV_THREADS 1
#if defined(_AIX) && defined(MUTEX_RECURSIVE_NP)
#undef MUTEX_RECURSIVE_NP
#endif
#endif

#if defined(HAVE_WIN32_THREADS)
#if defined(HAVE_WINSOCK2)
#include <winsock2.h>
#endif
#include <windows.h>
#define ENV_THREADS 1
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_THREAD_THREAD_HH */
