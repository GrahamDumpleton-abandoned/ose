AC_INIT

AC_CONFIG_AUX_DIR(makeit/scripts)
AC_CANONICAL_BUILD

AC_CONFIG_HEADER(include/OSE/config.h)

AC_SUBST(PACKAGE_NAME)
AC_SUBST(PACKAGE_VERSION)
AC_SUBST(PACKAGE_STRING)

AC_SUBST(PACKAGE_MAJOR_VERSION)
AC_SUBST(PACKAGE_MINOR_VERSION)
AC_SUBST(PACKAGE_PATCH_VERSION)

[ PACKAGE_STRING=`cat PACKAGE | sed -e 's/\n//'` ]

[ PACKAGE_NAME=`echo $PACKAGE_STRING | \
 sed -e 's/^\(.*\)-.*$/\1/'` ]

[ PACKAGE_VERSION=`echo $PACKAGE_STRING | \
 sed -e 's/^.*-\(.*\)$/\1/'` ]

[ PACKAGE_MAJOR_VERSION=`echo $PACKAGE_STRING | \
 sed -e 's/^.*-\([0-9][0-9]*\)\..*$/\1/'` ]

[ PACKAGE_MINOR_VERSION=`echo $PACKAGE_STRING | \
 sed -e 's/^.*-.*\.\([0-9][0-9]*\).*$/\1/' -e 's/^\([0-9]\)$/0\1/'` ]

[ PACKAGE_PATCH_VERSION=`echo $PACKAGE_STRING | \
 sed -e 's/^.*-.*pl\([0-9][0-9]*\)$/\1/' -e 's/^\([0-9]\)$/0\1/' | \
 sed -e "s/^$PACKAGE_STRING$/00/"` ]

AC_PROG_CPP

AC_STRUCT_ST_BLKSIZE
AC_STRUCT_ST_BLOCKS
AC_STRUCT_ST_RDEV

AC_HEADER_DIRENT

AC_CHECK_HEADERS(memory.h)
AC_CHECK_HEADERS(stropts.h)
AC_CHECK_HEADERS(unistd.h)
AC_CHECK_HEADERS(sys/resource.h)
AC_CHECK_HEADERS(sys/elf.h)
AC_CHECK_HEADERS(sys/select.h)
AC_CHECK_HEADERS(sys/param.h)
AC_CHECK_HEADERS(sys/types.h)
AC_CHECK_HEADERS(sys/socket.h)

AC_CHECK_FUNCS(getcwd)
AC_CHECK_FUNCS(getdtablesize)
AC_CHECK_FUNCS(getpid)
AC_CHECK_FUNCS(gettimeofday)
AC_CHECK_FUNCS(nl_langinfo)
AC_CHECK_FUNCS(poll)
AC_CHECK_FUNCS(select)
AC_CHECK_FUNCS(sigprocmask)
AC_CHECK_FUNCS(sigaction)
AC_CHECK_FUNCS(strcoll)
AC_CHECK_FUNCS(strtoll)
AC_CHECK_FUNCS(strtold)
AC_CHECK_FUNCS(sysconf)
AC_CHECK_FUNCS(uname)

AC_FUNC_STRCOLL

AC_SEARCH_LIBS(pthread_rwlock_init,pthread c_r)
AC_CHECK_FUNCS(pthread_rwlock_init)

