## Copyright 1992 OTC LIMITED
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

PROGRAMS := _cmmnpool _pool _heap _align _cluster _ptr _vecptr _arena

ifneq "$(filter GNU2.5.%,$(C++COMPILER))" ""
EXCLUDE += _ptr.cc
endif

CPPFLAGS += -I../../../include

ifeq "$(_makeit_cxx_supports_cafe_repositories)" "YES"
override MKPTR := ../$(MK).ptr
endif

LDLIBS += \
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

ifeq "$(ENV_OODBMS)" "OSTORE"
ifeq "$(_makeit_cxx_supports_cfront_repositories)" "YES"
EXCLUDE += _ptr.cc _vecptr.cc
endif
endif

## Makeit modules.

include $(SRCROOT)/config/modules.mk

## Local rules.

ifeq "$(ENV_OSEINSTALLALL)" "YES"
install.always :: mostlyclean

mostlyclean.always ::
	$(RM) $(MK)/lib.so
endif
