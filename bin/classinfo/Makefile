## Copyright 1992-1993 OTC LIMITED
## Copyright 1994 TELSTRA CORPORATION LIMITED
## Copyright 1994-1999 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

ifeq "$(SRCDIR)" ""
SRCDIR := .
endif

BLDROOT := ../..
SRCROOT := $(SRCDIR)/$(BLDROOT)

MODULES := sh install

ifndef VARIANT
VARIANT := opt
endif

include $(SRCROOT)/config/init.mk

## Local definitions.

EXECUTABLES1 += class2info$(CMDEXT) class2man$(CMDEXT) class2src$(CMDEXT)
EXECUTABLES1 += info2man$(CMDEXT) info2src$(CMDEXT) info2mml$(CMDEXT)
EXECUTABLES1 += class2mml$(CMDEXT) man2html$(CMDEXT) info2html$(CMDEXT)
EXECUTABLES1 += class2html$(CMDEXT)

PROGRAMS += class2info class2man class2src
PROGRAMS += info2man info2src info2mml
PROGRAMS += class2mml man2html info2html
PROGRAMS += class2html wrapper

BINDIR1 := $(OSE_HOST_HOME)/bin
BINDIR2 := $(OSE_HOME)/bin

ifeq "$(AWK)" ""
AWK = gawk
endif

SHFILTER := sed -e 's%@OSE_RELEASE_DIRECTORY@%$(OSE_RELEASE_DIRECTORY)%' \
 -e 's%@OSE_ROOT@%$(OSE_ROOT)%' -e 's%@OSE_HOST@%$(OSE_HOST)%' \
 -e 's%@AWK@%$(AWK)%' -e 's%@SHELL@%$(SHELL)%' -e 's%@CMDEXT@%$(CMDEXT)%'

AUXDIR1 := $(OSE_HOME)/lib
AUXILIARIES1 := $(filter %.awk %.fmt,$(SRCFILES))

MANDIR := $(OSE_HOME)/man
SECTION1 := $(filter %.1,$(SRCFILES))

MANEXT=1
SECTION="User Commands"
PACKAGE="ClassInfo Tools"

## Makeit modules.

vpath %.awk $(SRCDIR)
vpath %.fmt $(SRCDIR)

include $(SRCROOT)/config/modules.mk

install-bin.always :: $(BINDIR2)/class2info$(CMDEXT)
install-bin.always :: $(BINDIR2)/class2man$(CMDEXT)
install-bin.always :: $(BINDIR2)/class2mml$(CMDEXT)
install-bin.always :: $(BINDIR2)/class2src$(CMDEXT)
install-bin.always :: $(BINDIR2)/info2man$(CMDEXT)
install-bin.always :: $(BINDIR2)/info2mml$(CMDEXT)
install-bin.always :: $(BINDIR2)/info2src$(CMDEXT)
install-bin.always :: $(BINDIR2)/man2html$(CMDEXT)
install-bin.always :: $(BINDIR2)/info2html$(CMDEXT)
install-bin.always :: $(BINDIR2)/class2html$(CMDEXT)

$(BINDIR2)/class2info$(CMDEXT) : $(MK)/wrapper$(CMDEXT)
	$(INSTALL.BIN) $< $@

$(BINDIR2)/class2man$(CMDEXT) : $(MK)/wrapper$(CMDEXT)
	$(INSTALL.BIN) $< $@

$(BINDIR2)/class2mml$(CMDEXT) : $(MK)/wrapper$(CMDEXT)
	$(INSTALL.BIN) $< $@

$(BINDIR2)/class2src$(CMDEXT) : $(MK)/wrapper$(CMDEXT)
	$(INSTALL.BIN) $< $@

$(BINDIR2)/info2man$(CMDEXT) : $(MK)/wrapper$(CMDEXT)
	$(INSTALL.BIN) $< $@

$(BINDIR2)/info2mml$(CMDEXT) : $(MK)/wrapper$(CMDEXT)
	$(INSTALL.BIN) $< $@

$(BINDIR2)/info2src$(CMDEXT) : $(MK)/wrapper$(CMDEXT)
	$(INSTALL.BIN) $< $@

$(BINDIR2)/man2html$(CMDEXT) : $(MK)/wrapper$(CMDEXT)
	$(INSTALL.BIN) $< $@

$(BINDIR2)/info2html$(CMDEXT) : $(MK)/wrapper$(CMDEXT)
	$(INSTALL.BIN) $< $@

$(BINDIR2)/class2html$(CMDEXT) : $(MK)/wrapper$(CMDEXT)
	$(INSTALL.BIN) $< $@

build-documentation ::
	class2man -ext $(MANEXT) -section $(SECTION) -package $(PACKAGE) *.sh
