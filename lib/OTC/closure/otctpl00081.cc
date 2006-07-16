#undef EXPAND_TEMPLATES

#include <OTC/collctn/bucket.hh>
#include <OTC/collctn/bucket.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_Bucket<int>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_Bucket<int>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_Bucket<int>;
#endif
