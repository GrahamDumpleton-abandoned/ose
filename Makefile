## Copyright 1992 OTC LIMITED
## Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
##
## Note: This Makefile requires GNU make or makeit. The OSE_HOST
## environment must be set appropriately in your use environment
## for the makefile to work. See the INSTALL file for installation
## instructions.

## Initialisation.

ifeq "$(SRCDIR)" ""
SRCDIR := .
endif

BLDROOT := .
SRCROOT := $(SRCDIR)/$(BLDROOT)

MODULES := install
NOMK := YES

include $(SRCROOT)/config/init.mk

## Local definitions.

SUBDIRS := config include lib bin

## Makeit modules.

include $(SRCROOT)/config/modules.mk

## Local rules.

install.all :: install.setup

install.all ::
	$(MAKE) C++COMPILER=$(firstword $(C++COMPILERS)) clean
ifeq "$(DONTSTOPBUILD)" "YES"
	@ FAILED=""; for i in $(C++COMPILERS); \
	do \
	  echo $(MAKE) C++COMPILER=$$i ENV_OSEINSTALLALL=YES install.subdirs; \
	  $(MAKE) C++COMPILER=$$i ENV_OSEINSTALLALL=YES install.subdirs \
	   || FAILED="$$FAILED $$i"; \
	  echo $(MAKE) C++COMPILER=$$i clean; \
	  $(MAKE) C++COMPILER=$$i clean \
	   || exit 1; \
	done; \
	echo; \
	echo "**********************************************************"; \
	echo; \
	echo "The following compilers failed: $(FAILED)" ; \
	echo; \
	echo "**********************************************************"; \
	echo
else
	@ for i in $(C++COMPILERS); \
	do \
	  echo $(MAKE) C++COMPILER=$$i ENV_OSEINSTALLALL=YES install.subdirs; \
	  $(MAKE) C++COMPILER=$$i ENV_OSEINSTALLALL=YES install.subdirs \
	   || exit 1; \
	  echo $(MAKE) C++COMPILER=$$i clean; \
	  $(MAKE) C++COMPILER=$$i clean \
	   || exit 1; \
	done
endif

install.all :: install.always

$(OSE_BUILD_ROOT) :
	@echo
	@echo "$(OSE_BUILD_ROOT) must already exist, create it manually"
	@echo
	exit 1

install.setup :: $(OSE_BUILD_ROOT)
	- mkdir $(OSE_HOME)
	- mkdir $(OSE_HOME)/bin
	- mkdir $(OSE_HOME)/doc
	- mkdir $(OSE_HOME)/include
	- mkdir $(OSE_HOME)/lib
	- mkdir $(OSE_HOME)/man
	- mkdir $(OSE_HOME)/man/man1
	- mkdir $(OSE_HOME)/man/man3
ifneq "$(OSE_HOST)" "$(OSE_TARGET)"
	- mkdir $(OSE_HOST_HOME)
	- mkdir $(OSE_HOST_HOME)/bin
	- mkdir $(OSE_HOST_HOME)/etc
	- mkdir $(OSE_HOST_HOME)/makeit
	- mkdir $(OSE_HOST_HOME)/include
	- mkdir $(OSE_TARGET_HOME)
	- mkdir $(OSE_TARGET_HOME)/include
	- mkdir $(OSE_TARGET_HOME)/lib
else
	- mkdir $(OSE_TARGET_HOME)
	- mkdir $(OSE_TARGET_HOME)/makeit
	- mkdir $(OSE_TARGET_HOME)/bin
	- mkdir $(OSE_TARGET_HOME)/etc
	- mkdir $(OSE_TARGET_HOME)/include
	- mkdir $(OSE_TARGET_HOME)/lib
endif
	- cp $(SRCDIR)/COPYING $(OSE_HOME)
	- cp $(SRCDIR)/LICENSE $(OSE_HOME)
	echo $(OSE_RELEASE_NAME)pl$(OSE_MAINTENANCE_RELEASE) > \
	 $(OSE_TARGET_HOME)/VERSION

install.always ::
ifeq "$(ENV_TOOLSET)" "MKS"
	@echo
	@echo "**********************************************************"
	@echo
	@echo "Now copy whatever version of GNU make you are"
	@echo "using to install OSE to the directory:"
	@echo
	@echo "  $(OSE_HOST_HOME)/etc"
	@echo
	@echo "Call the file 'makeit$(BINEXT)' when copied in to"
	@echo "that directory."
	@echo
	@echo "**********************************************************"
	@echo
endif

install.makeit :: install.setup
	$(MAKE) -C config install
	$(MAKE) -C include/makeit install
	$(MAKE) -C include/makeit clean

install.makeit :: install.always

all.setup ::
	@echo
	@echo "**********************************************************"
	@echo
	@echo "Read the instructions in the INSTALL file. You should"
	@echo "be using the targets 'install.all' or 'install', not"
	@echo "the target which you have used."
	@echo
	@echo "**********************************************************"
	@echo
	exit 1

# Following are experimental targets which will most like become the
# default way of building OSE in the future. This methods is quicker
# than the current method but uses a lot more disk space.

build-release ::
	$(MAKE) -C lib/OSE all
	@$(foreach var,$(VARIANTS), \
	 $(MAKE) -C build VARIANT=$(var) all || $(FAILACTION);)
	$(MAKE) -C bin all QUICK=YES

install-release :: install.setup
	$(MAKE) -C config install
	$(MAKE) -C include install
	$(MAKE) -C lib/OSE install
	@$(foreach var,$(VARIANTS), \
	 $(MAKE) -C build VARIANT=$(var) install || $(FAILACTION);)
	$(MAKE) -C bin install QUICK=YES

clean.always ::
	@$(foreach var,$(VARIANTS), \
	 $(MAKE) -C build VARIANT=$(var) clean || $(FAILACTION);)
	$(MAKE) -C python clean
