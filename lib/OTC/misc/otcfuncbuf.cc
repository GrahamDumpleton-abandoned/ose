/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     misc/otcfuncbuf.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/misc/funcbuf.hh"
#endif
#endif

#include <OTC/misc/funcbuf.hh>

#ifndef zapeof
#define zapeof(c) ((c)&0377)
#endif

#if defined(ENV_OSTORE) && defined(CXX_DEC) && defined(__alpha)
#pragma __environment save
#pragma __environment header_defaults
#endif

/* ------------------------------------------------------------------------- */
OTC_FuncBuf::~OTC_FuncBuf()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_FuncBuf::OTC_FuncBuf(
 void (*theFunc)(char const*,u_int),
 char* theBuffer,
 u_int theSize
)
  : func_(theFunc), bufType_(OTCLIB_BUFFERED)
{
  if (theBuffer != 0 && theSize > (int)sizeof(buffer_))
    setp(theBuffer,theBuffer+theSize-2);
  else
    setp(buffer_,buffer_+sizeof(buffer_)-2);
}

/* ------------------------------------------------------------------------- */
OTC_FuncBuf::OTC_FuncBuf(
 void (*theFunc)(char const*,u_int),
 OTC_BufferingFlag theType
)
  : func_(theFunc), bufType_(theType)
{
  if (theType == OTCLIB_BUFFERED)
    setp(buffer_,buffer_+sizeof(buffer_)-2);
}

/* ------------------------------------------------------------------------- */
#if defined(ENV_CXXSTDLIB)
OTC_FuncBuf::int_type OTC_FuncBuf::overflow(int_type c)
#else
int OTC_FuncBuf::overflow(int c)
#endif
{
  if (bufType_ == OTCLIB_UNBUFFERED)
  {
#if defined(ENV_CXXSTDLIB)
    if (traits_type::eq_int_type(c,traits_type::eof()))
      return traits_type::eof();
#else
    if (c == EOF)
      return EOF;
#endif

    char b = char(c);
    (*func_)(&b,1);

#if defined(ENV_CXXSTDLIB)
    return traits_type::not_eof(c);
#else
    return zapeof(c);
#endif
  }
  else
  {
    char* endp = pptr();

    *endp = '\0';
    *(endp+1) = '\0';

#if defined(ENV_CXXSTDLIB)
    if (!traits_type::eq_int_type(c,traits_type::eof()) && c != '\0')
#else
    if (c != EOF && c != '\0')
#endif
      *endp++ = char(c);

    (*func_)(pbase(),endp-pbase());

    setp(pbase(),epptr());

#if defined(ENV_CXXSTDLIB)
    return traits_type::not_eof(c);
#else
    return zapeof(c);
#endif
  }
}

/* ------------------------------------------------------------------------- */
#if defined(ENV_CXXSTDLIB)
OTC_FuncBuf::int_type OTC_FuncBuf::underflow()
#else
int OTC_FuncBuf::underflow()
#endif
{
  return 0;
}

/* ------------------------------------------------------------------------- */
int OTC_FuncBuf::sync()
{
  if (pptr() && pptr() > pbase())
  {
    // LIBG++ does check explicitly for -1, so must make sure
    // we return 0 if all is okay.

#if defined(ENV_CXXSTDLIB)
    int_type c = overflow(traits_type::eof());
    if (traits_type::eq_int_type(c,traits_type::eof()))
      return -1;
    else
      return 0;
#else
    int c = overflow(EOF);
    return c == -1 ? -1 : 0;
#endif
  }

  return 0;
}

#if defined(ENV_OSTORE) && defined(CXX_DEC) && defined(__alpha)
#pragma __environment restore
#endif

/* ------------------------------------------------------------------------- */
