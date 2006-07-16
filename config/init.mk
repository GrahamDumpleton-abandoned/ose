## Copyright 1992 OTC LIMITED
## Copyright 1995-1996 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

all :

makeit.setup ::

## Make sure that OSE_ROOT not set in environment.

override OSE_ROOT :=

## Set target to compilation host if target isn't defined.

ifeq "$(OSE_TARGET)" ""
override OSE_TARGET := $(OSE_HOST)
endif

## Location of makeit files.

OSE_MAKEIT = $(SRCROOT)/include/makeit
OSE_HOST_MAKEIT = $(SRCROOT)/config/$(OSE_HOST)
OSE_TARGET_MAKEIT = $(SRCROOT)/config/$(OSE_TARGET)

## Location of OSE-CONFIG.h.

override PROJECT_CPPFLAGS += -I$(OSE_TARGET_MAKEIT)

## Force cleaning of all temporary files even though they
## may not ever be generated during a particular build.

#_makeit_cleanup_pure := YES
#_makeit_cleanup_objectstore := YES
#_makeit_cleanup_sentinel := YES
#_makeit_cleanup_testcenter := YES

## Dependency to avoid complaints about not be able to generate the header
## file <strstream.h> for platforms that do not require it. At the same time
## we ensure that we remove any dependency on the file.

override DEPFILTER := \
 sed -e "/ \/.*/d" -e "/ [a-zA-Z]:\/.*/d" -e "/config.*strstream/d"

$(OSE_TARGET_MAKEIT)/strstream.h :

## Brute force clean etc.

override MAKEIT_OPTIONS += brute_force_clean

ifeq "$(filter %MACOSX,$(OSE_HOST))" ""
override MAKEIT_OPTIONS += restricted_command_length
endif

## Include normal initialisation stuff.

include $(OSE_MAKEIT)/init.mk

## Win32 DLL export. Only Has affect when PIC also defined.

ifeq "$(ENV_SYSTEM)" "WIN32"
override PROJECT_PIC_CPPFLAGS += -DDLL_EXPORT_OSE
endif

## Versant flags.

ifeq "$(C++COMPILERENV)" "VERSANT"
ifneq "$(filter VERSANT-%,$(C++COMPILERTAG))" ""
override PROJECT_C++FLAGS += -noschema
endif
endif

## Sun C++ 4.0 workaround.

ifneq "$(filter SUN4.0%,$(C++COMPILER))" ""
override MAKEIT_OPTIONS += minimal_expansion
endif

## Enable OTCLIB_TRACE in dbg variant.

ifeq "$(VARIANT)" "dbg"
override PROJECT_CPPFLAGS += -DOTCLIB_TRACE
endif
