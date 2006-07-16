#undef EXPAND_TEMPLATES

#include <OTC/collctn/bucket.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/icursor.c>
#include <OTC/dispatch/servbind.hh>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_ItemCursor< OTC_ServiceBinding*,OTC_Bucket<OTC_ServiceBinding*> >
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_ItemCursor< OTC_ServiceBinding*,OTC_Bucket<OTC_ServiceBinding*> >)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_ItemCursor< OTC_ServiceBinding*,OTC_Bucket<OTC_ServiceBinding*> >;
#endif
