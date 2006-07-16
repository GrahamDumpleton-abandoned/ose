#
# General definitions.
#

# System type.

ENV_SYSTEM = WIN32

# Tool set.
#
# When using Cygnus Win32 Toolkit, have to use workaround to handle fact
# that bash doesn't remove carriage returns from output of DOS program
# when it forms part of a sequence of commands.
#
# Note that in this file the '^M' is a real carriage return character and
# not a caret followed by 'M'. Thus don't do a cut and paste.
#

#ENV_TOOLSET = MKS
ENV_TOOLSET = CYGWIN

# Installation directory.

OSE_ROOT_DIRECTORY = c:/ose
#OSE_RELEASE_DIRECTORY = tst

# Default and desired C++ compilers.

DEFAULTC++COMPILER = PTHREAD-MS7.0
C++COMPILERS = PTHREAD-MS7.0

# Default and desired compilation variants.

#DEFAULTVARIANT = dbg
#VARIANTS = dbg opt std
DEFAULTVARIANT = dbg
#VARIANTS = dbg opt
VARIANTS = dbg

#
# Default library management definitions.
#

define DEFAULT_SYM_CLEANUP
endef

ifeq "$(ENV_TOOLSET)" "CYGWIN"

define DEFAULT_LIB_CLOSURE
endef

define DEFAULT_LIB_ARCHIVE
$(AR) -out:@LIB@ @ALLOBJECTS@
endef

define DEFAULT_LIB_EXTRACT
for obj in `$(AR) @LIB@ /NOLOGO /LIST`; do \
 $(AR) @LIB@ /NOLOGO /EXTRACT:`echo $${obj} | tr '' ' '`; done
endef

#define DEFAULT_LIB_EXTRACT
#for obj in `$(AR) @LIB@ /NOLOGO /LIST | tr '' ' ' | sed -e 's/ //'`; do \
# $(AR) @LIB@ /NOLOGO /EXTRACT:`echo $${obj}`; done
#endef

define DEFAULT_PIC_CLOSURE
endef

define DEFAULT_PIC_ARCHIVE
$(AR) -out:@LIB@ @ALLOBJECTS@
endef

define DEFAULT_PIC_EXTRACT
for obj in `$(AR) @LIB@ /NOLOGO /LIST`; do \
 $(AR) @LIB@ /NOLOGO /EXTRACT:`echo $${obj} | tr '' ' '`; done
endef

#define DEFAULT_PIC_EXTRACT
#for obj in `$(AR) @LIB@ /NOLOGO /LIST | tr '' ' ' | sed -e 's/ //'`; do \
# $(AR) @LIB@ /NOLOGO /EXTRACT:`echo $${obj} | tr '' ' '`; done
#endef

define DEFAULT_PIC_LIBRARY
LIB="@LIBPATH@" lib="@LIBPATH@" \
 $(C++) /LD -o @LIB@ $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ \
 $(PIC_LDLIBS) @LDLIBS@
endef

else

define DEFAULT_LIB_CLOSURE
endef

define DEFAULT_LIB_ARCHIVE
$(AR) -out:@LIB@ @ALLOBJECTS@
endef

define DEFAULT_LIB_EXTRACT
for obj in `$(AR) @LIB@ /NOLOGO /LIST`; do \
 $(AR) @LIB@ /NOLOGO /EXTRACT:$${obj}; done
endef

define DEFAULT_PIC_CLOSURE
endef

define DEFAULT_PIC_ARCHIVE
$(AR) -out:@LIB@ @ALLOBJECTS@
endef

define DEFAULT_PIC_EXTRACT
for obj in `$(AR) @LIB@ /NOLOGO /LIST`; do \
 $(AR) @LIB@ /NOLOGO /EXTRACT:$${obj}; done
endef

define DEFAULT_PIC_LIBRARY
LIB="@LIBPATH@" lib="@LIBPATH@" \
 $(C++) /LD -o @LIB@ $(PIC_LDFLAGS) @LDFLAGS@ @OBJECTS@ \
 $(PIC_LDLIBS) @LDLIBS@
endef

endif

#
# Compiler definitions.
#

# Microsoft C++ 6.0

ifeq "$(C++COMPILERENV)" ""

MS6.0_VARIANTS = dbg opt std
MS6.0_PIC_VARIANTS = dbg opt std

MS6.0_ENV_OODBMS =

MS6.0_OPT_LIBRARIES =

MS6.0_C++ = c:/PROGRA~1/MICROS~1/VC98/BIN/cl
MS6.0_CC = c:/PROGRA~1/MICROS~1/VC98/BIN/cl
MS6.0_LD = c:/PROGRA~1/MICROS~1/VC98/BIN/cl
MS6.0_AR = c:/PROGRA~1/MICROS~1/VC98/BIN/lib

