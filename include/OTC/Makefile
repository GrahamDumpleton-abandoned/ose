## Copyright 1992 OTC LIMITED
## Copyright 1995-1999 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

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

SUBDIRS := \
 stdlib \
 misc \
 ostore \
 thread \
 debug \
 memory \
 refcnt \
 collctn \
 text \
 types \
 options \
 system \
 files \
 program \
 dispatch \
 message \
 internet \
 tcl \
 python \
 qt

INCDIR := $(OSE_HOME)/include/OTC

INCLUDES := $(filter %.h %.hh,$(SRCFILES)) PTMAP PTDUMMYS SCHEMA.cc

## Makeit modules.

include $(SRCROOT)/config/modules.mk

## Local targets/rules.

vpath PTMAP $(SRCDIR)
vpath PTDUMMYS $(SRCDIR)
