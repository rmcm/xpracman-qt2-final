#!/bin/bash -x
#    $Id: import_set-docs.sh 2897 2006-06-15 03:47:22Z source $
#
#  Copyright 2006 X=X Computer Software Trust
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

# Import process (qmvrelationwidget)

if test $# != 3 ; then \
    echo "usage: `basename $0 ` database relation key";
    exit 1;
fi

_DBNAME=$1
_DOCS=$2
_KEY=$3
umask 007

_IMPORT_ROOT=${PRAC_DOCROOT}/${_DBNAME}

## The upload directory
_UPLOAD_DIR=${_IMPORT_ROOT}/IMPORTED/UPLOAD
mkdir -p ${_UPLOAD_DIR} || exit 1

## Locate the file to import
_FILE_IMPORT="`kdialog --getopenfilename ${_UPLOAD_DIR}/ '*'`"
if ! test $? = 0 ; then
  exit 1
fi

## Destination path - based on patn__sequence
_SQL="select docs_patn__sequence from docs where docs__sequence = ${_KEY};"
_PATN__SEQUENCE="`echo ${_SQL} | psql -qAt -d ${_DBNAME}`"

_DESTIN_DIR=${_IMPORT_ROOT}/IMPORTED/${_PATN__SEQUENCE}
mkdir -p ${_DESTIN_DIR} || exit 1

_DOCS_URL=IMPORTED/${_PATN__SEQUENCE}/`date +%s`-`basename ${_FILE_IMPORT}`
mv ${_FILE_IMPORT} ${_IMPORT_ROOT}/${_DOCS_URL} || exit 1

## Update the reference in the database
##
_SQL="update docs set docs_url = '${_DOCS_URL}' where docs__sequence = $_KEY;"
echo $_SQL |  psql -qAt -d ${_DBNAME} || exit 1

exit 0

