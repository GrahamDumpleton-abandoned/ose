#undef EXPAND_TEMPLATES

#include <OTC/text/string.hh>
#include <OTC/collctn/prbucket.hh>
#include <OTC/collctn/cursor.hh>
#include <OTC/collctn/cursor.c>
#include <OTC/message/exchange.hh>

#undef FINISHED

#if defined(CXX_DEC)
#pragma define_template OTC_Cursor<OTC_Exchange*>
#define FINISHED
#endif

#if defined(CXX_XL)
#pragma define(OTC_Cursor<OTC_Exchange*>)
#define FINISHED
#endif

#if !defined(FINISHED)
template class OTC_Cursor<OTC_Exchange*>;
#endif