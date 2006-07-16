## Copyright 1992-1993 OTC LIMITED
## Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Wipe out some default rules in GNU make so they do not give us problems.
## These are also in module files, but if module files are not included,
## they can give us problems if people use RCS/SCCS.

%.h :
%.hh :
%.hpp :
%.hxx :
%.H :
%.l :
%.r :
%.f :
%.x :

%$(OBJEXT) : %.c
% : %.c
%$(OBJEXT) : %.cc
% : %.cc
%$(OBJEXT) : %.cpp
% : %.cpp
%$(OBJEXT) : %.cxx
% : %.cxx
%$(OBJEXT) : %.C
% : %.C
%$(OBJEXT) : %.f
% : %.f
%$(OBJEXT) : %.l
% : %.l
%$(OBJEXT) : %.r
% : %.r

## Setup preprocessor, compiler and linker flags.

# Add in old variables for backwards compatability.

override PROJECT_CPPFLAGS += $(EXTRA_CPPFLAGS)
override PROJECT_C++FLAGS += $(EXTRA_C++FLAGS)
override PROJECT_CFLAGS += $(EXTRA_CFLAGS)
override PROJECT_LDFLAGS += $(EXTRA_LDFLAGS)
override PROJECT_LDLIBS += $(EXTRA_LDLIBS)

# Values specific to a project.

override CPPFLAGS += $(PROJECT_CPPFLAGS)
override C++FLAGS += $(PROJECT_C++FLAGS)
override CFLAGS += $(PROJECT_CFLAGS)
override LDFLAGS += $(PROJECT_LDFLAGS)
override LDLIBS += $(PROJECT_LDLIBS)

override PIC_CPPFLAGS += $(PROJECT_PIC_CPPFLAGS)
override PIC_C++FLAGS += $(PROJECT_PIC_C++FLAGS)
override PIC_CFLAGS += $(PROJECT_PIC_CFLAGS)
override PIC_LDFLAGS += $(PROJECT_PIC_LDFLAGS)
override PIC_LDLIBS += $(PROJECT_PIC_LDLIBS)

# Add LOADLIBES to the start of LDLIBS for backward compatability.

#override LDLIBS := $(LOADLIBES) $(LDLIBS)
#override LOADLIBES :=

# Shared library options from values defined in host file.

_makeit_separate_pic_objects := YES

ifeq "$($(C++COMPILER)_PIC_CFLAGS)" ""
ifeq "$($(C++COMPILER)_PIC_C++FLAGS)" ""
_makeit_separate_pic_objects := NO
endif
endif

ifeq "$(origin $(C++COMPILER)_PIC_CPPFLAGS)" "file"
override INTERNAL_PIC_CPPFLAGS = $(strip $($(C++COMPILER)_PIC_CPPFLAGS))
endif

ifeq "$(origin $(C++COMPILER)_PIC_C++FLAGS)" "file"
override INTERNAL_PIC_C++FLAGS = $(strip $($(C++COMPILER)_PIC_C++FLAGS))
#_makeit_separate_pic_objects := YES
endif

ifeq "$(origin $(C++COMPILER)_PIC_CFLAGS)" "file"
override INTERNAL_PIC_CFLAGS = $(strip $($(C++COMPILER)_PIC_CFLAGS))
#_makeit_separate_pic_objects := YES
endif

ifeq "$(origin $(C++COMPILER)_PIC_LDFLAGS)" "file"
override INTERNAL_PIC_LDFLAGS = $(strip $($(C++COMPILER)_PIC_LDFLAGS))
endif

ifeq "$(origin $(C++COMPILER)_PIC_LDLIBS)" "file"
override INTERNAL_PIC_LDLIBS = $(strip $($(C++COMPILER)_PIC_LDLIBS))
endif

# Library search path option.

ifeq "$(origin $(C++COMPILER)_LOPT)" "file"
override LOPT := $($(C++COMPILER)_LOPT)
endif

# List of names of SYMDEF files created by ranlib in libraries.

override SYMDEFS := __.SYMDEF* _*EB_ _*EL_ so_locations

ifeq "$(origin DEFAULT_SYM_CLEANUP)" "undefined"
override DEFAULT_SYM_CLEANUP = $(RM) $(SYMDEFS)
endif

