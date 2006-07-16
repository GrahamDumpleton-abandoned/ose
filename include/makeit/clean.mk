## Copyright 1992-1993 OTC LIMITED
## Copyright 1994-2000 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Standard targets.

.PHONY : mostlyclean
.PHONY : mostlyclean.setup
.PHONY : mostlyclean.subdirs
.PHONY : mostlyclean.target
.PHONY : mostlyclean.always

mostlyclean : mostlyclean.setup
mostlyclean : mostlyclean.subdirs
mostlyclean : mostlyclean.target
mostlyclean : mostlyclean.always

mostlyclean.setup :: makeit.setup

mostlyclean.subdirs :: mostlyclean.setup
ifneq "$(SUBDIRS)" ""
ifneq "$(filter navigate_all_variants,$(MAKEIT_OPTIONS))" ""
	@$(foreach var,$(VARIANTS),$(foreach dir,$(SUBDIRS), \
	 $(MAKE) -C $(dir) VARIANT=$(var) mostlyclean || $(FAILACTION);))
else
	@$(foreach dir,$(SUBDIRS), \
	 $(MAKE) -C $(dir) mostlyclean || $(FAILACTION);)
endif
endif

mostlyclean.target :: mostlyclean.setup
ifneq "$(_makeit_MK_LIB_OBJECTS)" ""
	$(RM) $(_makeit_MK_LIB_OBJECTS)
ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifeq "$(_makeit_separate_pic_objects)" "YES"
	$(RM) $(MK).pic/*.o
endif
endif
ifeq "$(_makeit_cleanup_pure)" "YES"
	$(RM) $(patsubst %$(OBJEXT),%_pure_*$(OBJEXT),$(_makeit_MK_LIB_OBJECTS))
	$(RM) $(MK)/lib.pure$(LIBEXT) $(MK)/lib_pure_*$(LIBEXT)
endif
endif
ifneq "$(_makeit_MK_BINARIES)" ""
	$(RM) $(addsuffix $(OBJEXT),$(_makeit_MK_BINARIES))
ifneq "$(MEMEXT)" ""
	$(RM) $(addsuffix .$(MEMEXT),$(_makeit_MK_BINARIES))
endif
ifeq "$(_makeit_cleanup_pure)" "YES"
	$(RM) $(addsuffix _pure_*$(OBJEXT),$(_makeit_MK_BINARIES))
	$(RM) $(addsuffix .pure_*,$(_makeit_MK_BINARIES))
	$(RM) $(addsuffix .pure.pure_*,$(_makeit_MK_BINARIES))
endif
endif
ifeq "$(_makeit_cleanup_objectstore)" "YES"
	$(RM) .os_schema*.c* .os_schema*$(OBJEXT)
endif
ifeq "$(_makeit_cleanup_pure)" "YES"
	$(RM) $(MK)/.pure
	$(RM) $(MK)/.purifyversion
endif
ifeq "$(_makeit_cxx_supports_cafe_repositories)" "YES"
	$(RM) $(MKPTR)/Templates.DB/.pure
endif
ifeq "$(_makeit_cxx_requires_repository)" "YES"
	$(RM) $(MKPTR)/.pure
endif
ifeq "$(_makeit_cleanup_sentinel)" "YES"
	$(RM) $(MK)/.sentinel.tmp/.sent
	$(RM) $(MK)/.sentinel.tmp/*
	- rmdir $(MK)/.sentinel.tmp
endif

mostlyclean.always :: mostlyclean.setup
ifeq "$(_makeit_sys_restricted_command_length)" "YES"
ifeq "$(OBJEXT)" ".o"
	ls | sed -n -e '/^.*\.o$$/p' | xargs $(RM)
else
	$(RM) *$(OBJEXT)
endif
else
	$(RM) *$(OBJEXT)
endif
	$(RM) core a.out
	$(RM) mon.out gmon.out
	$(RMSYMS)
#ifeq "$(_makeit_cleanup_pure)" "YES"
#	$(RM) .pure
#endif
#ifeq "$(_makeit_cleanup_objectstore)" "YES"
#	$(RM) .os_schema*.c* .os_schema*$(OBJEXT)
#endif

.PHONY : clean
.PHONY : clean.setup
.PHONY : clean.subdirs
.PHONY : clean.target
.PHONY : clean.always

clean : clean.setup
clean : clean.subdirs
clean : clean.target
clean : clean.always

clean.setup :: mostlyclean.setup

clean.subdirs :: clean.setup
ifneq "$(SUBDIRS)" ""
ifneq "$(filter navigate_all_variants,$(MAKEIT_OPTIONS))" ""
	@$(foreach var,$(VARIANTS),$(foreach dir,$(SUBDIRS), \
	 $(MAKE) -C $(dir) VARIANT=$(var) clean || $(FAILACTION);))
else
	@$(foreach dir,$(SUBDIRS), \
	 $(MAKE) -C $(dir) clean || $(FAILACTION);)
endif
endif

clean.target :: clean.setup

clean.always :: clean.setup mostlyclean.always
ifeq "$(_makeit_sys_brute_force_clean)" "YES"
	rm -rf $(MK)
ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifeq "$(_makeit_separate_pic_objects)" "YES"
	rm -rf $(MK).pic
endif
endif
ifeq "$(_makeit_cxx_requires_repository)" "YES"
	rm -rf $(MKPTR)
endif
ifeq "$(_makeit_cxx_supports_dec_repositories)" "YES"
ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifeq "$(_makeit_separate_pic_objects)" "YES"
	rm -rf $(MKPTR).pic
endif
endif
endif
ifeq "$(_makeit_cxx_supports_cafe_repositories)" "YES"
ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifeq "$(_makeit_separate_pic_objects)" "YES"
	rm -rf $(MKPTR).pic
endif
endif
endif
else
ifeq "$(_makeit_cleanup_pure)" "YES"
	$(RM) $(MK)/.pure
	$(RM) $(MK)/.purifyversion
endif
ifeq "$(_makeit_cxx_supports_cafe_repositories)" "YES"
	$(RM) $(MKPTR)/Templates.DB/.pure
endif
ifneq "$(filter SG%,$(C++COMPILER))" ""
ifeq "$(filter SG3.%,$(C++COMPILER))" ""
	$(RM) $(MK)/ii_files/*
	$(RM) $(MK)/ILDUMPS/*
	- rmdir $(MK)/ii_files $(MK)/ILDUMPS
endif
endif
ifneq "$(filter KAI%,$(C++COMPILER))" ""
	$(RM) $(MK)/ti_files/*
	- rmdir $(MK)/ii_files
endif
ifeq "$(_makeit_cxx_requires_repository)" "YES"
	$(RM) $(MKPTR)/.pure
endif
ifeq "$(_makeit_cleanup_sentinel)" "YES"
	$(RM) $(MK)/.sentinel.tmp/.sent
	$(RM) $(MK)/.sentinel.tmp/*
	- rmdir $(MK)/.sentinel.tmp
endif
ifneq "$(filter SUN5.%,$(C++COMPILER))" ""
#	$(C++)admin -d$(MK) -clean
#	$(RM) $(MK)/SunWS_cache/*
#	- rmdir $(MK)/SunWS_cache
	rm -rf $(MK)/SunWS_cache
endif
	$(RM) $(MK)/*
	- rmdir $(MK)
ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifeq "$(_makeit_separate_pic_objects)" "YES"
	$(RM) $(MK).pic/*
	- rmdir $(MK).pic
endif
endif
ifeq "$(_makeit_cxx_supports_cafe_repositories)" "YES"
	$(RM) $(MKPTR)/Templates.DB/Module.DB/*
	- rmdir $(MKPTR)/Templates.DB/Module.DB
	$(RM) $(MKPTR)/Templates.DB/*
	- rmdir $(MKPTR)/Templates.DB
endif
ifeq "$(_makeit_cxx_requires_repository)" "YES"
	$(RM) $(MKPTR)/*
	- rmdir $(MKPTR)
endif
ifeq "$(_makeit_cxx_supports_cafe_repositories)" "YES"
ifeq "$(_makeit_enable_shared_libraries)" "YES"
ifeq "$(_makeit_separate_pic_objects)" "YES"
	$(RM) $(MKPTR).pic/Templates.DB/Module.DB/*
	- rmdir $(MKPTR).pic/Templates.DB/Module.DB
	$(RM) $(MKPTR).pic/Templates.DB/*
	- rmdir $(MKPTR).pic/Templates.DB
	$(RM) $(MKPTR).pic/*
	- rmdir $(MKPTR).pic
endif
endif
endif
endif
ifneq "$(filter LC%,$(C++COMPILER))" ""
	$(RM) lcc_hrepository/*
	- rmdir lcc_hrepository
endif
ifneq "$(filter SG%,$(C++COMPILER))" ""
ifeq "$(filter SG3.%,$(C++COMPILER))" ""
	$(RM) ii_files/*
	$(RM) ILDUMPS/*
	- rmdir ii_files ILDUMPS
endif
endif
ifneq "$(filter KAI%,$(C++COMPILER))" ""
	$(RM) ti_files/*
	- rmdir ti_files
endif
ifeq "$(_makeit_cleanup_testcenter)" "YES"
ifeq "$(_makeit_sys_brute_force_clean)" "YES"
	rm -rf TC.Cache
else
	$(RM) TC.Cache/*
	- rmdir TC.Cache
endif
endif
ifneq "$(filter KAI% NOV%,$(C++COMPILER))" ""
	$(RM) *.ii
endif
#ifneq "$(filter SUN5.%,$(C++COMPILER))" ""
#	rm -rf SunWS_cache
#endif

.PHONY : distclean
.PHONY : distclean.setup
.PHONY : distclean.subdirs
.PHONY : distclean.target
.PHONY : distclean.always

distclean : distclean.setup
distclean : distclean.subdirs
distclean : distclean.target
distclean : distclean.always

distclean.setup :: clean.setup

distclean.subdirs :: distclean.setup
ifneq "$(SUBDIRS)" ""
ifneq "$(filter navigate_all_variants,$(MAKEIT_OPTIONS))" ""
	@$(foreach var,$(VARIANTS),$(foreach dir,$(SUBDIRS), \
	 $(MAKE) -C $(dir) VARIANT=$(var) distclean || $(FAILACTION);))
else
	@$(foreach dir,$(SUBDIRS), \
	 $(MAKE) -C $(dir) distclean || $(FAILACTION);)
endif
endif

distclean.target :: distclean.setup

distclean.always :: distclean.setup clean.always

.PHONY : realclean
.PHONY : realclean.setup
.PHONY : realclean.subdirs
.PHONY : realclean.target
.PHONY : realclean.always

realclean : realclean.setup
realclean : realclean.subdirs
realclean : realclean.target
realclean : realclean.always

realclean.setup :: distclean.setup

realclean.subdirs :: realclean.setup
ifneq "$(SUBDIRS)" ""
ifneq "$(filter navigate_all_variants,$(MAKEIT_OPTIONS))" ""
	@$(foreach var,$(VARIANTS),$(foreach dir,$(SUBDIRS), \
	 $(MAKE) -C $(dir) VARIANT=$(var) realclean || $(FAILACTION);))
else
	@$(foreach dir,$(SUBDIRS), \
	 $(MAKE) -C $(dir) realclean || $(FAILACTION);)
endif
endif

realclean.target :: realclean.setup

realclean.always :: realclean.setup distclean.always
