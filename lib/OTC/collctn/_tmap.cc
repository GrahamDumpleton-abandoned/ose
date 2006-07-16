/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/_tmap.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1997-2000 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/tmap.hh>
#include <OTC/debug/tracer.hh>

#include <OTC/stdlib/fstream.h>

#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------------- */

// = TEST 2
//      Test properties of an empty map.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_TMap<int,int> map;

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  tracer() << "map.removeAll()" << endl;
  map.removeAll();

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  OTC_Iterator<int> keys = 0;
  keys = map.keys();
  tracer() << "keys.isValid() = " << keys.isValid() << endl;

  OTC_Iterator<int> items = 0;
  items = map.items();
  tracer() << "items.isValid() = " << items.isValid() << endl;
}

// = TEST 3
//     Test of addition. Use iterator to check.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_TMap<int,int> map;

  for (int i=0; i<100; i++)
    map.add(i,i);

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  OTC_Iterator<int> keys = 0;
  keys = map.keys();
  tracer() << "map.keys() =" << flush;
  for (keys.reset(); keys.isValid(); keys.next())
    tracer() << " " << keys.item() << flush;
  tracer() << endl;

  OTC_Iterator<int> items = 0;
  items = map.items();
  tracer() << "map.items() =" << flush;
  for (items.reset(); items.isValid(); items.next())
    tracer() << " " << items.item() << flush;
  tracer() << endl;
}

// = TEST 4
//     Test of copy constructor and assignment operator.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_TMap<int,int> map1;

  for (int i=0; i<100; i++)
    map1.add(i,i);

  tracer() << "map1.population() = " << map1.population() << endl;
  tracer() << "map1.isEmpty() = " << map1.isEmpty() << endl;

  tracer() << "map2(map1)" << endl;
  OTC_TMap<int,int> map2(map1);

  tracer() << "map1.population() = " << map1.population() << endl;
  tracer() << "map1.isEmpty() = " << map1.isEmpty() << endl;

  tracer() << "map2.population() = " << map2.population() << endl;
  tracer() << "map2.isEmpty() = " << map2.isEmpty() << endl;

  tracer() << "map2 = map2" << endl;
  map2 = map2;

  tracer() << "map2.population() = " << map2.population() << endl;
  tracer() << "map2.isEmpty() = " << map2.isEmpty() << endl;

  tracer() << "map2.removeAll()" << endl;
  map2.removeAll();

  tracer() << "map2.population() = " << map2.population() << endl;
  tracer() << "map2.isEmpty() = " << map2.isEmpty() << endl;

  tracer() << "map2 = map1" << endl;
  map2 = map1;

  tracer() << "map2.population() = " << map2.population() << endl;
  tracer() << "map2.isEmpty() = " << map2.isEmpty() << endl;
}

// = TEST 5
//     Test of contains and item.

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_TMap<int,int> map;

  for (int i=0; i<100; i++)
    map.add(i,i);

  OTC_Iterator<int> iter = 0;
  iter = map.keys();
  tracer() << "map items =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
  {
    int value = iter.item();
    if (!map.contains(value))
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << map.item(value) << flush;
    value = -value - 1;
    if (map.contains(value))
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;
}

// = TEST 6
//     Test of explicit removal.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_TMap<int,int> map;

  for (int i=0; i<100; i++)
    map.add(i,i);

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  OTC_Iterator<int> keys = 0;
  keys = map.keys();
  for (keys.reset(); keys.isValid(); keys.next())
  {
    int value = keys.item();
    map.remove(value);
  }

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  keys = map.keys();
  tracer() << "map.keys() =" << flush;
  for (keys.reset(); keys.isValid(); keys.next())
    tracer() << " " << keys.item() << flush;
  tracer() << endl;

  OTC_Iterator<int> items = 0;
  items = map.items();
  tracer() << "map.items() =" << flush;
  for (items.reset(); items.isValid(); items.next())
    tracer() << " " << items.item() << flush;
  tracer() << endl;
}

// = TEST 7
//     Test of large map.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_TMap<int,int> map;

  for (int i=0; i<2500; i++)
    map.add(i,i);

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  tracer() << "map.removeAll()" << endl;
  map.removeAll();

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;
}

// = TEST 8
//     Test of pairs().
 
void test8()
{
  OTC_Tracer tracer("void test8()");
 
  OTC_TMap<int,double> map;
 
  for (int i=0; i<100; i++)
  {
    double j = (double)i / 2;
    map.add(i,j);
  }
 
  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;
 
  OTC_PairIterator<int,double> pairs = 0;
  pairs = map.pairs();
  tracer() << "map.pairs() =" << flush;
  for (pairs.reset(); pairs.isValid(); pairs.next())
    tracer() << " (" << pairs.key() << "," << pairs.item() << ")" << flush;
  tracer() << endl;
}

// = TEST 9
//     Test of replacement.

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_TMap<int,int> map;

  int i;

  for (i=0; i<10; i++)
    map.add(i,i,OTCLIB_REPLACE);

  OTC_PairIterator<int,int> iter;

  iter = map.pairs();
  tracer() << "map.pairs() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " (" << iter.key() << ":" << iter.item() << ")" << flush;
  tracer() << endl;

  for (i=9; i>=0; i--)
  {
    int j = i*i;
    map.add(i,j,OTCLIB_REPLACE);
  }

  iter = map.pairs();
  tracer() << "map.pairs() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " (" << iter.key() << ":" << iter.item() << ")" << flush;
  tracer() << endl;
}

// = TEST 10
//     Test of items with supplied rank function.

int test10_rank(char const* const& s1, char const* const& s2)
{
  int l1 = strlen(s1);
  int l2 = strlen(s2);

  if (l1 <= l2)
    return OTC_Rank::rank(s1,l1,s2,l1);

  return OTC_Rank::rank(s1,l1,s2,l2);
}

void test10()
{
  OTC_Tracer tracer("void test10()");

  char buf1[1024];
  ifstream in("/usr/dict/words");

  OTC_TMap<char const*,int> map;

  int i = 0;
  while (in.good())
  {
    in >> buf1;
    if (!in.fail())
    {
      if (!map.contains(buf1))
      {
	i++;
        char* buf2;
        buf2 = new char[strlen(buf1)+1];
        strcpy(buf2,buf1);
        map.add(buf2,i);
      }
    }
  }

  cout << "loaded" << endl;
  cout << "> " << flush;

  OTC_Iterator<char const*> keys;
  OTC_Iterator<int> items;
  OTC_PairIterator<char const*,int> pairs;

  while (cin.good())
  {
    cin >> buf1;
    if (!cin.fail())
    {
      keys = map.keys((char const*)buf1,test10_rank);
      while (keys.isValid())
      {
        tracer() << keys.item() << endl;
        keys.next();
      }
      items = map.items((char const*)buf1,test10_rank);
      while (items.isValid())
      {
        tracer() << items.item() << endl;
        items.next();
      }
      pairs = map.pairsKey((char const*)buf1,test10_rank);
      while (pairs.isValid())
      {
        tracer() << pairs.key() << " " << pairs.item() << endl;
        pairs.next();
      }
      cout << "> " << flush;
    }
  }

  keys = map.keys();
  while (keys.isValid())
  {
    delete [] (char*)keys.item();
    keys.next();
  }
}

// = TEST 1
//     All tests.

void test1()
{
  OTC_Tracer tracer("void test1()");

  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
}

/* -------------------------------------------------------------------------- */

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3,
  test4,
  test5,
  test6,
  test7,
  test8,
  test9,
  test10
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
