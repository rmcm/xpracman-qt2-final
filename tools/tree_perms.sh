#!/bin/sh -x
#
# Set permissions on files and directories

[ $# -lt 1 ] && {
    echo "usage: basename $* paths"
    exit 1
}

_PATHS=$*
_PERMS_FILE=664
_PERMS_DIR=775

for p in ${_PATHS} ; do
    find $p -type f -exec chmod ${_PERMS_FILE} {} \;
    find $p -type d -exec chmod ${_PERMS_DIR} {} \;
done

