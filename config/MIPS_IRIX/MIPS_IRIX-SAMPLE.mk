#
# General definitions.
#

# System type.

ENV_SYSTEM = UNIX

# Installation directory.

#OSE_ROOT_DIRECTORY = /usr/local/ose
#OSE_RELEASE_DIRECTORY = tst

# Default and desired C++ compilers.

DEFAULTC++COMPILER = SG7.1
C++COMPILERS = SG7.1 N32ABI-SG7.1 64ABI-SG7.1

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
#
# Not that due to a compiler bug related to template instantiation.
#
#   SGI C++ 7.0 cannot be used.
#   SGI C++ 7.1 can only be used if patchSG0001985 from SGI applied.
#   SGI C++ 7.2 should be okay.
#

# GNU C++ 2.7.2 - USE C++COMPILER=GNU2.7.2

ifeq "$(C++COMPILERENV)" ""

GNU2.7.2_VARIANTS = dbg opt prf std
GNU2.7.2_PIC_VARIANTS = opt std

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
GNU2.7.2_PIC_CFLAGS =
GNU2.7.2_PIC_C++FLAGS =
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

# SGI C++ 3.0 (CFRONT) - USE C++COMPILER=SG3.0

ifeq "$(C++COMPILERENV)" ""

SG3.0_VARIANTS = dbg opt prf std
SG3.0_PIC_VARIANTS = opt std

SG3.0_ENV_OODBMS =

SG3.0_OPT_LIBRARIES =

SG3.0_C++ = /usr/bin/OCC
SG3.0_CC = /usr/bin/cc
SG3.0_LD = /usr/bin/ld
SG3.0_AR = /usr/bin/ar

SG3.0_CPPFLAGS =
SG3.0_CFLAGS =
SG3.0_C++FLAGS =
SG3.0_LDFLAGS =
SG3.0_LDLIBS =

SG3.0_PIC_CPPFLAGS =
SG3.0_PIC_CFLAGS =
SG3.0_PIC_C++FLAGS =
SG3.0_PIC_LDFLAGS =
SG3.0_PIC_LDLIBS =

SG3.0_CPPFLAGS_dbg =
SG3.0_CFLAGS_dbg = -g
SG3.0_C++FLAGS_dbg = -g
SG3.0_LDFLAGS_dbg =
SG3.0_LDLIBS_dbg =

SG3.0_CPPFLAGS_opt = -DNDEBUG
SG3.0_CFLAGS_opt = -O
SG3.0_C++FLAGS_opt = -O
SG3.0_LDFLAGS_opt =
SG3.0_LDLIBS_opt =

SG3.0_CPPFLAGS_prf =
SG3.0_CFLAGS_prf = -p
SG3.0_C++FLAGS_prf = -p
SG3.0_LDFLAGS_prf =
SG3.0_LDLIBS_prf =

SG3.0_CPPFLAGS_std =
SG3.0_CFLAGS_std =
SG3.0_C++FLAGS_std =
SG3.0_LDFLAGS_std =
SG3.0_LDLIBS_std =

define SG3.0_PIC_LIBRARY
$(C++) -shared -soname lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

# SGI C++ 4.0 (EDG) - USE C++COMPILER=SG4.0

ifeq "$(C++COMPILERENV)" ""

SG4.0_VARIANTS = dbg opt prf std
SG4.0_PIC_VARIANTS = opt std

SG4.0_ENV_OODBMS =

SG4.0_OPT_LIBRARIES =

SG4.0_C++ = /usr/bin/CC
SG4.0_CC = /usr/bin/cc
SG4.0_LD = /usr/bin/ld
SG4.0_AR = /usr/bin/ar

SG4.0_CPPFLAGS =
SG4.0_CFLAGS =
SG4.0_C++FLAGS =
SG4.0_LDFLAGS =
SG4.0_LDLIBS =

SG4.0_PIC_CPPFLAGS =
SG4.0_PIC_CFLAGS =
SG4.0_PIC_C++FLAGS =
SG4.0_PIC_LDFLAGS =
SG4.0_PIC_LDLIBS =

SG4.0_CPPFLAGS_dbg =
SG4.0_CFLAGS_dbg = -g
SG4.0_C++FLAGS_dbg = -g
SG4.0_LDFLAGS_dbg =
SG4.0_LDLIBS_dbg =

SG4.0_CPPFLAGS_opt = -DNDEBUG
SG4.0_CFLAGS_opt = -O
SG4.0_C++FLAGS_opt = -O
SG4.0_LDFLAGS_opt =
SG4.0_LDLIBS_opt =

SG4.0_CPPFLAGS_prf =
SG4.0_CFLAGS_prf = -p
SG4.0_C++FLAGS_prf = -p
SG4.0_LDFLAGS_prf =
SG4.0_LDLIBS_prf =

SG4.0_CPPFLAGS_std =
SG4.0_CFLAGS_std =
SG4.0_C++FLAGS_std =
SG4.0_LDFLAGS_std =
SG4.0_LDLIBS_std =

define SG4.0_PIC_LIBRARY
$(C++) -shared -soname lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

# SGI C++ 7.1 (32 BIT MODE) - USE C++COMPILER=SG7.1

ifeq "$(C++COMPILERENV)" ""

SG7.1_VARIANTS = dbg opt prf std
SG7.1_PIC_VARIANTS = opt std

SG7.1_ENV_OODBMS =

SG7.1_OPT_LIBRARIES =

SG7.1_C++ = /usr/bin/CC
SG7.1_CC = /usr/bin/cc
SG7.1_LD = /usr/bin/ld
SG7.1_AR = /usr/bin/ar

SG7.1_CPPFLAGS =
SG7.1_CFLAGS = -32
SG7.1_C++FLAGS = -32
SG7.1_LDFLAGS = -32
SG7.1_LDLIBS =

SG7.1_PIC_CPPFLAGS =
SG7.1_PIC_CFLAGS =
SG7.1_PIC_C++FLAGS =
SG7.1_PIC_LDFLAGS = -32
SG7.1_PIC_LDLIBS =

SG7.1_CPPFLAGS_dbg =
SG7.1_CFLAGS_dbg = -g
SG7.1_C++FLAGS_dbg = -g
SG7.1_LDFLAGS_dbg =
SG7.1_LDLIBS_dbg =

SG7.1_CPPFLAGS_opt = -DNDEBUG
SG7.1_CFLAGS_opt = -O
SG7.1_C++FLAGS_opt = -O
SG7.1_LDFLAGS_opt =
SG7.1_LDLIBS_opt =

SG7.1_CPPFLAGS_prf =
SG7.1_CFLAGS_prf = -p
SG7.1_C++FLAGS_prf = -p
SG7.1_LDFLAGS_prf =
SG7.1_LDLIBS_prf =

SG7.1_CPPFLAGS_std =
SG7.1_CFLAGS_std =
SG7.1_C++FLAGS_std =
SG7.1_LDFLAGS_std =
SG7.1_LDLIBS_std =

define SG7.1_PIC_LIBRARY
$(C++) -shared -soname lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

# SGI C++ 7.1 (N32 BIT MODE) - USE C++COMPILER=N32ABI-SG7.1

ifeq "$(C++COMPILERENV)" "N32ABI"

SG7.1_VARIANTS = dbg opt prf std
SG7.1_PIC_VARIANTS = opt std

SG7.1_ENV_OODBMS =

SG7.1_OPT_LIBRARIES =

SG7.1_C++ = /usr/bin/CC
SG7.1_CC = /usr/bin/cc
SG7.1_LD = /usr/bin/ld
SG7.1_AR = /usr/bin/ar

SG7.1_CPPFLAGS =
SG7.1_CFLAGS = -n32
SG7.1_C++FLAGS = -n32
SG7.1_LDFLAGS = -n32
SG7.1_LDLIBS =

SG7.1_PIC_CPPFLAGS =
SG7.1_PIC_CFLAGS =
SG7.1_PIC_C++FLAGS =
SG7.1_PIC_LDFLAGS = -n32
SG7.1_PIC_LDLIBS =

SG7.1_CPPFLAGS_dbg =
SG7.1_CFLAGS_dbg = -g
SG7.1_C++FLAGS_dbg = -g
SG7.1_LDFLAGS_dbg =
SG7.1_LDLIBS_dbg =

SG7.1_CPPFLAGS_opt = -DNDEBUG
SG7.1_CFLAGS_opt = -O
SG7.1_C++FLAGS_opt = -O
SG7.1_LDFLAGS_opt =
SG7.1_LDLIBS_opt =

SG7.1_CPPFLAGS_prf =
SG7.1_CFLAGS_prf = -p
SG7.1_C++FLAGS_prf = -p
SG7.1_LDFLAGS_prf =
SG7.1_LDLIBS_prf =

