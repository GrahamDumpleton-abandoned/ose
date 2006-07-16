## Copyright 1995-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

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

PROGRAMS := _date _time _duration

CPPFLAGS += -I../../../include

ifeq "$(_makeit_cxx_supports_cafe_repositories)" "YES"
override MKPTR := ../$(MK).ptr
endif

LDLIBS += \
 ../text/$(MK)/lib$(LIBEXT) \
 ../collctn/$(MK)/lib$(LIBEXT) \
 ../refcnt/$(MK)/lib$(LIBEXT) \
 ../memory/$(MK)/lib$(LIBEXT) \
 ../debug/$(MK)/lib$(LIBEXT) \
 ../misc/$(MK)/lib$(LIBEXT)

ifeq "$(ENV_OODBMS)" "OSTORE"
LDLIBS += ../ostore/$(MK)/lib$(LIBEXT)
endif

ifneq "$(filter -DHAVE_%_THREADS,$($(C++COMPILER)_CPPFLAGS))" ""
LDLIBS += ../thread/$(MK)/lib$(LIBEXT)
endif

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
