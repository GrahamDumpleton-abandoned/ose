## Copyright 1992-1993 OTC LIMITED
## Copyright 1995-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

(%) : %

## Standard targets.

.PHONY : lib
.PHONY : lib.setup
# .PHONY : lib.subdirs
.PHONY : lib.target
.PHONY : lib.always

lib : lib.setup
# lib : lib.subdirs
lib : lib.target
lib : lib.always

lib.setup :: makeit.setup

# lib.subdirs :: lib.setup

lib.target :: lib.setup

lib.always :: lib.setup

.PHONY : shlib
.PHONY : shlib.setup
# .PHONY : shlib.subdirs
.PHONY : shlib.target
.PHONY : shlib.always

shlib : shlib.setup
# shlib : shlib.subdirs
shlib : shlib.target
shlib : shlib.always

shlib.setup :: makeit.setup

# shlib.subdirs :: shlib.setup

shlib.target :: shlib.setup

shlib.always :: shlib.setup

## Only bother with most of this file if we have a library to build.

_makeit_MK_LIB_OBJECTS := $(strip $(_makeit_MK_LIB_OBJECTS))

ifneq "$(_makeit_MK_LIB_OBJECTS)" ""

## Setup target so that library is built when the targets "all" or "lib"
## are used.

all.always :: lib

lib.always :: $(MK)/lib$(LIBEXT)

## Setup library archive targets.

PTRFLAGS.lib = -ptr$(MKPTR)

ifeq "$(filter ignore_repositories,$(MAKEIT_OPTIONS))" ""
ifneq "$(PTRDIRS)" ""
PTRFLAGS.lib += $(addprefix -ptr,$(PTRDIRS))
endif
endif

PIC_PTRFLAGS.lib = -ptr$(MKPTR).pic

ifeq "$(filter ignore_repositories,$(MAKEIT_OPTIONS))" ""
ifneq "$(PTRDIRS)" ""
ifeq "$(_makeit_cxx_supports_cafe_repositories)" "YES"
PIC_PTRFLAGS.lib += $(patsubst %,-ptr%.pic,$(PTRDIRS))
else
PIC_PTRFLAGS.lib += $(addprefix -ptr,$(PTRDIRS))
endif
endif
endif

ifeq "$(origin CLOSURE_LDFLAGS)" "undefined"
override CLOSURE_LDFLAGS = $(LDFLAGS)
endif

ifeq "$(origin CLOSURE_LDLIBS)" "undefined"
override CLOSURE_LDLIBS = $(LDLIBS)
endif

LDFLAGS.lib = $(filter $(LOPT)%,$(LDFLAGS))
LDLIBS.lib = $(filter -l%,$(LDLIBS))

ifneq "$(filter loadable_module,$(SHLIB_OPTIONS))" ""
LIBNAME.lib = $(firstword $(OBJECT) NONAME)
else
LIBNAME.lib = $(firstword $(LIBRARY) NONAME)
endif

ifeq "$(origin IMLEXT)" "undefined"
IMPLIB.lib =
else
IMPLIB.lib = $(MK)/$(IMLPRE)$(LIBNAME.lib)$(IMLEXT)
endif

LIB_CLOSURE.lib = \
 $(subst @LDFLAGS@,$(filter $(LOPT)%,$(CLOSURE_LDFLAGS)), \
 $(subst @LDLIBS@,$(filter %$(LIBEXT) -l%,$(CLOSURE_LDLIBS)), \
 $(subst @OBJECTS@,$?, \
 $(subst @ALLOBJECTS@,$^, \
 $(subst @PTRFLAGS@,$(PTRFLAGS.lib), \
 $($(C++COMPILER)_LIB_CLOSURE))))))

RUN-LIB_CLOSURE.lib = \
 $(LIB_CLOSURE.lib)

LIB_ARCHIVE.lib = \
 $(subst @LIB@,$@, \
 $(subst @IMPLIB@,$(IMPLIB.lib), \
 $(subst @OBJECTS@,$?, \
 $(subst @ALLOBJECTS@,$^, \
 $(subst @PTRFLAGS@,$(PTRFLAGS.lib), \
 $($(C++COMPILER)_LIB_ARCHIVE))))))

RUN-LIB_ARCHIVE.lib = \
 $(LIB_ARCHIVE.lib); \
 $(RANLIB) $@

