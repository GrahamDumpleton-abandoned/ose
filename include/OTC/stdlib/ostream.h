#ifndef OTC_STDLIB_OSTREAM_H
#define OTC_STDLIB_OSTREAM_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     stdlib/ostream.h
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
#include <ostream>
//using namespace std;
//using std::cout;
//using std::cerr;
//using std::clog;
using std::ios;
using std::streambuf;
using std::ostream;
using std::dec;
using std::endl;
using std::ends;
using std::flush;
#else
#include <ostream.h>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_STDLIB_OSTREAM_H */
