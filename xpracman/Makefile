#     $Id: Makefile 3337 2007-01-28 05:21:40Z source $
# 
#   Copyright 2000-1 X=X Computer Software Trust
#    		  Kangaroo Ground Australia 3097
# 
# 
#  This is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License published by
#  the Free Software Foundation; either version 2, or (at your option)
#  any later version.
#  
#  This software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this software; see the file COPYING.  If not, write to
#  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
#  
#  Report problems and direct all questions to:
#  
#      Rex McMaster, rmcm@compsoft.com.au
#  
# 
#  
#

BUILDVERSION	?=

MAKE		= gmake
DEPEND_MK	= .depend
PROGRAM 	= xpracman
MKDEP		?= ../tools/mkdep

GTAGS		= gtags --file $(GTAGS_LIST) --gtagsconf $(GTAGS_CONFIG)
GTAGS_SRC	= $(SRC) $(HDR) \
			$(PGMARVINDIR)/*.c \
			$(PGMARVINDIR)/*.h \
			$(QTMARVINDIR)/*.cpp \
			$(QTMARVINDIR)/*.h \
			$(QTKUGARDIR)/*.cpp \
			$(QTKUGARDIR)/*.h
GTAGS_LIST	= gtags.list
GTAGS_CONFIG	= gtags.config


## Variables for major lib and header siting, and Qt multi-thread library
LOCALBASE 	?= /usr
PGLIB		?= $(LOCALBASE)/lib
PGINC		?= $(LOCALBASE)/include/pgsql
QTDIR		?= $(LOCALBASE)/qt

## Static or Dynamic Linking
ifeq ($(QTSTATIC),1)
QTLINK		?= $(QTDIR)/lib/libqt.a,-lmng
else
ifeq "$(shell uname -s)" "Darwin"
QTLINK		?= -Wl,-L$(QTDIR)/lib,-lqt
else
# opensuse,qt3 -> $QTDIR/lib/libqt-mt
QTLINK		?= -Wl,-L$(QTDIR)/lib,-R$(QTDIR)/lib,-lqt
endif
endif

## Marvin components
PGMARVINDIR	?= ../pgmarvin$(BUILDVERSION)
QTMARVINDIR	?= ../qtmarvin$(BUILDVERSION)
QTKUGARDIR	?= ../qtkugar$(BUILDVERSION)
PIXMAPDIR	?= ../pixmaps$(BUILDVERSION)

UIC		= $(QTDIR)/bin/uic
MOC		= $(QTDIR)/bin/moc

CC		= cc
CXX		?= g++

ifeq ($(PROFILE),1)
	CPROFILE	= -pg
	CFLOW		= -fprofile-arcs -ftest-coverage
endif

## Qt/X11 linking
ifeq "$(shell uname -s)" "Linux"
QTX11LINK	?= -Wl,-L/usr/X11R6/lib,-R/usr/X11R6/lib,-lGL,-lz,-lm
else
ifeq "$(shell uname -s)" "Darwin"
QTX11LINK	?= -Wl,-L/usr/X11R6/lib,-lGL,-lz,-lm
else
QTX11LINK	?= -Wl,-L$(LOCALBASE)/lib,-R$(LOCALBASE)/lib,-lSM,-lICE,-lmng,-llcms \
		   -Wl,-L/usr/X11R6/lib,-R/usr/X11R6/lib,-lGLU,-lGL,-lXmu,-lpng,-lz,-ljpeg,-lXext,-lX11,-lm
endif
endif

## PostgreSQL linking
ifeq "$(shell uname -s)" "Darwin"
PGLINK		?= -Wl,-L$(PGLIB),-lpq
else
PGLINK		?= -Wl,-L$(PGLIB),-R$(PGLIB),-lpq
endif

CINCLUDES	= -I$(PGMARVINDIR) \
		  -I$(QTMARVINDIR) \
		  -I$(QTKUGARDIR) \
		  -I$(PGINC) \
		  -I$(PIXMAPDIR) \
		  -I$(QTDIR)/include

# hack to stop repeating directory from system path
#  ... this should be done by checking in system path of cc1/cc1plus
ifneq "$(LOCALBASE)" "/usr"
CINCLUDES	+= -I$(LOCALBASE)/include
endif


CXXINCLUDES	= $(CINCLUDES)

DEBUG		?=  -DDEBUG -g
#CFLAGS		= -g -O2 $(CINCLUDES) $(DEBUG)
CFLAGS		= -g $(CPROFILE) $(CFLOW) $(CINCLUDES) $(DEBUG)
CXXFLAGS	= -g -Wno-deprecated $(CPROFILE) $(CFLOW) $(CXXINCLUDES) $(DEBUG)
LDFLAGS		= $(CPROFILE) $(CFLOW) \
		 $(PGLINK) \
		 -Wl,-L$(QTMARVINDIR),-lqtmarvin \
		 -Wl,-L$(QTKUGARDIR),-lqtkugar \
		 -Wl,-L$(PGMARVINDIR),-lpgmarvin \
		 $(QTLINK) \
		 $(QTX11LINK)
CXREFDIR	= cxref
CXREFFLAGS	= -O$(CXREFDIR) -xref -index -html32-src $(CINCLUDES) -I/usr/include/g++

XPRACMANCLASSES	= $(PROGRAM) \
		  xpracapptool

SRC		= $(XPRACMANCLASSES:%=%.cpp) \
		  main.cpp

HDR		= $(XPRACMANCLASSES:%=%.h)

XPRACMANOBJ	= $(XPRACMANCLASSES:%=%.o)
OBJ		= $(XPRACMANOBJ) $(XPRACMANOBJ:%=moc_%) \
		  main.o

LIB		= $(QTMARVINDIR)/libqtmarvin.a \
		  $(QTKUGARDIR)/libqtkugar.a \
		  $(PGMARVINDIR)/libpgmarvin.a

## May be locally modified
LOCALSHARED	= Balance.kdelnk \
		  XPracMan.kdelnk \
		  xpracmanrc \
		  userpreferences \
		  pcedit

## Not locally modified
SHARED		= invoice.awk \
		  format_lj.awk \
		  format_plain.awk \
		  print_invoice \
		  form.awk \
		  form_sql.awk \
		  form_label.example \
		  print_form \
		  bank.awk \
		  bank_sql.awk \
		  print_bank

SHAREDIR	?= $(LOCALBASE)/share/$(PROGRAM)/

DOCSRC		= doc
DOCROOT		= $(LOCALBASE)/share/doc/$(PROGRAM)
CXXFLAGS	+= -DDOCROOT='"$(DOCROOT)"'
TMPSRC		?= tmp-src

.PHONY : cxref
.PHONY : depend
.PHONY : doc
.PHONY : version

all:		$(PROGRAM)

$(PROGRAM):	$(OBJ) $(LIB)
		$(CXX) $(OBJ) $(LDFLAGS) -o $(PROGRAM)

version:
		./build_version.sh "$(_BUILD_MESSAGE)" > version.h

cxref:
		mkdir -p $(CXREFDIR)
		cxref $(CXREFFLAGS) $(SRC) $(HDR)

lint:
		lint $(CPPFLAGS) $(SRC)

install:	install-shared install-bin

install-bin:
		sed 's,_@_SHAREDIR_@_,$(SHAREDIR),g' \
			$(PROGRAM).sh > $(PROGRAM).sh.tmp || exit 1
		install -c -m 0775 -o root -g wheel \
			 $(PROGRAM).sh.tmp $(LOCALBASE)/bin/$(PROGRAM) || exit 1
		-mv $(SHAREDIR)/$(PROGRAM) $(SHAREDIR)/$(PROGRAM).bak
		install -c -m 0775 -o root -g wheel \
			 $(PROGRAM) $(SHAREDIR)/$(PROGRAM)

install-shared:
		@echo "Installing in --> $(SHAREDIR)"
		@if test ! -d $(SHAREDIR) ; then \
			echo "Building install directory"; \
			install -d -m 0755 -o root -g wheel $(SHAREDIR); \
		fi;
		@for file in $(LOCALSHARED) ; do \
		  if test ! -f $(SHAREDIR)/$$file ; then \
			echo " --> $$file"; \
			sed 's,_@_SHAREDIR_@_,$(SHAREDIR),g' \
				$$file > $$file.tmp || exit 1; \
			install -c -m 0755 -o root -g wheel \
				$$file.tmp $(SHAREDIR)/$$file ; \
		  fi; \
		done;
		@for file in $(SHARED) ; do \
			echo " --> $$file"; \
			sed 's,_@_SHAREDIR_@_,$(SHAREDIR),g' \
				$$file > $$file.tmp || exit 1; \
			install -c -m 0755 -o root -g wheel \
				$$file.tmp $(SHAREDIR)/$$file ; \
		done;


install-doc:
		mkdir -p $(DOCROOT) || exit 1
		@( cd $(DOCSRC); \
		  echo "Converting images from gif to jpg"; \
		  find . -name "*.gif" \
		     | awk '{ sub(/.gif$$/, ""); printf "convert %s.gif %s.jpg\n", $$1, $$1}' \
		     | /bin/sh; \
		  echo "Converting image references"; \
		  find . -name "*.html" \
		     | awk '{ printf "printf \",s/.gif/.jpg/g\\nw\\nq\\n\" | ed -s %s\n", $$i}' \
		     | /bin/sh; \
		  echo "Installing html, css, jpg and dot files"; \
		  find . -name "*.html" -o  -name "*.css" -o -name "*.jpg" -o -name "*.dot" \
			 -o  -name "*.png" \
		    | xargs tar -cf - | tar -C $(DOCROOT) -xvpf -; \
		)

depend:
		rm -f $(DEPEND_MK)
		$(MKDEP) $(CFLAGS) $(SRC)

clean:
		rm -f *.o *~ *.core *.bak

build:
		rm -rf $(TMPSRC)
		mkdir $(TMPSRC)
		svn -q export file:///home/svnroot/trunk/marvin $(TMPSRC)/xpracman
		svn -q export file:///home/svnroot/trunk/medical/pracman/xpracman $(TMPSRC)/xpracman/xpracman
		tar -C $(TMPSRC) -zcf xpracman-src.tgz xpracman
		(cd $(TMPSRC)/xpracman && make xpracman)
		tar -C $(TMPSRC)/xpracman/xpracman -zcf xpracman-bin.tgz xpracman
		rm -rf $(TMPSRC)

moc_%.cpp:	%.h
		$(MOC) -o $@ $<

doc:		doc-prog doc-user doc-admin install-doc doc-dist

doc-prog:
		@mkdir -p $(DOCSRC)/html $(DOCSRC)/latex $(DOCSRC)/man
		@doxygen

doc-user:
		@doxygen Doxyfile.user

doc-admin:
		@doxygen Doxyfile.admin

doc-dist:
		@echo "creating $(PROGRAM)-doc.tgz"
		@tar -C $(DOCROOT)/.. -zcf $(PROGRAM)-doc.tgz $(PROGRAM)

gtags:		$(GTAGS_LIST) $(GTAGS_CONFIG)
		$(GTAGS)

$(GTAGS_LIST):	$(GTAGS_SRC)
		for f in $(GTAGS_SRC) ; do \
			echo $$f; \
		done > $@

$(GTAGS_CONFIG):
		(printf "default:\n"; \
		 printf "\tlangmaps=:c++:.cpp,.h\n") > $@

-include $(DEPEND_MK)
