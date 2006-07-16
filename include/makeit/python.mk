## Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

ifeq "$(PY_VERSION)" ""
override PY_VERSION := 2.2
endif

ifeq "$(PY_VERSION)" "1.5"
ifneq "$(wildcard /usr/include/thread.h)" ""
override CPPFLAGS += -I/usr/include
endif
endif

ifneq "$(PY_INCDIR)" ""
override CPPFLAGS += -I$(PY_INCDIR)
else
override CPPFLAGS += -I/usr/include/python$(PY_VERSION)
endif

ifneq "$(PY_LIBDIR)" ""
override LDFLAGS += $(LOPT)$(PY_LIBDIR)
else
override LDFLAGS += $(LOPT)/usr/lib/python$(PY_VERSION)/config
endif

ifeq "$(PYTHON)" ""
ifeq "$(filter 1%,$(PY_VERSION))" ""
override PYTHON := python
else
override PYTHON := python$(PY_VERSION)
endif
endif

ifeq "$(ENV_SYSTEM)" "CYGWIN32"
override PIC_LDLIBS += -lpython$(PY_VERSION)
endif
