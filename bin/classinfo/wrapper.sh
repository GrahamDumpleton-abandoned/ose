#!/bin/sh

#
# = AUTHOR(S)
#     Graham Dumpleton
#
# = COPYRIGHT
#     Copyright 1994-1996 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
#

CMDEXT=@CMDEXT@

PROGRAM=`basename $0`

OSE_ROOT=${OSE_ROOT-@OSE_ROOT@}
export OSE_ROOT

OSE_RELEASE_DIRECTORY=@OSE_RELEASE_DIRECTORY@
export OSE_RELEASE_DIRECTORY

if test "$OSE_HOST" = ""
then
  echo "$PROGRAM: OSE_HOST variable is not defined in your environment" 1>&2
  exit 1
fi

OSE_HOST_HOME=$OSE_ROOT/$OSE_RELEASE_DIRECTORY/$OSE_HOST

if test -x $OSE_HOST_HOME/bin/$PROGRAM$CMDEXT
then
  if test "$#" != "0"
  then
    exec $OSE_HOST_HOME/bin/$PROGRAM "$@"
  else
    exec $OSE_HOST_HOME/bin/$PROGRAM
  fi
fi

echo "$PROGRAM: $OSE_HOST_HOME/bin/$PROGRAM does not exist, stopping" 1>&2
exit 1
