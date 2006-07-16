## Copyright 1992 OTC LIMITED
## Copyright 1995-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

ifeq "$(SRCDIR)" ""
SRCDIR := .
endif

BLDROOT := ../../..
SRCROOT := $(SRCDIR)/$(BLDROOT)

MODULES := c cc check install
MAKEIT_OPTIONS := purify shlib
CHECK_OPTIONS := short_suffixes
SHLIB_OPTIONS := suppress_library_generation

include $(SRCROOT)/config/init.mk

## Local definitions.

PROGRAMS := \
 _list _linklist _stack _queue _hset _hmap _huniqmap _hbag _deque _bitset \
 _ordlist _priqueue _bndstack _bndqueue _range _array _tbag _tmanymap \
 _ilist _single _pair _triple _icursor _visitor _worker _prworker _itmap \
 _prvisitr _mlist _ihmap _hindex _tindex _tuniqmap _tset _tmap _tdualmap

EXCLUDE +=

ifneq "$(filter CYG2.6.95Q3,$(C++COMPILER))" ""
EXCLUDE += _prvisitr.cc _prworker.cc _visitor.cc _worker.cc
endif

NONLIBSRC := $(filter xxx%.cc,$(SRCFILES))

CPPFLAGS += -I../../../include

ifeq "$(_makeit_cxx_supports_cafe_repositories)" "YES"
override MKPTR := ../$(MK).ptr
endif

LDLIBS += $(MK)/xxxrandom$(OBJEXT)

LDLIBS += \
 ../refcnt/$(MK)/lib$(LIBEXT) \
 ../memory/$(MK)/lib$(LIBEXT) \
 ../debug/$(MK)/lib$(LIBEXT) \
 ../misc/$(MK)/lib$(LIBEXT)

ifeq "$(ENV_OODBMS)" "OSTORE"
LDLIBS += ../ostore/$(MK)/lib$(LIBEXT)
endif

ifneq "$(filter -DHAVE_%_THREADS,$($(C++COMPILER)_CPPFLAGS))" ""
LDLIBS += ../thread/$(MK)/lib$(LIBEXT)
endif

LDLIBS += ../../OSE/$(WSPREFIX)$(MKTAG)_opt/lib$(LIBEXT)

LIBVERSION := 1.0

SCHEMAS := os_schema.cc

## Makeit modules.

include $(SRCROOT)/config/modules.mk

## Local rules.

ifeq "$(ENV_OSEINSTALLALL)" "YES"
install.always :: mostlyclean

mostlyclean.always ::
	$(RM) $(MK)/lib.so
endif
