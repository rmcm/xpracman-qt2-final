#!/bin/sh
#
# Script to build a base database using the "prac" test database
#

## create a new database
DB_TMP=tmp_prac
PSQL="psql -q -d ${DB_TMP}"
dropdb ${DB_TMP}
createdb ${DB_TMP} || {
    echo "Failed to create ${DB_TMP}"
    exit 1
}

## Transfer prac to tmp_prac
pg_dump  prac| tee prac.schema | sed -e 's/ EST//g' | psql -d ${DB_TMP} -q

## Cleanup data

${PSQL} <<EOF
delete from mvac_database where db_name != 'prac';
update mvac_database set db_name = '${DB_TMP}';
delete from invc where invc__sequence != 0;

delete from patn where patn__sequence != 0;
update patn set patn__sequence = 0;

delete from dbtr where dbtr_code != '-';
update dbtr set dbtr__sequence = 0;

delete from paym;
delete from bkdp where bkdp__sequence != 0;

delete from rfdr where rfdr_code != '-';
update rfdr set rfdr__sequence = 0;

delete from empl where empl_code != '-';
update empl set empl__sequence = 0;
delete from evnt;

vacuum analyze;
EOF