AC_MSG_CHECKING(for sysconf (_SC_OPEN_MAX))
AC_CACHE_VAL(my_cv_sysconf_sc_open_max, [dnl
AC_TRY_LINK([#include <unistd.h>], [int max = sysconf (_SC_OPEN_MAX);],
            [my_cv_sysconf_sc_open_max=yes], [my_cv_sysconf_sc_open_max=no])])
if test $my_cv_sysconf_sc_open_max = yes; then
  AC_DEFINE(HAVE_SYSCONF_SC_OPEN_MAX)
fi
AC_MSG_RESULT($my_cv_sysconf_sc_open_max)

AC_DEFUN(MY_SUBST_TYPE,
[AC_MSG_CHECKING([for $1])
AC_CACHE_VAL(my_cv_type_$1,
changequote(,)dnl
trfrom='[a-z]' trto='[A-Z]'
changequote([,])dnl
AC_HEADER_EGREP($1, sys/types.h,
eval "my_cv_type_$1=yes",
eval "my_cv_type_$1=no"
))
if eval "test \"`echo '$my_cv_type_'$1`\" = yes"; then
  AC_MSG_RESULT(yes)
  ifelse([$2], , :, [$2])
else
  AC_MSG_RESULT(no)
  ifelse([$3], , , [$3])
fi
])

AC_DEFUN(MY_SUBST_TYPES,
[for my_type in $1
do
MY_SUBST_TYPE($my_type,$2,
[changequote(, )dnl
  my_tr_type=NEED_`echo $my_type | tr '[a-z]' '[A-Z]'`
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($my_tr_type)] $3)dnl
done
])

MY_SUBST_TYPES(uid_t)
MY_SUBST_TYPES(pid_t)
MY_SUBST_TYPES(off_t)
MY_SUBST_TYPES(dev_t)
MY_SUBST_TYPES(ino_t)
MY_SUBST_TYPES(u_char)
MY_SUBST_TYPES(u_short)
MY_SUBST_TYPES(u_int)
MY_SUBST_TYPES(u_long)

AC_MSG_CHECKING([for socklen_t])
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
], [socklen_t t;], ac_cv_type_socklen_t=yes, ac_cv_type_socklen_t=no)
if test "x$ac_cv_type_socklen_t" = "xyes"; then
AC_MSG_RESULT([yes])
AC_DEFINE(HAVE_SOCKLEN_T,1,
  [Define if socklen_t is available])
else
AC_MSG_RESULT([no])
fi

# Checking for the third getpeername parameter
_peername_type_found=no
if test x"$ac_cv_type_socklen_t" = xyes; then
AC_MSG_CHECKING([for the third getpeername argument])
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],[
socklen_t i;
(void)getpeername(0,0,&i);],[
AC_DEFINE([HAVE_GETPEERNAME_SOCKLEN_T],1,[Define if getpeername() uses
socklen_t.])
AC_MSG_RESULT([socklen_t])
_peername_type_found=yes])
fi
if test x"$_peername_type_found" != xyes; then
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],[
size_t i;
(void)getpeername(0,0,&i);],[
AC_DEFINE([HAVE_GETPEERNAME_SIZE_T],1,[Define if getpeername() uses size_t.])
AC_MSG_RESULT([size_t])
_peername_type_found=yes])
fi
if test x"$_peername_type_found" != xyes; then
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],[
int i;
(void)getpeername(0,0,&i);],[
AC_DEFINE([HAVE_GETPEERNAME_INT],1,[Define if getpeername() uses int.])
AC_MSG_RESULT([int])
_peername_type_found=yes])
fi

if test x"$_peername_type_found" != xyes; then
  AC_MSG_RESULT([cannot resolve])
  AC_MSG_ERROR([No known getpeername type found])
fi

# Checking for the third getsockname parameter
_sockname_type_found=no
if test x"$ac_cv_type_socklen_t" = xyes; then
AC_MSG_CHECKING([for the third getsockname argument])
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],[
socklen_t i;
(void)getsockname(0,0,&i);],[
AC_DEFINE([HAVE_GETSOCKNAME_SOCKLEN_T],1,[Define if getsockname() uses
socklen_t.])
AC_MSG_RESULT([socklen_t])
_sockname_type_found=yes])
fi
if test x"$_sockname_type_found" != xyes; then
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],[
size_t i;
(void)getsockname(0,0,&i);],[
AC_DEFINE([HAVE_GETSOCKNAME_SIZE_T],1,[Define if getsockname() uses size_t.])
AC_MSG_RESULT([size_t])
_sockname_type_found=yes])
fi
if test x"$_sockname_type_found" != xyes; then
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],[
int i;
(void)getsockname(0,0,&i);],[
AC_DEFINE([HAVE_GETSOCKNAME_INT],1,[Define if getsockname() uses int.])
AC_MSG_RESULT([int])
_sockname_type_found=yes])
fi

if test x"$_sockname_type_found" != xyes; then
  AC_MSG_RESULT([cannot resolve])
  AC_MSG_ERROR([No known getsockname type found])
fi

