/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/otclinklist.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/linklist.hh"
#endif
#endif

#include <OTC/collctn/linklist.hh>

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_LinkList::typespec()
{
  static os_typespec ts("OTC_LinkList");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_LinkList::OTC_LinkList()
{
#if defined(ENV_OSTORE)
  OTC_Locality theLocality = OTC_Locality::of(this);
#endif

#if defined(ENV_OSTORE)
  start_ = new (theLocality,OTC_Anchor::get_os_typespec()) OTC_Anchor;
#else
  start_ = new OTC_Anchor;
#endif
  OTCLIB_ASSERT_M(start_ != 0);

#if defined(ENV_OSTORE)
  end_ = new (theLocality,OTC_Anchor::get_os_typespec()) OTC_Anchor;
#else
  end_ = new OTC_Anchor;
#endif
  OTCLIB_ASSERT_M(end_ != 0);

  start_->addAfter(end_);
}

/* ------------------------------------------------------------------------- */
OTC_LinkList::~OTC_LinkList()
{
  removeAll();
  start_->kill();
  end_->kill();
}

/* ------------------------------------------------------------------------- */
void OTC_LinkList::removeAll()
{
  OTC_Linkable* theLink = start_->next();
  while (theLink != end_)
  {
    OTCLIB_ASSERT(theLink != 0);

    OTC_Linkable* nextLink = theLink->next();
    if (!theLink->isDead())
      theLink->kill();
    theLink = nextLink;
  }
}

/* ------------------------------------------------------------------------- */
OTC_Link* OTC_LinkList::first() const
{
  OTC_Linkable* theLink = start_->next();
  while (theLink != (OTC_Linkable*)end_)
  {
    OTCLIB_ASSERT(theLink != 0);

    if (theLink->isLink() && !theLink->isDead())
      return (OTC_Link*)theLink;

    theLink = theLink->next();
  }
  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_Link* OTC_LinkList::last() const
{
  OTC_Linkable* theLink = end_->prev();
  while (theLink != (OTC_Linkable*)start_)
  {
    OTCLIB_ASSERT(theLink != 0);

    if (theLink->isLink() && !theLink->isDead())
      return (OTC_Link*)theLink;

    theLink = theLink->prev();
  }
  return 0;
}

/* ------------------------------------------------------------------------- */
u_int OTC_LinkList::population() const
{
  u_int thePopulation = 0;

  OTC_Linkable* theLink = start_->next();
  while (theLink != (OTC_Linkable*)end_)
  {
    OTCLIB_ASSERT(theLink != 0);

    if (theLink->isLink() && !theLink->isDead())
      thePopulation++;

    theLink = theLink->next();
  }
  return thePopulation;
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_LinkList::isEmpty() const
{
  OTC_Linkable* theLink = start_->next();
  while (theLink != (OTC_Linkable*)end_)
  {
    OTCLIB_ASSERT(theLink != 0);

    if (theLink->isLink() && !theLink->isDead())
      return OTCLIB_FALSE;

    theLink = theLink->next();
  }
  return OTCLIB_TRUE;
}

/* ------------------------------------------------------------------------- */
void OTC_LinkList::removeFirst()
{
  OTC_Link* theLink;
  theLink = first();

  OTCLIB_ENSURE_FN((theLink != 0),
   "OTC_LinkList::removeFirst()",
   "list is empty");

  theLink->kill();
}

/* ------------------------------------------------------------------------- */
void OTC_LinkList::removeLast()
{
  OTC_Link* theLink;
  theLink = last();

  OTCLIB_ENSURE_FN((theLink != 0),
   "OTC_LinkList::removeLast()",
   "list is empty");

  theLink->kill();
}

/* ------------------------------------------------------------------------- */
