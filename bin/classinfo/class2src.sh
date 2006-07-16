#!/bin/sh

# =============================================================================

: << END-OF-MANUAL-PAGE
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1995-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

// [class2src]
//
// = TITLE
//     Generates source stubs from an input file.
//
// = SYNOPSIS
//     class2src file.hh
//
// = DESCRIPTION
//     The <class2src> searches an input file specified for class member
//     functions and produces on standard output stubs suitable for inclusion
//     in the src file. In doing this, default values will be removed and if
//     the line is long then it will be split over a number of lines. The
//     program achieves this by running <class2info> on the input file and
//     then running <info2src> on the generated classinfo file.
//
// = NOTES
//     If the member function is defined as inline or pure virtual then no
//     stub will be created. Nested classes are not dealt with in a sensible
//     way yet.
//
// = SEE ALSO
//     class2info(1), info2src(1)
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

CLASS2INFO="$BINDIR/class2info@CMDEXT@"
INFO2SRC="$BINDIR/info2src@CMDEXT@"

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
  ERROR "Usage: `basename $0` files"
} 

#
# Check usage.
#
if test "$#" = "0"
then
  USAGE
fi

while test "$#" != "0"
do
  $CLASS2INFO $1
  if test "$?" != "0"
  then
    exit 1
  fi
  file=`basename $1`
  base="`echo $file | sed -e 's/\..*$//'`"
  if test -f "$base.ci"
  then
    $INFO2SRC $base.ci
    rm -f $base.ci
  fi
  shift
done
