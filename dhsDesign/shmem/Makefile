#///////////////////////////////////////////////////////////////////////////////
#//
#//
#///////////////////////////////////////////////////////////////////////////////

# primary dependencies

NAME =  smCache
VERSION = 1.0
HERE := $(shell /bin/pwd)


# secondary dependencies

LIBBASE = lib$(NAME)
STATICLIB = $(HERE)/$(LIBBASE).a
SHAREDLIB = $(HERE)/$(LIBBASE).so.$(VERSION)


# stuff that's precious or to keep

.PRECIOUS:	$(STATICLIB) $(SHAREDLIB)
.KEEP_STATE:


# includes, flags and libraries

CINCS  = -I$(HERE)
# CFLAGS = -O2 -Wall
CFLAGS = -O2 -DSMC_DEBUG -g


# list of source and include files

SRCS = smCache.c shmUtil.c
INCS = smCache.h


# targets

all: Shared $(SHAREDLIB) Static $(STATICLIB)

clean:
	/bin/rm -rf Shared Static *.o
	/bin/rm -rf .make.state .nse_depinfo

realclean:
	/bin/rm -rf Shared Static *.o
	/bin/rm -rf .archive/* *~
	/bin/rm -rf .make.state .nse_depinfo  $(SHAREDLIB) $(STATICLIB)

everything:
	make clean
	make all
	make install

help: HELP

install: all 


# leave this stuff alone

$(STATICLIB): $(SRCS:%.c=Static/%.o)
	/usr/bin/ar rv $@ $?
Static/%.o: %.c $(INCS)
	/usr/bin/gcc $(CINCS) $(CFLAGS) -c $< -o $@
Static:
	/bin/mkdir $@
	chmod 777 $@

$(SHAREDLIB): $(SRCS:%.c=Shared/%.o)
	/usr/bin/ld -shared -o $@ $? -lc -ldl
Shared/%.o: %.c $(INCS)
	/usr/bin/gcc $(CINCS) $(CFLAGS) -fpic -shared -c $< -o $@
Shared:
	/bin/mkdir $@
	chmod 777 $@
