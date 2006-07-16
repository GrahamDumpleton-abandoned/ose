#ifndef OTC_MISC_TERMNATE_HH
#define OTC_MISC_TERMNATE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     misc/termnate.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1995-2000 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */

#if defined(HAVE_EXCEPTIONS) || !defined(ENV_VERSANT)

#if defined(HAVE_EXCEPTIONS)

#if defined(ENV_CXXSTDLIB)

#include <exception>

#if !defined(_MSC_VER) || (_MSC_VER > 1200)
using std::terminate;
using std::set_terminate;
#endif

#else

#if defined(CXX_EGCS)
#include <exception>
#endif
#if defined(CXX_HP)
extern void terminate();
#include <exception.h>
#endif
#if defined(CXX_XL)
#include <terminate.h>
#endif
#if defined(CXX_KAI)
#include <exception.h>
#if defined(CXX_KAI2_0) || defined(CXX_KAI2_01) || defined(CXX_KAI2_02) || \
    defined(CXX_KAI2_03) || defined(CXX_KAI2_04)
extern void terminate();
#endif
#endif
#if defined(CXX_SUN) || defined(CXX_SG)
#include <exception.h>
#endif
#if defined(CXX_DEC)
#include <cxx_exception.h>
#endif
#ifdef __BORLANDC__
#include <except.h>
#endif
#ifdef _MSC_VER
#include <eh.h>
#endif
#ifdef __WATCOM_CPLUSPLUS__
#include <except.h>
#endif
#if defined(ENV_VERSANT)
#include <cxxcls/perror.h>
#endif

#endif

#else

// These will be defined in C++ when exception handling is introduced. To
// provide future compatibility define our own now if the compiler does not
// support exceptions.

#define NEED_TERMINATE 1

typedef void(*PFV)();

extern void terminate();
extern PFV set_terminate(PFV);

#endif

#else

// Versant defines it own version of the set_terminate() function which has
// a non standard prototype. Include their header file to get the prototype.

#include <cxxcls/try.h>

#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_MISC_TERMNATE_HH */
