#     $Id: Makefile 3337 2007-01-28 05:21:40Z source $
# 
#   Copyright 2000 X=X Computer Software Trust
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


#**************************************************
#
# 	CONFIGURABLE VARIABLES
#
#**************************************************
# The application name
PROGRAM		?=	xpracman
# Owner and group of installed files
PRAC_OWNER	= 	source
PRAC_GROUP	=	develop

# Locate of system libs and includes (default to SuSE)
LOCALBASE 	?= 	/usr
PGLIB		?= 	/usr/lib
PGINC		?= 	/usr/include/pgsql
# use local qt
QTDIR		=	/usr/local/qt2
#
#**************************************************
#
#  COMMAND VARIABLES
#
#**************************************************

SHELL           =	/bin/sh
MAKE		=	gmake
MAKEFILE	=	Makefile
PSQL		=	psql -q -A -t
PGDUMP		=	pg_dump
C4GL            =       
FORMBUILD	=	
MAKEINFO	=	makeinfo
MAKEHTML	=	texi2html
MAKEINFO	=	makeinfo
TEXI2DVI	=	texi2dvi
INSTALL		=	install
BACKUP		=	echo
PHP		=	php
CJPEG		=	cjpeg
MKDEP		=	$(shell pwd)/tools/mkdep

CC		=	gcc
CFLAGS		=	-g
LDFLAGS		=
CXXFLAGS	=	-O -gstabs
LIBCFLAGS	=	-lprac -lpq -L$(PGLIB) -L../lib
LIBCXXFLAGS	=	$(CXXFLAGS)
CFLAGS_SHLIB	=	-fpic
LDFLAGS_SHLIB	=	-Bshareable -assert pure-text
SUFFIX_SHLIB	=	so
CXXINCLUDES	=	-I. -I$(srcdir)
CINCLUDES	=	-I. -I$(PGINC)
XCFLAGS		=
XCXXFLAGS	=	$(LIBCXXFLAGS)
INSTALL_FLAGS	=	-c -o $(PRAC_OWNER) -g $(PRAC_GROUP)

#
#***********end local variables********************
#

# export variables by default
export

#**************************************************
#
# 	TARGETS
#
#**************************************************
.PHONY : all
.PHONY : install
.PHONY : clean
.PHONY : ChangeLog

## Some specific targets
## _VERSION may be passed in
BUILDVERSION	?= 

MARVINLIBS	?= pgmarvin \
		   qtkugar \
		   qtmarvin \
		   pixmaps

MARVINAPPS	?= xpracman \
		   xmicro \
		   patnotes

MARVINTOOLS	?= tools

DBNAME		?= prac

CVS_ARGS	?=
CVS_TAG		?= $(shell date +MS_%Y%m%d)

help:
		@echo "targets: all install database"

all:		$(PROGRAM)

install:
		(cd $(PROGRAM)$(BUILDVERSION) && $(MAKE) install)

database:
		createdb $(DBNAME)
		psql -d $(DBNAME) -q -f $(PROGRAM)$(BUILDVERSION)/prac_init.sql
		psql -d $(DBNAME) -q -c "update mvac_database set db_name = '$(DBNAME)'"

.PHONY : $(MARVINLIBS) $(MARVINAPPS)

cvsupdate:
		cvs -f update -l -dP $(CVS_ARGS)
		cvs -f update -dP $(CVS_ARGS) $(MARVINLIBS) $(MARVINTOOLS) $(PROGRAM)
		for d in $(MARVINLIBS) $(PROGRAM); do \
		 	(cd $${d}$(BUILDVERSION) && $(MAKE) depend) \
		done

cvstag:
		echo "cvs -f tag -l -F $(CVS_TAG)"
		echo "cvs -f tag -F $(CVS_TAG) $(MARVINLIBS) $(MARVINTOOLS) $(PROGRAM)"

ChangeLog:
		cvs2cl -r -b -t -T

clean:
		for d in $(MARVINLIBS) $(PROGRAM); do \
		 (cd $${d}$(BUILDVERSION) && $(MAKE) clean) \
		done

$(MARVINLIBS):
		$(MAKE) -C $@$(BUILDVERSION) all

$(MARVINAPPS):	$(MARVINLIBS)
		$(MAKE) -C $@$(BUILDVERSION) all


