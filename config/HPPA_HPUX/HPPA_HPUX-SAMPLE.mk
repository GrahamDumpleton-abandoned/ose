#
# General definitions.
#

# System type.

ENV_SYSTEM = UNIX

# Installation directory.

#OSE_ROOT_DIRECTORY = /usr/local/ose
#OSE_RELEASE_DIRECTORY = tst

# Default and desired C++ compilers.

DEFAULTC++COMPILER = AHP1.0
C++COMPILERS = AHP1.0

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

# Cygnus C++ 2.7.97R2.

ifeq "$(C++COMPILERENV)" "HPUX9"

CYG2.7.97R2_VARIANTS = dbg std
#CYG2.7.97R2_VARIANTS = dbg opt std prf
CYG2.7.97R2_PIC_VARIANTS =
#CYG2.7.97R2_PIC_VARIANTS = opt std

CYG2.7.97R2_ENV_OODBMS =

CYG2.7.97R2_OPT_LIBRARIES =

CYG2.7.97R2_BIN = /u0004/home/gosbild/cell_src/cygnus/bin
CYG2.7.97R2_C++ = $(CYG2.7.97R2_BIN)/g++
CYG2.7.97R2_CC = $(CYG2.7.97R2_BIN)/gcc
CYG2.7.97R2_LD = /bin/ld
CYG2.7.97R2_AR = /bin/ar

CYG2.7.97R2_CPPFLAGS = -DENV_NOTHROWEXCEPTION
CYG2.7.97R2_CFLAGS =
CYG2.7.97R2_C++FLAGS =
CYG2.7.97R2_LDFLAGS =
CYG2.7.97R2_LDLIBS =

CYG2.7.97R2_PIC_CPPFLAGS =
CYG2.7.97R2_PIC_CFLAGS = -fPIC
CYG2.7.97R2_PIC_C++FLAGS = -fPIC
CYG2.7.97R2_PIC_LDFLAGS =
CYG2.7.97R2_PIC_LDLIBS =

CYG2.7.97R2_CPPFLAGS_dbg =
CYG2.7.97R2_CFLAGS_dbg = -g
CYG2.7.97R2_C++FLAGS_dbg = -g
CYG2.7.97R2_LDFLAGS_dbg =
CYG2.7.97R2_LDLIBS_dbg =

CYG2.7.97R2_CPPFLAGS_opt = -DNDEBUG
CYG2.7.97R2_CFLAGS_opt = -O
CYG2.7.97R2_C++FLAGS_opt = -O
CYG2.7.97R2_LDFLAGS_opt =
CYG2.7.97R2_LDLIBS_opt =

CYG2.7.97R2_CPPFLAGS_prf =
CYG2.7.97R2_CFLAGS_prf = -pg
CYG2.7.97R2_C++FLAGS_prf = -pg
CYG2.7.97R2_LDFLAGS_prf =
CYG2.7.97R2_LDLIBS_prf =

CYG2.7.97R2_CPPFLAGS_std =
CYG2.7.97R2_CFLAGS_std =
CYG2.7.97R2_C++FLAGS_std =
CYG2.7.97R2_LDFLAGS_std =
CYG2.7.97R2_LDLIBS_std =

define CYG2.7.97R2_PIC_LIBRARY
$(C++) -shared -h lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

# GNU C++ 2.7.2.

ifeq "$(C++COMPILERENV)" ""

GNU2.7.2_VARIANTS = dbg opt prf std
GNU2.7.2_PIC_VARIANTS =

GNU2.7.2_ENV_OODBMS =

GNU2.7.2_OPT_LIBRARIES =

GNU2.7.2_C++ = /usr/local/bin/g++
GNU2.7.2_CC = /usr/local/bin/gcc
GNU2.7.2_LD = /bin/ld
GNU2.7.2_AR = /bin/ar

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

# HP C++ 3.7.

ifeq "$(C++COMPILERENV)" ""

HP3.7_VARIANTS = dbg opt prf std
HP3.7_PIC_VARIANTS = opt std

HP3.7_ENV_OODBMS =

HP3.7_OPT_LIBRARIES =

HP3.7_C++ = /usr/bin/CC
HP3.7_CC = /bin/cc
HP3.7_LD = /bin/ld
HP3.7_AR = /bin/ar

