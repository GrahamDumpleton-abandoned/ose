#!/bin/sh

# Set some defaults.

AUTHOR=${MKSRC_AUTHOR-"???"}
ORGANISATION=${ORGANISATION-"CHANGE ME QUICK SOFTWARE"}
YEAR=${YEAR-`date +%Y`}

FILENAME=""

# Parse the command line.

while test "$#" != 0
do
  case "$1" in
    -D*=*)
      EVAL="`echo $1 | sed -e 's/^-D//' -e 's/=/=\"/' -e 's/$/\"/'`"
      eval ${EVAL}
      ;;
    -D*)
      EVAL="`echo $1 | sed -e 's/^-D//'`"
      eval ${EVAL}=1
      ;;
    -*)
      echo "Usage: `basename $0` [ -DNAME -DNAME=VALUE ] filename" 1>&2
      exit 1
      ;;
    *)
      if test "${FILENAME}" != ""
      then
	echo "Usage: `basename $0` [ -DNAME -DNAME=VALUE ] filename" 1>&2
	exit 1
      fi
      FILENAME=$1
      ;;
  esac
  shift
done

# Check that a filename has been given.

if test "${FILENAME}" = ""
then
  echo "Usage: `basename $0` [ -DNAME -DNAME=VALUE ] filename" 1>&2
  exit 1
fi

# Make sure that file doesn't already exist.

if test -f "${FILENAME}"
then
  echo "`basename $0`: ${FILENAME} already exists" 1>&2
  exit 1
fi

# Fix up variables a bit.

REALFILENAME=${FILENAME}

# Do it.

if test -z "${QUIET}"
then
  echo "`basename $0`: ${REALFILENAME}"
fi

if test -z "${SRCDIR}"
then

  cat >> ${REALFILENAME} << EOT
# Copyright ${YEAR} ${ORGANISATION}

# Makeit Initialisation.

MODULES := c cc ose

include makeit/init.mk

# Module Initialisation/Definitions.

SUBDIRS :=

PROGRAMS :=
EXCLUDE :=
NONLIBSRC :=

CPPFLAGS :=
CFLAGS :=
C++FLAGS :=
LDFLAGS :=
LDLIBS :=

include makeit/modules.mk

# Additional Dependencies/Rules.

EOT

else

  if test -z "${MAKEFILE}"
  then
    MAKEFILE="Makeit.mk"
  fi

  cat >> ${REALFILENAME} << EOT
# Copyright ${YEAR} ${ORGANISATION}

override SRCDIR := ${SRCDIR}

include \$(SRCDIR)/${MAKEFILE}
EOT

fi
