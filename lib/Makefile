## Copyright 1992 OTC LIMITED
## Copyright 1996-1999 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

ifeq "$(SRCDIR)" ""
SRCDIR := .
endif

BLDROOT := ..
SRCROOT := $(SRCDIR)/$(BLDROOT)

MODULES := install
MAKEIT_OPTIONS := navigate_all_variants
NOMK := YES

include $(SRCROOT)/config/init.mk

## Local definitions.

SUBDIRS := OSE OTC $($(C++COMPILER)_OPT_LIBRARIES)

## Makeit modules.

include $(SRCROOT)/config/modules.mk
