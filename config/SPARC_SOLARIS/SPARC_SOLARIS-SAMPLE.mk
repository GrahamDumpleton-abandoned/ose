#
# General definitions.
#

# System type.

ENV_SYSTEM = UNIX

# Installation directory.

#OSE_ROOT_DIRECTORY = /usr/local/ose
#OSE_RELEASE_DIRECTORY = tst

# Default and desired C++ compilers.

DEFAULTC++COMPILER = PTHREAD-SUN5.2
C++COMPILERS = PTHREAD-SUN5.2

# Default and desired compilation variants.

DEFAULTVARIANT = dbg
VARIANTS = dbg opt prf std

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
$(LD) -G -o @LIB@ $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

#
# Compiler definitions.
#

# Cygnus C++ 2.9.98R1.

ifeq "$(C++COMPILERENV)" ""

CYG2.9.98R1_VARIANTS = dbg opt std prf
CYG2.9.98R1_PIC_VARIANTS = opt std

CYG2.9.98R1_ENV_OODBMS =

CYG2.9.98R1_OPT_LIBRARIES =

CYG2.9.98R1_ROOT = /usr/local/cygnus/gnupro-98r1/H-sparc-sun-solaris2.5
CYG2.9.98R1_BIN = $(CYG2.9.98R1_ROOT)/bin
CYG2.9.98R1_LIB = $(CYG2.9.98R1_ROOT)/lib/gcc-lib/sparc-sun-solaris2.5/2.9-98r1

CYG2.9.98R1_C++ = $(CYG2.9.98R1_BIN)/g++
CYG2.9.98R1_CC = $(CYG2.9.98R1_BIN)/gcc
CYG2.9.98R1_LD = /usr/ccs/bin/ld
CYG2.9.98R1_AR = /usr/ccs/bin/ar

CYG2.9.98R1_CPPFLAGS = -DENV_NOTHROWEXCEPTION
CYG2.9.98R1_CFLAGS = -pipe
CYG2.9.98R1_C++FLAGS = -pipe
CYG2.9.98R1_LDFLAGS = -R $$LD_RUN_PATH
CYG2.9.98R1_LDLIBS = -lsocket -lnsl

CYG2.9.98R1_PIC_CPPFLAGS =
CYG2.9.98R1_PIC_CFLAGS = -fPIC
CYG2.9.98R1_PIC_C++FLAGS = -fPIC
CYG2.9.98R1_PIC_LDFLAGS = -R $$LD_RUN_PATH
CYG2.9.98R1_PIC_LDLIBS =

CYG2.9.98R1_CPPFLAGS_dbg =
CYG2.9.98R1_CFLAGS_dbg = -g
CYG2.9.98R1_C++FLAGS_dbg = -g
CYG2.9.98R1_LDFLAGS_dbg =
CYG2.9.98R1_LDLIBS_dbg =

CYG2.9.98R1_CPPFLAGS_opt = -DNDEBUG
CYG2.9.98R1_CFLAGS_opt = -O
CYG2.9.98R1_C++FLAGS_opt = -O
CYG2.9.98R1_LDFLAGS_opt =
CYG2.9.98R1_LDLIBS_opt =

CYG2.9.98R1_CPPFLAGS_prf =
CYG2.9.98R1_CFLAGS_prf = -pg
CYG2.9.98R1_C++FLAGS_prf = -pg
CYG2.9.98R1_LDFLAGS_prf =
CYG2.9.98R1_LDLIBS_prf =

CYG2.9.98R1_CPPFLAGS_std =
CYG2.9.98R1_CFLAGS_std =
CYG2.9.98R1_C++FLAGS_std =
CYG2.9.98R1_LDFLAGS_std =
CYG2.9.98R1_LDLIBS_std =

define CYG2.9.98R1_PIC_LIBRARY
$(C++) -shared -h lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

ifeq "$(C++COMPILERENV)" "REPO"

CYG2.9.98R1_VARIANTS = dbg opt std prf
CYG2.9.98R1_PIC_VARIANTS = opt std

CYG2.9.98R1_ENV_OODBMS =

CYG2.9.98R1_OPT_LIBRARIES =

CYG2.9.98R1_ROOT = /usr/local/cygnus/gnupro-98r1/H-sparc-sun-solaris2.5
CYG2.9.98R1_BIN = $(CYG2.9.98R1_ROOT)/bin
CYG2.9.98R1_LIB = $(CYG2.9.98R1_ROOT)/lib/gcc-lib/sparc-sun-solaris2.5/2.9-98r1

CYG2.9.98R1_C++ = $(CYG2.9.98R1_BIN)/g++
CYG2.9.98R1_CC = $(CYG2.9.98R1_BIN)/gcc
CYG2.9.98R1_LD = /usr/ccs/bin/ld
CYG2.9.98R1_AR = /usr/ccs/bin/ar

CYG2.9.98R1_CPPFLAGS = -DENV_NOTHROWEXCEPTION
CYG2.9.98R1_CFLAGS = -pipe
CYG2.9.98R1_C++FLAGS = -pipe -frepo
CYG2.9.98R1_LDFLAGS = -R $$LD_RUN_PATH
CYG2.9.98R1_LDLIBS = -lsocket -lnsl

CYG2.9.98R1_PIC_CPPFLAGS =
CYG2.9.98R1_PIC_CFLAGS = -fPIC
CYG2.9.98R1_PIC_C++FLAGS = -fPIC
CYG2.9.98R1_PIC_LDFLAGS = -R $$LD_RUN_PATH
CYG2.9.98R1_PIC_LDLIBS =

CYG2.9.98R1_CPPFLAGS_dbg =
CYG2.9.98R1_CFLAGS_dbg = -g
CYG2.9.98R1_C++FLAGS_dbg = -g
CYG2.9.98R1_LDFLAGS_dbg =
CYG2.9.98R1_LDLIBS_dbg =

CYG2.9.98R1_CPPFLAGS_opt = -DNDEBUG
CYG2.9.98R1_CFLAGS_opt = -O
CYG2.9.98R1_C++FLAGS_opt = -O
CYG2.9.98R1_LDFLAGS_opt =
CYG2.9.98R1_LDLIBS_opt =

CYG2.9.98R1_CPPFLAGS_prf =
CYG2.9.98R1_CFLAGS_prf = -pg
CYG2.9.98R1_C++FLAGS_prf = -pg
CYG2.9.98R1_LDFLAGS_prf =
CYG2.9.98R1_LDLIBS_prf =

