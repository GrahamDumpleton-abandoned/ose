## Copyright 1997-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED 

vpath %.java $(SRCDIR) $(SRCDIRS)

## Standard targets.

.PHONY : java-classes
.PHONY : java-classes.setup
.PHONY : java-classes.subdirs
.PHONY : java-classes.target
.PHONY : java-classes.always

java-classes : java-classes.setup
java-classes : java-classes.subdirs
java-classes : java-classes.target
java-classes : java-classes.always

java-classes.setup :: makeit.setup

all.always :: java-classes

lib.always :: java-classes

shlib.always :: java-classes

programs.always :: java-classes

## Java home directory.

ifeq "$(OSE_TARGET)" "MIPS_IRIX"
ifeq "$(JAVA_HOME)" ""
override JAVA_HOME = /usr/java
endif
endif

ifeq "$(OSE_TARGET)" "POWER_MACOSX"
ifeq "$(JAVA_HOME)" ""
override JAVA_HOME = /usr
endif
endif

ifeq "$(JAVA_HOME)" ""
override JAVA_HOME = /usr/local/java
endif

## Stop redundant checks on Java files.

%.java :
%.class :

## Default Java compiler location, options and directories.

ifeq "$(ZIP)" ""
override ZIP := zip
endif

ifeq "$(JAR)" ""
ifneq "$(JAVA_HOME)" ""
override JAR := $(JAVA_HOME)/bin/jar
else
override JAR := jar
endif
endif

ifeq "$(JAVAC)" ""
ifneq "$(JAVA_HOME)" ""
override JAVAC := $(JAVA_HOME)/bin/javac
else
override JAVAC := javac
endif
endif

ifeq "$(JAVAH)" ""
ifneq "$(JAVA_HOME)" ""
override JAVAH := $(JAVA_HOME)/bin/javah
else
override JAVAH := javah
endif
endif

ifeq "$(JAVADOC)" ""
ifneq "$(JAVA_HOME)" ""
override JAVADOC := $(JAVA_HOME)/bin/javadoc
else
override JAVADOC := javadoc
endif
endif

ifeq "$(origin JAVA_VARIANT)" "undefined"
override JAVA_VARIANT := $(VARIANT)
endif

JAVACFLAGS_dbg += -g
JAVACFLAGS_opt += -O
JAVACFLAGS_std +=
JAVACFLAGS_prf +=

ifeq "$(MKJAVA)" ""
override MKJAVA := CLASSES_$(JAVA_VARIANT)
endif

ifeq "$(MKJAVADOC)" ""
override MKJAVADOC := $(MKJAVA)
endif

ifneq "$(filter %WIN32,$(ENV_SYSTEM))" ""
override CLASSPATH := $(SRCDIR);$(MKJAVA);$(CLASSPATH)
else
override CLASSPATH := $(SRCDIR):$(MKJAVA):$(CLASSPATH)
endif

export CLASSPATH

## Extend user supply Java options.

override JAVACFLAGS += $(JAVACFLAGS_$(VARIANT))

## Calculate language src.

#_java_real_SRC := $(patsubst ./%,%, \
# $(filter-out $(EXCLUDE),$(shell find $(SRCDIR) -name "*.java" -print)))
_java_real_SRC := $(filter-out $(EXCLUDE),$(patsubst $(SRCDIR)/%,%, \
 $(shell find $(SRCDIR) -name "*.java" -print)))

## Generate stems for src.

_java_real_SRC_STEMS := $(basename $(_java_real_SRC))

## Rules for creating Java output directory.

$(MKJAVA) :
	@mkdir $@

makeit.setup :: $(MKJAVA)

## Rules for compiling Java files.

ifneq "$(_java_real_SRC_STEMS)" ""

java-classes.always :: $(patsubst %,$(MKJAVA)/%.class,$(_java_real_SRC_STEMS))

define RUN-JAVAC
$(JAVAC) $(JAVACFLAGS) -d $(MKJAVA)
endef

ifneq "$(filter jni,$(JAVA_OPTIONS))" ""

override CPPFLAGS += -I$(MK) -I$(JAVA_HOME)/include

ifeq "$(OSE_TARGET)" "SPARC_SOL2"
override CPPFLAGS += -I$(JAVA_HOME)/include/solaris
else
ifeq "$(OSE_TARGET)" "X86_WIN32"
override CPPFLAGS += -I$(JAVA_HOME)/include/win32
else
ifeq "$(OSE_TARGET)" "X86_LINUX"
override CPPFLAGS += -I$(JAVA_HOME)/include/genunix
else
ifeq "$(OSE_TARGET)" "MIPS_IRIX"
override CPPFLAGS += -I$(JAVA_HOME)/include/irix
else
ifeq "$(OSE_TARGET)" "HPPA_HPUX"
override CPPFLAGS += -I$(JAVA_HOME)/include/hp-ux
else
ifeq "$(OSE_TARGET)" "POWER_MACOSX"
override CPPFLAGS += -I/System/Library/Frameworks/JavaVM.framework/Headers
endif
endif
endif
endif
endif
endif

_java_native = '(public|protected|private|final|transient|static|\
synchronised|threadsafe)[\t ][\t ]*native'

define RUN-JAVAH
$(JAVAH) $(JAVAHFLAGS) -d $(MKJAVA)
endef

