#ifndef OSE_OSE_H
#define OSE_OSE_H
/*
// ============================================================================
//
// = LIBRARY
//     OSE
// 
// = FILENAME
//     OSE.h
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */

#include <OSE-VERSION.h>
#include <OSE-CONFIG.h>

/* ------------------------------------------------------------------------- */

#if defined(unix) || defined(_unix) || defined(__unix) || \
    defined(_AIX) || defined(LYNX) || defined(__CYGWIN32__) || \
    defined(__APPLE__)
#define SYS_UNIX 1
#endif

/* ------------------------------------------------------------------------- */

#if defined(__APPLE__)
#if 0
#ifndef HAVE_POSIX_THREADS
#define HAVE_POSIX_THREADS
#endif
#endif
#ifndef ENV_CXXSTDLIB
#define ENV_CXXSTDLIB
#endif
#ifndef EXPAND_TEMPLATES
#define EXPAND_TEMPLATES
#endif
#endif

#if defined(__GNUG__)
#ifndef HAVE_BOOL
#define HAVE_BOOL
#endif
#ifndef HAVE_EXCEPTIONS
#define HAVE_EXCEPTIONS
#endif
#endif

/* ------------------------------------------------------------------------- */

#if !defined(SYS_UNIX)
#if defined(PIC) && defined(DLL_EXPORT_OSE)
#define OSE_EXPORT __declspec(dllexport)
#else
#if defined(DLL_IMPORT_OSE)
#define OSE_EXPORT __declspec(dllimport)
#else
#define OSE_EXPORT
#endif
#endif
#else
#define OSE_EXPORT
#endif

/* ------------------------------------------------------------------------- */

#if defined(SYS_UNIX)
#if !defined(CXX_GNU2_5)
#if defined(__cplusplus) && !defined(PIC)
extern char const* const OSE_RELEASE_TAG;
static char const* const OSE_RELEASE = OSE_RELEASE_TAG;
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

#endif /* OSE_OSE_H */
