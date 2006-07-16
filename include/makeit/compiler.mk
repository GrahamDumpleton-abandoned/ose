## Copyright 1992-1993 OTC LIMITED
## Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Fall back on default for C++COMPILER is not defined. Also work out
## the environment to use.

override C++COMPILER := $(strip $(C++COMPILER))
override DEFAULTC++COMPILER := $(strip $(DEFAULTC++COMPILER))

ifeq "$(C++COMPILER)" ""
override C++COMPILER := $(DEFAULTC++COMPILER)
endif

override C++COMPILERTAG := $(C++COMPILER)
override C++COMPILERENV := $(word 1,$(subst -, ,$(C++COMPILER)))

ifneq "$(C++COMPILERENV)" "$(C++COMPILER)"
override C++COMPILER := $(word 2,$(subst -, ,$(C++COMPILER)))
else
override C++COMPILERENV :=
endif

## Workaround for CYGWIN32 GNU Make which doesn't pass command
## line variable settings through to all submakes correctly.

# Presume this is fixed by now.

#ifneq "$(filter %WIN32,$(ENV_SYSTEM))" ""
#ifneq "$(filter cygwin%,$(OSTYPE))" ""
#
#ifeq "$(origin VARIANT)" "command line"
#MAKE_COMMAND += VARIANT=$(VARIANT)
#endif
#
#MAKE_COMMAND += -I$(OSE_HOST_HOME)
#
#endif
#endif

## Fall back on default for VARIANT if not defined.

override VARIANTS := $(strip $(VARIANTS))

ifneq "$(VARIANTS)" ""
override $(C++COMPILER)_VARIANTS := \
 $(filter $($(C++COMPILER)_VARIANTS),$(VARIANTS))
override $(C++COMPILER)_PIC_VARIANTS := \
 $(filter $($(C++COMPILER)_PIC_VARIANTS),$(VARIANTS))
endif

override VARIANTS := $($(C++COMPILER)_VARIANTS)

override VARIANT := $(strip $(VARIANT))
override DEFAULTVARIANT := $(strip $(DEFAULTVARIANT))

ifeq "$(filter $(DEFAULTVARIANT),$(VARIANTS))" ""
override DEFAULTVARIANT := $(firstword $(VARIANTS))
endif

ifeq "$(VARIANT)" ""
override VARIANT := $(DEFAULTVARIANT)
endif

# Can we build shared libraries in this variant.

ifneq "$(filter $(VARIANT),$($(C++COMPILER)_PIC_VARIANTS))" ""
ifneq "$(filter shlib,$(MAKEIT_OPTIONS))" ""
_makeit_enable_shared_libraries := YES
else
_makeit_enable_shared_libraries := NO
endif
endif

## Define symbol identifying the target system.

override INTERNAL_CPPFLAGS += -DSYS_$(OSE_TARGET)

## Define symbol identifying database environment being used.

ifneq "$($(C++COMPILER)_ENV_OODBMS)" ""
override ENV_OODBMS := $($(C++COMPILER)_ENV_OODBMS)
endif

ifneq "$(ENV_OODBMS)" ""
override INTERNAL_CPPFLAGS += -DENV_$(ENV_OODBMS)
endif

## Setup paths of compilers and linkers we are going to use based
## on information in host file. Allow CC and LD to be overriden.
## Always set C++ according to value of C++COMPILER.

override C++ := $($(C++COMPILER)_C++)

ifneq "$(filter default environment,$(origin CC))" ""
ifneq "$($(C++COMPILER)_CC)" ""
override CC := $($(C++COMPILER)_CC)
endif
endif

ifneq "$(filter default environment,$(origin LD))" ""
ifneq "$($(C++COMPILER)_LD)" ""
override LD := $($(C++COMPILER)_LD)
endif
endif

ifneq "$(filter default environment,$(origin AR))" ""
ifneq "$($(C++COMPILER)_AR)" ""
override AR := $($(C++COMPILER)_AR)
endif
endif

ifeq "$(C++)" ""
override C++ := exit 1; *** bad compiler definition ***
endif

ifeq "$(CC)" ""
override CC := exit 1; *** bad compiler definition ***
endif

ifeq "$(LD)" ""
override LD := exit 1; *** bad compiler definition ***
endif

ifeq "$(AR)" ""
override AR := exit 1; *** bad compiler definition ***
endif

## Setup library management macros.

ifeq "$(origin $(C++COMPILER)_LIB_CLOSURE)" "undefined"
override $(C++COMPILER)_LIB_CLOSURE = $(DEFAULT_LIB_CLOSURE)
endif