CYG2.9.98R1_CPPFLAGS_std =
CYG2.9.98R1_CFLAGS_std =
CYG2.9.98R1_C++FLAGS_std =
CYG2.9.98R1_LDFLAGS_std =
CYG2.9.98R1_LDLIBS_std =

define CYG2.9.98R1_PIC_LIBRARY
$(C++) -shared -h lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

# Following to be used only when -frepo added to C++FLAGS above.

define CYG2.9.98R1_LIB_CLOSURE
$(RM) $(MK)/__ptdummy0.cc; \
echo "int main() { return 0; }" > $(MK)/__ptdummy0.cc; \
$(C++) $(MK)/__ptdummy0.cc -o $(MK)/__ptdummy0.exe \
 @ALLOBJECTS@ @LDFLAGS@ @LDLIBS@; \
$(RM) $(MK)/__ptdummy0.*
endef

define CYG2.9.98R1_PIC_CLOSURE
$(RM) $(MK)/__ptdummy0.cc; \
echo "int main() { return 0; }" > $(MK)/__ptdummy0.cc; \
$(C++) $(MK)/__ptdummy0.cc -o $(MK)/__ptdummy0.exe \
 @ALLOBJECTS@ @LDFLAGS@ @LDLIBS@; \
$(RM) $(MK)/__ptdummy0.*
endef

endif

ifeq "$(C++COMPILERENV)" "THREAD"

CYG2.9.98R1_VARIANTS = dbg opt std prf
CYG2.9.98R1_PIC_VARIANTS = opt std

CYG2.9.98R1_ENV_OODBMS =

CYG2.9.98R1_OPT_LIBRARIES =

CYG2.9.98R1_ROOT = /usr/local/cygnus/gnupro-98r1/H-sparc-sun-solaris2.5
CYG2.9.98R1_BIN = $(CYG2.9.98R1_ROOT)/bin
CYG2.9.98R1_LIB = $(CYG2.9.98R1_ROOT)/lib/gcc-lib/sparc-sun-solaris2.5/2.9-98r1

CYG2.9.98R1_C++ = $(CYG2.9.98R1_BIN)/g++
CYG2.9.98R1_CC = $(CYG2.9.98R1_BIN)/gcc
CYG2.9.98R1_LD = /usr/ccs/bin/ld
CYG2.9.98R1_AR = /usr/ccs/bin/ar

CYG2.9.98R1_CPPFLAGS = -DENV_NOTHROWEXCEPTION
CYG2.9.98R1_CPPFLAGS += -DHAVE_SOLARIS_THREADS -D_REENTRANT
CYG2.9.98R1_CFLAGS = -pipe
CYG2.9.98R1_C++FLAGS = -pipe
CYG2.9.98R1_LDFLAGS = -R $$LD_RUN_PATH
CYG2.9.98R1_LDLIBS = -lsocket -lnsl -lthread

CYG2.9.98R1_PIC_CPPFLAGS =
CYG2.9.98R1_PIC_CFLAGS = -fPIC
CYG2.9.98R1_PIC_C++FLAGS = -fPIC
CYG2.9.98R1_PIC_LDFLAGS = -R $$LD_RUN_PATH
CYG2.9.98R1_PIC_LDLIBS =

CYG2.9.98R1_CPPFLAGS_dbg =
CYG2.9.98R1_CFLAGS_dbg = -g
CYG2.9.98R1_C++FLAGS_dbg = -g
CYG2.9.98R1_LDFLAGS_dbg =
CYG2.9.98R1_LDLIBS_dbg =

CYG2.9.98R1_CPPFLAGS_opt = -DNDEBUG
CYG2.9.98R1_CFLAGS_opt = -O
CYG2.9.98R1_C++FLAGS_opt = -O
CYG2.9.98R1_LDFLAGS_opt =
CYG2.9.98R1_LDLIBS_opt =

CYG2.9.98R1_CPPFLAGS_prf =
CYG2.9.98R1_CFLAGS_prf = -pg
CYG2.9.98R1_C++FLAGS_prf = -pg
CYG2.9.98R1_LDFLAGS_prf =
CYG2.9.98R1_LDLIBS_prf =

CYG2.9.98R1_CPPFLAGS_std =
CYG2.9.98R1_CFLAGS_std =
CYG2.9.98R1_C++FLAGS_std =
CYG2.9.98R1_LDFLAGS_std =
CYG2.9.98R1_LDLIBS_std =

define CYG2.9.98R1_PIC_LIBRARY
$(C++) -shared -h lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

ifeq "$(C++COMPILERENV)" "PTHREAD"

CYG2.9.98R1_VARIANTS = dbg opt std prf
CYG2.9.98R1_PIC_VARIANTS = opt std

CYG2.9.98R1_ENV_OODBMS =

CYG2.9.98R1_OPT_LIBRARIES =

CYG2.9.98R1_ROOT = /usr/local/cygnus/gnupro-98r1/H-sparc-sun-solaris2.5
CYG2.9.98R1_BIN = $(CYG2.9.98R1_ROOT)/bin
CYG2.9.98R1_LIB = $(CYG2.9.98R1_ROOT)/lib/gcc-lib/sparc-sun-solaris2.5/2.9-98r1

CYG2.9.98R1_C++ = $(CYG2.9.98R1_BIN)/g++
CYG2.9.98R1_CC = $(CYG2.9.98R1_BIN)/gcc
CYG2.9.98R1_LD = /usr/ccs/bin/ld
CYG2.9.98R1_AR = /usr/ccs/bin/ar

CYG2.9.98R1_CPPFLAGS = -DENV_NOTHROWEXCEPTION -DHAVE_POSIX_THREADS -D_REENTRANT
CYG2.9.98R1_CFLAGS = -pipe
CYG2.9.98R1_C++FLAGS = -pipe
CYG2.9.98R1_LDFLAGS = -R $$LD_RUN_PATH
CYG2.9.98R1_LDLIBS = -lsocket -lnsl -lpthread

CYG2.9.98R1_PIC_CPPFLAGS =
CYG2.9.98R1_PIC_CFLAGS = -fPIC
CYG2.9.98R1_PIC_C++FLAGS = -fPIC
CYG2.9.98R1_PIC_LDFLAGS = -R $$LD_RUN_PATH
CYG2.9.98R1_PIC_LDLIBS =