#$(patsubst %,$(MKJAVA)/%.class,$(_java_real_SRC_STEMS)) \
# : $(MKJAVA)/%.class : java-classes.setup %.java
$(patsubst %,$(MKJAVA)/%.class,$(_java_real_SRC_STEMS)) \
 : $(MKJAVA)/%.class : %.java
	$(RUN-JAVAC) $(SRCDIR)/$(patsubst $(MKJAVA)/%.class,%.java,$@)
	@egrep $(_java_native) \
	 $(SRCDIR)/$(patsubst $(MKJAVA)/%.class,%.java,$@); \
	if test "$$?" = '0'; \
	then \
	  echo "$(RUN-JAVAH) $(subst /,.,$(patsubst $(MKJAVA)/%.class,%,$@))"; \
	  $(RUN-JAVAH) $(subst /,.,$(patsubst $(MKJAVA)/%.class,%,$@)); \
	  cp $(MKJAVA)/$(patsubst %.class,%.h, \
	   $(subst /,_,$(patsubst $(MKJAVA)/%,%,$@))) \
	   $(MK)/$(patsubst %.class,%.h, \
	   $(subst /,_,$(patsubst $(MKJAVA)/%,%,$@))); \
	fi

$(MK)/%.h : $(MKJAVA)/%.h
	cp $< $@

depend.setup :: $(patsubst %,$(MKJAVA)/%.class,$(_java_real_SRC_STEMS))

else

#$(patsubst %,$(MKJAVA)/%.class,$(_java_real_SRC_STEMS)) \
# : $(MKJAVA)/%.class : java-classes.setup %.java
$(patsubst %,$(MKJAVA)/%.class,$(_java_real_SRC_STEMS)) \
 : $(MKJAVA)/%.class : %.java
	$(RUN-JAVAC) $(SRCDIR)/$(patsubst $(MKJAVA)/%.class,%.java,$@)

endif

$(patsubst %,%.class,$(_java_real_SRC_STEMS)) \
 : %.class : java-classes.setup $(MKJAVA)/%.class

endif

## Rules for creating zip file.

.PHONY : java-classes.zip

java-classes.zip ::

ifneq "$(_java_real_SRC_STEMS)" ""

java-classes.zip :: $(MKJAVA)/classes.zip

$(MKJAVA)/classes.zip : \
 $(patsubst %,$(MKJAVA)/%.class,$(_java_real_SRC_STEMS))
	(cd $(dir $@); find . -name "*.class" -print | \
	 $(ZIP) $(notdir $@) -@)
	
endif

ifneq "$(filter zip,$(JAVA_OPTIONS))" ""

java-classes.always :: java-classes.zip

endif

## Rules for creating jar file.

# This isn't ideal as it only includes Java class files.

.PHONY : java-classes.jar

java-classes.jar ::

ifneq "$(_java_real_SRC_STEMS)" ""

java-classes.jar :: $(MKJAVA)/classes.jar

$(MKJAVA)/classes.jar : \
 $(patsubst %,$(MKJAVA)/%.class,$(_java_real_SRC_STEMS))
	(cd $(dir $@); $(JAR) cf $(notdir $@) \
	 `find . -name "*.class" -print`)
ifneq "$(MANIFEST)" ""
	$(JAR) umf $(MANIFEST) $@ 
endif

endif

ifneq "$(filter jar,$(JAVA_OPTIONS))" ""

java-classes.always :: java-classes.jar

endif

## Rules for creating HTML documentation.

ifneq "$(filter javadoc,$(JAVA_OPTIONS))" ""
ifneq "$(_java_real_SRC)" ""

all.always :: $(MKJAVA)/packages.html

$(MKJAVA)/packages.html : $(_java_real_SRC)
	$(JAVADOC) $(JAVADOCFLAGS) -d $(MKJAVADOC) $^
	
endif

endif

## Rules for cleaning up.

distclean.always ::
	rm -r $(MKJAVA)

## Dependency generation.

# This is a really stupid way of generating dependencies for Java.
# What is needed is a proper dependency generator for Java classes
# but no one has yet offered one up for use.

ifneq "" ""

ifneq "$(_java_real_SRC)" ""

_java_real_D := $(patsubst %.java,$(MK)/%.d,$(subst /,.,$(_java_real_SRC)))

# .PHONY : $(_java_real_D)

$(_java_real_D) : % :
	@$(RM) $@
	@file=`echo $(@F) | sed -e 's%\\.d$$%%' -e 's%\\.%/%'`; \
	echo makeit: generating dependencies for $$file.java; \
	echo "$$(MKJAVA)/$$file.class : $$file.java" >> $@; \
	imports=`grep '^[\t ]*import[\t ][\t ]*.*;' $(SRCDIR)/$$file.java | \
	 sed -e 's%^[\t ]*import[\t ][\t ]*%%' -e 's%[\t ]*;.*$$%.java%'`; \
	echo "$@ : $$file.java" >> $@; \
	if test -n "$$imports"; \
	then \
	  for i in $$imports; \
	  do \
	    files=`ls $(SRCDIR)/$$i 2>$(MK)/tmp.err.out | \
	     sed -e "s%^$(SRCDIR)/%%"`; \
	    echo files=$$files; \
	    if test -n "$$files"; \
	    then \
	      for j in $$files; \
	      do \
		echo $$j | sed -e "s%^%$@ : %" >> $@; \
		echo $$j | sed -e "s%^%$$(MKJAVA)/%" \
		 -e "s%\\.java$$%.class%" \
		 -e "s%^%$$(MKJAVA)/$$file.class : %" >> $@; \
	      done; \
	    fi \
	  done; \
	fi; \
	$(RM) $(MK)/tmp.err.out

_depend_SRC += $(_java_real_D)

endif

endif
