#
# General definitions.
#

# System type.

ENV_SYSTEM = UNIX

# Installation directory.

#OSE_ROOT_DIRECTORY = /usr/local/ose
#OSE_RELEASE_DIRECTORY = tst

# Default and desired C++ compilers.

DEFAULTC++COMPILER = PTHREAD-GNU2.96
C++COMPILERS = PTHREAD-GNU2.96 GNU2.96

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
exit 1; *** do not know how to create shared library ***
endef

#
# Compiler definitions.
#

# GNU C++ 2.7.2.

ifeq "$(C++COMPILERENV)" ""

GNU2.7.2_VARIANTS = dbg opt prf std
GNU2.7.2_PIC_VARIANTS = opt std

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

define GNU2.7.2_PIC_LIBRARY
$(C++) -shared -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

# GNU EGCS C++ 1.1.2.

ifeq "$(C++COMPILERENV)" ""

EGCS1.1_VARIANTS = dbg opt prf std
EGCS1.1_PIC_VARIANTS = opt std

EGCS1.1_ENV_OODBMS =

EGCS1.1_OPT_LIBRARIES =

#EGCS1.1_C++ = /usr/bin/g++
EGCS1.1_C++ = /usr/bin/g++ -Vegcs-2.91.66 -D__cplusplus
#EGCS1.1_C++ = /usr/bin/egcs++
#EGCS1.1_C++ = /usr/bin/compat-egcs++
#EGCS1.1_CC = /usr/bin/gcc
EGCS1.1_CC = /usr/bin/gcc -Vegcs-2.91.66
#EGCS1.1_CC = /usr/bin/egcs
#EGCS1.1_CC = /usr/bin/compat-egcs
EGCS1.1_LD = /usr/bin/ld
EGCS1.1_AR = /usr/bin/ar

EGCS1.1_CPPFLAGS =
EGCS1.1_CFLAGS = -pipe
EGCS1.1_C++FLAGS = -pipe -Winline
EGCS1.1_LDFLAGS =
EGCS1.1_LDLIBS = -lm

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

define EGCS1.1_PIC_LIBRARY
$(C++) -shared -o @LIB@ $(PIC_LDFLAGS) \
 @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

ifeq "$(C++COMPILERENV)" "REPO"

EGCS1.1_VARIANTS = dbg opt prf std
EGCS1.1_PIC_VARIANTS = opt std

EGCS1.1_ENV_OODBMS =

EGCS1.1_OPT_LIBRARIES =

EGCS1.1_C++ = /usr/bin/g++
#EGCS1.1_C++ = /usr/bin/g++ -Vegcs-2.91.66 -D_cplusplus
#EGCS1.1_C++ = /usr/bin/egcs++
#EGCS1.1_C++ = /usr/bin/compat-egcs++
EGCS1.1_CC = /usr/bin/gcc
#EGCS1.1_CC = /usr/bin/gcc -Vegcs-2.91.66
#EGCS1.1_CC = /usr/bin/egcs
#EGCS1.1_CC = /usr/bin/compat-egcs
EGCS1.1_LD = /usr/bin/ld
EGCS1.1_AR = /usr/bin/ar

EGCS1.1_CPPFLAGS =
EGCS1.1_CFLAGS = -pipe
EGCS1.1_C++FLAGS = -pipe -Winline -frepo
EGCS1.1_LDFLAGS =
EGCS1.1_LDLIBS = -lm

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

define EGCS1.1_PIC_LIBRARY
$(C++) -shared -o @LIB@ $(PIC_LDFLAGS) \
 @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

ifeq "$(C++COMPILERENV)" "PTHREAD"

EGCS1.1_VARIANTS = dbg opt prf std
EGCS1.1_PIC_VARIANTS = opt std

EGCS1.1_ENV_OODBMS =

EGCS1.1_OPT_LIBRARIES =

