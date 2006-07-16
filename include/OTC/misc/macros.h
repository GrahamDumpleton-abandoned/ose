#ifndef OTC_MISC_MACROS_H
#define OTC_MISC_MACROS_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     misc/macros.h
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1999-2000 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <stdio.h>

/* ------------------------------------------------------------------------- */

/* Force this header into dependencies. */
extern int OTCLIB_HEADERDUMMY;

/*
// Useful constants. We include <stdio.h> here as most systems define EOF
// in that file. We only want to define it if they do not.
*/

#ifdef EOF
#if EOF != -1
#define EOF (-1)
#endif
#else
#define EOF (-1)
#endif

#ifndef EOL
#define EOL '\n'
#endif

#ifndef EOS
#define EOS '\0'
#endif

/*
// Check for ANSI preprocessor. Define single symbol __STDCPP__ so
// we only have to check for one symbol elsewhere.
*/

/*
#ifndef __STDCPP__
#if defined(__STDC__) || defined(__ANSI_CPP) || defined(__ANSI_CPP__) || \
 defined(CENTERLINE_CLPP) || defined(__LUCID_ANSI_PP__) || defined(_MSC_VER)
#define __STDCPP__ 1
#endif
#endif
*/

/* ------------------------------------------------------------------------ */

#endif /* OTC_MISC_MACROS_H */
