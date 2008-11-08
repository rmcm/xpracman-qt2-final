#     $Id: Makefile 3221 2006-12-11 05:16:56Z source $
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
# 
#  
#
MAKE		= gmake
DEPEND_MK	= .depend
PROGRAM 	= pgmarvin
MKDEP		?= ../tools/mkdep

## Variables for major lib and header siting
LOCALBASE 	?= /usr
PGLIB		?= $(LOCALBASE)/lib
PGINC		?= $(LOCALBASE)/include/pgsql

LIB_STATIC	= lib$(PROGRAM).a

CC		= cc
RANLIB		= ranlib

CINCLUDES	= -I$(PGINC)
#CFLAGS		= -g -O2 $(CINCLUDES) $(DEBUG)
CFLAGS		= -g $(CINCLUDES) $(DEBUG)
CXXFLAGS	= -g $(CXXINCLUDES) $(DEBUG)
CPPFLAGS	= -I$(PGINC)

# hack to stop repeating directory from system path
#  ... this should be done by checking in system path of cc1/cc1plus
ifneq "$(LOCALBASE)" "/usr"
CINCLUDES	+= -I$(LOCALBASE)/include -I$(LOCALBASE)/X11/include
endif

CXREFDIR	= cxref
CXREFFLAGS	= -O$(CXREFDIR) -xref -index -html32-src $(CPPFLAGS)

BASE		= dbi_pg test_dbi sql misc version
SRC		= $(BASE:%=%.c)
HDR		= dbi_pg.h sql.h misc.h
OBJ		= $(BASE:%=%.o)
CPROTO		= $(BASE:%=%.cproto)
GTAGS		= gtags --file $(GTAGS_LIST) --gtagsconf $(GTAGS_CONFIG)
GTAGS_SRC	= $(SRC) $(HDR)
GTAGS_LIST	= gtags.list
GTAGS_CONFIG	= gtags.config

.PHONY : cxref
.PHONY : depend
.PHONY : $(CPROTO)
.PHONY : $(GTAGS_LIST)
.PHONY : $(GTAGS_CONFIG)

all:		$(LIB_STATIC)

$(LIB_STATIC):	$(OBJ) $(HDR)
		$(AR) $(ARFLAGS) $@ $(OBJ)
		$(RANLIB) $@

cxref:
		mkdir -p $(CXREFDIR)
		cxref $(CXREFFLAGS) $(SRC) $(HDR)

$(CPROTO:%=%): %.cproto: %.c
		cproto -M__P -edvm ${CPPFLAGS} $<

lint:
		lint $(CPPFLAGS) $(SRC)

depend:
		rm -f $(DEPEND_MK)
		$(MKDEP) $(CFLAGS) $(SRC)

clean:
		rm -f $(OBJ) *~

gtags:		$(GTAGS_LIST) $(GTAGS_CONFIG)
		$(GTAGS)

$(GTAGS_LIST):	$(GTAGS_SRC)
		for f in $(GTAGS_SRC) ; do \
			echo $$f; \
		done > $@

$(GTAGS_CONFIG):
		(printf "default:\n"; \
		 printf "\tlangmaps=:c:.c,.h\n") > $@


-include $(DEPEND_MK)