#EGCS1.1_C++ = /usr/bin/g++
EGCS1.1_C++ = /usr/bin/g++ -Vegcs-2.91.66 -D__cplusplus
#EGCS1.1_C++ = /usr/bin/egcs++
#EGCS1.1_C++ = /usr/bin/compat-egcs++
#EGCS1.1_CC = /usr/bin/gcc
EGCS1.1_CC = /usr/bin/gcc -Vegcs-2.91.66
#EGCS1.1_CC = /usr/bin/egcs
#EGCS1.1_CC = /usr/bin/compat-egcs
EGCS1.1_LD = /usr/bin/ld
EGCS1.1_AR = /usr/bin/ar

EGCS1.1_CPPFLAGS = -DHAVE_POSIX_THREADS
EGCS1.1_CFLAGS = -pipe
EGCS1.1_C++FLAGS = -pipe
EGCS1.1_LDFLAGS =
EGCS1.1_LDLIBS = -lm -lpthread

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

define EGCS1.1_PIC_LIBRARY
$(C++) -shared -o @LIB@ $(PIC_LDFLAGS) \
 @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

ifeq "$(C++COMPILERENV)" "PENTIUM"

EGCS1.1_VARIANTS = dbg opt prf std
EGCS1.1_PIC_VARIANTS = opt std

EGCS1.1_ENV_OODBMS =

EGCS1.1_OPT_LIBRARIES =

#EGCS1.1_C++ = /usr/bin/g++
EGCS1.1_C++ = /usr/bin/g++ -Vegcs-2.91.66 -D__cplusplus
#EGCS1.1_C++ = /usr/bin/egcs++
#EGCS1.1_C++ = /usr/bin/compat-egcs++
#EGCS1.1_CC = /usr/bin/gcc
EGCS1.1_CC = /usr/bin/gcc -Vegcs-2.91.66
#EGCS1.1_CC = /usr/bin/egcs
#EGCS1.1_CC = /usr/bin/compat-egcs
EGCS1.1_LD = /usr/bin/ld
EGCS1.1_AR = /usr/bin/ar

EGCS1.1_CPPFLAGS = -DHAVE_POSIX_THREADS
EGCS1.1_CFLAGS = -pipe
EGCS1.1_C++FLAGS = -pipe
EGCS1.1_LDFLAGS =
EGCS1.1_LDLIBS = -lm -lpthread

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
EGCS1.1_CFLAGS_opt = -O3 -march=pentiumpro -Winline
EGCS1.1_C++FLAGS_opt = -O3 -march=pentiumpro -Winline
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
$(C++) -shared -o @LIB@ $(PIC_LDFLAGS) \
 @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

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

ifeq "$(C++COMPILERENV)" "OSTORE"

GNU2.95.3_VARIANTS = dbg opt prf std
GNU2.95.3_PIC_VARIANTS = opt std

GNU2.95.3_ENV_OODBMS = OSTORE

GNU2.95.3_OPT_LIBRARIES =

GNU2.95.3_C++ = /usr/bin/g++ -D__cplusplus
GNU2.95.3_CC = /usr/bin/gcc
GNU2.95.3_LD = /usr/bin/ld
GNU2.95.3_AR = /usr/bin/ar

GNU2.95.3_CPPFLAGS = -DHAVE_POSIX_THREADS
GNU2.95.3_CFLAGS = -pipe
GNU2.95.3_C++FLAGS = -pipe -Winline
GNU2.95.3_LDFLAGS =
GNU2.95.3_LDLIBS = -lm -lpthread -lrt

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

# GNU C++ 2.96.

ifeq "$(C++COMPILERENV)" ""

GNU2.96_VARIANTS = dbg opt prf std
GNU2.96_PIC_VARIANTS = opt std

GNU2.96_ENV_OODBMS =

GNU2.96_OPT_LIBRARIES =

GNU2.96_C++ = /usr/bin/g++
GNU2.96_CC = /usr/bin/gcc
GNU2.96_LD = /usr/bin/ld
GNU2.96_AR = /usr/bin/ar

GNU2.96_CPPFLAGS =
GNU2.96_CFLAGS = -pipe
GNU2.96_C++FLAGS = -pipe -Winline
GNU2.96_LDFLAGS =
GNU2.96_LDLIBS = -lm

