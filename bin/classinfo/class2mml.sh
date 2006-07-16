#!/bin/sh

# =============================================================================

: << END-OF-MANUAL-PAGE
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1995-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

// [class2mml]
//
// = TITLE
//     Generates FrameMaker MML files from input files.
//
// = SYNOPSIS
//     class2mml [ -nested ] file.hh
//
// = DESCRIPTION
//     The <class2mml> program generates FrameMaker MML files from an input
//     file. The names of the generated files will correspond to the names of
//     the classes the classinfo file describes. The generated files will
//     have the extension ".mml". The <class2mml> program achieves this by
//     running <class2info> on the input file and then running <info2mml> on
//     the generated classinfo file. Options to this program are simply
//     passed onto <info2mml>.
//
//     By default, manual pages for nested classes will not be output.
//     To generated manual pages for nested classes specify the "-nested"
//     option.
//
// = FRAMEMAKER
//     To use the file with FrameMaker, create a file called "All.mml"
//     which contains the following.
//
// = BEGIN<CODE>
//     <MML>
//     <!DefineFont Code <Family Courier>>
//     <!DefineFont NoCode <Family Times>>
//     <!DefineTag Title>
//     <!DefineTag Manual>
//     <!DefineTag ManualBody>
//     <!DefineTag Section>
//     <!DefineTag SectionBody>
//     <!DefineTag Synopsis>
//     <!DefineTag SynopsisBody>
//     <!DefineTag MethodPrototype>
//     <!DefineTag MethodBody>
//     <!DefineTag CopyrightBody>
//     <!DefineTag ExampleBody>
//     <!DefineTag IndentedBody>
//     <!DefineTag PreformattedBody>
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
//     class2info(1), info2mml(1)
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
INFO2MML="$BINDIR/info2mml@CMDEXT@"

FILES=
MANEXT="mml"
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
      $INFO2MML -ext "$MANEXT" -nested $base.ci
    else
      $INFO2MML -ext "$MANEXT" $base.ci
    fi
    rm -f $base.ci
  fi
done
