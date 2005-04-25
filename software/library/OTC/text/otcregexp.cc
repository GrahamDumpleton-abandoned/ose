/*
// ============================================================================
//
// = LIBRARY
//      OTC
//
// = FILENAME
//     text/otcregexp.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/regexp.hh"
#endif
#endif

#include <OTC/text/regexp.hh>
#include <OTC/thread/mxreaper.hh>

#include <string.h>
#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_Regexp::mutex_;
OTC_Regexp* OTC_Regexp::whiteSpace_ = 0;
OTC_Regexp* OTC_Regexp::optWhiteSpace_ = 0;
OTC_Regexp* OTC_Regexp::nonWhiteSpace_ = 0;
OTC_Regexp* OTC_Regexp::integerValue_ = 0;
OTC_Regexp* OTC_Regexp::realValue_ = 0;
OTC_Regexp* OTC_Regexp::alphabetic_ = 0;
OTC_Regexp* OTC_Regexp::lowerCase_ = 0;
OTC_Regexp* OTC_Regexp::upperCase_ = 0;
OTC_Regexp* OTC_Regexp::alphaNumeric_ = 0;
OTC_Regexp* OTC_Regexp::identifier_ = 0;
OTC_Regexp* OTC_Regexp::matchingQuotes_ = 0;
OTC_Regexp* OTC_Regexp::quotedString_ = 0;

/* ------------------------------------------------------------------------- */
OTC_Regexp::OTC_Regexp(char const* thePattern, u_int theMaxSubPatterns)
  : string_(0),
    regexp_(0),
    error_(0),
    maxSubPatterns_(theMaxSubPatterns+1)
{
  pattern_ = new char[strlen(thePattern)+1];
  OTCLIB_ASSERT_M(pattern_ != 0);
  strcpy(pattern_,thePattern);

  if (maxSubPatterns_ < 10)
    maxSubPatterns_ = 10;
  else if (maxSubPatterns_ > 50)
    maxSubPatterns_ = 50;

  compile();
}

/* ------------------------------------------------------------------------- */
OTC_Regexp::~OTC_Regexp()
{
  if (regexp_ != 0)
  {
    delete [] regexp_->endp;
    delete [] regexp_->startp;
    delete [] regexp_->program;
    delete regexp_;
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Regexp::compile()
{
  regexp_ = regcomp(pattern());
}

/* ------------------------------------------------------------------------- */
bool OTC_Regexp::match(char const* theString)
{
  string_ = theString;
  bool matched = regexec(regexp_,theString);
  return matched;
}

/* ------------------------------------------------------------------------- */
u_int OTC_Regexp::start(u_int theIndex) const
{
  if (regexp_ && theIndex < maxSubPatterns_
   && regexp_->startp[theIndex] != 0)
  {
    return ptrdiff_t(regexp_->startp[theIndex]) - ptrdiff_t(string_);
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
size_t OTC_Regexp::length(u_int theIndex) const
{
  if (regexp_ && theIndex < maxSubPatterns_
   && regexp_->startp[theIndex] != 0)
  {
    return ptrdiff_t(regexp_->endp[theIndex])
     - ptrdiff_t(regexp_->startp[theIndex]);
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_Range OTC_Regexp::range(u_int theIndex) const
{
  return OTC_Range(start(theIndex),length(theIndex));
}

/* ------------------------------------------------------------------------- */
bool OTC_Regexp::isValid() const
{
  // return regexp_ != 0 ? true : false;
  return error_ == 0 ? true : false;
}

/* ------------------------------------------------------------------------- */
OTC_Regexp& OTC_Regexp::whiteSpace()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (whiteSpace_ == 0)
  {
    whiteSpace_ = new OTC_Regexp("[ \n\t\r\f]+");
    OTCLIB_ASSERT_M(whiteSpace_ != 0);
  }

  xxxMutex.release();

  mutex_.unlock();

  return *whiteSpace_;
}

/* ------------------------------------------------------------------------- */
OTC_Regexp& OTC_Regexp::optWhiteSpace()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (optWhiteSpace_ == 0)
  {
    optWhiteSpace_ = new OTC_Regexp("[ \n\t\r\f]*");
    OTCLIB_ASSERT_M(optWhiteSpace_ != 0);
  }

  xxxMutex.release();

  mutex_.unlock();

  return *optWhiteSpace_;
}

/* ------------------------------------------------------------------------- */
OTC_Regexp& OTC_Regexp::nonWhiteSpace()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (nonWhiteSpace_ == 0)
  {
    nonWhiteSpace_ = new OTC_Regexp("[^ \n\t\r\f]+");
    OTCLIB_ASSERT_M(nonWhiteSpace_ != 0);
  }

  xxxMutex.release();

  mutex_.unlock();

  return *nonWhiteSpace_;
}

/* ------------------------------------------------------------------------- */
OTC_Regexp& OTC_Regexp::integerValue()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (integerValue_ == 0)
  {
    integerValue_ = new OTC_Regexp("-?[0-9]+");
    OTCLIB_ASSERT_M(integerValue_ != 0);
  }

  xxxMutex.release();

  mutex_.unlock();

  return *integerValue_;
}

/* ------------------------------------------------------------------------- */
OTC_Regexp& OTC_Regexp::realValue()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (realValue_ == 0)
  {
    realValue_ = new OTC_Regexp("-?(([0-9]+\\.[0-9]*)|([0-9]+)|(\\.[0-9]+))([eE][---+]?[0-9]+)?");
    OTCLIB_ASSERT_M(realValue_ != 0);
  }

  xxxMutex.release();

  mutex_.unlock();

  return *realValue_;
}

/* ------------------------------------------------------------------------- */
OTC_Regexp& OTC_Regexp::alphabetic()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (alphabetic_ == 0)
  {
    alphabetic_ = new OTC_Regexp("[A-Za-z]+");
    OTCLIB_ASSERT_M(alphabetic_ != 0);
  }

  xxxMutex.release();

  mutex_.unlock();

  return *alphabetic_;
}

