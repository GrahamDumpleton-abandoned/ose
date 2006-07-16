## Copyright 1992 OTC LIMITED
## Copyright 1995-1996 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

ifeq "$(SRCDIR)" ""
SRCDIR := .
endif

BLDROOT := ../..
SRCROOT := $(SRCDIR)/$(BLDROOT)

MODULES := cc c install
MAKEIT_OPTIONS := purify
override VARIANT := opt

include $(SRCROOT)/config/init.mk

## Local definitions.

LIBDIR := $(OSE_TARGET_HOME)/lib/$(C++COMPILERTAG)
LIBRARY := OSE

CPPFLAGS := $(CPPFLAGS) -I../../include

## Makeit Modules

include $(SRCROOT)/config/modules.mk
