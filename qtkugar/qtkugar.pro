# -*-Makefile-*-
#     $Id$
# 
#   Copyright 2006 X=X Computer Software Trust
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

TEMPLATE	=  lib
#
HEADERS		+=kugar.h
HEADERS		+=mcalcobject.h
HEADERS		+=mfieldobject.h
HEADERS		+=mlabelobject.h
HEADERS		+=mlineobject.h
HEADERS		+=mpagecollection.h
HEADERS		+=mpagedisplay.h
HEADERS		+=mreportdetail.h
HEADERS		+=mreportengine.h
HEADERS		+=mreportobject.h
HEADERS		+=mreportsection.h
HEADERS		+=mreportviewer.h
HEADERS		+=mspecialobject.h
HEADERS		+=mutil.h
HEADERS		+=qtkugar.h
#
SOURCES		+= mcalcobject.cpp
SOURCES		+= mfieldobject.cpp
SOURCES		+= mlabelobject.cpp
SOURCES		+= mlineobject.cpp
SOURCES		+= mpagecollection.cpp
SOURCES		+= mpagedisplay.cpp
SOURCES		+= mreportdetail.cpp
SOURCES		+= mreportengine.cpp
SOURCES		+= mreportobject.cpp
SOURCES		+= mreportsection.cpp
SOURCES		+= mreportviewer.cpp
SOURCES		+= mspecialobject.cpp
SOURCES		+= mutil.cpp
SOURCES		+= qtkugar.cpp
SOURCES		+= version.cpp
#
CONFIG		+= qt
CONFIG		+= debug
CONFIG		+= staticlib
