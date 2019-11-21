#///////////////////////////////////////////////////////////////////////////////
#//
#//  MONSOON Makefile.Linux
#//  Copyright © 2002 AURA Inc. All rights reserved.
#//
#//  Author(s):		Philip N Daly
#//  Contact(s):	pnd@noao.edu
#//  Original date:	Monday, 04 November 2002
#//  Description:	Makefile for Linux
#//  Version(s): 	Red Hat 7.3, Linux 2.4.18
#//  Command(s):	% make all clean doc everything help install
#//
#///////////////////////////////////////////////////////////////////////////////

#///////////////////////////////////////////////////////////////////////////////
#// primary target(s)
#///////////////////////////////////////////////////////////////////////////////
NAME = dhsNOAO
TCLP = dhsTcl
UTIL = dhsUtil
TCLP_VERSION := $(shell /bin/grep 'define _$(TCLP)_H_' $(TCLP).h | /bin/awk '{print $$3}')
UTIL_VERSION := $(shell /bin/grep 'define _dhsImpl_H_' dhsImplementationSpecifics.h | /bin/awk '{print $$3}')
HERE := $(shell /bin/pwd)

#///////////////////////////////////////////////////////////////////////////////
#// secondary dependencies
#///////////////////////////////////////////////////////////////////////////////
LIBNAME = lib$(NAME)
LIBTCLP = lib$(TCLP)
LIBUTIL = lib$(UTIL)
SHAREDTCLPLIB = $(LIBNAME)Tcl.so.$(TCLP_VERSION)
SHAREDUTILLIB = $(LIBNAME).so.$(UTIL_VERSION)

#///////////////////////////////////////////////////////////////////////////////
#// dependencies
#///////////////////////////////////////////////////////////////////////////////
MONSOON_HOME  := $(shell /bin/echo $$MONSOON_HOME)
MONSOON_BIN    = $(MONSOON_HOME)/bin
MONSOON_CFG    = $(MONSOON_HOME)/cfg
MONSOON_DATA   = $(MONSOON_HOME)/data
MONSOON_DOC    = $(MONSOON_HOME)/doc
MONSOON_ETC    = $(MONSOON_HOME)/etc
MONSOON_INC    = $(MONSOON_HOME)/inc
MONSOON_LIB    = $(MONSOON_HOME)/lib
MONSOON_SBIN   = $(MONSOON_HOME)/sbin
MONSOON_SRC    = $(MONSOON_HOME)/src
MONSOON_TCL    = $(MONSOON_HOME)/tcl
MONSOON_DHSSRC = $(MONSOON_SRC)/Util/dhsLibraries
MONSOON_DHSLIB = $(MONSOON_LIB)/dhsLibs

#///////////////////////////////////////////////////////////////////////////////
#// stuff that's precious or to keep
#///////////////////////////////////////////////////////////////////////////////
.PRECIOUS:	$(SHAREDTCLPLIB) $(SHAREDUTILLIB)
.KEEP_STATE:

#///////////////////////////////////////////////////////////////////////////////
#// includes, flags and libraries
#///////////////////////////////////////////////////////////////////////////////
QUANTA_INC = /home/chisholm/QUANTA-0.4/include
CPPINCS = -I$(QUANTA_INC)
QUANTA_LIB = /home/chisholm/QUANTA-0.4/src/libquanta_32.la
CINCS  = -I$(HERE) -I$(MONSOON_INC)
CFLAGS = -O2 -Wall # -DDEBUG -g

#///////////////////////////////////////////////////////////////////////////////
#// list of source and include files
#///////////////////////////////////////////////////////////////////////////////
TCLPSRC := $(shell /bin/ls $(UTIL)*.c $(TCLP)*.c)
UTILSRC := $(shell /bin/ls $(UTIL)*.c)
TCLPINC := $(shell /bin/ls $(TCLP)*.h)
UTILINC := $(shell /bin/ls $(UTIL)*.h)

#///////////////////////////////////////////////////////////////////////////////
#// targets
#///////////////////////////////////////////////////////////////////////////////
HELP:
	/usr/bin/clear
	@echo ""
	@echo "$(LIBUTIL) $(UTIL_VERSION) Makefile.Linux"
	@echo "(c) 2002, AURA Inc. All rights reserved."
	@echo ""
	@echo "The following targets are available:"
	@echo ""
	@echo "   all clean doc help install"
	@echo ""
	@echo "To re-build the library, use: % make everything"
	@echo ""

