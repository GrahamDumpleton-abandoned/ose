#
# General definitions.
#

# System type.

ENV_SYSTEM = UNIX

# Installation directory.

#OSE_ROOT_DIRECTORY = /usr/local/ose
#OSE_RELEASE_DIRECTORY = tst

# Default and desired C++ compilers.

DEFAULTC++COMPILER = PTHREAD-GNU2.95.3
C++COMPILERS = PTHREAD-GNU2.95.3

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

GNU2.7.2_C++ = /usr/bin/g++
GNU2.7.2_CC = /usr/bin/gcc
GNU2.7.2_LD = /usr/bin/ld
GNU2.7.2_AR = /usr/bin/ar

GNU2.7.2_CPPFLAGS =
GNU2.7.2_CFLAGS = -pipe
GNU2.7.2_C++FLAGS = -pipe
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

endif

# GNU C++ 2.95.3.

ifeq "$(C++COMPILERENV)" ""

GNU2.95.3_VARIANTS = dbg opt prf std
GNU2.95.3_PIC_VARIANTS = opt std

GNU2.95.3_ENV_OODBMS =

GNU2.95.3_OPT_LIBRARIES =

GNU2.95.3_C++ = /usr/bin/g++
GNU2.95.3_CC = /usr/bin/gcc
GNU2.95.3_LD = /usr/bin/ld
GNU2.95.3_AR = /usr/bin/ar

GNU2.95.3_CPPFLAGS =
GNU2.95.3_CFLAGS = -pipe
GNU2.95.3_C++FLAGS = -pipe -Winline
GNU2.95.3_LDFLAGS =
GNU2.95.3_LDLIBS = -lm

GNU2.95.3_PIC_CPPFLAGS =
GNU2.95.3_PIC_CFLAGS =
GNU2.95.3_PIC_C++FLAGS =
GNU2.95.3_PIC_LDFLAGS =
GNU2.95.3_PIC_LDLIBS =

GNU2.95.3_CPPFLAGS_dbg =
GNU2.95.3_CFLAGS_dbg = -g
GNU2.95.3_C++FLAGS_dbg = -g
GNU2.95.3_LDFLAGS_dbg =
GNU2.95.3_LDLIBS_dbg =

GNU2.95.3_CPPFLAGS_opt = -DNDEBUG
GNU2.95.3_CFLAGS_opt = -O
GNU2.95.3_C++FLAGS_opt = -O
GNU2.95.3_LDFLAGS_opt =
GNU2.95.3_LDLIBS_opt =

GNU2.95.3_CPPFLAGS_prf =
GNU2.95.3_CFLAGS_prf = -pg
GNU2.95.3_C++FLAGS_prf = -pg
GNU2.95.3_LDFLAGS_prf =
GNU2.95.3_LDLIBS_prf =

GNU2.95.3_CPPFLAGS_std =
GNU2.95.3_CFLAGS_std =
GNU2.95.3_C++FLAGS_std =
GNU2.95.3_LDFLAGS_std =
GNU2.95.3_LDLIBS_std =

define GNU2.95.3_PIC_LIBRARY
$(C++) -shared -o @LIB@ $(PIC_LDFLAGS) \
 @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

ifeq "$(C++COMPILERENV)" "REPO"

GNU2.95.3_VARIANTS = dbg opt prf std
GNU2.95.3_PIC_VARIANTS = opt std

GNU2.95.3_ENV_OODBMS =

GNU2.95.3_OPT_LIBRARIES =

GNU2.95.3_C++ = /usr/bin/g++
GNU2.95.3_CC = /usr/bin/gcc
GNU2.95.3_LD = /usr/bin/ld
GNU2.95.3_AR = /usr/bin/ar

GNU2.95.3_CPPFLAGS =
GNU2.95.3_CFLAGS = -pipe
GNU2.95.3_C++FLAGS = -pipe -Winline -frepo
GNU2.95.3_LDFLAGS =
GNU2.95.3_LDLIBS = -lm

GNU2.95.3_PIC_CPPFLAGS =
GNU2.95.3_PIC_CFLAGS =
GNU2.95.3_PIC_C++FLAGS =
GNU2.95.3_PIC_LDFLAGS =
GNU2.95.3_PIC_LDLIBS =

