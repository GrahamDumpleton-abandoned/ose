#ifndef OTC_STDLIB_STRSTREAM_H
#define OTC_STDLIB_STRSTREAM_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     stdlib/strstream.h
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OSE_OSE_H
#include <OSE/OSE.h>
#endif

/* ------------------------------------------------------------------------- */

#if defined(ENV_CXXSTDLIB)
#include <strstream>
//using namespace std;
using std::istrstream;
using std::ostrstream;
using std::strstream;
#else
#include <strstream.h>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_STDLIB_STRSTREAM_H */