/* ------------------------------------------------------------------------- */
OTC_Regexp& OTC_Regexp::lowerCase()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (lowerCase_ == 0)
  {
    lowerCase_ = new OTC_Regexp("[a-z]+");
    OTCLIB_ASSERT_M(lowerCase_ != 0);
  }

  xxxMutex.release();

  mutex_.unlock();

  return *lowerCase_;
}

/* ------------------------------------------------------------------------- */
OTC_Regexp& OTC_Regexp::upperCase()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (upperCase_ == 0)
  {
    upperCase_ = new OTC_Regexp("[A-Z]+");
    OTCLIB_ASSERT_M(upperCase_ != 0);
  }

  xxxMutex.release();

  mutex_.unlock();

  return *upperCase_;
}

/* ------------------------------------------------------------------------- */
OTC_Regexp& OTC_Regexp::alphaNumeric()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (alphaNumeric_ == 0)
  {
    alphaNumeric_ = new OTC_Regexp("[0-9A-Za-z]+");
    OTCLIB_ASSERT_M(alphaNumeric_ != 0);
  }

  xxxMutex.release();

  mutex_.unlock();

  return *alphaNumeric_;
}

/* ------------------------------------------------------------------------- */
OTC_Regexp& OTC_Regexp::identifier()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (identifier_ == 0)
  {
    identifier_ = new OTC_Regexp("[A-Za-z_][A-Za-z0-9_]*");
    OTCLIB_ASSERT_M(identifier_ != 0);
  }

  xxxMutex.release();

  mutex_.unlock();

  return *identifier_;
}

/* ------------------------------------------------------------------------- */
OTC_Regexp& OTC_Regexp::matchingQuotes()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (matchingQuotes_ == 0)
  {
    matchingQuotes_ = new OTC_Regexp("\"[^\"]*\"");
    OTCLIB_ASSERT_M(matchingQuotes_ != 0);
  }

  xxxMutex.release();

  mutex_.unlock();

  return *matchingQuotes_;
}

/* ------------------------------------------------------------------------- */
OTC_Regexp& OTC_Regexp::quotedString()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (quotedString_ == 0)
  {
    quotedString_ = new OTC_Regexp("\"([^\"\\\\]|\\\\\"|\\\\)*\"");
    OTCLIB_ASSERT_M(quotedString_ != 0);
  }

  xxxMutex.release();

  mutex_.unlock();

  return *quotedString_;
}

/* ------------------------------------------------------------------------- */

// #define NSUBEXP OTCLIB_NSUBEXP

/* ------------------------------------------------------------------------- */

/*
 * regcomp and regexec -- regsub and regerror are elsewhere
 *      regexp.c	1.3 of 18 April 87
 *
 *	Copyright (c) 1986 by University of Toronto.
 *	Written by Henry Spencer.  Not derived from licensed software.
 *
 *	Permission is granted to anyone to use this software for any
 *	purpose on any computer system, and to redistribute it freely,
 *	subject to the following restrictions:
 *
 *	1. The author is not responsible for the consequences of use of
 *		this software, no matter how awful, even if they arise
 *		from defects in it.
 *
 *	2. The origin of this software must not be misrepresented, either
 *		by explicit claim or by omission.
 *
 *	3. Altered versions must be plainly marked as such, and must not
 *		be misrepresented as being the original software.
 *
 * Beware that some of this code is subtly aware of the way operator
 * precedence is structured in regular expressions.  Serious changes in
 * regular-expression syntax might require a total rethink.
 */

#include <stdio.h>
#include <string.h>
#ifndef pyr
#include <stdlib.h>
#endif

/*
 * The "internal use only" fields in regexp.h are present to pass info from
 * compile to execute that permits the execute phase to run lots faster on
 * simple cases.  They are:
 *
 * regstart	char that must begin a match; '\0' if none obvious
 * reganch	is the match anchored (at beginning-of-line only)?
 * regmust	string (pointer into program) that match must include, or 0
 * regmlen	length of regmust string
 *
 * Regstart and reganch permit very fast decisions on suitable starting points
 * for a match, cutting down the work a lot.  Regmust permits fast rejection
 * of lines that cannot possibly match.  The regmust tests are costly enough
 * that regcomp() supplies a regmust only if the r.e. contains something
 * potentially expensive (at present, the only such thing detected is * or +
 * at the start of the r.e., which can involve a lot of backup).  Regmlen is
 * supplied because the test in regexec() needs it and regcomp() is computing
 * it anyway.
 */

/*
 * Structure for regexp "program".  This is essentially a linear encoding
 * of a nondeterministic finite-state machine (aka syntax charts or
 * "railroad normal form" in parsing technology).  Each node is an opcode
 * plus a "next" pointer, possibly plus an operand.  "Next" pointers of
 * all nodes except BRANCH implement concatenation; a "next" pointer with
 * a BRANCH on both ends of it is connecting two alternatives.  (Here we
 * have one of the subtle syntax dependencies:  an individual BRANCH (as
 * opposed to a collection of them) is never concatenated with anything
 * because of operator precedence.)  The operand of some types of node is
 * a literal string; for others, it is a node leading into a sub-FSM.  In
 * particular, the operand of a BRANCH node is the first node of the branch.
 * (NB this is *not* a tree structure:  the tail of the branch connects
 * to the thing following the set of BRANCHes.)  The opcodes are:
 */

