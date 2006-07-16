## Copyright 1992-1993 OTC LIMITED
## Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Standard targets.

.PHONY : combine
.PHONY : combine.setup
.PHONY : combine.target
.PHONY : combine.always

combine : combine.setup
combine : combine.target
combine : combine.always

combine.setup :: makeit.setup

combine.target :: combine.setup

combine.always :: combine.setup

## Setup library management macros.

ifeq "$($(C++COMPILER)_COMBINE_LIB_CLOSURE)" ""
override $(C++COMPILER)_COMBINE_LIB_CLOSURE = $($(C++COMPILER)_LIB_CLOSURE)
endif

ifeq "$($(C++COMPILER)_COMBINE_LIB_ARCHIVE)" ""
override $(C++COMPILER)_COMBINE_LIB_ARCHIVE = $($(C++COMPILER)_LIB_ARCHIVE)
endif

ifeq "$($(C++COMPILER)_COMBINE_LIB_EXTRACT)" ""
override $(C++COMPILER)_COMBINE_LIB_EXTRACT = $($(C++COMPILER)_LIB_EXTRACT)
endif

ifeq "$($(C++COMPILER)_COMBINE_PIC_CLOSURE)" ""
override $(C++COMPILER)_COMBINE_PIC_CLOSURE = $($(C++COMPILER)_PIC_CLOSURE)
endif

ifeq "$($(C++COMPILER)_COMBINE_PIC_ARCHIVE)" ""
override $(C++COMPILER)_COMBINE_PIC_ARCHIVE = $($(C++COMPILER)_PIC_ARCHIVE)
endif

ifeq "$($(C++COMPILER)_COMBINE_PIC_EXTRACT)" ""
override $(C++COMPILER)_COMBINE_PIC_EXTRACT = $($(C++COMPILER)_PIC_EXTRACT)
endif

ifeq "$($(C++COMPILER)_COMBINE_PIC_LIBRARY)" ""
override $(C++COMPILER)_COMBINE_PIC_LIBRARY = $($(C++COMPILER)_PIC_LIBRARY)
endif

## Work out which directories we are combining.

ifeq "$(LIBDIRS)" ""
ifneq "$(NONLIBDIRS)" ""
LIBDIRS := $(filter-out $(NONLIBDIRS),$(SUBDIRS))
else
LIBDIRS := $(SUBDIRS)
endif
endif

## Only do something if we have some directories to combine. This module
## isn't mean't to combine object files from the current directory. But,
## don't do anything if compilable code in this directory.

ifeq "$(_makeit_MK_LIB_OBJECTS)$(_makeit_MK_EXECUTABLES)" ""

ifneq "$(LIBDIRS)" ""

## Adjust variables listing files if not in SRCDIR.

ifneq "$(SRCDIR)" "."

ifneq "$(PTDEFMAP)" ""
override PTDEFMAP := $(firstword $(wildcard $(PTDEFMAP) \
 $(SRCDIR)/$(PTDEFMAP)) $(PTDEFMAP))
endif

ifneq "$(PTDUMMYS)" ""
override PTDUMMYS := $(foreach file,$(PTDUMMYS),$(firstword $(wildcard \
 $(file) $(SRCDIR)/$(file)) $(file)))
endif

ifneq "$(PTMAPS)" ""
override PTMAPS := $(foreach file,$(PTMAPS),$(firstword $(wildcard \
 $(file) $(SRCDIR)/$(file)) $(file)))
endif

endif

## How to extract and archive object files from/to a library.

PTRFLAGS.combine = -ptr$(MKPTR)

ifeq "$(filter ignore_repositories,$(MAKEIT_OPTIONS))" ""
ifneq "$(PTRDIRS)" ""
PTRFLAGS.combine += $(addprefix -ptr,$(PTRDIRS))
endif
endif

PIC_PTRFLAGS.combine = -ptr$(MKPTR).pic

ifeq "$(filter ignore_repositories,$(MAKEIT_OPTIONS))" ""
ifneq "$(PTRDIRS)" ""
ifeq "$(_makeit_cxx_supports_cafe_repositories)" "YES"
PIC_PTRFLAGS.combine += $(patsubst %,-ptr%.pic,$(PTRDIRS))
else
PIC_PTRFLAGS.combine += $(addprefix -ptr,$(PTRDIRS))
endif
endif
endif

LDFLAGS.combine = $(filter $(LOPT)%,$(LDFLAGS))
LDLIBS.combine = $(filter -l%,$(LDLIBS))

