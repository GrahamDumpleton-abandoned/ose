# Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

# Makeit Initialisation.

ifeq "$(SRCDIR)" ""
SRCDIR := .
endif

SRCDIRS += $(SRCDIR)/../lib/OTC/collctn
SRCDIRS += $(SRCDIR)/../lib/OTC/debug
SRCDIRS += $(SRCDIR)/../lib/OTC/dispatch
SRCDIRS += $(SRCDIR)/../lib/OTC/internet
SRCDIRS += $(SRCDIR)/../lib/OTC/memory
SRCDIRS += $(SRCDIR)/../lib/OTC/message
SRCDIRS += $(SRCDIR)/../lib/OTC/misc
SRCDIRS += $(SRCDIR)/../lib/OTC/refcnt
SRCDIRS += $(SRCDIR)/../lib/OTC/system
SRCDIRS += $(SRCDIR)/../lib/OTC/text
SRCDIRS += $(SRCDIR)/../lib/OTC/types

MODULES := cc ose check

CHECK_OPTIONS := short_suffixes

include makeit/init.mk

# Module Initialisation/Definitions.

SUBDIRS :=

PROGRAMS := $(patsubst %.cc,%,$(filter _%.cc,$(SRCFILES)))
EXCLUDE := $(filter os_schema.cc t%.cc otc%.cc,$(SRCFILES))
NONLIBSRC :=

CPPFLAGS :=
CFLAGS :=
C++FLAGS :=
LDFLAGS :=
LDLIBS :=

include makeit/modules.mk

# Additional Dependencies/Rules.

