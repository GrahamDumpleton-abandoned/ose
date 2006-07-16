#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/deque.hh>
#include <OTC/collctn/deque.c>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_Deque<OTC_String>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_Deque<OTC_String>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_Deque<OTC_String>;
#endif
