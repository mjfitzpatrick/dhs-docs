#///////////////////////////////////////////////////////////////////////////////
#//
#//  MONSOON Makefile.Linux
#//  Copyright © 2002 AURA Inc. All rights reserved.
#//
#//  Author(s):		Philip N Daly
#//  Contact(s):	pnd@noao.edu
#//  Original date:	Tue, 04 September 2002
#//  Description:	Makefile for Linux
#//  Version(s): 	Red Hat 7.3, Linux 2.4.18
#//  Command(s):	% make all clean help install
#//
#///////////////////////////////////////////////////////////////////////////////

#///////////////////////////////////////////////////////////////////////////////
#// primary dependencies
#///////////////////////////////////////////////////////////////////////////////
TARGET = semTest
HERE := $(shell /bin/pwd)
SRCS := $(shell /bin/ls *.c)
INCS := $(shell /bin/ls *.h)
OBJS = $(SRCS:.c=.o)

#///////////////////////////////////////////////////////////////////////////////
#// dependencies
#///////////////////////////////////////////////////////////////////////////////
MONSOON_HOME := $(shell /bin/echo $$MONSOON_HOME)
MONSOON_BIN  = $(MONSOON_HOME)/bin
MONSOON_CFG  = $(MONSOON_HOME)/cfg
MONSOON_DATA = $(MONSOON_HOME)/data
MONSOON_DOC  = $(MONSOON_HOME)/doc
MONSOON_ETC  = $(MONSOON_HOME)/etc
MONSOON_INC  = $(MONSOON_HOME)/inc
MONSOON_LIB  = $(MONSOON_HOME)/lib
MONSOON_SBIN = $(MONSOON_HOME)/sbin
MONSOON_SRC  = $(MONSOON_HOME)/src
MONSOON_TCL  = $(MONSOON_HOME)/tcl

#///////////////////////////////////////////////////////////////////////////////
#// includes and flags
#///////////////////////////////////////////////////////////////////////////////
LOCAL_FLAGS = -O2 -Wall -DDEBUG -g -I$(HERE)
MONSOON_FLAGS = -I$(MONSOON_INC)

CFLAGS = $(LOCAL_FLAGS) $(MONSOON_FLAGS)

LOCAL_LIBS = -L/usr/lib -lm
MONSOON_LIBS = -L$(MONSOON_LIB)  -lsemUtil

CLIBS = $(MONSOON_LIBS) $(LOCAL_LIBS)

#///////////////////////////////////////////////////////////////////////////////
#// rules
#///////////////////////////////////////////////////////////////////////////////
help:
	/usr/bin/clear
	@echo ""
	@echo "$(TARGET) Makefile.Linux"
	@echo "(c) 2002, AURA Inc. All rights reserved."
	@echo ""
	@echo "The following targets are available:"
	@echo ""
	@echo "   all clean everything help install"
	@echo ""
	@echo "To re-build the code, use: % make everything"
	@echo ""

all: $(OBJS) $(TARGET)

clean:
	/bin/rm -rf .make.state .nse_depinfo *.o $(TARGET)

everything:
	/usr/bin/gmake clean
	/usr/bin/gmake all
	/usr/bin/gmake install

install:
	@for target in $(INCS); do\
	 /bin/rm -f $(MONSOON_INC)/$$target;\
	 /bin/ln -sf $(HERE)/$$target $(MONSOON_INC)/$$target;\
	done
	@for target in $(TARGET); do\
	 /bin/rm -f $(MONSOON_BIN)/$$target;\
	 /bin/mv -f ./$$target $(MONSOON_BIN)/$$target;\
	done

%.o: %.c
	/usr/bin/gcc $(CFLAGS) -c -o $@ $<

$(TARGET): $(TARGET).o
	/usr/bin/gcc $(CFLAGS) $(OBJS) -o $@ $(CLIBS)
