#ifndef OTC_OSTORE_TYPESPEC_HH
#define OTC_OSTORE_TYPESPEC_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     ostore/typespec.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OTC_OSTORE_OSTORE_HH
#include <OTC/ostore/ostore.hh>
#endif

#if defined(ENV_OSTORE)

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_TypeSpec
    // = TITLE
    //     Class to aid in the creation of typespec objects for ObjectStore.
    //
    // = AUDIENCE
    //     This class is specifically related to the use of the C++ library
    //     interface for ObjectStore, ie. without DML extensions.
    //
    // = DESCRIPTION
    //     This class provides a convenient handle for creating and storing
    //     of typespec objects for ObjectStore. The class only provides a
    //     class declaration, it does not provide any implementation. It is
    //     the users responsibility to provide an implementation.
    //
    //     This header file will automatically be included by <OTC/OTC.h>
    //     when working in the <{OSTORE}> compiler environment.
{
  public:

    static os_typespec*	typespec();
				// Returns a typespec object for type <T>.
};

#if defined(EXPAND_TEMPLATES)
#include <OTC/ostore/typespec.c>
#endif

#if defined(CXX_SUN3_0)
class OTC_TSDummy { public: static os_typespec* get_os_typespec() {return 0;}};
typedef OTC_TypeSpec<OTC_TSDummy> otc_ostore_typespec_hh_typedef1;
#endif

extern os_typespec* otclib_get_charP();
extern os_typespec* otclib_get_shortP();
extern os_typespec* otclib_get_intP();
extern os_typespec* otclib_get_longP();
extern os_typespec* otclib_get_floatP();
extern os_typespec* otclib_get_doubleP();

#define OTCLIB_TYPESPEC(T,F) \
class OTC_TypeSpec<T> { public: static os_typespec* typespec() { return F(); }}

OTCLIB_TYPESPEC(char,os_typespec::get_char);
OTCLIB_TYPESPEC(short,os_typespec::get_short);
OTCLIB_TYPESPEC(int,os_typespec::get_int);
OTCLIB_TYPESPEC(long,os_typespec::get_long);
OTCLIB_TYPESPEC(float,os_typespec::get_float);
OTCLIB_TYPESPEC(double,os_typespec::get_double);
OTCLIB_TYPESPEC(unsigned char,os_typespec::get_unsigned_char);
OTCLIB_TYPESPEC(unsigned short,os_typespec::get_unsigned_short);
OTCLIB_TYPESPEC(unsigned int,os_typespec::get_unsigned_int);
OTCLIB_TYPESPEC(unsigned long,os_typespec::get_unsigned_long);
OTCLIB_TYPESPEC(void*,os_typespec::get_pointer);
OTCLIB_TYPESPEC(void const*,os_typespec::get_pointer);
OTCLIB_TYPESPEC(char*,otclib_get_charP);
OTCLIB_TYPESPEC(short*,otclib_get_shortP);
OTCLIB_TYPESPEC(int*,otclib_get_intP);
OTCLIB_TYPESPEC(long*,otclib_get_longP);
OTCLIB_TYPESPEC(float*,otclib_get_floatP);
OTCLIB_TYPESPEC(double*,otclib_get_doubleP);
OTCLIB_TYPESPEC(char const*,otclib_get_charP);
OTCLIB_TYPESPEC(short const*,otclib_get_shortP);
OTCLIB_TYPESPEC(int const*,otclib_get_intP);
OTCLIB_TYPESPEC(long const*,otclib_get_longP);
OTCLIB_TYPESPEC(float const*,otclib_get_floatP);
OTCLIB_TYPESPEC(double const*,otclib_get_doubleP);

/* ------------------------------------------------------------------------- */

#endif

#endif /* OTC_OSTORE_TYPESPEC_HH */