MS6.0_CPPFLAGS = -DHAVE_WINSOCK2
MS6.0_CFLAGS =
MS6.0_C++FLAGS =
MS6.0_LDFLAGS =
MS6.0_LDLIBS =

MS6.0_PIC_CPPFLAGS =
MS6.0_PIC_CFLAGS = -DPIC_WIN32
MS6.0_PIC_C++FLAGS = -DPIC_WIN32
MS6.0_PIC_LDFLAGS =
MS6.0_PIC_LDLIBS =

MS6.0_CPPFLAGS_dbg =
MS6.0_CFLAGS_dbg = -Z7
MS6.0_C++FLAGS_dbg = -Z7
MS6.0_LDFLAGS_dbg =
MS6.0_LDLIBS_dbg =

MS6.0_CPPFLAGS_opt = -DNDEBUG
MS6.0_CFLAGS_opt = -O
MS6.0_C++FLAGS_opt = -O
MS6.0_LDFLAGS_opt =
MS6.0_LDLIBS_opt =

MS6.0_CPPFLAGS_prf =
MS6.0_CFLAGS_prf =
MS6.0_C++FLAGS_prf =
MS6.0_LDFLAGS_prf =
MS6.0_LDLIBS_prf =

MS6.0_CPPFLAGS_std =
MS6.0_CFLAGS_std =
MS6.0_C++FLAGS_std =
MS6.0_LDFLAGS_std =
MS6.0_LDLIBS_std =

endif

ifeq "$(C++COMPILERENV)" "THREAD"

MS6.0_VARIANTS = dbg opt std
MS6.0_PIC_VARIANTS = dbg opt std

MS6.0_ENV_OODBMS =

MS6.0_OPT_LIBRARIES =

MS6.0_C++ = c:/PROGRA~1/MICROS~1/VC98/BIN/cl
MS6.0_CC = c:/PROGRA~1/MICROS~1/VC98/BIN/cl
MS6.0_LD = c:/PROGRA~1/MICROS~1/VC98/BIN/cl
MS6.0_AR = c:/PROGRA~1/MICROS~1/VC98/BIN/lib

MS6.0_CPPFLAGS = -DHAVE_WINSOCK2 -DHAVE_WIN32_THREADS
MS6.0_CFLAGS =
MS6.0_C++FLAGS =
MS6.0_LDFLAGS =
MS6.0_LDLIBS =

MS6.0_PIC_CPPFLAGS =
MS6.0_PIC_CFLAGS = -DPIC_WIN32
MS6.0_PIC_C++FLAGS = -DPIC_WIN32
MS6.0_PIC_LDFLAGS =
MS6.0_PIC_LDLIBS =

MS6.0_CPPFLAGS_dbg =
MS6.0_CFLAGS_dbg = -Z7
MS6.0_C++FLAGS_dbg = -Z7
MS6.0_LDFLAGS_dbg =
MS6.0_LDLIBS_dbg =

MS6.0_CPPFLAGS_opt = -DNDEBUG
MS6.0_CFLAGS_opt = -O
MS6.0_C++FLAGS_opt = -O
MS6.0_LDFLAGS_opt =
MS6.0_LDLIBS_opt =

MS6.0_CPPFLAGS_prf =
MS6.0_CFLAGS_prf =
MS6.0_C++FLAGS_prf =
MS6.0_LDFLAGS_prf =
MS6.0_LDLIBS_prf =

MS6.0_CPPFLAGS_std =
MS6.0_CFLAGS_std =
MS6.0_C++FLAGS_std =
MS6.0_LDFLAGS_std =
MS6.0_LDLIBS_std =

endif

# Microsoft C++ 7.0

ifeq "$(C++COMPILERENV)" ""

MS7.0_VARIANTS = dbg opt std
MS7.0_PIC_VARIANTS = dbg opt std

MS7.0_ENV_OODBMS =

MS7.0_OPT_LIBRARIES =

MS7.0_C++ = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/cl
MS7.0_CC = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/cl
MS7.0_LD = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/cl
MS7.0_AR = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/lib

MS7.0_CPPFLAGS = -DHAVE_WINSOCK2
MS7.0_CFLAGS =
MS7.0_C++FLAGS =
MS7.0_LDFLAGS =
MS7.0_LDLIBS =

MS7.0_PIC_CPPFLAGS =
MS7.0_PIC_CFLAGS = -DPIC_WIN32
MS7.0_PIC_C++FLAGS = -DPIC_WIN32
MS7.0_PIC_LDFLAGS =
MS7.0_PIC_LDLIBS =

