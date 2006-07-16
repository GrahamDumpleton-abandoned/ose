#!/bin/sh

# Set some defaults.

AUTHOR=${MKSRC_AUTHOR-"???"}
ORGANISATION=${ORGANISATION-"CHANGE ME QUICK SOFTWARE"}
YEAR=${YEAR-`date +%Y`}

DIRNAME=""
FILENAME=""
LIBNAME=""
INCLUDE=""

REMOVE=""
GUARD=""
CLASSNAME=""

# Parse the command line.

while test "$#" != 0
do
  case "$1" in
    -remove)
      REMOVE="YES"
      ;;
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

if test -f "${FILENAME}" -a -z "${REMOVE}"
then
  echo "`basename $0`: ${FILENAME} already exists" 1>&2
  exit 1
fi

if test ! -z "$REMOVE"
then
  rm -f ${FILENAME}
fi

# Construct string to be used as header file guard.

GUARD=`basename ${FILENAME}`
if test "${DIRNAME}" != ""
then
  GUARD=${DIRNAME}_${GUARD}
fi
if test "${LIBNAME}" != ""
then
  GUARD=${LIBNAME}_${GUARD}
fi
GUARD="`echo ${GUARD} | tr '[a-z].' '[A-Z]_' | sed -e 's/-/_/g'`"

# Fix up variables a bit.

REALFILENAME=${FILENAME}
FILENAME=`basename ${FILENAME}`

if test "${DIRNAME}" != ""
then
  FILENAME=${DIRNAME}/${FILENAME}
fi
if test "${LIBNAME}" = ""
then
  LIBNAME="???"
fi

# Do it.

echo "`basename $0`: ${REALFILENAME}"

cat >> ${REALFILENAME} << EOT
#ifndef ${GUARD}
#define ${GUARD}
/*
// ============================================================================
//
// = LIBRARY
//     ${LIBNAME}
// 
// = FILENAME
//     ${FILENAME}
EOT

if test "${RCSID}" != ""
then
  cat >> ${REALFILENAME} << EOT
//
// = RCSID
//     \$Id\$
EOT
fi

if test "${SCCSID}" != ""
then
  cat >> ${REALFILENAME} << EOT
//
// = SCCSID
//     %W%
EOT
fi

cat >> ${REALFILENAME} << EOT
//
// = AUTHOR(S)
//     ${AUTHOR}
// 
// = COPYRIGHT
//     Copyright ${YEAR} ${ORGANISATION}
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */

EOT

if test "${INCLUDE}" != ""
then
  INCGUARD="`echo ${INCLUDE} | tr '[a-z]./' '[A-Z]__' | sed -e 's/-/_/g'`"
  cat >> ${REALFILENAME} << EOT
#ifndef ${INCGUARD}
#include <${INCLUDE}>
#endif
EOT
else
  if test "${CLASSNAME}" != ""
  then
    cat >> ${REALFILENAME} << EOT
class ${CLASSNAME}
    // = TITLE
    //
    // = CLASS TYPE
    //
    // = AUDIENCE
    //
    // = DESCRIPTION
    //
    // = NOTES
    //
    // = SEE ALSO
    //
    // = EXAMPLE
{
  public:
 
			${CLASSNAME}();

			${CLASSNAME}(${CLASSNAME} const& the${CLASSNAME});

			~${CLASSNAME}();

    ${CLASSNAME}&	operator=(${CLASSNAME} const& the${CLASSNAME});
};
EOT
  else
    cat >> ${REALFILENAME} << EOT
/* Place code here */
EOT
  fi
fi
cat >> ${REALFILENAME} << EOT

/* ------------------------------------------------------------------------- */

#endif /* ${GUARD} */
EOT
