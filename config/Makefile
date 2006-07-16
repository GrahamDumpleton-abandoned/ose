## Copyright 1995-1998 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

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

SUBDIRS := $(sort $(OSE_HOST) $(OSE_TARGET))

## Makeit modules.

include $(SRCROOT)/config/modules.mk

dist :: $(wildcard */Makefile)

$(wildcard */Makefile) : %/Makefile : Makefile.in
	cat $? | sed -e 's/@SUBDIR@/$*/' > $@

distclean.always ::
	$(RM) config.status
	$(RM) config.cache*
