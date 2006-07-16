## Copyright 1992-1993 OTC LIMITED
## Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Setup include file search path.

override CPPFLAGS += -I$(OSE_TARGET_HOME)/include
override CPPFLAGS += -I$(OSE_HOME)/include

## Setup location of parameterised types map file.

override PTMAPS += $(OSE_HOME)/include/OTC/PTMAP

## Setup library search path.

ifeq "$(filter use_static_libraries,$(OSE_OPTIONS))" ""
override LD_RUN_PATH := $(OSE_TARGET_HOME)/lib/$(C++COMPILERTAG):$(LD_RUN_PATH)
override LDFLAGS += $(LOPT)$(OSE_TARGET_HOME)/lib/$(C++COMPILERTAG)
else
ifeq "$(ENV_SYSTEM)" "WIN32"
override LDFLAGS += $(LOPT)$(OSE_TARGET_HOME)/lib/$(C++COMPILERTAG)
endif
endif

## Setup ObjectStore library schema databse search path.

ifneq "$(filter OS%,$(C++COMPILER))" ""
override LDFLAGS += -OSL$(OSE_TARGET_HOME)/lib/$(C++COMPILERTAG)
endif

ifeq "$(ENV_SYSTEM)" "WIN32"
override SCHEMAS += $(subst /,\,$(OSE_HOME)/include/OTC/SCHEMA.cc)
else
override SCHEMAS += $(OSE_HOME)/include/OTC/SCHEMA.cc
endif

## Work out which version of the library we are going to link with.
## Always force it to "prf" if that is the variant which the user is
## working in.

ifeq "$(VARIANT)" "prf"
OSE_VARIANT := prf
endif

ifeq "$(OSE_VARIANT)" ""
OSE_VARIANT := $(VARIANT)
endif

## Force loading of any required modules.

ifneq "$(ENV_SYSTEM)" "WIN32"

ifneq "$(filter OTC-QT,$(OSE_OPT_LIBRARIES))" ""
REQUIRE += qt
endif

ifneq "$(filter OTC-PY,$(OSE_OPT_LIBRARIES))" ""
REQUIRE += python
endif

ifneq "$(filter OTC-TCL,$(OSE_OPT_LIBRARIES))" ""
REQUIRE += tcl
endif

endif

## Setup linking in of the libraries.

OSE_LIBRARY_RELTAG := $(subst .,X,$(OSE_RELEASE_NAME))
OSE_LIBRARY_PLTAG := $(patsubst pl00,,pl$(OSE_MAINTENANCE_RELEASE))
OSE_LIBRARY_CXXTAG := $(subst -,_,$(subst .,X,$(C++COMPILERTAG)))

OSE_LIBRARY_TAG := \
 $(OSE_LIBRARY_RELTAG)$(OSE_LIBRARY_PLTAG)_$(OSE_LIBRARY_CXXTAG)

OSE_LIBSUFFIX := $(OSE_LIBRARY_TAG)_$(OSE_VARIANT)

ifneq "$(filter use_static_libraries,$(OSE_OPTIONS))" ""

ifeq "$(ENV_SYSTEM)" "WIN32"

ifneq "$(wildcard \
 $(OSE_TARGET_HOME)/lib/$(C++COMPILERTAG)/OTC_$(OSE_LIBSUFFIX)_static$(LIBEXT))" ""
override LDLIBS += -lOTC_$(OSE_LIBSUFFIX)_static -lOSE
else
override LDLIBS += -lOTC_$(OSE_LIBSUFFIX) -lOSE
endif

else

ifneq "$(filter OTC-QT,$(OSE_OPT_LIBRARIES))" ""
override LDLIBS += \
 $(OSE_TARGET_HOME)/lib/$(C++COMPILERTAG)/libOTC_QT_$(OSE_LIBSUFFIX)$(LIBEXT)
endif

ifneq "$(filter OTC-PY,$(OSE_OPT_LIBRARIES))" ""
override LDLIBS += \
 $(OSE_TARGET_HOME)/lib/$(C++COMPILERTAG)/libOTC_PY_$(OSE_LIBSUFFIX)$(LIBEXT)
endif

ifneq "$(filter OTC-TCL,$(OSE_OPT_LIBRARIES))" ""
override LDLIBS += \
 $(OSE_TARGET_HOME)/lib/$(C++COMPILERTAG)/libOTC_TCL_$(OSE_LIBSUFFIX)$(LIBEXT)
endif

override LDLIBS += \
 $(OSE_TARGET_HOME)/lib/$(C++COMPILERTAG)/libOTC_$(OSE_LIBSUFFIX)$(LIBEXT) \
 $(OSE_TARGET_HOME)/lib/$(C++COMPILERTAG)/libOSE$(LIBEXT)

endif

else

ifeq "$(ENV_SYSTEM)" "WIN32"

ifneq "$(wildcard \
 $(OSE_TARGET_HOME)/lib/$(C++COMPILERTAG)/OTC_$(OSE_LIBSUFFIX)$(SHLEXT))" ""
override CPPFLAGS += -DDLL_IMPORT_OSE
endif

else

ifneq "$(filter OTC-QT,$(OSE_OPT_LIBRARIES))" ""
override LDLIBS += -lOTC_QT_$(OSE_LIBSUFFIX)
endif

endif

ifneq "$(filter OTC-PY,$(OSE_OPT_LIBRARIES))" ""
override LDLIBS += -lOTC_PY_$(OSE_LIBSUFFIX)
endif

ifneq "$(filter OTC-TCL,$(OSE_OPT_LIBRARIES))" ""
override LDLIBS += -lOTC_TCL_$(OSE_LIBSUFFIX)
endif

override LDLIBS += -lOTC_$(OSE_LIBSUFFIX) -lOSE

endif
