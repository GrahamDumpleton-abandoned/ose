#ifndef OTC_STDLIB_IOSTREAM_H
#define OTC_STDLIB_IOSTREAM_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     stdlib/iostream.h
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
#include <iostream>
//using namespace std;
using std::cin;
using std::cout;
using std::cerr;
using std::clog;
using std::ios;
using std::streambuf;
using std::iostream;
using std::ostream;
using std::istream;
using std::dec;
using std::hex;
using std::endl;
using std::ends;
using std::flush;
using std::ws;
#else
#include <iostream.h>
#if defined(CXX_SUN5_0) && __SUNPRO_CC_COMPAT != 4
using std::ws;
#endif
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_STDLIB_IOSTREAM_H */
