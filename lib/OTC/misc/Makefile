## Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

ifeq "$(SRCDIR)" ""
SRCDIR := .
endif

BLDROOT := ../../..
SRCROOT := $(SRCDIR)/$(BLDROOT)

MODULES := c cc check install
MAKEIT_OPTIONS := purify shlib
CHECK_OPTIONS := short_suffixes
SHLIB_OPTIONS := suppress_library_generation

include $(SRCROOT)/config/init.mk

## Local definitions.

PROGRAMS := _funcstrm _vfuncbuf

#QUALIFIED_VARIANT += otccopy.cc:dbg=opt
#QUALIFIED_VARIANT += otccopy.cc:std=opt
#QUALIFIED_VARIANT += otccopy.cc:prf=opt
#QUALIFIED_VARIANT += otchash.cc:dbg=opt
#QUALIFIED_VARIANT += otchash.cc:std=opt
#QUALIFIED_VARIANT += otchash.cc:prf=opt
#QUALIFIED_VARIANT += otcrank.cc:dbg=opt
#QUALIFIED_VARIANT += otcrank.cc:std=opt
#QUALIFIED_VARIANT += otcrank.cc:prf=opt

CPPFLAGS += -I../../../include

LDLIBS += ../../OSE/$(WSPREFIX)$(MKTAG)_opt/lib$(LIBEXT)

LIBVERSION := 1.0

## Makeit modules.

include $(SRCROOT)/config/modules.mk

## Local rules.

ifeq "$(ENV_OSEINSTALLALL)" "YES"
install.always :: mostlyclean

mostlyclean.always ::
	$(RM) $(MK)/lib.so
endif
