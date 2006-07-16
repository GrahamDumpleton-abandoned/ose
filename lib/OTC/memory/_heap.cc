/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/_heap.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1999 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/heap.hh>

#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_Tracer tracer("void test1()");

  char* theMemory = new char[1024];
  OTC_Heap* theHeap = new OTC_Heap(theMemory,1024);
  theHeap->dump(tracer());

  delete theHeap;
  delete [] theMemory;
}

void test2()
{
  OTC_Tracer tracer("void test2()");

  char* theMemory = new char[1024];
  OTC_Heap* theHeap = new OTC_Heap(theMemory,1024);

  theHeap->dump(tracer());

  void* o1 = theHeap->allocate(10);

  theHeap->dump(tracer());

  void* o2 = theHeap->allocate(20);

  theHeap->dump(tracer());

  theHeap->release(o1);

  theHeap->dump(tracer());

  theHeap->release(o2);

  theHeap->dump(tracer());

  delete theHeap;
  delete [] theMemory;
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2
};

/* ------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
  u_int const numTests = sizeof(tests)/sizeof(tests[0]);

  set_terminate(otclib_terminate_handler);

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
