#!/bin/sh

# =============================================================================

: << END-OF-MANUAL-PAGE
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

// [info2man]
//
// = TITLE
//     Generates manual pages from a classinfo file.
//
// = SYNOPSIS
//     info2man [ -ext 3 ] file.ci
//     info2man [ -section name ] [ -package name ] file.ci
//     info2man [ -nested ] file.ci
//     info2man [ -output dirname ] file.ci
//
// = DESCRIPTION
//     The <info2man> program generates a standard style UNIX manual page
//     from the classinfo file specified. The names of the generated files
//     will correspond to the names of the classes the classinfo file
//     describes.
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
//     By default, any manual pages produced will be generated into the
//     current directory, regardless of where the classinfo input file
//     is located. To have generated files output to an alternate directory
//     use the "-output" option.
//
// = EXAMPLE
// = BEGIN<CODE>
//     class2info foo.hh
//     info2man foo.ci
// = END<CODE>
//
// = NOTES
//     Although reference to generating manual pages for only classes is
//     mentioned, manual pages for structs, unions and templates are
//     also generated. Further, if the syntax for standalone manual
//     pages has been used, then manual pages will be generated for that
//     also
//
// = SEE ALSO
//     class2info(1), class2man(1)
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

INFO2MAN=$LIBDIR/info2doc.awk
MANFMT=${CLASSINFOMANFMT-"$LIBDIR/info2doc.fmt"}

FILES=
MANEXT="3"
MANSECTION=
MANPACKAGE=
NESTED=
OUTPUT="."

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
    -fmt)
      shift
      if test "$#" != "0"
      then
	MANFMT=$1
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
    -output)
      shift
      if test "$#" != "0"
      then
        OUTPUT=$1
        shift
      fi
      ;;
    *.ci)
      FILES="$FILES $1"
      shift
      ;;
    *)
      USAGE
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

#
# Check for awk file etc.
#

if test ! -f $INFO2MAN
then
  ERROR "Can't find $INFO2MAN"
fi

if test ! -f $MANFMT
then
  ERROR "Can't find $MANFMT"
fi

for input in $FILES
do
  ADTS=`$AWK $VARG nested="$NESTED" '
  BEGIN {
    FS="\n"; RS=""
  }
  $1 ~ "^(CLASS|STRUCT|UNION|MANUAL)$" && $2 !~ "(::|<)" {
    printf( "%s\n", $2 )
  }
  $1 ~ "^(CLASS|STRUCT|UNION)$" && $2 ~ "::" && $2 !~ "(<)" {
    if ( nested == "1" && $2 !~ "::ANON" )
      printf( "%s\n", $2 )
  }
  $1 ~ "^TEMPLATE$" && $3 !~ "::" {
    printf( "%s\n", $3 )
  }
  $1 ~ "^TEMPLATE$" && $3 ~ "::" {
    if ( nested == "1" && $3 !~ "::ANON" )
      printf( "%s\n", $3 )
  }' $input`

  if test ! -z "$ADTS"
  then
    for ADT in $ADTS
    do
      echo "info2man: generating documentation for $ADT"

      $AWK -f $INFO2MAN $VARG pass=0 $VARG device=troff $VARG infile=$input \
       $VARG class=$ADT $VARG manext="$MANEXT" $VARG mansection="$MANSECTION" \
       $VARG manpackage="$MANPACKAGE" $MANFMT $MANFMT > \
       $OUTPUT/`echo $ADT | tr : _`.$MANEXT
    done
  fi
done
