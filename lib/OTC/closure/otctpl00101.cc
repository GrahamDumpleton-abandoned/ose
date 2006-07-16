#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/collctn/hmap.c>
#include <OTC/dispatch/svregsub.hh>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_HMap<OTC_String,OTC_SVRegistrySubscription*>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_HMap<OTC_String,OTC_SVRegistrySubscription*>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_HMap<OTC_String,OTC_SVRegistrySubscription*>;
#endif
