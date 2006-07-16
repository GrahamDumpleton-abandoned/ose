#
# General definitions.
#

# System type.

ENV_SYSTEM = UNIX

# Installation directory.

#OSE_ROOT_DIRECTORY = /usr/local/ose
#OSE_RELEASE_DIRECTORY = tst

# Default and desired C++ compilers.

DEFAULTC++COMPILER = PTHREAD-GNU3.3
C++COMPILERS = PTHREAD-GNU3.3

# Default and desired compilation variants.

DEFAULTVARIANT = dbg
VARIANTS = dbg opt std prf

#
# Default library management definitions.
#

define DEFAULT_SYM_CLEANUP
$(RM) __.SYMDEF*
endef

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

RANLIB = : /usr/bin/ranlib

#
# Compiler definitions.
#

# GNU C++ 2.95.2.

ifeq "$(C++COMPILERENV)" ""

GNU2.95.2_VARIANTS = dbg opt std
GNU2.95.2_PIC_VARIANTS = opt std

GNU2.95.2_ENV_OODBMS =

GNU2.95.2_OPT_LIBRARIES =

#GNU2.95.2_C++ = /usr/bin/c++
GNU2.95.2_C++ = /usr/bin/g++2
GNU2.95.2_CC = /usr/bin/cc
GNU2.95.2_LD = /usr/bin/ld
GNU2.95.2_AR = /usr/bin/ar

GNU2.95.2_CPPFLAGS =
GNU2.95.2_CFLAGS = -fno-common
GNU2.95.2_C++FLAGS = -fno-common
GNU2.95.2_LDFLAGS =
GNU2.95.2_LDLIBS = -lm

GNU2.95.2_PIC_CPPFLAGS =
GNU2.95.2_PIC_CFLAGS =
GNU2.95.2_PIC_C++FLAGS =
GNU2.95.2_PIC_LDFLAGS =
GNU2.95.2_PIC_LDLIBS =

GNU2.95.2_CPPFLAGS_dbg =
GNU2.95.2_CFLAGS_dbg = -g
GNU2.95.2_C++FLAGS_dbg = -g
GNU2.95.2_LDFLAGS_dbg =
GNU2.95.2_LDLIBS_dbg =

GNU2.95.2_CPPFLAGS_opt = -DNDEBUG
GNU2.95.2_CFLAGS_opt = -O
GNU2.95.2_C++FLAGS_opt = -O
GNU2.95.2_LDFLAGS_opt =
GNU2.95.2_LDLIBS_opt =

GNU2.95.2_CPPFLAGS_prf =
GNU2.95.2_CFLAGS_prf = -pg
GNU2.95.2_C++FLAGS_prf = -pg
GNU2.95.2_LDFLAGS_prf =
GNU2.95.2_LDLIBS_prf =

GNU2.95.2_CPPFLAGS_std =
GNU2.95.2_CFLAGS_std =
GNU2.95.2_C++FLAGS_std =
GNU2.95.2_LDFLAGS_std =
GNU2.95.2_LDLIBS_std =

define GNU2.95.2_PIC_LIBRARY
$(C++) -dynamiclib -o @LIB@ -install_name $(LIBDIR)/@LIBFILE@ \
 -current_version $(LIBVERSION) $(PIC_LDFLAGS) @LDFLAGS@ \
 @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

define GNU2.95.2_PIC_OBJECT
$(C++) -bundle -flat_namespace -undefined suppress -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

ifeq "$(C++COMPILERENV)" "PTHREAD"

GNU2.95.2_VARIANTS = dbg opt std
GNU2.95.2_PIC_VARIANTS = opt std

GNU2.95.2_ENV_OODBMS =

GNU2.95.2_OPT_LIBRARIES =

#GNU2.95.2_C++ = /usr/bin/c++
GNU2.95.2_C++ = /usr/bin/g++2
GNU2.95.2_CC = /usr/bin/cc
GNU2.95.2_LD = /usr/bin/ld
GNU2.95.2_AR = /usr/bin/ar

GNU2.95.2_CPPFLAGS = -DHAVE_POSIX_THREADS -D__RENTRANT
GNU2.95.2_CFLAGS = -fno-common
GNU2.95.2_C++FLAGS = -fno-common
GNU2.95.2_LDFLAGS =
GNU2.95.2_LDLIBS = -lm -lpthread

GNU2.95.2_PIC_CPPFLAGS =
GNU2.95.2_PIC_CFLAGS =
GNU2.95.2_PIC_C++FLAGS =
GNU2.95.2_PIC_LDFLAGS =
GNU2.95.2_PIC_LDLIBS =

GNU2.95.2_CPPFLAGS_dbg =
GNU2.95.2_CFLAGS_dbg = -g
GNU2.95.2_C++FLAGS_dbg = -g
GNU2.95.2_LDFLAGS_dbg =
GNU2.95.2_LDLIBS_dbg =

GNU2.95.2_CPPFLAGS_opt = -DNDEBUG
GNU2.95.2_CFLAGS_opt = -O
GNU2.95.2_C++FLAGS_opt = -O
GNU2.95.2_LDFLAGS_opt =
GNU2.95.2_LDLIBS_opt =

GNU2.95.2_CPPFLAGS_prf =
GNU2.95.2_CFLAGS_prf = -pg
GNU2.95.2_C++FLAGS_prf = -pg
GNU2.95.2_LDFLAGS_prf =
GNU2.95.2_LDLIBS_prf =

GNU2.95.2_CPPFLAGS_std =
GNU2.95.2_CFLAGS_std =
GNU2.95.2_C++FLAGS_std =
GNU2.95.2_LDFLAGS_std =
GNU2.95.2_LDLIBS_std =

define GNU2.95.2_PIC_LIBRARY
$(C++) -dynamiclib -o @LIB@ -install_name $(LIBDIR)/@LIBFILE@ \
 -current_version $(LIBVERSION) $(PIC_LDFLAGS) @LDFLAGS@ \
 @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

define GNU2.95.2_PIC_OBJECT
$(C++) -bundle -flat_namespace -undefined suppress -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

# GNU C++ 3.1.

ifeq "$(C++COMPILERENV)" ""

GNU3.1_VARIANTS = dbg opt std prf
GNU3.1_PIC_VARIANTS = opt std

GNU3.1_ENV_OODBMS =

GNU3.1_OPT_LIBRARIES =

#GNU3.1_C++ = /usr/bin/c++
GNU3.1_C++ = /usr/bin/g++3
GNU3.1_CC = /usr/bin/cc
GNU3.1_LD = /usr/bin/ld
GNU3.1_AR = /usr/bin/ar

GNU3.1_CPPFLAGS =
GNU3.1_CFLAGS = -fno-common
GNU3.1_C++FLAGS = -fno-common
GNU3.1_LDFLAGS =
GNU3.1_LDLIBS = -lm

GNU3.1_PIC_CPPFLAGS =
GNU3.1_PIC_CFLAGS =
GNU3.1_PIC_C++FLAGS =
GNU3.1_PIC_LDFLAGS =
GNU3.1_PIC_LDLIBS =

GNU3.1_CPPFLAGS_dbg =
GNU3.1_CFLAGS_dbg = -g
GNU3.1_C++FLAGS_dbg = -g
GNU3.1_LDFLAGS_dbg =
GNU3.1_LDLIBS_dbg =

GNU3.1_CPPFLAGS_opt = -DNDEBUG
GNU3.1_CFLAGS_opt = -O
GNU3.1_C++FLAGS_opt = -O
GNU3.1_LDFLAGS_opt =
GNU3.1_LDLIBS_opt =

GNU3.1_CPPFLAGS_prf =
GNU3.1_CFLAGS_prf = -pg
GNU3.1_C++FLAGS_prf = -pg
GNU3.1_LDFLAGS_prf =
GNU3.1_LDLIBS_prf =

GNU3.1_CPPFLAGS_std =
GNU3.1_CFLAGS_std =
GNU3.1_C++FLAGS_std =
GNU3.1_LDFLAGS_std =
GNU3.1_LDLIBS_std =

define GNU3.1_PIC_LIBRARY
$(C++) -dynamiclib -o @LIB@ -install_name $(LIBDIR)/@LIBFILE@ \
 -current_version $(LIBVERSION) $(PIC_LDFLAGS) @LDFLAGS@ \
 @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

define GNU3.1_PIC_OBJECT
$(C++) -bundle -flat_namespace -undefined suppress -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

ifeq "$(C++COMPILERENV)" "PTHREAD"

GNU3.1_VARIANTS = dbg opt std prf
GNU3.1_PIC_VARIANTS = opt std

GNU3.1_ENV_OODBMS =

GNU3.1_OPT_LIBRARIES =

#GNU3.1_C++ = /usr/bin/c++
GNU3.1_C++ = /usr/bin/g++3
GNU3.1_CC = /usr/bin/cc
GNU3.1_LD = /usr/bin/ld
GNU3.1_AR = /usr/bin/ar

GNU3.1_CPPFLAGS = -DHAVE_POSIX_THREADS -D__RENTRANT
GNU3.1_CFLAGS = -fno-common
GNU3.1_C++FLAGS = -fno-common
GNU3.1_LDFLAGS =
GNU3.1_LDLIBS = -lm -lpthread

GNU3.1_PIC_CPPFLAGS =
GNU3.1_PIC_CFLAGS =
GNU3.1_PIC_C++FLAGS =
GNU3.1_PIC_LDFLAGS =
GNU3.1_PIC_LDLIBS =

GNU3.1_CPPFLAGS_dbg =
GNU3.1_CFLAGS_dbg = -g
GNU3.1_C++FLAGS_dbg = -g
GNU3.1_LDFLAGS_dbg =
GNU3.1_LDLIBS_dbg =

GNU3.1_CPPFLAGS_opt = -DNDEBUG
GNU3.1_CFLAGS_opt = -O
GNU3.1_C++FLAGS_opt = -O
GNU3.1_LDFLAGS_opt =
GNU3.1_LDLIBS_opt =

GNU3.1_CPPFLAGS_prf =
GNU3.1_CFLAGS_prf = -pg
GNU3.1_C++FLAGS_prf = -pg
GNU3.1_LDFLAGS_prf =
GNU3.1_LDLIBS_prf =

GNU3.1_CPPFLAGS_std =
GNU3.1_CFLAGS_std =
GNU3.1_C++FLAGS_std =
GNU3.1_LDFLAGS_std =
GNU3.1_LDLIBS_std =

define GNU3.1_PIC_LIBRARY
$(C++) -dynamiclib -o @LIB@ -install_name $(LIBDIR)/@LIBFILE@ \
 -current_version $(LIBVERSION) $(PIC_LDFLAGS) @LDFLAGS@ \
 @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

define GNU3.1_PIC_OBJECT
$(C++) -bundle -flat_namespace -undefined suppress -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

# GNU C++ 3.3.

ifeq "$(C++COMPILERENV)" ""

GNU3.3_VARIANTS = dbg opt std prf
GNU3.3_PIC_VARIANTS = opt std

GNU3.3_ENV_OODBMS =

GNU3.3_OPT_LIBRARIES =

GNU3.3_C++ = /usr/bin/g++-3.3
GNU3.3_CC = /usr/bin/gcc-3.3
GNU3.3_LD = /usr/bin/ld
GNU3.3_AR = /usr/bin/ar

GNU3.3_CPPFLAGS =
GNU3.3_CFLAGS = -fno-common
GNU3.3_C++FLAGS = -fno-common
GNU3.3_LDFLAGS =
GNU3.3_LDLIBS = -lm