ifeq "$(origin $(C++COMPILER)_SYM_CLEANUP)" "undefined"
override $(C++COMPILER)_SYM_CLEANUP = $(DEFAULT_SYM_CLEANUP)
endif

override RMSYMS = $($(C++COMPILER)_SYM_CLEANUP)

## Set up Sentinel products.

ifeq "$(SENTINEL)" ""
ifneq "$(SE_HOME)" ""
export SE_HOME
SENTINEL := $(SE_HOME)/sentinel $(SENTINELOPTIONS)
else
SENTINEL := sentinel $(SENTINELOPTIONS)
endif
endif

# Check for enabling of Sentinel.

ifeq "$(_makeit_enable_memcheck)" ""
ifneq "$(filter sentinel,$(MAKEIT_OPTIONS))" ""
_makeit_cleanup_sentinel := YES
_makeit_enable_memcheck := YES
override MEMCHECK := $(SENTINEL)
override MEMEXT := sent
endif
endif

ifeq "$(_makeit_enable_memcheck_all)" ""
ifneq "$(filter sentinel_all,$(MAKEIT_OPTIONS))" ""
_makeit_cleanup_sentinel := YES
_makeit_enable_memcheck_all := YES
override MEMCHECKALL := $(SENTINEL)
endif
endif

## Set up TestCenter products.

ifeq "$(PROOF)" ""
ifneq "$(CENTERLINE_HOME)" ""
export CENTERLINE_HOME
PROOF := $(CENTERLINE_HOME)/bin/proof $(PROOFOPTIONS)
else
PROOF := proof $(PROOFOPTIONS)
endif
endif

# Check for enabling of TestCenter.

ifeq "$(_makeit_enable_memcheck)" ""
ifneq "$(filter testcenter,$(MAKEIT_OPTIONS))" ""
_makeit_cleanup_testcenter := YES
_makeit_enable_memcheck := YES
override MEMCHECK := $(PROOF)
override MEMEXT := tc
endif
endif

ifeq "$(_makeit_enable_memcheck_all)" ""
ifneq "$(filter testcenter_all,$(MAKEIT_OPTIONS))" ""
_makeit_cleanup_testcenter := YES
_makeit_enable_memcheck_all := YES
override MEMCHECKALL := $(PROOF)
endif
endif

## Set up Pure Software products.

ifeq "$(PURIFY)" ""
ifneq "$(PURIFYHOME)" ""
export PURIFYHOME
PURIFY := $(PURIFYHOME)/purify
else
PURIFY := purify
endif
endif

ifeq "$(PURECOV)" ""
ifneq "$(PURECOVHOME)" ""
export PURECOVHOME
PURECOV := $(PURECOVHOME)/purecov
else
PURECOV := purecov
endif
endif

ifeq "$(PURELINK)" ""
ifneq "$(PURELINKHOME)" ""
export PURELINKHOME
PURELINK := $(PURELINKHOME)/purelink
else
PURELINK := purelink
endif
endif

ifeq "$(QUANTIFY)" ""
ifneq "$(QUANTIFYHOME)" ""
export QUANTIFYHOME
QUANTIFY := $(QUANTIFYHOME)/quantify
else
QUANTIFY := quantify
endif
endif

ifeq "$(PURIFY_MEMEXT)" ""
override PURIFY_MEMEXT := pure
endif

ifeq "$(PURECOV_MEMEXT)" ""
override PURECOV_MEMEXT := pure
endif

ifeq "$(QUANTIFY_MEMEXT)" ""
override QUANTIFY_MEMEXT := pure
endif

# Add compiler specific options.

ifeq "$(origin $(C++COMPILER)_PURIFYOPTIONS)" "file"
override PURIFYOPTIONS += $($(C++COMPILER)_PURIFYOPTIONS)
endif

ifeq "$(origin $(C++COMPILER)_PURECOVOPTIONS)" "file"
override PURECOVOPTIONS += $($(C++COMPILER)_PURECOVOPTIONS)
endif

ifeq "$(origin $(C++COMPILER)_PURELINKOPTIONS)" "file"
override PURELINKOPTIONS += $($(C++COMPILER)_PURELINKOPTIONS)
endif