GNU2.95.3_CPPFLAGS_dbg =
GNU2.95.3_CFLAGS_dbg = -g
GNU2.95.3_C++FLAGS_dbg = -g
GNU2.95.3_LDFLAGS_dbg =
GNU2.95.3_LDLIBS_dbg =

GNU2.95.3_CPPFLAGS_opt = -DNDEBUG
GNU2.95.3_CFLAGS_opt = -O
GNU2.95.3_C++FLAGS_opt = -O
GNU2.95.3_LDFLAGS_opt =
GNU2.95.3_LDLIBS_opt =

GNU2.95.3_CPPFLAGS_prf =
GNU2.95.3_CFLAGS_prf = -pg
GNU2.95.3_C++FLAGS_prf = -pg
GNU2.95.3_LDFLAGS_prf =
GNU2.95.3_LDLIBS_prf =

GNU2.95.3_CPPFLAGS_std =
GNU2.95.3_CFLAGS_std =
GNU2.95.3_C++FLAGS_std =
GNU2.95.3_LDFLAGS_std =
GNU2.95.3_LDLIBS_std =

define GNU2.95.3_LIB_CLOSURE
$(RM) $(MK)/__ptdummy0.cc; \
echo "int main() { return 0; }" > $(MK)/__ptdummy0.cc; \
$(C++) $(MK)/__ptdummy0.cc -o $(MK)/__ptdummy0.exe \
 @ALLOBJECTS@ @LDFLAGS@ @LDLIBS@; \
$(RM) $(MK)/__ptdummy0.*
endef

define GNU2.95.3_PIC_CLOSURE
$(RM) $(MK)/__ptdummy0.cc; \
echo "int main() { return 0; }" > $(MK)/__ptdummy0.cc; \
$(C++) $(MK)/__ptdummy0.cc -o $(MK)/__ptdummy0.exe \
 @ALLOBJECTS@ @LDFLAGS@ @LDLIBS@; \
$(RM) $(MK)/__ptdummy0.*
endef

define GNU2.95.3_PIC_LIBRARY
$(C++) -shared -o @LIB@ $(PIC_LDFLAGS) \
 @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

ifeq "$(C++COMPILERENV)" "PTHREAD"

GNU2.95.3_VARIANTS = dbg opt prf std
GNU2.95.3_PIC_VARIANTS = opt std

GNU2.95.3_ENV_OODBMS =

GNU2.95.3_OPT_LIBRARIES = OTC-PY

GNU2.95.3_C++ = /usr/bin/g++
GNU2.95.3_CC = /usr/bin/gcc
GNU2.95.3_LD = /usr/bin/ld
GNU2.95.3_AR = /usr/bin/ar

GNU2.95.3_CPPFLAGS = -DHAVE_POSIX_THREADS
GNU2.95.3_CFLAGS = -pipe
GNU2.95.3_C++FLAGS = -pipe
GNU2.95.3_LDFLAGS = -pthread
GNU2.95.3_LDLIBS = -lm

GNU2.95.3_PIC_CPPFLAGS =
GNU2.95.3_PIC_CFLAGS =
GNU2.95.3_PIC_C++FLAGS =
GNU2.95.3_PIC_LDFLAGS = -pthread
GNU2.95.3_PIC_LDLIBS =

GNU2.95.3_CPPFLAGS_dbg =
GNU2.95.3_CFLAGS_dbg = -g
GNU2.95.3_C++FLAGS_dbg = -g
GNU2.95.3_LDFLAGS_dbg =
GNU2.95.3_LDLIBS_dbg =

GNU2.95.3_CPPFLAGS_opt = -DNDEBUG
GNU2.95.3_CFLAGS_opt = -O
GNU2.95.3_C++FLAGS_opt = -O
GNU2.95.3_LDFLAGS_opt =
GNU2.95.3_LDLIBS_opt =

GNU2.95.3_CPPFLAGS_prf =
GNU2.95.3_CFLAGS_prf = -pg
GNU2.95.3_C++FLAGS_prf = -pg
GNU2.95.3_LDFLAGS_prf =
GNU2.95.3_LDLIBS_prf =

GNU2.95.3_CPPFLAGS_std =
GNU2.95.3_CFLAGS_std =
GNU2.95.3_C++FLAGS_std =
GNU2.95.3_LDFLAGS_std =
GNU2.95.3_LDLIBS_std =

define GNU2.95.3_PIC_LIBRARY
$(C++) -shared -o @LIB@ $(PIC_LDFLAGS) \
 @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif
