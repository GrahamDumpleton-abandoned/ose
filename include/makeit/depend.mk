## Copyright 1992-1993 OTC LIMITED
## Copyright 1995-2000 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Standard targets.

.PHONY : depend
.PHONY : depend.setup
.PHONY : depend.subdirs
.PHONY : depend.target
.PHONY : depend.always

depend : depend.setup
depend : depend.subdirs
depend : depend.target
depend : depend.always

depend.setup :: makeit.setup

depend.subdirs :: depend.setup
ifneq "$(SUBDIRS)" ""
	@$(foreach dir,$(SUBDIRS), \
	 $(MAKE) -C $(dir) depend || $(FAILACTION);)
endif

depend.target :: depend.setup

depend.always :: depend.setup

## Default name for dependency file.

override DEPFILE := $(strip $(DEPFILE))

ifeq "$(DEPFILE)" ""
override DEPFILE := DEPS.mk
endif

## Default dependency filter. Eliminates any files not included from
## relative include directory.

override DEPFILTER := $(strip $(DEPFILTER))

ifeq "$(DEPFILTER)" ""
override DEPFILTER := sed -e "/ \/.*/d" -e "/ [a-zA-Z]:\/.*/d"
endif

## Don't bother with whats left unless we have to. Note that DEPFILE
## should always be used as written. It should not be placed in SRCDIR.

ifneq "$(_depend_SRC)" ""

ifeq "$(AUTODEPEND)" "YES"
#depend.always :: $(DEPFILE)

$(DEPFILE) :: depend.setup
$(DEPFILE) :: depend.target
$(DEPFILE) :: depend.always
depend.always :: $(_depend_SRC)
#$(DEPFILE) :: makeit.setup $(_depend_SRC)
else
depend.always :: $(_depend_SRC)
endif
	@$(RM) $(DEPFILE)
	@echo makeit: updating $(DEPFILE)
	@for i in $(_depend_SRC); \
	do \
	  cat $$i | $(DEPFILTER) | sed -e 's%$(MK)/%$$(MK)/%' | \
	   sed -e 's%$(OSE_TARGET)%$$(OSE_TARGET)%' >> $(DEPFILE); \
	done

## Clean targets.

mostlyclean.target ::
	$(RM) $(_depend_SRC)

realclean.always ::
	$(RM) $(DEPFILE)

endif

## Include dependencies if they exist. Always check current directory
## before SRCDIR for DEPFILE. Note that first check will pick up a
## pathname which is absolute.

ifneq "$(NODEPFILE)" "YES"
ifneq "$(wildcard $(DEPFILE))" ""
include $(DEPFILE)
else
ifneq "$(SRCDIR)" "."
ifneq "$(filter $(DEPFILE),$(SRCFILES))" ""
include $(SRCDIR)/$(DEPFILE)
endif
endif
endif
endif
