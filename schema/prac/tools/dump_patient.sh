#!/bin/sh
#    $Id$
#
#  Copyright 2005 X=X Computer Software Trust
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
if [ $# -ne 3 ] ; then
    echo "Usage: $0 DBNAME master-table master__sequence"
    exit 1
fi
_PROG=$0
_ARGS="$*"
_DBNAME=$1
_MASTER=$2
_SEQN=$3
_PSQL="psql -d ${_DBNAME} -tA -q"
_TABLES="`${_PSQL} \
           -c "select mtat_class_name \
               from   mtat \
               where  mtat_name ~ '${_MASTER}__sequence'\
               and mtat_class_name in (select tablename from pg_tables)" ;`"

if [ "xx" = "XX${_TABLES}" ] ; then
    echo "no tables found"
    exit 1
fi
echo "-- $_PROG $_ARGS"
echo "-- `eval echo $_TABLES`"
echo "-- select disable_triggers();"
for t in $_TABLES ; do
   if [ $t = "${_MASTER}" ] ; then
       seq="${_MASTER}__sequence"
   else
       seq="${t}_${_MASTER}__sequence"
   fi
   temp_table="tmp_${t}_copy"
   echo "copy ${t} from stdin;"
   ${_PSQL} \
       -c "select *\
           into temp ${temp_table}
           from   $t\
           where  ${seq} = ${_SEQN};\
           copy ${temp_table} to stdout"
   echo "\\."
done
echo "-- select enable_triggers()"