GNU3.3_PIC_CPPFLAGS =
GNU3.3_PIC_CFLAGS =
GNU3.3_PIC_C++FLAGS =
GNU3.3_PIC_LDFLAGS =
GNU3.3_PIC_LDLIBS =

GNU3.3_CPPFLAGS_dbg =
GNU3.3_CFLAGS_dbg = -g
GNU3.3_C++FLAGS_dbg = -g
GNU3.3_LDFLAGS_dbg =
GNU3.3_LDLIBS_dbg =

GNU3.3_CPPFLAGS_opt = -DNDEBUG
GNU3.3_CFLAGS_opt = -O
GNU3.3_C++FLAGS_opt = -O
GNU3.3_LDFLAGS_opt =
GNU3.3_LDLIBS_opt =

GNU3.3_CPPFLAGS_prf =
GNU3.3_CFLAGS_prf = -pg
GNU3.3_C++FLAGS_prf = -pg
GNU3.3_LDFLAGS_prf =
GNU3.3_LDLIBS_prf =

GNU3.3_CPPFLAGS_std =
GNU3.3_CFLAGS_std =
GNU3.3_C++FLAGS_std =
GNU3.3_LDFLAGS_std =
GNU3.3_LDLIBS_std =

define GNU3.3_PIC_LIBRARY
$(C++) -dynamiclib -o @LIB@ -install_name $(LIBDIR)/@LIBFILE@ \
 -current_version $(LIBVERSION) $(PIC_LDFLAGS) @LDFLAGS@ \
 @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

define GNU3.3_PIC_OBJECT
$(C++) -bundle -flat_namespace -undefined suppress -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

ifeq "$(C++COMPILERENV)" "PTHREAD"

GNU3.3_VARIANTS = dbg opt std prf
GNU3.3_PIC_VARIANTS = opt std

GNU3.3_ENV_OODBMS =

GNU3.3_OPT_LIBRARIES =

GNU3.3_C++ = /usr/bin/g++-3.3
GNU3.3_CC = /usr/bin/gcc-3.3
GNU3.3_LD = /usr/bin/ld
GNU3.3_AR = /usr/bin/ar

GNU3.3_CPPFLAGS = -DHAVE_POSIX_THREADS -D__RENTRANT
GNU3.3_CFLAGS = -fno-common
GNU3.3_C++FLAGS = -fno-common
GNU3.3_LDFLAGS =
GNU3.3_LDLIBS = -lm -lpthread

GNU3.3_PIC_CPPFLAGS =
GNU3.3_PIC_CFLAGS =
GNU3.3_PIC_C++FLAGS =
GNU3.3_PIC_LDFLAGS =
GNU3.3_PIC_LDLIBS =

GNU3.3_CPPFLAGS_dbg =
GNU3.3_CFLAGS_dbg = -g
GNU3.3_C++FLAGS_dbg = -g
GNU3.3_LDFLAGS_dbg =
GNU3.3_LDLIBS_dbg =

GNU3.3_CPPFLAGS_opt = -DNDEBUG
GNU3.3_CFLAGS_opt = -O
GNU3.3_C++FLAGS_opt = -O
GNU3.3_LDFLAGS_opt =
GNU3.3_LDLIBS_opt =

GNU3.3_CPPFLAGS_prf =
GNU3.3_CFLAGS_prf = -pg
GNU3.3_C++FLAGS_prf = -pg
GNU3.3_LDFLAGS_prf =
GNU3.3_LDLIBS_prf =

GNU3.3_CPPFLAGS_std =
GNU3.3_CFLAGS_std =
GNU3.3_C++FLAGS_std =
GNU3.3_LDFLAGS_std =
GNU3.3_LDLIBS_std =

define GNU3.3_PIC_LIBRARY
$(C++) -dynamiclib -o @LIB@ -install_name $(LIBDIR)/@LIBFILE@ \
 -current_version $(LIBVERSION) $(PIC_LDFLAGS) @LDFLAGS@ \
 @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

define GNU3.3_PIC_OBJECT
$(C++) -bundle -flat_namespace -undefined suppress -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif
