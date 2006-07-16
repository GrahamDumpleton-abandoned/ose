#undef EXPAND_TEMPLATES

#include <OTC/text/svlink.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/icursor.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_ItemCursor<void*,OTC_SVLink>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_ItemCursor<void*,OTC_SVLink>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_ItemCursor<void*,OTC_SVLink>;
#endif