CYG2.9.98R1_CPPFLAGS_dbg =
CYG2.9.98R1_CFLAGS_dbg = -g
CYG2.9.98R1_C++FLAGS_dbg = -g
CYG2.9.98R1_LDFLAGS_dbg =
CYG2.9.98R1_LDLIBS_dbg =

CYG2.9.98R1_CPPFLAGS_opt = -DNDEBUG
CYG2.9.98R1_CFLAGS_opt = -O
CYG2.9.98R1_C++FLAGS_opt = -O
CYG2.9.98R1_LDFLAGS_opt =
CYG2.9.98R1_LDLIBS_opt =

CYG2.9.98R1_CPPFLAGS_prf =
CYG2.9.98R1_CFLAGS_prf = -pg
CYG2.9.98R1_C++FLAGS_prf = -pg
CYG2.9.98R1_LDFLAGS_prf =
CYG2.9.98R1_LDLIBS_prf =

CYG2.9.98R1_CPPFLAGS_std =
CYG2.9.98R1_CFLAGS_std =
CYG2.9.98R1_C++FLAGS_std =
CYG2.9.98R1_LDFLAGS_std =
CYG2.9.98R1_LDLIBS_std =

define CYG2.9.98R1_PIC_LIBRARY
$(C++) -shared -h lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

# Cygnus C++ 2.9.98R2.

ifeq "$(C++COMPILERENV)" ""

CYG2.9.98R2_VARIANTS = dbg opt std prf
CYG2.9.98R2_PIC_VARIANTS = opt std

CYG2.9.98R2_ENV_OODBMS =

CYG2.9.98R2_OPT_LIBRARIES =

CYG2.9.98R2_ROOT = /usr/local/cygnus/gnupro-98r2/H-sparc-sun-solaris2.5
CYG2.9.98R2_BIN = $(CYG2.9.98R2_ROOT)/bin
CYG2.9.98R2_LIB = $(CYG2.9.98R2_ROOT)/lib/gcc-lib/sparc-sun-solaris2.5/2.9-98r2

CYG2.9.98R2_C++ = $(CYG2.9.98R2_BIN)/g++
CYG2.9.98R2_CC = $(CYG2.9.98R2_BIN)/gcc
CYG2.9.98R2_LD = /usr/ccs/bin/ld
CYG2.9.98R2_AR = /usr/ccs/bin/ar

CYG2.9.98R2_CPPFLAGS = -DENV_NOTHROWEXCEPTION
CYG2.9.98R2_CFLAGS = -pipe
CYG2.9.98R2_C++FLAGS = -pipe
CYG2.9.98R2_LDFLAGS = -R $$LD_RUN_PATH
CYG2.9.98R2_LDLIBS = -lsocket -lnsl

CYG2.9.98R2_PIC_CPPFLAGS =
CYG2.9.98R2_PIC_CFLAGS = -fPIC
CYG2.9.98R2_PIC_C++FLAGS = -fPIC
CYG2.9.98R2_PIC_LDFLAGS = -R $$LD_RUN_PATH
CYG2.9.98R2_PIC_LDLIBS =

CYG2.9.98R2_CPPFLAGS_dbg =
CYG2.9.98R2_CFLAGS_dbg = -g
CYG2.9.98R2_C++FLAGS_dbg = -g
CYG2.9.98R2_LDFLAGS_dbg =
CYG2.9.98R2_LDLIBS_dbg =

CYG2.9.98R2_CPPFLAGS_opt = -DNDEBUG
CYG2.9.98R2_CFLAGS_opt = -O
CYG2.9.98R2_C++FLAGS_opt = -O
CYG2.9.98R2_LDFLAGS_opt =
CYG2.9.98R2_LDLIBS_opt =

CYG2.9.98R2_CPPFLAGS_prf =
CYG2.9.98R2_CFLAGS_prf = -pg
CYG2.9.98R2_C++FLAGS_prf = -pg
CYG2.9.98R2_LDFLAGS_prf =
CYG2.9.98R2_LDLIBS_prf =

CYG2.9.98R2_CPPFLAGS_std =
CYG2.9.98R2_CFLAGS_std =
CYG2.9.98R2_C++FLAGS_std =
CYG2.9.98R2_LDFLAGS_std =
CYG2.9.98R2_LDLIBS_std =

define CYG2.9.98R2_PIC_LIBRARY
$(C++) -shared -h lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

ifeq "$(C++COMPILERENV)" "REPO"

CYG2.9.98R2_VARIANTS = dbg opt std prf
CYG2.9.98R2_PIC_VARIANTS = opt std

CYG2.9.98R2_ENV_OODBMS =

CYG2.9.98R2_OPT_LIBRARIES =

CYG2.9.98R2_ROOT = /usr/local/cygnus/gnupro-98r2/H-sparc-sun-solaris2.5
CYG2.9.98R2_BIN = $(CYG2.9.98R2_ROOT)/bin
CYG2.9.98R2_LIB = $(CYG2.9.98R2_ROOT)/lib/gcc-lib/sparc-sun-solaris2.5/2.9-98r2

CYG2.9.98R2_C++ = $(CYG2.9.98R2_BIN)/g++
CYG2.9.98R2_CC = $(CYG2.9.98R2_BIN)/gcc
CYG2.9.98R2_LD = /usr/ccs/bin/ld
CYG2.9.98R2_AR = /usr/ccs/bin/ar

CYG2.9.98R2_CPPFLAGS = -DENV_NOTHROWEXCEPTION
CYG2.9.98R2_CFLAGS = -pipe
CYG2.9.98R2_C++FLAGS = -pipe -frepo
CYG2.9.98R2_LDFLAGS = -R $$LD_RUN_PATH
CYG2.9.98R2_LDLIBS = -lsocket -lnsl

CYG2.9.98R2_PIC_CPPFLAGS =
CYG2.9.98R2_PIC_CFLAGS = -fPIC
CYG2.9.98R2_PIC_C++FLAGS = -fPIC
CYG2.9.98R2_PIC_LDFLAGS = -R $$LD_RUN_PATH
CYG2.9.98R2_PIC_LDLIBS =

CYG2.9.98R2_CPPFLAGS_dbg =
CYG2.9.98R2_CFLAGS_dbg = -g
CYG2.9.98R2_C++FLAGS_dbg = -g
CYG2.9.98R2_LDFLAGS_dbg =
CYG2.9.98R2_LDLIBS_dbg =