MS7.0_CPPFLAGS_dbg =
MS7.0_CFLAGS_dbg = -Z7
MS7.0_C++FLAGS_dbg = -Z7
MS7.0_LDFLAGS_dbg =
MS7.0_LDLIBS_dbg =

MS7.0_CPPFLAGS_opt = -DNDEBUG
MS7.0_CFLAGS_opt = -O
MS7.0_C++FLAGS_opt = -O
MS7.0_LDFLAGS_opt =
MS7.0_LDLIBS_opt =

MS7.0_CPPFLAGS_prf =
MS7.0_CFLAGS_prf =
MS7.0_C++FLAGS_prf =
MS7.0_LDFLAGS_prf =
MS7.0_LDLIBS_prf =

MS7.0_CPPFLAGS_std =
MS7.0_CFLAGS_std =
MS7.0_C++FLAGS_std =
MS7.0_LDFLAGS_std =
MS7.0_LDLIBS_std =

endif

ifeq "$(C++COMPILERENV)" "THREAD"

MS7.0_VARIANTS = dbg opt std
MS7.0_PIC_VARIANTS = dbg opt std

MS7.0_ENV_OODBMS =

MS7.0_OPT_LIBRARIES =

MS7.0_C++ = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/cl
MS7.0_CC = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/cl
MS7.0_LD = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/cl
MS7.0_AR = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/lib

MS7.0_CPPFLAGS = -DHAVE_WINSOCK2 -DHAVE_WIN32_THREADS
MS7.0_CFLAGS =
MS7.0_C++FLAGS =
MS7.0_LDFLAGS =
MS7.0_LDLIBS =

MS7.0_PIC_CPPFLAGS =
MS7.0_PIC_CFLAGS = -DPIC_WIN32
MS7.0_PIC_C++FLAGS = -DPIC_WIN32
MS7.0_PIC_LDFLAGS =
MS7.0_PIC_LDLIBS =

MS7.0_CPPFLAGS_dbg =
MS7.0_CFLAGS_dbg = -Z7
MS7.0_C++FLAGS_dbg = -Z7
MS7.0_LDFLAGS_dbg =
MS7.0_LDLIBS_dbg =

MS7.0_CPPFLAGS_opt = -DNDEBUG
MS7.0_CFLAGS_opt = -O
MS7.0_C++FLAGS_opt = -O
MS7.0_LDFLAGS_opt =
MS7.0_LDLIBS_opt =

MS7.0_CPPFLAGS_prf =
MS7.0_CFLAGS_prf =
MS7.0_C++FLAGS_prf =
MS7.0_LDFLAGS_prf =
MS7.0_LDLIBS_prf =

MS7.0_CPPFLAGS_std =
MS7.0_CFLAGS_std =
MS7.0_C++FLAGS_std =
MS7.0_LDFLAGS_std =
MS7.0_LDLIBS_std =

endif

ifeq "$(C++COMPILERENV)" "PTHREAD"

MS7.0_VARIANTS = dbg opt std
MS7.0_PIC_VARIANTS = dbg opt std

MS7.0_ENV_OODBMS =

MS7.0_OPT_LIBRARIES =

MS7.0_C++ = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/cl
MS7.0_CC = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/cl
MS7.0_LD = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/cl
MS7.0_AR = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/lib

MS7.0_CPPFLAGS = -DHAVE_WINSOCK2 -DHAVE_POSIX_THREADS -Ic:/posix/include
MS7.0_CFLAGS =
MS7.0_C++FLAGS =
MS7.0_LDFLAGS = -Lc:/posix/lib
MS7.0_LDLIBS = -lpthreadVC

MS7.0_PIC_CPPFLAGS =
MS7.0_PIC_CFLAGS = -DPIC_WIN32
MS7.0_PIC_C++FLAGS = -DPIC_WIN32
MS7.0_PIC_LDFLAGS =
MS7.0_PIC_LDLIBS =

MS7.0_CPPFLAGS_dbg =
MS7.0_CFLAGS_dbg = -Z7
MS7.0_C++FLAGS_dbg = -Z7
MS7.0_LDFLAGS_dbg =
MS7.0_LDLIBS_dbg =

MS7.0_CPPFLAGS_opt = -DNDEBUG
MS7.0_CFLAGS_opt = -O
MS7.0_C++FLAGS_opt = -O
MS7.0_LDFLAGS_opt =
MS7.0_LDLIBS_opt =

MS7.0_CPPFLAGS_prf =
MS7.0_CFLAGS_prf =
MS7.0_C++FLAGS_prf =
MS7.0_LDFLAGS_prf =
MS7.0_LDLIBS_prf =

MS7.0_CPPFLAGS_std =
MS7.0_CFLAGS_std =
MS7.0_C++FLAGS_std =
MS7.0_LDFLAGS_std =
MS7.0_LDLIBS_std =

endif
