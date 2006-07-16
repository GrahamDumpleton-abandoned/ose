#ifndef OTC_STDLIB_IOMANIP_H
#define OTC_STDLIB_IOMANIP_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     stdlib/iomanip.h
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
#include <iomanip>
//using namespace std;
using std::skipws;
using std::setw;
#else
#include <iomanip.h>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_STDLIB_IOMANIP_H */
