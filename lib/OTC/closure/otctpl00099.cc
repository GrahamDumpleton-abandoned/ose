#undef EXPAND_TEMPLATES

#include <OTC/collctn/prcursor.hh>
#include <OTC/collctn/prcursor.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_PairCursor<int,void*>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_PairCursor<int,void*>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_PairCursor<int,void*>;
#endif
