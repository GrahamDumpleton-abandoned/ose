#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/tdualmap.hh>
#include <OTC/collctn/tdualmap.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_TDualMap<OTC_String,int>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_TDualMap<OTC_String,int>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_TDualMap<OTC_String,int>;
#endif