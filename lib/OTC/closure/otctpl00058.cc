#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/hset.hh>
#include <OTC/collctn/hset.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_HSet<OTC_String>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_HSet<OTC_String>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_HSet<OTC_String>;
#endif
