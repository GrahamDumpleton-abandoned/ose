#undef EXPAND_TEMPLATES

#include <OTC/collctn/ivlink.hh>
#include <OTC/collctn/kcursor.hh>
#include <OTC/collctn/kcursor.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_KeyCursor<int,OTC_IVLink>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_KeyCursor<int,OTC_IVLink>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_KeyCursor<int,OTC_IVLink>;
#endif
