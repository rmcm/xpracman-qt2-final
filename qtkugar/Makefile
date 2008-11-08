#     $Id: Makefile 3222 2006-12-11 05:23:49Z source $
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
PROGRAM 	= qtkugar
MKDEP		?= ../tools/mkdep

## Variables for major lib and header siting
LOCALBASE 	?= /usr
PGLIB		?= $(LOCALBASE)/lib
PGINC		?= $(LOCALBASE)/include/pgsql
QTDIR		?= $(LOCALBASE)/qt

LIB_STATIC	= lib$(PROGRAM).a

PGMARVINDIR	= ../pgmarvin$(BUILDVERSION)
QTMARVINDIR	= ../qtmarvin$(BUILDVERSION)
PIXMAPDIR	= ../pixmaps$(BUILDVERSION)

UIC		= $(QTDIR)/bin/uic
MOC		= $(QTDIR)/bin/moc

CC		= cc
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
		  -I$(PIXMAPDIR) \
		  -I$(QTDIR)/include \
		  -I$(PGINC)

# hack to stop repeating directory from system path
#  ... this should be done by checking in system path of cc1/cc1plus
ifneq "$(LOCALBASE)" "/usr"
CINCLUDES	+= -I$(LOCALBASE)/include
endif

CXXINCLUDES	= $(CINCLUDES)
DEBUG		?=  -DNO_DEBUG

#CFLAGS		= -g -O2 $(CINCLUDES) $(DEBUG)
CFLAGS		= -g $(CPROFILE) $(CFLOW) $(CINCLUDES) $(DEBUG)
CXXFLAGS	= -g $(CPROFILE) $(CFLOW) $(CXXINCLUDES) $(DEBUG)

LDFLAGS		= $(CPROFILE) $(CFLOW)  \
		  -L$(PGLIB) -R$(PGLIB) -lpq \
		  -L$(PGMARVINDIR) -lpgmarvin \
		  -L$(QTDIR)/lib -R$(QTDIR)/lib -lqt \
		  -L$(LOCALBASE)/lib -lSM -lICE \
		  -L/usr/X11R6/lib -R/usr/X11R6/lib -lGLU -lGL -lXmu -lpng -lz -ljpeg -lXext -lX11 -lm

CXREFDIR	= cxref
CXREFFLAGS	= -O$(CXREFDIR) -xref -index -html32-src $(CPPFLAGS)

KUGARCLASSES	= \
	          mcalcobject \
	          mfieldobject \
	          mlabelobject \
	          mlineobject \
	          mpagecollection \
	          mpagedisplay \
	          mreportdetail \
	          mreportengine \
	          mreportobject \
	          mreportsection \
	          mreportviewer \
	          mspecialobject \
	          mutil \
		  qtkugar


SRC		= $(KUGARCLASSES:%=%.cpp) \
		  version.cpp

KUGAROBJ	= $(KUGARCLASSES:%=%.o)

HDR		= $(KUGARCLASSES:%=%.h)

OBJ		= $(KUGAROBJ) $(KUGAROBJ:%=moc_%) \
		  version.o

LIB		= $(LIB_STATIC)

DOCSRC		= doc
DOCROOT		= $(LOCALBASE)/share/doc/$(PROGRAM)

TAGS		= etags

.PHONY : cxref
.PHONY : depend
.PHONY : doc

all:		$(LIB_STATIC)

$(LIB_STATIC):	$(OBJ)
		rm -f $@
		$(AR) $(ARFLAGS) $@ $(OBJ)
		$(RANLIB) $@

$(KUGAROBJ:%=%): %.o:	%.cpp %.h 
		$(CXX) $(CXXFLAGS) -c $<

$(PROGRAM):	$(OBJ) $(LIB) main.o
		$(CXX) $(OBJ) main.o $(LDFLAGS) -o $(PROGRAM)


cxref:
		mkdir -p $(CXREFDIR)
		cxref $(CXREFFLAGS) $(SRC) $(HDR)

lint:
		lint $(CPPFLAGS) $(SRC)


depend:
		rm -f $(DEPEND_MK)
		$(MKDEP) $(CFLAGS) $(SRC)

clean:
		rm -f *.o *~ *.core *.bak

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

tags:
		$(TAGS) $(SRC) $(HDR) \
			$(PGMARVINDIR)/*.c \
			$(PGMARVINDIR)/*.h

-include $(DEPEND_MK)
