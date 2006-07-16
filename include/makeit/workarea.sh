#!/bin/sh

#
# = AUTHOR(S)
#     Graham Dumpleton
#
# = COPYRIGHT
#     Copyright 1996 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
#

FILE="$1"

if test "$1" = ""
then
  FILE="project"
fi

WD="."

if test "$2" != ""
then
  WD="$2"
fi

cd $WD

while test `pwd` != '/'
do
  if test -r "$FILE"
  then
    echo "$WD"
    exit
  else
    cd ..
    if test "$WD" = "."
    then
      WD=".."
    else
      WD="$WD/.."
    fi
  fi
done

echo "invalid_workarea"