ifeq "$(origin $(C++COMPILER)_LIB_ARCHIVE)" "undefined"
override $(C++COMPILER)_LIB_ARCHIVE = $(DEFAULT_LIB_ARCHIVE)
endif

ifeq "$(origin $(C++COMPILER)_LIB_EXTRACT)" "undefined"
override $(C++COMPILER)_LIB_EXTRACT = $(DEFAULT_LIB_EXTRACT)
endif

ifeq "$(origin $(C++COMPILER)_PIC_CLOSURE)" "undefined"
override $(C++COMPILER)_PIC_CLOSURE = $(DEFAULT_PIC_CLOSURE)
endif

ifeq "$(origin $(C++COMPILER)_PIC_ARCHIVE)" "undefined"
override $(C++COMPILER)_PIC_ARCHIVE = $(DEFAULT_PIC_ARCHIVE)
endif

ifeq "$(origin $(C++COMPILER)_PIC_EXTRACT)" "undefined"
override $(C++COMPILER)_PIC_EXTRACT = $(DEFAULT_PIC_EXTRACT)
endif

ifeq "$(origin $(C++COMPILER)_PIC_LIBRARY)" "undefined"
override $(C++COMPILER)_PIC_LIBRARY = $(DEFAULT_PIC_LIBRARY)
endif

ifeq "$(origin $(C++COMPILER)_PIC_OBJECT)" "undefined"
ifeq "$(origin $(C++COMPILER)_PIC_LIBRARY)" "undefined"
override $(C++COMPILER)_PIC_OBJECT = $(DEFAULT_PIC_LIBRARY)
else
override $(C++COMPILER)_PIC_OBJECT = $($(C++COMPILER)_PIC_LIBRARY)
endif
endif

## Define location of Versant if not defined.

ifeq "$(ENV_OODBMS)" "VERSANT"
ifeq "$(VPP_RELEASE)" ""
override VPP_RELEASE = $(shell oscp -p)
endif
endif

## Defines properties of various C++ compilers we know about.

# AT&T C++ Version 3.0

ifneq "$(filter ATT3.0 ATT3.0.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_cfront_repositories := YES
_makeit_cxx_supports_template_file := YES
_makeit_cxx_supports_direct_instantiation := YES

override INTERNAL_C++FLAGS += -DCXX_ATT -DCXX_ATT3_0

ifneq "$(filter ATT3.0.%,$(C++COMPILER))" ""
override INTERNAL_C++FLAGS += -DCXX_$(subst .,_,$(C++COMPILER))
endif

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += +w
endif

ifeq "$(ENV_OODBMS)" "OSTORE"
override INTERNAL_C++FLAGS += -D__cfront21
endif

ifeq "$(ENV_OODBMS)" "VERSANT"
export CXXATT_PATH := $(patsubst %/bin/,%,$(dir $(C++)))
export CXXATT_CC := $(C++)
override C++ := $(VPP_RELEASE)/bin/att.3.0/cxxvatt
ifneq "$(filter production_library,$(VERSANT_OPTIONS))" ""
override LD_RUN_PATH := $(LD_RUN_PATH):$(VPP_RELEASE)/lib/vatt.3.0
override INTERNAL_LDFLAGS += $(LOPT)$(VPP_RELEASE)/lib/vatt.3.0
else
override LD_RUN_PATH := $(LD_RUN_PATH):$(VPP_RELEASE)/lib/vatt.3.0.chk
override INTERNAL_LDFLAGS += $(LOPT)$(VPP_RELEASE)/lib/vatt.3.0.chk
endif
endif

endif

# CenterLine C++ Version 2.0/2.1

ifneq "$(filter CL2.0 CL2.0.% CL2.1 CL2.1.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_o_for_objects := YES
_makeit_cxx_supports_cfront_repositories := YES
_makeit_cxx_supports_template_file := YES
_makeit_cxx_supports_direct_instantiation := YES

override INTERNAL_C++FLAGS += -DCXX_CL

ifneq "$(filter CL2.0 CL2.0.%,$(C++COMPILER))" ""
override INTERNAL_C++FLAGS += -DCXX_CL2_0
endif

ifneq "$(filter CL2.1 CL2.1.%,$(C++COMPILER))" ""
override INTERNAL_C++FLAGS += -DCXX_CL2_1
endif

ifneq "$(filter CL2.0.% CL2.1.%,$(C++COMPILER))" ""
override INTERNAL_C++FLAGS += -DCXX_$(subst .,_,$(C++COMPILER))
else
override INTERNAL_C++FLAGS += -DCXX_$(subst .,_,$(C++COMPILER))_0
endif

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += +w
endif

ifeq "$(ENV_OODBMS)" "OSTORE"
override INTERNAL_C++FLAGS += -D__cfront21
endif

ifeq "$(ENV_OODBMS)" "VERSANT"
export CXXCENTER_PATH := $(patsubst %/bin/,%,$(dir $(C++)))
export CXXCENTER_CC := $(C++)
override C++ := $(VPP_RELEASE)/bin/center.3.0/cxxcenter
ifneq "$(filter production_library,$(VERSANT_OPTIONS))" ""
override LD_RUN_PATH := $(LD_RUN_PATH):$(VPP_RELEASE)/lib/center.3.0
override INTERNAL_LDFLAGS += $(LOPT)$(VPP_RELEASE)/lib/center.3.0
else
override LD_RUN_PATH := $(LD_RUN_PATH):$(VPP_RELEASE)/lib/center.3.0.chk
override INTERNAL_LDFLAGS += $(LOPT)$(VPP_RELEASE)/lib/center.3.0.chk
endif
endif

endif

# CDS C++

ifneq "$(filter CDS%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_ansi_exceptions := YES
_makeit_combine_compile_and_link := YES
_makeit_cxx_supports_o_for_objects := YES
_makeit_cxx_supports_direct_instantiation := YES

override INTERNAL_C++FLAGS += -DCXX_CDS -DCXX_$(subst .,_,$(C++COMPILER))

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
#override INTERNAL_C++FLAGS += +w
endif

endif

# DEC C++

ifneq "$(filter DEC1.% DEC2.% DEC3.% DEC4.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_precompiled_templates := YES
_makeit_cxx_supports_direct_instantiation := YES
_makeit_cxx_supports_template_file := YES
_makeit_cxx_supports_ansi_exceptions := YES

override PTFLAGS += -define_templates
override INTERNAL_C++FLAGS += -D__STDCPP__
override INTERNAL_C++FLAGS += -DCXX_DEC -DCXX_$(subst .,_,$(C++COMPILER))

# ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
# override INTERNAL_C++FLAGS += -w0
# endif

endif

# DEC C++ with -pt support

ifneq "$(filter DEC5.% DEC6.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_direct_instantiation := YES
_makeit_cxx_supports_template_file := YES
_makeit_cxx_supports_ansi_exceptions := YES

ifneq "$(filter DEC5.0,$(C++COMPILER))" ""
_makeit_cxx_supports_precompiled_templates := YES
override PTFLAGS += -define_templates
else
ifneq "$(filter -nopt,$($(C++COMPILER)_C++FLAGS))" ""
_makeit_cxx_supports_precompiled_templates := YES
else
_makeit_sys_restricted_command_length := YES
_makeit_cxx_supports_dec_repositories := YES
endif
endif

override INTERNAL_C++FLAGS += -D__STDCPP__
override INTERNAL_C++FLAGS += -DCXX_DEC -DCXX_$(subst .,_,$(C++COMPILER))

# ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
# override INTERNAL_C++FLAGS += -w0
# endif

endif

# GNU/Cygnus C++ Version 2.5.X

ifneq "$(filter CYG2.5.% GNU2.5.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_o_for_objects := YES
_makeit_cxx_supports_template_file := YES
_makeit_cxx_supports_direct_instantiation := YES

override PTFLAGS += -fexternal-templates
override INTERNAL_C++FLAGS += -DCXX_GNU -DCXX_GNU2_5
override INTERNAL_C++FLAGS += -DCXX_$(subst .,_,$(C++COMPILER))

ifneq "$(filter CYG%,$(C++COMPILER))" ""
override INTERNAL_C++FLAGS += -DCXX_CYGNUS
endif

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += -Wall
endif

endif

# GNU/Cygnus C++ Version 2.[67].X

ifneq "$(filter CYG2.6.% CYG2.7.% GNU2.6.% GNU2.7.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_o_for_objects := YES
_makeit_cxx_supports_template_file := YES
_makeit_cxx_supports_direct_instantiation := YES
_makeit_cxx_supports_precompiled_templates := YES

ifneq "$($(C++COMPILER)_ROOT)" ""
override LD_RUN_PATH := $(LD_RUN_PATH):$($(C++COMPILER)_ROOT)/lib
endif

ifneq "$(filter CYG2.7.97R2 CYG2.7.2.97R2,$(C++COMPILER))" ""
_makeit_cxx_supports_ansi_exceptions := YES
else
ifneq "$(filter CYG2.7.96Q4 CYG2.7.97R1 CYG2.7.2.97R1,$(C++COMPILER))" ""
_makeit_cxx_supports_ansi_exceptions := YES
override INTERNAL_C++FLAGS += -fexceptions
else
ifneq "$(filter ansi_exceptions,$($(C++COMPILER)_OPTIONS))" ""
_makeit_cxx_supports_ansi_exceptions := YES
override INTERNAL_C++FLAGS += -fhandle-exceptions
endif
endif
endif

