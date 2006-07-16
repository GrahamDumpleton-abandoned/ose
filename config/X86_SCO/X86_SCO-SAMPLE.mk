#
# General definitions.
#

# System type.

ENV_SYSTEM = UNIX

# Installation directory.

#OSE_ROOT_DIRECTORY = /usr/local/ose
#OSE_RELEASE_DIRECTORY = tst

# Default and desired C++ compilers.

DEFAULTC++COMPILER = ATT3.0.2
C++COMPILERS = ATT3.0.2

# Default and desired compilation variants.

DEFAULTVARIANT = dbg
VARIANTS = dbg opt prf std

#
# Default library management definitions.
#

define DEFAULT_LIB_CLOSURE
endef

define DEFAULT_LIB_ARCHIVE
$(AR) rv @LIB@ @OBJECTS@
endef

define DEFAULT_LIB_EXTRACT
$(AR) xv @LIB@
endef

define DEFAULT_PIC_CLOSURE
endef

define DEFAULT_PIC_ARCHIVE
$(AR) rv @LIB@ @OBJECTS@
endef

define DEFAULT_PIC_EXTRACT
$(AR) xv @LIB@
endef

define DEFAULT_PIC_LIBRARY
exit 1; *** do not know how to create shared library ***
endef

#
# Tool definitions.
#

RANLIB = : ranlib

#
# Compiler definitions.
#

# ATT C++ 3.0.1.

ifeq "$(C++COMPILERENV)" ""

ATT3.0.1_VARIANTS = dbg opt prf std
ATT3.0.1_PIC_VARIANTS =

ATT3.0.1_ENV_OODBMS =

ATT3.0.1_OPT_LIBRARIES =

ATT3.0.1_C++ = /usr/bin/CC
ATT3.0.1_CC = /bin/cc
ATT3.0.1_LD = /usr/bin/ld
ATT3.0.1_AR = /usr/bin/ar

ATT3.0.1_CPPFLAGS =
ATT3.0.1_CFLAGS =
ATT3.0.1_C++FLAGS = +p +.cc
ATT3.0.1_LDFLAGS =
ATT3.0.1_LDLIBS =

ATT3.0.1_PIC_CPPFLAGS =
ATT3.0.1_PIC_CFLAGS =
ATT3.0.1_PIC_C++FLAGS =
ATT3.0.1_PIC_LDFLAGS =
ATT3.0.1_PIC_LDLIBS =

ATT3.0.1_CPPFLAGS_dbg =
ATT3.0.1_CFLAGS_dbg = -g
ATT3.0.1_C++FLAGS_dbg = -g
ATT3.0.1_LDFLAGS_dbg =
ATT3.0.1_LDLIBS_dbg =

ATT3.0.1_CPPFLAGS_opt = -DNDEBUG
ATT3.0.1_CFLAGS_opt = -O
ATT3.0.1_C++FLAGS_opt = -O
ATT3.0.1_LDFLAGS_opt =
ATT3.0.1_LDLIBS_opt =

ATT3.0.1_CPPFLAGS_prf =
ATT3.0.1_CFLAGS_prf = -pg
ATT3.0.1_C++FLAGS_prf = -pg
ATT3.0.1_LDFLAGS_prf =
ATT3.0.1_LDLIBS_prf =

ATT3.0.1_CPPFLAGS_std =
ATT3.0.1_CFLAGS_std =
ATT3.0.1_C++FLAGS_std =
ATT3.0.1_LDFLAGS_std =
ATT3.0.1_LDLIBS_std =

endif

# ATT C++ 3.0.2.

ifeq "$(C++COMPILERENV)" ""

ATT3.0.2_VARIANTS = dbg opt prf std
ATT3.0.2_PIC_VARIANTS =

ATT3.0.2_ENV_OODBMS =

ATT3.0.2_OPT_LIBRARIES =

ATT3.0.2_C++ = /usr/bin/CC
ATT3.0.2_CC = /bin/cc
ATT3.0.2_LD = /usr/bin/ld
ATT3.0.2_AR = /usr/bin/ar

ATT3.0.2_CPPFLAGS =
ATT3.0.2_CFLAGS = -b coff -a xpg4plus
ATT3.0.2_C++FLAGS = +a1 +p -ptv +.cc
ATT3.0.2_LDFLAGS = -b coff
ATT3.0.2_LDLIBS =

ATT3.0.2_PIC_CPPFLAGS =
ATT3.0.2_PIC_CFLAGS =
ATT3.0.2_PIC_C++FLAGS =
ATT3.0.2_PIC_LDFLAGS =
ATT3.0.2_PIC_LDLIBS =

ATT3.0.2_CPPFLAGS_dbg =
ATT3.0.2_CFLAGS_dbg = -g
ATT3.0.2_C++FLAGS_dbg = -g
ATT3.0.2_LDFLAGS_dbg =
ATT3.0.2_LDLIBS_dbg =

ATT3.0.2_CPPFLAGS_opt = -DNDEBUG
ATT3.0.2_CFLAGS_opt = -O
ATT3.0.2_C++FLAGS_opt = -O
ATT3.0.2_LDFLAGS_opt =
ATT3.0.2_LDLIBS_opt =

ATT3.0.2_CPPFLAGS_prf =
ATT3.0.2_CFLAGS_prf = -pg
ATT3.0.2_C++FLAGS_prf = -pg
ATT3.0.2_LDFLAGS_prf =
ATT3.0.2_LDLIBS_prf =

ATT3.0.2_CPPFLAGS_std =
ATT3.0.2_CFLAGS_std =
ATT3.0.2_C++FLAGS_std =
ATT3.0.2_LDFLAGS_std =
ATT3.0.2_LDLIBS_std =

endif

# GNU C++ 2.7.2.

ifeq "$(C++COMPILERENV)" ""

GNU2.7.2_VARIANTS = dbg opt prf std
GNU2.7.2_PIC_VARIANTS =

GNU2.7.2_ENV_OODBMS =

GNU2.7.2_OPT_LIBRARIES =

GNU2.7.2_C++ = /usr/local/bin/g++
GNU2.7.2_CC = /usr/local/bin/gcc
GNU2.7.2_LD = /usr/bin/ld
GNU2.7.2_AR = /usr/bin/ar

GNU2.7.2_CPPFLAGS =
GNU2.7.2_CFLAGS = -V 2.7.2 -pipe
GNU2.7.2_C++FLAGS = -V 2.7.2 -pipe
GNU2.7.2_LDFLAGS =
GNU2.7.2_LDLIBS = -lm

GNU2.7.2_PIC_CPPFLAGS =
GNU2.7.2_PIC_CFLAGS = -fpic
GNU2.7.2_PIC_C++FLAGS = -fpic
GNU2.7.2_PIC_LDFLAGS =
GNU2.7.2_PIC_LDLIBS =

GNU2.7.2_CPPFLAGS_dbg =
GNU2.7.2_CFLAGS_dbg = -g
GNU2.7.2_C++FLAGS_dbg = -g
GNU2.7.2_LDFLAGS_dbg =
GNU2.7.2_LDLIBS_dbg =

GNU2.7.2_CPPFLAGS_opt = -DNDEBUG
GNU2.7.2_CFLAGS_opt = -O
GNU2.7.2_C++FLAGS_opt = -O
GNU2.7.2_LDFLAGS_opt =
GNU2.7.2_LDLIBS_opt =

GNU2.7.2_CPPFLAGS_prf =
GNU2.7.2_CFLAGS_prf = -pg
GNU2.7.2_C++FLAGS_prf = -pg
GNU2.7.2_LDFLAGS_prf =
GNU2.7.2_LDLIBS_prf =

GNU2.7.2_CPPFLAGS_std =
GNU2.7.2_CFLAGS_std =
GNU2.7.2_C++FLAGS_std =
GNU2.7.2_LDFLAGS_std =
GNU2.7.2_LDLIBS_std =

endif
