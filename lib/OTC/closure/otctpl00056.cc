#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/hindexen.hh>
#include <OTC/collctn/hindexen.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_HIndexEntry<OTC_String>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_HIndexEntry<OTC_String>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_HIndexEntry<OTC_String>;
#endif