ifeq "$(origin $(C++COMPILER)_QUANTIFYOPTIONS)" "file"
override QUANTIFYOPTIONS += $($(C++COMPILER)_QUANTIFYOPTIONS)
endif

# Check for environment variables which customise the behaviour of
# Purify/Quantify. If any are set then make sure they are exported as they
# may only be defined in the make files and not in the users environment.

ifdef PURIFYTMPDIR
export PURIFYTMPDIR
endif

ifdef PURIFYCACHEDIR
export PURIFYCACHEDIR
endif

ifdef PURIFYALWAYSUSECACHEDIR
export PURIFYALWAYSUSECACHEDIR
endif

ifdef PURIFYOPTIONS
export PURIFYOPTIONS
endif

ifdef PURECOVOPTIONS
export PURECOVOPTIONS
endif

ifdef PURELINKOPTIONS
export PURELINKOPTIONS
endif

ifdef QUANTIFYOPTIONS
export QUANTIFYOPTIONS
endif

# Check for enabling of Purify/PureLink/Quantify. In the case of conflicts,
# Purify takes precedence.

ifeq "$(_makeit_enable_memcheck)" ""
ifneq "$(filter purify,$(MAKEIT_OPTIONS))" ""
_makeit_cleanup_pure := YES
_makeit_enable_memcheck := YES
override MEMCHECK :=
override MEMEXT := $(PURIFY_MEMEXT)
ifneq "$(filter purelink,$(MAKEIT_OPTIONS))" ""
override MEMCHECK += $(PURELINK)
endif
override MEMCHECK += $(PURIFY)
ifneq "$(filter purecov,$(MAKEIT_OPTIONS))" ""
override MEMCHECK += $(PURECOV)
endif
endif
endif

ifeq "$(_makeit_enable_memcheck_all)" ""
ifneq "$(filter purify_all,$(MAKEIT_OPTIONS))" ""
_makeit_cleanup_pure := YES
_makeit_enable_memcheck_all := YES
override MEMCHECKALL :=
ifneq "$(filter purelink_all,$(MAKEIT_OPTIONS))" ""
override MEMCHECKALL += $(PURELINK)
endif
override MEMCHECKALL += $(PURIFY)
ifneq "$(filter purecov_all,$(MAKEIT_OPTIONS))" ""
override MEMCHECKALL += $(PURECOV)
endif
endif
endif

ifeq "$(_makeit_enable_memcheck)" ""
ifneq "$(filter quantify,$(MAKEIT_OPTIONS))" ""
_makeit_cleanup_pure := YES
_makeit_enable_memcheck := YES
override MEMCHECK := $(QUANTIFY)
override MEMEXT := $(QUANTIFY_MEMEXT)
endif
endif

ifeq "$(_makeit_enable_memcheck_all)" ""
ifneq "$(filter quantify_all,$(MAKEIT_OPTIONS))" ""
_makeit_cleanup_pure := YES
_makeit_enable_memcheck_all := YES
override MEMCHECKALL := $(QUANTIFY)
endif
endif

ifeq "$(_makeit_enable_memcheck_all)" ""
ifneq "$(filter purelink_all,$(MAKEIT_OPTIONS))" ""
_makeit_cleanup_pure := YES
_makeit_enable_memcheck_all := YES
override MEMCHECKALL := $(PURELINK)
endif
endif

ifeq "$(_makeit_enable_memcheck)" ""
ifneq "$(filter purecov,$(MAKEIT_OPTIONS))" ""
_makeit_cleanup_pure := YES
_makeit_enable_memcheck := YES
override MEMCHECK := $(PURECOV)
override PURECOV_MEMEXT := pure
override MEMEXT := $(PURECOV_MEMEXT)
endif
endif

ifeq "$(_makeit_enable_memcheck_all)" ""
ifneq "$(filter purecov_all,$(MAKEIT_OPTIONS))" ""
_makeit_cleanup_pure := YES
_makeit_enable_memcheck_all := YES
override MEMCHECKALL := $(PURECOV)
endif
endif

## Set up location of scripts.

ifeq "$(origin SCRIPTDIR)" "undefined"
SCRIPTDIR = $(OSE_HOME)/etc
endif

## Some stuff for ObjectStore.