#undef EOL		/* Hide definition from <OTC/OTC.h> */

/* definition	number	opnd?	meaning */
#define	END	0	/* no	End of program. */
#define	BOL	1	/* no	Match "" at beginning of line. */
#define	EOL	2	/* no	Match "" at end of line. */
#define	ANY	3	/* no	Match any one character. */
#define	ANYOF	4	/* str	Match any character in this string. */
#define	ANYBUT	5	/* str	Match any character not in this string. */
#define	BRANCH	6	/* node	Match this alternative, or the next... */
#define	BACK	7	/* no	Match "", "next" ptr points backward. */
#define	EXACTLY	8	/* str	Match this string. */
#define	NOTHING	9	/* no	Match empty string. */
#define	STAR	10	/* node	Match this (simple) thing 0 or more times. */
#define	PLUS	11	/* node	Match this (simple) thing 1 or more times. */
#define	OPEN	20	/* no	Mark this point in input as start of #n. */
			/*	OPEN+1 is number 1, etc. */
#define	CLOSE	70	/* no	Analogous to OPEN. */

/*
 * The first byte of the regexp internal "program" is actually this magic
 * number; the start node begins in the second byte.
 */
#define MAGIC   0234

/*
 * Opcode notes:
 *
 * BRANCH	The set of branches constituting a single choice are hooked
 *		together with their "next" pointers, since precedence prevents
 *		anything being concatenated to any individual branch.  The
 *		"next" pointer of the last BRANCH in a choice points to the
 *		thing following the whole choice.  This is also where the
 *		final "next" pointer of each individual branch points; each
 *		branch starts with the operand node of a BRANCH node.
 *
 * BACK		Normal "next" pointers all implicitly point forward; BACK
 *		exists to make loop structures possible.
 *
 * STAR,PLUS	'?', and complex '*' and '+', are implemented as circular
 *		BRANCH structures using BACK.  Simple cases (one character
 *		per match) are implemented with STAR and PLUS for speed
 *		and to minimize recursive plunges.
 *
 * OPEN,CLOSE	...are numbered at compile time.
 */

/*
 * A node is one char of opcode followed by two chars of "next" pointer.
 * "Next" pointers are stored as two 8-bit pieces, high order first.  The
 * value is a positive offset from the opcode of the node containing it.
 * An operand, if any, simply follows the node.  (Note that much of the
 * code generation knows about this implicit relationship.)
 *
 * Using two bytes for the "next" pointer is vast overkill for most things,
 * but allows patterns to get big without disasters.
 */
#define	OP(p)	(*(p))
#define	NEXT(p)	(((*((p)+1)&0377)<<8) + (*((p)+2)&0377))
#define	OPERAND(p)	((p) + 3)

/*
 * See regmagic.h for one further detail of program structure.
 */


/*
 * Utility definitions.
 */
#ifndef CHARBITS
#define	UCHARAT(p)	((int)*(unsigned char *)(p))
#else
#define	UCHARAT(p)	((int)*(p)&CHARBITS)
#endif

#define	FAIL(m)	{ regerror(m); return(0); }
#define	ISMULT(c)	((c) == '*' || (c) == '+' || (c) == '?')
#define	META	"^$.[()|?+*\\"

/*
 * Flags to be passed up and down.
 */
#define	HASWIDTH	01	/* Known never to match null string. */
#define	SIMPLE		02	/* Simple enough to be STAR/PLUS operand. */
#define	SPSTART		04	/* Starts with * or +. */
#define	WORST		0	/* Worst case. */

#if 0
/*
 * Global work variables for regcomp().
 */
static char const *regparse;		/* Input-scan pointer. */
static int regnpar;		/* () count. */
static char regdummy;
static char *regcode;		/* Code-emit pointer; &regdummy = don't. */
static long regsize;		/* Code size. */
#endif

/*
 - regcomp - compile a regular expression into internal code
 *
 * We can't allocate space until we know how big the compiled form will be,
 * but we can't compile it (and thus know how big it is) until we've got a
 * place to put the code.  So we cheat:  we compile it twice, once with code
 * generation turned off and size counting turned on, and once "for real".
 * This also means that we don't allocate space until we are sure that the
 * thing really will compile successfully, and we never have to move the
 * code and thus invalidate pointers into it.  (Note that it has to be in
 * one piece because free() must be able to free it all.)
 *
 * Beware that the optimization-preparation code in here knows about some
 * of the structure of the compiled regexp.
 */
OTC_RegexpStruct* OTC_Regexp::regcomp(char const* exp)
{
	register OTC_RegexpStruct *r;
	register char *scan;
	register char *longest;
	register int len;
	int flags;

	if (exp == 0)
		FAIL("0 argument");

	/* First pass: determine size, legality. */
	regparse = exp;
	regnpar = 1;
	regsize = 0L;
	regcode = &regdummy;
	regc(MAGIC);
	if (reg(0, &flags) == 0)
		return(0);

	/* Small enough for pointer-storage convention? */
	//if (regsize >= 32767L)		/* Probably could be 65535L. */
	//	FAIL("regexp too big");
	OTCLIB_ASSERT(regsize < 32767L);

	/* Allocate space. */
	// r = (regexp *)malloc(sizeof(OTC_Regexp::regexp) + (unsigned)regsize);
	// Hope the following is okay.
	r = new OTC_RegexpStruct;
	OTCLIB_ASSERT_M(r != 0);
	memset((char*)r,0,sizeof(OTC_RegexpStruct));
	r->program = new char[regsize+1];
	OTCLIB_ASSERT_M(r->program != 0);
	memset(r->program,0,regsize);
	r->startp = new char const*[maxSubPatterns_];
	OTCLIB_ASSERT_M(r->startp != 0);
	memset((char*)r->startp,0,sizeof(r->startp[0])*maxSubPatterns_);
	r->endp = new char const*[maxSubPatterns_];
	OTCLIB_ASSERT_M(r->endp != 0);
	memset((char*)r->endp,0,sizeof(r->endp[0])*maxSubPatterns_);

	/* Second pass: emit code. */
	regparse = exp;
	regnpar = 1;
	regcode = r->program;
	regc(MAGIC);
	if (reg(0, &flags) == 0)
		return(0);

	/* Dig out information for optimizations. */
	r->regstart = '\0';	/* Worst-case defaults. */
	r->reganch = 0;
	r->regmust = 0;
	r->regmlen = 0;
	scan = r->program+1;			/* First BRANCH. */
	if (OP(regnext(scan)) == END) {		/* Only one top-level choice. */
		scan = OPERAND(scan);

		/* Starting-point info. */
		if (OP(scan) == EXACTLY)
			r->regstart = *OPERAND(scan);
		else if (OP(scan) == BOL)
			r->reganch++;

		/*
		 * If there's something expensive in the r.e., find the
		 * longest literal string that must appear and make it the
		 * regmust.  Resolve ties in favor of later strings, since
		 * the regstart check works with the beginning of the r.e.
		 * and avoiding duplication strengthens checking.  Not a
		 * strong reason, but sufficient in the absence of others.
		 */
		if (flags&SPSTART) {
			longest = 0;
			len = 0;
			for (; scan != 0; scan = regnext(scan))
				if (OP(scan) == EXACTLY && (int)strlen(OPERAND(scan)) >= len) {
					longest = OPERAND(scan);
					len = strlen(OPERAND(scan));
				}
			r->regmust = longest;
			r->regmlen = len;
		}
	}

	return(r);
}

/*
 - reg - regular expression, i.e. main body or parenthesized thing
 *
 * Caller must absorb opening parenthesis.
 *
 * Combining parenthesis handling with the base level of regular expression
 * is a trifle forced, but the need to tie the tails of the branches to what
 * follows makes it hard to avoid.
 */
char* OTC_Regexp::reg(int paren, int* flagp)
{
	register char *ret;
	register char *br;
	register char *ender;
	register int parno = 0;
	int flags;

	*flagp = HASWIDTH;	/* Tentatively. */

	/* Make an OPEN node, if parenthesized. */
	if (paren) {
		if (regnpar >= int(maxSubPatterns_))
			FAIL("too many ()");
		parno = regnpar;
		regnpar++;
		ret = regnode(OPEN+parno);
	} else
		ret = 0;

	/* Pick up the branches, linking them together. */
	br = regbranch(&flags);
	if (br == 0)
		return(0);
	if (ret != 0)
		regtail(ret, br);	/* OPEN -> first. */
	else
		ret = br;
	if (!(flags&HASWIDTH))
		*flagp &= ~HASWIDTH;
	*flagp |= flags&SPSTART;
	while (*regparse == '|') {
		regparse++;
		br = regbranch(&flags);
		if (br == 0)
			return(0);
		regtail(ret, br);	/* BRANCH -> BRANCH. */
		if (!(flags&HASWIDTH))
			*flagp &= ~HASWIDTH;
		*flagp |= flags&SPSTART;
	}

	/* Make a closing node, and hook it on the end. */
	ender = regnode((paren) ? CLOSE+parno : END);	
	regtail(ret, ender);

	/* Hook the tails of the branches to the closing node. */
	for (br = ret; br != 0; br = regnext(br))
		regoptail(br, ender);

	/* Check for proper termination. */
	if (paren && *regparse++ != ')') {
		FAIL("unmatched ()");
	} else if (!paren && *regparse != '\0') {
		if (*regparse == ')') {
			FAIL("unmatched ()");
		} else
			FAIL("junk on end");	/* "Can't happen". */
		/* NOTREACHED */
	}

	return(ret);
}

/*
 - regbranch - one alternative of an | operator
 *
 * Implements the concatenation operator.
 */
char* OTC_Regexp::regbranch(int* flagp)
{
	register char *ret;
	register char *chain;
	register char *latest;
	int flags;

	*flagp = WORST;		/* Tentatively. */

	ret = regnode(BRANCH);
	chain = 0;
	while (*regparse != '\0' && *regparse != '|' && *regparse != ')') {
		latest = regpiece(&flags);
		if (latest == 0)
			return(0);
		*flagp |= flags&HASWIDTH;
		if (chain == 0)	/* First piece. */
			*flagp |= flags&SPSTART;
		else
			regtail(chain, latest);
		chain = latest;
	}
	if (chain == 0)	/* Loop ran zero times. */
		(void) regnode(NOTHING);

	return(ret);
}

/*
 - regpiece - something followed by possible [*+?]
 *
 * Note that the branching code sequences used for ? and the general cases
 * of * and + are somewhat optimized:  they use the same NOTHING node as
 * both the endmarker for their branch list and the body of the last branch.
 * It might seem that this node could be dispensed with entirely, but the
 * endmarker role is not redundant.
 */
