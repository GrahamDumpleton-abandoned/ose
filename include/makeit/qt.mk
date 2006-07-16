## Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

vpath %.moc $(SRCDIR) $(SRCDIRS)

ifeq "$(QT_INCDIR)" ""
override QT_INCDIR := $(firstword \
 $(wildcard /usr/include/qt /usr/lib/qt/include))
endif

override CPPFLAGS += -I$(QT_INCDIR)

ifneq "$(QT_LIBDIR)" ""
override LDFLAGS += -L$(QT_LIBDIR)
override LD_RUN_PATH := $LD_RUN_PATH:$(QT_LIBDIR)
endif

ifeq "$(QT_BINDIR)" ""
override QT_BINDIR := /usr/bin
endif

ifeq "$(MOC)" ""
override MOC := $(QT_BINDIR)/moc
endif

override LDLIBS += -lqt

.PHONY : qt-moc
.PHONY : qt-moc.setup
.PHONY : qt-moc.subdirs
.PHONY : qt-moc.target
.PHONY : qt-moc.always

qt-moc : qt-moc.setup
qt-moc : qt-moc.subdirs
qt-moc : qt-moc.target
qt-moc : qt-moc.always

qt-moc.setup :: makeit.setup

all.always :: qt-moc

lib.always :: qt-moc

shlib.always :: qt-moc

programs.always :: qt-moc

depend.always :: qt-moc

_qt_input_1 := $(filter %.h,$(SRCFILES))
_qt_input_2 := $(filter %.hh,$(SRCFILES))
_qt_input_3 := $(filter %.h++,$(SRCFILES))
_qt_input_4 := $(filter %.hxx,$(SRCFILES))
_qt_input_5 := $(filter %.hpp,$(SRCFILES))
_qt_input_6 := $(filter %.H,$(SRCFILES))

ifneq "$(_qt_input_1)" ""
_qt_real_SRC_1 := $(shell grep -l Q_OBJECT $(_qt_input_1))
REQUIRE += cc
endif

ifneq "$(_qt_input_2)" ""
_qt_real_SRC_2 := $(shell grep -l Q_OBJECT $(_qt_input_2))
REQUIRE += cc
endif

ifneq "$(_qt_input_3)" ""
_qt_real_SRC_3 := $(shell grep -l Q_OBJECT $(_qt_input_3))
REQUIRE += cc
endif

ifneq "$(_qt_input_4)" ""
_qt_real_SRC_4 := $(shell grep -l Q_OBJECT $(_qt_input_4))
REQUIRE += cc
endif

ifneq "$(_qt_input_5)" ""
_qt_real_SRC_5 := $(shell grep -l Q_OBJECT $(_qt_input_5))
REQUIRE += cc
endif

ifneq "$(_qt_input_6)" ""
_qt_real_SRC_6 := $(shell grep -l Q_OBJECT $(_qt_input_6))
REQUIRE += cc
endif

_qt_real_SRC_STEMS_1 := $(notdir $(basename $(_qt_real_SRC_1)))
_qt_real_SRC_STEMS_2 := $(notdir $(basename $(_qt_real_SRC_2)))
_qt_real_SRC_STEMS_3 := $(notdir $(basename $(_qt_real_SRC_3)))
_qt_real_SRC_STEMS_4 := $(notdir $(basename $(_qt_real_SRC_4)))
_qt_real_SRC_STEMS_5 := $(notdir $(basename $(_qt_real_SRC_5)))
_qt_real_SRC_STEMS_6 := $(notdir $(basename $(_qt_real_SRC_6)))

_qt_OUTPUT_SRC_1 := $(addsuffix .cc,$(_qt_real_SRC_STEMS_1))
_qt_OUTPUT_SRC_2 := $(addsuffix .cc,$(_qt_real_SRC_STEMS_2))
_qt_OUTPUT_SRC_3 := $(addsuffix .cc,$(_qt_real_SRC_STEMS_3))
_qt_OUTPUT_SRC_4 := $(addsuffix .cc,$(_qt_real_SRC_STEMS_4))
_qt_OUTPUT_SRC_5 := $(addsuffix .cc,$(_qt_real_SRC_STEMS_5))
_qt_OUTPUT_SRC_6 := $(addsuffix .cc,$(_qt_real_SRC_STEMS_6))

_cc_generated_SRC += $(addprefix moc_,$(_qt_OUTPUT_SRC_1))
_cc_generated_SRC += $(addprefix moc_,$(_qt_OUTPUT_SRC_2))
_cc_generated_SRC += $(addprefix moc_,$(_qt_OUTPUT_SRC_3))
_cc_generated_SRC += $(addprefix moc_,$(_qt_OUTPUT_SRC_4))
_cc_generated_SRC += $(addprefix moc_,$(_qt_OUTPUT_SRC_5))
_cc_generated_SRC += $(addprefix moc_,$(_qt_OUTPUT_SRC_6))

qt-moc.always :: $(addprefix $(MK)/moc_,$(_qt_OUTPUT_SRC_1))
qt-moc.always :: $(addprefix $(MK)/moc_,$(_qt_OUTPUT_SRC_2))
qt-moc.always :: $(addprefix $(MK)/moc_,$(_qt_OUTPUT_SRC_3))
qt-moc.always :: $(addprefix $(MK)/moc_,$(_qt_OUTPUT_SRC_4))
qt-moc.always :: $(addprefix $(MK)/moc_,$(_qt_OUTPUT_SRC_5))
qt-moc.always :: $(addprefix $(MK)/moc_,$(_qt_OUTPUT_SRC_6))

ifneq "$(_qt_real_SRC_STEMS_1)" ""
$(patsubst %,$(MK)/moc_%.cc,$(_qt_real_SRC_STEMS_1)) : $(MK)/moc_%.cc : %.h
	$(MOC) $< -o $@
endif

ifneq "$(_qt_real_SRC_STEMS_2)" ""
$(patsubst %,$(MK)/moc_%.cc,$(_qt_real_SRC_STEMS_2)) : $(MK)/moc_%.cc : %.hh
	$(MOC) $< -o $@
endif

ifneq "$(_qt_real_SRC_STEMS_3)" ""
$(patsubst %,$(MK)/moc_%.cc,$(_qt_real_SRC_STEMS_3)) : $(MK)/moc_%.cc : %.h++
	$(MOC) $< -o $@
endif

ifneq "$(_qt_real_SRC_STEMS_4)" ""
$(patsubst %,$(MK)/moc_%.cc,$(_qt_real_SRC_STEMS_4)) : $(MK)/moc_%.cc : %.hxx
	$(MOC) $< -o $@
endif

ifneq "$(_qt_real_SRC_STEMS_5)" ""
$(patsubst %,$(MK)/moc_%.cc,$(_qt_real_SRC_STEMS_5)) : $(MK)/moc_%.cc : %.hpp
	$(MOC) $< -o $@
endif

ifneq "$(_qt_real_SRC_STEMS_6)" ""
$(patsubst %,$(MK)/moc_%.cc,$(_qt_real_SRC_STEMS_6)) : $(MK)/moc_%.cc : %.H
	$(MOC) $< -o $@
endif
