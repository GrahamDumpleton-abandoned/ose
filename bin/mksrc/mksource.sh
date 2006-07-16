#!/bin/sh

# Set some defaults.

AUTHOR=${MKSRC_AUTHOR-"???"}
ORGANISATION=${ORGANISATION-"CHANGE ME QUICK SOFTWARE"}
YEAR=${YEAR-`date +%Y`}

DIRNAME=""
FILENAME=""
LIBNAME=""

GUARD=""
CLASS2SRC=""
MAIN=""
TEST=""

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

# Construct string to be used as header file guard.

if test "${GUARD}" != ""
then
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
fi

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

if test "${GUARD}" != ""
then
  cat >> ${REALFILENAME} << EOT
#ifndef ${GUARD}
#define ${GUARD}
EOT
fi

cat >> ${REALFILENAME} << EOT
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

EOT

if test "${CLASS2SRC}" != "" -a -f "${CLASS2SRC}"
then
  class2src ${CLASS2SRC} >> ${REALFILENAME}
else
  if test "${MAIN}" != ""
  then
    cat >> ${REALFILENAME} << EOT
#include <OTC/system/program.hh>

/* ------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
  set_terminate(otclib_terminate_function);

  OTC_Program::initialise(argc,argv);

  /* Place code here */

  cout << "hello world" << endl;

  return 0;
}

/* ------------------------------------------------------------------------- */
EOT
  else
    if test "${TEST}" != ""
    then
      cat >> ${REALFILENAME} << EOT
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_Tracer tracer("void test1()");

  // ...
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1
};

/* ------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
  u_int const numTests = sizeof(tests)/sizeof(tests[0]);

  set_terminate(otclib_terminate_handler);

  if (argc != 2)
  {
    cout << numTests << endl;
    return 1;
  }
  else
  {
    int testNum = atoi(argv[1]);
    if (testNum > 0 && u_int(testNum) <= numTests)
    {
      tests[testNum-1]();
      return 0;
    }
    else
      return 1;
  }
}

/* ------------------------------------------------------------------------- */
EOT
    else
      cat >> ${REALFILENAME} << EOT
/* ------------------------------------------------------------------------- */

/* Place code here */

/* ------------------------------------------------------------------------- */
EOT
    fi
  fi
fi

if test "${GUARD}" != ""
then
  cat >> ${REALFILENAME} << EOT

#endif /* ${GUARD} */
EOT
fi
