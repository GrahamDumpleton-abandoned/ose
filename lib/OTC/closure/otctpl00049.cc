#undef EXPAND_TEMPLATES

#include <OTC/collctn/bucket.hh>
#include <OTC/collctn/bucket.c>
#include <OTC/message/exchange.hh>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_Bucket<OTC_Exchange*>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_Bucket<OTC_Exchange*>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_Bucket<OTC_Exchange*>;
#endif
