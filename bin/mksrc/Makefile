## Copyright 1996-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

ifeq "$(SRCDIR)" ""
SRCDIR := .
endif

BLDROOT := ../..
SRCROOT := $(SRCDIR)/$(BLDROOT)

MODULES := cc sh install
MAKEIT_OPTIONS := purify

include $(SRCROOT)/config/init.mk

## Local definitions.

PROGRAMS += mksrc
PROGRAMS += mkheader
PROGRAMS += mkmakefile
PROGRAMS += mksource
PROGRAMS += wrapper

CPPFLAGS += -I$(MK)
CPPFLAGS += -I../../include
CPPFLAGS += -DOTCLIB_TRACE

ifeq "$(QUICK)" "YES"
LDLIBS += \
 ../../build/$(MK)/lib$(LIBEXT) \
 ../../lib/OSE/$(WSPREFIX)$(MKTAG)_opt/lib$(LIBEXT)
else
LDLIBS += \
 ../../lib/OTC/$(MK)/lib$(LIBEXT) \
 ../../lib/OSE/$(WSPREFIX)$(MKTAG)_opt/lib$(LIBEXT)
endif

SCHEMAS = osschema.cc ../../include/OTC/SCHEMA.cc

BINDIR1 = $(OSE_HOME)/$(OSE_TARGET)/bin
EXECUTABLES1 = mksrc$(BINEXT)

BINDIR2 = $(OSE_HOME)/$(OSE_TARGET)/etc
EXECUTABLES2 = mkheader$(CMDEXT) mkmakefile$(CMDEXT) mksource$(CMDEXT)

BINDIR3 = $(OSE_HOME)/bin

AUXDIR1 = $(OSE_HOME)/$(OSE_TARGET)/etc
AUXILIARIES1 = mkheader.hlp mkmakefile.hlp mksource.hlp

SHFILTER = sed \
 -e 's%@SHELL@%$(SHELL)%' \
 -e 's%@CMDEXT@%$(CMDEXT)%' \
 -e 's%@BINEXT@%$(BINEXT)%' \
 -e 's%@OSE_ROOT@%$(OSE_ROOT)%' \
 -e 's%@OSE_RELEASE_DIRECTORY@%$(OSE_RELEASE_DIRECTORY)%'

## Makeit modules.

include $(SRCROOT)/config/modules.mk

## Local rules.

vpath %.hlp $(SRCDIR)

depend.setup :: $(MK)/config.hh

$(MK)/mksrc$(OBJEXT) : $(MK)/config.hh

$(MK)/config.hh : $(OSE_MAKEIT)/version.mk
	$(RM) $@
ifeq "$(ENV_SYSTEM)" "WIN32"
	echo '#define OSE_TARGET_HOME "$(OSE_TARGET_HOME)"' | \
	 sed -e 's%"//\([a-zA-Z]\)/%"\1:/%g' >> $@
else
	echo '#define OSE_TARGET_HOME "$(OSE_TARGET_HOME)"' >> $@
endif
	echo '#define OSE_RELEASE_DIRECTORY "$(OSE_RELEASE_DIRECTORY)"' >> $@

install-aux.always :: $(AUXDIR1)/mksrc.cf

$(AUXDIR1)/mksrc.cf : $(MK)/mksrc.cf
	$(INSTALL.AUX) $< $@

$(MK)/mksrc.cf : $(SRCDIR)/mksrc.cf.in
	cat $< | sed -e 's%@CMDEXT@%$(CMDEXT)%' > $@

install-bin.always :: $(BINDIR3)/mksrc$(CMDEXT)

$(BINDIR3)/mksrc$(CMDEXT) : $(MK)/wrapper$(CMDEXT)
	$(INSTALL.BIN) $< $@