CYG2.9.98R2_CPPFLAGS_opt = -DNDEBUG
CYG2.9.98R2_CFLAGS_opt = -O
CYG2.9.98R2_C++FLAGS_opt = -O
CYG2.9.98R2_LDFLAGS_opt =
CYG2.9.98R2_LDLIBS_opt =

CYG2.9.98R2_CPPFLAGS_prf =
CYG2.9.98R2_CFLAGS_prf = -pg
CYG2.9.98R2_C++FLAGS_prf = -pg
CYG2.9.98R2_LDFLAGS_prf =
CYG2.9.98R2_LDLIBS_prf =

CYG2.9.98R2_CPPFLAGS_std =
CYG2.9.98R2_CFLAGS_std =
CYG2.9.98R2_C++FLAGS_std =
CYG2.9.98R2_LDFLAGS_std =
CYG2.9.98R2_LDLIBS_std =

define CYG2.9.98R2_PIC_LIBRARY
$(C++) -shared -h lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

# Following to be used only when -frepo added to C++FLAGS above.

define CYG2.9.98R2_LIB_CLOSURE
$(RM) $(MK)/__ptdummy0.cc; \
echo "int main() { return 0; }" > $(MK)/__ptdummy0.cc; \
$(C++) $(MK)/__ptdummy0.cc -o $(MK)/__ptdummy0.exe \
 @ALLOBJECTS@ @LDFLAGS@ @LDLIBS@; \
$(RM) $(MK)/__ptdummy0.*
endef

define CYG2.9.98R2_PIC_CLOSURE
$(RM) $(MK)/__ptdummy0.cc; \
echo "int main() { return 0; }" > $(MK)/__ptdummy0.cc; \
$(C++) $(MK)/__ptdummy0.cc -o $(MK)/__ptdummy0.exe \
 @ALLOBJECTS@ @LDFLAGS@ @LDLIBS@; \
$(RM) $(MK)/__ptdummy0.*
endef

endif

# EGCS C++ 1.1

ifeq "$(C++COMPILERENV)" ""

EGCS1.1_VARIANTS = dbg opt std prf
EGCS1.1_PIC_VARIANTS = opt std

EGCS1.1_ENV_OODBMS =

EGCS1.1_OPT_LIBRARIES =

EGCS1.1_ROOT = /usr/local/egcs/egcs-1.1b/H-sparc-sun-solaris2.5.1
EGCS1.1_BIN = $(EGCS1.1_ROOT)/bin
EGCS1.1_LIB = $(EGCS1.1_ROOT)/lib/gcc-lib/sparc-sun-solaris2.5.1/egcs-2.91.57

EGCS1.1_C++ = $(EGCS1.1_BIN)/g++
EGCS1.1_CC = $(EGCS1.1_BIN)/gcc
EGCS1.1_LD = /usr/ccs/bin/ld
EGCS1.1_AR = /usr/ccs/bin/ar

EGCS1.1_CPPFLAGS = -DENV_NOTHROWEXCEPTION
EGCS1.1_CFLAGS = -pipe
EGCS1.1_C++FLAGS = -pipe
EGCS1.1_LDFLAGS = -R $$LD_RUN_PATH
EGCS1.1_LDLIBS = -lsocket -lnsl -ldl

EGCS1.1_PIC_CPPFLAGS =
EGCS1.1_PIC_CFLAGS = -fPIC
EGCS1.1_PIC_C++FLAGS = -fPIC
EGCS1.1_PIC_LDFLAGS = -R $$LD_RUN_PATH
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

define EGCS1.1_PIC_LIBRARY
$(C++) -shared -h lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

ifeq "$(C++COMPILERENV)" "REPO"

EGCS1.1_VARIANTS = dbg opt std prf
EGCS1.1_PIC_VARIANTS = opt std

EGCS1.1_ENV_OODBMS =

EGCS1.1_OPT_LIBRARIES =

EGCS1.1_ROOT = /usr/local/egcs/egcs-1.1b/H-sparc-sun-solaris2.5.1
EGCS1.1_BIN = $(EGCS1.1_ROOT)/bin
EGCS1.1_LIB = $(EGCS1.1_ROOT)/lib/gcc-lib/sparc-sun-solaris2.5.1/egcs-2.91.57

EGCS1.1_C++ = $(EGCS1.1_BIN)/g++
EGCS1.1_CC = $(EGCS1.1_BIN)/gcc
EGCS1.1_LD = /usr/ccs/bin/ld
EGCS1.1_AR = /usr/ccs/bin/ar

EGCS1.1_CPPFLAGS = -DENV_NOTHROWEXCEPTION
EGCS1.1_CFLAGS = -pipe
EGCS1.1_C++FLAGS = -pipe -frepo
EGCS1.1_LDFLAGS = -R $$LD_RUN_PATH
EGCS1.1_LDLIBS = -lsocket -lnsl -ldl

EGCS1.1_PIC_CPPFLAGS =
EGCS1.1_PIC_CFLAGS = -fPIC
EGCS1.1_PIC_C++FLAGS = -fPIC
EGCS1.1_PIC_LDFLAGS = -R $$LD_RUN_PATH
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

define EGCS1.1_PIC_LIBRARY
$(C++) -shared -h lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

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
GNU2.7.2_LDLIBS = -lsocket -lnsl -lm

GNU2.7.2_PIC_CPPFLAGS =
GNU2.7.2_PIC_CFLAGS = -fPIC
GNU2.7.2_PIC_C++FLAGS = -fPIC
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

# GNU C++ 2.8.0

ifeq "$(C++COMPILERENV)" ""

GNU2.8.0_VARIANTS = dbg opt prf std
GNU2.8.0_PIC_VARIANTS =

GNU2.8.0_ENV_OODBMS =

GNU2.8.0_OPT_LIBRARIES =

GNU2.8.0_C++ = /usr/local/bin/g++
GNU2.8.0_CC = /usr/local/bin/gcc
GNU2.8.0_LD = /usr/ccs/bin/ld
GNU2.8.0_AR = /usr/ccs/bin/ar

GNU2.8.0_CPPFLAGS =
GNU2.8.0_CFLAGS = -pipe
GNU2.8.0_C++FLAGS = -pipe
GNU2.8.0_LDFLAGS =
GNU2.8.0_LDLIBS = -lsocket -lnsl -lm

