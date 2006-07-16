## Copyright 1993 OTC LIMITED
## Copyright 1995-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

all :

.PHONY : makeit.setup

makeit.setup ::

depend.setup :: makeit.setup
all.setup :: makeit.setup
lib.setup :: makeit.setup
shlib.setup :: makeit.setup
programs.setup :: makeit.setup
mostlyclean.setup :: makeit.setup
clean.setup :: makeit.setup
distclean.setup :: makeit.setup
realclean.setup :: makeit.setup

# Following is to handle version 3.X makeit being run but with OSE_VERSION
# defined to 4.X or later. Don't support doing this for cross compilation
# environment.

ifeq "$(OSE_HOST_MAKEIT)" ""
ifneq "$(OSE_HOST)" ""
OSE_HOST_MAKEIT = $(OSE_ROOT)/$(OSE_VERSION)/$(OSE_HOST)/include/makeit
else
override OSE_MAKEIT :=
endif
endif

## Following is to handle version of makeit prior to 4.3 being run with
## OSE_VERSION set to 4.3 or later.

ifeq "$(OSE_TARGET)" ""
override OSE_TARGET = $(OSE_HOST)
endif

ifeq "$(OSE_TARGET_MAKEIT)" ""
OSE_TARGET_MAKEIT = $(OSE_ROOT)/$(OSE_VERSION)/$(OSE_TARGET)/include/makeit
endif

# Make sure LD_RUN_PATH is exported for platforms where we have
# to play with encoding library path in executables. Also, make sure
# it has some value at least. Note that we have to do a bit of a
# fiddle here to stop it continually being extended in sub makes.

export LD_RUN_PATH

#ifeq "$(MAKELEVEL)" "0"
ifeq "$(origin _makeit_ENVIRONMENT_LD_RUN_PATH)" "undefined"
ifeq "$(origin LD_RUN_PATH)" "environment"
override _makeit_ENVIRONMENT_LD_RUN_PATH := $(LD_RUN_PATH)
else
ifeq "$(LD_RUN_PATH)" ""
override _makeit_ENVIRONMENT_LD_RUN_PATH := /usr/local/lib
LD_RUN_PATH := $(_makeit_ENVIRONMENT_LD_RUN_PATH)
endif
endif
export _makeit_ENVIRONMENT_LD_RUN_PATH
else
ifeq "$(origin LD_RUN_PATH)" "environment"
LD_RUN_PATH := $(_makeit_ENVIRONMENT_LD_RUN_PATH)
endif
endif

# Look locally defined modules.

ifneq "$(OSE_ROOT)" ""
ifneq "$(wildcard $(OSE_ROOT)/modules/init.mk)" ""

include $(OSE_ROOT)/modules/init.mk

override MODULEPATH += $(OSE_ROOT)/modules

endif
endif

# Add default search directories for modules.

override MODULEPATH += $(OSE_HOST_MAKEIT)
override MODULEPATH += $(OSE_MAKEIT)

# Use default set of modules if none defined.

override INTERNAL-COREMODULES += java
override INTERNAL-COREMODULES += ose
override INTERNAL-COREMODULES += qt
override INTERNAL-COREMODULES += python
override INTERNAL-COREMODULES += tcl
override INTERNAL-COREMODULES += xll
override INTERNAL-COREMODULES += yacc lex rpcgen
override INTERNAL-COREMODULES += cc cpp cxx C c-cc
override INTERNAL-COREMODULES += c
override INTERNAL-COREMODULES += sh
override INTERNAL-COREMODULES += check
override INTERNAL-COREMODULES += combine
override INTERNAL-COREMODULES += install

# Build list of all modules. Filter out duplicates which might
# be defined in LOCALMODULES. Similarly for ALLMODULES if it has
# been overridden by user. If their overrides stuff up ordering
# or other things, that is there fault and results are undefined.
# Note that if they override ALLMODULES, they must have used "="
# and not ":=" or this all will not work.

override INTERNAL-COREMODULES := \
 $(filter-out $(LOCALMODULES),$(INTERNAL-COREMODULES))

ifneq "$(ALLMODULES)" ""

override INTERNAL-COREMODULES := \
 $(filter-out $(ALLMODULES),$(INTERNAL-COREMODULES))

else

override ALLMODULES += $(LOCALMODULES)
override ALLMODULES += $(COREMODULES)

endif

override COREMODULES := $(INTERNAL-COREMODULES)

# Work out which modules we can find to include.

_makeit_modules_options := \
 $(foreach module,$(filter $(MODULES),$(ALLMODULES)),\
  $(firstword $(wildcard $(addsuffix /$(module)-options.mk,$(MODULEPATH)))))

_makeit_modules_defns := \
 $(foreach module,$(filter $(MODULES),$(ALLMODULES)),\
  $(firstword $(wildcard $(addsuffix /$(module)-defns.mk,$(MODULEPATH)))))

_makeit_modules_init := \
 $(foreach module,$(filter $(MODULES),$(ALLMODULES)),\
  $(firstword $(wildcard $(addsuffix /$(module)-init.mk,$(MODULEPATH)))))

_makeit_modules_rules := \
 $(foreach module,$(filter $(MODULES),$(ALLMODULES)),\
  $(firstword $(wildcard $(addsuffix /$(module)-rules.mk,$(MODULEPATH)))))

# Include "options" part of any module files.

ifneq "$(_makeit_modules_options)" ""
include $(_makeit_modules_options)
endif

# Define macros sufficient to be able to calculate C++COMPILERENV such
# it can be used in host config file for this target.

_makeit_dummy_A = $(firstword $(C++COMPILER) $(DEFAULTC++COMPILER))
_makeit_dummy_B = $(subst -, ,$(_makeit_dummy_A))
_makeit_dummy_C = $(_makeit_dummy_B) $(_makeit_dummy_B)
_makeit_dummy_D = $(word 2,$(_makeit_dummy_C))
_makeit_dummy_E = $(word 3,$(_makeit_dummy_C))

override C++COMPILERENV = $(_makeit_dummy_E)

# Include host config file for this target.

include $(OSE_HOST_MAKEIT)/$(OSE_TARGET).mk

override OSE_CONFIGURATION := $(strip $(OSE_CONFIGURATION))

include $(OSE_HOST_MAKEIT)/$(OSE_TARGET)-$(OSE_CONFIGURATION).mk

# Include version information.

include $(OSE_MAKEIT)/version.mk

# Include internal definitions.

include $(OSE_MAKEIT)/compiler.mk

include $(OSE_MAKEIT)/general.mk

# Include "defns" part of any module files.
# "init" files are included for backwward
# compatbility only.

ifneq "$(_makeit_modules_defns)" ""
include $(_makeit_modules_defns)
endif

ifneq "$(_makeit_modules_init)" ""
include $(_makeit_modules_init)
endif