char* OTC_Regexp::regpiece(int* flagp)
{
	register char *ret;
	register char op;
	register char *next;
	int flags;

	ret = regatom(&flags);
	if (ret == 0)
		return(0);

	op = *regparse;
	if (!ISMULT(op)) {
		*flagp = flags;
		return(ret);
	}

	if (!(flags&HASWIDTH) && op != '?')
		FAIL("*+ operand could be empty");
	*flagp = (op != '+') ? (WORST|SPSTART) : (WORST|HASWIDTH);

	if (op == '*' && (flags&SIMPLE))
		reginsert(STAR, ret);
	else if (op == '*') {
		/* Emit x* as (x&|), where & means "self". */
		reginsert(BRANCH, ret);			/* Either x */
		regoptail(ret, regnode(BACK));		/* and loop */
		regoptail(ret, ret);			/* back */
		regtail(ret, regnode(BRANCH));		/* or */
		regtail(ret, regnode(NOTHING));		/* null. */
	} else if (op == '+' && (flags&SIMPLE))
		reginsert(PLUS, ret);
	else if (op == '+') {
		/* Emit x+ as x(&|), where & means "self". */
		next = regnode(BRANCH);			/* Either */
		regtail(ret, next);
		regtail(regnode(BACK), ret);		/* loop back */
		regtail(next, regnode(BRANCH));		/* or */
		regtail(ret, regnode(NOTHING));		/* null. */
	} else if (op == '?') {
		/* Emit x? as (x|) */
		reginsert(BRANCH, ret);			/* Either x */
		regtail(ret, regnode(BRANCH));		/* or */
		next = regnode(NOTHING);		/* null. */
		regtail(ret, next);
		regoptail(ret, next);
	}
	regparse++;
	if (ISMULT(*regparse))
		FAIL("nested *?+");

	return(ret);
}

/*
 - regatom - the lowest level
 *
 * Optimization:  gobbles an entire sequence of ordinary characters so that
 * it can turn them into a single node, which is smaller to store and
 * faster to run.  Backslashed characters are exceptions, each becoming a
 * separate node; the code is simpler that way and it's not worth fixing.
 */
char* OTC_Regexp::regatom(int* flagp)
{
	register char *ret;
	int flags;

	*flagp = WORST;		/* Tentatively. */

	switch (*regparse++) {
	case '^':
		ret = regnode(BOL);
		break;
	case '$':
		ret = regnode(EOL);
		break;
	case '.':
		ret = regnode(ANY);
		*flagp |= HASWIDTH|SIMPLE;
		break;
	case '[': {
			register int cclass;
			register int classend;

			if (*regparse == '^') {	/* Complement of range. */
				ret = regnode(ANYBUT);
				regparse++;
			} else
				ret = regnode(ANYOF);
			if (*regparse == ']' || *regparse == '-')
				regc(*regparse++);
			while (*regparse != '\0' && *regparse != ']') {
				if (*regparse == '-') {
					regparse++;
					if (*regparse == ']' || *regparse == '\0')
						regc('-');
					else {
						cclass = UCHARAT(regparse-2)+1;
						classend = UCHARAT(regparse);
						if (cclass > classend+1)
							FAIL("invalid [] range");
						for (; cclass <= classend; cclass++)
							regc(cclass);
						regparse++;
					}
				} else
					regc(*regparse++);
			}
			regc('\0');
			if (*regparse != ']')
				FAIL("unmatched []");
			regparse++;
			*flagp |= HASWIDTH|SIMPLE;
		}
		break;
	case '(':
		ret = reg(1, &flags);
		if (ret == 0)
			return(0);
		*flagp |= flags&(HASWIDTH|SPSTART);
		break;
	case '\0':
	case '|':
	case ')':
		FAIL("internal urp");	/* Supposed to be caught earlier. */
		break;
	case '?':
	case '+':
	case '*':
		FAIL("?+* follows nothing");
		break;
	case '\\':
		if (*regparse == '\0')
			FAIL("trailing \\");
		ret = regnode(EXACTLY);
		regc(*regparse++);
		regc('\0');
		*flagp |= HASWIDTH|SIMPLE;
		break;
	default: {
			register int len;
			register char ender;

			regparse--;
			len = strcspn(regparse, META);
			if (len <= 0)
				FAIL("internal disaster");
			ender = *(regparse+len);
			if (len > 1 && ISMULT(ender))
				len--;		/* Back off clear of ?+* operand. */
			*flagp |= HASWIDTH;
			if (len == 1)
				*flagp |= SIMPLE;
			ret = regnode(EXACTLY);
			while (len > 0) {
				regc(*regparse++);
				len--;
			}
			regc('\0');
		}
		break;
	}

	return(ret);
}

/*
 - regnode - emit a node
 */
char* OTC_Regexp::regnode(char op)
{
	register char *ret;
	register char *ptr;

	ret = regcode;
	if (ret == &regdummy) {
		regsize += 3;
		return(ret);
	}

	ptr = ret;
	*ptr++ = op;
	*ptr++ = '\0';		/* Null "next" pointer. */
	*ptr++ = '\0';
	regcode = ptr;

	return(ret);
}

/*
 - regc - emit (if appropriate) a byte of code
 */
void OTC_Regexp::regc(char b)
{
	if (regcode != &regdummy)
		*regcode++ = b;
	else
		regsize++;
}

/*
 - reginsert - insert an operator in front of already-emitted operand
 *
 * Means relocating the operand.
 */
void OTC_Regexp::reginsert(char op, char* opnd)
{
	register char *src;
	register char *dst;
	register char *place;

	if (regcode == &regdummy) {
		regsize += 3;
		return;
	}

	src = regcode;
	regcode += 3;
	dst = regcode;
	while (src > opnd)
		*--dst = *--src;

	place = opnd;		/* Op node, where operand used to be. */
	*place++ = op;
	*place++ = '\0';
	*place++ = '\0';
}

/*
 - regtail - set the next-pointer at the end of a node chain
 */
