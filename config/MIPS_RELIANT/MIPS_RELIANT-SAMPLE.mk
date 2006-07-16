#
# General definitions.
#

# System type.

ENV_SYSTEM = UNIX

# Installation directory.

#OSE_ROOT_DIRECTORY = /usr/local/ose
#OSE_RELEASE_DIRECTORY = tst

# Default and desired C++ compilers.

DEFAULTC++COMPILER = CDS1.0
C++COMPILERS = CDS1.0

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

# CDS C++ 1.0

ifeq "$(C++COMPILERENV)" ""

CDS1.0_VARIANTS = dbg opt prf std
CDS1.0_PIC_VARIANTS = dbg opt std

CDS1.0_ENV_OODBMS =

CDS1.0_OPT_LIBRARIES =

CDS1.0_C++ = /bin/CC
CDS1.0_CC = /usr/bin/cc
CDS1.0_LD = /usr/ucb/ld
CDS1.0_AR = /usr/ccs/bin/ar

CDS1.0_CPPFLAGS = -K old_specialization
CDS1.0_CFLAGS =
CDS1.0_C++FLAGS =

CDS1.0_LDFLAGS = 
CDS1.0_LDLIBS = -lsocket -lnsl -lm

CDS1.0_PIC_CPPFLAGS =
CDS1.0_PIC_CFLAGS = -K pic
CDS1.0_PIC_C++FLAGS = -K pic
CDS1.0_PIC_LDFLAGS =
CDS1.0_PIC_LDLIBS =

CDS1.0_CPPFLAGS_dbg =
CDS1.0_CFLAGS_dbg = -g
CDS1.0_C++FLAGS_dbg = -g
CDS1.0_LDFLAGS_dbg =
CDS1.0_LDLIBS_dbg =

CDS1.0_CPPFLAGS_opt = -DNDEBUG
CDS1.0_CFLAGS_opt = -O
CDS1.0_C++FLAGS_opt = -O
CDS1.0_LDFLAGS_opt =
CDS1.0_LDLIBS_opt = 

CDS1.0_CPPFLAGS_prf =
CDS1.0_CFLAGS_prf = -p 
CDS1.0_C++FLAGS_prf = -p
CDS1.0_LDFLAGS_prf =
CDS1.0_LDLIBS_prf =

CDS1.0_CPPFLAGS_std =
CDS1.0_CFLAGS_std =
CDS1.0_C++FLAGS_std =
CDS1.0_LDFLAGS_std =
CDS1.0_LDLIBS_std =

define CDS1.0_PIC_LIBRARY
$(C++) -G -h lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS)
endef

endif