ifneq "$(filter no_implicit_templates,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += -fno-implicit-templates
else
override PTFLAGS += -fexternal-templates
endif

override INTERNAL_C++FLAGS += -DCXX_GNU

ifneq "$(filter CYG2.6.% GNU2.6.%,$(C++COMPILER))" ""
override INTERNAL_C++FLAGS += -DCXX_GNU2_6
endif
ifneq "$(filter CYG2.7.% GNU2.7.%,$(C++COMPILER))" ""
override INTERNAL_C++FLAGS += -DCXX_GNU2_7
endif

override INTERNAL_C++FLAGS += -DCXX_$(subst .,_,$(C++COMPILER))

ifneq "$(filter CYG%,$(C++COMPILER))" ""
override INTERNAL_C++FLAGS += -DCXX_CYGNUS
endif

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += -Wall
endif

ifneq "$(filter GNU2.7.0,$(C++COMPILER))" ""
# Prohibit inclusion of <std/cstring.h>, it conflicts with
# definitions in <memory.h>. Problem is fixed in GNU 2.7.1.
override INTERNAL_C++FLAGS += -D__CSTRING__
endif

ifneq "$(filter -frtti,$($(C++COMPILER)_C++FLAGS))" ""
override INTERNAL_C++FLAGS += -DHAVE_RTTI
endif

endif

# GNU C++ Version 2.8.X

ifneq "$(filter GNU2.8.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_o_for_objects := YES
_makeit_cxx_supports_template_file := YES
_makeit_cxx_supports_direct_instantiation := YES
_makeit_cxx_supports_precompiled_templates := YES
_makeit_cxx_supports_ansi_exceptions := YES

ifneq "$(filter no_implicit_templates,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += -fno-implicit-templates
else
override PTFLAGS += -fexternal-templates
endif

override INTERNAL_C++FLAGS += -DCXX_GNU

ifneq "$(filter GNU2.8.%,$(C++COMPILER))" ""
override INTERNAL_C++FLAGS += -DCXX_GNU2_8
endif

override INTERNAL_C++FLAGS += -DCXX_$(subst .,_,$(C++COMPILER))

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += -Wall
endif

endif

# GNU EGCS C++ Version 1.X

ifneq "$(filter EGCS1.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_o_for_objects := YES
_makeit_cxx_supports_template_file := YES
_makeit_cxx_supports_ansi_exceptions := YES

ifeq "$(filter -frepo,$($(C++COMPILER)_C++FLAGS))" ""
_makeit_cxx_supports_precompiled_templates := YES
_makeit_cxx_supports_direct_instantiation := YES
ifeq "$(filter X86_CYGWIN32,$(OSE_TARGET))" ""
_makeit_cxx_supports_direct_instantiation := YES
ifneq "$(filter no_implicit_templates,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += -fno-implicit-templates
else
override PTFLAGS += -fexternal-templates
endif
else
override INTERNAL_C++FLAGS += -DEXPAND_TEMPLATES
endif
else
override INTERNAL_C++FLAGS += -DEXPAND_TEMPLATES
endif

override INTERNAL_C++FLAGS += -DCXX_GNU -DCXX_EGCS

override INTERNAL_C++FLAGS += -DCXX_$(subst .,_,$(C++COMPILER))

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += -Wall
endif

endif

# Cygnus C++ Version 2.9.X

ifneq "$(filter CYG2.9%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_o_for_objects := YES
_makeit_cxx_supports_template_file := YES
_makeit_cxx_supports_ansi_exceptions := YES

ifeq "$(filter -frepo,$($(C++COMPILER)_C++FLAGS))" ""
_makeit_cxx_supports_precompiled_templates := YES
_makeit_cxx_supports_direct_instantiation := YES
else
override INTERNAL_C++FLAGS += -DEXPAND_TEMPLATES
endif

ifneq "$($(C++COMPILER)_ROOT)" ""
override LD_RUN_PATH := $(LD_RUN_PATH):$($(C++COMPILER)_ROOT)/lib
endif

ifneq "$(filter no_implicit_templates,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += -fno-implicit-templates
else
override PTFLAGS += -fexternal-templates
endif

override INTERNAL_C++FLAGS += -DCXX_GNU

ifneq "$(filter CYG2.9.%,$(C++COMPILER))" ""
override INTERNAL_C++FLAGS += -DCXX_GNU2_9
endif

