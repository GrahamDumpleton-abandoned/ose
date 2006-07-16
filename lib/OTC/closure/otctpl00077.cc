#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/prcursor.hh>
#include <OTC/collctn/prcursor.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_PairCursor<OTC_String,int>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_PairCursor<OTC_String,int>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_PairCursor<OTC_String,int>;
#endif
