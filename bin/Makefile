## Copyright 1992 OTC LIMITED
## Copyright 1996-1998 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

ifeq "$(SRCDIR)" ""
SRCDIR := .
endif

BLDROOT := ..
SRCROOT := $(SRCDIR)/$(BLDROOT)

MODULES := install
NOMK := YES

include $(SRCROOT)/config/init.mk

## Local definitions.

ifeq "$(ENV_OODBMS)" "OSTORE"
SUBDIRS := classinfo
else
ifeq "$(OSE_HOST)" "$(OSE_TARGET)"
SUBDIRS := classinfo mksrc
else
SUBDIRS := classinfo
endif
endif

## Makeit modules.

include $(SRCROOT)/config/modules.mk