override INTERNAL_C++FLAGS += -DCXX_$(subst .,_,$(C++COMPILER))

ifneq "$(filter CYG%,$(C++COMPILER))" ""
override INTERNAL_C++FLAGS += -DCXX_CYGNUS
endif

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += -Wall
endif

endif

# GNU C++ Version 2.95.X and later development versions

ifneq "$(filter GNU2.95% GNU2.96% GNU2.97%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_o_for_objects := YES
_makeit_cxx_supports_template_file := YES
_makeit_cxx_supports_ansi_exceptions := YES

#ifeq "$(filter -frepo,$($(C++COMPILER)_C++FLAGS))" ""
_makeit_cxx_supports_precompiled_templates := YES
#_makeit_cxx_supports_direct_instantiation := YES
#ifeq "$(filter X86_CYGWIN32,$(OSE_TARGET))" ""
#_makeit_cxx_supports_direct_instantiation := YES
#ifneq "$(filter no_implicit_templates,$(MAKEIT_OPTIONS))" ""
#override INTERNAL_C++FLAGS += -fno-implicit-templates
#else
#override PTFLAGS += -fexternal-templates
#endif
#else
#override INTERNAL_C++FLAGS += -DEXPAND_TEMPLATES
#endif
#else
override INTERNAL_C++FLAGS += -DEXPAND_TEMPLATES
#endif

ifneq "$(filter %MACOSX,$(OSE_HOST))" ""
_makeit_cxx_supports_precompiled_templates := NO
endif

#ifneq "$(filter GNU2.96% GNU2.97%,$(C++COMPILER))" ""
#override INTERNAL_C++FLAGS += -DENV_CXXSTDLIB
#endif

override INTERNAL_C++FLAGS += -DCXX_GNU

override INTERNAL_C++FLAGS += -DCXX_$(subst .,_,$(C++COMPILER))

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += -Wall
endif

endif

# GNU C++ Version 3.X.

ifneq "$(filter GNU3.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_o_for_objects := YES
_makeit_cxx_supports_template_file := YES
_makeit_cxx_supports_ansi_exceptions := YES

_makeit_cxx_supports_precompiled_templates := YES
override INTERNAL_C++FLAGS += -DEXPAND_TEMPLATES

ifneq "$(filter %MACOSX,$(OSE_HOST))" ""
_makeit_cxx_supports_precompiled_templates := NO
endif

override INTERNAL_C++FLAGS += -DENV_CXXSTDLIB

override INTERNAL_C++FLAGS += -DCXX_GNU

override INTERNAL_C++FLAGS += -DCXX_$(subst .,_,$(C++COMPILER))

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += -Wall -Wno-deprecated
endif

endif

# HP C++

ifneq "$(filter HP%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

ifneq "$(filter +eh,$($(C++COMPILER)_C++FLAGS))" ""
_makeit_cxx_supports_ansi_exceptions := YES
endif

_makeit_cxx_supports_o_for_objects := YES
_makeit_cxx_supports_cfront_repositories := YES
_makeit_cxx_supports_direct_instantiation := YES

override INTERNAL_C++FLAGS += -DCXX_HP -DCXX_$(subst .,_,$(C++COMPILER))

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += +w
endif

ifeq "$(ENV_OODBMS)" "OSTORE"
override INTERNAL_C++FLAGS += -D__cfront21
endif

endif

# HP aC++

ifneq "$(filter AHP%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

ifeq "$(filter +noeh,$($(C++COMPILER)_C++FLAGS))" ""
_makeit_cxx_supports_ansi_exceptions := YES
endif

_makeit_cxx_supports_o_for_objects := YES
_makeit_cxx_supports_direct_instantiation := YES
#_makeit_cxx_supports_precompiled_templates := YES

override INTERNAL_C++FLAGS += +inst_implicit_include -DHAVE_BOOL
override INTERNAL_C++FLAGS += -DCXX_AHP -DCXX_$(subst .,_,$(C++COMPILER))

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += +w
endif

endif

# KAI C++

ifneq "$(filter KAI%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_o_for_objects := YES
_makeit_cxx_supports_edg_repositories := YES

ifneq "$(filter -x --exceptions,$($(C++COMPILER)_C++FLAGS))" ""
_makeit_cxx_supports_ansi_exceptions := YES
endif

override INTERNAL_C++FLAGS += -DCXX_KAI -DCXX_$(subst .,_,$(C++COMPILER))
override INTERNAL_C++FLAGS += -DHAVE_BOOL

