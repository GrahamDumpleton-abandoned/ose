## Copyright 1993 OTC LIMITED
## Copyright 1995-2000 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## If not compiling in SRCDIR, add addition search directories so that
## directories relative to SRCDIR will also be searched. No where else
## to really stick this. It must come after modules have been included.

ifneq "$(SRCDIR)" "."

# Must use ":=" here. If we use "+=" and CPPFLAGS has "=" behaviour so
# far, will end up with a loop.

override CPPFLAGS := $(foreach opt,$(CPPFLAGS),$(opt) \
 $(patsubst -I%,-I$(SRCDIR)/%,$(filter-out -I/%, \
 $(filter -I%,$(filter-out -I$(MK),$(opt))))))

ifeq "$(filter -I.,$(CPPFLAGS))" ""
override CPPFLAGS := -I$(SRCDIR) $(CPPFLAGS)
else
ifneq "$(firstword $(filter -I%,$(CPPFLAGS)))" "-I."
override CPPFLAGS := -I. -I$(SRCDIR) $(CPPFLAGS)
endif
endif

endif

## Build everything.

.PHONY : all
.PHONY : all.setup
.PHONY : all.subdirs
.PHONY : all.target
.PHONY : all.always

all : all.setup
all : all.subdirs
all : all.target
all : all.always

all.setup :: makeit.setup

all.subdirs :: all.setup
ifneq "$(SUBDIRS)" ""
ifneq "$(filter navigate_all_variants,$(MAKEIT_OPTIONS))" ""
	@$(foreach var,$(VARIANTS),$(foreach dir,$(SUBDIRS), \
	 $(MAKE) -C $(dir) VARIANT=$(var) all || $(FAILACTION);))
else
	@$(foreach dir,$(SUBDIRS), \
	 $(MAKE) -C $(dir) all || $(FAILACTION);)
endif
endif

all.target :: all.setup

all.always :: all.setup

## Target for creating a parallel directory hierarchy for compilation.

WSMAKEFILES = makeit.mk Makeit.mk GNUmakefile makefile Makefile

WSMAKEFILE = \
 $(firstword $(notdir $(wildcard $(addprefix $(SRCDIR)/,$(WSMAKEFILES)))))

WSSUBMAKEFILE = \
 $(firstword $(notdir $(wildcard $(addprefix \
 $(SRCDIR)/$(dir)/,$(WSMAKEFILES)))) Makeit.mk)

ifeq "$(MKWSMAKEFILE)" ""
define MKWSMAKEFILE
$(RM) $(WSMAKEFILE)
echo 'SRCDIR := $(SRCDIR)' >> $(WSMAKEFILE)
echo 'include $$(SRCDIR)/$(WSMAKEFILE)' >> $(WSMAKEFILE)
endef
endif

ifeq "$(ALLDIRS)" ""
override ALLDIRS := $(SUBDIRS)
endif

.PHONY : workspace
.PHONY : workspace.setup
.PHONY : workspace.subdirs
.PHONY : workspace.target
.PHONY : workspace.always

workspace : workspace.setup
workspace : workspace.subdirs
workspace : workspace.target
workspace : workspace.always

workspace.setup :: makeit.setup
ifneq "$(ALLDIRS)" ""
	@$(foreach dir,$(ALLDIRS),test -d $(dir) || \
	 (echo "mkdir $(dir)" && mkdir $(dir)) || $(FAILACTION);)
endif

workspace.subdirs :: workspace.setup
ifneq "$(ALLDIRS)" ""
ifneq "$(filter /%,$(SRCDIR))" ""
	@$(foreach dir,$(ALLDIRS), \
	 $(MAKE) -C $(dir) SRCDIR=$(SRCDIR)/$(dir) \
	 -f $(SRCDIR)/$(dir)/$(WSSUBMAKEFILE) workspace || $(FAILACTION);)
else
	@$(foreach dir,$(ALLDIRS), \
	 $(MAKE) -C $(dir) SRCDIR=../$(SRCDIR)/$(dir) \
	 -f ../$(SRCDIR)/$(dir)/$(WSSUBMAKEFILE) workspace || $(FAILACTION);)
endif
endif

workspace.target :: workspace.setup

workspace.always :: workspace.setup
ifeq "$(wildcard $(WSMAKEFILE))" ""
	$(MKWSMAKEFILE)
endif
