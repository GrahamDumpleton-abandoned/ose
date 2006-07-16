/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     ostore/otcstorage.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/ostore/storage.hh>

#if defined(ENV_OSTORE)

/* ------------------------------------------------------------------------- */
OTC_Storage* OTC_Storage::first_ = 0;

/* ------------------------------------------------------------------------- */
OTC_Storage::~OTC_Storage()
{
  if (prev_ != 0)
    prev_->next_ = next_;
  if (next_ != 0)
    next_->prev_ = prev_;
  if (first_ == this)
    first_ = next_;
}

/* ------------------------------------------------------------------------- */
OTC_Storage::OTC_Storage()
{
  prev_ = 0;

  if (first_ == 0)
  {
    first_ = this;
    next_ = 0;
  }
  else
  {
    next_ = first_;
    first_->prev_ = this;
    first_ = this;
  }
}

/* ------------------------------------------------------------------------- */

#endif
