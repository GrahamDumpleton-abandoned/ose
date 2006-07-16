#ifndef OTC_MISC_TYPES_H
#define OTC_MISC_TYPES_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     misc/types.h
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

#include <sys/types.h>

#ifndef OSE_CONFIG_H
#include <OSE/OSE.h>
#endif

/* ------------------------------------------------------------------------- */

/* Force this header into dependencies. */
extern int OTCLIB_HEADERDUMMY;

/*
// Standard type abbreviations.
*/

#ifdef M_XENIX
#if defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE)
#define NEED_U_CHAR
#define NEED_U_SHORT
#define NEED_U_INT
#define NEED_U_LONG
#endif
#endif

#ifndef SYS_UNIX
#ifndef OSE_CONFIG_H

#ifdef __BORLANDC__
#define NEED_DEV_T
#define NEED_OFF_T
#define NEED_U_CHAR
#define NEED_U_SHORT
#define NEED_U_INT
#define NEED_U_LONG
#endif

#ifdef __IBMCPP__
#define NEED_U_CHAR
#define NEED_U_SHORT
#define NEED_U_INT
#define NEED_U_LONG
#endif

#ifdef __WATCOMC__
#define NEED_U_CHAR
#define NEED_U_SHORT
#define NEED_U_INT
#define NEED_U_LONG
#endif

#endif
#endif

#ifdef NEED_U_CHAR
typedef unsigned char u_char;
#endif
#ifdef NEED_U_SHORT
typedef unsigned short u_short;
#endif
#ifdef NEED_U_INT
typedef unsigned int u_int;
#endif
#ifdef NEED_U_LONG
typedef unsigned long u_long;
#endif

#ifdef NEED_UID_T
typedef int uid_t;
#endif
#ifdef NEED_GID_T
typedef int gid_t;
#endif
#ifdef NEED_DEV_T
typedef short dev_t;
#endif
#ifdef NEED_OFF_T
typedef long off_t;
#endif
#ifdef NEED_INO_T
typedef unsigned long ino_t;
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_MISC_TYPES_H */
