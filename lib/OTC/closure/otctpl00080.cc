#undef EXPAND_TEMPLATES

#include <OTC/collctn/prbucket.hh>
#include <OTC/collctn/kicursor.hh>
#include <OTC/collctn/kicursor.c>
#include <OTC/text/string.hh>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_KeyItemCursor< OTC_String,int,OTC_PairBucket<OTC_String,int> >
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_KeyItemCursor< OTC_String,int,OTC_PairBucket<OTC_String,int> >)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_KeyItemCursor< OTC_String,int,OTC_PairBucket<OTC_String,int> >;
#endif
