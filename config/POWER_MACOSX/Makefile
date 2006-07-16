## Copyright 1994-1998 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

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

ifeq "$(OSE_TARGET)" "POWER_MACOSX"
INCDIR := $(OSE_TARGET_HOME)/include
INCLUDES := OSE-CONFIG.h $(filter strstream.h,$(SRCFILES))
endif

ifeq "$(OSE_HOST)" "POWER_MACOSX"
AUXDIR := $(OSE_HOST_HOME)/include/makeit
AUXILIARIES += $(filter $(OSE_TARGET)%.mk,$(SRCFILES))
AUXILIARIES += $(filter %-options.mk,$(SRCFILES))
AUXILIARIES += $(filter %-defns.mk,$(SRCFILES))
AUXILIARIES += $(filter %-rules.mk,$(SRCFILES))
endif

## Makeit modules.

include $(SRCROOT)/config/modules.mk
