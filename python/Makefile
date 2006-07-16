# Copyright 2000-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

STANDALONE = YES

## Initialisation.

VARIANT = opt

ifeq "$(SRCDIR)" ""
SRCDIR := .
endif

BLDROOT := ..

ifeq "$(STANDALONE)" "YES"
SRCROOT := $(SRCDIR)/$(BLDROOT)
CFGROOT := $(SRCROOT)/config
MODULES = cc python install sh
else
CFGROOT := $(OSE_ROOT)/$(OSE_RELEASE_DIRECTORY)/$(OSE_HOST)/makeit
MODULES = cc python install sh ose
endif

MAKEIT_OPTIONS = shlib

SHLIB_OPTIONS += loadable_module

ifeq "$(STANDALONE)" "YES"
SRCDIRS += ../lib/OSE
SRCDIRS += ../lib/OTC/misc
SRCDIRS += ../lib/OTC/debug
SRCDIRS += ../lib/OTC/memory
SRCDIRS += ../lib/OTC/refcnt
SRCDIRS += ../lib/OTC/collctn
SRCDIRS += ../lib/OTC/text
SRCDIRS += ../lib/OTC/types
SRCDIRS += ../lib/OTC/system
SRCDIRS += ../lib/OTC/dispatch
SRCDIRS += ../lib/OTC/message
SRCDIRS += ../lib/OTC/internet
SRCDIRS += ../lib/OTC/thread
SRCDIRS += ../lib/OTC/closure
endif

SRCDIRS += ../lib/OTC-PY/python

include $(CFGROOT)/init.mk

ifeq "$(STANDALONE)" "YES"
EXCLUDE += $(filter os_schema.cc x%.cc t%.cc _%.cc,$(SRCFILES))
ifneq "$(_makeit_cxx_supports_precompiled_templates)" "YES"
EXCLUDE += $(filter otctpl%.cc,$(SRCFILES))
endif
endif

## Local Definitions.

CPPFLAGS += -I../include
CPPFLAGS += -DOTCLIB_TRACE

INSTALLDIRS += $(OSE_HOME)
INSTALLDIRS += $(OSE_HOME)/lib
INSTALLDIRS += $(OSE_TARGET_HOME)
INSTALLDIRS += $(OSE_TARGET_HOME)/lib
INSTALLDIRS += $(OSE_TARGET_HOME)/lib/python

AUXDIR1 = $(OSE_HOME)/lib/python
AUXDIR2 = $(OSE_HOME)/lib/python-apps

LIBDIR = $(OSE_TARGET_HOME)/lib/python

INSTALLDIRS += $(OSE_HOME)/lib/python/netrpc
INSTALLDIRS += $(OSE_HOME)/lib/python/netsvc
INSTALLDIRS += $(OSE_HOME)/lib/python/netsvc/xmlrpc
INSTALLDIRS += $(OSE_HOME)/lib/python/netsvc/soap
INSTALLDIRS += $(OSE_HOME)/lib/python/netsvc/spyon

AUXILIARIES1 += netrpc/common.py
AUXILIARIES1 += netrpc/remote.py
AUXILIARIES1 += netrpc/xmlrpc.py
AUXILIARIES1 += netrpc/soap.py

AUXILIARIES1 += netsvc/xmlrpc/__init__.py
AUXILIARIES1 += netsvc/xmlrpc/internal1.py
AUXILIARIES1 += netsvc/xmlrpc/internal2.py
AUXILIARIES1 += netsvc/xmlrpc/external1.py
AUXILIARIES1 += netsvc/xmlrpc/external2.py

AUXILIARIES1 += netsvc/soap/__init__.py
AUXILIARIES1 += netsvc/soap/external1.py
AUXILIARIES1 += netsvc/soap/external2.py
AUXILIARIES1 += netsvc/soap/external3.py

AUXILIARIES1 += netsvc/spyon/__init__.py
AUXILIARIES1 += netsvc/spyon/StartupDialog.py
AUXILIARIES1 += netsvc/spyon/AgentTracker.py
AUXILIARIES1 += netsvc/spyon/AgentBrowser.py
AUXILIARIES1 += netsvc/spyon/AgentWindow.py
AUXILIARIES1 += netsvc/spyon/ServiceTracker.py
AUXILIARIES1 += netsvc/spyon/ServiceBrowser.py
AUXILIARIES1 += netsvc/spyon/ServiceWindow.py
AUXILIARIES1 += netsvc/spyon/GroupBrowser.py

AUXILIARIES2 += spyon.py

BINDIR1 = $(OSE_HOME)/bin

PROGRAMS += spyon

SHFILTER = sed \
 -e 's%@OSE_ROOT@%$(OSE_ROOT)%' \
 -e 's%@OSE_RELEASE_DIRECTORY@%$(OSE_RELEASE_DIRECTORY)%' \
 -e 's%@PYTHON@%$(PYTHON)%'

ifeq "$(OSE_TARGET)" "X86_WIN32"
OBJECT = _ose
else
OBJECT = _osemodule
endif

## Makeit modules.

include $(CFGROOT)/modules.mk

## Local Rules.

python-build :: all
	$(PYTHON) setup.py build
	cp $(MK)/$(OBJECT)$(SHLEXT) build/lib/netsvc

python-install :: python-build
ifneq "$(PYTHON_HOME)" ""
	$(PYTHON) setup.py install --home=$(PYTHON_HOME)
else
ifneq "$(PYTHON_PREFIX)" ""
	$(PYTHON) setup.py install --prefix=$(PYTHON_PREFIX)
else
	$(PYTHON) setup.py install
endif
endif

python-bdist :: python-build
ifeq "$(OSE_HOST)" "X86_WIN32"
	$(PYTHON) setup.py bdist --formats=wininst
else
	$(PYTHON) setup.py bdist
endif

python-clean ::
	rm -rf build dist

clean.always :: python-clean

install.always :: $(AUXDIR1)/netrpc/__init__.py
install.always :: $(AUXDIR1)/netsvc/__init__.py

AUXFILTER1 = sed \
 -e 's%@OSE_ROOT@%$(OSE_ROOT)%' \
 -e 's%@OSE_RELEASE_DIRECTORY@%$(OSE_RELEASE_DIRECTORY)%' \
 -e 's%@OSE_RELEASE_FULL_NAME@%$(OSE_RELEASE_FULL_NAME)%'

$(AUXDIR1)/netrpc/__init__.py : netrpc/__init__.py
	$(RM) $@
	cat $< | $(AUXFILTER1) > $@

$(AUXDIR1)/netsvc/__init__.py : netsvc/__init__.py
	$(RM) $@
	cat $< | $(AUXFILTER1) > $@