HP3.7_CPPFLAGS =
HP3.7_CFLAGS =
HP3.7_C++FLAGS =
HP3.7_LDFLAGS =
HP3.7_LDLIBS =

HP3.7_PIC_CPPFLAGS =
HP3.7_PIC_CFLAGS = +z
HP3.7_PIC_C++FLAGS = +z
HP3.7_PIC_LDFLAGS =
HP3.7_PIC_LDLIBS =

HP3.7_CPPFLAGS_dbg =
HP3.7_CFLAGS_dbg = -g
HP3.7_C++FLAGS_dbg = -g
HP3.7_LDFLAGS_dbg =
HP3.7_LDLIBS_dbg =

HP3.7_CPPFLAGS_opt = -DNDEBUG
HP3.7_CFLAGS_opt = -O
HP3.7_C++FLAGS_opt = -O
HP3.7_LDFLAGS_opt =
HP3.7_LDLIBS_opt =

HP3.7_CPPFLAGS_prf =
HP3.7_CFLAGS_prf = -G
HP3.7_C++FLAGS_prf = -G
HP3.7_LDFLAGS_prf =
HP3.7_LDLIBS_prf =

HP3.7_CPPFLAGS_std =
HP3.7_CFLAGS_std =
HP3.7_C++FLAGS_std =
HP3.7_LDFLAGS_std =
HP3.7_LDLIBS_std =

define HP3.7_PIC_LIBRARY
$(C++) -b @PIC_PTRFLAGS@ -DPIC $(CPPFLAGS) $(C++FLAGS) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

# HP aC++ 1.0.

ifeq "$(C++COMPILERENV)" ""

AHP1.0_VARIANTS = dbg opt prf std
AHP1.0_PIC_VARIANTS = opt std

AHP1.0_ENV_OODBMS =

AHP1.0_OPT_LIBRARIES =

AHP1.0_C++ = /opt/aCC/bin/aCC
AHP1.0_CC = /bin/cc
AHP1.0_LD = /bin/ld
AHP1.0_AR = /bin/ar

AHP1.0_CPPFLAGS =
AHP1.0_CFLAGS =
AHP1.0_C++FLAGS =
AHP1.0_LDFLAGS =
AHP1.0_LDLIBS =

AHP1.0_PIC_CPPFLAGS =
AHP1.0_PIC_CFLAGS = +z
AHP1.0_PIC_C++FLAGS = +z
AHP1.0_PIC_LDFLAGS =
AHP1.0_PIC_LDLIBS =

AHP1.0_CPPFLAGS_dbg =
AHP1.0_CFLAGS_dbg = -g
AHP1.0_C++FLAGS_dbg = -g
AHP1.0_LDFLAGS_dbg =
AHP1.0_LDLIBS_dbg =

AHP1.0_CPPFLAGS_opt = -DNDEBUG
AHP1.0_CFLAGS_opt = -O
AHP1.0_C++FLAGS_opt = -O
AHP1.0_LDFLAGS_opt =
AHP1.0_LDLIBS_opt =

AHP1.0_CPPFLAGS_prf =
AHP1.0_CFLAGS_prf = -G
AHP1.0_C++FLAGS_prf = -G
AHP1.0_LDFLAGS_prf =
AHP1.0_LDLIBS_prf =

AHP1.0_CPPFLAGS_std =
AHP1.0_CFLAGS_std =
AHP1.0_C++FLAGS_std =
AHP1.0_LDFLAGS_std =
AHP1.0_LDLIBS_std =

# Uncomment following only if +inst_auto is added to C++FLAGS above.

#define AHP1.0_LIB_CLOSURE
#$(RM) $(MK)/__ptdummy0.cc; \
#echo "int main() { return 0; }" > $(MK)/__ptdummy0.cc; \
#$(C++) +inst_auto +inst_close $(MK)/__ptdummy0.cc -o $(MK)/__ptdummy0.exe \
# @ALLOBJECTS@ @LDFLAGS@ @LDLIBS@; \
# $(RM) $(MK)/__ptdummy0.*
#endef

define AHP1.0_PIC_LIBRARY
$(C++) -b -DPIC $(CPPFLAGS) $(C++FLAGS) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif
