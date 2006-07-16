## Copyright 1992 OTC LIMITED
## Copyright 1996-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

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

INCDIR := $(OSE_HOME)/include/OTC/options
INCLUDES := $(filter %.h %.hh %.i %.c,$(SRCFILES))

## Makeit modules.

include $(SRCROOT)/config/modules.mk
