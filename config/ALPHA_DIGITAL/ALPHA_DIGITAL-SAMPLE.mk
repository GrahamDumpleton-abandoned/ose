#
# General definitions.
#

# System type.

ENV_SYSTEM = UNIX

# Installation directory.

#OSE_ROOT_DIRECTORY = /usr/local/ose
#OSE_RELEASE_DIRECTORY = tst

# Default and desired C++ compilers.

DEFAULTC++COMPILER = DEC6.2
C++COMPILERS = DEC6.2

# Default and desired compilation variants.

#DEFAULTVARIANT = dbg
DEFAULTVARIANT = std
#VARIANTS = dbg opt prf std
VARIANTS = std

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
$(LD) -shared -soname lib$(LIBRARY)$(SHLEXT).$(LIBVERSION) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ $(PIC_LDLIBS) @LDLIBS@
endef

#
# Compiler definitions.
#

# DEC C++ 5.4.

ifeq "$(C++COMPILERENV)" ""

DEC5.4_VARIANTS = dbg opt prf std
DEC5.4_PIC_VARIANTS = opt std

DEC5.4_ENV_OODBMS =

DEC5.4_OPT_LIBRARIES =

DEC5.4_C++ = /usr/bin/cxx
DEC5.4_CC = /usr/bin/cc
DEC5.4_LD = /usr/bin/ld
DEC5.4_AR = /usr/bin/ar

DEC5.4_CPPFLAGS =
DEC5.4_CFLAGS =
DEC5.4_C++FLAGS =
DEC5.4_LDFLAGS = -rpath $$LD_RUN_PATH
DEC5.4_LDLIBS =

DEC5.4_PIC_CPPFLAGS =
DEC5.4_PIC_CFLAGS =
DEC5.4_PIC_C++FLAGS =
DEC5.4_PIC_LDFLAGS =
DEC5.4_PIC_LDLIBS =

DEC5.4_CPPFLAGS_dbg =
DEC5.4_CFLAGS_dbg = -g
DEC5.4_C++FLAGS_dbg = -g
DEC5.4_LDFLAGS_dbg =
DEC5.4_LDLIBS_dbg =

DEC5.4_CPPFLAGS_opt = -DNDEBUG
DEC5.4_CFLAGS_opt = -O
DEC5.4_C++FLAGS_opt = -O
DEC5.4_LDFLAGS_opt =
DEC5.4_LDLIBS_opt =

DEC5.4_CPPFLAGS_prf =
DEC5.4_CFLAGS_prf = -pg
DEC5.4_C++FLAGS_prf = -pg
DEC5.4_LDFLAGS_prf =
DEC5.4_LDLIBS_prf =

DEC5.4_CPPFLAGS_std =
DEC5.4_CFLAGS_std =
DEC5.4_C++FLAGS_std =
DEC5.4_LDFLAGS_std =
DEC5.4_LDLIBS_std =

endif

ifeq "$(C++COMPILERENV)" "PTHREAD"

DEC5.4_VARIANTS = dbg opt prf std
DEC5.4_PIC_VARIANTS = opt std

DEC5.4_ENV_OODBMS =

DEC5.4_OPT_LIBRARIES =

DEC5.4_C++ = /usr/bin/cxx
DEC5.4_CC = /usr/bin/cc
DEC5.4_LD = /usr/bin/ld
DEC5.4_AR = /usr/bin/ar

DEC5.4_CPPFLAGS = -DHAVE_POSIX_THREADS -D_REENTRANT -D_PTHREAD_USE_D4
#DEC5.4_CPPFLAGS = -DHAVE_POSIX_THREADS -D_REENTRANT
DEC5.4_CFLAGS =
DEC5.4_C++FLAGS =
DEC5.4_LDFLAGS = -rpath $$LD_RUN_PATH
DEC5.4_LDLIBS = -threads
#DEC5.4_LDLIBS = -pthread

DEC5.4_PIC_CPPFLAGS =
DEC5.4_PIC_CFLAGS =
DEC5.4_PIC_C++FLAGS =
DEC5.4_PIC_LDFLAGS =
DEC5.4_PIC_LDLIBS =

DEC5.4_CPPFLAGS_dbg =
DEC5.4_CFLAGS_dbg = -g
DEC5.4_C++FLAGS_dbg = -g
DEC5.4_LDFLAGS_dbg =
DEC5.4_LDLIBS_dbg =

DEC5.4_CPPFLAGS_opt = -DNDEBUG
DEC5.4_CFLAGS_opt = -O
DEC5.4_C++FLAGS_opt = -O
DEC5.4_LDFLAGS_opt =
DEC5.4_LDLIBS_opt =

DEC5.4_CPPFLAGS_prf =
DEC5.4_CFLAGS_prf = -pg
DEC5.4_C++FLAGS_prf = -pg
DEC5.4_LDFLAGS_prf =
DEC5.4_LDLIBS_prf =