$(MK)/lib$(LIBEXT) :: \
 $(_makeit_MK_LIB_OBJECTS)
	-$(RUN-LIB_CLOSURE.lib)
	$(RUN-LIB_ARCHIVE.lib)

ifeq "$(_makeit_enable_shared_libraries)" "YES"

all.always :: shlib

LIBPATH.lib =

PIC_CLOSURE.lib = \
 $(subst @LDFLAGS@,$(filter $(LOPT)%,$(CLOSURE_LDFLAGS)), \
 $(subst @LDLIBS@,$(filter %$(LIBEXT) -l%,$(CLOSURE_LDLIBS)), \
 $(subst @OBJECTS@,$?, \
 $(subst @ALLOBJECTS@,$^, \
 $(subst @PIC_PTRFLAGS@,$(PIC_PTRFLAGS.lib), \
 $($(C++COMPILER)_PIC_CLOSURE))))))

RUN-PIC_CLOSURE.lib = \
 $(PIC_CLOSURE.lib)

PIC_ARCHIVE.lib = \
 $(subst @LIB@,$(MK)/pic$(LIBEXT), \
 $(subst @LIBNAME@,$(LIBNAME.lib), \
 $(subst @OBJECTS@,$?, \
 $(subst @ALLOBJECTS@,$^, \
 $(subst @PIC_PTRFLAGS@,$(PIC_PTRFLAGS.lib), \
 $($(C++COMPILER)_PIC_ARCHIVE))))))

RUN-PIC_ARCHIVE.lib = \
 $(PIC_ARCHIVE.lib); \
 $(RANLIB) $@

ifneq "$(filter loadable_module,$(SHLIB_OPTIONS))" ""
$(C++COMPILER)_PIC_TARGET = $($(C++COMPILER)_PIC_OBJECT)
else
$(C++COMPILER)_PIC_TARGET = $($(C++COMPILER)_PIC_LIBRARY)
endif

ifeq "$(_makeit_separate_pic_objects)" "YES"

PIC_LIBRARY.lib = \
 $(subst @LIB@,$(MK)/$(SHLPRE)$(LIBNAME.lib)$(SHLEXT), \
 $(subst @LIBFILE@,$(MK)/$(SHLPRE)$(LIBNAME.lib)$(SHLEXT), \
 $(subst @LIBNAME@,$(LIBNAME.lib), \
 $(subst @IMPLIB@,$(IMPLIB.lib), \
 $(subst @LDFLAGS@,$(LDFLAGS.lib), \
 $(subst @LDLIBS@,$(LDLIBS.lib), \
 $(subst @OBJECTS@,$^, \
 $(subst @PIC_PTRFLAGS@,$(PIC_PTRFLAGS.lib), \
 $(subst @LIBPATH@,${LIBPATH.lib}, \
 $($(C++COMPILER)_PIC_TARGET))))))))))

else

PIC_LIBRARY.lib = \
 $(subst @LIB@,$(MK)/$(SHLPRE)$(LIBNAME.lib)$(SHLEXT), \
 $(subst @LIBFILE@,$(SHLPRE)$(LIBNAME.lib)$(SHLEXT), \
 $(subst @LIBNAME@,$(LIBNAME.lib), \
 $(subst @IMPLIB@,$(IMPLIB.lib), \
 $(subst @LDFLAGS@,$(LDFLAGS.lib), \
 $(subst @LDLIBS@,$(LDLIBS.lib), \
 $(subst @OBJECTS@,$(_makeit_MK_LIB_OBJECTS), \
 $(subst @PIC_PTRFLAGS@,$(PIC_PTRFLAGS.lib), \
 $(subst @LIBPATH@,${LIBPATH.lib}, \
 $($(C++COMPILER)_PIC_TARGET))))))))))

endif

RUN-PIC_LIBRARY.lib = \
 $(PIC_LIBRARY.lib)

override STATICSRC := $(strip $(filter-out $(EXCLUDE),$(STATIC) $(STATICSRC)))

ifneq "$(STATICSRC)" ""
ifneq "$(STLEXT)" ""
ifeq "$(filter suppress_library_generation,$(SHLIB_OPTIONS))" ""

shlib.always :: $(MK)/$(STLPRE)$(LIBNAME.lib)$(STLEXT)

