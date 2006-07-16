/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     ostore/otctypespec.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1994-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/ostore/typespec.hh>

#if defined(ENV_OSTORE)

/* ------------------------------------------------------------------------- */

os_typespec* otclib_get_charP()
{
  static os_typespec* typeSpec_ = 0;
  if (!typeSpec_)
    typeSpec_ = new os_typespec("char*");
  return typeSpec_;
}

os_typespec* otclib_get_shortP()
{
  static os_typespec* typeSpec_ = 0;
  if (!typeSpec_)
    typeSpec_ = new os_typespec("short*");
  return typeSpec_;
}

os_typespec* otclib_get_intP()
{
  static os_typespec* typeSpec_ = 0;
  if (!typeSpec_)
    typeSpec_ = new os_typespec("int*");
  return typeSpec_;
}

os_typespec* otclib_get_longP()
{
  static os_typespec* typeSpec_ = 0;
  if (!typeSpec_)
    typeSpec_ = new os_typespec("long*");
  return typeSpec_;
}

os_typespec* otclib_get_floatP()
{
  static os_typespec* typeSpec_ = 0;
  if (!typeSpec_)
    typeSpec_ = new os_typespec("float*");
  return typeSpec_;
}

os_typespec* otclib_get_doubleP()
{
  static os_typespec* typeSpec_ = 0;
  if (!typeSpec_)
    typeSpec_ = new os_typespec("double*");
  return typeSpec_;
}

/* ------------------------------------------------------------------------- */

#endif