_getsockopt_type_found=no
if test x"$ac_cv_type_socklen_t" = xyes; then
AC_MSG_CHECKING([for the fourth getsockopt argument])
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],[
socklen_t i;
(void)getsockopt(0,0,0,0,&i);],[
AC_DEFINE([HAVE_GETSOCKOPT_SOCKLEN_T],1,[Define if getsockopt() uses
socklen_t.])
AC_MSG_RESULT([socklen_t])
_getsockopt_type_found=yes])
fi
if test x"$_getsockopt_type_found" != xyes; then
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],[
size_t i;
(void)getsockopt(0,0,0,0,&i);],[
AC_DEFINE([HAVE_GETSOCKOPT_SIZE_T],1,[Define if getsockopt() uses size_t.])
AC_MSG_RESULT([size_t])
_getsockopt_type_found=yes])
fi
if test x"$_getsockopt_type_found" != xyes; then
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],[
int i;
(void)getsockopt(0,0,0,0,&i);],[
AC_DEFINE([HAVE_GETSOCKOPT_INT],1,[Define if getsockopt() uses int.])
AC_MSG_RESULT([int])
_getsockopt_type_found=yes])
fi

if test x"$_getsockopt_type_found" != xyes; then
  AC_MSG_RESULT([cannot resolve])
  AC_MSG_ERROR([No known getsockopt type found])
fi

# Checking for the third accept parameter
_accept_type_found=no
if test x"$ac_cv_type_socklen_t" = xyes; then
AC_MSG_CHECKING([for the third accept argument])
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],[
socklen_t i;
(void)accept(0,0,&i);],[
AC_DEFINE([HAVE_ACCEPT_SOCKLEN_T],1,[Define if accept() uses socklen_t.])
AC_MSG_RESULT([socklen_t])
_accept_type_found=yes])
fi
if test x"$_accept_type_found" != xyes; then
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],[
size_t i;
(void)accept(0,0,&i);],[
AC_DEFINE([HAVE_ACCEPT_SIZE_T],1,[Define if accept() uses size_t.])
AC_MSG_RESULT([size_t])
_accept_type_found=yes])
fi
if test x"$_accept_type_found" != xyes; then
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],[
int i;
(void)accept(0,0,&i);],[
AC_DEFINE([HAVE_ACCEPT_INT],1,[Define if accept() uses int.])
AC_MSG_RESULT([int])
_accept_type_found=yes])
fi

if test x"$_accept_type_found" != xyes; then
  AC_MSG_RESULT([cannot resolve])
  AC_MSG_ERROR([No known accept type found])
fi

# Checking for the sixth recvfrom parameter
_recvfrom_type_found=no
if test x"$ac_cv_type_socklen_t" = xyes; then
AC_MSG_CHECKING([for the sixth recvfrom argument])
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],[
socklen_t i;
(void)recvfrom(0,0,0,0,0,&i);],[
AC_DEFINE([HAVE_RECVFROM_SOCKLEN_T],1,[Define if recvfrom() uses socklen_t.])
AC_MSG_RESULT([socklen_t])
_recvfrom_type_found=yes])
fi
if test x"$_recvfrom_type_found" != xyes; then
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],[
size_t i;
(void)recvfrom(0,0,0,0,0,&i);],[
AC_DEFINE([HAVE_RECVFROM_SIZE_T],1,[Define if recvfrom() uses size_t.])
AC_MSG_RESULT([size_t])
_recvfrom_type_found=yes])
fi
if test x"$_recvfrom_type_found" != xyes; then
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],[
int i;
(void)recvfrom(0,0,0,0,0,&i);],[
AC_DEFINE([HAVE_RECVFROM_INT],1,[Define if recvfrom() uses int.])
AC_MSG_RESULT([int])
_recvfrom_type_found=yes])
fi

if test x"$_recvfrom_type_found" != xyes; then
  AC_MSG_RESULT([cannot resolve])
  AC_MSG_ERROR([No known recvfrom type found])
fi

AC_CHECK_SIZEOF(double)
AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(long double)

CPPFLAGS="$CPPFLAGS -D_REENTRANT"

AC_SEARCH_LIBS(gethostbyname_r,nsl c_r)
AC_caolan_FUNC_WHICH_GETHOSTBYNAME_R

AC_SEARCH_LIBS(getservbyname_r,socket c_r)
AC_raf_FUNC_WHICH_GETSERVBYNAME_R

AC_SEARCH_LIBS(gethostbyaddr_r,nsl c_r)
AC_ose_FUNC_WHICH_GETHOSTBYADDR_R

AC_OUTPUT(config.mk include/OSE/version.h modules/ose.mk modules/ose.pkg)

# [ echo cp include/OSE/config.h include/OSE/${ac_cv_build_alias}.h]
# [ cp include/OSE/config.h include/OSE/${ac_cv_build_alias}.h]