ifeq "$(filter KAI1.% KAI2.% KAI3.0 KAI3.1 KAI3.2,$(C++COMPILER))" ""
override INTERNAL_C++FLAGS += -DEXPAND_TEMPLATES
endif

endif

# Micro$oft C++ 2.X (WIN32)

ifneq "$(filter MS2.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

#_makeit_cxx_supports_precompiled_templates := YES
_makeit_cxx_supports_ansi_exceptions := YES

override INTERNAL_CPPFLAGS += -I$(dir $(C++))../INCLUDE

override INTERNAL_C++FLAGS += -DCXX_MS -DCXX_$(subst .,_,$(C++COMPILER))
override INTERNAL_C++FLAGS += -DWIN32 -DEXPAND_TEMPLATES

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += -W3
endif

override INTERNAL_LDFLAGS += -L$(dir $(C++))../LIB

override SYSTEM_LDLIBS += WSOCK32.LIB

ifneq "$(filter -DHAVE_WIN32_THREADS,$($(C++COMPILER)_CPPFLAGS))" ""
ifeq "$(VARIANT)" "dbg"
override INTERNAL_CFLAGS += -MDd
override INTERNAL_C++FLAGS += -MDd
override SYSTEM_LDFLAGS += /NODEFAULTLIB:MSVCRT.LIB
override SYSTEM_LDFLAGS += /NODEFAULTLIB:LIBC.LIB
else
override INTERNAL_CFLAGS += -MD
override INTERNAL_C++FLAGS += -MD
endif
else
ifeq "$(VARIANT)" "dbg"
override INTERNAL_CFLAGS += -MLd
override INTERNAL_C++FLAGS += -MLd
override SYSTEM_LDFLAGS += /NODEFAULTLIB:LIBC.LIB
else
override INTERNAL_CFLAGS += -ML
override INTERNAL_C++FLAGS += -ML
endif
endif

endif

# Micro$oft C++ 4.X/5.X (WIN32)

ifneq "$(filter MS4.% MS5.% MS6.% MS7.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

#_makeit_cxx_supports_precompiled_templates := YES
_makeit_cxx_supports_ansi_exceptions := YES

override INTERNAL_CPPFLAGS += -I$(dir $(C++))../INCLUDE

ifeq "$(filter MS4.%,$(C++COMPILER))" ""
override INTERNAL_CPPFLAGS += -DHAVE_BOOL
endif

override INTERNAL_C++FLAGS += -GX
override INTERNAL_C++FLAGS += -DCXX_MS -DCXX_$(subst .,_,$(C++COMPILER))
override INTERNAL_C++FLAGS += -DWIN32 -DEXPAND_TEMPLATES

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += -W3
endif

override INTERNAL_LDFLAGS += -L$(dir $(C++))../LIB

ifneq "$(filter -DHAVE_WINSOCK2,$($(C++COMPILER)_CPPFLAGS))" ""
override SYSTEM_LDLIBS += WS2_32.LIB
else
override SYSTEM_LDLIBS += WSOCK32.LIB
endif

ifneq "$(filter MS7.%,$(C++COMPILER))" ""
override INTERNAL_CPPFLAGS += -I$(dir $(C++))../PlatformSDK/Include
override INTERNAL_CPPFLAGS += -DENV_CXXSTDLIB
override INTERNAL_LDFLAGS += -L$(dir $(C++))../PlatformSDK/Lib
endif

#ifneq "$(filter -DHAVE_WIN32_THREADS,$($(C++COMPILER)_CPPFLAGS))" ""
ifeq "$(VARIANT)" "dbg"
override INTERNAL_CFLAGS += -MDd
override INTERNAL_C++FLAGS += -MDd
#override SYSTEM_LDFLAGS += /NODEFAULTLIB:MSVCRT.LIB
#override SYSTEM_LDFLAGS += /NODEFAULTLIB:LIBC.LIB
else
override INTERNAL_CFLAGS += -MD
override INTERNAL_C++FLAGS += -MD
endif
#else
#ifeq "$(VARIANT)" "dbg"
#override INTERNAL_CFLAGS += -MLd
#override INTERNAL_C++FLAGS += -MLd
#override SYSTEM_LDFLAGS += /NODEFAULTLIB:LIBC.LIB
#else
#override INTERNAL_CFLAGS += -ML
#override INTERNAL_C++FLAGS += -ML
#endif
#endif

endif

# Novell C++

ifneq "$(filter NOV%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_combine_compile_and_link := YES

override INTERNAL_C++FLAGS += -Tauto
override INTERNAL_C++FLAGS += -DCXX_NOV -DCXX_$(subst .,_,$(C++COMPILER))

endif

# SGI C++ (CFRONT Port)

