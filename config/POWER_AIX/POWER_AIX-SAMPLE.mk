#
# General definitions.
#

# System type.

ENV_SYSTEM = UNIX

# Installation directory.

#OSE_ROOT_DIRECTORY = /usr/local/ose
#OSE_RELEASE_DIRECTORY = tst

# Default and desired C++ compilers.

DEFAULTC++COMPILER = XL3.1
C++COMPILERS = XL3.1

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

# IBM XL C++ 3.1.

ifeq "$(C++COMPILERENV)" ""

XL3.1_VARIANTS = dbg opt prf std
XL3.1_PIC_VARIANTS =
#XL3.1_PIC_VARIANTS = opt std

XL3.1_ENV_OODBMS =

XL3.1_OPT_LIBRARIES =

XL3.1_C++ = /bin/xlC
XL3.1_CC = /bin/xlc
XL3.1_LD = /bin/ld
XL3.1_AR = /bin/ar

XL3.1_CPPFLAGS =
XL3.1_CFLAGS =
XL3.1_C++FLAGS =
XL3.1_LDFLAGS =
XL3.1_LDLIBS =

XL3.1_PIC_CPPFLAGS =
XL3.1_PIC_CFLAGS =
XL3.1_PIC_C++FLAGS =
XL3.1_PIC_LDFLAGS =
XL3.1_PIC_LDLIBS =

XL3.1_CPPFLAGS_dbg =
XL3.1_CFLAGS_dbg = -g
XL3.1_C++FLAGS_dbg = -g
XL3.1_LDFLAGS_dbg =
XL3.1_LDLIBS_dbg =

XL3.1_CPPFLAGS_opt = -DNDEBUG
XL3.1_CFLAGS_opt = -O
XL3.1_C++FLAGS_opt = -O
XL3.1_LDFLAGS_opt =
XL3.1_LDLIBS_opt =

XL3.1_CPPFLAGS_prf =
XL3.1_CFLAGS_prf = -pg
XL3.1_C++FLAGS_prf = -pg
XL3.1_LDFLAGS_prf =
XL3.1_LDLIBS_prf =

XL3.1_CPPFLAGS_std =
XL3.1_CFLAGS_std =
XL3.1_C++FLAGS_std =
XL3.1_LDFLAGS_std =
XL3.1_LDLIBS_std =

define XL3.1_LIB_ARCHIVE
$(C++) -o @LIB@ @PTRFLAGS@ @ALLOBJECTS@
endef

define XL3.1_PIC_LIBRARY
makeC++SharedLib -o @LIB@ -p0 $(PIC_LDFLAGS) @LDFLAGS@ \
 @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif
