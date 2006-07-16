## Copyright 1992 OTC LIMITED
## Copyright 1995-1999 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

ifeq "$(SRCDIR)" ""
SRCDIR := .
endif

BLDROOT := ../..
SRCROOT := $(SRCDIR)/$(BLDROOT)

MODULES := combine check install
MAKEIT_OPTIONS := purify shlib ignore_repositories
COMBINE_OPTIONS := closure
CLOSURE_PREFIX := t_otc_
CLOSURE_OPTIONS := ignore_undefined_symbols

include $(SRCROOT)/config/init.mk

## Local definitions.

SUBDIRS := \
 misc \
 debug \
 memory \
 refcnt \
 collctn \
 text \
 types \
 system \
 dispatch \
 message \
 internet

ALLDIRS := ostore thread $(SUBDIRS)

ifeq "$(ENV_OODBMS)" "OSTORE"
SUBDIRS := ostore $(SUBDIRS)
else
ifneq "$(filter OS%,$(C++COMPILER))" ""
SUBDIRS := ostore $(SUBDIRS)
endif
endif

ifneq "$(filter -DHAVE_%_THREADS,$($(C++COMPILER)_CPPFLAGS))" ""
SUBDIRS := thread $(SUBDIRS)
endif

ifeq "$(_makeit_cxx_supports_precompiled_templates)" "YES"
SUBDIRS := $(SUBDIRS) closure
endif

LIBDIR := $(OSE_TARGET_HOME)/lib/$(C++COMPILERTAG)
DLLDIR := $(OSE_HOME)/bin

LIBRARY_RELTAG := $(subst .,X,$(OSE_RELEASE_NAME))
LIBRARY_PLTAG := $(patsubst pl00,,pl$(OSE_MAINTENANCE_RELEASE))
LIBRARY_CXXTAG := $(subst -,_,$(subst .,X,$(C++COMPILERTAG)))

LIBRARY_TAG := $(LIBRARY_RELTAG)$(LIBRARY_PLTAG)_$(LIBRARY_CXXTAG)

LIBRARY := OTC_$(LIBRARY_TAG)_$(VARIANT)
LIBVERSION := 1.0
PTMAPS := ../../include/OTC/PTMAP
ifeq "$(ENV_OODBMS)" "OSTORE"
PTDUMMYS := ../../include/OTC/PTDUMMYS
endif

CPPFLAGS += -I../../include

LDLIBS += ../OSE/$(WSPREFIX)$(MKTAG)_opt/lib$(LIBEXT)

## Makeit modules.

include $(SRCROOT)/config/modules.mk

## Local rules.

all.always :: lib.always shlib.always

ifeq "$(ENV_OSEINSTALLALL)" "YES"
install.always :: clean.subdirs
	$(RM) $(MK).ptr/*
	$(RM) $(MK).ptr.pic/*
	- rmdir $(MK).ptr
	- rmdir $(MK).ptr.pic/*
endif