ifneq "$(filter SG3.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_cfront_repositories := YES
_makeit_cxx_supports_template_file := YES
_makeit_cxx_supports_direct_instantiation := YES

override INTERNAL_C++FLAGS += -DCXX_SG
override INTERNAL_C++FLAGS += -DCXX_$(subst .,_,$(C++COMPILER))

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += +w
endif

ifeq "$(ENV_OODBMS)" "VERSANT"
export CXXSGI_PATH := $(dir $(C++))
export CXXSGI_CC := $(C++)
override C++ := $(VPP_RELEASE)/bin/sgi.3.0/cxxsgi
ifneq "$(filter production_library,$(VERSANT_OPTIONS))" ""
override INTERNAL_LDFLAGS += $(LOPT)$(VPP_RELEASE)/lib/sgi.3.0
else
override INTERNAL_LDFLAGS += $(LOPT)$(VPP_RELEASE)/lib/sgi.3.0.chk
endif
endif

endif

# SGI C++ (EDG Front End)

ifneq "$(filter SG4.% SG6.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_o_for_objects := YES

override INTERNAL_C++FLAGS += -ptused -prelink
override INTERNAL_C++FLAGS += -DCXX_SG -DCXX_$(subst .,_,$(C++COMPILER))

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += +w
endif

endif

# SGI C++ (EDG Front End)

ifneq "$(filter SG7.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_o_for_objects := YES
# _makeit_cxx_supports_edg_repositories := YES

override INTERNAL_C++FLAGS += -D__STDCPP__ -ptall
override INTERNAL_C++FLAGS += -DCXX_SG -DCXX_$(subst .,_,$(C++COMPILER))

ifneq "$(filter -n32 -64,$($(C++COMPILER)_C++FLAGS))" ""
_makeit_cxx_supports_ansi_exceptions := YES
override INTERNAL_C++FLAGS += -DHAVE_BOOL
endif

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += -fullwarn
endif

endif

# SGI DELTA C++ (DELTA C++ / EDG Front End)

ifneq "$(filter SGD%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_o_for_objects := YES

override INTERNAL_C++FLAGS += -ptused -prelink
override INTERNAL_C++FLAGS += -DCXX_SGD -DCXX_$(subst .,_,$(C++COMPILER))

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += +w
endif

endif

# Sun C++ Version 3.0

ifneq "$(filter SUN3.0 SUN3.0.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_o_for_objects := YES
_makeit_cxx_supports_cfront_repositories := YES
_makeit_cxx_supports_template_file := YES
_makeit_cxx_supports_direct_instantiation := YES

override INTERNAL_C++FLAGS += -DCXX_SUN -DCXX_SUN3_0

ifneq "$(filter SUN3.0.%,$(C++COMPILER))" ""
override INTERNAL_C++FLAGS += -DCXX_$(subst .,_,$(C++COMPILER))
endif

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += +w
endif

ifeq "$(ENV_OODBMS)" "OSTORE"
override INTERNAL_C++FLAGS += -D__cfront21
endif

ifeq "$(ENV_OODBMS)" "VERSANT"
ifneq "$(filter %/SC2.0.1/CC %/SC2.0.1patch/CC,$(C++))" ""
export CXXSUN_PATH := $(dir $(C++))
else
export CXXSUN_PATH := \
 $(firstword $(wildcard $(dir $(C++))SC2.0.1patch $(dir $(C++))SC2.0.1))
endif
export CXXSUN_CC := $(C++)
override C++ := $(VPP_RELEASE)/bin/sun.3.0/cxxsun
ifneq "$(filter production_library,$(VERSANT_OPTIONS))" ""
override LD_RUN_PATH := $(LD_RUN_PATH):$(VPP_RELEASE)/lib/sun.3.0
override INTERNAL_LDFLAGS += $(LOPT)$(VPP_RELEASE)/lib/sun.3.0
else
override LD_RUN_PATH := $(LD_RUN_PATH):$(VPP_RELEASE)/lib/sun.3.0.chk
override INTERNAL_LDFLAGS += $(LOPT)$(VPP_RELEASE)/lib/sun.3.0.chk
endif
endif

endif

# Sun C++ Version 4.X

ifneq "$(filter SUN4.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_o_for_objects := YES
_makeit_cxx_supports_cafe_repositories := YES
_makeit_cxx_supports_ansi_exceptions := YES

