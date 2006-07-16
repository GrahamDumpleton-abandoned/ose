#undef EXPAND_TEMPLATES

#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/icursor.c>
#include <OTC/collctn/bucket.hh>
#include <OTC/text/string.hh>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_ItemCursor< OTC_String,OTC_Bucket<OTC_String> >
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_ItemCursor< OTC_String,OTC_Bucket<OTC_String> >)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_ItemCursor< OTC_String,OTC_Bucket<OTC_String> >;
#endif
