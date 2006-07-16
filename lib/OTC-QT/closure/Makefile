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

CPPFLAGS += -I../../../include

LDLIBS += \
 ../qt/$(MK)/lib$(LIBEXT)

LDLIBS += ../../OTC/$(WSPREFIX)$(MKTAG)_$(VARIANT)/lib$(LIBEXT)
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
