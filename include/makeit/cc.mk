## Copyright 1992-1993 OTC LIMITED
## Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

%.h :
%.h++ :
%.hxx :
%.hpp :
%.hh :
%.H :

vpath %.c $(SRCDIR) $(SRCDIRS)
vpath %.c++ $(SRCDIR) $(SRCDIRS)
vpath %.cxx $(SRCDIR) $(SRCDIRS)
vpath %.cpp $(SRCDIR) $(SRCDIRS)
vpath %.cc $(SRCDIR) $(SRCDIRS)
vpath %.C $(SRCDIR) $(SRCDIRS)

vpath %.h $(SRCDIR) $(SRCDIRS)
vpath %.h++ $(SRCDIR) $(SRCDIRS)
vpath %.hxx $(SRCDIR) $(SRCDIRS)
vpath %.hpp $(SRCDIR) $(SRCDIRS)
vpath %.hh $(SRCDIR) $(SRCDIRS)
vpath %.H $(SRCDIR) $(SRCDIRS)
vpath %.i $(SRCDIR) $(SRCDIRS)

## Define compilation commands. We cannot use ":=" for assignment here
## so must use only "=" and "+=". This is necessary to ensure that
## variables are only expanded when action is actually run. This
## restriction means that commands must be constructed from the front
## and all new bits added to the end only.

PREPROCESS.cc =
COMPILE.cc =
COMPILE.cc.o =
COMPILE.cc.so =
PCOMPILE.cc =
LINK.cc =
CLINK.cc =
POSTLINK.cc =
LINK.cc.memcheck =
CLINK.cc.memcheck =
POSTLINK.cc.memcheck =

LINKCOMMAND.cc =
LINKCOMMAND.cc.memcheck =

ifeq "$(_makeit_cxx_supports_o_for_objects)" "YES"
TARGET.cc = -o $@
else
TARGET.cc = && mv $(<F:.cc=$(OBJEXT)) $@
endif

# ObjectStore link processing.

ifneq "$(filter OS%,$(C++COMPILER))" ""
ifeq "$(filter +OSTD,$(INTERNAL_C++FLAGS))" ""

ifneq "$(OS_COMPILATION_SCHEMA_DB_PATH)" ""
ifneq "$(OS_APPLICATION_SCHEMA_DB_PATH)" ""
LINK.cc += \
 $(OS_ROOTDIR)/lib/os_do_link "$(OS_COMPILATION_SCHEMA_DB_PATH)" \
 "$(OS_APPLICATION_SCHEMA_DB_PATH)" "$(@F)"
endif
endif

endif
endif

# Purify/Sentinel all applications by default. Purify takes precedence.
# Cannot use either if using wrapper around compiler. Also, if
# LINKCOMMAND.cc already defined then it will take precedence.

ifneq "$(_makeit_cxx_requires_wrapper)" "YES"
ifeq "$(_makeit_enable_memcheck_all)" "YES"
LINKCOMMAND.cc += $(MEMCHECKALL)
endif
endif

# Compilation commands.

LINKCOMMAND.cc += $(C++)
LINKCOMMAND.cc.memcheck += $(MEMCHECK) $(C++)

PREPROCESS.cc += $(C++) -E $(C++FLAGS.cc) $(CPPFLAGS.cc)
COMPILE.cc += $(C++) $(C++FLAGS.cc) $(CPPFLAGS.cc) -c

LINK.cc += $(LINKCOMMAND.cc) $(C++FLAGS.cc) $(CPPFLAGS.cc)
LINK.cc.memcheck += $(LINKCOMMAND.cc.memcheck) $(C++FLAGS.cc) $(CPPFLAGS.cc)

# ObjectStore compilation schema database update.

ifneq "$(filter OS%,$(C++COMPILER))" ""
ifeq "$(filter +OSTD,$(INTERNAL_C++FLAGS))" ""

ifneq "$(OS_COMPILATION_SCHEMA_DB_PATH)" ""
COMPILE.cc += -update_schema $(OS_COMPILATION_SCHEMA_DB_PATH)
endif

endif
endif

# PIC flags. We doing something weird here if using the Sun CAFE
# compiler. That is, force use of PIC version of repositories. The
# actual -ptr flags do not get added to COMPILE.cc until further
# down in this file.

COMPILE.cc.o += $(COMPILE.cc)

ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifneq "$(_makeit_separate_pic_objects)" "YES"
COMPILE.cc.o += -DPIC $(PIC_CPPFLAGS)
endif
endif