GNU2.8.0_PIC_CPPFLAGS =
GNU2.8.0_PIC_CFLAGS = -fPIC
GNU2.8.0_PIC_C++FLAGS = -fPIC
GNU2.8.0_PIC_LDFLAGS =
GNU2.8.0_PIC_LDLIBS =

GNU2.8.0_CPPFLAGS_dbg =
GNU2.8.0_CFLAGS_dbg = -g
GNU2.8.0_C++FLAGS_dbg = -g
GNU2.8.0_LDFLAGS_dbg =
GNU2.8.0_LDLIBS_dbg =

GNU2.8.0_CPPFLAGS_opt = -DNDEBUG
GNU2.8.0_CFLAGS_opt = -O
GNU2.8.0_C++FLAGS_opt = -O
GNU2.8.0_LDFLAGS_opt =
GNU2.8.0_LDLIBS_opt =

GNU2.8.0_CPPFLAGS_prf =
GNU2.8.0_CFLAGS_prf = -pg
GNU2.8.0_C++FLAGS_prf = -pg
GNU2.8.0_LDFLAGS_prf =
GNU2.8.0_LDLIBS_prf =

GNU2.8.0_CPPFLAGS_std =
GNU2.8.0_CFLAGS_std =
GNU2.8.0_C++FLAGS_std =
GNU2.8.0_LDFLAGS_std =
GNU2.8.0_LDLIBS_std =

define GNU2.8.0_PIC_LIBRARY
$(C++) -shared -h lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

# GNU C++ 2.95.2

ifeq "$(C++COMPILERENV)" ""

GNU2.95.2_VARIANTS = dbg opt std prf
GNU2.95.2_PIC_VARIANTS = opt std

GNU2.95.2_ENV_OODBMS =

GNU2.95.2_OPT_LIBRARIES =

GNU2.95.2_ROOT = /opt/sfw
GNU2.95.2_BIN = $(GNU2.95.2_ROOT)/bin
GNU2.95.2_LIB = $(GNU2.95.2_ROOT)/lib/gcc-lib/sparc-sun-solaris2.8/2.95.2

GNU2.95.2_C++ = $(GNU2.95.2_BIN)/g++
GNU2.95.2_CC = $(GNU2.95.2_BIN)/gcc
GNU2.95.2_LD = /usr/ccs/bin/ld
GNU2.95.2_AR = /usr/ccs/bin/ar

GNU2.95.2_CPPFLAGS =
GNU2.95.2_CFLAGS = -pipe
GNU2.95.2_C++FLAGS = -pipe
GNU2.95.2_LDFLAGS =
GNU2.95.2_LDLIBS = -lsocket -lnsl -ldl

GNU2.95.2_PIC_CPPFLAGS =
GNU2.95.2_PIC_CFLAGS = -fPIC
GNU2.95.2_PIC_C++FLAGS = -fPIC
GNU2.95.2_PIC_LDFLAGS = -R $$LD_RUN_PATH
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
$(C++) -shared -h lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

ifeq "$(C++COMPILERENV)" "REPO"

GNU2.95.2_VARIANTS = dbg opt std prf
GNU2.95.2_PIC_VARIANTS = opt std

GNU2.95.2_ENV_OODBMS =

GNU2.95.2_OPT_LIBRARIES =

GNU2.95.2_ROOT = /opt/sfw
GNU2.95.2_BIN = $(GNU2.95.2_ROOT)/bin
GNU2.95.2_LIB = $(GNU2.95.2_ROOT)/lib/gcc-lib/sparc-sun-solaris2.8/2.95.2

GNU2.95.2_C++ = $(GNU2.95.2_BIN)/g++
GNU2.95.2_CC = $(GNU2.95.2_BIN)/gcc
GNU2.95.2_LD = /usr/ccs/bin/ld
GNU2.95.2_AR = /usr/ccs/bin/ar

GNU2.95.2_CPPFLAGS =
GNU2.95.2_CFLAGS = -pipe
GNU2.95.2_C++FLAGS = -pipe -frepo
GNU2.95.2_LDFLAGS =
GNU2.95.2_LDLIBS = -lsocket -lnsl -ldl

GNU2.95.2_PIC_CPPFLAGS =
GNU2.95.2_PIC_CFLAGS = -fPIC
GNU2.95.2_PIC_C++FLAGS = -fPIC
GNU2.95.2_PIC_LDFLAGS = -R $$LD_RUN_PATH
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
$(C++) -shared -h lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

# Following to be used only when -frepo added to C++FLAGS above.

define GNU2.95.2_LIB_CLOSURE
$(RM) $(MK)/__ptdummy0.cc; \
echo "int main() { return 0; }" > $(MK)/__ptdummy0.cc; \
$(C++) $(MK)/__ptdummy0.cc -o $(MK)/__ptdummy0.exe \
 @ALLOBJECTS@ @LDFLAGS@ @LDLIBS@; \
$(RM) $(MK)/__ptdummy0.*
endef

define GNU2.95.2_PIC_CLOSURE
$(RM) $(MK)/__ptdummy0.cc; \
echo "int main() { return 0; }" > $(MK)/__ptdummy0.cc; \
$(C++) $(MK)/__ptdummy0.cc -o $(MK)/__ptdummy0.exe \
 @ALLOBJECTS@ @LDFLAGS@ @LDLIBS@; \
$(RM) $(MK)/__ptdummy0.*
endef

endif

# SUN C++ 4.2.

ifeq "$(C++COMPILERENV)" ""

SUN4.2_VARIANTS = dbg opt prf std
SUN4.2_PIC_VARIANTS = opt std

SUN4.2_ENV_OODBMS =

SUN4.2_OPT_LIBRARIES =

SUN4.2_C++ = /opt/SUNWspro/SC4.0/bin/CC
SUN4.2_CC = /opt/SUNWspro/SC4.0/bin/cc
SUN4.2_LD = /usr/ccs/bin/ld
SUN4.2_AR = /usr/ccs/bin/ar

SUN4.2_CPPFLAGS =
SUN4.2_CFLAGS =
SUN4.2_C++FLAGS = +p
SUN4.2_LDFLAGS =
SUN4.2_LDLIBS = -lsocket -lnsl -ldl

