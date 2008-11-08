#     $Id: Makefile 3223 2006-12-11 05:31:54Z source $
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
PROGRAM 	= qtmarvin
MKDEP		?= ../tools/mkdep

## Variables for major lib and header siting
LOCALBASE 	?= /usr
PGLIB		?= $(LOCALBASE)/lib
PGINC		?= $(LOCALBASE)/include/pgsql
QTDIR		?= $(LOCALBASE)/qt

LIB_STATIC	= lib$(PROGRAM).a

PGMARVINDIR	?= ../pgmarvin$(BUILDVERSION)
QTMARVINDIR	?= ../qtmarvin$(BUILDVERSION)
QTKUGARDIR	?= ../qtkugar$(BUILDVERSION)
PIXMAPDIR	?= ../pixmaps$(BUILDVERSION)

UIC		= $(QTDIR)/bin/uic
MOC		= $(QTDIR)/bin/moc

CC		?= gcc
CXX		?= g++
RANLIB		= ranlib

ifeq ($(PROFILE),1)
	CPROFILE	= -pg
	CFLOW		= -fprofile-arcs -ftest-coverage
endif

ifeq "$(shell uname -s)" "Linux"
DEBUG		+= -Wno-deprecated
endif

CINCLUDES	= -I$(PGMARVINDIR) \
		  -I$(QTMARVINDIR) \
		  -I$(QTKUGARDIR) \
		  -I$(PIXMAPDIR) \
		  -I$(QTDIR)/include \
		  -I$(PGINC)

# hack to stop repeating directory from system path
#  ... this should be done by checking in system path of cc1/cc1plus
ifneq "$(LOCALBASE)" "/usr"
CINCLUDES	+= -I$(LOCALBASE)/include
endif

CXXINCLUDES	= $(CINCLUDES)
IMACROS_DEBUG	= $(if $(DEBUG), -imacros .imacros_debug,)
#DEBUG		?=  -DNO_DEBUG

#CFLAGS		= -g -O2 $(CINCLUDES) $(DEBUG)
CFLAGS		= -g $(IMACROS_DEBUG) $(CPROFILE) $(CFLOW) $(CINCLUDES) $(DEBUG)
CXXFLAGS	= -g $(IMACROS_DEBUG) $(CPROFILE) $(CFLOW) $(CXXINCLUDES) $(DEBUG)

LDFLAGS		= $(CPROFILE) $(CFLOW)  \
		  -L$(PGLIB) -R$(PGLIB) -lpq \
		  -L$(QTMARVINDIR) -lqtmarvin \
		  -L$(QTKUGARDIR) -lqtkugar \
		  -L$(PGMARVINDIR) -lpgmarvin \
		  -L$(QTDIR)/lib -R$(QTDIR)/lib -lqt \
		  -L$(LOCALBASE)/lib -lSM -lICE \
		  -L/usr/X11R6/lib -R/usr/X11R6/lib -lGLU -lGL -lXmu -lpng -lz -ljpeg -lXext -lX11 -lm

CXREFDIR	= cxref
CXREFFLAGS	= -O$(CXREFDIR) -xref -index -html32-src $(CPPFLAGS)

MARVINCLASSES	= qmvdb \
		  qmvclass \
		  qmvtuple \
		  qmvattribute \
		  qmvtupleattribute \
		  qmvsqlquery \
		  qmvquerycondition \
		  qmvqueryconditionwidget \
		  qmvquerywidget \
		  qmvattlist \
		  qmvicons \
		  qmvtable \
		  qmvcombobox \
		  qmvaccess \
		  qmvrelationwidget \
		  qmvexception \
		  qmvhelp \
		  qmvapplication \
		  qmvimport  \
		  qmvexport \
		  qmvform \
		  qmvselect \
		  qmvmultilineedit \
		  qmvimage \
		  qmvcanvasview \
		  qmvcanvasgrid \
		  qmvhttp \
		  qmvurltransfer \
		  qmvtoolwindow \
		  qmvpopup \
		  qmvroweditor \
		  qmvtabbar \
		  qmvtswidget \
		  qmvformtool \
		  qmvbitgraph \
		  qmv_form_api \
		  qmvformeditfields

SRC		= $(MARVINCLASSES:%=%.cpp) \
		  qmcdatenav.cpp \
		  version.cpp

MARVINOBJ	= $(MARVINCLASSES:%=%.o)

HDR		= $(MARVINCLASSES:%=%.h) \
		  qmv.h \
		  qmcdatenav.h
OBJ		= $(MARVINOBJ) $(MARVINOBJ:%=moc_%) \
		  qmcdatenav.o moc_qmcdatenav.o \
		  version.o

LIB		= $(LIB_STATIC) \
		  $(PGMARVINDIR)/libpgmarvin.a

DOCSRC		= doc
DOCROOT		= $(LOCALBASE)/share/doc/$(PROGRAM)

GTAGS		= gtags --file $(GTAGS_LIST) --gtagsconf $(GTAGS_CONFIG)
GTAGS_SRC	= $(SRC) $(HDR)
GTAGS_LIST	= gtags.list
GTAGS_CONFIG	= gtags.config

.PHONY : cxref
.PHONY : depend
.PHONY : doc

qtmarvin:	all

all:		$(LIB_STATIC)

$(LIB_STATIC):	$(OBJ)
		rm -f $@
		$(AR) $(ARFLAGS) $@ $(OBJ)
		$(RANLIB) $@

$(PROGRAM):	$(PROGRAM).o $(OBJ) $(LIB)
		$(CXX) $(PROGRAM).o $(OBJ) $(LDFLAGS) -o $(PROGRAM)

marvin-test:		marvin-test.o $(LIB_STATIC) $(HDR)
			$(CXX) $< $(LDFLAGS) -o $@

$(MARVINOBJ:%=%): %.o:	%.cpp %.h 
		$(CXX) $(CXXFLAGS) -c $<

# moc_worklist.cpp:	worklist.h
# 		$(MOC) -o moc_worklist.cpp worklist.h

# moc_qmcdatenav.cpp:	qmcdatenav.h
# 		$(MOC) -o moc_qmcdatenav.cpp qmcdatenav.h

cxref:
		mkdir -p $(CXREFDIR)
		cxref $(CXREFFLAGS) $(SRC) $(HDR)

lint:
		lint $(CPPFLAGS) $(SRC)

install:
		install -c -m 0775 -o root -g wheel -b -B '.%d' \
			 $(PROGRAM) $(LOCALBASE)/bin/$(PROGRAM)

depend:
		rm -f $(DEPEND_MK)
		$(MKDEP) $(CFLAGS) $(SRC)

clean:
		rm -f *.o *~ *.core *.bak

%-local.h %-global.h:
		./marvin2cdef.sh $*

moc_%.cpp:	%.h
		$(MOC) -o $@ $<
doc:
		@mkdir -p $(DOCSRC)/html $(DOCSRC)/latex $(DOCSRC)/man
		@doxygen
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
		    | xargs tar -cf - | tar -C $(DOCROOT) -xvpf -; \
		)

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
