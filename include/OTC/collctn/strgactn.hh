#ifndef OTC_COLLCTN_STRGACTN_HH
#define OTC_COLLCTN_STRGACTN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/strgactn.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2000 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OTC_COLLCTN_HASHACTN_HH
#include <OTC/collctn/hashactn.hh>
#endif
#ifndef OTC_COLLCTN_RANKACTN_HH
#include <OTC/collctn/rankactn.hh>
#endif
#ifndef OTC_COLLCTN_COPYACTN_HH
#include <OTC/collctn/copyactn.hh>
#endif

/* ------------------------------------------------------------------------- */

#if defined(CXX_SUN3_0)
typedef OTC_HashActions<void*> otc_collctn_hashactn_hh_typedef1;
#endif

#if defined(ENV_CXXSTDTPL)
template<>
#endif
class OSE_EXPORT OTC_HashActions<char*>
{
  public:
    static int          hash(char* const& s)
				{ return OTC_Hash::hash(s); }
};

#if defined(ENV_CXXSTDTPL)
template<>
#endif
class OSE_EXPORT OTC_HashActions<const char*>
{
  public:
    static int          hash(const char* const& s)
				{ return OTC_Hash::hash(s); }
};

/* ------------------------------------------------------------------------- */

#if defined(CXX_SUN3_0)
typedef OTC_RankActions<void*> otc_collctn_rankactn_hh_typedef1;
#endif

#if defined(ENV_CXXSTDTPL)
template<>
#endif
class OSE_EXPORT OTC_RankActions<char*>
{
  public:
    static int          rank(char* const& s1, char* const& s2)
				{ return OTC_Rank::rank(s1,s2); }
};

#if defined(ENV_CXXSTDTPL)
template<>
#endif
class OSE_EXPORT OTC_RankActions<const char*>
{
  public:
    static int          rank(const char* const& s1, const char* const& s2)
				{ return OTC_Rank::rank(s1,s2); }
};

/* ------------------------------------------------------------------------- */

#if defined(CXX_SUN3_0)
typedef OTC_CopyActions<void*> otc_collctn_copyactn_hh_typedef1;
#endif

#if defined(ENV_CXXSTDTPL)
template<>
#endif
class OSE_EXPORT OTC_CopyActions<char>
{
  public:
    static void		copy(char* s1, char const* s2, size_t l)
				{ OTC_Copy::copy(s1,s2,l); }
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_STRGACTN_HH */