ifeq "$(_makeit_cxx_supports_cafe_repositories)" "YES"
COMPILE.cc.so += $(patsubst -ptr%,-ptr%.pic,$(COMPILE.cc)) $(PIC_C++FLAGS)
else
ifeq "$(_makeit_cxx_supports_dec_repositories)" "YES"
COMPILE.cc.so += $(patsubst $(MKPTR),$(MKPTR).pic,$(COMPILE.cc)) $(PIC_C++FLAGS)
else
COMPILE.cc.so += $(COMPILE.cc) $(PIC_C++FLAGS)
endif
endif

COMPILE.cc.so += -DPIC $(PIC_CPPFLAGS)

# Combined compilation and link.

CLINK.cc += $(LINK.cc)
CLINK.cc.memcheck += $(LINK.cc.memcheck)

# Compilation of program object file.

PCOMPILE.cc += $(COMPILE.cc)

# XL C++ style repositories.

ifeq "$(_makeit_cxx_supports_xl_repositories)" "YES"

PTRFLAGS.cc = -qtempinc=$(MKPTR)

COMPILE.cc += $(PTRFLAGS.cc)
LINK.cc += $(PTRFLAGS.cc)
LINK.cc.memcheck += $(PTRFLAGS.cc)

endif

## Cfront style repositories.

ifeq "$(_makeit_cxx_supports_cfront_repositories)" "YES"

PTRFLAGS.cc = -ptr$(MKPTR)

ifeq "$(filter ignore_repositories,$(MAKEIT_OPTIONS))" ""
ifneq "$(PTRDIRS)" ""
PTRFLAGS.cc += $(addprefix -ptr,$(PTRDIRS))
endif
endif

ifeq "$(filter minimal_expansion,$(MAKEIT_OPTIONS))" ""
PTRFLAGS.cc += -pta
endif

ifneq "$(filter separate_functions,$(MAKEIT_OPTIONS))" ""
PTRFLAGS.cc += -pts
endif

COMPILE.cc += $(PTRFLAGS.cc)
LINK.cc += $(PTRFLAGS.cc)
LINK.cc.memcheck += $(PTRFLAGS.cc)

endif

## DEC style repositories.

ifeq "$(_makeit_cxx_supports_dec_repositories)" "YES"

PTRFLAGS.cc = -ptr $(MKPTR)

ifeq "$(filter ignore_repositories,$(MAKEIT_OPTIONS))" ""
ifneq "$(PTRDIRS)" ""
PTRFLAGS.cc += $(addprefix -ptr ,$(PTRDIRS))
endif
endif

COMPILE.cc += $(PTRFLAGS.cc)
LINK.cc += $(PTRFLAGS.cc)
LINK.cc.memcheck += $(PTRFLAGS.cc)

endif

# Sun C++ 4.0 style repositories.

ifeq "$(_makeit_cxx_supports_cafe_repositories)" "YES"

_makeit_combine_compile_and_link := YES

PTRFLAGS.cc = -ptr$(MKPTR)

ifeq "$(filter ignore_repositories,$(MAKEIT_OPTIONS))" ""
ifneq "$(PTRDIRS)" ""
PTRFLAGS.cc += $(addprefix -ptr,$(PTRDIRS))
endif
endif

# Following -pta option is to work around a bug in the compiler.
# If taken out compiles will be a lot quicker. You however run the
# risk of undefined symbols at link time. Sometimes a second
# attempt to build the program will work. This problem hadn't
# been fixed as of patch level 5 of the compiler.

ifeq "$(filter minimal_expansion,$(MAKEIT_OPTIONS))" ""
PTRFLAGS.cc += -pta
endif

COMPILE.cc += $(PTRFLAGS.cc)
LINK.cc += $(PTRFLAGS.cc)
LINK.cc.memcheck += $(PTRFLAGS.cc)

endif

## Directed instantiation.

TEMPLATEFLAGS.cc = -DEXPAND_TEMPLATES $(PTFLAGS)

_cc_TEMPLATES := $(filter %.cc,$(TEMPLATES))

ifeq "$(_makeit_cxx_supports_direct_instantiation)" "YES"

ifeq "$(_cc_TEMPLATES)" ""

ifeq "$(_makeit_cxx_supports_cfront_repositories)" "YES"
ifeq "$(filter -ptn,$(C++FLAGS))" ""
ifeq "$(filter use_repository_only,$(MAKEIT_OPTIONS))" ""
_makeit_combine_compile_and_link := YES
endif
ifeq "$(filter no_implicit_templates,$(MAKEIT_OPTIONS))" ""
_makeit_combine_compile_and_link := YES
endif
endif
endif