SUN4.2_PIC_CPPFLAGS =
SUN4.2_PIC_CFLAGS = -KPIC
SUN4.2_PIC_C++FLAGS = -KPIC
SUN4.2_PIC_LDFLAGS =
SUN4.2_PIC_LDLIBS =

SUN4.2_CPPFLAGS_dbg =
SUN4.2_CFLAGS_dbg = -g
SUN4.2_C++FLAGS_dbg = -g
SUN4.2_LDFLAGS_dbg =
SUN4.2_LDLIBS_dbg =

SUN4.2_CPPFLAGS_opt = -DNDEBUG
SUN4.2_CFLAGS_opt = -O
SUN4.2_C++FLAGS_opt = -O
SUN4.2_LDFLAGS_opt =
SUN4.2_LDLIBS_opt =

SUN4.2_CPPFLAGS_prf =
SUN4.2_CFLAGS_prf = -pg
SUN4.2_C++FLAGS_prf = -pg
SUN4.2_LDFLAGS_prf =
SUN4.2_LDLIBS_prf =

SUN4.2_CPPFLAGS_std =
SUN4.2_CFLAGS_std =
SUN4.2_C++FLAGS_std =
SUN4.2_LDFLAGS_std =
SUN4.2_LDLIBS_std =

define SUN4.2_LIB_ARCHIVE
$(C++) -xar @PTRFLAGS@ -o @LIB@ @ALLOBJECTS@
endef

define SUN4.2_PIC_LIBRARY
$(C++) -G @PIC_PTRFLAGS@ -o @LIB@ $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ \
 $(PIC_LDLIBS) @LDLIBS@ -lC
endef

endif

ifeq "$(C++COMPILERENV)" "OSTORE"

SUN4.2_VARIANTS = dbg opt prf std
SUN4.2_PIC_VARIANTS = opt std

SUN4.2_ENV_OODBMS = OSTORE

SUN4.2_OPT_LIBRARIES =

SUN4.2_C++ = /opt/SUNWspro/SC4.0/bin/CC
SUN4.2_CC = /opt/SUNWspro/SC4.0/bin/cc
SUN4.2_LD = /usr/ccs/bin/ld
SUN4.2_AR = /usr/ccs/bin/ar

SUN4.2_CPPFLAGS =
SUN4.2_CFLAGS =
SUN4.2_C++FLAGS = +p -vdelx
SUN4.2_LDFLAGS =
SUN4.2_LDLIBS = -lsocket -lnsl -ldl

SUN4.2_PIC_CPPFLAGS =
SUN4.2_PIC_CFLAGS = -KPIC
SUN4.2_PIC_C++FLAGS = -KPIC
SUN4.2_PIC_LDFLAGS =
SUN4.2_PIC_LDLIBS =

SUN4.2_CPPFLAGS_dbg =
SUN4.2_CFLAGS_dbg = -g
SUN4.2_C++FLAGS_dbg = -g0
SUN4.2_LDFLAGS_dbg =
SUN4.2_LDLIBS_dbg =

SUN4.2_CPPFLAGS_opt = -DNDEBUG
SUN4.2_CFLAGS_opt = -O
SUN4.2_C++FLAGS_opt = -O
SUN4.2_LDFLAGS_opt =
SUN4.2_LDLIBS_opt =

SUN4.2_CPPFLAGS_prf =
SUN4.2_CFLAGS_prf = -pg
SUN4.2_C++FLAGS_prf = -pg
SUN4.2_LDFLAGS_prf =
SUN4.2_LDLIBS_prf =

SUN4.2_CPPFLAGS_std =
SUN4.2_CFLAGS_std =
SUN4.2_C++FLAGS_std =
SUN4.2_LDFLAGS_std =
SUN4.2_LDLIBS_std =

define SUN4.2_LIB_ARCHIVE
$(C++) -xar @PTRFLAGS@ -o @LIB@ @ALLOBJECTS@
endef

define SUN4.2_PIC_LIBRARY
$(C++) -G @PIC_PTRFLAGS@ -o @LIB@ $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ \
 $(PIC_LDLIBS) @LDLIBS@ -lC
endef

endif

ifeq "$(C++COMPILERENV)" "VERSANT"

SUN4.2_VARIANTS = dbg opt prf std
SUN4.2_PIC_VARIANTS = opt std

SUN4.2_ENV_OODBMS = VERSANT

SUN4.2_OPT_LIBRARIES =

SUN4.2_C++ = /opt/SUNWspro/SC4.0/bin/CC
SUN4.2_CC = /opt/SUNWspro/SC4.0/bin/cc
SUN4.2_LD = /usr/ccs/bin/ld
SUN4.2_AR = /usr/ccs/bin/ar

SUN4.2_CPPFLAGS =
SUN4.2_CFLAGS =
SUN4.2_C++FLAGS =
SUN4.2_LDFLAGS =
SUN4.2_LDLIBS = -lsocket -lnsl -lm -ldl

SUN4.2_PIC_CPPFLAGS =
SUN4.2_PIC_CFLAGS = -KPIC
SUN4.2_PIC_C++FLAGS = -KPIC
SUN4.2_PIC_LDFLAGS =
SUN4.2_PIC_LDLIBS =

SUN4.2_CPPFLAGS_dbg =
SUN4.2_CFLAGS_dbg = -g
SUN4.2_C++FLAGS_dbg = -g
SUN4.2_LDFLAGS_dbg =
SUN4.2_LDLIBS_dbg =

SUN4.2_CPPFLAGS_opt = -DNDEBUG
SUN4.2_CFLAGS_opt = -O
SUN4.2_C++FLAGS_opt = -O
SUN4.2_LDFLAGS_opt =
SUN4.2_LDLIBS_opt =

SUN4.2_CPPFLAGS_prf =
SUN4.2_CFLAGS_prf = -pg
SUN4.2_C++FLAGS_prf = -pg
SUN4.2_LDFLAGS_prf =
SUN4.2_LDLIBS_prf =

SUN4.2_CPPFLAGS_std =
SUN4.2_CFLAGS_std =
SUN4.2_C++FLAGS_std =
SUN4.2_LDFLAGS_std =
SUN4.2_LDLIBS_std =

define SUN4.2_LIB_ARCHIVE
$(C++) -xar @PTRFLAGS@ -o @LIB@ @ALLOBJECTS@
endef

define SUN4.2_PIC_LIBRARY
$(C++) -G @PIC_PTRFLAGS@ -o @LIB@ $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ \
 $(PIC_LDLIBS) @LDLIBS@ -lC