LIBNAME.combine = $(firstword $(LIBRARY) NONAME)

ifeq "$(origin IMLEXT)" "undefined"
IMPLIB.combine =
else
IMPLIB.combine = $(MK)/$(IMLPRE)$(LIBNAME.combine)$(IMLEXT)
endif

LIBPATH.combine =

LIB_ARCHIVE.combine = \
 $(subst @LIB@,$(MK)/lib$(LIBEXT), \
 $(subst @LIBNAME@,$(LIBNAME.combine), \
 $(subst @OBJECTS@,*$(OBJEXT), \
 $(subst @ALLOBJECTS@,*$(OBJEXT), \
 $(subst @PTRFLAGS@,$(PTRFLAGS.combine), \
 $($(C++COMPILER)_COMBINE_LIB_ARCHIVE))))))

RUN-LIB_ARCHIVE.combine = \
 $(LIB_ARCHIVE.combine)

LIB_EXTRACT.combine = \
 $(subst @LIB@,$${lib}, \
 $($(C++COMPILER)_COMBINE_LIB_EXTRACT))

RUN-LIB_EXTRACT.combine = \
 $(LIB_EXTRACT.combine) \

PIC_EXTRACT.combine = \
 $(subst @LIB@,$${lib}, \
 $($(C++COMPILER)_COMBINE_PIC_EXTRACT))

RUN-PIC_EXTRACT.combine = \
 $(PIC_EXTRACT.combine) \

STL_ARCHIVE.combine = \
 $(subst @LIB@,$(MK)/$(STLPRE)$(LIBNAME.combine)$(STLEXT), \
 $(subst @LIBNAME@,$(LIBNAME.combine), \
 $(subst @OBJECTS@,*$(OBJEXT), \
 $(subst @ALLOBJECTS@,*$(OBJEXT), \
 $($(C++COMPILER)_COMBINE_PIC_ARCHIVE)))))

RUN-STL_ARCHIVE.combine = \
 $(STL_ARCHIVE.combine) \

PIC_ARCHIVE.combine = \
 $(subst @LIB@,$(MK)/pic$(LIBEXT), \
 $(subst @LIBNAME@,$(LIBNAME.combine), \
 $(subst @OBJECTS@,*$(OBJEXT), \
 $(subst @ALLOBJECTS@,*$(OBJEXT), \
 $($(C++COMPILER)_COMBINE_PIC_ARCHIVE)))))

RUN-PIC_ARCHIVE.combine = \
 $(PIC_ARCHIVE.combine)

PIC_LIBRARY.combine = \
 $(subst @LIB@,$(MK)/$(SHLPRE)$(LIBNAME.combine)$(SHLEXT), \
 $(subst @LIBFILE@,$(SHLPRE)$(LIBNAME.combine)$(SHLEXT), \
 $(subst @LIBNAME@,$(LIBNAME.combine), \
 $(subst @IMPLIB@,$(IMPLIB.combine), \
 $(subst @LDFLAGS@,$(LDFLAGS.combine), \
 $(subst @LDLIBS@,$(LDLIBS.combine), \
 $(subst @OBJECTS@,*$(OBJEXT), \
 $(subst @PIC_PTRFLAGS@,$(PIC_PTRFLAGS.combine), \
 $(subst @LIBPATH@,${LIBPATH.combine}, \
 $($(C++COMPILER)_COMBINE_PIC_LIBRARY))))))))))

RUN-PIC_LIBRARY.combine = \
 $(PIC_LIBRARY.combine)

## Setup any options for template closure on library.

ifeq "$(_makeit_cxx_supports_cfront_repositories)" "YES"

ifeq "$(filter LC%,$(C++COMPILER))" ""
_combine_defmap := defmap
else
_combine_defmap := defaultmap
endif

ifneq "$(filter closure,$(COMBINE_OPTIONS))" ""
_combine_cfront_closure := YES
endif

ifeq "$(filter minimal_expansion,$(CLOSURE_OPTIONS))" ""
override C++FLAGS += -pta
endif

ifneq "$(filter separate_functions,$(CLOSURE_OPTIONS))" ""
override C++FLAGS += -pts
endif

override PTRDIRS += $(addsuffix /$(MK).ptr,$(LIBDIRS))

ifeq "$(filter ignore_repositories,$(MAKEIT_OPTIONS))" ""
ifneq "$(PTRDIRS)" ""
override PTRFLAGS.combine = $(addprefix -ptr,$(PTRDIRS))
endif
endif

