#ifndef OTC_MISC_BOOL_H
#define OTC_MISC_BOOL_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     misc/bool.h
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1995-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */

/*
// Definition of a boolean type and values for TRUE and FALSE. Don't define
// TRUE and FALSE if already defined. If they were defined pray that they
// were given sensible values.
*/

#ifdef __cplusplus

/* C++ Code */

#if defined(__GNUG__) && ((__GNUC__ >= 3) || (__GNUC_MINOR >= 6))
#ifndef HAVE_BOOL
#define HAVE_BOOL
#endif
#endif

/*
// Note: Cygnus C++ has dropped definition of __GNUC_MINOR. However, also
// seems that __GNUC_MINOR wasn't used in GNU C++ prior to 2.6.0.
*/

#if !defined(CXX_GNU2_5)
#if defined(__GNUG__) && !defined(__GNUC_MINOR)
#ifndef HAVE_BOOL
#define HAVE_BOOL
#endif
#endif
#endif

#if defined(HAVE_BOOL)

typedef bool OTC_Boolean;

#define OTCLIB_FALSE false
#define OTCLIB_TRUE true

#else

typedef int OTC_Boolean;

#define OTCLIB_FALSE 0
#define OTCLIB_TRUE 1

#endif

#else

/* C Code */

typedef int OTC_Boolean;

#define OTCLIB_FALSE 0
#define OTCLIB_TRUE 1

#endif

/* Backward compatability */

#if defined(OTCLIB_BOOLBC)

#if !defined(__cplusplus) || !defined(HAVE_BOOL)
typedef int bool;
#endif

#ifndef FALSE
#define	FALSE 0
#define TRUE 1
#endif

#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_MISC_BOOL_H */
