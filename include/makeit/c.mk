## Copyright 1992-1993 OTC LIMITED
## Copyright 1994-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

vpath %.h $(SRCDIR) $(SRCDIRS)
vpath %.c $(SRCDIR) $(SRCDIRS)

## Define compilation commands. We cannot use ":=" for assignment here
## so must use only "=" and "+=". This is necessary to ensure that
## variables are only expanded when action is actually run. This
## restriction means that commands must be constructed from the front
## and all new bits added to the end only.

PREPROCESS.c =
COMPILE.c =
LINK.c =
COMPILE.c.o =
COMPILE.c.so =
LINK.c.memcheck =

LINKCOMMAND.c =
LINKCOMMAND.c.memcheck =

ifeq "$(_makeit_cxx_supports_o_for_objects)" "YES"
TARGET.c = -o $@
else
TARGET.c = && mv $(<F:.c=$(OBJEXT)) $@
endif
 
# Purify/Sentinel all applications by default. Purify takes precedence.
# If LINKCOMMAND.c is defined however, it will take precedence.

ifeq "$(_makeit_enable_memcheck_all)" "YES"
LINKCOMMAND.c += $(MEMCHECKALL)
endif

# Normal commands.

LINKCOMMAND.c += $(CC)
LINKCOMMAND.c.memcheck += $(MEMCHECK) $(CC)

PREPROCESS.c += $(CC) -E $(CFLAGS.c) $(CPPFLAGS.c)
COMPILE.c += $(CC) $(CFLAGS.c) $(CPPFLAGS.c) -c

LINK.c += $(LINKCOMMAND.c) $(CFLAGS.c) $(CPPFLAGS.c)
LINK.c.memcheck += $(LINKCOMMAND.c.memcheck) $(CFLAGS.c) $(CPPFLAGS.c)

# PIC flags.

COMPILE.c.o += $(COMPILE.c)

ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifneq "$(_makeit_separate_pic_objects)" "YES"
COMPILE.c.o += -DPIC $(PIC_CPPFLAGS)
endif
endif

COMPILE.c.so += $(COMPILE.c) $(PIC_CFLAGS)

COMPILE.c.so += -DPIC $(PIC_CPPFLAGS)

## Modify various compiler flags etc as required for this module.

#QUALIFIED_CPPFLAGS.c = \
# $(patsubst $(patsubst $(SRCDIR)/%,%,$<):%,%, \
# $(filter -% $(patsubst $(SRCDIR)/%,%,$<):%,$(QUALIFIED_CPPFLAGS)))

#QUALIFIED_CPPFLAGS.c = \
# $(patsubst $(notdir $<):%,%, \
# $(filter -% $(notdir $<):%,$(QUALIFIED_CPPFLAGS)))

QUALIFIED_VARIANT.c = \
 $(firstword $(patsubst $(VARIANT)=%,%, \
 $(filter $(VARIANT)=%, $(patsubst $(notdir $<):%,%, \
 $(filter $(notdir $<):%,$(QUALIFIED_VARIANT))))) $(VARIANT))

QUALIFIED_VARIANT_CPPFLAGS.c = \
 $(CPPFLAGS_$(QUALIFIED_VARIANT.c)) \
 $($(C++COMPILER)_CPPFLAGS_$(QUALIFIED_VARIANT.c))

QUALIFIED_CPPFLAGS.c = \
 $(patsubst $(notdir $<):%,%, \
 $(filter -% $(notdir $<):%,$(QUALIFIED_CPPFLAGS))) \
 $(QUALIFIED_VARIANT_CPPFLAGS.c)

#QUALIFIED_CFLAGS.c = \
# $(patsubst $(patsubst $(SRCDIR)/%,%,$<):%,%, \
# $(filter -% $(patsubst $(SRCDIR)/%,%,$<):%,$(QUALIFIED_CFLAGS)))

QUALIFIED_VARIANT_CFLAGS.c = \
 $(CFLAGS_$(QUALIFIED_VARIANT.c)) \
 $($(C++COMPILER)_CFLAGS_$(QUALIFIED_VARIANT.c))

QUALIFIED_CFLAGS.c = \
 $(patsubst $(notdir $<):%,%, \
 $(filter -% $(notdir $<):%,$(QUALIFIED_CFLAGS))) \
 $(QUALIFIED_VARIANT_CFLAGS.c)

CPPFLAGS.c = $(QUALIFIED_CPPFLAGS.c) $(CPPFLAGS)

CFLAGS.c = $(QUALIFIED_CFLAGS.c) $(CFLAGS)

