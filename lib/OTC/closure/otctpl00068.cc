#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/prbucket.hh>
#include <OTC/collctn/kicursor.hh>
#include <OTC/collctn/kicursor.c>
#include <OTC/dispatch/servbind.hh>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_KeyItemCursor< OTC_String,OTC_ServiceBinding*,OTC_PairBucket<OTC_String,OTC_ServiceBinding*> >
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_KeyItemCursor< OTC_String,OTC_ServiceBinding*,OTC_PairBucket<OTC_String,OTC_ServiceBinding*> >)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_KeyItemCursor< OTC_String,OTC_ServiceBinding*,OTC_PairBucket<OTC_String,OTC_ServiceBinding*> >;
#endif
