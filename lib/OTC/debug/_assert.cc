/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     debug/_assert.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1996-1999 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#undef NDEBUG

#include <OTC/debug/tracer.hh>
#include <OTC/debug/asrtfail.hh>
#include <OTC/OTC.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

// = TEST 1
//     Test failure of assertion.

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_Exception::enableLogging();

  OTCLIB_ASSERT(OTCLIB_TRUE == OTCLIB_FALSE);

  tracer() << "XXX - Shouldn't reach here" << endl;
}

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Exception::enableLogging();

  OTCLIB_ASSERT_W(OTCLIB_TRUE == OTCLIB_FALSE);
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Exception::enableLogging();

  char* theMemory = 0;
  OTCLIB_ASSERT_M(theMemory != 0);
}

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_Exception::enableLogging();

#if defined(HAVE_EXCEPTIONS) && !defined(ENV_NOTHROWEXCEPTION)
  try
  {
    tracer() << "try" << endl;
    OTCLIB_ASSERT(OTCLIB_TRUE == OTCLIB_FALSE);
  }
  catch (OTCERR_AssertionFailure& theException2)
  {
    tracer() << "catch OTCERR_AssertionFailure" << endl;
    theException2.display(tracer());
    tracer() << flush;
  }
  catch (OTC_Exception& theException1)
  {
    tracer() << "catch OTC_Exception" << endl;
    theException1.display(tracer());
    tracer() << flush;
  }
#endif
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3,
  test4
};

/* ------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
  u_int const numTests = sizeof(tests)/sizeof(tests[0]);

  set_terminate(otclib_terminate_function);

  if (argc != 2)
  {
    cout << numTests << endl;
    return 1;
  }
  else
  {
    int testNum = atoi(argv[1]);
    if (testNum > 0 && u_int(testNum) <= numTests)
    {
      tests[testNum-1]();
      return 0;
    }
    else
      return 1;
  }
}

/* ------------------------------------------------------------------------- */
