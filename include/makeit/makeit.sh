#!/bin/sh

#
# = AUTHOR(S)
#     Graham Dumpleton
#
# = COPYRIGHT
#     Copyright 1993 TELSTRA CORPORATION LIMITED
#     Copyright 1994-1996 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
#

BINEXT=@BINEXT@

OSE_ROOT=${OSE_ROOT-@OSE_ROOT@}
export OSE_ROOT

OSE_RELEASE_DIRECTORY=@OSE_RELEASE_DIRECTORY@
export OSE_RELEASE_DIRECTORY

OSE_HOST_HOME=$OSE_ROOT/$OSE_RELEASE_DIRECTORY/@OSE_HOST@

OSE_MAKE_PROGRAM=${OSE_MAKE_PROGRAM-$OSE_HOST_HOME/etc/makeit}

MAKEIT=$OSE_HOST_HOME/bin/makeit
export MAKEIT

if test ! -x $OSE_MAKE_PROGRAM$BINEXT
then
    echo "makeit: $OSE_MAKE_PROGRAM$BINEXT does not exist, stopping" 1>&2
    exit 1
fi

if test "$#" != "0"
then
    $OSE_MAKE_PROGRAM SHELL=@SHELL@ MAKE=$OSE_MAKE_PROGRAM$BINEXT \
      -I$OSE_HOST_HOME "$@"
else
    $OSE_MAKE_PROGRAM SHELL=@SHELL@ MAKE=$OSE_MAKE_PROGRAM$BINEXT \
      -I$OSE_HOST_HOME
fi

# if test "$#" != "0"
# then
#     case $OSE_MAKE_PROGRAM in
#         *clearmake)
# 	  $OSE_MAKE_PROGRAM SHELL=@SHELL@ -I $OSE_HOST_HOME "$@"
# 	  ;;
#         *)
# 	  $OSE_MAKE_PROGRAM SHELL=@SHELL@ -I$OSE_HOST_HOME "$@"
# 	  ;;
#     esac
# else
#     case $OSE_MAKE_PROGRAM in
# 	*clearmake)
# 	    $OSE_MAKE_PROGRAM SHELL=@SHELL@ -I $OSE_HOST_HOME
# 	    ;;
# 	*)
# 	  $OSE_MAKE_PROGRAM SHELL=@SHELL@ -I$OSE_HOST_HOME
# 	  ;;
#     esac
# fi