void OTC_Regexp::regtail(char* p, char* val)
{
	register char *scan;
	register char *temp;
	register int offset;

	if (p == &regdummy)
		return;

	/* Find last node. */
	scan = p;
	for (;;) {
		temp = regnext(scan);
		if (temp == 0)
			break;
		scan = temp;
	}

	if (OP(scan) == BACK)
		offset = scan - val;
	else
		offset = val - scan;
	*(scan+1) = (offset>>8)&0377;
	*(scan+2) = offset&0377;
}

/*
 - regoptail - regtail on operand of first argument; nop if operandless
 */
void OTC_Regexp::regoptail(char* p, char* val)
{
	/* "Operandless" and "op != BRANCH" are synonymous in practice. */
	if (p == 0 || p == &regdummy || OP(p) != BRANCH)
		return;
	regtail(OPERAND(p), val);
}

/*
 * regexec and friends
 */

#if 0
/*
 * Global work variables for regexec().
 */
static char const *reginput;		/* String-input pointer. */
static char const *regbol;		/* Beginning of input, for ^ check. */
static char const **regstartp;	/* Pointer to startp array. */
static char const **regendp;		/* Ditto for endp. */
#endif

#ifdef DEBUG
int regnarrate = 0;
void regdump();
static char *regprop();
#endif

/*
 - regexec - match a regexp against a string
 */
int
OTC_Regexp::regexec(OTC_RegexpStruct* prog, char const* string)
{
	register char const *s;

	/* Be paranoid... */
	if (prog == 0 || string == 0) {
		regerror("0 parameter");
		return(0);
	}

	/* Check validity of program. */
	if (UCHARAT(prog->program) != MAGIC) {
		regerror("corrupted program");
		return(0);
	}

	/* If there is a "must appear" string, look for it. */
	if (prog->regmust != 0) {
		s = string;
		while ((s = strchr(s, prog->regmust[0])) != 0) {
			if (strncmp(s, prog->regmust, prog->regmlen) == 0)
				break;	/* Found it. */
			s++;
		}
		if (s == 0)	/* Not present. */
			return(0);
	}

	/* Mark beginning of line for ^ . */
	regbol = string;

	/* Simplest case:  anchored match need be tried only once. */
	if (prog->reganch)
		return(regtry(prog, string));

	/* Messy cases:  unanchored match. */
	s = string;
	if (prog->regstart != '\0')
		/* We know what char it must start with. */
		while ((s = strchr(s, prog->regstart)) != 0) {
			if (regtry(prog, s))
				return(1);
			s++;
		}
	else
		/* We don't -- general case. */
		do {
			if (regtry(prog, s))
				return(1);
		} while (*s++ != '\0');

	/* Failure. */
	return(0);
}

/*
 - regtry - try match at specific point
 */
/* Returns 0 failure, 1 success */
int OTC_Regexp::regtry(OTC_RegexpStruct* prog, char const* string)
{
	register int i;
	register char const **sp;
	register char const **ep;

	reginput = string;
	regstartp = prog->startp;
	regendp = prog->endp;

	sp = prog->startp;
	ep = prog->endp;
	for (i = maxSubPatterns_; i > 0; i--) {
		*sp++ = 0;
		*ep++ = 0;
	}
	if (regmatch(prog->program + 1)) {
		prog->startp[0] = string;
		prog->endp[0] = reginput;
		return(1);
	} else
		return(0);
}

/*
 - regmatch - main matching routine
 *
 * Conceptually the strategy is simple:  check to see whether the current
 * node matches, call self recursively to see whether the rest matches,
 * and then act accordingly.  In practice we make some effort to avoid
 * recursion, in particular by going through "ordinary" nodes (that don't
 * need to know whether the rest of the match failed) by a loop instead of
 * by recursion.
 */
