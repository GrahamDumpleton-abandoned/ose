## Copyright 1992 OTC LIMITED
## Copyright 1996 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

ifeq "$(SRCDIR)" ""
SRCDIR := .
endif

BLDROOT := ../../..
SRCROOT := $(SRCDIR)/$(BLDROOT)

MODULES := install
NOMK := YES

include $(SRCROOT)/config/init.mk

## Local definitions.

INCDIR := $(OSE_HOME)/include/OUX/system
INCLUDES := $(filter %.h %.hh %.c,$(SRCFILES))

## Makeit modules.

include $(SRCROOT)/config/modules.mk
