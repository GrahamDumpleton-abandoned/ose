dnl Obtained from:
dnl   http://www.gnu.org/software/ac-archive/htmldoc/ac_caolan_func_which_gethostbyname_r.html
dnl GNU General Public License with the following exception:
dnl   http://www.gnu.org/software/ac-archive/htmldoc/COPYING-Exception.html

AC_DEFUN([AC_caolan_FUNC_WHICH_GETHOSTBYNAME_R],
[AC_CACHE_CHECK(for which type of gethostbyname_r,
ac_cv_func_which_gethostname_r, [
AC_CHECK_FUNC(gethostbyname_r, [
        AC_TRY_COMPILE([
#               include <netdb.h>
        ],      [

        char *name;
        struct hostent *he;
        struct hostent_data data;
        (void) gethostbyname_r(name, he, &data);

                ],ac_cv_func_which_gethostname_r=three,
                        [
dnl                     ac_cv_func_which_gethostname_r=no
  AC_TRY_COMPILE([
#   include <netdb.h>
  ], [
        char *name;
        struct hostent *he, *res;
        char buffer[2048];
        int buflen = 2048;
        int h_errnop;
        (void) gethostbyname_r(name, he, buffer, buflen, &res, &h_errnop)
  ],ac_cv_func_which_gethostname_r=six,

  [
dnl  ac_cv_func_which_gethostname_r=no
  AC_TRY_COMPILE([
#   include <netdb.h>
  ], [
                        char *name;
                        struct hostent *he;
                        char buffer[2048];
                        int buflen = 2048;
                        int h_errnop;
                        (void) gethostbyname_r(name, he, buffer, buflen,
&h_errnop)
  ],ac_cv_func_which_gethostname_r=five,ac_cv_func_which_gethostname_r=no)

  ]

  )
                        ]
                )]
        ,ac_cv_func_which_gethostname_r=no)])

if test $ac_cv_func_which_gethostname_r = six; then
  AC_DEFINE(HAVE_FUNC_GETHOSTBYNAME_R_6)
elif test $ac_cv_func_which_gethostname_r = five; then
  AC_DEFINE(HAVE_FUNC_GETHOSTBYNAME_R_5)
elif test $ac_cv_func_which_gethostname_r = three; then
  AC_DEFINE(HAVE_FUNC_GETHOSTBYNAME_R_3)

fi

])

dnl Obtained from:
dnl   http://www.gnu.org/software/ac-archive/htmldoc/ac_raf_func_which_getservbyname_r.html
dnl GNU General Public License with the following exception:
dnl   http://www.gnu.org/software/ac-archive/htmldoc/COPYING-Exception.html

AC_DEFUN([AC_raf_FUNC_WHICH_GETSERVBYNAME_R],
[AC_CACHE_CHECK(for getservbyname_r, ac_cv_func_which_getservbyname_r, [
AC_CHECK_FUNC(getservbyname_r, [
        AC_TRY_COMPILE([
#               include <netdb.h>
        ],      [

        char *name;
        char *proto;
        struct servent *se;
        struct servent_data data;
        (void) getservbyname_r(name, proto, se, &data);

                ],ac_cv_func_which_getservbyname_r=four,
                        [
  AC_TRY_COMPILE([
#   include <netdb.h>
  ], [
        char *name;
        char *proto;
        struct servent *se, *res;
        char buffer[2048];
        int buflen = 2048;
        (void) getservbyname_r(name, proto, se, buffer, buflen, &res)
  ],ac_cv_func_which_getservbyname_r=six,

  [
  AC_TRY_COMPILE([
#   include <netdb.h>
  ], [
        char *name;
        char *proto;
        struct servent *se;
        char buffer[2048];
        int buflen = 2048;
        (void) getservbyname_r(name, proto, se, buffer, buflen)
  ],ac_cv_func_which_getservbyname_r=five,ac_cv_func_which_getservbyname_r=no)

  ]

  )
                        ]
                )]
        ,ac_cv_func_which_getservbyname_r=no)])

if test $ac_cv_func_which_getservbyname_r = six; then
  AC_DEFINE(HAVE_FUNC_GETSERVBYNAME_R_6)
elif test $ac_cv_func_which_getservbyname_r = five; then
  AC_DEFINE(HAVE_FUNC_GETSERVBYNAME_R_5)
elif test $ac_cv_func_which_getservbyname_r = four; then
  AC_DEFINE(HAVE_FUNC_GETSERVBYNAME_R_4)

fi

])

dnl Based on above.

AC_DEFUN([AC_ose_FUNC_WHICH_GETHOSTBYADDR_R],
[AC_CACHE_CHECK(for gethostbyaddr_r, ac_cv_func_which_gethostbyaddr_r, [
AC_CHECK_FUNC(gethostbyaddr_r, [
	AC_TRY_COMPILE([
#include <sys/types.h>
#include <netdb.h>
	],	[
                char * address;
                int length;
                int type;
                struct hostent h;
                struct hostent_data hdata;
                int rc;
                rc = gethostbyaddr_r(address, length, type, &h, &hdata);

	    ],ac_cv_func_which_gethostbyaddr_r=five,
			[
  AC_TRY_COMPILE([
#include <sys/types.h>
#include <netdb.h>
	],	[
                char * address;
                int length;
                int type;
                struct hostent h;
		char buffer[10];
		int buflen;
		int h_errnop;
		struct hostent * hp;

		hp = gethostbyaddr_r(address, length, type, &h,
				     buffer, buflen, &h_errnop);
  ],ac_cv_func_which_gethostbyaddr_r=seven,

  [
  AC_TRY_COMPILE([
#include <sys/types.h>
#include <netdb.h>],
	    [
                char * address;
                int length;
                int type;
                struct hostent h;
		char buffer[10];
		int buflen;
		int h_errnop;
		struct hostent * hp;
		int rc;

		rc = gethostbyaddr_r(address, length, type, &h,
				     buffer, buflen, &hp, &h_errnop);

  ],ac_cv_func_which_gethostbyaddr_r=eight,ac_cv_func_which_gethostbyaddr_r=no)

  ]

  )
                        ]
                )]
        ,ac_cv_func_which_gethostbyaddr_r=no)])

if test $ac_cv_func_which_gethostbyaddr_r = five; then
  AC_DEFINE(HAVE_FUNC_GETHOSTBYADDR_R_5)
elif test $ac_cv_func_which_gethostbyaddr_r = seven; then
  AC_DEFINE(HAVE_FUNC_GETHOSTBYADDR_R_7)
elif test $ac_cv_func_which_gethostbyaddr_r = eight; then
  AC_DEFINE(HAVE_FUNC_GETHOSTBYADDR_R_8)

fi

])
