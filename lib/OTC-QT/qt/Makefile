## Copyright 1999 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

ifeq "$(SRCDIR)" ""
SRCDIR := .
endif

BLDROOT := ../../..
SRCROOT := $(SRCDIR)/$(BLDROOT)

MODULES := c cc check install qt
MAKEIT_OPTIONS := purify shlib
CHECK_OPTIONS := short_suffixes
SHLIB_OPTIONS := suppress_library_generation

include $(SRCROOT)/config/init.mk

## Local definitions.

PROGRAMS +=

CPPFLAGS += -I../../../include

ifeq "$(_makeit_cxx_supports_cafe_repositories)" "YES"
override MKPTR := ../$(MK).ptr
endif

LDLIBS += ../../OTC/$(WSPREFIX)$(MKTAG)_$(VARIANT)/lib$(LIBEXT)
LDLIBS += ../../OSE/$(WSPREFIX)$(MKTAG)_opt/lib$(LIBEXT)

LIBVERSION := 1.0

# Following is a hack to get around fact
# that header file is in a totally different
# directory to the source code.

vpath %.hh $(SRCDIR)/../../../include/OTC/qt

override SRCFILES += $(SRCDIR)/../../../include/OTC/qt/qtqueue.hh

## Makeit modules.

include $(SRCROOT)/config/modules.mk

## Local rules.

ifeq "$(ENV_OSEINSTALLALL)" "YES"
install.always :: mostlyclean

mostlyclean.always ::
	$(RM) $(MK)/lib.so
endif
