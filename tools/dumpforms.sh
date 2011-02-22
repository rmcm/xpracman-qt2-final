#!/bin/bash

#    $Id: dumpforms.sh 2912 2006-07-05 04:56:18Z source $
#
#  Copyright __YEAR__ X=X Computer Software Trust
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
#

_PROG=`basename $0`
_DBHOST=${PGHOST:-}
_DBNAME="template1"
_FORMS=

function usage() {
     printf "usage: %s args FORMNAME ... args FORMNAME\n"  ${_PROG}
      printf "   args:\n"
      printf "          --database DATABASE_NAME\n"
      printf "          --hostname HOSTNAME\n"
      exit 1
}

## --------------------------------------------------
function exerr() {
## --------------------------------------------------
    echo "Exerr: " $*
    exit 1
}

## --------------------------------------------------
## Parse Arguments
## --------------------------------------------------
while :
 do
  if test $# = 0 ; then
    break
  fi
  case $1 in
    "--help")
            usage
            ;;
    "--database")
      [ $# -lt 2 ] && {
            exerr "--database requires an argument"
      }
      shift 1
      _DBNAME=$1
      ;;
    "--hostname")
      [ $# -lt 2 ] && {
            exerr "--hostname requires an argumant"
      }
      shift 1
      _DBHOST=$1
      ;;
    *)
      _FORMS=$*
  esac
  shift 1
done
export PGHOST=${_DBHOST}

## --------------------------------------------------
function sqlexec() {
## --------------------------------------------------
    psql -At -d "${_DBNAME}" -c "$*"
    [ $? = 0 ] || exerr "database error($?)"
    exit 1
}

## --------------------------------------------------
## Create SQL statements
## --------------------------------------------------

_FORM_ATTS=`sqlexec "select mtat_name from mtat where mtat_class_name = 'form' and mtat_name !~* '^form__'"\
        | awk 'BEGIN{sep=""} {printf("%s\"%s\"",sep,$1);sep=","}'`

_FMDT_ATTS=`sqlexec "select mtat_name from mtat where mtat_class_name = 'fmdt' and mtat_name !~* '^fmdt__'"\
        | awk 'BEGIN{sep=""} {printf("%s\"%s\"",sep,$1);sep=","}'`

if test -z "${_FORMS}" ; then
    _FORMS=`sqlexec "select form_code from form order by form_code"`
fi

for f in ${_FORMS} ; do 
    printf "delete from form where form_code = '$f';\n"
    sqlexec "select ${_FORM_ATTS} from form where form_code = '$f'"\
             | awk -v form_atts="${_FORM_ATTS}" \
                    '{gsub(/\|/,"\047,\047");printf("insert into \"form\" (%s) values(\047%s\047);\n", form_atts, $0)}'
    sqlexec "select ${_FMDT_ATTS} from fmdt where fmdt_form_code = '$f'"\
             | awk -v form_atts="${_FMDT_ATTS}" \
                    '{gsub(/\|/,"\047,\047");printf("insert into \"fmdt\" (%s) values(\047%s\047);\n", form_atts, $0)}'

done