RUN-LIB_CLOSURE.combine = \
 $(C++) $(C++FLAGS) $(CPPFLAGS) $(LDFLAGS) \
 $(PTRFLAGS.combine) $(MK)/__ptdummy*.cc *$(OBJEXT) $(LDLIBS)

RUN-PIC_CLOSURE.combine = \
 $(C++) $(C++FLAGS) $(PIC_C++FLAGS) $(CPPFLAGS) $(LDFLAGS) \
  $(PIC_PTRFLAGS.combine) $(MK)/__ptdummy*.cc *$(OBJEXT) $(LDLIBS)

endif

ifneq "$(filter MS%,$(C++COMPILER))" ""

ifeq "$(_makeit_MK_LIB_OBJECTS)$(_makeit_MK_EXECUTABLES)" ""

.PHONY : $(MK)/lib.cfg

makeit.setup :: $(MK)/lib.cfg

$(MK)/lib.cfg :
	@$(RM) $@
	@echo '-link' >> $@
	@echo $(filter %.dll %.DLL %.lib %.LIB %.obj %.OBJ, \
	 $(patsubst -l%,%$(LIBEXT),$(LDLIBS))) | sed -e 's%/%\\%g' >> $@

endif

LDLIBS.combine = $(LDLIBS1.combine) $(LDLIBS2.combine)

LDFLAGS.combine = @$(MK)/lib.cfg -link $(filter-out -L%,$(LDFLAGS))

LDLIBS1.combine = \
 $(filter-out $< $(MK)/lib$(LIBEXT) $(LDLIBS) \
 $(patsubst -l%,\%/%$(LIBEXT),$(LDLIBS)), \
 $(filter %$(LIBEXT),$^)) \
 $(filter $(MK)/lib$(LIBEXT),$^)

LDLIBS2.combine = \
 $(filter-out -l% %.so %.DLL %.lib %.LIB %.obj %.OBJ,$(LDLIBS))

ifeq "$(ENV_TOOLSET)" "CYGWIN"

override LIBPATH.combine = \
 $(subst /,\,$(subst $(empty) $(empty),;,$(patsubst -L%,%, \
 $(shell echo $(filter -L%,$(LDFLAGS)) | \
 sed -e 's%-L//\([a-zA-Z]\)/%-L\1:/%g' \
 -e 's%-L/cygdrive/\([a-zA-Z]\)/%-L\1:/%g' | \
 tr '' ';')));$(lib);$(LIB))

else

override LIBPATH.combine = \
 $(subst /,\,$(subst $(empty) $(empty),;,$(patsubst -L%,%, \
 $(shell echo $(filter -L%,$(LDFLAGS)) | \
 sed -e 's%-L//\([a-zA-Z]\)/%-L\1:/%g' \
 -e 's%-L/cygdrive/\([a-zA-Z]\)/%-L\1:/%g')));$(lib);$(LIB))

endif

endif  

## Rules for creating libraries.

combine.always :: lib.always

.PHONY : lib.subdirs

lib.subdirs :: lib.setup
	@$(foreach dir,$(LIBDIRS), \
	 $(MAKE) -C $(dir) lib || $(FAILACTION);)

lib.always :: $(MK)/lib$(LIBEXT)

$(addsuffix /$(MK)/lib.standin,$(LIBDIRS)) : %/lib.standin :

$(addsuffix /$(MK)/lib$(LIBEXT),$(LIBDIRS)) : %/lib$(LIBEXT) : %/lib.standin
	$(MAKE) -C $(patsubst %/,%,$(dir $(patsubst %/,%,$(dir $@)))) lib

$(MK)/lib.standin :: $(addsuffix /$(MK)/lib$(LIBEXT),$(LIBDIRS))
	touch $@

$(MK)/lib$(LIBEXT) :: $(MK)/lib.standin
ifeq "$(_makeit_sys_restricted_command_length)" "YES"
ifeq "$(OBJEXT)" ".o"
	ls | sed -n -e '/^.*\.o$$/p' | xargs $(RM)
else
	$(RM) *$(OBJEXT)
endif
else
	$(RM) *$(OBJEXT)
