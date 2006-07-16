/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otcexcption.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1995-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/debug/excption.hh"
#endif
#endif

#include <OTC/debug/excption.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/logger.hh>

#include <OTC/stdlib/iostream.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_Exception::mutex_;
OTC_ThrowFunction OTC_Exception::globThrowFunction_ = 0;
OTC_Boolean OTC_Exception::globInitialised_ = OTCLIB_FALSE;
OTC_Boolean OTC_Exception::globLogException_ = OTCLIB_FALSE;
OTC_Boolean OTC_Exception::globStackTrace_ = OTCLIB_FALSE;

/* ------------------------------------------------------------------------- */
OTC_Exception::OTC_Exception(char const* theError)
  : error_(theError), file_(0), line_(0)
{
  if (error_ == 0)
    error_ = "Unknown error";
}

/* ------------------------------------------------------------------------- */
OTC_Exception::OTC_Exception(
 char const* theError,
 char const* theFile,
 u_int theLine
)
  : error_(theError), file_(theFile), line_(theLine)
{
  if (error_ == 0)
    error_ = "Unknown error";
}

/* ------------------------------------------------------------------------- */
OTC_Exception::OTC_Exception(OTC_Exception const& theException)
  : error_(theException.error_),
    file_(theException.file_),
    line_(theException.line_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Exception::~OTC_Exception()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_Exception::display(ostream& outs) const
{
  mutex_.lock();

  initialise();

  mutex_.unlock();

  outs << error_ << EOL;

  if (file_ != 0)
    outs << "Location: " << file_ << ", line " << line_ << EOL;

  if (globStackTrace_ != OTCLIB_FALSE)
    dumpStackTrace(outs);
}

/* ------------------------------------------------------------------------- */
void OTC_Exception::throwException(
 OTC_Exception& theException,
 char const* theFile,
 u_int theLine
)
{
  if (theException.file_ == 0)
  {
    theException.file_ = theFile;
    theException.line_ = theLine;
  }

  OTC_ThrowFunction theFunction;
  OTC_Boolean theLogException;

  mutex_.lock();

  initialise();

  theFunction = globThrowFunction_;

#if defined(HAVE_EXCEPTIONS) && !defined(ENV_NOTHROWEXCEPTION)
  theLogException = globLogException_;
#else
  theLogException = OTCLIB_TRUE;
#endif

  mutex_.unlock();

  if (theFunction)
    theFunction(theException);

  if (theLogException != OTCLIB_FALSE)
  {
    char buf[8192];
    OTC_LogStream outs(buf,sizeof(buf));
    outs.setLevel(OTCLIB_LOG_ERROR);
    theException.display(outs);
    outs.flush();
  }

#if defined(HAVE_EXCEPTIONS) && !defined(ENV_NOTHROWEXCEPTION)
  theException.throwIt();
#else
  OTCLIB_TERMINATE();
#endif
}

/* ------------------------------------------------------------------------- */
OTC_ThrowFunction OTC_Exception::setThrow(OTC_ThrowFunction theFunction)
{
  OTC_ThrowFunction oldFunction;

  mutex_.lock();

  oldFunction = globThrowFunction_;
  globThrowFunction_ = theFunction;

  mutex_.unlock();

  return oldFunction;
}

/* ------------------------------------------------------------------------- */
void OTC_Exception::throwIt() const
{
#if defined(HAVE_EXCEPTIONS) && !defined(ENV_NOTHROWEXCEPTION)
  throw *this;
#endif
}

/* ------------------------------------------------------------------------- */
#if !defined(OTCLIB_NOSTACKTRACE)
#if defined(sun) || defined(__sun)
#if defined(HAVE_SYS_RESOURCE_H) && defined(HAVE_SYS_ELF_H)

// This is Solaris specific code. It came
// from somewhere off the net but I don't
// know where. It has been modified a fair
// bit to fit into the context it is being
// use. This stuff will generate UMR errors
// in Purify as it goes.

// Walks up call stack, printing
// library:routine+offset for each routine

#include <dlfcn.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/reg.h>
#include <sys/frame.h>

#include <stdio.h>

#if defined(sparc) || defined(__sparc)
#define FLUSHWIN() asm("ta 3");
#define FRAME_PTR_INDEX 1
#define SKIP_FRAMES 0
#endif

#if defined(i386) || defined(__i386)
#define FLUSHWIN() 
#define FRAME_PTR_INDEX 3
#define SKIP_FRAMES 1
#endif

#if defined(ppc) || defined(__ppc)
#define FLUSHWIN() 
#define FRAME_PTR_INDEX 0
#define SKIP_FRAMES 2
#endif

// This function walks up call stack, calling
// user-supplied function once for each stack
// frame, passing the pc and the
// user-supplied usrarg as the argument.

static int print_frame(ostream& strm, int& depth, void *pc)
{
  Dl_info info;
  char const* func;
  char const* lib;

  if (dladdr(pc,&info) == 0)
  {
    func = "???";
    lib = "???";
  }
  else
  {
    lib = (char const*)info.dli_fname;
    func = (char const*)info.dli_sname;
  }

  if (depth >= 4)
  {
    strm << "Location: (" << depth-4 << ") " << func << " + " << hex;
    strm << "0x" << (unsigned int)pc-(unsigned int)info.dli_saddr;
    strm << dec << " [" << lib << "]" << EOL;
  }

  depth++;

  return (1);
}

static int scan_stack(ostream& strm, int& depth)
{
  struct frame *sp;
  jmp_buf env;
  int i;
  int* iptr;

  FLUSHWIN();

  setjmp(env);
  iptr = (int*)env;

  sp = (struct frame*)iptr[FRAME_PTR_INDEX];

  for(i=0; i<SKIP_FRAMES && sp; i++)
    sp = sp->fr_savfp;

  i = 0;

  while(sp && sp->fr_savpc && ++i &&
   print_frame(strm,depth,(void*)sp->fr_savpc))
  {
    sp = sp->fr_savfp;
  }

  return (i);
}

void OTC_Exception::dumpStackTrace(ostream& theStream)
{
  int theDepth = 0;
  scan_stack(theStream,theDepth);
}

#define HAVE_DUMPSTACKTRACE 1

#endif
#endif
#endif

/* ------------------------------------------------------------------------- */
#if !defined(HAVE_DUMPSTACKTRACE)
void OTC_Exception::dumpStackTrace(ostream&)
{
  // Don't know how to do it.
}
#endif

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_Exception::loggingEnabled()
{
  OTC_Boolean theResult;

  mutex_.lock();

  initialise();

  theResult = globLogException_;

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_Exception::enableLogging()
{
  mutex_.lock();

  initialise();

  globLogException_ = OTCLIB_TRUE;

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Exception::disableLogging()
{
  mutex_.lock();

  initialise();

  globLogException_ = OTCLIB_FALSE;

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Exception::initialise()
{
  if (globInitialised_ == OTCLIB_FALSE)
  {
    globInitialised_ = OTCLIB_TRUE;

    char const* theVariable;
    theVariable = getenv("OTCLIB_LOGEXCEPTION");
    if (theVariable != 0)
      globLogException_ = OTCLIB_TRUE;

    theVariable = getenv("OTCLIB_EXCEPTIONSTACKTRACE");
    if (theVariable != 0)
      globStackTrace_ = OTCLIB_TRUE;
  }
}

/* ------------------------------------------------------------------------- */
