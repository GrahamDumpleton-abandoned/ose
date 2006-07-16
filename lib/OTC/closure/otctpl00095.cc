#undef EXPAND_TEMPLATES

#include <OTC/text/svlink.hh>
#include <OTC/collctn/kicursor.hh>
#include <OTC/collctn/kicursor.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_KeyItemCursor<OTC_String,void*,OTC_SVLink>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_KeyItemCursor<OTC_String,void*,OTC_SVLink>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_KeyItemCursor<OTC_String,void*,OTC_SVLink>;
#endif
