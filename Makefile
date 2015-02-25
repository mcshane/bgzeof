CC       = gcc
CPPFLAGS = $(DFLAGS) $(INCLUDES)
CFLAGS   = -g -Wall -O2
LDFLAGS  =
LDLIBS   =
DFLAGS=     -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_CURSES_LIB=1
INCLUDES=   -I. -I$(HTSDIR)

PROGRAMS = bgzeof

all: $(PROGRAMS)

# Adjust $(HTSDIR) to point to your top-level htslib directory
HTSDIR = ../htslib
HTSLIB = $(HTSDIR)/libhts.a

PACKAGE_VERSION = 0.1

# If building from a Git repository, replace $(PACKAGE_VERSION) with the Git
# description of the working tree: either a release tag with the same value
# as $(PACKAGE_VERSION) above, or an exact description likely based on a tag.
# $(shell), :=, etc are GNU Make-specific.  If you don't have GNU Make,
# comment out this conditional.
ifneq "$(wildcard .git)" ""
PACKAGE_VERSION := $(shell git describe --always --dirty)

# Force version.h to be remade if $(PACKAGE_VERSION) has changed.
version.h: $(if $(wildcard version.h),$(if $(findstring "$(PACKAGE_VERSION)",$(shell cat version.h)),,force))
endif

# If you don't have GNU Make but are building from a Git repository, you may
# wish to replace this with a rule that always rebuilds version.h:
# version.h: force
#	echo '#define BGZEOF_VERSION "`git describe --always --dirty`"' > $@
version.h:
	echo '#define BGZEOF_VERSION "$(PACKAGE_VERSION)"' > $@

.PHONY:all clean force

force:

bgzeof: bgzeof.c $(HTSLIB) version.h
	$(CC) -pthread $(LDFLAGS) -o bgzeof bgzeof.c $(HTSLIB) $(INCLUDES) -lz

clean:
	rm bgzeof
