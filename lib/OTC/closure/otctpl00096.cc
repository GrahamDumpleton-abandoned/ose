#undef EXPAND_TEMPLATES

#include <OTC/collctn/ivlink.hh>
#include <OTC/collctn/kicursor.hh>
#include <OTC/collctn/kicursor.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_KeyItemCursor<int,void*,OTC_IVLink>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_KeyItemCursor<int,void*,OTC_IVLink>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_KeyItemCursor<int,void*,OTC_IVLink>;
#endif
