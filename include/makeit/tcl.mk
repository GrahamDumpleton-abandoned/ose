## Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

ifeq "$(TK_VERSION)" ""
override TK_VERSION := TCL_VERSION
endif

ifneq "$(TCL_INCDIR)" ""
override CPPFLAGS += -I$(TCL_INCDIR)
endif

ifneq "$(TCL_LIBDIR)" ""
override LD_RUN_PATH := $(LD_RUN_PATH):$(TCL_LIBDIR)
override LDFLAGS += $(LOPT)$(TCL_LIBDIR)
endif

ifneq "$(filter tk,$(TCL_OPTIONS))" ""

override REQUIRE += x11
override LDLIBS += -ltk$(TK_VERSION)

ifeq "$(filter -lm,$(LDLIBS))" ""
override SYSTEM_LDLIBS += -lm
endif

endif

override LDLIBS += -ltcl$(TCL_VERSION)
