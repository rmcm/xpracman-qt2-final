#     $Id$
# 
#   Copyright 2001 X=X Computer Software Trust
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
#**************************************************
#
# 	LOCAL VARIABLES
#
#**************************************************
#

CLASSES		=	accl bank bkdp cash clsp clst cnrt cnty conf \
                        cred dbtr diag docs eftr empl epsd evnt feeb feet hlfd \
                        icd9 inst invc labl lthd mdaf mdbt mdcr \
                        note patf patn paym prnt prov pyas rfdr serv \
                        stts surg svpf tdtp \
			apst aptd aptp locn

## NOTE the order of creation is important for nested views.
VIEWS		=	agdi agdd agdp agdt \
			pmsc pmsp \
			ssms ssmp ssmc \
			crev fept crep \
			dbst dbag \
			cnrv \
			svpv \
			patv \
			eftv evnv \
			gstv \
			surv

FUNCTIONS	=	bkdp conf crep crev dbtr invc rfdr

TRIGGERS	=	bkdp cred feet hlfd invc patn paym \
			serv svpf

METACLASSES	=	mtcl mtat mtfn mtrl

## This can be used to set RELATIONS to existing meta - eg in remote metadumps.
## RELATIONS	= $(shell  $(PSQL) -d $(DBNAME) -q -t -A -c "select mtcl_name from mtcl")

## This can be used to set FORMS to existing FORMS - eg in remote formdumps.
## FORMS	= $(shell  $(PSQL) -d $(DBNAME) -q -t -A -c "select form_code from form")

FORMS		=	list-patn dbstatement dbaged lhinvc \
			invoice-svpv invoice-svpv_base \
			invoice-svpv_1 invoice-svpv_5 invoice-svpv_6 \
			invoice-svpv_7 invoice-svpv_11 invoice-svpv_12 \
			form-envelope \
			creditcard \
			gst-payments gst-receipts \
			directed_report

# -*- makefile -*-