endef

endif

ifeq "$(C++COMPILERENV)" "THREAD"

SUN4.2_VARIANTS = dbg opt std
SUN4.2_PIC_VARIANTS = opt std

SUN4.2_ENV_OODBMS =

SUN4.2_OPT_LIBRARIES =

SUN4.2_C++ = /opt/SUNWspro/SC4.0/bin/CC
SUN4.2_CC = /opt/SUNWspro/SC4.0/bin/cc
SUN4.2_LD = /usr/ccs/bin/ld
SUN4.2_AR = /usr/ccs/bin/ar

SUN4.2_CPPFLAGS = -DHAVE_SOLARIS_THREADS -D_REENTRANT
SUN4.2_CFLAGS =
SUN4.2_C++FLAGS = -mt
SUN4.2_LDFLAGS =
SUN4.2_LDLIBS = -lsocket -lnsl -lthread -ldl

SUN4.2_PIC_CPPFLAGS =
SUN4.2_PIC_CFLAGS = -KPIC
SUN4.2_PIC_C++FLAGS = -KPIC
SUN4.2_PIC_LDFLAGS =
SUN4.2_PIC_LDLIBS =

SUN4.2_CPPFLAGS_dbg =
SUN4.2_CFLAGS_dbg = -g
SUN4.2_C++FLAGS_dbg = -g
SUN4.2_LDFLAGS_dbg =
SUN4.2_LDLIBS_dbg =

SUN4.2_CPPFLAGS_opt = -DNDEBUG
SUN4.2_CFLAGS_opt = -O
SUN4.2_C++FLAGS_opt = -O
SUN4.2_LDFLAGS_opt =
SUN4.2_LDLIBS_opt =

SUN4.2_CPPFLAGS_prf =
SUN4.2_CFLAGS_prf = -pg
SUN4.2_C++FLAGS_prf = -pg
SUN4.2_LDFLAGS_prf =
SUN4.2_LDLIBS_prf =

SUN4.2_CPPFLAGS_std =
SUN4.2_CFLAGS_std =
SUN4.2_C++FLAGS_std =
SUN4.2_LDFLAGS_std =
SUN4.2_LDLIBS_std =

define SUN4.2_LIB_ARCHIVE
$(C++) -xar @PTRFLAGS@ -o @LIB@ @ALLOBJECTS@
endef

define SUN4.2_PIC_LIBRARY
$(C++) -G @PIC_PTRFLAGS@ -o @LIB@ $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ \
 $(PIC_LDLIBS) @LDLIBS@ -lC
endef

endif

ifeq "$(C++COMPILERENV)" "PTHREAD"

SUN4.2_VARIANTS = dbg opt std
SUN4.2_PIC_VARIANTS = opt std

SUN4.2_ENV_OODBMS =

SUN4.2_OPT_LIBRARIES =

SUN4.2_C++ = /opt/SUNWspro/SC4.0/bin/CC
SUN4.2_CC = /opt/SUNWspro/SC4.0/bin/cc
SUN4.2_LD = /usr/ccs/bin/ld
SUN4.2_AR = /usr/ccs/bin/ar

SUN4.2_CPPFLAGS = -DHAVE_POSIX_THREADS -D_REENTRANT
SUN4.2_CFLAGS =
SUN4.2_C++FLAGS = -mt
SUN4.2_LDFLAGS =
SUN4.2_LDLIBS = -lsocket -lnsl -lpthread -ldl

SUN4.2_PIC_CPPFLAGS =
SUN4.2_PIC_CFLAGS = -KPIC
SUN4.2_PIC_C++FLAGS = -KPIC
SUN4.2_PIC_LDFLAGS =
SUN4.2_PIC_LDLIBS =

SUN4.2_CPPFLAGS_dbg =
SUN4.2_CFLAGS_dbg = -g
SUN4.2_C++FLAGS_dbg = -g
SUN4.2_LDFLAGS_dbg =
SUN4.2_LDLIBS_dbg =

SUN4.2_CPPFLAGS_opt = -DNDEBUG
SUN4.2_CFLAGS_opt = -O
SUN4.2_C++FLAGS_opt = -O
SUN4.2_LDFLAGS_opt =
SUN4.2_LDLIBS_opt =

SUN4.2_CPPFLAGS_prf =
SUN4.2_CFLAGS_prf = -pg
SUN4.2_C++FLAGS_prf = -pg
SUN4.2_LDFLAGS_prf =
SUN4.2_LDLIBS_prf =

SUN4.2_CPPFLAGS_std =
SUN4.2_CFLAGS_std =
SUN4.2_C++FLAGS_std =
SUN4.2_LDFLAGS_std =
SUN4.2_LDLIBS_std =

define SUN4.2_LIB_ARCHIVE
$(C++) -xar @PTRFLAGS@ -o @LIB@ @ALLOBJECTS@
endef

define SUN4.2_PIC_LIBRARY
$(C++) -G @PIC_PTRFLAGS@ -o @LIB@ $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ \
 $(PIC_LDLIBS) @LDLIBS@ -lC
endef

endif

# SUN C++ 5.2.

ifeq "$(C++COMPILERENV)" ""

SUN5.2_VARIANTS = dbg opt prf std
SUN5.2_PIC_VARIANTS = opt std

SUN5.2_ENV_OODBMS =

SUN5.2_OPT_LIBRARIES =

SUN5.2_C++ = /opt/SUNWspro/bin/CC
SUN5.2_CC = /opt/SUNWspro/bin/cc
SUN5.2_LD = /usr/ccs/bin/ld
SUN5.2_AR = /usr/ccs/bin/ar

SUN5.2_CPPFLAGS =
SUN5.2_CFLAGS =
SUN5.2_C++FLAGS = -compat=5
SUN5.2_LDFLAGS =
SUN5.2_LDLIBS = -lsocket -lnsl -ldl

SUN5.2_PIC_CPPFLAGS =
SUN5.2_PIC_CFLAGS = -KPIC
SUN5.2_PIC_C++FLAGS = -KPIC
SUN5.2_PIC_LDFLAGS =
SUN5.2_PIC_LDLIBS =

SUN5.2_CPPFLAGS_dbg =
SUN5.2_CFLAGS_dbg = -g
SUN5.2_C++FLAGS_dbg = -g
SUN5.2_LDFLAGS_dbg =
SUN5.2_LDLIBS_dbg =

