## Copyright 1992-1993 OTC LIMITED
## Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Define the fail action for a sub make. If -k passed on command line
## want to keep going.

#ifneq "$(findstring k,$(filter -%,$(filter-out --%,$(MAKEFLAGS))))" ""
#ifneq "$(findstring k,$(firstword $(MAKEFLAGS)))" ""
ifneq "$(findstring k,$(firstword $(filter-out --no-print-directory \
 --warn-undefined-variables,$(MAKEFLAGS))))" ""
FAILACTION := true
else
FAILACTION := exit 1
endif

## Work out name of makeit subdirectory.

MKTAG := $(strip $(MKTAG))

ifeq "$(MKTAG)" ""
override MKTAG := $(OSE_TARGET)
endif

override CPU := $(MKTAG)

ifneq "$(MKTAG)" ""
override MK := $(MKTAG)
else
override MK := MK
endif

override WSPREFIX :=

ifneq "$(SRCDIR)" ""
ifneq "$(SRCDIR)" "."
override WSPREFIX := WS-
endif
endif

override MK := $(MK)_$(VARIANT)

ifneq "$(filter encode_compiler_name,$(MAKEIT_OPTIONS))" ""
override MK := $(MK).$(subst .,_,$(C++COMPILERTAG))
endif

ifneq "$(WSPREFIX)" ""
override MK := $(WSPREFIX)$(MK)
endif

ifneq "$(MKPREFIX)" ""
override MK := $(MKPREFIX)$(MK)
endif

ifneq "$(MKSUFFIX)" ""
override MK := $(MK)$(MKSUFFIX)
endif

ifneq "$(PTRDIR)" ""
override MKPTR := $(PTRDIR)
endif

ifeq "$(MKPTR)" ""
override MKPTR := $(MK).ptr
endif

## Calculate SRCDIR, source files etc.

override SRCDIR := $(strip $(SRCDIR))

ifeq "$(SRCDIR)" ""
override SRCDIR := .
endif

# Prevent SRCDIR being passed on command line to sub makes. While we
# are doing this, do the same thing for SUBDIRS.

override MAKEOVERRIDES := $(filter-out SUBDIRS=% SRCDIR=%,$(MAKEOVERRIDES))

# Handle specification of additional source directories.

ifneq "$(SRCDIR)" "."
override SRCDIRS := $(foreach dir,$(SRCDIRS),$(dir) \
 $(patsubst %,$(SRCDIR)/%,$(filter-out /%,$(dir))))
endif

override SRCDIRS := $(wildcard $(strip $(SRCDIRS)))

override MAKEOVERRIDES := $(filter-out SRCDIRS=%,$(MAKEOVERRIDES))

# Work out set of source files. If compilation in separate directory, also
# merge in source files in that directory.

override SRCFILES := $(notdir $(wildcard $(SRCDIR)/*))

ifneq "$(SRCDIR)" "."
override SRCFILES += $(wildcard *)
#override SRCFILES := $(sort $(SRCFILES))
endif

ifneq "$(SRCDIR)" "."
override SRCFILES += $(wildcard *)
endif

ifneq "$(SRCDIRS)" ""
override SRCFILES += $(notdir $(wildcard $(addsuffix /*,$(SRCDIRS))))
endif

override SRCFILES := $(sort $(SRCFILES))

# Check for illegal files. These are files containing a '%' or '#'
# in their name. Take them out and give a diagnostic warning.

_makeit_illegal_files :=
_makeit_illegal_files += $(filter-out $(subst %,,$(SRCFILES)),$(SRCFILES))
_makeit_illegal_files += $(filter-out $(subst \#,,$(SRCFILES)),$(SRCFILES))
_makeit_illegal_files := $(sort $(_makeit_illegal_files))

ifneq "$(_makeit_illegal_files)" ""

override SRCFILES := $(filter $(subst %,,$(SRCFILES)),$(SRCFILES))
override SRCFILES := $(filter $(subst \#,,$(SRCFILES)),$(SRCFILES))

makeit.setup ::
	@echo
	@echo "The following files contained illegal"
	@echo "characters in their names:"
	@echo
	@echo "  $(_makeit_illegal_files)"
	@echo
	@echo "These files will be ignored."
	@echo

endif

# Backwards compatability define for ENV_WIN32.

ifeq "$(ENV_SYSTEM)" "WIN32"
override ENV_WIN32 := YES
endif

# Define suffixes for files if undefined.

ifeq "$(ENV_SYSTEM)" "WIN32"
override BINEXT := .exe
ifeq "$(ENV_TOOLSET)" "MKS"
override CMDEXT := .ksh
endif
override LIBPRE :=
override LIBEXT := .lib
override OBJEXT := .obj
override SHLPRE :=
override SHLEXT := .dll
endif

ifeq "$(ENV_SYSTEM)" "CYGWIN32"
override BINEXT := .exe
override SHLPRE :=
override SHLEXT := .dll
override IMLPRE := lib
override IMLEXT := .dll.a
endif

ifneq "$(wildcard /lib/libc.sl /usr/lib/libc.sl)" ""
override SHLEXT := .sl
else
ifneq "$(wildcard /lib/libc_s.a /usr/lib/libc_s.a)" ""
override SHLEXT := _s.a
else
ifneq "$(wildcard /usr/lib/libc.dylib)" ""
override SHLEXT := .dylib
endif
endif
endif

ifeq "$(origin BINEXT)" "undefined"
override BINEXT :=
endif
ifeq "$(origin CMDEXT)" "undefined"
override CMDEXT :=
endif
ifeq "$(origin LIBPRE)" "undefined"
override LIBPRE := lib
endif
ifeq "$(origin LIBEXT)" "undefined"
override LIBEXT := .a
endif
ifeq "$(origin OBJEXT)" "undefined"
override OBJEXT := .o
endif
ifeq "$(origin SHLPRE)" "undefined"
override SHLPRE := lib
endif
ifeq "$(origin SHLEXT)" "undefined"
override SHLEXT := .so
endif

ifeq "$(SHLEXT)" ".so"
ifneq "$(wildcard /lib/libc.sa* /usr/lib/libc.sa* /usr/shlib/libc.sa.*)" ""
override STLPRE := lib
override STLEXT := .sa
endif
endif

ifneq "$(filter loadable_module,$(SHLIB_OPTIONS))" ""
ifeq "$(ENV_SYSTEM)" "UNIX"
override SHLPRE =
override SHLEXT = .so
endif
endif

# Default LIBVERSION if not defined.

ifeq "$(LIBVERSION)" ""
LIBVERSION = 1.0
endif

# Default for RANLIB.

ifeq "$(RANLIB)" ""
override RANLIB := $(firstword $(wildcard /bin/ranlib /usr/bin/ranlib))
ifeq "$(RANLIB)" ""
override RANLIB := : ranlib
endif
endif

# Default for LOPT.

ifeq "$(LOPT)" ""
override LOPT := -L
endif

# How a clean is performed. Some platforms can have command line length
# problems. In these cases we have to be tricky.

ifneq "$(filter brute_force_clean,$(MAKEIT_OPTIONS))" ""
_makeit_sys_brute_force_clean := YES
endif

ifneq "$(filter restricted_command_length,$(MAKEIT_OPTIONS))" ""
_makeit_sys_restricted_command_length := YES
endif

ifeq "$(_makeit_sys_restricted_command_length)" "YES"
_makeit_sys_brute_force_clean := YES
endif
