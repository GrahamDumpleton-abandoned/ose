## Copyright 1992 OTC LIMITED
## Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

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

EXECUTABLES1 := makeit$(CMDEXT)
EXECUTABLES2 := workarea$(CMDEXT)

PROGRAMS := makeit workarea wrapper

SHFILTER := sed -e 's%@OSE_RELEASE_DIRECTORY@%$(OSE_RELEASE_DIRECTORY)%' \
 -e 's%@OSE_ROOT@%$(OSE_ROOT)%' -e 's%@OSE_HOST@%$(OSE_HOST)%' \
 -e 's%@SHELL@%$(SHELL)%' -e 's%@BINEXT@%$(BINEXT)%' -e 's%@CMDEXT@%$(CMDEXT)%'

BINDIR1 := $(OSE_HOST_HOME)/bin
BINDIR2 := $(OSE_HOME)/bin

INCDIR := $(OSE_HOME)/include/makeit

INCLUDES := $(filter bsdinstall.sh %.sed %.mk,$(SRCFILES))
INCLUDES := $(filter-out c.mk C.mk cpp.mk cxx.mk c-cc.mk,$(INCLUDES))

_CASE_SENSITIVE = YES

ifneq "$(filter %WIN32,$(ENV_SYSTEM))" ""
_CASE_SENSITIVE = NO
endif
ifneq "$(filter %MACOSX,$(OSE_HOST))" ""
_CASE_SENSITIVE = NO
endif

#ifeq "$(_CASE_SENSITIVE)" "YES"
#INCLUDES := $(INCLUDES) c.mk C.mk cpp.mk cxx.mk c-cc.mk
#else
INCLUDES := $(INCLUDES) c.mk cpp.mk cxx.mk c-cc.mk
#endif

AUXDIR1 := $(OSE_HOST_HOME)/makeit

## Makeit modules.

include $(SRCROOT)/config/modules.mk

## Local rules.

vpath %.top $(SRCDIR)

ifeq "$(_CASE_SENSITIVE)" "YES"
C.mk : cc.mk
	cat $< | sed -e 's/cc/C/g' > $@
endif

c-cc.mk : cc.mk
	cat $< | sed -e 's/cc/c/g' > $@

cxx.mk : cc.mk
	cat $< | sed -e 's/cc/cxx/g' > $@

cpp.mk : cc.mk
	cat $< | sed -e 's/cc/cpp/g' > $@

clean.always ::
#ifeq "$(_CASE_SENSITIVE)" "YES"
#	$(RM) C.mk
#endif
	$(RM) cpp.mk cxx.mk c-cc.mk

install-bin.always :: $(BINDIR2)/makeit$(CMDEXT)

ifneq "$(ENV_TOOLSET)" "MKS"
MAKE_LOCATION = \
 $(strip $(shell which $(MAKE_COMMAND) 2>/dev/null || echo ""))

ifneq "$(filter /%,$(MAKE_LOCATION))" ""
install-bin.always :: $(OSE_HOST_HOME)/etc/makeit$(BINEXT)

$(OSE_HOST_HOME)/etc/makeit$(BINEXT) :
	$(RM) $(OSE_HOST_HOME)/etc/makeit$(BINEXT)
	ln -s $(MAKE_LOCATION) $(OSE_HOST_HOME)/etc/makeit$(BINEXT)
endif
endif

$(BINDIR2)/makeit$(CMDEXT) : $(MK)/wrapper$(CMDEXT)
	$(INSTALL.BIN) $< $@

install-aux.always :: $(AUXDIR1)/init.mk
install-aux.always :: $(AUXDIR1)/modules.mk

$(AUXDIR1)/init.mk : init.mk.top
	$(INSTALL.AUX) $< $@

$(AUXDIR1)/modules.mk : modules.mk.top
	$(INSTALL.AUX) $< $@
