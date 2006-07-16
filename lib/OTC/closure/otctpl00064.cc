#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/prbucket.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/icursor.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_ItemCursor< OTC_String,OTC_PairBucket<OTC_String,OTC_String> >
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_ItemCursor< OTC_String,OTC_PairBucket<OTC_String,OTC_String> >)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_ItemCursor< OTC_String,OTC_PairBucket<OTC_String,OTC_String> >;
#endif
