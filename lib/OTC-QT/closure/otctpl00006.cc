#undef EXPAND_TEMPLATES

#include <qobject.h>
#include <OTC/collctn/prbucket.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/icursor.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_ItemCursor< int,OTC_PairBucket<int,QObject*> >
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_ItemCursor< int,OTC_PairBucket<int,QObject*> >)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_ItemCursor< int,OTC_PairBucket<int,QObject*> >;
#endif
