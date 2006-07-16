#undef EXPAND_TEMPLATES

#include <OTC/text/svlink.hh>
#include <OTC/collctn/kcursor.hh>
#include <OTC/collctn/kcursor.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_KeyCursor<OTC_String,OTC_SVLink>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_KeyCursor<OTC_String,OTC_SVLink>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_KeyCursor<OTC_String,OTC_SVLink>;
#endif