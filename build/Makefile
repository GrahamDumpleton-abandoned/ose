## Copyright 2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

ifeq "$(SRCDIR)" ""
SRCDIR := .
endif

BLDROOT := ..
SRCROOT := $(SRCDIR)/$(BLDROOT)

MODULES := cc install
MAKEIT_OPTIONS := shlib

SRCDIRS += ../lib/OTC/misc
SRCDIRS += ../lib/OTC/debug
SRCDIRS += ../lib/OTC/memory
SRCDIRS += ../lib/OTC/refcnt
SRCDIRS += ../lib/OTC/collctn
SRCDIRS += ../lib/OTC/text
SRCDIRS += ../lib/OTC/types
SRCDIRS += ../lib/OTC/system
SRCDIRS += ../lib/OTC/dispatch
SRCDIRS += ../lib/OTC/message
SRCDIRS += ../lib/OTC/internet
SRCDIRS += ../lib/OTC/thread
SRCDIRS += ../lib/OTC/closure

include $(SRCROOT)/config/init.mk

## Local definitions.

EXCLUDE += $(filter os_schema.cc x%.cc t%.cc _%.cc,$(SRCFILES))
ifneq "$(_makeit_cxx_supports_precompiled_templates)" "YES"
EXCLUDE += $(filter otctpl%.cc,$(SRCFILES))
endif

CPPFLAGS += -I../include

LIBDIR := $(OSE_TARGET_HOME)/lib/$(C++COMPILERTAG)
DLLDIR := $(OSE_HOME)/bin

LIBRARY_RELTAG := $(subst .,X,$(OSE_RELEASE_NAME))
LIBRARY_PLTAG := $(patsubst pl00,,pl$(OSE_MAINTENANCE_RELEASE))
LIBRARY_CXXTAG := $(subst -,_,$(subst .,X,$(C++COMPILERTAG)))

LIBRARY_TAG := $(LIBRARY_RELTAG)$(LIBRARY_PLTAG)_$(LIBRARY_CXXTAG)

LIBRARY := OTC_$(LIBRARY_TAG)_$(VARIANT)
LIBVERSION := 1.0

## Makeit modules.

include $(SRCROOT)/config/modules.mk

## Local rules.

