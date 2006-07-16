#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/list.hh>
#include <OTC/collctn/list.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_List<OTC_String>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_List<OTC_String>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_List<OTC_String>;
#endif
