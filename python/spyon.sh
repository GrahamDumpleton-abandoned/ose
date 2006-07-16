#!/bin/sh

#
# = AUTHOR(S)
#     Graham Dumpleton
#
# = COPYRIGHT
#     Copyright 2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
#

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

OSE_HOME=$OSE_ROOT/$OSE_RELEASE_DIRECTORY

if [ "$OSE_HOST" = "X86_WIN32" ]
then
  LIBPATH="$OSE_HOME/lib/python"
  PYTHONPATH="`echo $LIBPATH | sed -e 's%/%\\\\%g'`;$PYTHONPATH"
else
  PYTHONPATH="$OSE_HOME/lib/python:$PYTHONPATH"
fi
export PYTHONPATH

exec @PYTHON@ $OSE_HOME/lib/python-apps/spyon.py "$@"
