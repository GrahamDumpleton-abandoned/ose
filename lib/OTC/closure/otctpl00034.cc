#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/prbucket.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/icursor.c>
#include <OTC/message/epserver.hh>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_ItemCursor< OTC_EPServer*,OTC_PairBucket<OTC_String,OTC_EPServer*> >
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_ItemCursor< OTC_EPServer*,OTC_PairBucket<OTC_String,OTC_EPServer*> >)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_ItemCursor< OTC_EPServer*,OTC_PairBucket<OTC_String,OTC_EPServer*> >;
#endif
