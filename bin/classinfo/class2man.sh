#!/bin/sh

# =============================================================================

: << END-OF-MANUAL-PAGE
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1995-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

// [class2man]
//
// = TITLE
//     Generates manual pages from input files.
//
// = SYNOPSIS
//     class2man [ -ext 3 ] file.hh
//     class2man [ -section name ] [ -package name ] file.hh
//     class2man [ -nested ] file.hh
//
// = DESCRIPTION
//     The <class2man> program generates standard style UNIX manual page from
//     an input file. The names of the generated files will correspond to the
//     names of the classes the classinfo file describes. The <class2man>
//     program achieves this by running <class2info> on the input file and
//     then running <info2man> on the generated classinfo file. Options to
//     this program are simply passed onto <info2man>.
//
//     By default, manual pages for nested classes will not be output.
//     To generated manual pages for nested classes specify the "-nested"
//     option.
//
//     By default the suffix appended to the manual pages will be ".3". The
//     extension appended to the manual pages can be changed by specifying
//     the "-ext" option.
//
//     By default the section name output in the header of the manual
//     page will be "C++ Reference Manual". This can be overridden by
//     providing a "LIBRARY" tag in the original source file, in which
//     case "LIB Reference Manual" will be generated where "LIB" is the
//     value associated with the "LIBRARY" tag. Alternatively, the complete
//     string can be replaced by specifying the "-section" option.
//
//     By default the package name output in the footer of the manual page
//     will be "C++ Library". This can be overridden by providing a "LIBRARY"
//     tag in the original source file, in which case "LIB Library" will be
//     generated where "LIB" is the value associated with the "LIBRARY" tag.
//     Alternatively, the complete string can be replaced by specifying the
//     "-package" option.
//
// = NOTES
//     Although reference to generating manual pages for only classes is
//     mentioned, manual pages for structs, unions and templates are
//     also generated. Further, if the syntax for standalone manual
//     pages has been used, then manual pages will be generated for that
//     also
//
// = SEE ALSO
//     class2info(1), info2man(1)
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
INFO2MAN="$BINDIR/info2man@CMDEXT@"

FILES=
MANEXT="3"
MANSECTION=
MANPACKAGE=
NESTED=

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
# Parse command line.
#
while test $# != 0
do
  case $1 in
    -ext)
      shift
      if test "$#" != "0"
      then
        MANEXT=$1
        shift
      fi
      ;;
    -section)
      shift
      if test "$#" != "0"
      then
        MANSECTION=$1
        shift
      fi
      ;;
    -package)
      shift
      if test "$#" != "0"
      then
        MANPACKAGE=$1
        shift
      fi
      ;;
    -nested)
      shift
      NESTED=1
      ;;
    *)
      FILES="$FILES $1"
      shift
      ;;
    esac
done

#
# Check usage.
#
if test "$FILES" = ""
then
  USAGE
fi

for input in $FILES
do
  $CLASS2INFO $input
  if test "$?" != "0"
  then
    exit 1
  fi
  file=`basename $input`
  base="`echo $file | sed -e 's/\..*$//'`"
  if test -f "$base.ci"
  then
    if test "$NESTED" = "1"
    then
      $INFO2MAN -section "$MANSECTION" -package "$MANPACKAGE" \
       -ext "$MANEXT" -nested $base.ci
    else
      $INFO2MAN -section "$MANSECTION" -package "$MANPACKAGE" \
       -ext "$MANEXT" $base.ci
    fi
    rm -f $base.ci
  fi
done
