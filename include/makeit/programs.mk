## Copyright 1992-1993 OTC LIMITED
## Copyright 1995-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

% : %$(OBJEXT)

## Make sure we link in ObjectStore library if necessary.

ifeq "$(filter no_objectstore_libraries,$(MAKEIT_OPTIONS))" ""

override OS_THREADLIB :=

ifneq "$(ENV_SYSTEM)" "WIN32"
ifneq "$(wildcard $(OS_ROOTDIR)/lib/libosthr*)" ""
override OS_THREADLIB := -losthr
else
override OS_THREADLIB := -losths
endif
endif

ifeq "$(ENV_OODBMS)" "OSTORE"

ifeq "$(ENV_SYSTEM)" "WIN32"

override INTERNAL_LDLIBS += ostore.lib
override NO_LIBRARY_DEPENDENCY += $(notdir $(wildcard $(OS_ROOTDIR)/lib/*.lib))

else

override INTERNAL_LDLIBS += -los $(OS_THREADLIB)

endif

endif

endif

## Setup preprocessor, compiler and linker flags.

# Values specific to variant.

#override CPPFLAGS += $($(C++COMPILER)_CPPFLAGS_$(VARIANT))
#override C++FLAGS += $($(C++COMPILER)_C++FLAGS_$(VARIANT))
#override CFLAGS += $($(C++COMPILER)_CFLAGS_$(VARIANT))
override LDFLAGS += $($(C++COMPILER)_LDFLAGS_$(VARIANT))
override LDLIBS += $($(C++COMPILER)_LDLIBS_$(VARIANT))

#override CPPFLAGS += $(CPPFLAGS_$(VARIANT))
#override C++FLAGS += $(C++FLAGS_$(VARIANT))
#override CFLAGS += $(CFLAGS_$(VARIANT))
override LDFLAGS += $(LDFLAGS_$(VARIANT))
override LDLIBS += $(LDLIBS_$(VARIANT))

# Values added internally to get compilers
# or other products to work properly.

override PIC_CPPFLAGS += $(INTERNAL_PIC_CPPFLAGS)
override PIC_C++FLAGS += $(INTERNAL_PIC_C++FLAGS)
override PIC_CFLAGS += $(INTERNAL_PIC_CFLAGS)
override PIC_LDFLAGS += $(INTERNAL_PIC_LDFLAGS)
override PIC_LDLIBS += $(INTERNAL_PIC_LDLIBS)

override CPPFLAGS += $(INTERNAL_CPPFLAGS)
override C++FLAGS += $(INTERNAL_C++FLAGS)
override CFLAGS += $(INTERNAL_CFLAGS)
override LDFLAGS += $(INTERNAL_LDFLAGS)
override LDLIBS += $(INTERNAL_LDLIBS)

# Values specific to system.

override CPPFLAGS += $(SYSTEM_CPPFLAGS)
override C++FLAGS += $(SYSTEM_C++FLAGS)
override CFLAGS += $(SYSTEM_CFLAGS)
override LDFLAGS += $(SYSTEM_LDFLAGS)
override LDLIBS += $(SYSTEM_LDLIBS)

# Values specific to compiler.

override CPPFLAGS += $($(C++COMPILER)_CPPFLAGS)
override C++FLAGS += $($(C++COMPILER)_C++FLAGS)
override CFLAGS += $($(C++COMPILER)_CFLAGS)
override LDFLAGS += $($(C++COMPILER)_LDFLAGS)
override LDLIBS += $($(C++COMPILER)_LDLIBS)

## Standard targets.

.PHONY : programs
.PHONY : programs.setup
.PHONY : programs.subdirs
.PHONY : programs.target
.PHONY : programs.always

programs : programs.setup
# programs : programs.subdirs
programs : programs.target
programs : programs.always

programs.setup :: makeit.setup

# programs.subdirs :: programs.setup
# ifneq "$(SUBDIRS)" ""
# ifneq "$(filter navigate_all_variants,$(MAKEIT_OPTIONS))" ""
# 	@$(foreach var,$(VARIANTS),$(foreach dir,$(SUBDIRS), \
# 	 $(MAKE) -C $(dir) VARIANT=$(var) programs || $(FAILACTION);))
# else
# 	@$(foreach dir,$(SUBDIRS), \
# 	 $(MAKE) -C $(dir) programs || $(FAILACTION);)
# endif
# endif

programs.target :: programs.setup

programs.always :: programs.setup

## Add binaries and scripts to general class of executables.

_makeit_MK_SCRIPTS := $(strip $(_makeit_MK_SCRIPTS))
_makeit_MK_BINARIES := $(strip $(_makeit_MK_BINARIES))

_makeit_MK_EXECUTABLES := \
 $(strip $(_makeit_MK_SCRIPTS) $(_makeit_MK_BINARIES))

## Setup targets so that programs will be built when the targets
## "all" and "programs" are used.

all.always :: programs

programs.always :: $(_makeit_MK_EXECUTABLES)

## Setup targets so that user doesn't have to give name of program
## within makeit subdirectory in order to build it.

_makeit_EXECUTABLES := \
 $(patsubst %$(BINEXT),%,$(notdir $(_makeit_MK_EXECUTABLES)))

ifneq "$(_makeit_EXECUTABLES)" ""

$(_makeit_EXECUTABLES) : % : makeit.setup

$(_makeit_EXECUTABLES) : % : $(MK)/%$(BINEXT)

endif

## Stuff for memcheck program binaries only. Setup targets so that
## user doesn't have to give name of program within makeit
## subdirectory in order to build it.

ifeq "$(BINEXT)" ""
ifneq "$(MEMEXT)" ""

ifneq "$(_makeit_MK_BINARIES)" ""

ifeq "$(MEMEXT)" "pure"
.PHONY : $(addsuffix .$(MEMEXT),$(_makeit_MK_BINARIES))
.PHONY : $(addsuffix .$(MEMEXT),$(_makeit_EXECUTABLES))
endif

programs.$(MEMEXT) :: makeit.setup

programs.$(MEMEXT) :: $(addsuffix .$(MEMEXT),$(_makeit_MK_BINARIES))

$(patsubst $(MK)/%,%.$(MEMEXT),$(_makeit_MK_BINARIES)) : \
 %.$(MEMEXT) : makeit.setup

$(patsubst $(MK)/%,%.$(MEMEXT),$(_makeit_MK_BINARIES)) : \
 %.$(MEMEXT) : $(MK)/%.$(MEMEXT)

endif

endif
endif

## Setup dependencies for binaries on library in this directory
## if a library exists and if binaries require library.

ifneq "$(_makeit_MK_LIB_OBJECTS)" ""

_makeit_MK_LIB_BINARIES := \
 $(filter-out $(patsubst %,$(MK)/%$(BINEXT),$(NOLIB) $(NONLIBPROGRAMS)), \
 $(_makeit_MK_BINARIES))

ifneq "$(_makeit_MK_LIB_BINARIES)" ""

$(_makeit_MK_LIB_BINARIES) : % : $(MK)/lib$(LIBEXT)

ifeq "$(BINEXT)" ""
ifneq "$(MEMEXT)" ""
$(addsuffix .$(MEMEXT),$(_makeit_MK_LIB_BINARIES)) : % : $(MK)/lib$(LIBEXT)
endif
endif

endif

endif

## Setup dependencies for binaries on libraries and objects coming
## from other locations and non liibrary objects coming from this
## directory.

ifeq "$(filter no_library_dependencies,$(MAKEIT_OPTIONS))" ""

ifneq "$(_makeit_MK_BINARIES)" ""

ifneq "$(filter MS%,$(C++COMPILER))" ""
override NO_OBJECT_DEPENDENCY += \
 $(notdir $(wildcard $(patsubst %/BIN/,%/LIB/*.OBJ,$(dir $(C++)))))
override NO_LIBRARY_DEPENDENCY += \
 $(notdir $(wildcard $(patsubst %/BIN/,%/LIB/*.LIB,$(dir $(C++)))))
override NO_LIBRARY_DEPENDENCY += \
 $(notdir $(wildcard $(OSE_TARGET_HOME)/lib/$(C++COMPILERTAG)/*.lib))
endif

$(_makeit_MK_BINARIES) \
 : % : $(filter-out $(NO_OBJECT_DEPENDENCY),$(filter %$(OBJEXT),$(LDLIBS)))

$(_makeit_MK_BINARIES) \
 : % : $(filter-out $(NO_LIBRARY_DEPENDENCY),$(filter %$(LIBEXT),$(LDLIBS)))

ifeq "$(BINEXT)" ""
ifneq "$(MEMEXT)" ""

$(addsuffix .$(MEMEXT),$(_makeit_MK_BINARIES)) \
 : % : $(filter-out $(NO_OBJECT_DEPENDENCY),$(filter %$(OBJEXT),$(LDLIBS)))

$(addsuffix .$(MEMEXT),$(_makeit_MK_BINARIES)) \
 : % : $(filter-out $(NO_LIBRARY_DEPENDENCY),$(filter %$(LIBEXT),$(LDLIBS)))

endif
endif

endif

endif

## Drag in cfront template map files.

ifeq "$(_makeit_cxx_supports_cfront_repositories)" "YES"
ifneq "$(PTMAPS)" ""

ifneq "$(_makeit_MK_BINARIES)" ""
$(_makeit_MK_BINARIES) : $(MKPTR)/nmap0
endif

$(MKPTR)/nmap0 : $(PTMAPS)
	$(RM) $(MKPTR)/nmap*
	@NUM=1; \
	for i in $^; \
	do \
	  echo "cp $$i $(MKPTR)/nmap$$NUM"; \
	  cp $$i $(MKPTR)/nmap$$NUM; \
	  NUM=`expr $$NUM + 1`; \
	done
	$(RM) $@
	echo "@dec MAKEITDUMMY" >> $@
	echo "<MAKEITDUMMY>" >> $@

endif
endif

## Drag in Sun C++ 4.0 template options file.

ifeq "$(_makeit_cxx_supports_cafe_repositories)" "YES"
ifneq "$(PTOPTIONS)" ""

makeit.setup :: $(MKPTR)/Templates.DB/Template.opt

$(MKPTR)/Templates.DB/Template.opt : $(PTOPTIONS)
	cp $(PTOPTIONS) $@

ifeq "$(_makeit_enable_shared_libraries)" "YES"

makeit.setup :: $(MKPTR).pic/Templates.DB/Template.opt

$(MKPTR).pic/Templates.DB/Template.opt : $(PTOPTIONS)
	cp $(PTOPTIONS) $@

endif

endif
endif

## Special stuff for Versant.

ifeq "$(ENV_OODBMS)" "VERSANT"
ifneq "$(IMPFILES)" ""

#$(patsubst %.imp,$(MK)/%.cxx,$(IMPFILES)) : $(MK)/%.cxx : %.imp
$(MK)/%.cxx $(MK)/%.sch : %.imp
ifneq "$(filter run_schcomp_in_mk,$(VERSANT_OPTIONS))" ""
	cd $(MK); VERSANT_CPP="$($(C++COMPILER)_C++) -E" \
	 $(VPP_RELEASE)/bin/schcomp $(CPPFLAGS) ../$<
else
	VERSANT_CPP="$($(C++COMPILER)_C++) -E" \
	 $(VPP_RELEASE)/bin/schcomp $(CPPFLAGS) $<
	mv $(@F) $@
endif

ifneq "$(_makeit_MK_BINARIES)" ""
$(_makeit_MK_BINARIES) $(addsuffix .$(MEMEXT),$(_makeit_MK_BINARIES)) : \
 $(patsubst %.imp,$(MK)/%$(OBJEXT),$(IMPFILES))
endif

endif
endif

## Special stuff for ObjectStore

ifeq "$(ENV_OODBMS)" "OSTORE"

ifneq "$(SCHEMAS)" ""

ifneq "$(filter MS%,$(C++COMPILER))" ""

OS_LOAD_APPLICATION_SCHEMA = $(OS_CMDENV) $(OS_ROOTDIR)/bin/ossg \
 $(OS_SCHEMAFLAGS) $(CPPFLAGS) $(filter -D% -I%,$(C++FLAGS)) \
 -DSCHEMA_GENERATION -asof $(MK)/osschema$(OBJEXT) -asdb \
 $(OS_APPLICATION_SCHEMA_DB_PATH) $(OS_COMPILATION_SCHEMA_DB_PATH) \
 $(LIBSCHEMAS)

$(MK)/osschema$(OBJEXT) : \
 $(OS_COMPILATION_SCHEMA_DB_PATH) $(MK)/osschema.standin
	$(OS_LOAD_APPLICATION_SCHEMA)

else

OS_LOAD_APPLICATION_SCHEMA = $(OS_CMDENV) $(OS_ROOTDIR)/bin/ossg \
 $(OS_SCHEMAFLAGS) $(CPPFLAGS) $(filter -D% -I%,$(C++FLAGS)) \
 -DSCHEMA_GENERATION -assf $(MK)/osschema.cc -asdb \
 $(OS_APPLICATION_SCHEMA_DB_PATH) $(OS_COMPILATION_SCHEMA_DB_PATH) \
 $(LIBSCHEMAS)

$(MK)/osschema.cc : \
 $(OS_COMPILATION_SCHEMA_DB_PATH) $(MK)/osschema.standin
	$(OS_LOAD_APPLICATION_SCHEMA)

endif

ifneq "$(_makeit_MK_BINARIES)" ""

$(_makeit_MK_BINARIES) : $(MK)/osschema$(OBJEXT)

ifeq "$(BINEXT)" ""
ifneq "$(MEMEXT)" ""
$(addsuffix .$(MEMEXT),$(_makeit_MK_BINARIES)) : $(MK)/osschema$(OBJEXT)
endif
endif

endif

endif

endif
