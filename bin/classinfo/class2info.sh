#!/bin/sh

# =============================================================================

: << END-OF-MANUAL-PAGE
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

// [class2info]
//
// = TITLE
//     Generates a class info description file from an input file.
//
// = SYNOPSIS
//     class2info [ -output dirname ] file.hh
//
// = DESCRIPTION
//     The <class2info> program is a script for generating classinfo
//     description files from an input file. Normally the input file will be
//     a C++ class header file. For each file listed, it will generate a
//     classinfo file. The generated file will have the same basename as the
//     original, but will be suffixed by ".ci".
//
//     By default the classinfo file which is generated is put in the
//     current directory. This can be changed by specifying an alternate
//     directory using the "-output" option.
//
//     Although <class2info> can generally extract member variable and
//     function information from any class as long as it is reasonably
//     conventionally formatted, it cannot extract documentation properly
//     unless it is included in the class definition in a particular format.
//
// = NOTES
//     As <class2info> uses regular expression matching and does not use
//     parsing techniques, it can get confused if you use strange formatting
//     conventions for your code. A few known problems are given below. This
//     list is by no means exchaustive.
//
//     C style comments can cause confusion and can the program to hang.
//     You should avoid using C style comments inside the body of a class.
//
//     Pointers to functions appearing anywhere in the class will probably
//     cause incorrect results. Use a typedef before the class to give the
//     pointer to function type a name and then use that.
//
//     Declaring a pointer to class type when it is used in a return type
//     will probably result in the whole line being ignored. e.g. don't
//     declare a function prototype with 'class Foo* bar();'.
//
//     Blank lines and C++ comments inside of inline code will cause the end
//     of the inline to be incorrectly detected. You should avoid using blank
//     lines and C++ comments in inline code.
END-OF-MANUAL-PAGE

# =============================================================================

OSE_HOST=@OSE_HOST@

OSE_RELEASE_DIRECTORY=@OSE_RELEASE_DIRECTORY@
export OSE_RELEASE_DIRECTORY

OSE_ROOT=${OSE_ROOT-@OSE_ROOT@}
export OSE_ROOT

OSE_VERSION_ROOT=$OSE_ROOT/$OSE_RELEASE_DIRECTORY
export OSE_VERSION_ROOT

BINDIR="$OSE_VERSION_ROOT/$OSE_HOST/bin"
LIBDIR=${CLASSINFOLIBDIR-"$OSE_VERSION_ROOT/lib"}

INFO2INFO="$BINDIR/info2info@CMDEXT@"

SYSTEM=`uname -s 2>/dev/null | tr 'A-Z' 'a-z' 2>/dev/null`

case "$SYSTEM" in
  darwin)
    AWK="awk"
    VARG="-v"
    ;;
  *)
    AWK="@AWK@"
    VARG=
    if test "$AWK" = "gawk"
    then
      VARG="-v"
    fi
    ;;
esac

FILES=
OUTPUT="."

TMPFILE=.ci.$$

if test -d "/tmp"
then
  TMPFILE=/tmp/ci.$$
else
  if test -d "/temp"
  then
    TMPFILE=/temp/ci.$$
  fi
fi

trap 'rm -f $TMPFILE; exit' 1 2 3 13 15

#
# Error.
#
ERROR()
{
  echo "`basename $0`: $1" >&2
  shift
  while test $# != "0"
  do
    echo $1 >&2
    shift
  done
  exit 1
}

#
# Usage message.
#
USAGE()
{
  ERROR "Usage: `basename $0` file.h ..."
}

#
# Check usage.
#
if test $# = 0
then
  USAGE
fi

#
# Parse command line.
#
while test $# != 0
do
  case $1 in
    -output)
      shift
      if test "$#" != "0"
      then
        OUTPUT=$1
        shift
      fi
      ;;
    *.*)
      FILES="$FILES $1"
      shift
      ;;
    *)
      USAGE
      ;;
    esac
done

#
# Check usage again.
#
if test -z "$FILES"
then
  USAGE
fi

#
# Check for awk file.
#
CLASS2INFO=$LIBDIR/class2info.awk

if ( test ! -f $CLASS2INFO )
then
  ERROR "Can't find $CLASS2INFO"
fi

#
# Parse each file.
#
for i in $FILES
do
  if test ! -f $i
  then
    ERROR "$i doesn't exist."
  fi
  FILENAME=`basename $i`
  INFOFILE=$OUTPUT/`echo $FILENAME | sed -e 's/\..*$//'`.ci
  rm -f $INFOFILE
  if test "$?" != 0
  then
    ERROR "Couldn't remove info file $INFOFILE."
  fi
  if test -f class2info.sed
  then
    expand $i | sed -e 's/ OSE_EXPORT / /' | sed -f class2info.sed > $TMPFILE
  else
    expand $i | sed -e 's/ OSE_EXPORT / /' > $TMPFILE 
  fi
  $AWK -f $CLASS2INFO $VARG filename=$i $TMPFILE > $INFOFILE
  rm -f /tmp/ci.$$
done