SG7.1_CPPFLAGS_std =
SG7.1_CFLAGS_std =
SG7.1_C++FLAGS_std =
SG7.1_LDFLAGS_std =
SG7.1_LDLIBS_std =

define SG7.1_PIC_LIBRARY
$(C++) -shared -soname lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

# SGI C++ 7.1 (64 BIT MODE) - USE C++COMPILER=64ABI-SG7.1

ifeq "$(C++COMPILERENV)" "64ABI"

SG7.1_VARIANTS = dbg opt prf std
SG7.1_PIC_VARIANTS = opt std

SG7.1_ENV_OODBMS =

SG7.1_OPT_LIBRARIES =

SG7.1_C++ = /usr/bin/CC
SG7.1_CC = /usr/bin/cc
SG7.1_LD = /usr/bin/ld
SG7.1_AR = /usr/bin/ar

SG7.1_CPPFLAGS =
SG7.1_CFLAGS = -64
SG7.1_C++FLAGS = -64
SG7.1_LDFLAGS = -64
SG7.1_LDLIBS =

SG7.1_PIC_CPPFLAGS =
SG7.1_PIC_CFLAGS =
SG7.1_PIC_C++FLAGS =
SG7.1_PIC_LDFLAGS = -64
SG7.1_PIC_LDLIBS =

SG7.1_CPPFLAGS_dbg =
SG7.1_CFLAGS_dbg = -g
SG7.1_C++FLAGS_dbg = -g
SG7.1_LDFLAGS_dbg =
SG7.1_LDLIBS_dbg =

SG7.1_CPPFLAGS_opt = -DNDEBUG
SG7.1_CFLAGS_opt = -O
SG7.1_C++FLAGS_opt = -O
SG7.1_LDFLAGS_opt =
SG7.1_LDLIBS_opt =

SG7.1_CPPFLAGS_prf =
SG7.1_CFLAGS_prf = -p
SG7.1_C++FLAGS_prf = -p
SG7.1_LDFLAGS_prf =
SG7.1_LDLIBS_prf =

SG7.1_CPPFLAGS_std =
SG7.1_CFLAGS_std =
SG7.1_C++FLAGS_std =
SG7.1_LDFLAGS_std =
SG7.1_LDLIBS_std =

define SG7.1_PIC_LIBRARY
$(C++) -shared -soname lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

# SGI DELTA C++ 1.0 - USE C++COMPILER=SGD1.0

ifeq "$(C++COMPILERENV)" ""

SGD1.0_VARIANTS = dbg opt prf std
SGD1.0_PIC_VARIANTS = opt std

SGD1.0_ENV_OODBMS =

SGD1.0_OPT_LIBRARIES =

SGD1.0_C++ = /usr/bin/DCC
SGD1.0_CC = /usr/bin/cc
SGD1.0_LD = /usr/bin/ld
SGD1.0_AR = /usr/bin/ar

SGD1.0_CPPFLAGS =
SGD1.0_CFLAGS =
SGD1.0_C++FLAGS =
SGD1.0_LDFLAGS =
SGD1.0_LDLIBS =

SGD1.0_PIC_CPPFLAGS =
SGD1.0_PIC_CFLAGS =
SGD1.0_PIC_C++FLAGS =
SGD1.0_PIC_LDFLAGS =
SGD1.0_PIC_LDLIBS =

SGD1.0_CPPFLAGS_dbg =
SGD1.0_CFLAGS_dbg = -g
SGD1.0_C++FLAGS_dbg = -g
SGD1.0_LDFLAGS_dbg =
SGD1.0_LDLIBS_dbg =

SGD1.0_CPPFLAGS_opt = -DNDEBUG
SGD1.0_CFLAGS_opt = -O
SGD1.0_C++FLAGS_opt = -O
SGD1.0_LDFLAGS_opt =
SGD1.0_LDLIBS_opt =

SGD1.0_CPPFLAGS_prf =
SGD1.0_CFLAGS_prf = -p
SGD1.0_C++FLAGS_prf = -p
SGD1.0_LDFLAGS_prf =
SGD1.0_LDLIBS_prf =

SGD1.0_CPPFLAGS_std =
SGD1.0_CFLAGS_std =
SGD1.0_C++FLAGS_std =
SGD1.0_LDFLAGS_std =
SGD1.0_LDLIBS_std =

define SGD1.0_PIC_LIBRARY
$(C++) -shared -soname lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif
