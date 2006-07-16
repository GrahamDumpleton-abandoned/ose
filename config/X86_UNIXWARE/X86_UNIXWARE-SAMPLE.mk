#
# General definitions.
#

# System type.

ENV_SYSTEM = UNIX

# Installation directory.

#OSE_ROOT_DIRECTORY = /usr/local/ose
#OSE_RELEASE_DIRECTORY = tst

# Default and desired C++ compilers.

DEFAULTC++COMPILER = NOV2.0
C++COMPILERS = NOV2.0

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
# Compiler definitions.
#

# GNU C++ 2.7.2.

ifeq "$(C++COMPILERENV)" ""

GNU2.7.2_VARIANTS = dbg opt prf std
GNU2.7.2_PIC_VARIANTS =

GNU2.7.2_ENV_OODBMS =

GNU2.7.2_OPT_LIBRARIES =

GNU2.7.2_C++ = /usr/local/bin/g++
GNU2.7.2_CC = /usr/local/bin/gcc
GNU2.7.2_LD = /usr/ccs/bin/ld
GNU2.7.2_AR = /usr/ccs/bin/ar

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

define GNU2.7.2_PIC_LIBRARY
$(C++) -shared -h lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

# Novell C++ 2.0.

ifeq "$(C++COMPILERENV)" ""

NOV2.0_VARIANTS = dbg opt prf std
NOV2.0_PIC_VARIANTS = std opt

NOV2.0_ENV_OODBMS =

NOV2.0_OPT_LIBRARIES =

NOV2.0_C++ = /usr/ccs/bin/CC
NOV2.0_CC = /usr/ccs/bin/cc
NOV2.0_LD = /usr/ccs/bin/ld
NOV2.0_AR = /usr/ccs/bin/ar

NOV2.0_CPPFLAGS =
NOV2.0_CFLAGS =
NOV2.0_C++FLAGS =
NOV2.0_LDFLAGS =
NOV2.0_LDLIBS =

NOV2.0_PIC_CPPFLAGS =
NOV2.0_PIC_CFLAGS = -K pic
NOV2.0_PIC_C++FLAGS = -K pic
NOV2.0_PIC_LDFLAGS =
NOV2.0_PIC_LDLIBS =

NOV2.0_CPPFLAGS_dbg =
NOV2.0_CFLAGS_dbg = -g
NOV2.0_C++FLAGS_dbg = -g
NOV2.0_LDFLAGS_dbg =
NOV2.0_LDLIBS_dbg =

NOV2.0_CPPFLAGS_opt = -DNDEBUG
NOV2.0_CFLAGS_opt = -O
NOV2.0_C++FLAGS_opt = -O
NOV2.0_LDFLAGS_opt =
NOV2.0_LDLIBS_opt =

NOV2.0_CPPFLAGS_prf =
NOV2.0_CFLAGS_prf = -pg
NOV2.0_C++FLAGS_prf = -pg
NOV2.0_LDFLAGS_prf =
NOV2.0_LDLIBS_prf =

NOV2.0_CPPFLAGS_std =
NOV2.0_CFLAGS_std =
NOV2.0_C++FLAGS_std =
NOV2.0_LDFLAGS_std =
NOV2.0_LDLIBS_std =

define NOV2.0_PIC_LIBRARY
$(C++) -G -o @LIB@ $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

# SCO UDK C++ 3.4.

ifeq "$(C++COMPILERENV)" ""

UDK3.4_VARIANTS = dbg opt prf std
UDK3.4_PIC_VARIANTS = std opt

UDK3.4_ENV_OODBMS =

UDK3.4_OPT_LIBRARIES =

UDK3.4_C++ = /usr/ccs/bin/CC
UDK3.4_CC = /usr/ccs/bin/cc
UDK3.4_LD = /usr/ccs/bin/ld
UDK3.4_AR = /usr/ccs/bin/ar

UDK3.4_CPPFLAGS =
UDK3.4_CFLAGS =
UDK3.4_C++FLAGS =
UDK3.4_LDFLAGS =
UDK3.4_LDLIBS = -lsocket

UDK3.4_PIC_CPPFLAGS =
UDK3.4_PIC_CFLAGS = -K pic
UDK3.4_PIC_C++FLAGS = -K pic
UDK3.4_PIC_LDFLAGS =
UDK3.4_PIC_LDLIBS =

UDK3.4_CPPFLAGS_dbg =
UDK3.4_CFLAGS_dbg = -g
UDK3.4_C++FLAGS_dbg = -g
UDK3.4_LDFLAGS_dbg =
UDK3.4_LDLIBS_dbg =

UDK3.4_CPPFLAGS_opt = -DNDEBUG
UDK3.4_CFLAGS_opt = -O
UDK3.4_C++FLAGS_opt = -O
UDK3.4_LDFLAGS_opt =
UDK3.4_LDLIBS_opt =

UDK3.4_CPPFLAGS_prf =
UDK3.4_CFLAGS_prf = -pg
UDK3.4_C++FLAGS_prf = -pg
UDK3.4_LDFLAGS_prf =
UDK3.4_LDLIBS_prf =

UDK3.4_CPPFLAGS_std =
UDK3.4_CFLAGS_std =
UDK3.4_C++FLAGS_std =
UDK3.4_LDFLAGS_std =
UDK3.4_LDLIBS_std =

define UDK3.4_PIC_LIBRARY
$(C++) -G -o @LIB@ $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif
