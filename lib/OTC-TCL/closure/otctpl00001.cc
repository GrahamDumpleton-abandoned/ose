#undef EXPAND_TEMPLATES

#include <OTC/collctn/hmap.hh>
#include <OTC/collctn/hmap.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_HMap<int,int>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_HMap<int,int>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_HMap<int,int>;
#endif
