## Copyright 1992 OTC LIMITED
## Copyright 1996 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

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

SUBDIRS := files program system dispatch

INCDIR := $(OSE_HOME)/include/OUX

INCLUDES := $(filter %.h %.hh,$(SRCFILES))

## Makeit modules.

include $(SRCROOT)/config/modules.mk

## Local targets/rules.