ifeq "$(_makeit_combine_compile_and_link)" "YES"
CLINK.cc += $(TEMPLATEFLAGS.cc)
CLINK.cc.memcheck += $(TEMPLATEFLAGS.cc)
else
ifneq "$(_makeit_cxx_supports_cfront_repositories)" "YES"
PCOMPILE.cc += $(TEMPLATEFLAGS.cc)
endif
endif

endif

endif

## Macros for ObjectStore when not using DML.

ifeq "$(ENV_OODBMS)" "OSTORE"
ifneq "$(SCHEMAS)" ""
ifneq "$(ENV_SYSTEM)" "WIN32"
ifneq "$(wildcard $(OS_ROOTDIR)/bin/os_postlink)" ""
POSTLINK.cc += \
 $(OS_CMDENV) $(OS_ROOTDIR)/bin/os_postlink $(OS_POSTLINK_FLAGS) $@
POSTLINK.cc.memcheck += \
 $(OS_CMDENV) $(OS_ROOTDIR)/bin/os_postlink $(OS_POSTLINK_FLAGS) $@
endif
endif
endif
endif

## Modify various compiler flags etc as required for this module.

#QUALIFIED_CPPFLAGS.cc = \
# $(patsubst $(patsubst $(SRCDIR)/%,%,$<):%,%, \
# $(filter -% $(patsubst $(SRCDIR)/%,%,$<):%,$(QUALIFIED_CPPFLAGS)))

#QUALIFIED_CPPFLAGS.cc = \
# $(patsubst $(notdir $<):%,%, \
# $(filter -% $(notdir $<):%,$(QUALIFIED_CPPFLAGS)))

QUALIFIED_VARIANT.cc = \
 $(firstword $(patsubst $(VARIANT)=%,%, \
 $(filter $(VARIANT)=%, $(patsubst $(notdir $<):%,%, \
 $(filter $(notdir $<):%,$(QUALIFIED_VARIANT))))) $(VARIANT))

QUALIFIED_VARIANT_CPPFLAGS.cc = \
 $(CPPFLAGS_$(QUALIFIED_VARIANT.cc)) \
 $($(C++COMPILER)_CPPFLAGS_$(QUALIFIED_VARIANT.cc))

QUALIFIED_CPPFLAGS.cc = \
 $(patsubst $(notdir $<):%,%, \
 $(filter -% $(notdir $<):%,$(QUALIFIED_CPPFLAGS))) \
 $(QUALIFIED_VARIANT_CPPFLAGS.cc)

#QUALIFIED_C++FLAGS.cc = \
# $(patsubst $(patsubst $(SRCDIR)/%,%,$<):%,%, \
# $(filter -% $(patsubst $(SRCDIR)/%,%,$<):%,$(QUALIFIED_C++FLAGS)))

QUALIFIED_VARIANT_C++FLAGS.cc = \
 $(C++FLAGS_$(QUALIFIED_VARIANT.cc)) \
 $($(C++COMPILER)_C++FLAGS_$(QUALIFIED_VARIANT.cc))

QUALIFIED_C++FLAGS.cc = \
 $(patsubst $(notdir $<):%,%, \
 $(filter -% $(notdir $<):%,$(QUALIFIED_C++FLAGS))) \
 $(QUALIFIED_VARIANT_C++FLAGS.cc)

CPPFLAGS.cc = $(QUALIFIED_CPPFLAGS.cc) $(CPPFLAGS)

C++FLAGS.cc = $(QUALIFIED_C++FLAGS.cc) $(C++FLAGS)

LDFLAGS.cc = $(LDFLAGS)

LDLIBS1.cc = \
 $(filter-out $< $(MK)/lib$(LIBEXT) $(LDLIBS) \
 $(patsubst -l%,\%/lib%$(LIBEXT),$(LDLIBS)), \
 $(filter %$(LIBEXT) %$(OBJEXT),$^)) \
 $(filter $(MK)/lib$(LIBEXT),$^)

LDLIBS2.cc = $(LDLIBS)

LDLIBS.cc = $(LDLIBS1.cc) $(LDLIBS2.cc)

# Overrides for above for Micro$oft compiler.

ifneq "$(filter MS%,$(C++COMPILER))" ""

CPPFLAGS.cc = @$(MK)/cc.cfg $(QUALIFIED_CPPFLAGS.cc) \
 $(filter-out -D% -I% -U%,$(CPPFLAGS))

C++FLAGS.cc = $(QUALIFIED_C++FLAGS.cc) $(filter-out -D% -I% -U%,$(C++FLAGS))

LDFLAGS.cc = @$(MK)/cc.link.cfg -link $(filter-out -L%,$(LDFLAGS))