all: SharedTclp SharedUtil $(SHAREDTCLPLIB) $(SHAREDUTILLIB)

clean:
	/bin/rm -rf SharedTclp/*.o SharedUtil/*.o *.o
	/bin/rm -rf $(MONSOON_DHSLIB)/$(LIBTCLP)* $(MONSOON_DHSLIB)/$(LIBUTIL)*
	/bin/rm -rf .make.state .nse_depinfo $(SHAREDTCLPLIB) $(SHAREDUTILLIB)

client: client.cxx
	/usr/bin/libtool --mode=compile g++ -O $(CPPINCS) -c client.cxx
	/usr/bin/libtool --mode=link g++ -g -O2 -o client client.o $(QUANTA_LIB)

doc:
	/bin/rm -rf $(HERE)/$(UTIL).tex
	@for target in $(UTILINC) $(UTILSRC) ; do\
	 docline=`/bin/grep __doc__ $$target | /bin/awk -F__doc__ '{print $$2}'` ;\
	 /bin/echo $$docline >>  $(HERE)/$(UTIL).TEX ;\
	done
	/bin/sed '1,$$s/<<VERSION>>/$(UTIL_VERSION)/g' ./$(UTIL).TEX >> ./$(UTIL).tex
	/bin/rm -rf $(HERE)/$(UTIL).TEX

everything:
	/usr/bin/gmake clean
	/usr/bin/gmake all
	/usr/bin/gmake client
	/usr/bin/gmake doc
	/usr/bin/gmake install

help: HELP

install: all $(MONSOON_INC) $(MONSOON_DHSLIB) $(MONSOON_DOC) 
	/bin/rm -rf $(MONSOON_DOC)/$(UTIL).tex
	/bin/cp -f ./$(UTIL).tex $(MONSOON_DOC)/$(UTIL).tex
	/bin/rm -rf $(MONSOON_INC)/$(TCLP).h $(MONSOON_INC)/$(UTIL).h
	/bin/ln -sf $(HERE)/$(TCLP).h $(MONSOON_INC)/$(TCLP).h
	/bin/ln -sf $(MONSOON_DHSSRC)/$(UTIL).h $(MONSOON_INC)/$(UTIL).h
	/bin/rm -rf $(MONSOON_LIB)/$(LIBTCLP).so* $(MONSOON_LIB)/$(LIBUTIL).so*
	/bin/mv $(SHAREDTCLPLIB) $(SHAREDUTILLIB) $(MONSOON_DHSLIB)/
	/bin/ln -sf ../lib/dhsLibs/$(SHAREDTCLPLIB) $(MONSOON_LIB)/$(LIBTCLP).so
	/bin/ln -sf ../lib/dhsLibs/$(SHAREDUTILLIB) $(MONSOON_LIB)/$(LIBUTIL).so


#///////////////////////////////////////////////////////////////////////////////
#// leave this stuff alone
#///////////////////////////////////////////////////////////////////////////////
$(MONSOON_DHSLIB):
	/bin/mkdir $(MONSOON_DHSLIB)

$(UTIL).h:
	/bin/ln -sf ../$(UTIL).h ./$(UTIL).h

$(SHAREDTCLPLIB): $(TCLPSRC:%.c=SharedTclp/%.o)
	/usr/bin/ld -shared -o $@ $? -lc -ldl
SharedTclp/%.o: %.c $(TCLPINC)
	/usr/bin/gcc $(CINCS) $(CFLAGS) -fpic -shared -c $< -o $@
SharedTclp:
	/bin/mkdir $@
	chmod 777 $@

$(SHAREDUTILLIB): $(UTILSRC:%.c=SharedUtil/%.o)
	/usr/bin/ld -shared -o $@ $? -lc -ldl
SharedUtil/%.o: %.c $(UTILINC)
	/usr/bin/gcc $(CINCS) $(CFLAGS) -fpic -shared -c $< -o $@
SharedUtil:
	/bin/mkdir $@
	chmod 777 $@
