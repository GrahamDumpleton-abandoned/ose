#ifndef OTC_COLLCTN_BNDSTACK_C
#define OTC_COLLCTN_BNDSTACK_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/bndstack.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1995-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T>
OTC_BoundedStack<T>::~OTC_BoundedStack()
{
  if (items_ && destroy_)
  {
    delete [] items_;
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_BoundedStack<T>::OTC_BoundedStack(u_int theCapacity, T* theMem)
  : capacity_(theCapacity), put_(0), items_(theMem), destroy_(OTCLIB_FALSE)
{
  if (theMem == 0 && capacity_ > 0)
  {
#if defined(ENV_OSTORE)
    OTC_Locality theLocality = OTC_Locality::of(this);
#endif

#if defined(ENV_OSTORE)
    items_ = new (theLocality,OTC_TypeSpec<T>::typespec(),capacity_)
     T[capacity_];
#else
    items_ = new T[capacity_];
#endif
    OTCLIB_ASSERT_M(items_ != 0);
    destroy_ = OTCLIB_TRUE;
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_BoundedStack<T>::OTC_BoundedStack(OTC_BoundedStack<T> const& theStack)
  : capacity_(theStack.capacity_), put_(0), items_(0),
    destroy_(OTCLIB_TRUE)
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

#if defined(ENV_OSTORE)
  items_ = new (theLocality,OTC_TypeSpec<T>::typespec(),capacity_)
   T[capacity_];
#else
  items_ = new T[capacity_];
#endif
  OTCLIB_ASSERT_M(items_ != 0);

  for (int i=theStack.count(); i>0; i--)
    push(theStack.peek(i-1));
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_BoundedStack<T>& OTC_BoundedStack<T>::operator=(
 OTC_BoundedStack<T> const& theStack
)
{
  if (&theStack == this)
    return *this;

  if (destroy_)
    delete [] items_;

  capacity_ = theStack.capacity_;
  put_ = 0;
  destroy_ = OTCLIB_TRUE;

#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

#if defined(ENV_OSTORE)
  items_ = new (theLocality,OTC_TypeSpec<T>::typespec(),capacity_)
   T[capacity_];
#else
  items_ = new T[capacity_];
#endif
  OTCLIB_ASSERT_M(items_ != 0);

  for (int i=theStack.count(); i>0; i--)
    push(theStack.peek(i-1));

  return *this;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_BNDSTACK_C */
