#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/prbucket.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/icursor.c>
#include <OTC/message/exchange.hh>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_ItemCursor< OTC_Exchange*,OTC_PairBucket<OTC_String,OTC_Exchange*> >
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_ItemCursor< OTC_Exchange*,OTC_PairBucket<OTC_String,OTC_Exchange*> >)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_ItemCursor< OTC_Exchange*,OTC_PairBucket<OTC_String,OTC_Exchange*> >;
#endif
