/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otctrcebuf.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1997-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/debug/trcebuf.hh"
#endif
#endif

#include <OTC/debug/trcebuf.hh>
#include <OTC/debug/trcestrm.hh>
#include <OTC/debug/tracer.hh>

#include <string.h>

#ifndef zapeof
#define zapeof(c) ((c)&0377)
#endif

#if defined(ENV_OSTORE) && defined(CXX_DEC) && defined(__alpha)
#pragma __environment save
#pragma __environment header_defaults
#endif

/* ------------------------------------------------------------------------- */
OTC_TraceBuf::~OTC_TraceBuf()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_TraceBuf::OTC_TraceBuf(OTC_TraceStream* theStream)
  : stream_(theStream), startOfLine_(OTCLIB_TRUE)
{
  OTCLIB_ASSERT(theStream != 0);

  setp(buffer_,buffer_+sizeof(buffer_)-2);
}

/* ------------------------------------------------------------------------- */
int OTC_TraceBuf::sync()
{
  if (pptr() && pptr() > pbase())
  {
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

/* ------------------------------------------------------------------------- */
#if defined(ENV_CXXSTDLIB)
OTC_TraceBuf::int_type OTC_TraceBuf::overflow(int_type c)
#else
int OTC_TraceBuf::overflow(int c)
#endif
{
  char* endp = pptr();

#if defined(ENV_CXXSTDLIB)
  if (!traits_type::eq_int_type(c,traits_type::eof()))
#else
  if (c != EOF)
#endif
  {
    *endp = char(c);
    endp++;
  }

  char* startl = pbase();
  char* p = startl;

  ostream* outs = OTC_Tracer::stream();
  if (outs == 0)
    outs = &clog;

  while (p != endp)
  {
    while (p != endp && *p != EOL)
      p++;

    if (startOfLine_ != OTCLIB_FALSE)
    {
      if (OTC_Tracer::traceIndent() != OTCLIB_FALSE)
      {
	for (long i = stream_->level(); i>0; i--)
	  outs->rdbuf()->sputc(' ');
      }
    }

    if (p == endp)
      outs->rdbuf()->sputn(startl,p-startl);
    else
      outs->rdbuf()->sputn(startl,p-startl+1);

#if !defined(ENV_CXXSTDLIB)
    outs->rdbuf()->sync();
#else
    outs->rdbuf()->pubsync();
#endif

    startl = p+1;

    if (p != endp)
    {
      if (*p == EOL)
	startOfLine_ = OTCLIB_TRUE;
      else
	startOfLine_ = OTCLIB_FALSE;
      p++;
    }
    else
      startOfLine_ = OTCLIB_FALSE;
  }

  setp(pbase(),epptr());

#if defined(ENV_CXXSTDLIB)
  return traits_type::not_eof(c);
#else
  return zapeof(c);
#endif
}

/* ------------------------------------------------------------------------- */
#if defined(ENV_CXXSTDLIB)
OTC_TraceBuf::int_type OTC_TraceBuf::underflow()
#else
int OTC_TraceBuf::underflow()
#endif
{
  return 0;
}

#if defined(ENV_OSTORE) && defined(CXX_DEC) && defined(__alpha)
#pragma __environment restore
#endif

/* ------------------------------------------------------------------------- */
