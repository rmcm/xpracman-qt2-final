#     $Id: local.mk 1663 2003-04-16 03:45:34Z source $
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

CLASSES		=	mtcl mtat mtfn mtrl form fmdt

## NOTE the order of creation is important for nested views.
VIEWS		=	mtsv mttv

METACLASSES	=	mtcl mtat mtfn mtrl


GLOBALFUNCTIONS	=	create_metadata byteaparam create_links \
			short_date quarter-date conf-functions get_param

# -*- makefile -*-
