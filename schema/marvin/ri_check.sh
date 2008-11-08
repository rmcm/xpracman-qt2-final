#!/bin/sh

## checks the RI of constrained tables.

if [ $# -lt 1 ] ; then
    echo "usage: `basename $0` database-name [constraint-name]" >&2
    exit 1
fi

if [ ${DEBUG}X != "X" ] ; then
    set -x
fi

DBNAME=$1
CONSTRAINT=$2
OUTFILTER="psql -d ${DBNAME}"

case "${CONSTRAINT}" in
    "")        SELECT="select ri_sql from ri_check;"
               ;;
    *)         SELECT="select ri_sql from ri_check where ri_name ~ '^${CONSTRAINT}';"
               ;;
esac

psql -d ${DBNAME} -A -t -c "${SELECT}" | ${OUTFILTER}
