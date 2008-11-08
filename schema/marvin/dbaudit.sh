#!/bin/sh
#
#    $Id: dbaudit.sh 1680 2003-04-18 10:34:03Z source $
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
# These dumps are not meant for use in restoration of a database, as the table
# dumps have been dissected into separate files, with no record of the required
# restoration order. These dumps are solely for auditing changes using CVS.
#
# usage example:  env COMMIT=1 DBNAME=prac_init SCHEMA=prac schema/marvin/dbaudit.sh
#
# Requirements:
#
# The CVS sandpit has to be setup first - as this script assumes that it is present.
# ENV Variables:
#
#    DBNAME: the name of the database
#    SCHEMA: the name of the schema
#    AUDITHOME: the parent of the audit directory (dbaudits/${SCHEMA}/${DBNAME}
#    TABLES: list of tables to dump - if not set, all tables are dumped.
#    COMMIT: 1=yes 
#
#  DBNAME, SCHEMA and AUDITHOME must be set.
#

if test -z "$DBNAME"; then
  echo "DBNAME is not set"
  exit 1;
fi

if test -z "$SCHEMA"; then
  echo "SCHEMA is not set"
  exit 1
fi

if test -z "$AUDITHOME"; then
  echo "AUDITHOME is not set"
  exit 1
fi

AUDITDIR=${AUDITHOME}/dbaudits/${SCHEMA}/${DBNAME}
mkdir -p ${AUDITDIR}

if test -z "${TABLES}"; then
TABLES="`
  printf 'select relname from pg_class where relname !~ \047^pg_\047 and relkind ~ \047[rv]\047' |
  psql -q -t -A -d ${DBNAME}`"
fi

for t in $TABLES; do
     ( printf "\055\055 \044Id\044\n\n"
       printf "\055\055   Schema Name: $SCHEMA\n"
       printf "\055\055 Database Name: $DBNAME\n"
       printf "\055\055    Table Name: $t\n\n";
       pg_dump -t $t ${DBNAME} ) |
       sed -e '/--.*TOC Entry/d' \
           -e 's/ EST//g' \
           -e 's/\(Name: "RI_ConstraintTrigger_\)[0-9]*"/\1XXX/' \
       > ${AUDITDIR}/${t}.sql
done

if test "$COMMIT" = 1; then
    (cd ${AUDITDIR} && cvs commit -m 'Commited from $Id: dbaudit.sh 1680 2003-04-18 10:34:03Z source $')
fi