LDLIBS1.cc = \
 $(filter-out $< $(MK)/lib$(LIBEXT) $(LDLIBS) \
 $(patsubst -l%,\%/%$(LIBEXT),$(LDLIBS)), \
 $(filter %$(LIBEXT) %$(OBJEXT),$^)) \
 $(filter $(MK)/lib$(LIBEXT),$^)

LDLIBS2.cc = \
 $(filter-out -l% %.dll %.DLL %.lib %.LIB %.obj %.OBJ,$(LDLIBS))

endif

## Complete commands. Note that RUN-TCOMPILE.cc is defined further
## down in this file.

RUN-COMPILE.cc = \
 $(COMPILE.cc.o) $< $(TARGET.cc)

RUN-COMPILE.cc.so = \
 $(COMPILE.cc.so) $< $(TARGET.cc)

RUN-PCOMPILE.cc = \
 $(PCOMPILE.cc) $(PROGRAM_CPPFLAGS) $(PROGRAM_C++FLAGS) $< $(TARGET.cc)

RUN-CLINK.cc = \
 $(CLINK.cc) $(PROGRAM_CPPFLAGS) $(PROGRAM_C++FLAGS) $< \
 $(LDFLAGS.cc) $(LDLIBS.cc) -o $@ || ($(RM) $@; exit 1)

RUN-CLINK.cc.memcheck = \
 $(CLINK.cc.memcheck) $(PROGRAM_CPPFLAGS) $(PROGRAM_C++FLAGS) $< \
 $(LDFLAGS.cc) $(LDLIBS.cc) -o $@

RUN-LINK.cc = \
 $(LINK.cc) $< $(LDFLAGS.cc) $(LDLIBS.cc) -o $@ || ($(RM) $@; exit 1)

RUN-LINK.cc.memcheck =  \
 $(LINK.cc.memcheck) $< $(LDFLAGS.cc) $(LDLIBS.cc) -o $@

# Overrides for above rules for Micro$oft compiler. In this case
# we need to use command files as it isn't possible to munge some
# options so they will work on the command line. Also have to avoid
# problems with command line length when using MKS Korn Shell.

ifneq "$(filter MS%,$(C++COMPILER))" ""

.PHONY : $(MK)/cc.cfg
.PHONY : $(MK)/cc.link.cfg

makeit.setup :: $(MK)/cc.cfg

makeit.setup :: $(MK)/cc.link.cfg

$(MK)/cc.cfg :
	@$(RM) $@
	@echo $(filter -D% -I% -U%,$(CPPFLAGS)) | \
	 sed -e 's%-I//\([a-zA-Z]\)/%-I\1:/%g' \
	 -e 's%-I/cygdrive/\([a-zA-Z]\)/%-I\1:/%g' >> $@
	@echo $(filter -D% -I% -U%,$(C++FLAGS)) | \
	 sed -e 's%-I//\([a-zA-Z]\)/%-I\1:/%g' \
	 -e 's%-I/cygdrive/\([a-zA-Z]\)/%-I\1:/%g' >> $@

$(MK)/cc.link.cfg :
	@$(RM) $@
	@echo '-link' >> $@
	@echo $(filter %.dll %.DLL %.lib %.LIB %.obj %.OBJ, \
	 $(patsubst -l%,%$(LIBEXT),$(LDLIBS))) | sed -e 's%/%\\%g' >> $@

ifeq "$(ENV_TOOLSET)" "CYGWIN"

override LIB.cc = \
 $(subst /,\,$(subst $(empty) $(empty),;,$(patsubst -L%,%, \
 $(shell echo $(filter -L%,$(LDFLAGS)) | \
 sed -e 's%-L//\([a-zA-Z]\)/%-L\1:/%g' \
 -e 's%-L/cygdrive/\([a-zA-Z]\)/%-L\1:/%g' | tr '' ';')));$(lib);$(LIB))

else

override LIB.cc = \
 $(subst /,\,$(subst $(empty) $(empty),;,$(patsubst -L%,%, \
 $(shell echo $(filter -L%,$(LDFLAGS)) | \
 sed -e 's%-L//\([a-zA-Z]\)/%-L\1:/%g' \
 -e 's%-L/cygdrive/\([a-zA-Z]\)/%-L\1:/%g')));$(lib);$(LIB))

endif

RUN-COMPILE.cc = \
 $(COMPILE.cc.o) -Tp $< $(TARGET.cc)

RUN-COMPILE.cc.so = \
 $(COMPILE.cc.so) -Tp $< $(TARGET.cc)