$(MK)/$(STLPRE)$(LIBNAME.lib)$(STLEXT) :: \
 $(patsubst %,$(MK)/%$(OBJEXT),$(basename $(STATICSRC)))
	-$(RUN-PIC_CLOSURE.lib)
	$(RUN-PIC_ARCHIVE.lib)

endif
endif
endif

#shlib.always :: $(MK)/pic$(LIBEXT)
ifneq "$(filter loadable_module,$(SHLIB_OPTIONS))" ""
shlib.always :: $(MK)/$(LIBNAME.lib)$(SHLEXT)
else
shlib.always :: $(MK)/$(SHLPRE)$(LIBNAME.lib)$(SHLEXT)
endif

ifneq "$(filter MS%,$(C++COMPILER))" ""

.PHONY : $(MK)/lib.cfg

makeit.setup :: $(MK)/lib.cfg

$(MK)/lib.cfg :
	@$(RM) $@
	@echo '-link' >> $@
	@echo $(filter %.dll %.DLL %.lib %.LIB %.obj %.OBJ, \
	 $(patsubst -l%,%$(LIBEXT),$(LDLIBS))) | sed -e 's%/%\\%g' >> $@

LDLIBS.lib = $(LDLIBS1.lib) $(LDLIBS2.lib)

LDFLAGS.lib = @$(MK)/lib.cfg -link $(filter-out -L%,$(LDFLAGS))

LDLIBS1.lib = \
 $(filter-out $< $(MK)/lib$(LIBEXT) $(LDLIBS) \
 $(patsubst -l%,\%/%$(LIBEXT),$(LDLIBS)), \
 $(filter %$(LIBEXT),$^)) \
 $(filter $(MK)/lib$(LIBEXT),$^)

LDLIBS2.lib = \
 $(filter-out -l% %.so %.DLL %.lib %.LIB %.obj %.OBJ,$(LDLIBS))

ifeq "$(ENV_TOOLSET)" "CYGWIN"

override LIBPATH.lib = \
 $(subst /,\,$(subst $(empty) $(empty),;,$(patsubst -L%,%, \
 $(shell echo $(filter -L%,$(LDFLAGS)) | \
 sed -e 's%-L//\([a-zA-Z]\)/%-L\1:/%g' \
 -e 's%-L/cygdrive/\([a-zA-Z]\)/%-L\1:/%g' | \
 tr '' ';')));$(lib);$(LIB))

else

override LIBPATH.lib = \
 $(subst /,\,$(subst $(empty) $(empty),;,$(patsubst -L%,%, \
 $(shell echo $(filter -L%,$(LDFLAGS)) | \
 sed -e 's%-L//\([a-zA-Z]\)/%-L\1:/%g' -e \
 's%-L/cygdrive/\([a-zA-Z]\)/%-L\1:/%g')));$(lib);$(LIB))

endif

endif

ifeq "$(_makeit_separate_pic_objects)" "YES"

#$(MK)/pic$(LIBEXT) :: \
# $(patsubst $(MK)/%$(OBJEXT),$(MK).pic/%$(OBJEXT),$(_makeit_MK_LIB_OBJECTS))
ifneq "$(filter loadable_module,$(SHLIB_OPTIONS))" ""
$(MK)/$(LIBNAME.lib)$(SHLEXT) :: \
 $(patsubst $(MK)/%$(OBJEXT),$(MK).pic/%$(OBJEXT),$(_makeit_MK_LIB_OBJECTS))
else
$(MK)/$(SHLPRE)$(LIBNAME.lib)$(SHLEXT) :: \
 $(patsubst $(MK)/%$(OBJEXT),$(MK).pic/%$(OBJEXT),$(_makeit_MK_LIB_OBJECTS))
endif
	-$(RUN-PIC_CLOSURE.lib)
	$(RUN-PIC_ARCHIVE.lib)
ifeq "$(filter suppress_library_generation,$(SHLIB_OPTIONS))" ""
	$(RUN-PIC_LIBRARY.lib) || ($(RM) $@; exit 1)
ifneq "$(ENV_SYSTEM)" "WIN32"
	$(RM) $(MK)/lib$(SHLEXT)
	ln -s $(SHLPRE)$(LIBNAME.lib)$(SHLEXT) $(MK)/lib$(SHLEXT)
endif
endif

else