/* Returns 0 failure, 1 success */
int OTC_Regexp::regmatch(char* prog)
{
	register char *scan;	/* Current node. */
	char *next;		/* Next node. */

	scan = prog;
#ifdef DEBUG
	if (scan != 0 && regnarrate)
		fprintf(stderr, "%s(\n", regprop(scan));
#endif
	while (scan != 0) {
#ifdef DEBUG
		if (regnarrate)
			fprintf(stderr, "%s...\n", regprop(scan));
#endif
		next = regnext(scan);

		switch (OP(scan)) {
		case BOL:
			if (reginput != regbol)
				return(0);
			break;
		case EOL:
			if (*reginput != '\0')
				return(0);
			break;
		case ANY:
			if (*reginput == '\0')
				return(0);
			reginput++;
			break;
		case EXACTLY: {
				register int len;
				register char *opnd;

				opnd = OPERAND(scan);
				/* Inline the first character, for speed. */
				if (*opnd != *reginput)
					return(0);
				len = strlen(opnd);
				if (len > 1 && strncmp(opnd, reginput, len) != 0)
					return(0);
				reginput += len;
			}
			break;
		case ANYOF:
			if (*reginput == '\0' || strchr(OPERAND(scan), *reginput) == 0)
				return(0);
			reginput++;
			break;
		case ANYBUT:
			if (*reginput == '\0' || strchr(OPERAND(scan), *reginput) != 0)
				return(0);
			reginput++;
			break;
		case NOTHING:
			break;
		case BACK:
			break;
		case OPEN+1:
		case OPEN+2:
		case OPEN+3:
		case OPEN+4:
		case OPEN+5:
		case OPEN+6:
		case OPEN+7:
		case OPEN+8:
		case OPEN+9:
		case OPEN+10:
		case OPEN+11:
		case OPEN+12:
		case OPEN+13:
		case OPEN+14:
		case OPEN+15:
		case OPEN+16:
		case OPEN+17:
		case OPEN+18:
		case OPEN+19:
		case OPEN+20:
		case OPEN+21:
		case OPEN+22:
		case OPEN+23:
		case OPEN+24:
		case OPEN+25:
		case OPEN+26:
		case OPEN+27:
		case OPEN+28:
		case OPEN+29:
		case OPEN+30:
		case OPEN+31:
		case OPEN+32:
		case OPEN+33:
		case OPEN+34:
		case OPEN+35:
		case OPEN+36:
		case OPEN+37:
		case OPEN+38:
		case OPEN+39:
		case OPEN+40:
		case OPEN+41:
		case OPEN+42:
		case OPEN+43:
		case OPEN+44:
		case OPEN+45:
		case OPEN+46:
		case OPEN+47:
		case OPEN+48:
		case OPEN+49: {
				register int no;
				register char const *save;

				no = OP(scan) - OPEN;
				save = reginput;

				if (regmatch(next)) {
					/*
					 * Don't set startp if some later
					 * invocation of the same parentheses
					 * already has.
					 */
					if (regstartp[no] == 0)
						regstartp[no] = save;
					return(1);
				} else
					return(0);
			}
			break;
		case CLOSE+1:
		case CLOSE+2:
		case CLOSE+3:
		case CLOSE+4:
		case CLOSE+5:
		case CLOSE+6:
		case CLOSE+7:
		case CLOSE+8:
		case CLOSE+9:
		case CLOSE+10:
		case CLOSE+11:
		case CLOSE+12:
		case CLOSE+13:
		case CLOSE+14:
		case CLOSE+15:
		case CLOSE+16:
		case CLOSE+17:
		case CLOSE+18:
		case CLOSE+19:
		case CLOSE+20:
		case CLOSE+21:
		case CLOSE+22:
		case CLOSE+23:
		case CLOSE+24:
		case CLOSE+25:
		case CLOSE+26:
		case CLOSE+27:
		case CLOSE+28:
		case CLOSE+29:
		case CLOSE+30:
		case CLOSE+31:
		case CLOSE+32:
		case CLOSE+33:
		case CLOSE+34:
		case CLOSE+35:
		case CLOSE+36:
		case CLOSE+37:
		case CLOSE+38:
		case CLOSE+39:
		case CLOSE+40:
		case CLOSE+41:
		case CLOSE+42:
		case CLOSE+43:
		case CLOSE+44:
		case CLOSE+45:
		case CLOSE+46:
		case CLOSE+47:
		case CLOSE+48:
		case CLOSE+49: {
				register int no;
				register char const *save;

				no = OP(scan) - CLOSE;
				save = reginput;

				if (regmatch(next)) {
					/*
					 * Don't set endp if some later
					 * invocation of the same parentheses
					 * already has.
					 */
					if (regendp[no] == 0)
						regendp[no] = save;
					return(1);
				} else
					return(0);
			}
			break;
		case BRANCH: {
				register char const *save;

				if (OP(next) != BRANCH)		/* No choice. */
					next = OPERAND(scan);	/* Avoid recursion. */
				else {
					do {
						save = reginput;
						if (regmatch(OPERAND(scan)))
							return(1);
						reginput = save;
						scan = regnext(scan);
					} while (scan != 0 && OP(scan) == BRANCH);
					return(0);
					/* NOTREACHED */
				}
			}
			break;
		case STAR:
		case PLUS: {
				register char nextch;
				register int no;
				register char const *save;
				register int min;

				/*
				 * Lookahead to avoid useless match attempts
				 * when we know what character comes next.
				 */
				nextch = '\0';
				if (OP(next) == EXACTLY)
					nextch = *OPERAND(next);
				min = (OP(scan) == STAR) ? 0 : 1;
				save = reginput;
				no = regrepeat(OPERAND(scan));
				while (no >= min) {
					/* If it could work, try it. */
					if (nextch == '\0' || *reginput == nextch)
						if (regmatch(next))
							return(1);
					/* Couldn't or didn't -- back up. */
					no--;
					reginput = save + no;
				}
				return(0);
			}
			break;
		case END:
			return(1);	/* Success! */
			break;
		default:
			regerror("memory corruption");
			return(0);
			break;
		}

		scan = next;
	}

	/*
	 * We get here only if there's trouble -- normally "case END" is
	 * the terminating point.
	 */
	regerror("corrupted pointers");
	return(0);
}

/*
 - regrepeat - repeatedly match something simple, report how many
 */
int OTC_Regexp::regrepeat(char* p)
{
	register int count = 0;
	register char const *scan;
	register char *opnd;

	scan = reginput;
	opnd = OPERAND(p);
	switch (OP(p)) {
	case ANY:
		count = strlen(scan);
		scan += count;
		break;
	case EXACTLY:
		while (*opnd == *scan) {
			count++;
			scan++;
		}
		break;
	case ANYOF:
		while (*scan != '\0' && strchr(opnd, *scan) != 0) {
			count++;
			scan++;
		}
		break;
	case ANYBUT:
		while (*scan != '\0' && strchr(opnd, *scan) == 0) {
			count++;
			scan++;
		}
		break;
	default:		/* Oh dear.  Called inappropriately. */
		regerror("internal foulup");
		count = 0;	/* Best compromise. */
		break;
	}
	reginput = scan;

	return(count);
}

/*
 - regnext - dig the "next" pointer out of a node
 */
char* OTC_Regexp::regnext(char* p)
{
	register int offset;

	if (p == &regdummy)
		return(0);

	offset = NEXT(p);
	if (offset == 0)
		return(0);

	if (OP(p) == BACK)
		return(p-offset);
	else
		return(p+offset);
}

#ifdef DEBUG

static char *regprop();

/*
 - regdump - dump a regexp onto stdout in vaguely comprehensible form
 */
