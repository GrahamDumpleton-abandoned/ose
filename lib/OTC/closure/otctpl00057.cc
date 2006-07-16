#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/hindex.hh>
#include <OTC/collctn/hindex.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_HIndex<OTC_String>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_HIndex<OTC_String>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_HIndex<OTC_String>;
#endif
