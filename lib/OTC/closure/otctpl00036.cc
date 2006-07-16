#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/collctn/hmap.c>
#include <OTC/message/eprgstry.hh>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_HMap<OTC_String,OTC_EPRegistry*>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_HMap<OTC_String,OTC_EPRegistry*>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_HMap<OTC_String,OTC_EPRegistry*>;
#endif