GNU2.96_PIC_CPPFLAGS =
GNU2.96_PIC_CFLAGS =
GNU2.96_PIC_C++FLAGS =
GNU2.96_PIC_LDFLAGS =
GNU2.96_PIC_LDLIBS =

GNU2.96_CPPFLAGS_dbg =
GNU2.96_CFLAGS_dbg = -g
GNU2.96_C++FLAGS_dbg = -g
GNU2.96_LDFLAGS_dbg =
GNU2.96_LDLIBS_dbg =

GNU2.96_CPPFLAGS_opt = -DNDEBUG
GNU2.96_CFLAGS_opt = -O
GNU2.96_C++FLAGS_opt = -O
GNU2.96_LDFLAGS_opt =
GNU2.96_LDLIBS_opt =

GNU2.96_CPPFLAGS_prf =
GNU2.96_CFLAGS_prf = -pg
GNU2.96_C++FLAGS_prf = -pg
GNU2.96_LDFLAGS_prf =
GNU2.96_LDLIBS_prf =

GNU2.96_CPPFLAGS_std =
GNU2.96_CFLAGS_std =
GNU2.96_C++FLAGS_std =
GNU2.96_LDFLAGS_std =
GNU2.96_LDLIBS_std =

define GNU2.96_PIC_LIBRARY
$(C++) -shared -o @LIB@ $(PIC_LDFLAGS) \
 @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

ifeq "$(C++COMPILERENV)" "REPO"

GNU2.96_VARIANTS = dbg opt prf std
GNU2.96_PIC_VARIANTS = opt std

GNU2.96_ENV_OODBMS =

GNU2.96_OPT_LIBRARIES =

GNU2.96_C++ = /usr/bin/g++
GNU2.96_CC = /usr/bin/gcc
GNU2.96_LD = /usr/bin/ld
GNU2.96_AR = /usr/bin/ar

GNU2.96_CPPFLAGS =
GNU2.96_CFLAGS = -pipe
GNU2.96_C++FLAGS = -pipe -Winline -frepo
GNU2.96_LDFLAGS =
GNU2.96_LDLIBS = -lm

GNU2.96_PIC_CPPFLAGS =
GNU2.96_PIC_CFLAGS =
GNU2.96_PIC_C++FLAGS =
GNU2.96_PIC_LDFLAGS =
GNU2.96_PIC_LDLIBS =

GNU2.96_CPPFLAGS_dbg =
GNU2.96_CFLAGS_dbg = -g
GNU2.96_C++FLAGS_dbg = -g
GNU2.96_LDFLAGS_dbg =
GNU2.96_LDLIBS_dbg =

GNU2.96_CPPFLAGS_opt = -DNDEBUG
GNU2.96_CFLAGS_opt = -O
GNU2.96_C++FLAGS_opt = -O
GNU2.96_LDFLAGS_opt =
GNU2.96_LDLIBS_opt =

GNU2.96_CPPFLAGS_prf =
GNU2.96_CFLAGS_prf = -pg
GNU2.96_C++FLAGS_prf = -pg
GNU2.96_LDFLAGS_prf =
GNU2.96_LDLIBS_prf =

GNU2.96_CPPFLAGS_std =
GNU2.96_CFLAGS_std =
GNU2.96_C++FLAGS_std =
GNU2.96_LDFLAGS_std =
GNU2.96_LDLIBS_std =

define GNU2.96_LIB_CLOSURE
$(RM) $(MK)/__ptdummy0.cc; \
echo "int main() { return 0; }" > $(MK)/__ptdummy0.cc; \
$(C++) $(MK)/__ptdummy0.cc -o $(MK)/__ptdummy0.exe \
 @ALLOBJECTS@ @LDFLAGS@ @LDLIBS@; \
$(RM) $(MK)/__ptdummy0.*
endef

define GNU2.96_PIC_CLOSURE
$(RM) $(MK)/__ptdummy0.cc; \
echo "int main() { return 0; }" > $(MK)/__ptdummy0.cc; \
$(C++) $(MK)/__ptdummy0.cc -o $(MK)/__ptdummy0.exe \
 @ALLOBJECTS@ @LDFLAGS@ @LDLIBS@; \
$(RM) $(MK)/__ptdummy0.*
endef