ifeq "$(ENV_OODBMS)" "OSTORE"
ifeq "$(origin OS_APPLICATION_SCHEMA_DB_PATH)" "undefined"
OS_APPLICATION_SCHEMA_DB_PATH := $(MK)/osschema.adb
endif
ifeq "$(origin OS_COMPILATION_SCHEMA_DB_PATH)" "undefined"
OS_COMPILATION_SCHEMA_DB_PATH := $(MK)/osschema.cdb
endif
ifneq "$(SCHEMAS)" ""
ifeq "$(filter MS%,$(C++COMPILER))" ""
_cc_generated_NONLIB_SRC += osschema.cc
REQUIRE += cc
endif
endif
endif

## Some stuff for Versant.

ifeq "$(ENV_OODBMS)" "VERSANT"

IMPFILES := $(filter-out $(EXCLUDE),$(filter %.imp,$(SRCFILES)))

_imp_cxx_generated_SRC := $(patsubst %.imp,%.cxx,$(IMPFILES))
_imp_cxx_generated_MK_SRC := $(patsubst %.imp,$(MK)/%.cxx,$(IMPFILES))

override EXCLUDE += $(_imp_cxx_generated_SRC)

ifneq "$(filter archive_in_library,$(VERSANT_OPTIONS))" ""
_cxx_generated_SRC += $(_imp_cxx_generated_SRC)
else
_cxx_generated_NONLIB_SRC += $(_imp_cxx_generated_SRC)
endif

ifneq "$(IMPFILES)" ""
REQUIRE += cxx
endif

depend.setup :: $(_imp_cxx_generated_MK_SRC)

DEPFILTER.imp = sed -f $(OSE_MAKEIT)/depend.sed | sort -u | \
 sed -e "/$(*F).i2/d" -e "/$(*F).imp/d" -e "/$(*F).cxx/d"

_imp_D := $(patsubst %,$(MK)/%.d,$(IMPFILES))

$(_imp_D) : $(MK)/%.imp.d : $(MK)/%.cxx
	@echo makeit: generating dependencies for $(patsubst %.d,%,$(@F))
	@$(PREPROCESS.cxx) $(TEMPLATEFLAGS.cxx) $< | $(DEPFILTER.imp) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F).cxx%' \
	    -e 's%^TARGET%$$(MK)/$(*F).imp.d%' > $@

_depend_SRC += $(_imp_D)

endif

## Make subdirectories into which compiled components will be placed.

ifneq "$(NOMK)" "YES"

makeit.setup :: $(MK)

$(MK) :
	@mkdir $@

ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifeq "$(_makeit_separate_pic_objects)" "YES"

makeit.setup :: $(MK).pic

$(MK).pic :
	@mkdir $@

endif
endif

ifneq "$(filter always_create_subdirectory,$(MAKEIT_OPTIONS))" ""
override _makeit_dummy := $(shell test -d $(MK) || mkdir $(MK))
endif

ifeq "$(_makeit_cxx_supports_xl_repositories)" "YES"

_makeit_cxx_requires_repository := YES

makeit.setup :: $(MKPTR)

$(MKPTR) :
	@mkdir $@

endif

ifeq "$(_makeit_cxx_supports_cfront_repositories)" "YES"

_makeit_cxx_requires_repository := YES

makeit.setup :: $(MKPTR)

$(MKPTR) :
	@mkdir $@

endif

ifeq "$(_makeit_cxx_supports_dec_repositories)" "YES"

_makeit_cxx_requires_repository := YES

makeit.setup :: $(MKPTR)

$(MKPTR) :
	@mkdir $@

ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifeq "$(_makeit_separate_pic_objects)" "YES"

makeit.setup :: $(MKPTR).pic

$(MKPTR).pic :
	@mkdir $@

endif
endif

endif

ifeq "$(_makeit_cxx_supports_cafe_repositories)" "YES"

_makeit_cxx_requires_repository := YES

makeit.setup :: $(MKPTR)

$(MKPTR) :
	@mkdir $@
	@mkdir $@/Templates.DB
	@mkdir $@/Templates.DB/Module.DB

ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifeq "$(_makeit_separate_pic_objects)" "YES"

makeit.setup :: $(MKPTR).pic

$(MKPTR).pic :
	@mkdir $@
	@mkdir $@/Templates.DB
	@mkdir $@/Templates.DB/Module.DB

endif
endif

endif

endif
