#!/bin/sh

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

PATTERNS=""
DSCPL=""

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
  ERROR "Usage: `basename $0` [ -link \"patterns\" ] files"
}

#
# Check usage.
#
if test "$#" = "0"
then
  USAGE
fi

#
# Parse command line.
#
if test "$1" = "-dscpl"
then
  shift
  DSCPL=1
fi

#
# Check usage again.
#
if test "$#" = "0"
then
  USAGE
fi

#
# Parse command line.
#
if test "$1" = "-link"
then
  shift
  if test "$#" != "1"
  then
    PATTERNS=$1
    shift
  fi
fi

#
# Check usage again.
#
if test "$#" = "0"
then
  USAGE
fi

while test "$#" != "0"
do
  INPUT=$1
  BASENAME=`basename $INPUT | sed -e 's/\(.*\)\.[^.]*$/\1/'`
  OUTPUT=${BASENAME}.html

  if test -f ${INPUT}
  then
    echo ${BASENAME}
    if test "$DSCPL" = "1"
    then
      cat > ${OUTPUT} <<!
<HTML>
<HEAD>
<TITLE>${BASENAME}</TITLE>
</HEAD>
<BODY BACKGROUND="background.gif" BGCOLOR="#FFFFFF">
<TABLE BORDER=0 WIDTH=600 CELLPADDING=0 CELLSPACING=0>
<TD WIDTH=120 VALIGN=TOP>
<A HREF="/"><IMG SRC="logo.gif" ALIGN=LEFT BORDER=0></A>
</TD>
<TD WIDTH=480 VALIGN=TOP>
!
    else
      cat > ${OUTPUT} <<!
<HTML>
<HEAD>
<TITLE>${BASENAME}</TITLE>
</HEAD>
<BODY>
!
    fi

    cat ${INPUT} | 
      sed  -e 's/\\-/-/g' \
	   -e 's/\\ / /g' \
	   -e 's/\\[0&]/  /g' \
	   -e 's/&/\&amp;/g' \
	   -e 's/</\&lt;/g' \
	   -e 's/>/\&gt;/g' \
	   -e 's/\\|//g' \
	   -e 's/\\e/\\/g'     |
      ${AWK} -f ${LIBDIR}/man2html1.awk | 
       sed -e 's$\\fB\([^\\]*\)\\fR$<B>\1</B></I>$g' \
	  -e 's$\\f(CO\(.[^\\]*\)\\fR$<CODE>\1</CODE>$g' \
	  -e 's$\\fI\(.[^\\]*\)\\fR$<I>\1</I></B>$g' \
	  -e 's^\\fB^<B>^g' \
	  -e 's^\\f(CO^<CODE>^g' \
	  -e 's^\\fI^<I>^g' \
	  -e 's^\\f[RP]^</B></I></CODE>^g' \
	  -e 's/^.[LP]P/<P>/' \
	  -e 's/^.br/<BR>/' \
	  -e 's/^\.DS.*/<pre>/' \
	  -e 's$^\.DE.*$</pre>$' \
	  -e 's/^\.nf */<pre>/' \
	  -e 's$^\.fi *$</pre>$' \
	  -e 's$^\.BE *$</pre><HR>$' \
	  -e 's/^\.RS.*/<UL>/' \
	  -e 's$^\.RE.*$</UL>$' \
	  -e 's$\.SH *"*\([^"]*\)"*$</pre><H2>\1</H2>$' \
	  -e '/^\.[a-zA-Z]*.*/d'	|
      ${AWK} -f ${LIBDIR}/man2html2.awk $VARG link="$PATTERNS" >> ${OUTPUT}

    if test "$DSCPL" = "1"
    then
      cat >> ${OUTPUT} <<!
</TABLE>
</BODY>
</HTML>
!
    else
      cat >> ${OUTPUT} <<!
</BODY>
</HTML>
!
    fi
  fi
  shift
done