RUN-PCOMPILE.cc = \
 $(PCOMPILE.cc) $(PROGRAM_CPPFLAGS) $(PROGRAM_C++FLAGS) -Tp $< $(TARGET.cc)

RUN-CLINK.cc = \
 LIB="${LIB.cc}" lib="${LIB.cc}" \
 $(CLINK.cc) $(PROGRAM_CPPFLAGS) $(PROGRAM_C++FLAGS) -Tp $< -o $@ \
 $(LDFLAGS.cc) $(LDLIBS.cc) || ($(RM) $@; exit 1)

RUN-CLINK.cc.memcheck = \
 LIB="${LIB.cc}" lib="${LIB.cc}" \
 $(CLINK.cc.memcheck) $(PROGRAM_CPPFLAGS) $(PROGRAM_C++FLAGS) -Tp $< -o $@ \
 $(LDFLAGS.cc) $(LDLIBS.cc)

RUN-LINK.cc = \
 LIB="${LIB.cc}" lib="${LIB.cc}" \
 $(LINK.cc) $< -o $@ $(LDFLAGS.cc) $(LDLIBS.cc) || ($(RM) $@; exit 1)

RUN-LINK.cc.memcheck =  \
 LIB="${LIB.cc}" lib="${LIB.cc}" \
 $(LINK.cc.memcheck) $< -o $@ $(LDFLAGS.cc) $(LDLIBS.cc)

endif

## Wipe out standard target for creation of object file from src file
## and program from src file.

%.o : %.cc
%.obj : %.cc

%$(BINEXT) : %.cc

## Make sure we exclude temporary files created by Versant.

override EXCLUDE += __PClass_%.c

## Calculate all language src.

_cc_real_SRC := \
 $(filter-out $(EXCLUDE),$(filter %.cc,$(SRCFILES)))

_cc_generated_MK_SRC := \
 $(addprefix $(MK)/,$(_cc_generated_SRC))

_cc_make_MK_SRC := \
 $(filter-out $(EXCLUDE),$(_cc_generated_MK_SRC))

_cc_make_SRC := \
 $(notdir $(_cc_make_MK_SRC))

## Calculate program src.

_cc_real_PROGRAM_SRC := \
 $(filter-out $(EXCLUDE),$(filter $(addsuffix .cc,$(PROGRAMS)),$(SRCFILES)))

_cc_generated_MK_PROGRAM_SRC := \
 $(addprefix $(MK)/,$(_cc_generated_PROGRAM_SRC))

_cc_make_MK_PROGRAM_SRC := \
 $(filter-out $(EXCLUDE),$(_cc_generated_MK_PROGRAM_SRC))

_cc_make_PROGRAM_SRC := \
 $(notdir $(_cc_make_MK_PROGRAM_SRC))

## Calculate non library src.

_cc_real_NONLIB_SRC := \
 $(filter-out $(EXCLUDE),$(filter %.cc,$(NONLIBSRC)))

_cc_generated_MK_NONLIB_SRC := \
 $(addprefix $(MK)/,$(_cc_generated_NONLIB_SRC))

_cc_make_MK_NONLIB_SRC := \
 $(filter-out $(EXCLUDE),$(_cc_generated_MK_NONLIB_SRC))

_cc_make_NONLIB_SRC := \
 $(notdir $(_cc_make_NONLIB_SRC))

## Calculate library src.

_cc_real_LIB_SRC := \
 $(filter-out $(SCHEMAS) $(_cc_TEMPLATES) $(_cc_real_NONLIB_SRC) \
  $(_cc_real_PROGRAM_SRC),$(_cc_real_SRC))

_cc_make_MK_LIB_SRC := \
 $(filter-out \
 $(_cc_make_MK_NONLIB_SRC) $(_cc_make_MK_PROGRAM_SRC), \
 $(_cc_make_MK_SRC))

_cc_make_LIB_SRC := \
 $(notdir $(_cc_make_MK_LIB_SRC))

## Setup library dependencies for real src.

ifneq "$(_cc_real_LIB_SRC)" ""

_cc_real_LIB_OBJECTS := \
 $(patsubst %.cc,%.o,$(_cc_real_LIB_SRC))

_cc_real_MK_LIB_OBJECTS := \
 $(patsubst %.o,$(MK)/%$(OBJEXT),$(_cc_real_LIB_OBJECTS))

$(_cc_real_MK_LIB_OBJECTS) : $(MK)/%$(OBJEXT) : %.cc
	$(RUN-COMPILE.cc)
	@touch $(MK)/lib.standin

$(_cc_real_LIB_OBJECTS) : %.o : \
 makeit.setup $(MK)/%$(OBJEXT)

ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifeq "$(_makeit_separate_pic_objects)" "YES"

