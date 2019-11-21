#///////////////////////////////////////////////////////////////////////////////
#//
#//  MONSOON - Makefile.Linux
#//  Copyright © 2000 AURA
#//
#//  This program is free software; you can redistribute it and/or modify
#//  it under the terms of the GNU General Public License as published by
#//  the Free Software Foundation; either version 2 of the License, or
#//  (at your option) any later version.
#//
#//  This program is distributed in the hope that it will be useful,
#//  but WITHOUT ANY WARRANTY; without even the implied warranty of
#//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#//  GNU General Public License for more details.
#//
#//  You should have received a copy of the GNU General Public License
#//  along with this program; if not, write to the Free Software
#//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#//
#//  Authors:		Philip N Daly
#//  Contact:		pnd@noao.edu
#//  Original date:	Friday, 14 September 2001
#//  Description:	MONSOON Makefile.Linux
#//  Rebuild:           % make everything
#//
#///////////////////////////////////////////////////////////////////////////////

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
MONSOON_TMP  = $(MONSOON_HOME)/tmp

#///////////////////////////////////////////////////////////////////////////////
#// target(s) - in order! The LAST element in $(TARGETS) becomes the default
#///////////////////////////////////////////////////////////////////////////////
TARGETS = \
  $(MONSOON_SRC)/Util/dhsLibraries/_null \
  $(MONSOON_SRC)/Util/dhsLibraries/_fits

#///////////////////////////////////////////////////////////////////////////////
#// default target
#///////////////////////////////////////////////////////////////////////////////
HELP: help
help:
	/usr/bin/clear
	@echo ""
	@echo "MONSOON Makefile.Linux"
	@echo "(c) 2004, AURA. All rights reserved."
	@echo "Released under the GPL."
	@echo ""
	@echo "The following targets are available:"
	@echo ""
	@echo "   all clean dirs distclean everything help init install postmake"
	@echo ""
	@echo "To re-build the code, use: % make everything"
	@echo ""

#///////////////////////////////////////////////////////////////////////////////
#// other make target(s)
#///////////////////////////////////////////////////////////////////////////////
ALL: all
all: 
	/bin/rm -f $(MONSOON_INC)/dhsUtil.h
	/bin/ln -sf ../src/Util/dhsLibraries/dhsUtil.h $(MONSOON_INC)/dhsUtil.h
	-for target in $(TARGETS); do\
         cd $$target; \
	 /usr/bin/make all;\
	done

CLEAN: clean
clean:
	-for target in $(TARGETS); do\
         cd $$target; \
	 /usr/bin/make clean;\
	done

EVERYTHING: everything
everything: 
	/bin/rm -f $(MONSOON_INC)/dhsUtil.h
	/bin/ln -sf ../src/Util/dhsLibraries/dhsUtil.h $(MONSOON_INC)/dhsUtil.h
	-for target in $(TARGETS); do\
         cd $$target; \
	 /usr/bin/make everything;\
	done

INSTALL: install
install: 
	/bin/rm -f $(MONSOON_INC)/dhsUtil.h
	/bin/ln -sf ../src/Util/dhsLibraries/dhsUtil.h $(MONSOON_INC)/dhsUtil.h
	-for target in $(TARGETS); do\
         cd $$target; \
	 /usr/bin/make install;\
	done

