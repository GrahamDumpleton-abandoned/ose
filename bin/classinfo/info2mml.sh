#!/bin/sh

# =============================================================================

: << END-OF-MANUAL-PAGE
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

// [info2mml]
//
// = TITLE
//     Generates FrameMaker MML files from a classinfo file.
//
// = SYNOPSIS
//     info2mml [ -nested ] file.ci
//     info2mml [ -output dirname ] file.ci
//
// = DESCRIPTION
//     The <info2mml> program generates a FrameMaker MML file
//     from the classinfo file specified. The names of the generated files
//     will correspond to the names of the classes the classinfo file
//     describes. The suffix of the file will be ".mml".
//
//     By default, manual pages for nested classes will not be output.
//     To generated manual pages for nested classes specify the "-nested"
//     option.
//
//     By default, any manual pages produced will be generated into the
//     current directory, regardless of where the classinfo input file
//     is located. To have generated files output to an alternate directory
//     use the "-output" option.
//
// = EXAMPLE
// = BEGIN<CODE>
//     class2info foo.hh
//     info2mml foo.ci
// = END<CODE>
//
// = FRAMEMAKER
//     To use the file with FrameMaker, create a file called "All.mml"
//     which contains the following.
//
// = BEGIN<CODE>
//     <MML>
//     <!DefinePar Title>
//     <!DefinePar Manual>
//     <!DefinePar ManualBody>
//     <!DefinePar Section>
//     <!DefinePar SectionBody>
//     <!DefinePar Synopsis>
//     <!DefinePar SynopsisBody>
//     <!DefinePar Method>
//     <!DefinePar MethodBody>
//     <!DefinePar Code>
//     <!DefinePar Indent>
//     <!DefinePar NoFill>
// = END<CODE>
//
//     At the end of this file, include the generated MML files.
//
// = BEGIN<CODE>
//     <Include ClassName.mml>
// = END<CODE>
//
//     This file can then be imported by reference into a FrameMaker
//     document which defines the above paragraph formats in an appropriate
//     way to give the style of formatting desired.
//
// = NOTES
//     Although reference to generating manual pages for only classes is
//     mentioned, manual pages for structs, unions and templates are
//     also generated. Further, if the syntax for standalone manual
//     pages has been used, then manual pages will be generated for that
//     also
//
// = SEE ALSO
//     class2info(1), class2mml(1)
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
MANEXT="mml"
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
      echo "info2mml: generating documentation for $ADT"

      $AWK -f $INFO2MAN $VARG pass=0 $VARG device=mml $VARG infile=$input \
       $VARG class=$ADT $VARG manext="$MANEXT" $MANFMT $MANFMT | \
       tr -d '\r' | tr '\n' ' ' > $OUTPUT/`echo $ADT | tr : _`.$MANEXT
    done
  fi
done