_cc_real_MK_SHLIB_OBJECTS := \
 $(patsubst %.o,$(MK).pic/%$(OBJEXT),$(_cc_real_LIB_OBJECTS))

$(_cc_real_MK_SHLIB_OBJECTS) : $(MK).pic/%$(OBJEXT) : %.cc
	$(RUN-COMPILE.cc.so)
	@touch $(MK)/pic.standin

$(patsubst %.o,%.so,$(_cc_real_LIB_OBJECTS)) : %.so : \
 makeit.setup $(MK).pic/%$(OBJEXT)

endif
endif

_makeit_MK_LIB_OBJECTS += $(_cc_real_MK_LIB_OBJECTS)

endif

## Setup library dependencies for generated src.

ifneq "$(_cc_make_MK_LIB_SRC)" ""

_cc_make_LIB_OBJECTS := \
 $(patsubst %.cc,%.o,$(_cc_make_LIB_SRC))

_cc_make_MK_LIB_OBJECTS := \
 $(patsubst %.o,$(MK)/%$(OBJEXT),$(_cc_make_LIB_OBJECTS))

$(_cc_make_MK_LIB_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.cc
	$(RUN-COMPILE.cc)
	@touch $(MK)/lib.standin

$(_cc_make_LIB_OBJECTS) : %.o : \
 makeit.setup $(MK)/%$(OBJEXT)

ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifeq "$(_makeit_separate_pic_objects)" "YES"

_cc_make_MK_SHLIB_OBJECTS := \
 $(patsubst %.o,$(MK).pic/%$(OBJEXT),$(_cc_make_LIB_OBJECTS))

$(_cc_make_MK_SHLIB_OBJECTS) : $(MK).pic/%$(OBJEXT) : $(MK)/%.cc
	$(RUN-COMPILE.cc.so)
	@touch $(MK)/pic.standin

$(patsubst %.o,%.so,$(_cc_make_LIB_OBJECTS)) : %.so : \
 makeit.setup $(MK).pic/%$(OBJEXT)

endif
endif

_makeit_MK_LIB_OBJECTS += $(_cc_make_MK_LIB_OBJECTS)

endif

## Setup dependencies for non library objects coming from real src.

ifneq "$(_cc_real_NONLIB_SRC)" ""

_cc_real_NONLIB_OBJECTS := \
 $(patsubst %.cc,%$(OBJEXT),$(_cc_real_NONLIB_SRC))

_cc_real_MK_NONLIB_OBJECTS := \
 $(addprefix $(MK)/,$(_cc_real_NONLIB_OBJECTS))

$(_cc_real_MK_NONLIB_OBJECTS) : $(MK)/%$(OBJEXT) : %.cc
	$(RUN-COMPILE.cc)

$(_cc_real_NONLIB_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cc_real_NONLIB_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

endif

## Setup dependencies for non library objects coming from generated src.

ifneq "$(_cc_make_MK_NONLIB_SRC)" ""

_cc_make_MK_NONLIB_OBJECTS := \
 $(patsubst %.cc,%$(OBJEXT),$(_cc_make_MK_NONLIB_SRC))

_cc_make_NONLIB_OBJECTS := \
 $(notdir $(_cc_make_MK_NONLIB_OBJECTS))

$(_cc_make_MK_NONLIB_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.cc
	$(RUN-COMPILE.cc)

$(_cc_make_NONLIB_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cc_make_NONLIB_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

endif

## Setup template expansion file dependencies.

ifeq "$(_makeit_cxx_supports_template_file)" "YES"
_cc_TPL_OBJECTS := $(patsubst %.cc,%$(OBJEXT),$(_cc_TEMPLATES))
endif

_cc_MK_TPL_OBJECTS := \
 $(addprefix $(MK)/,$(_cc_TPL_OBJECTS))

ifneq "$(_cc_TPL_OBJECTS)" ""

ifeq "$(_makeit_cxx_supports_cfront_repositories)" "YES"

ifneq "$(filter cfront_alltc_patch,$(MAKEIT_OPTIONS))" ""
$(MK)/__ptinst :
	echo "@alltc" > $@
else
$(MK)/__ptinst :
	echo "@all" > $@
endif

RUN-TCOMPILE.cc = \
 $(COMPILE.cc) +T$(MK)/__ptinst $(TEMPLATEFLAGS.cc) $< $(TARGET.cc)

$(_cc_MK_TPL_OBJECTS) : $(MK)/%$(OBJEXT) : %.cc $(MK)/__ptinst
	$(RUN-TCOMPILE.cc)

else

RUN-TCOMPILE.cc = \
 $(COMPILE.cc) $(TEMPLATEFLAGS.cc) $< $(TARGET.cc)

$(_cc_MK_TPL_OBJECTS) : $(MK)/%$(OBJEXT) : %.cc
	$(RUN-TCOMPILE.cc)

endif

$(_cc_TPL_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cc_TPL_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

endif

## Setup dependencies for programs coming from real src.

ifneq "$(_cc_real_PROGRAM_SRC)" ""

_cc_real_PROGRAM_OBJECTS := \
 $(patsubst %.cc,%$(OBJEXT),$(_cc_real_PROGRAM_SRC))

_cc_real_MK_PROGRAM_OBJECTS := \
 $(addprefix $(MK)/,$(_cc_real_PROGRAM_OBJECTS))

_cc_real_MK_PROGRAMS := \
 $(addsuffix $(BINEXT),$(addprefix $(MK)/,$(basename $(_cc_real_PROGRAM_SRC))))

ifeq "$(_makeit_enable_memcheck)" "YES"
_cc_real_MK_MEMCHECK_PROGRAMS := \
 $(addsuffix .$(MEMEXT),$(_cc_real_MK_PROGRAMS))
endif

$(_cc_real_PROGRAM_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cc_real_PROGRAM_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

ifeq "$(_makeit_combine_compile_and_link)" "YES"

$(_cc_real_MK_PROGRAMS) \
 : $(MK)/%$(BINEXT) : %.cc $(_cc_MK_TPL_OBJECTS)
	@$(RM) $@
	$(RUN-CLINK.cc)
	@$(RM) $(<F:.cc=$(OBJEXT))
	$(POSTLINK.cc)

ifeq "$(_makeit_enable_memcheck)" "YES"

$(_cc_real_MK_MEMCHECK_PROGRAMS) \
 : $(MK)/%$(BINEXT).$(MEMEXT) : %.cc $(_cc_MK_TPL_OBJECTS)
	@$(RM) $@
	$(RUN-CLINK.cc.memcheck)
	@$(RM) $(<F:.cc=$(OBJEXT)) *.pure_* *_pure_*$(OBJEXT)
	$(POSTLINK.cc.memcheck)

endif

else

$(_cc_real_MK_PROGRAMS) \
 : $(MK)/%$(BINEXT) : $(MK)/%$(OBJEXT) $(_cc_MK_TPL_OBJECTS)
	@$(RM) $@
	$(RUN-LINK.cc)
	$(POSTLINK.cc)

ifeq "$(_makeit_enable_memcheck)" "YES"

$(_cc_real_MK_MEMCHECK_PROGRAMS) \
 : $(MK)/%$(BINEXT).$(MEMEXT) : $(MK)/%$(OBJEXT) $(_cc_MK_TPL_OBJECTS)
	@$(RM) $@
	$(RUN-LINK.cc.memcheck)
	$(POSTLINK.cc.memcheck)

endif

endif

$(_cc_real_MK_PROGRAM_OBJECTS) : $(MK)/%$(OBJEXT) : %.cc
	$(RUN-PCOMPILE.cc)

_makeit_MK_BINARIES += $(_cc_real_MK_PROGRAMS)

endif

## Setup dependencies for programs coming from generated src.

ifneq "$(_cc_make_MK_PROGRAM_SRC)" ""

_cc_make_MK_PROGRAM_OBJECTS := \
 $(patsubst %.cc,%$(OBJEXT),$(_cc_make_MK_PROGRAM_SRC))

_cc_make_PROGRAM_OBJECTS := \
 $(notdir $(_cc_make_MK_PROGRAM_OBJECTS))

_cc_make_MK_PROGRAMS := \
 $(addsuffix $(BINEXT),$(basename $(_cc_make_MK_PROGRAM_SRC)))

ifeq "$(_makeit_enable_memcheck)" "YES"
_cc_make_MK_MEMCHECK_PROGRAMS := \
 $(addsuffix .$(MEMEXT),$(_cc_make_MK_PROGRAMS))
endif

$(_cc_make_PROGRAM_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

ifeq "$(_makeit_combine_compile_and_link)" "YES"

$(_cc_make_MK_PROGRAMS) \
 : $(MK)/%$(BINEXT) : $(MK)/%.cc $(_cc_MK_TPL_OBJECTS)
	@$(RM) $@
	$(RUN-CLINK.cc)
	@$(RM) $(<F:.cc=$(OBJEXT))
	$(POSTLINK.cc)

ifeq "$(_makeit_enable_memcheck)" "YES"

$(_cc_make_MK_MEMCHECK_PROGRAMS) \
 : $(MK)/%$(BINEXT).$(MEMEXT) : $(MK)/%.cc $(_cc_MK_TPL_OBJECTS)
	@$(RM) $@
	$(RUN-CLINK.cc.memcheck)
	@$(RM) $(<F:.cc=$(OBJEXT)) *.pure_* *_pure_*$(OBJEXT)
	$(POSTLINK.cc.memcheck)

endif

else

$(_cc_make_MK_PROGRAMS) \
 : $(MK)/%$(BINEXT) : $(MK)/%$(OBJEXT) $(_cc_MK_TPL_OBJECTS)
	@$(RM) $@
	$(RUN-LINK.cc)
	$(POSTLINK.cc)

ifeq "$(_makeit_enable_memcheck)" "YES"

$(_cc_make_MK_MEMCHECK_PROGRAMS) \
 : $(MK)/%$(BINEXT).$(MEMEXT) : $(MK)/%$(OBJEXT) $(_cc_MK_TPL_OBJECTS)
	@$(RM) $@
	$(RUN-LINK.cc.memcheck)
	$(POSTLINK.cc.memcheck)

endif

endif

$(_cc_make_MK_PROGRAM_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.cc
	$(RUN-PCOMPILE.cc)

_makeit_MK_BINARIES += $(_cc_make_MK_PROGRAMS)

endif

## Dependency files.

DEPFILTER.cc = sed -f $(OSE_MAKEIT)/depend.sed | sort -u

# DEPFILTER.cc = sed -n -e 's/\# *[1-9][0-9]* "\(.*\)".*$$/TARGET : \1/p' \
#  -e 's/\#line [1-9][0-9]* "\(.*\)".*$$/TARGET : \1/p' | sort -u

ifneq "$(_cc_TEMPLATES)" ""

_cc_TEMPLATE_D := $(patsubst %.cc,$(MK)/%.d,$(_cc_TEMPLATES))

$(_cc_TEMPLATE_D) : $(MK)/%.d : %.cc
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cc) $(TEMPLATEFLAGS.cc) $< | $(DEPFILTER.cc) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK).pic/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cc_TEMPLATE_D)

endif

ifneq "$(_cc_real_LIB_SRC)" ""

_cc_real_LIB_D := $(patsubst %.cc,$(MK)/%.d,$(_cc_real_LIB_SRC))

$(_cc_real_LIB_D) : $(MK)/%.d : %.cc
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cc) $< | $(DEPFILTER.cc) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK).pic/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cc_real_LIB_D)

endif

ifneq "$(_cc_make_MK_LIB_SRC)" ""

_cc_make_LIB_D := $(patsubst %.cc,%.d,$(_cc_make_MK_LIB_SRC))

$(_cc_make_LIB_D) : $(MK)/%.d : $(MK)/%.cc
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cc) $< | $(DEPFILTER.cc) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK).pic/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cc_make_LIB_D)