define GNU2.96_PIC_LIBRARY
$(C++) -shared -o @LIB@ $(PIC_LDFLAGS) \
 @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

ifeq "$(C++COMPILERENV)" "PTHREAD"

GNU2.96_VARIANTS = dbg opt prf std
GNU2.96_PIC_VARIANTS = opt std

GNU2.96_ENV_OODBMS =

GNU2.96_OPT_LIBRARIES = OTC-PY

GNU2.96_C++ = /usr/bin/g++
GNU2.96_CC = /usr/bin/gcc
GNU2.96_LD = /usr/bin/ld
GNU2.96_AR = /usr/bin/ar

GNU2.96_CPPFLAGS = -DHAVE_POSIX_THREADS
GNU2.96_CFLAGS = -pipe
GNU2.96_C++FLAGS = -pipe
GNU2.96_LDFLAGS =
GNU2.96_LDLIBS = -lm -lpthread

GNU2.96_PIC_CPPFLAGS =
GNU2.96_PIC_CFLAGS =
GNU2.96_PIC_C++FLAGS =
GNU2.96_PIC_LDFLAGS =
GNU2.96_PIC_LDLIBS =

GNU2.96_CPPFLAGS_dbg =
GNU2.96_CFLAGS_dbg = -g
GNU2.96_C++FLAGS_dbg = -g
GNU2.96_LDFLAGS_dbg =
GNU2.96_LDLIBS_dbg =

GNU2.96_CPPFLAGS_opt = -DNDEBUG
GNU2.96_CFLAGS_opt = -O
GNU2.96_C++FLAGS_opt = -O
GNU2.96_LDFLAGS_opt =
GNU2.96_LDLIBS_opt =

GNU2.96_CPPFLAGS_prf =
GNU2.96_CFLAGS_prf = -pg
GNU2.96_C++FLAGS_prf = -pg
GNU2.96_LDFLAGS_prf =
GNU2.96_LDLIBS_prf =

GNU2.96_CPPFLAGS_std =
GNU2.96_CFLAGS_std =
GNU2.96_C++FLAGS_std =
GNU2.96_LDFLAGS_std =
GNU2.96_LDLIBS_std =

define GNU2.96_PIC_LIBRARY
$(C++) -shared -o @LIB@ $(PIC_LDFLAGS) \
 @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

endif

# KAI C++ 3.4

KCC_LICENSE = CXP-0282-XLK-000628-68783
export KCC_LICENSE

ifeq "$(C++COMPILERENV)" ""

KAI3.4_VARIANTS = dbg opt prf std
KAI3.4_PIC_VARIANTS = opt std

KAI3.4_ENV_OODBMS =

KAI3.4_OPT_LIBRARIES =

KAI3.4_C++ = /usr/local/KAI/KCC.pu-3.4g-1/KCC_BASE/bin/KCC
KAI3.4_CC = /usr/local/KAI/KCC.pu-3.4g-1/KCC_BASE/bin/KCC
KAI3.4_LD = /usr/bin/ld
KAI3.4_AR = /usr/bin/ar

KAI3.4_CPPFLAGS =
KAI3.4_CFLAGS = --c
KAI3.4_C++FLAGS = --c++ --one_instantiation_per_object \
 --exceptions -DHAVE_BOOL -DENV_CXXSTDLIB
KAI3.4_LDFLAGS =
KAI3.4_LDLIBS = -lm

KAI3.4_PIC_CPPFLAGS =
KAI3.4_PIC_CFLAGS =
KAI3.4_PIC_C++FLAGS =
KAI3.4_PIC_LDFLAGS =
KAI3.4_PIC_LDLIBS =

KAI3.4_CPPFLAGS_dbg =
KAI3.4_CFLAGS_dbg = -g
KAI3.4_C++FLAGS_dbg = -g
KAI3.4_LDFLAGS_dbg =
KAI3.4_LDLIBS_dbg =

KAI3.4_CPPFLAGS_opt = -DNDEBUG
KAI3.4_CFLAGS_opt = +K3 -O --abstract_pointer --abstract_float
KAI3.4_C++FLAGS_opt = +K3 -O --abstract_pointer --abstract_float
KAI3.4_LDFLAGS_opt =
KAI3.4_LDLIBS_opt =

