/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     misc/otcvfuncbuf.cc
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
#pragma implementation "OTC/misc/vfuncbuf.hh"
#endif
#endif

#include <OTC/misc/vfuncbuf.hh>

#ifndef zapeof
#define zapeof(c) ((c)&0377)
#endif

#if defined(ENV_OSTORE) && defined(CXX_DEC) && defined(__alpha)
#pragma __environment save
#pragma __environment header_defaults
#endif

/* ------------------------------------------------------------------------- */
OTC_VFuncBuf::~OTC_VFuncBuf()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_VFuncBuf::OTC_VFuncBuf(char* theBuffer, u_int theSize)
  : bufType_(OTCLIB_BUFFERED)
{
  if (theBuffer != 0 && theSize > (int)sizeof(buffer_))
    setp(theBuffer,theBuffer+theSize-2);
  else
    setp(buffer_,buffer_+sizeof(buffer_)-2);
}

/* ------------------------------------------------------------------------- */
OTC_VFuncBuf::OTC_VFuncBuf(OTC_BufferingFlag theType)
  : bufType_(theType)
{
  if (theType == OTCLIB_BUFFERED)
    setp(buffer_,buffer_+sizeof(buffer_)-2);
}

/* ------------------------------------------------------------------------- */
#if defined(ENV_CXXSTDLIB)
OTC_VFuncBuf::int_type OTC_VFuncBuf::overflow(int_type c)
#else
int OTC_VFuncBuf::overflow(int c)
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
    handle(&b,1);

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

    handle(pbase(),endp-pbase());

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
OTC_VFuncBuf::int_type OTC_VFuncBuf::underflow()
#else
int OTC_VFuncBuf::underflow()
#endif
{
  return 0;
}

/* ------------------------------------------------------------------------- */
int OTC_VFuncBuf::sync()
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
