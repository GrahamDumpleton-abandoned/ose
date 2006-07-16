## Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

ifneq "$(X11_INCDIR)" ""
override CPPFLAGS += -I$(X11_INCDIR)
endif

ifneq "$(X11_LIBDIR)" ""
override LD_RUN_PATH := $(LD_RUN_PATH):$(X11_LIBDIR)
override LDFLAGS += $(LOPT)$(X11_LIBDIR)
endif

override LDLIBS += -lX11

ifeq "$(filter -lm,$(LDLIBS))" ""
override SYSTEM_LDLIBS += -lm
endif