LDFLAGS.c = $(LDFLAGS)

LDLIBS1.c = \
 $(filter-out $< $(MK)/lib$(LIBEXT) $(LDLIBS) \
 $(patsubst -l%,\%/lib%$(LIBEXT),$(LDLIBS)), \
 $(filter %$(LIBEXT) %$(OBJEXT),$^)) \
 $(filter $(MK)/lib$(LIBEXT),$^)

LDLIBS2.c = $(LDLIBS)

LDLIBS.c = $(LDLIBS1.c) $(LDLIBS2.c)

# Overrides for above for Micro$oft compiler.

ifneq "$(filter MS%,$(C++COMPILER))" ""

CPPFLAGS.c = @$(MK)/c.cfg $(QUALIFIED_CPPFLAGS.c) \
 $(filter-out -D% -I% -U%,$(CPPFLAGS))

CFLAGS.c = $(QUALIFIED_CFLAGS.c) $(filter-out -D% -I% -U%,$(CFLAGS))

LDFLAGS.c = @$(MK)/c.link.cfg -link $(filter-out -L%,$(LDFLAGS))

LDLIBS1.c = \
 $(filter-out $< $(MK)/lib$(LIBEXT) $(LDLIBS) \
 $(patsubst -l%,\%/%$(LIBEXT),$(LDLIBS)), \
 $(filter %$(LIBEXT) %$(OBJEXT),$^)) \
 $(filter $(MK)/lib$(LIBEXT),$^)

LDLIBS2.c = \
 $(filter-out -l% %.dll %.DLL %.lib %.LIB %.obj %.OBJ,$(LDLIBS))

endif

## Complete commands.

RUN-COMPILE.c = \
 $(COMPILE.c.o) $< $(TARGET.c)

RUN-COMPILE.c.so = \
 $(COMPILE.c.so) $< $(TARGET.c)

RUN-PCOMPILE.c = \
 $(COMPILE.c) $(PROGRAM_CPPFLAGS) $(PROGRAM_CFLAGS) $< $(TARGET.c)

RUN-LINK.c = \
 $(LINK.c) $< $(LDFLAGS.c) $(LDLIBS.c) -o $@ || ($(RM) $@; exit 1)

RUN-LINK.c.memcheck = \
 $(LINK.c.memcheck) $< $(LDFLAGS.c) $(LDLIBS.c) -o $@

# Overrides for above rules for Micro$oft compiler. In this case
# we need to use command files as it isn't possible to munge some
# options so they will work on the command line. Also have to avoid
# problems with command line length when using MKS Korn Shell.

ifneq "$(filter MS%,$(C++COMPILER))" ""

.PHONY : $(MK)/c.cfg
.PHONY : $(MK)/c.link.cfg

makeit.setup :: $(MK)/c.cfg

makeit.setup :: $(MK)/c.link.cfg

$(MK)/c.cfg :
	@$(RM) $@
	@echo $(filter -D% -I% -U%,$(CPPFLAGS)) | \
	 sed -e 's%-I//\([a-zA-Z]\)/%-I\1:/%g' \
	 -e 's%-I/cygdrive/\([a-zA-Z]\)/%-I\1:/%g' >> $@
	@echo $(filter -D% -I% -U%,$(CFLAGS)) | \
	 sed -e 's%-I//\([a-zA-Z]\)/%-I\1:/%g' \
	 -e 's%-I/cygdrive/\([a-zA-Z]\)/%-I\1:/%g' >> $@

$(MK)/c.link.cfg :
	@$(RM) $@
	@echo '-link' >> $@
	@echo $(filter %.dll %.DLL %.lib %.LIB %.obj %.OBJ, \
	 $(patsubst -l%,%$(LIBEXT),$(LDLIBS))) | sed -e 's%/%\\%g' >> $@

ifeq "$(ENV_TOOLSET)" "CYGWIN"

override LIB.c = \
 $(subst /,\,$(subst $(empty) $(empty),;,$(patsubst -L%,%, \
 $(shell echo $(filter -L%,$(LDFLAGS)) | \
 sed -e 's%-L//\([a-zA-Z]\)/%-L\1:/%g' \
 -e 's%-L/cygdrive/\([a-zA-Z]\)/%-L\1:/%g' | tr '' ';')));$(LIB))

else

