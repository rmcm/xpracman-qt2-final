#!/bin/sh

export LD_LIBRARY_PATH=$INSTALL_PATH
mkdir -p ${HOME}/.xpracman

PRAC_DIR=${PRAC_DIR:-$INSTALL_PATH} export PRAC_DIR
if test -f ${PRAC_DIR}/xpracmanrc ; then\
  . ${PRAC_DIR}/xpracmanrc 
fi

if test -f ${HOME}/.xpracmanrc ; then\
  . ${HOME}/.xpracmanrc 
fi

DBNAME=${DBNAME:-prac}
PRAC_HELPROOT=${PRAC_HELPROOT:-$INSTALL_PATH/doc} export PRAC_HELPROOT

## make sure you are in the database prac-access table 
## - if you can insert a record, then you should be ok
## Otherwise you are either have no access rights to the database
## at the postgresql level, or there is already a record in your name.

(printf "insert into mvac_user(uid,username,perms)"
printf " select \047${USER}\047,\047${USER}\047,\047\063\047"
printf " except select \047${USER}\047,username,\047\063\047"
printf " from mvac_user where username = \047${USER}\047;\n") | psql -d ${DBNAME}

if test -z "${PRAC_LOG}" ; then \
    PRAC_LOG=${HOME}/.xpracman/${DBNAME}-${DISPLAY}.log export PRAC_LOG
fi

mkdir -p `dirname ${PRAC_LOG}`

if test -f "${PRAC_LOG}" ; then \
    mv ${PRAC_LOG} ${PRAC_LOG}.0
fi

if test $# = 0 ; then
    if test -n "${DBNAME}" ; then
        ARGS="--database ${DBNAME}";
    fi
else
    ARGS=$*;
fi

if test -n "${BACKGROUND}" ; then
    ARGS="${ARGS} -background ${BACKGROUND}"
fi

if test -n "${XPRACMAN_BIN}" ; then
    _PRAC_EXEC="${XPRACMAN_BIN}"
elif test -x "${_PRAC_EXEC}_${DBNAME}" ; then
    _PRAC_EXEC="${_PRAC_EXEC}_${DBNAME}"
else
    _PRAC_EXEC="${PRAC_DIR}/xpracman"
fi

ulimit -c unlimited -m 500000 -v 500000
exec /bin/sh -c "${_PRAC_EXEC} ${ARGS} ${EXTRAARGS}" > ${PRAC_LOG} 2>&1

