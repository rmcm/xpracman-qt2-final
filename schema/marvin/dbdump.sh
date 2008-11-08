#!/bin/sh
#
#    $Id: dbdump.sh 1678 2003-04-18 06:38:18Z source $
#
#  Copyright 2003 X=X Computer Software Trust
#   		  Kangaroo Ground Australia 3097
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
#
# Dump the database in a format suitable for using in CVS for auditing changes.
# This is a single db dump, suitable for restoring, or establishing a database, and
# has had certain comments stripped, in order to allow for version control.
#
# usage example:  env DBNAME=prac_init SCHEMA=prac schema/marvin/dbdump.sh
#
# ENV Variables:
#
#    DBNAME: the name of the database
#    SCHEMA: the name of the schema
#  WITHDATA: 1=yes, include data
#  DBNAME and SCHEMA must be set.
#

if test -z "$DBNAME"; then
  echo "DBNAME is not set"
  exit 1;
fi

if test -z "$SCHEMA"; then
  echo "SCHEMA is not set"
  exit 1
fi

if test "${WITHDATA}" = 1 ; then
   SCHEMA_ONLY=
else
   SCHEMA_ONLY="-s"
fi

( printf "\055\055 \044Id\044\n\n"
  printf "\055\055   Schema Name: $SCHEMA\n"
  printf "\055\055 Database Name: $DBNAME\n\n"
  pg_dump ${SCHEMA_ONLY} ${DBNAME} ) |
  sed -e '/--.*TOC Entry/d' \
      -e 's/ EST//g' \
      -e 's/\(Name: "RI_ConstraintTrigger_\)[0-9]*"/\1XXX/'