DEC5.4_CPPFLAGS_std =
DEC5.4_CFLAGS_std =
DEC5.4_C++FLAGS_std =
DEC5.4_LDFLAGS_std =
DEC5.4_LDLIBS_std =

endif

ifeq "$(C++COMPILERENV)" "OSTORE"

DEC5.4_VARIANTS = dbg opt prf std
DEC5.4_PIC_VARIANTS = opt std

DEC5.4_ENV_OODBMS = OSTORE

DEC5.4_OPT_LIBRARIES =

DEC5.4_C++ = /usr/bin/cxx
DEC5.4_CC = /usr/bin/cc
DEC5.4_LD = /usr/bin/ld
DEC5.4_AR = /usr/bin/ar

DEC5.4_CPPFLAGS =
DEC5.4_CFLAGS = -xtaso_short
DEC5.4_C++FLAGS = -xtaso_short -vptr_size_short
DEC5.4_LDFLAGS = -rpath $$LD_RUN_PATH
DEC5.4_LDLIBS =

DEC5.4_PIC_CPPFLAGS =
DEC5.4_PIC_CFLAGS =
DEC5.4_PIC_C++FLAGS =
DEC5.4_PIC_LDFLAGS =
DEC5.4_PIC_LDLIBS = -taso

DEC5.4_CPPFLAGS_dbg =
DEC5.4_CFLAGS_dbg = -g
DEC5.4_C++FLAGS_dbg = -g
DEC5.4_LDFLAGS_dbg =
DEC5.4_LDLIBS_dbg =

DEC5.4_CPPFLAGS_opt = -DNDEBUG
DEC5.4_CFLAGS_opt = -O
DEC5.4_C++FLAGS_opt = -O
DEC5.4_LDFLAGS_opt =
DEC5.4_LDLIBS_opt =

DEC5.4_CPPFLAGS_prf =
DEC5.4_CFLAGS_prf = -pg
DEC5.4_C++FLAGS_prf = -pg
DEC5.4_LDFLAGS_prf =
DEC5.4_LDLIBS_prf =

DEC5.4_CPPFLAGS_std =
DEC5.4_CFLAGS_std =
DEC5.4_C++FLAGS_std =
DEC5.4_LDFLAGS_std =
DEC5.4_LDLIBS_std =

endif

# DEC C++ 6.2.

ifeq "$(C++COMPILERENV)" ""

DEC6.2_VARIANTS = dbg opt prf std
DEC6.2_PIC_VARIANTS = opt std

DEC6.2_ENV_OODBMS =

DEC6.2_OPT_LIBRARIES =

DEC6.2_C++ = /usr/bin/cxx
DEC6.2_CC = /usr/bin/cc
DEC6.2_LD = /usr/bin/ld
DEC6.2_AR = /usr/bin/ar

DEC6.2_CPPFLAGS =
DEC6.2_CFLAGS =
DEC6.2_C++FLAGS =
DEC6.2_LDFLAGS = -rpath $$LD_RUN_PATH
DEC6.2_LDLIBS =

DEC6.2_PIC_CPPFLAGS =
DEC6.2_PIC_CFLAGS =
DEC6.2_PIC_C++FLAGS =
DEC6.2_PIC_LDFLAGS =
DEC6.2_PIC_LDLIBS = -lcxx -lc

DEC6.2_CPPFLAGS_dbg =
DEC6.2_CFLAGS_dbg = -g
DEC6.2_C++FLAGS_dbg = -g
DEC6.2_LDFLAGS_dbg =
DEC6.2_LDLIBS_dbg =

DEC6.2_CPPFLAGS_opt = -DNDEBUG
DEC6.2_CFLAGS_opt = -O
DEC6.2_C++FLAGS_opt = -O
DEC6.2_LDFLAGS_opt =
DEC6.2_LDLIBS_opt =

DEC6.2_CPPFLAGS_prf =
DEC6.2_CFLAGS_prf = -pg
DEC6.2_C++FLAGS_prf = -pg
DEC6.2_LDFLAGS_prf =
DEC6.2_LDLIBS_prf =

DEC6.2_CPPFLAGS_std =
DEC6.2_CFLAGS_std =
DEC6.2_C++FLAGS_std =
DEC6.2_LDFLAGS_std =
DEC6.2_LDLIBS_std =

# Following requires you to have DEC UNIX 5.0.
# If you only have DEC UNIX 4.0, comment out
# all the define/endef style macro definitions.

define DEC6.2_LIB_CLOSURE
$(RM) $(MK)/__ptdummy0.cc; \
echo "int main() { return 0; }" > $(MK)/__ptdummy0.cc; \
$(C++) $(MK)/__ptdummy0.cc -o $(MK)/__ptdummy0.exe \
 -ptr $(MKPTR) @ALLOBJECTS@ @LDFLAGS@ @LDLIBS@; \