void
regdump(r)
regexp *r;
{
	register char *s;
	register char op = EXACTLY;	/* Arbitrary non-END op. */
	register char *next;
	extern char *strchr();


	s = r->program + 1;
	while (op != END) {	/* While that wasn't END last time... */
		op = OP(s);
		printf("%2d%s", s-r->program, regprop(s));	/* Where, what. */
		next = regnext(s);
		if (next == 0)		/* Next ptr. */
			printf("(0)");
		else 
			printf("(%d)", (s-r->program)+(next-s));
		s += 3;
		if (op == ANYOF || op == ANYBUT || op == EXACTLY) {
			/* Literal string, where present. */
			while (*s != '\0') {
				putchar(*s);
				s++;
			}
			s++;
		}
		putchar('\n');
	}

	/* Header fields of interest. */
	if (r->regstart != '\0')
		printf("start `%c' ", r->regstart);
	if (r->reganch)
		printf("anchored ");
	if (r->regmust != 0)
		printf("must have \"%s\"", r->regmust);
	printf("\n");
}

/*
 - regprop - printable representation of opcode
 */
static char *
regprop(op)
char *op;
{
	register char *p;
	static char buf[50];

	(void) strcpy(buf, ":");

	switch (OP(op)) {
	case BOL:
		p = "BOL";
		break;
	case EOL:
		p = "EOL";
		break;
	case ANY:
		p = "ANY";
		break;
	case ANYOF:
		p = "ANYOF";
		break;
	case ANYBUT:
		p = "ANYBUT";
		break;
	case BRANCH:
		p = "BRANCH";
		break;
	case EXACTLY:
		p = "EXACTLY";
		break;
	case NOTHING:
		p = "NOTHING";
		break;
	case BACK:
		p = "BACK";
		break;
	case END:
		p = "END";
		break;
	case OPEN+1:
	case OPEN+2:
	case OPEN+3:
	case OPEN+4:
	case OPEN+5:
	case OPEN+6:
	case OPEN+7:
	case OPEN+8:
	case OPEN+9:
	case OPEN+10:
	case OPEN+11:
	case OPEN+12:
	case OPEN+13:
	case OPEN+14:
	case OPEN+15:
	case OPEN+16:
	case OPEN+17:
	case OPEN+18:
	case OPEN+19:
	case OPEN+20:
	case OPEN+21:
	case OPEN+22:
	case OPEN+23:
	case OPEN+24:
	case OPEN+25:
	case OPEN+26:
	case OPEN+27:
	case OPEN+28:
	case OPEN+29:
	case OPEN+30:
	case OPEN+31:
	case OPEN+32:
	case OPEN+33:
	case OPEN+34:
	case OPEN+35:
	case OPEN+36:
	case OPEN+37:
	case OPEN+38:
	case OPEN+39:
	case OPEN+40:
	case OPEN+41:
	case OPEN+42:
	case OPEN+43:
	case OPEN+44:
	case OPEN+45:
	case OPEN+46:
	case OPEN+47:
	case OPEN+48:
	case OPEN+49:
		sprintf(buf+strlen(buf), "OPEN%d", OP(op)-OPEN);
		p = 0;
		break;
	case CLOSE+1:
	case CLOSE+2:
	case CLOSE+3:
	case CLOSE+4:
	case CLOSE+5:
	case CLOSE+6:
	case CLOSE+7:
	case CLOSE+8:
	case CLOSE+9:
	case CLOSE+10:
	case CLOSE+11:
	case CLOSE+12:
	case CLOSE+13:
	case CLOSE+14:
	case CLOSE+15:
	case CLOSE+16:
	case CLOSE+17:
	case CLOSE+18:
	case CLOSE+19:
	case CLOSE+20:
	case CLOSE+21:
	case CLOSE+22:
	case CLOSE+23:
	case CLOSE+24:
	case CLOSE+25:
	case CLOSE+26:
	case CLOSE+27:
	case CLOSE+28:
	case CLOSE+29:
	case CLOSE+30:
	case CLOSE+31:
	case CLOSE+32:
	case CLOSE+33:
	case CLOSE+34:
	case CLOSE+35:
	case CLOSE+36:
	case CLOSE+37:
	case CLOSE+38:
	case CLOSE+39:
	case CLOSE+40:
	case CLOSE+41:
	case CLOSE+42:
	case CLOSE+43:
	case CLOSE+44:
	case CLOSE+45:
	case CLOSE+46:
	case CLOSE+47:
	case CLOSE+48:
	case CLOSE+49:
		sprintf(buf+strlen(buf), "CLOSE%d", OP(op)-CLOSE);
		p = 0;
		break;
	case STAR:
		p = "STAR";
		break;
	case PLUS:
		p = "PLUS";
		break;
	default:
		regerror("corrupted opcode");
		break;
	}
	if (p != 0)
		(void) strcat(buf, p);
	return(buf);
}
#endif

/*
 * The following is provided for those people who do not have strcspn() in
 * their C libraries.  They should get off their butts and do something
 * about it; at least one public-domain implementation of those (highly
 * useful) string routines has been published on Usenet.
 */
#ifdef STRCSPN
/*
 * strcspn - find length of initial segment of s1 consisting entirely
 * of characters not from s2
 */

static int
strcspn(s1, s2)
char *s1;
char *s2;
{
	register char *scan1;
	register char *scan2;
	register int count;

	count = 0;
	for (scan1 = s1; *scan1 != '\0'; scan1++) {
		for (scan2 = s2; *scan2 != '\0';)	/* ++ moved down. */
			if (*scan1 == *scan2++)
				return(count);
		count++;
	}
	return(count);
}
#endif

/* ------------------------------------------------------------------------- */
