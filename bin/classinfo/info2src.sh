#!/bin/sh

# =============================================================================

: << END-OF-MANUAL-PAGE
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1995-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

// [info2src]
//
// = TITLE
//     Generates source stubs from a classinfo file.
//
// = SYNOPSIS
//     info2src file.ci
//
// = DESCRIPTION
//     The <info2src> searches the classinfo file specified for class member
//     functions and produces on standard output stubs suitable for inclusion
//     in the src file. In doing this, default values will be removed and if
//     the line is long then it will be split over a number of lines.
//
// = NOTES
//     If the member function is defined as inline or pure virtual then no
//     stub will be created. Nested classes are not dealt with in a sensible
//     way yet.
//
// = SEE ALSO
//     class2info(1), class2src(1)
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

AWK="@AWK@"

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
  ERROR "Usage: `basename $0` file.ci"
} 

if test $# != "1" -o "'basename $1 .ci'" = "$1"
then
  USAGE
fi

#
# Check for awk file.
#
INFO2SRC=$LIBDIR/info2src.awk

if test ! -f $INFO2SRC
then
  ERROR "Can't find $INFO2SRC"
fi

$AWK -f $INFO2SRC $1
