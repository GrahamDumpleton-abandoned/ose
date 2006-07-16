/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     ostore/typespec.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T>
os_typespec* OTC_TypeSpec<T>::typespec()
{
  return T::get_os_typespec();
}

/* ------------------------------------------------------------------------- */
