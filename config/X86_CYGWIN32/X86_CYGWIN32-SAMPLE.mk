#
# General definitions.
#

# System type.

ENV_SYSTEM = CYGWIN32

# Installation directory.

OSE_ROOT_DIRECTORY = c:/ose
#OSE_RELEASE_DIRECTORY = tst

# Default and desired C++ compilers.

DEFAULTC++COMPILER = GNU2.95.3
C++COMPILERS = GNU2.95.3

# Default and desired compilation variants.

#DEFAULTVARIANT = dbg
DEFAULTVARIANT = opt
#VARIANTS = dbg opt std
VARIANTS = opt

#
# Default library management definitions.
#

define DEFAULT_SYM_CLEANUP
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
# Compiler definitions.
#

# Cygnus GNU EGCS C/C++ 1.1

ifeq "$(C++COMPILERENV)" ""

EGCS1.1_VARIANTS = dbg opt std
EGCS1.1_PIC_VARIANTS =

EGCS1.1_ENV_OODBMS =

EGCS1.1_OPT_LIBRARIES =

EGCS1.1_ROOT = d:/cygnus/cygwin-b20/H-i586-cygwin32

EGCS1.1_C++ = $(EGCS1.1_ROOT)/bin/g++
EGCS1.1_CC = $(EGCS1.1_ROOT)/bin/gcc
EGCS1.1_LD = $(EGCS1.1_ROOT)/bin/ld
EGCS1.1_AR = $(EGCS1.1_ROOT)/bin/ar

EGCS1.1_CPPFLAGS = -DENV_NOTHROWEXCEPTION
EGCS1.1_CFLAGS =
EGCS1.1_C++FLAGS =
EGCS1.1_LDFLAGS =
EGCS1.1_LDLIBS =

EGCS1.1_PIC_CPPFLAGS =
EGCS1.1_PIC_CFLAGS =
EGCS1.1_PIC_C++FLAGS =
EGCS1.1_PIC_LDFLAGS =
EGCS1.1_PIC_LDLIBS =

EGCS1.1_CPPFLAGS_dbg =
EGCS1.1_CFLAGS_dbg = -g
EGCS1.1_C++FLAGS_dbg = -g
EGCS1.1_LDFLAGS_dbg =
EGCS1.1_LDLIBS_dbg =

EGCS1.1_CPPFLAGS_opt = -DNDEBUG
EGCS1.1_CFLAGS_opt = -O
EGCS1.1_C++FLAGS_opt = -O
EGCS1.1_LDFLAGS_opt =
EGCS1.1_LDLIBS_opt =

EGCS1.1_CPPFLAGS_prf =
EGCS1.1_CFLAGS_prf = -pg
EGCS1.1_C++FLAGS_prf = -pg
EGCS1.1_LDFLAGS_prf =
EGCS1.1_LDLIBS_prf =

EGCS1.1_CPPFLAGS_std =
EGCS1.1_CFLAGS_std =
EGCS1.1_C++FLAGS_std =
EGCS1.1_LDFLAGS_std =
EGCS1.1_LDLIBS_std =

endif

ifeq "$(C++COMPILERENV)" "REPO"

EGCS1.1_VARIANTS = dbg opt std
EGCS1.1_PIC_VARIANTS =

EGCS1.1_ENV_OODBMS =

EGCS1.1_OPT_LIBRARIES =

EGCS1.1_ROOT = d:/cygnus/cygwin-b20/H-i586-cygwin32

EGCS1.1_C++ = $(EGCS1.1_ROOT)/bin/g++
EGCS1.1_CC = $(EGCS1.1_ROOT)/bin/gcc
EGCS1.1_LD = $(EGCS1.1_ROOT)/bin/ld
EGCS1.1_AR = $(EGCS1.1_ROOT)/bin/ar

EGCS1.1_CPPFLAGS = -DENV_NOTHROWEXCEPTION
EGCS1.1_CFLAGS =
EGCS1.1_C++FLAGS = -frepo
EGCS1.1_LDFLAGS =
EGCS1.1_LDLIBS =

EGCS1.1_PIC_CPPFLAGS =
EGCS1.1_PIC_CFLAGS =
EGCS1.1_PIC_C++FLAGS =
EGCS1.1_PIC_LDFLAGS =
EGCS1.1_PIC_LDLIBS =

EGCS1.1_CPPFLAGS_dbg =
EGCS1.1_CFLAGS_dbg = -g
EGCS1.1_C++FLAGS_dbg = -g
EGCS1.1_LDFLAGS_dbg =
EGCS1.1_LDLIBS_dbg =

EGCS1.1_CPPFLAGS_opt = -DNDEBUG
EGCS1.1_CFLAGS_opt = -O
EGCS1.1_C++FLAGS_opt = -O
EGCS1.1_LDFLAGS_opt =
EGCS1.1_LDLIBS_opt =

EGCS1.1_CPPFLAGS_prf =
EGCS1.1_CFLAGS_prf = -pg
EGCS1.1_C++FLAGS_prf = -pg
EGCS1.1_LDFLAGS_prf =
EGCS1.1_LDLIBS_prf =

EGCS1.1_CPPFLAGS_std =
EGCS1.1_CFLAGS_std =
EGCS1.1_C++FLAGS_std =
EGCS1.1_LDFLAGS_std =
EGCS1.1_LDLIBS_std =

# Following to be used only when -frepo added to C++FLAGS above.

define EGCS1.1_LIB_CLOSURE
$(RM) $(MK)/__ptdummy0.cc; \
echo "int main() { return 0; }" > $(MK)/__ptdummy0.cc; \
$(C++) $(MK)/__ptdummy0.cc -o $(MK)/__ptdummy0.exe \
 @ALLOBJECTS@ @LDFLAGS@ @LDLIBS@; \
$(RM) $(MK)/__ptdummy0.*
endef

define EGCS1.1_PIC_CLOSURE
$(RM) $(MK)/__ptdummy0.cc; \
echo "int main() { return 0; }" > $(MK)/__ptdummy0.cc; \
$(C++) $(MK)/__ptdummy0.cc -o $(MK)/__ptdummy0.exe \
 @ALLOBJECTS@ @LDFLAGS@ @LDLIBS@; \
$(RM) $(MK)/__ptdummy0.*
endef

endif

# Cygnus GNU C++ 2.95.3

ifeq "$(C++COMPILERENV)" ""

GNU2.95.3_VARIANTS = dbg opt std
#GNU2.95.3_PIC_VARIANTS =
GNU2.95.3_PIC_VARIANTS = opt

GNU2.95.3_ENV_OODBMS =

GNU2.95.3_OPT_LIBRARIES =

GNU2.95.3_ROOT = /usr

GNU2.95.3_C++ = $(GNU2.95.3_ROOT)/bin/g++
GNU2.95.3_CC = $(GNU2.95.3_ROOT)/bin/gcc
GNU2.95.3_LD = $(GNU2.95.3_ROOT)/bin/ld
GNU2.95.3_AR = $(GNU2.95.3_ROOT)/bin/ar

GNU2.95.3_CPPFLAGS =
GNU2.95.3_CFLAGS =
GNU2.95.3_C++FLAGS =
GNU2.95.3_LDFLAGS =
GNU2.95.3_LDLIBS =

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
$(C++) -shared -Wl,--out-implib=@IMPLIB@ -Wl,--export-all-symbols \
 @LDFLAGS@ $(PIC_LDFLAGS) -o @LIB@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif
