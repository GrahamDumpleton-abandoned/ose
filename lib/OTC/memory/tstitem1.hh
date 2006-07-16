#ifndef OTC_MEMORY_TSTITEM1_HH
#define OTC_MEMORY_TSTITEM1_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/tstitem1.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */

class TST_Item1
{
  public:

#if defined(ENV_OSTORE) && !defined(SCHEMA_GENERATION)
    static os_typespec* typespec();
    static os_typespec* get_os_typespec() { return typespec(); }
#endif

			TST_Item1() : value_(0) {}

			operator int() const
				{ return value_; }

    TST_Item1&		operator=(int theValue)
				{ value_ = theValue; return *this; }

  private:

    int			value_;
};

#if defined(ENV_OSTORE)
os_typespec* TST_Item1::typespec()
{
  static os_typespec ts("TST_Item1");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_TSTITEM1_HH */
