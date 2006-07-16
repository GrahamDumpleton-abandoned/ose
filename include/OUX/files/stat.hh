#ifndef OUX_FILES_STAT_HH
#define OUX_FILES_STAT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OUX
// 
// = FILENAME
//     files/stat.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1996 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/system/stat.hh>

/* ------------------------------------------------------------------------- */

// The OUX_Stat class has been made obsolete. All functions
// of the OUX_Stat class have been pushed into the OTC_Stat
// base class, with the functions only being available when
// a UNIX system is being used which supports hat it is they
// do.

typedef OTC_Stat OUX_Stat;

/* ------------------------------------------------------------------------- */

#endif /* OUX_FILES_STAT_HH */