KAI3.4_CPPFLAGS_prf =
KAI3.4_CFLAGS_prf = -p
KAI3.4_C++FLAGS_prf = -p
KAI3.4_LDFLAGS_prf =
KAI3.4_LDLIBS_prf =

KAI3.4_CPPFLAGS_std =
KAI3.4_CFLAGS_std =
KAI3.4_C++FLAGS_std =
KAI3.4_LDFLAGS_std =
KAI3.4_LDLIBS_std =

define KAI3.4_LIB_ARCHIVE
$(C++) -o @LIB@  @ALLOBJECTS@ $(filter $(LOPT)%,$(LDFLAGS)) \
 $(filter -l% %$(LIBEXT),$(LDLIBS))
endef

define KAI3.4_PIC_LIBRARY
$(C++) -o @LIB@ $(PIC_LDFLAGS) $(filter $(LOPT)%,$(LDFLAGS)) \
 @OBJECTS@ $(PIC_LDLIBS) $(filter -l% %$(LIBEXT),$(LDLIBS))
endef

endif

ifeq "$(C++COMPILERENV)" "PTHREAD"

KAI3.4_VARIANTS = dbg opt prf std
KAI3.4_PIC_VARIANTS = opt std

KAI3.4_ENV_OODBMS =

KAI3.4_OPT_LIBRARIES =

KAI3.4_C++ = /usr/local/KAI/KCC.pu-3.4g-1/KCC_BASE/bin/KCC
KAI3.4_CC = /usr/local/KAI/KCC.pu-3.4g-1/KCC_BASE/bin/KCC
KAI3.4_LD = /usr/bin/ld
KAI3.4_AR = /usr/bin/ar

KAI3.4_CPPFLAGS = -DHAVE_POSIX_THREADS
KAI3.4_CFLAGS = --c
KAI3.4_C++FLAGS = --c++ --one_instantiation_per_object \
 --exceptions -DHAVE_BOOL -DENV_CXXSTDLIB
KAI3.4_LDFLAGS =
KAI3.4_LDLIBS = -lm -lpthread

KAI3.4_PIC_CPPFLAGS =
KAI3.4_PIC_CFLAGS =
KAI3.4_PIC_C++FLAGS =
KAI3.4_PIC_LDFLAGS =
KAI3.4_PIC_LDLIBS =

KAI3.4_CPPFLAGS_dbg =
KAI3.4_CFLAGS_dbg = -g
KAI3.4_C++FLAGS_dbg = -g
KAI3.4_LDFLAGS_dbg =
KAI3.4_LDLIBS_dbg =

KAI3.4_CPPFLAGS_opt = -DNDEBUG
KAI3.4_CFLAGS_opt = +K3 -O --abstract_pointer --abstract_float
KAI3.4_C++FLAGS_opt = +K3 -O --abstract_pointer --abstract_float
KAI3.4_LDFLAGS_opt =
KAI3.4_LDLIBS_opt =

KAI3.4_CPPFLAGS_prf =
KAI3.4_CFLAGS_prf = -p
KAI3.4_C++FLAGS_prf = -p
KAI3.4_LDFLAGS_prf =
KAI3.4_LDLIBS_prf =

KAI3.4_CPPFLAGS_std =
KAI3.4_CFLAGS_std =
KAI3.4_C++FLAGS_std =
KAI3.4_LDFLAGS_std =
KAI3.4_LDLIBS_std =

define KAI3.4_LIB_ARCHIVE
$(C++) -o @LIB@  @ALLOBJECTS@ $(filter $(LOPT)%,$(LDFLAGS)) \
 $(filter -l% %$(LIBEXT),$(LDLIBS))
endef

define KAI3.4_PIC_LIBRARY
$(C++) -o @LIB@ $(PIC_LDFLAGS) $(filter $(LOPT)%,$(LDFLAGS)) \
 @OBJECTS@ $(PIC_LDLIBS) $(filter -l% %$(LIBEXT),$(LDLIBS))
endef

endif
