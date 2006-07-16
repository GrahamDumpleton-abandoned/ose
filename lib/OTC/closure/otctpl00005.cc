#undef EXPAND_TEMPLATES

#include <OTC/collctn/kicursor.hh>
#include <OTC/collctn/kicursor.c>
#include <OTC/collctn/iilink.hh>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_KeyItemCursor< int,int,OTC_IILink >
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_KeyItemCursor< int,int,OTC_IILink >)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_KeyItemCursor< int,int,OTC_IILink >;
#endif
