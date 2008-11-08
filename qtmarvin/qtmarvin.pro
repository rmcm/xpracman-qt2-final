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
HEADERS 	+= qmvdb.h
HEADERS		+= qmvclass.h
HEADERS		+= qmvtuple.h
HEADERS		+= qmvattribute.h
HEADERS		+= qmvtupleattribute.h
HEADERS		+= qmvsqlquery.h
HEADERS		+= qmvquerycondition.h
HEADERS		+= qmvqueryconditionwidget.h
HEADERS		+= qmvquerywidget.h
HEADERS		+= qmvattlist.h
HEADERS		+= qmvicons.h
HEADERS		+= qmvtable.h
HEADERS		+= qmvcombobox.h
HEADERS		+= qmvaccess.h
HEADERS		+= qmvrelationwidget.h
HEADERS		+= qmvexception.h
HEADERS		+= qmvhelp.h
HEADERS		+= qmvapplication.h
HEADERS		+= qmvimport.h
HEADERS		+= qmvexport.h
HEADERS		+= qmvform.h
HEADERS		+= qmvselect.h
HEADERS		+= qmvmultilineedit.h
HEADERS		+= qmvimage.h
HEADERS		+= qmvcanvasview.h
HEADERS		+= qmvcanvasgrid.h
HEADERS		+= qmvhttp.h
HEADERS		+= qmvurltransfer.h
HEADERS		+= qmvtoolwindow.h
HEADERS		+= qmvpopup.h
HEADERS		+= qmvroweditor.h
HEADERS		+= qmvtabbar.h
HEADERS		+= qmvtswidget.h
HEADERS		+= qmvformtool.h
HEADERS		+= qmvbitgraph.h
HEADERS		+= qmv_form_api.h
HEADERS		+= qmvformeditfields.h
HEADERS		+= qmv.h
HEADERS		+= qmcdatenav.h
#
SOURCES 	+= qmvdb.cpp
SOURCES		+= qmvclass.cpp
SOURCES		+= qmvtuple.cpp
SOURCES		+= qmvattribute.cpp
SOURCES		+= qmvtupleattribute.cpp
SOURCES		+= qmvsqlquery.cpp
SOURCES		+= qmvquerycondition.cpp
SOURCES		+= qmvqueryconditionwidget.cpp
SOURCES		+= qmvquerywidget.cpp
SOURCES		+= qmvattlist.cpp
SOURCES		+= qmvicons.cpp
SOURCES		+= qmvtable.cpp
SOURCES		+= qmvcombobox.cpp
SOURCES		+= qmvaccess.cpp
SOURCES		+= qmvrelationwidget.cpp
SOURCES		+= qmvexception.cpp
SOURCES		+= qmvhelp.cpp
SOURCES		+= qmvapplication.cpp
SOURCES		+= qmvimport.cpp
SOURCES		+= qmvexport.cpp
SOURCES		+= qmvform.cpp
SOURCES		+= qmvselect.cpp
SOURCES		+= qmvmultilineedit.cpp
SOURCES		+= qmvimage.cpp
SOURCES		+= qmvcanvasview.cpp
SOURCES		+= qmvcanvasgrid.cpp
SOURCES		+= qmvhttp.cpp
SOURCES		+= qmvurltransfer.cpp
SOURCES		+= qmvtoolwindow.cpp
SOURCES		+= qmvpopup.cpp
SOURCES		+= qmvroweditor.cpp
SOURCES		+= qmvtabbar.cpp
SOURCES		+= qmvtswidget.cpp
SOURCES		+= qmvformtool.cpp
SOURCES		+= qmvbitgraph.cpp
SOURCES		+= qmv_form_api.cpp
SOURCES		+= qmvformeditfields.cpp
SOURCES		+= qmcdatenav.cpp
SOURCES		+= version.cpp
#
INCLUDEPATH	+= ../pgmarvin
INCLUDEPATH	+= ../qtkugar
INCLUDEPATH	+= ../pixmaps
#
LIBS		+= ../pgmarvin/libpgmarvin.a
LIBS		+= ../qtkugar/libqtkugar.a
LIBS		+= /usr/local/lib/libpq.a
#
CONFIG		+= qt
CONFIG		+= debug
CONFIG		+= staticlib
