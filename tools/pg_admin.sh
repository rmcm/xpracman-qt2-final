#!/bin/sh
#Dump the database
_WKDAY=`date +%w`
_WKYEAR=`date +%W`
_DUMPDIR=/export/backups/postgres-dumps
mkdir -p ${_DUMPDIR} || {
    echo "Failed to create <${_DUMPDIR}>"
    exit 1
}
_WKDAY="`date +%w`"
echo -n "Dumping .."
pg_dumpall | gzip -c > ${_DUMPDIR}/dumpall.${_WKDAY}.gz
echo " done"
# Vacuum
echo "Vacuuming .."
for db in `psql -t -A -d template1 -c "select datname from pg_database"`; do \
    if test "$db" = "template0" ; then \
      continue; \
    fi; \
    echo -n "  ==> $db .. "; \
    vacuumdb --analyze $db ; \
    pg_dump $db | gzip -c > ${_DUMPDIR}/$db.dump.${_WKDAY}.gz ; \
done;
## Make the weekly copies
WEEKLIST=${WEEKLIST:-"db1 db2"}
for db in ${WEEKLIST} ; do
    if test -f {_DUMPDIR}/$db.dump.${_WKDAY}.gz ; then
	cp -p ${_DUMPDIR}/$db.dump.${_WKDAY}.gz ${_DUMPDIR}/$db.dump.W${_WKYEAR}.gz
    fi
done
echo " done"
date