endif
	@for lib in $(addsuffix /$(MK)/lib$(LIBEXT),$(LIBDIRS)); \
	do \
	  if test -f $${lib}; \
	  then \
	    echo $(RM) $(SYMDEFS); \
	    $(RM) $(SYMDEFS); \
	    echo "$(RUN-LIB_EXTRACT.combine)"; \
	    $(RUN-LIB_EXTRACT.combine); \
	  fi; \
	done
ifeq "$(_combine_cfront_closure)" "YES"
ifneq "$(PTDEFMAP)" ""
	cp $(PTDEFMAP) $(MKPTR)/$(_combine_defmap)
else
	@if test ! -s $(MKPTR)/$(_combine_defmap); \
	then \
	  echo "@dec ____DUMMY____" > $(MKPTR)/$(_combine_defmap); \
	  echo "<____DUMMY____>" >> $(MKPTR)/$(_combine_defmap); \
	fi
endif
ifneq "$(PTMAPS)" ""
	$(RM) $(MKPTR)/nmap*
	@NUM=1; \
	for i in $(PTMAPS); \
	do \
	  echo "cp $$i $(MKPTR)/nmap$$NUM"; \
	  cp $$i $(MKPTR)/nmap$$NUM; \
	  NUM=`expr $$NUM + 1`; \
	done
endif
	$(RM) $(MK)/__ptdummy*.cc
ifneq "$(PTDUMMYS)" ""
	@NUM=1; \
	for i in $(PTDUMMYS); \
	do \
	  echo "cp $$i $(MK)/__ptdummy$$NUM.cc"; \
	  cp $$i $(MK)/__ptdummy$$NUM.cc; \
	  NUM=`expr $$NUM + 1`; \
	done
endif
	echo "main() { return 0; }" > $(MK)/__ptdummy0.cc
ifneq "$(filter ignore_undefined_symbols,$(CLOSURE_OPTIONS))" ""
	-$(RUN-LIB_CLOSURE.combine)
else
	$(RUN-LIB_CLOSURE.combine)
endif
	$(RM) a.out __ptdummy*$(OBJEXT)
ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifneq "$(PTDEFMAP)" ""
	cp $(PTDEFMAP) $(MKPTR).pic/$(_combine_defmap)
else
	@if test ! -s $(MKPTR).pic/$(_combine_defmap); \
	then \
	  echo "@dec ____DUMMY____" > $(MKPTR).pic/$(_combine_defmap); \
	  echo "<____DUMMY____>" >> $(MKPTR).pic/$(_combine_defmap); \
	fi
endif
ifneq "$(PTMAPS)" ""
	$(RM) $(MKPTR).pic/nmap*
	@NUM=1; \
	for i in $(PTMAPS); \
	do \
	  echo "cp $$i $(MKPTR).pic/nmap$$NUM"; \
	  cp $$i $(MKPTR).pic/nmap$$NUM; \
	  NUM=`expr $$NUM + 1`; \
	done
endif
ifneq "$(filter ignore_undefined_symbols,$(CLOSURE_OPTIONS))" ""
	-$(RUN-PIC_CLOSURE.combine)
else
	$(RUN-PIC_CLOSURE.combine)
endif
	$(RM) a.out __ptdummy*$(OBJEXT)