#$(MK)/pic$(LIBEXT) :: $(_makeit_MK_LIB_OBJECTS)
#$(MK)/pic$(LIBEXT) :: $(MK)/lib$(LIBEXT)
ifneq "$(filter loadable_module,$(SHLIB_OPTIONS))" ""
$(MK)/$(LIBNAME.lib)$(SHLEXT) :: $(MK)/lib$(LIBEXT)
else
$(MK)/$(SHLPRE)$(LIBNAME.lib)$(SHLEXT) :: $(MK)/lib$(LIBEXT)
endif
ifeq "$(filter suppress_library_generation,$(SHLIB_OPTIONS))" ""
	$(RUN-PIC_LIBRARY.lib) || ($(RM) $@; exit 1)
ifneq "$(ENV_SYSTEM)" "WIN32"
	$(RM) $(MK)/lib$(SHLEXT)
	ln -s $(SHLPRE)$(LIBNAME.lib)$(SHLEXT) $(MK)/lib$(SHLEXT)
endif
endif
ifneq "$(ENV_SYSTEM)" "WIN32"
	$(RM) $(MK)/pic$(LIBEXT)
	ln -s lib$(LIBEXT) $(MK)/pic$(LIBEXT)
endif

endif

endif

endif

## ObjectStore schema database generation.

ifeq "$(ENV_OODBMS)" "OSTORE"

ifneq "$(SCHEMAS)" ""

# Adjust variables listing files if not in SRCDIR.

ifneq "$(SRCDIR)" "."
override SCHEMAS := $(foreach file,$(SCHEMAS),$(firstword $(wildcard \
 $(file) $(SRCDIR)/$(file)) $(file)))
endif

## Creation of schema database.

OS_LOAD_COMPILATION_SCHEMA = $(OS_CMDENV) $(OS_ROOTDIR)/bin/ossg \
 $(OS_SCHEMAFLAGS) $(CPPFLAGS) $(filter -D% -I%,$(C++FLAGS)) \
 -DSCHEMA_GENERATION -csdb $(OS_COMPILATION_SCHEMA_DB_PATH)

lib.schema : $(OS_COMPILATION_SCHEMA_DB_PATH) $(MK)/osschema.standin

$(OS_COMPILATION_SCHEMA_DB_PATH) : $(SCHEMAS)
	@for i in $(SCHEMAS); \
	do \
	  echo $(OS_LOAD_COMPILATION_SCHEMA) $$i; \
	  $(OS_LOAD_COMPILATION_SCHEMA) $$i; \
	done
	@touch $(MK)/osschema.standin

endif

endif

ifneq "$(filter OS%,$(C++COMPILER))" ""

clean.always ::
ifneq "$(OS_COMPILATION_SCHEMA_DB_PATH)" ""
	$(OS_CMDENV) $(OS_ROOTDIR)/bin/osrm -f $(OS_COMPILATION_SCHEMA_DB_PATH)
	$(RM) $(MK)/osschema.standin
endif
ifneq "$(OS_APPLICATION_SCHEMA_DB_PATH)" ""
	$(OS_CMDENV) $(OS_ROOTDIR)/bin/osrm -f $(OS_APPLICATION_SCHEMA_DB_PATH)
endif

else
ifeq "$(ENV_OODBMS)" "OSTORE"

ifeq "$(ENV_SYSTEM)" "WIN32"

clean.always ::
ifneq "$(OS_COMPILATION_SCHEMA_DB_PATH)" ""
	$(RM) $(OS_COMPILATION_SCHEMA_DB_PATH)
	$(RM) $(MK)/osschema.standin
endif
ifneq "$(OS_APPLICATION_SCHEMA_DB_PATH)" ""
	$(RM) $(OS_APPLICATION_SCHEMA_DB_PATH)
endif

else

clean.always ::
ifneq "$(OS_COMPILATION_SCHEMA_DB_PATH)" ""
	$(OS_CMDENV) $(OS_ROOTDIR)/bin/osrm -f $(OS_COMPILATION_SCHEMA_DB_PATH)
	$(RM) $(MK)/osschema.standin
endif
ifneq "$(OS_APPLICATION_SCHEMA_DB_PATH)" ""
	$(OS_CMDENV) $(OS_ROOTDIR)/bin/osrm -f $(OS_APPLICATION_SCHEMA_DB_PATH)
endif

endif

endif
endif
