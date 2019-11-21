# Makefile - top level

# files to include in the distro but not build
EXTRA_DIST = \
		reconf \
		$(top_srcdir)/template \
		$(top_srcdir)/test \
		$(top_srcdir)/debug \
		$(top_srcdir)/radlib-api-reference.html \
		$(top_srcdir)/MAINTAIN \
		$(top_srcdir)/config-radlib-arm-linux

# this target removes CVS directories
dist-hook:
	rm -rf `find $(distdir) -name CVS`


# add subdirectories to be included in the distribution 
# (thus requiring Makefile.am files)
SUBDIRS =

#define the library to be built
lib_LIBRARIES     = librad.a

# define include directories
INCLUDES = \
		-I$(top_srcdir)/h \
		-I$(prefix)/include \
		-D_GNU_SOURCE

if MYSQL
INCLUDES += \
		-I$(prefix)/include/mysql \
		-I/usr/include/mysql
endif
if PGRESQL
INCLUDES += \
		-I$(prefix)/include \
		-I/usr/include
endif


# define library directories
AM_LDFLAGS =

# define libraries to link against
librad_a_LIBADD =

# define the source files for the library
librad_a_SOURCES  = \
		$(top_srcdir)/src/radbuffers.c \
		$(top_srcdir)/src/radconffile.c \
		$(top_srcdir)/src/raddebug.c \
		$(top_srcdir)/src/radevents.c \
		$(top_srcdir)/src/radlist.c \
		$(top_srcdir)/src/radmsgLog.c \
		$(top_srcdir)/src/radprocess.c \
		$(top_srcdir)/src/radproclist.c \
		$(top_srcdir)/src/radprocutils.c \
		$(top_srcdir)/src/radqueue.c \
		$(top_srcdir)/src/radsemaphores.c \
		$(top_srcdir)/src/radshmem.c \
		$(top_srcdir)/src/radsocket.c \
		$(top_srcdir)/src/radsortlist.c \
		$(top_srcdir)/src/radstack.c \
		$(top_srcdir)/src/radstates.c \
		$(top_srcdir)/src/radsysdefs.c \
		$(top_srcdir)/src/radsystem.c \
		$(top_srcdir)/src/radsysutils.c \
		$(top_srcdir)/src/radtimers.c \
		$(top_srcdir)/src/radtimeUtils.c \
		$(top_srcdir)/src/radUDPsocket.c

if MYSQL
librad_a_SOURCES  += \
		$(top_srcdir)/database/mysql/my_database.c \
		$(top_srcdir)/src/raddatabase.c
endif
if PGRESQL
librad_a_SOURCES  += \
		$(top_srcdir)/database/postgresql/pg_database.c \
		$(top_srcdir)/src/raddatabase.c
endif

# define the header files we install for the library
include_HEADERS   = \
		$(top_srcdir)/h/radbuffers.h \
		$(top_srcdir)/h/radconffile.h \
		$(top_srcdir)/h/raddebug.h \
		$(top_srcdir)/h/radevents.h \
		$(top_srcdir)/h/radlist.h \
		$(top_srcdir)/h/radmsgLog.h \
		$(top_srcdir)/h/radprocess.h \
		$(top_srcdir)/h/radproclist.h \
		$(top_srcdir)/h/radprocutils.h \
		$(top_srcdir)/h/radqueue.h \
		$(top_srcdir)/h/radsemaphores.h \
		$(top_srcdir)/h/radshmem.h \
		$(top_srcdir)/h/radsocket.h \
		$(top_srcdir)/h/radsortlist.h \
		$(top_srcdir)/h/radstack.h \
		$(top_srcdir)/h/radstates.h \
		$(top_srcdir)/h/radsysdefs.h \
		$(top_srcdir)/h/radsystem.h \
		$(top_srcdir)/h/radsysutils.h \
		$(top_srcdir)/h/radtimers.h \
		$(top_srcdir)/h/radtimeUtils.h \
		$(top_srcdir)/h/radUDPsocket.h

if MYSQL
include_HEADERS   += \
		$(top_srcdir)/h/raddatabase.h
endif
if PGRESQL
include_HEADERS   += \
		$(top_srcdir)/h/raddatabase.h \
		$(top_srcdir)/database/postgresql/_pg-types.h
endif