override LIB.c = \
 $(subst /,\,$(subst $(empty) $(empty),;,$(patsubst -L%,%, \
 $(shell echo $(filter -L%,$(LDFLAGS)) | \
 sed -e 's%-L//\([a-zA-Z]\)/%-L\1:/%g' \
 -e 's%-L/cygdrive/\([a-zA-Z]\)/%-L\1:/%g')));$(LIB))

endif

RUN-COMPILE.c = \
 $(COMPILE.c) -Tc $< $(TARGET.c)

RUN-COMPILE.c.so = \
 $(COMPILE.c.so) -Tc $< $(TARGET.c)

RUN-PCOMPILE.c = \
 $(COMPILE.c) $(PROGRAM_CPPFLAGS) $(PROGRAM_CFLAGS) -Tc $< $(TARGET.c)

RUN-LINK.c = \
 LIB="${LIB.c}" \
 $(LINK.c) $< -o $@ $(LDFLAGS.c) $(LDLIBS.c) || ($(RM) $@; exit 1)

RUN-LINK.c.memcheck = \
 LIB="${LIB.c}" \
 $(LINK.c.memcheck) $< -o $@ $(LDFLAGS.c) $(LDLIBS.c)

endif

## Wipe out standard target for creation of object file from src file
## and program from src file.

%$(OBJEXT) : %.c
% : %.c

## Make sure we exclude output of cfront and kfront style C++ compilers.

override EXCLUDE += %..c %.int.c

## Calculate all language src.

_c_real_SRC := \
 $(filter-out $(EXCLUDE),$(filter %.c,$(SRCFILES)))

_c_generated_MK_SRC := \
 $(addprefix $(MK)/,$(_c_generated_SRC))

_c_make_MK_SRC :=  \
 $(filter-out $(EXCLUDE),$(_c_generated_MK_SRC))

_c_make_SRC :=  \
 $(notdir $(_c_make_MK_SRC))

## Calculate program src.

_c_real_PROGRAM_SRC := \
 $(filter-out $(EXCLUDE),$(filter $(addsuffix .c,$(PROGRAMS)),$(SRCFILES)))

_c_generated_MK_PROGRAM_SRC := \
 $(addprefix $(MK)/,$(_c_generated_PROGRAM_SRC))

_c_make_MK_PROGRAM_SRC := \
 $(filter-out $(EXCLUDE),$(_c_generated_MK_PROGRAM_SRC))

_c_make_PROGRAM_SRC := \
 $(notdir $(_c_make_MK_PROGRAM_SRC))

## Calculate non library src.

_c_real_NONLIB_SRC := \
 $(filter-out $(EXCLUDE),$(filter %.c,$(NONLIBSRC)))

_c_generated_MK_NONLIB_SRC := \
 $(addprefix $(MK)/,$(_c_generated_NONLIB_SRC))

_c_make_MK_NONLIB_SRC := \
 $(filter-out $(EXCLUDE),$(_c_generated_MK_NONLIB_SRC))

_c_make_NONLIB_SRC := \
 $(notdir $(_c_make_MK_NONLIB_SRC))

## Calculate library src.

_c_real_LIB_SRC := \
 $(filter-out \
 $(SCHEMAS) $(_c_real_NONLIB_SRC) $(_c_real_PROGRAM_SRC), \
 $(_c_real_SRC))

_c_make_MK_LIB_SRC := \
 $(filter-out \
 $(_c_make_MK_NONLIB_SRC) $(_c_make_MK_PROGRAM_SRC), \
 $(_c_make_MK_SRC))

_c_make_LIB_SRC := \
 $(notdir $(_c_make_MK_LIB_SRC))

## Setup library dependencies for real src.

ifneq "$(_c_real_LIB_SRC)" ""

_c_real_LIB_OBJECTS := \
 $(patsubst %.c,%.o,$(_c_real_LIB_SRC))

_c_real_MK_LIB_OBJECTS := \
 $(patsubst %.o,$(MK)/%$(OBJEXT),$(_c_real_LIB_OBJECTS))

$(_c_real_MK_LIB_OBJECTS) : $(MK)/%$(OBJEXT) : %.c
	$(RUN-COMPILE.c)
	@touch $(MK)/lib.standin

$(_c_real_LIB_OBJECTS) : %.o : \
 makeit.setup $(MK)/%$(OBJEXT)

ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifeq "$(_makeit_separate_pic_objects)" "YES"

_c_real_MK_SHLIB_OBJECTS := \
 $(patsubst %.o,$(MK).pic/%$(OBJEXT),$(_c_real_LIB_OBJECTS))

$(_c_real_MK_SHLIB_OBJECTS) : $(MK).pic/%$(OBJEXT) : %.c
	$(RUN-COMPILE.c.so)
	@touch $(MK)/pic.standin

