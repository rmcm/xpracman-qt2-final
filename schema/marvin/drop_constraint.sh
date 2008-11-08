#!/bin/sh

## Drop a RI constraint

if [ $# != 2 ] ; then
    echo "usage: `basename $0` database-name constraint-name" >&2
    exit 1
fi

if [ ${DEBUG}X != "X" ] ; then
    set -x
fi

DBNAME=$1
CONSTRAINT=$2
OUTFILTER="psql -d ${DBNAME}"

case "${CONSTRAINT}" in
    "FIX")     SELECT="select drop from bad_triggers;"
               ;;
    "GOOD")    SELECT='select tgconstrname as good_ri_triggers from drop_trigger;'
               OUTFILTER=cat
               ;;
    "BAD")     SELECT='select tgname,tgconstrname as bad_ri_triggers from bad_triggers;'
               OUTFILTER=cat
               ;;
    *)         SELECT="select drop from drop_trigger where tgconstrname ~ '^${CONSTRAINT}';"
               ;;
esac

psql -d ${DBNAME} -A -t -c "${SELECT}" | ${OUTFILTER}