ifeq "$(ENV_OODBMS)" "VERSANT"
override C++COMPILERTAG := $(C++COMPILER)
ifneq "$(filter production_library,$(VERSANT_OPTIONS))" ""
override LD_RUN_PATH := $(LD_RUN_PATH):$(VPP_RELEASE)/lib/sun.4.0
override INTERNAL_LDFLAGS += $(LOPT)$(VPP_RELEASE)/lib/sun.4.0
else
override LD_RUN_PATH := $(LD_RUN_PATH):$(VPP_RELEASE)/lib/sun.4.0.chk
override INTERNAL_LDFLAGS += $(LOPT)$(VPP_RELEASE)/lib/sun.4.0.chk
endif
endif

override INTERNAL_C++FLAGS += -DCXX_SUN -DCXX_$(subst .,_,$(C++COMPILER))

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += +w
endif

ifeq "$(ENV_OODBMS)" "OSTORE"
override INTERNAL_C++FLAGS += -vdelx
endif

endif

# Sun C++ Version 5.X

ifneq "$(filter SUN5.%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_o_for_objects := YES
_makeit_cxx_supports_ansi_exceptions := YES

ifeq "$(filter -compat -compat=4,$($(C++COMPILER)_C++FLAGS))" ""
override INTERNAL_C++FLAGS += -DHAVE_BOOL
override INTERNAL_C++FLAGS += -DENV_CXXSTDLIB -DENV_CXXSTDTPL
endif

override INTERNAL_C++FLAGS += -DCXX_SUN -DCXX_$(subst .,_,$(C++COMPILER))

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
#override INTERNAL_C++FLAGS += +w
endif

ifneq "$(filter -compat -compat=4,$($(C++COMPILER)_C++FLAGS))" ""
ifeq "$(ENV_OODBMS)" "OSTORE"
override INTERNAL_C++FLAGS += -vdelx
endif
endif

endif

# SCO UDK C++

ifneq "$(filter UDK%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_combine_compile_and_link := YES
_makeit_cxx_supports_precompiled_templates := YES

override INTERNAL_C++FLAGS += -DCXX_UDK
override INTERNAL_C++FLAGS += -DENV_CXXSTDTPL
override INTERNAL_C++FLAGS += -DHAVE_BOOL -DHAVE_EXCEPTIONS

endif

# XL C++

ifneq "$(filter XL%,$(C++COMPILER))" ""

_makeit_cxx_valid_compiler_tag := YES

_makeit_cxx_supports_ansi_exceptions := YES
#_makeit_cxx_supports_precompiled_templates := YES
_makeit_cxx_supports_xl_repositories := YES
# _makeit_cxx_supports_direct_instantiation := YES

override INTERNAL_C++FLAGS += -+ -qlanglvl=compat
override INTERNAL_C++FLAGS += -DCXX_XL -DCXX_$(subst .,_,$(C++COMPILER))

ifeq "$(filter no_compiler_warnings,$(MAKEIT_OPTIONS))" ""
override INTERNAL_C++FLAGS += -qflag=w:w
endif

endif

##
## Check if valid C++ compiler tag was provided.
##

ifneq "$(_makeit_cxx_valid_compiler_tag)" "YES"

override C++ := exit 1; *** invalid compiler tag ***
override CC := exit 1; *** invalid compiler tag ***
override LD := exit 1; *** invalid compiler tag ***

endif

##
## Check for special environments.
##

ifeq "$(ENV_OODBMS)" "OSTORE"
_makeit_cleanup_objectstore := YES
ifeq "$(ENV_SYSTEM)" "WIN32"
override OS_ROOTDIR := $(subst \,/,$(OS_ROOTDIR))
endif
override INTERNAL_CPPFLAGS += -I$(OS_ROOTDIR)/include
override LD_RUN_PATH := $(LD_RUN_PATH):$(OS_ROOTDIR)/lib
override INTERNAL_LDFLAGS += $(LOPT)$(OS_ROOTDIR)/lib
endif

ifeq "$(ENV_OODBMS)" "VERSANT"
override INTERNAL_CPPFLAGS += -I$(VPP_RELEASE)/h
override LD_RUN_PATH := $(LD_RUN_PATH):$(VPP_RELEASE)/lib
override INTERNAL_LDFLAGS += $(LOPT)$(VPP_RELEASE)/lib
override INTERNAL_LDLIBS += -lcxxcls
ifneq "$(filter dual_processes,$(VERSANT_OPTIONS))" ""
override INTERNAL_LDLIBS += -loscfe
else
override INTERNAL_LDLIBS += -losc
endif
endif

## Define HAVE_EXCEPTIONS if C++ compiler has ANSI exceptions. Note, they
## must include terminate() and set_terminate() functions.

ifeq "$(_makeit_cxx_supports_ansi_exceptions)" "YES"
override INTERNAL_C++FLAGS += -DHAVE_EXCEPTIONS
endif