endif

ifneq "$(_cc_real_NONLIB_SRC)" ""

_cc_real_NONLIB_D := $(patsubst %.cc,$(MK)/%.d,$(_cc_real_NONLIB_SRC))

$(_cc_real_NONLIB_D) : $(MK)/%.d : %.cc
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cc) $< | $(DEPFILTER.cc) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cc_real_NONLIB_D)

endif

ifneq "$(_cc_make_MK_NONLIB_SRC)" ""

_cc_make_NONLIB_D := $(patsubst %.cc,%.d,$(_cc_make_MK_NONLIB_SRC))

$(_cc_make_NONLIB_D) : $(MK)/%.d : $(MK)/%.cc
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cc) $< | $(DEPFILTER.cc) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cc_make_NONLIB_D)

endif

ifneq "$(_cc_real_PROGRAM_SRC)" ""

_cc_real_PROGRAM_D := $(patsubst %.cc,$(MK)/%.d,$(_cc_real_PROGRAM_SRC))

$(_cc_real_PROGRAM_D) : $(MK)/%.d : %.cc
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cc) $(TEMPLATEFLAGS.cc) $< | $(DEPFILTER.cc) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(BINEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cc_real_PROGRAM_D)

endif

ifneq "$(_cc_make_MK_PROGRAM_SRC)" ""

_cc_make_PROGRAM_D := $(patsubst %.cc,%.d,$(_cc_make_MK_PROGRAM_SRC))

$(_cc_make_PROGRAM_D) : $(MK)/%.d : $(MK)/%.cc
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cc) $(TEMPLATEFLAGS.cc) $< | $(DEPFILTER.cc) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(BINEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cc_make_PROGRAM_D)

endif
