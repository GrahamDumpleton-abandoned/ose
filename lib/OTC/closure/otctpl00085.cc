#undef EXPAND_TEMPLATES

#include <OTC/dispatch/servbind.hh>
#include <OTC/collctn/ordlist.hh>
#include <OTC/collctn/ordlist.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_OrderedList<OTC_ServiceBinding*>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_OrderedList<OTC_ServiceBinding*>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_OrderedList<OTC_ServiceBinding*>;
#endif
