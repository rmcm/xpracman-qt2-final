#!/bin/sh
#
#     $Id: dbdiff.sh 1656 2003-04-16 02:36:46Z source $
# 
#   Copyright __YEAR__ X=X Computer Software Trust
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
## Compare the schema of two databases
#

case $# in
    2)  DB1=$1
        DB2=$2
        ;;
    *)  echo "usage $0 db1 db2"
        exit 1
        ;;
esac

DB1_OUT=/tmp/${DB1}.out
DB2_OUT=/tmp/${DB2}.out

## First the application schema

psql -q -t -A -d ${DB1}  <<EOF
  select mttv_class_name, mttv_attribute_order,mttv_attribute_name into temp table tmp_mttv from mttv;
  \\copy tmp_mttv to ${DB1_OUT}
EOF

psql -q -t -A -d ${DB2}  <<EOF
  select mttv_class_name, mttv_attribute_order,mttv_attribute_name into temp table tmp_mttv from mttv;
  \\copy tmp_mttv to ${DB2_OUT}
EOF

echo
echo "Comparing application schema ... "
echo "---------------------------------"
echo
diff -cw ${DB1_OUT} ${DB2_OUT}

rm -rf ${DB1_OUT} ${DB2_OUT}

## Reconcile the application schema and the metadata

echo
echo "Tables without metadata - $DB1"
echo "-------------------------------------"
echo
psql -q -t -A -d ${DB1} <<EOF
  select distinct mttv_class_name  from mttv where mttv_class_name not in (select mtcl_name from mtcl) order by mttv_class_name
EOF

echo
echo "Tables without metadata - $DB2"
echo "-------------------------------------"
echo
psql -q -t -A -d ${DB2} <<EOF
  select distinct mttv_class_name  from mttv where mttv_class_name not in (select mtcl_name from mtcl) order by mttv_class_name
EOF

echo
echo "Attributes without metadata - $DB1"
echo "-------------------------------------"
echo
psql -q -t -A -d ${DB1} <<EOF
  select distinct mttv_class_name, mttv_attribute_name  from mttv where mttv_attribute_name not in (select mtat_name from mtat) order by mttv_class_name, mttv_attribute_name
EOF

echo
echo "Attributes without metadata - $DB2"
echo "-------------------------------------"
echo
psql -q -t -A -d ${DB2} <<EOF
  select distinct mttv_class_name, mttv_attribute_name  from mttv where mttv_attribute_name not in (select mtat_name from mtat) order by mttv_class_name, mttv_attribute_name
EOF
