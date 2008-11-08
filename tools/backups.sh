#!/bin/sh
#Dump specified directories
case $# in
  0)  echo "usage: $0 dirs ..."
      exit 1
      ;;
esac
_DIRS=$*
_WKDAY=`date +%w`
_WKYEAR=`date +%W`
_DUMPDIR=/net/fellini/export/backups/`hostname`
mkdir -p ${_DUMPDIR} || {
    echo "Failed to create <${_DUMPDIR}>"
    exit 1
}
for d in ${_DIRS} ; do
  _d="`echo ${d} | sed 's,/,_,g'`"
  _dd=${_DUMPDIR}/${_d}-D${_WKDAY}.zip
  _dw=${_DUMPDIR}/${_d}-W${_WKYEAR}.zip
  zip -qry ${_dd} $d
  ## Make the weekly copies
  cp -p ${_dd} ${_dw}
done
