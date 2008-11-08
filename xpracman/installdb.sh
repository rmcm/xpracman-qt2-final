#!/bin/sh
##
# Create a database
case `psql -d template1 -A -t -c "select count(*) from pg_database where datname = 'prac'"` in
    1)  msg="A database called <prac> already exists, so this version cannot be installed"
        if test -n "$DISPLAY" ; then
            echo $msg
        else
            xmessage $msg
        fi
        exit 1
esac

createdb prac || {
    msg="Failed to create database <prac>"
    if test -n "$DISPLAY" ; then
        echo $msg
    else
        xmessage $msg
    fi
    exit 1
}

psql -d prac -q -f $INSTALL_PATH/prac_init.sql || {
    msg="Failed to install the <prac> database."
    if test -n "$DISPLAY" ; then
        echo $msg
    else
        xmessage $msg
    fi
    exit 1
}

exit 0
