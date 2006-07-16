#ifndef OTC_STDLIB_ISTREAM_H
#define OTC_STDLIB_ISTREAM_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     stdlib/istream.h
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
#include <istream>
//using namespace std;
//using std::cin;
using std::ios;
using std::streambuf;
using std::istream;
using std::dec;
using std::ws;
#else
#include <istream.h>
#if defined(CXX_SUN5_0) && __SUNPRO_CC_COMPAT != 4
using std::ws;
#endif
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_STDLIB_ISTREAM_H */
