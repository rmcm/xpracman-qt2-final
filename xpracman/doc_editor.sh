#!/bin/sh -x
#    $Id: pcedit,v 1.5 2002/02/05 04:50:42 source Exp $
#
#  Copyright 2005 X=X Computer Software Trust
#                  Kangaroo Ground Australia 3097
#
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
# 
# Report problems and direct all questions to:
# 
#     Rex McMaster, rmcm@compsoft.com.au
# 
# Interface to document editor - pass URL of document to real editor
#


case "$1" in
    "/*")       DOCUMENT=$1
                ;;
    *)          DOCUMENT=${PRAC_DOCROOT}/${DBNAME}/$1
                ;;
esac

oowriter ${DOCUMENT} 
