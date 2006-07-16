## Copyright 1992 OTC LIMITED
## Copyright 1995 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

ifeq "$(SRCDIR)" ""
SRCDIR := .
endif

BLDROOT := ../..
SRCROOT := $(SRCDIR)/$(BLDROOT)

MODULES := install
NOMK := YES

include $(SRCROOT)/config/init.mk

## Local definitions.

INCDIR := $(OSE_HOME)/include/OSE
INCLUDES := $(filter %.h %.hh,$(SRCFILES))

## Makeit modules.

include $(SRCROOT)/config/modules.mk
