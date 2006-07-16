#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/tmap.hh>
#include <OTC/collctn/tmap.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_TMap<OTC_String,OTC_String>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_TMap<OTC_String,OTC_String>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_TMap<OTC_String,OTC_String>;
#endif