$(RM) $(MK)/__ptdummy0.*
endef

define DEC6.2_COMBINE_LIB_CLOSURE
endef

#define DEC6.2_LIB_ARCHIVE
#ls $(MKPTR) | sed -n -e '/^.*\.o$$/p' | \
# sed -e 's/^/$(MKPTR)\//' > $(MK)/ptr.lst
#$(AR) rv @LIB@ @OBJECTS@ -input $(MK)/ptr.lst
#endef

define DEC6.2_LIB_ARCHIVE
ls $(MKPTR) | sed -n -e '/^.*\.o$$/p' | \
 sed -e 's/^/$(MKPTR)\//' > $(MK)/ptr.lst
$(AR) r @LIB@ @OBJECTS@ -input $(MK)/ptr.lst
endef

#define DEC6.2_COMBINE_LIB_ARCHIVE
#ls | sed -n -e '/^.*\.o$$/p' > $(MK)/obj.lst
#$(AR) rv @LIB@ -input $(MK)/obj.lst
#endef

define DEC6.2_COMBINE_LIB_ARCHIVE
ls | sed -n -e '/^.*\.o$$/p' > $(MK)/obj.lst
$(AR) r @LIB@ -input $(MK)/obj.lst
endef

define DEC6.2_PIC_CLOSURE
$(RM) $(MK)/__ptdummy0.cc; \
echo "int main() { return 0; }" > $(MK)/__ptdummy0.cc; \
$(C++) $(MK)/__ptdummy0.cc -o $(MK)/__ptdummy0.exe \
 -ptr $(MKPTR).pic @ALLOBJECTS@ @LDFLAGS@ @LDLIBS@; \
$(RM) $(MK)/__ptdummy0.*
endef

define DEC6.2_COMBINE_PIC_CLOSURE
endef

#define DEC6.2_PIC_ARCHIVE
#ls $(MKPTR).pic | sed -n -e '/^.*\.o$$/p' | \
# sed -e 's/^/$(MKPTR).pic\//' > $(MK)/ptr.pic.lst
#$(AR) rv @LIB@ @OBJECTS@ -input $(MK)/ptr.pic.lst
#endef

define DEC6.2_PIC_ARCHIVE
ls $(MKPTR).pic | sed -n -e '/^.*\.o$$/p' | \
 sed -e 's/^/$(MKPTR).pic\//' > $(MK)/ptr.pic.lst
$(AR) r @LIB@ @OBJECTS@ -input $(MK)/ptr.pic.lst
endef

#define DEC6.2_COMBINE_PIC_ARCHIVE
#ls | sed -n -e '/^.*\.o$$/p' > $(MK)/obj.lst
#$(AR) rv @LIB@ -input $(MK)/obj.lst
#endef

define DEC6.2_COMBINE_PIC_ARCHIVE
ls | sed -n -e '/^.*\.o$$/p' > $(MK)/obj.lst
$(AR) r @LIB@ -input $(MK)/obj.lst
endef

#define DEC6.2_PIC_LIBRARY
#ls $(MKPTR).pic | sed -n -e '/^.*\.o$$/p' | \
# sed -e 's/^/$(MKPTR).pic\//' > $(MK)/ptr.pic.lst
#$(LD) -v -shared -soname lib$(LIBRARY)$(SHLEXT) -o @LIB@ \
# $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ -input $(MK)/ptr.pic.lst \
# $(PIC_LDLIBS) @LDLIBS@
#$(RM) so_locations
#endef

define DEC6.2_PIC_LIBRARY
ls $(MKPTR).pic | sed -n -e '/^.*\.o$$/p' | \
 sed -e 's/^/$(MKPTR).pic\//' > $(MK)/ptr.pic.lst
$(LD) -shared -soname lib$(LIBRARY)$(SHLEXT) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ -input $(MK)/ptr.pic.lst \
 $(PIC_LDLIBS) @LDLIBS@
$(RM) so_locations
endef

#define DEC6.2_COMBINE_PIC_LIBRARY
#ls | sed -n -e '/^.*\.o$$/p' > $(MK)/obj.lst
#$(LD) -v -shared -soname lib$(LIBRARY)$(SHLEXT) -o @LIB@ \
# $(PIC_LDFLAGS) @LDFLAGS@ -input $(MK)/obj.lst \
# $(PIC_LDLIBS) @LDLIBS@
#$(RM) so_locations
#endef

define DEC6.2_COMBINE_PIC_LIBRARY
ls | sed -n -e '/^.*\.o$$/p' > $(MK)/obj.lst
$(LD) -shared -soname lib$(LIBRARY)$(SHLEXT) -o @LIB@ \
 $(PIC_LDFLAGS) @LDFLAGS@ -input $(MK)/obj.lst \
 $(PIC_LDLIBS) @LDLIBS@
$(RM) so_locations
endef

endif