$(patsubst %.o,%.so,$(_c_real_LIB_OBJECTS)) : %.so : \
 makeit.setup $(MK).pic/%$(OBJEXT)

endif
endif

_makeit_MK_LIB_OBJECTS += $(_c_real_MK_LIB_OBJECTS)

endif

## Setup library dependencies for generated src.

ifneq "$(_c_make_MK_LIB_SRC)" ""

_c_make_LIB_OBJECTS := \
 $(patsubst %.c,%.o,$(_c_make_LIB_SRC))

_c_make_MK_LIB_OBJECTS := \
 $(patsubst %.o,$(MK)/%$(OBJEXT),$(_c_make_LIB_OBJECTS))

$(_c_make_MK_LIB_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.c
	$(RUN-COMPILE.c)
	@touch $(MK)/lib.standin

$(_c_make_LIB_OBJECTS) : %.o : \
 makeit.setup $(MK)/%$(OBJEXT)

ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifeq "$(_makeit_separate_pic_objects)" "YES"

_c_make_MK_SHLIB_OBJECTS := \
 $(patsubst %.o,$(MK).pic/%$(OBJEXT),$(_c_make_LIB_OBJECTS))

$(_c_make_MK_SHLIB_OBJECTS) : $(MK).pic/%$(OBJEXT) : $(MK)/%.c
	$(RUN-COMPILE.c.so)
	@touch $(MK)/pic.standin

$(patsubst %.o,%.so,$(_c_make_LIB_OBJECTS)) : %.so : \
 makeit.setup $(MK).pic/%$(OBJEXT)

endif
endif

_makeit_MK_LIB_OBJECTS += $(_c_make_MK_LIB_OBJECTS)

endif

## Setup dependencies for non library objects coming from real src.

ifneq "$(_c_real_NONLIB_SRC)" ""

_c_real_NONLIB_OBJECTS := \
 $(patsubst %.c,%$(OBJEXT),$(_c_real_NONLIB_SRC))

_c_real_MK_NONLIB_OBJECTS := \
 $(addprefix $(MK)/,$(_c_real_NONLIB_OBJECTS))

$(_c_real_MK_NONLIB_OBJECTS) : $(MK)/%$(OBJEXT) : %.c
	$(RUN-COMPILE.c)

$(_c_real_NONLIB_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_c_real_NONLIB_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

endif

## Setup dependencies for non library objects coming from generated src.

ifneq "$(_c_make_MK_NONLIB_SRC)" ""

_c_make_MK_NONLIB_OBJECTS := \
 $(patsubst %.c,%$(OBJEXT),$(_c_make_MK_NONLIB_SRC))

_c_make_NONLIB_OBJECTS := \
 $(notdir $(_c_make_MK_NONLIB_OBJECTS))

$(_c_make_MK_NONLIB_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.c
	$(RUN-COMPILE.c)

$(_c_make_NONLIB_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_c_make_NONLIB_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

endif

## Setup dependencies for programs coming from real src.

ifneq "$(_c_real_PROGRAM_SRC)" ""

_c_real_PROGRAM_OBJECTS := \
 $(patsubst %.c,%$(OBJEXT),$(_c_real_PROGRAM_SRC))

_c_real_MK_PROGRAM_OBJECTS := \
 $(addprefix $(MK)/,$(_c_real_PROGRAM_OBJECTS))

_c_real_MK_PROGRAMS := \
 $(addsuffix $(BINEXT),$(addprefix $(MK)/,$(basename $(_c_real_PROGRAM_SRC))))

ifeq "$(_makeit_enable_memcheck)" "YES"
_c_real_MK_MEMCHECK_PROGRAMS := \
 $(addsuffix .$(MEMEXT),$(_c_real_MK_PROGRAMS))
endif

$(_c_real_MK_PROGRAMS) : $(MK)/%$(BINEXT) : $(MK)/%$(OBJEXT)
	@$(RM) $@
	$(RUN-LINK.c)

$(_c_real_PROGRAM_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_c_real_PROGRAM_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

ifeq "$(_makeit_enable_memcheck)" "YES"

$(_c_real_MK_MEMCHECK_PROGRAMS) : $(MK)/%$(BINEXT).$(MEMEXT) : $(MK)/%$(OBJEXT)
	@$(RM) $@
	$(RUN-LINK.c.memcheck)
	@$(RM) *.pure_* *_pure_*$(OBJEXT)

endif

$(_c_real_MK_PROGRAM_OBJECTS) : $(MK)/%$(OBJEXT) : %.c
	$(RUN-PCOMPILE.c)

_makeit_MK_BINARIES += $(_c_real_MK_PROGRAMS)

endif

## Setup dependencies for programs coming from generated src.

ifneq "$(_c_make_MK_PROGRAM_SRC)" ""

_c_make_MK_PROGRAM_OBJECTS := \
 $(patsubst %.c,%$(OBJEXT),$(_c_make_MK_PROGRAM_SRC))

_c_make_PROGRAM_OBJECTS := \
 $(notdir $(_c_make_MK_PROGRAM_OBJECTS))

_c_make_MK_PROGRAMS := \
 $(addsuffix $(BINEXT),$(basename $(_c_make_MK_PROGRAM_SRC)))

ifeq "$(_makeit_enable_memcheck)" "YES"
_c_make_MK_MEMCHECK_PROGRAMS := \
 $(addsuffix .$(MEMEXT),$(_c_make_MK_PROGRAMS))
endif

$(_c_make_MK_PROGRAMS) : $(MK)/%$(BINEXT) : $(MK)/%$(OBJEXT)
	@$(RM) $@
	$(RUN-LINK.c)

$(_c_make_PROGRAM_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_c_make_PROGRAM_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

ifeq "$(_makeit_enable_memcheck)" "YES"

$(_c_make_MK_MEMCHECK_PROGRAMS) : $(MK)/%$(BINEXT).$(MEMEXT) : $(MK)/%$(OBJEXT)
	@$(RM) $@
	$(RUN-LINK.c.memcheck)
	@$(RM) *.pure_* *_pure_*$(OBJEXT)

endif

$(_c_make_MK_PROGRAM_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.c
	$(RUN-PCOMPILE.c)

_makeit_MK_BINARIES += $(_c_make_MK_PROGRAMS)

endif

## Dependency files.

DEPFILTER.c = sed -f $(OSE_MAKEIT)/depend.sed | sort -u

# DEPFILTER.c = sed -n -e 's/\# *[1-9][0-9]* "\(.*\)".*$$/TARGET : \1/p' \
#  -e 's/\#line [1-9][0-9]* "\(.*\)".*$$/TARGET : \1/p' | sort -u

ifneq "$(_c_real_LIB_SRC)" ""

_c_real_LIB_D := $(patsubst %.c,$(MK)/%.d,$(_c_real_LIB_SRC))

$(_c_real_LIB_D) : $(MK)/%.d : %.c
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.c) $< | $(DEPFILTER.c) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK).pic/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_c_real_LIB_D)

endif

ifneq "$(_c_make_MK_LIB_SRC)" ""

_c_make_LIB_D := $(patsubst %.c,%.d,$(_c_make_MK_LIB_SRC))

$(_c_make_LIB_D) : $(MK)/%.d : $(MK)/%.c
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.c) $< | $(DEPFILTER.c) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK).pic/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_c_make_LIB_D)

endif

ifneq "$(_c_real_NONLIB_SRC)" ""

_c_real_NONLIB_D := $(patsubst %.c,$(MK)/%.d,$(_c_real_NONLIB_SRC))

$(_c_real_NONLIB_D) : $(MK)/%.d : %.c
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.c) $< | $(DEPFILTER.c) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_c_real_NONLIB_D)

endif

ifneq "$(_c_make_MK_NONLIB_SRC)" ""

_c_make_NONLIB_D := $(patsubst %.c,%.d,$(_c_make_MK_NONLIB_SRC))

$(_c_make_NONLIB_D) : $(MK)/%.d : $(MK)/%.c
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.c) $< | $(DEPFILTER.c) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_c_make_NONLIB_D)

endif

ifneq "$(_c_real_PROGRAM_SRC)" ""

_c_real_PROGRAM_D := $(patsubst %.c,$(MK)/%.d,$(_c_real_PROGRAM_SRC))

$(_c_real_PROGRAM_D) : $(MK)/%.d : %.c
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.c) $< | $(DEPFILTER.c) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(BINEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_c_real_PROGRAM_D)

endif

ifneq "$(_c_make_MK_PROGRAM_SRC)" ""

_c_make_PROGRAM_D := $(patsubst %.c,%.d,$(_c_make_MK_PROGRAM_SRC))

$(_c_make_PROGRAM_D) : $(MK)/%.d : $(MK)/%.c
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.c) $< | $(DEPFILTER.c) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(BINEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_c_make_PROGRAM_D)

endif
