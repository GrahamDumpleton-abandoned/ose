## Copyright 1992-1993 OTC LIMITED
## Copyright 1995-1996 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Work out which modules we can find to include.

_makeit_modules := \
 $(foreach module,$(filter $(MODULES),$(ALLMODULES)),\
 $(firstword $(wildcard $(addsuffix /$(module).mk,$(MODULEPATH))) \
 missing/module/$(module))) $(addprefix invalid/module/,\
 $(filter-out $(ALLMODULES),$(MODULES)))

# Include makeit preamble.

include $(OSE_MAKEIT)/start.mk

# Include modules.

include $(_makeit_modules)

# Include makeit postamble.

include $(OSE_MAKEIT)/end.mk

# Include "rules" part of any module files.

ifneq "$(_makeit_modules_rules)" ""
include $(_makeit_modules_rules)
endif

## Print diagnostics about modules which were required but not included.

_makeit_missing_modules := $(sort $(filter-out $(MODULES),$(REQUIRE)))

ifneq "$(_makeit_missing_modules)" ""

makeit.setup ::
	@echo "The following modules were required but not included:"
	@echo
	@echo "  $(_makeit_missing_modules)"
	@echo
	@echo "Add them to the MODULES variable in your makefile."
	@echo "The results of the current build are undefined."
	@echo
	exit 1

# override _makeit_dummy := $(shell echo 1>&2)
# override _makeit_dummy := \
#  $(shell echo "The following modules were required but not included:" 1>&2)
# override _makeit_dummy := $(shell echo 1>&2)
# override _makeit_dummy := $(shell echo "  $(_makeit_missing_modules)" 1>&2)
# override _makeit_dummy := $(shell echo 1>&2)
# override _makeit_dummy := \
#  $(shell echo "Add them to the MODULES variable in your makefile." 1>&2)
# override _makeit_dummy := \
#  $(shell echo "The results of the current build are undefined." 1>&2)
# override _makeit_dummy := $(shell echo 1>&2)

endif