endif
	@NUM=1; \
	for i in $(MKPTR)/*$(OBJEXT); \
	do \
	  if test -f $$i; \
	  then \
	    echo "ln $$i $(CLOSURE_PREFIX)$$NUM$(OBJEXT)"; \
	    ln $$i $(CLOSURE_PREFIX)$$NUM$(OBJEXT); \
	    NUM=`expr $$NUM + 1`; \
	  fi; \
	done
	$(RM) $(MK)/__ptdummy*.cc
endif
	$(RM) $@ __0*;
	$(RUN-LIB_ARCHIVE.combine)
ifeq "$(_makeit_sys_restricted_command_length)" "YES"
ifeq "$(OBJEXT)" ".o"
	ls | sed -n -e '/^.*\.o$$/p' | xargs $(RM)
else
	$(RM) *$(OBJEXT)
endif
else
	$(RM) *$(OBJEXT)
endif
	$(RMSYMS)
	$(RANLIB) $@

ifeq "$(_makeit_enable_shared_libraries)" "YES"

combine.always :: shlib.always

.PHONY : shlib.subdirs

shlib.subdirs :: shlib.setup
	@$(foreach dir,$(LIBDIRS), \
	 $(MAKE) -C $(dir) shlib || $(FAILACTION);)

#shlib.always :: $(MK)/pic$(LIBEXT)
shlib.always :: $(MK)/$(SHLPRE)$(LIBRARY)$(SHLEXT)

$(addsuffix /$(MK)/pic.standin,$(LIBDIRS)) : %/pic.standin :

$(addsuffix /$(MK)/pic$(LIBEXT),$(LIBDIRS)) : %/pic$(LIBEXT) : %/pic.standin
	$(MAKE) -C $(patsubst %/,%,$(dir $(patsubst %/,%,$(dir $@)))) shlib

$(MK)/pic.standin :: $(addsuffix /$(MK)/pic$(LIBEXT),$(LIBDIRS))
	touch $@

#$(MK)/pic$(LIBEXT) :: $(MK)/pic.standin
$(MK)/$(SHLPRE)$(LIBRARY)$(SHLEXT) :: $(MK)/pic.standin
ifeq "$(_makeit_sys_restricted_command_length)" "YES"
ifeq "$(OBJEXT)" ".o"
	ls | sed -n -e '/^.*\.o$$/p' | xargs $(RM)
else
	$(RM) *$(OBJEXT)
endif
else
	$(RM) *$(OBJEXT)
endif
ifneq "$(STLEXT)" ""
	$(RM) $(MK)/.static
	@for lib in $(addsuffix /$(MK)/lib$(STLEXT),$(LIBDIRS)); \
	do \
	  if test -f $${lib}; \
	  then \
	    echo $(RM) $(SYMDEFS); \
	    $(RM) $(SYMDEFS); \
	    echo "$(RUN-LIB_EXTRACT.combine)"; \
	    $(RUN-LIB_EXTRACT.combine); \
	    touch $(MK)/.static; \
	  fi; \
	done
	@if test -f "$(MK)/.static"; \
	then \
	  $(RM) $@; \
	  echo "$(RUN-STL_ARCHIVE.combine)"; \
	  $(RUN-STL_ARCHIVE.combine); \
	  echo $(RANLIB) $@; \
	  $(RANLIB) $@; \
	  echo "$(RM) $(SYMDEFS) *$(OBJEXT)"; \
	  $(RM) $(SYMDEFS) *$(OBJEXT) $(MK)/.static; \
	fi
endif
	@for lib in $(addsuffix /$(MK)/pic$(LIBEXT),$(LIBDIRS)); \
	do \
	  if test -f $${lib}; \
	  then \
	    echo $(RM) $(SYMDEFS); \
	    $(RM) $(SYMDEFS); \
	    echo "$(RUN-PIC_EXTRACT.combine)"; \
	    $(RUN-PIC_EXTRACT.combine); \
	  fi; \
	done
ifeq "$(_combine_cfront_closure)" "YES"
	@NUM=1; \
	for i in $(MKPTR).pic/*$(OBJEXT); \
	do \
	  if test -f $$i; \
	  then \
	    echo "ln $$i $(CLOSURE_PREFIX)$$NUM$(OBJEXT)"; \
	    ln $$i $(CLOSURE_PREFIX)$$NUM$(OBJEXT); \
	    NUM=`expr $$NUM + 1`; \
	  fi; \
	done
endif
ifeq "$(filter suppress_library_generation,$(SHLIB_OPTIONS))" ""
	$(RUN-PIC_LIBRARY.combine)
ifneq "$(ENV_SYSTEM)" "WIN32"
	$(RM) $(MK)/lib$(SHLEXT)
	ln -s $(SHLPRE)$(LIBNAME.combine)$(SHLEXT) $(MK)/lib$(SHLEXT)
endif
endif
	$(RM) $(MK)/pic$(LIBEXT)
	$(RUN-PIC_ARCHIVE.combine)
ifeq "$(_makeit_sys_restricted_command_length)" "YES"
ifeq "$(OBJEXT)" ".o"
	ls | sed -n -e '/^.*\.o$$/p' | xargs $(RM)
else
	$(RM) *$(OBJEXT)
endif
else
	$(RM) *$(OBJEXT)
endif
	$(RMSYMS)
endif

## Make template repository for shared library objects. Normal repository
## would already have been created elsewhere.

ifeq "$(_combine_cfront_closure)" "YES"
ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifeq "$(_makeit_cxx_supports_cfront_repositories)" "YES"

makeit.setup :: $(MKPTR).pic

$(MKPTR).pic :
	mkdir $@

clean.always ::
	$(RM) $(MKPTR).pic/*
	- rmdir $(MKPTR).pic

endif
endif
endif

endif

endif
