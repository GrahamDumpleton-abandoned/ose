#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/prbucket.hh>
#include <OTC/collctn/prbucket.c>
#include <OTC/message/eprgstry.hh>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_PairBucket<OTC_String,OTC_EPRegistry*>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_PairBucket<OTC_String,OTC_EPRegistry*>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_PairBucket<OTC_String,OTC_EPRegistry*>;
#endif