SUN5.2_CPPFLAGS_opt = -DNDEBUG
SUN5.2_CFLAGS_opt = -O
SUN5.2_C++FLAGS_opt = -O
SUN5.2_LDFLAGS_opt =
SUN5.2_LDLIBS_opt =

SUN5.2_CPPFLAGS_prf =
SUN5.2_CFLAGS_prf = -pg
SUN5.2_C++FLAGS_prf = -pg
SUN5.2_LDFLAGS_prf =
SUN5.2_LDLIBS_prf =

SUN5.2_CPPFLAGS_std =
SUN5.2_CFLAGS_std =
SUN5.2_C++FLAGS_std =
SUN5.2_LDFLAGS_std =
SUN5.2_LDLIBS_std =

define SUN5.2_LIB_ARCHIVE
$(C++) -xar -o @LIB@ @ALLOBJECTS@
endef

define SUN5.2_PIC_ARCHIVE
$(C++) -xar -o @LIB@ @ALLOBJECTS@
endef

define SUN5.2_PIC_LIBRARY
$(C++) -G -o @LIB@ $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ \
 $(PIC_LDLIBS) @LDLIBS@ -lCstd -lCrun
endef

endif

ifeq "$(C++COMPILERENV)" "THREAD"

SUN5.2_VARIANTS = dbg opt prf std
SUN5.2_PIC_VARIANTS = opt std

SUN5.2_ENV_OODBMS =

SUN5.2_OPT_LIBRARIES =

SUN5.2_C++ = /opt/SUNWspro/bin/CC
SUN5.2_CC = /opt/SUNWspro/bin/cc
SUN5.2_LD = /usr/ccs/bin/ld
SUN5.2_AR = /usr/ccs/bin/ar

SUN5.2_CPPFLAGS = -DHAVE_SOLARIS_THREADS -D_REENTRANT
SUN5.2_CFLAGS =
SUN5.2_C++FLAGS = -compat=5 -mt
SUN5.2_LDFLAGS =
SUN5.2_LDLIBS = -lsocket -lnsl -lthread -ldl

SUN5.2_PIC_CPPFLAGS =
SUN5.2_PIC_CFLAGS = -KPIC
SUN5.2_PIC_C++FLAGS = -KPIC
SUN5.2_PIC_LDFLAGS =
SUN5.2_PIC_LDLIBS =

SUN5.2_CPPFLAGS_dbg =
SUN5.2_CFLAGS_dbg = -g
SUN5.2_C++FLAGS_dbg = -g
SUN5.2_LDFLAGS_dbg =
SUN5.2_LDLIBS_dbg =

SUN5.2_CPPFLAGS_opt = -DNDEBUG
SUN5.2_CFLAGS_opt = -O
SUN5.2_C++FLAGS_opt = -O
SUN5.2_LDFLAGS_opt =
SUN5.2_LDLIBS_opt =

SUN5.2_CPPFLAGS_prf =
SUN5.2_CFLAGS_prf = -pg
SUN5.2_C++FLAGS_prf = -pg
SUN5.2_LDFLAGS_prf =
SUN5.2_LDLIBS_prf =

SUN5.2_CPPFLAGS_std =
SUN5.2_CFLAGS_std =
SUN5.2_C++FLAGS_std =
SUN5.2_LDFLAGS_std =
SUN5.2_LDLIBS_std =

define SUN5.2_LIB_ARCHIVE
$(C++) -xar -o @LIB@ @ALLOBJECTS@
endef

define SUN5.2_PIC_ARCHIVE
$(C++) -xar -o @LIB@ @ALLOBJECTS@
endef

define SUN5.2_PIC_LIBRARY
$(C++) -G -o @LIB@ $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ \
 $(PIC_LDLIBS) @LDLIBS@ -lCstd -lCrun
endef

endif

ifeq "$(C++COMPILERENV)" "PTHREAD"

SUN5.2_VARIANTS = dbg opt prf std
SUN5.2_PIC_VARIANTS = opt std

SUN5.2_ENV_OODBMS =

SUN5.2_OPT_LIBRARIES =

SUN5.2_C++ = /opt/SUNWspro/bin/CC
SUN5.2_CC = /opt/SUNWspro/bin/cc
SUN5.2_LD = /usr/ccs/bin/ld
SUN5.2_AR = /usr/ccs/bin/ar

SUN5.2_CPPFLAGS = -DHAVE_POSIX_THREADS -D_REENTRANT
SUN5.2_CFLAGS =
SUN5.2_C++FLAGS = -compat=5 -mt
SUN5.2_LDFLAGS =
SUN5.2_LDLIBS = -lsocket -lnsl -lpthread -ldl

SUN5.2_PIC_CPPFLAGS =
SUN5.2_PIC_CFLAGS = -KPIC
SUN5.2_PIC_C++FLAGS = -KPIC
SUN5.2_PIC_LDFLAGS =
SUN5.2_PIC_LDLIBS =

SUN5.2_CPPFLAGS_dbg =
SUN5.2_CFLAGS_dbg = -g
SUN5.2_C++FLAGS_dbg = -g
SUN5.2_LDFLAGS_dbg =
SUN5.2_LDLIBS_dbg =

SUN5.2_CPPFLAGS_opt = -DNDEBUG
SUN5.2_CFLAGS_opt = -O
SUN5.2_C++FLAGS_opt = -O
SUN5.2_LDFLAGS_opt =
SUN5.2_LDLIBS_opt =

SUN5.2_CPPFLAGS_prf =
SUN5.2_CFLAGS_prf = -pg
SUN5.2_C++FLAGS_prf = -pg
SUN5.2_LDFLAGS_prf =
SUN5.2_LDLIBS_prf =

SUN5.2_CPPFLAGS_std =
SUN5.2_CFLAGS_std =
SUN5.2_C++FLAGS_std =
SUN5.2_LDFLAGS_std =
SUN5.2_LDLIBS_std =

define SUN5.2_LIB_ARCHIVE
$(C++) -xar -o @LIB@ @ALLOBJECTS@
endef

define SUN5.2_PIC_ARCHIVE
$(C++) -xar -o @LIB@ @ALLOBJECTS@
endef

define SUN5.2_PIC_LIBRARY
$(C++) -G -o @LIB@ $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ \
 $(PIC_LDLIBS) @LDLIBS@ -lCstd -lCrun
endef

endif
