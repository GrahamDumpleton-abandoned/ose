/* host/OSE-CONFIG.h.  Generated automatically by configure.  */
#ifndef OSE_CONFIG_H
#define OSE_CONFIG_H
/*
// ============================================================================
//
// = FILENAME
//     OSE-CONFIG.h
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-1998 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */

/* Directory access. */

#define HAVE_DIRENT_H 1
/* #undef HAVE_SYS_NDIR_H */
/* #undef HAVE_SYS_DIR_H */
/* #undef HAVE_NDIR_H */

/* C header files. */

/* #undef HAVE_WINDOWS_H */

#define HAVE_MEMORY_H 1
/* #undef HAVE_STROPTS_H */
#define HAVE_UNISTD_H 1
#define HAVE_SYS_RESOURCE_H 1
/* #undef HAVE_SYS_ELF_H */
/* #undef HAVE_SYS_SELECT_H */
#define HAVE_SYS_PARAM_H 1

/* Functions we do have. */

#define HAVE_GETCWD 1
/* #undef HAVE_GETDTABLESIZE */
#define HAVE_GETPID 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_NL_LANGINFO 1
/* #undef HAVE_POLL */
#define HAVE_SELECT 1
#define HAVE_SIGPROCMASK 1
#define HAVE_SIGACTION 1
#define HAVE_STRCOLL 1
#define HAVE_SYSCONF 1
#define HAVE_UNAME 1

/* Options that sysconf accepts. */

#define HAVE_SYSCONF_SC_OPEN_MAX 1

/* Types we do not have. */

/* #undef NEED_UID_T */
/* #undef NEED_PID_T */
/* #undef NEED_OFF_T */
/* #undef NEED_DEV_T */
/* #undef NEED_INO_T */
/* #undef NEED_U_CHAR */
/* #undef NEED_U_SHORT */
/* #undef NEED_U_INT */
/* #undef NEED_U_LONG */

/* Parts of the stat structure we have. */

#define HAVE_ST_BLKSIZE 1
#define HAVE_ST_BLOCKS 1
#define HAVE_ST_RDEV 1

/* ------------------------------------------------------------------------- */

#endif /* OSE_CONFIG_H */
