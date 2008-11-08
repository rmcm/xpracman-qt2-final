/* $Id: sql.c 917 2001-10-14 21:59:23Z source $
 * 
 *   Copyright 2000 X=X Computer Software Trust
 *    		  Kangaroo Ground Australia 3097
 * 
 * 
 *  This is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *  
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this software; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  
 *  Report problems and direct all questions to:
 *  
 *      Rex McMaster, rmcm@compsoft.com.au
 *  
 * 
 *  
 *

 */

/*
 * ----------------------------------------------------------------------
 * DESCRIPTION: SQL Interface
 *
 * ----------------------------------------------------------------------
 */



/*
 * ----------------------------------------------------------------------
 * INCLUDES:
 * ----------------------------------------------------------------------
 */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "sql.h"
#include "misc.h"
#include "dbi_pg.h"

/*
 * ----------------------------------------------------------------------
 * DEFINES:
 * ----------------------------------------------------------------------
 */

#define MAX_SQL_CONN    8
#define MAX_DBI_RESULTS 8

/*
 * ----------------------------------------------------------------------
 * LOCAL VARIABLES:
 * ----------------------------------------------------------------------
 */

static char sql_version[] = "$Id: sql.c 917 2001-10-14 21:59:23Z source $";
static DBI_conn *sql_conn[MAX_SQL_CONN];
enum pgargtype
{
    PG_HOST = 0,
    PG_PORT,
    PG_OPTIONS,
    PG_TTY,
    PG_DATABASE,
    PG_LOGIN,
    PG_PWD,
    PG_ARGS_COUNT
};

static struct dbiconn {
    DBI_conn * sql_conn;                /* Connection object */
    int status;                         /* asynch query pending 0=no  1=yes */
    char * pgargs[PG_ARGS_COUNT];       /* Connection parameters */
} dbiconn[MAX_SQL_CONN];

static int def_conn_index = -1;
static DBI_result *sql_res[MAX_DBI_RESULTS];
static int def_result_index = -1;


/*
 * ----------------------------------------------------------------------
 * LOCAL FUNCTION PROTOTYPES:
 * ----------------------------------------------------------------------
 */

/*
 * ----------------------------------------------------------------------
 * LOCAL FUNCTIONS:
 * ----------------------------------------------------------------------
 */

/*
 * ----------------------------------------------------------------------
 * GLOBAL FUNCTIONS:
 * ----------------------------------------------------------------------
 */

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    r_sql_dbconnect
 * DESCRIPTION: Establish database connection options
 *               - initialy set to NULL, then ENV, then changed by
 *                 non-NULL args.
 * ARGS:        pghost, pgport, pgoptions, pgtty, pgdatabase
 * RETURNS:     >= 0 == index into connection index, else
 *              -1 = No free connection slots
 *              -2 = connection error
 * ----------------------------------------------------------------------
 */

/*+ Set database connection options +*/
#define FUNCTION_NAME "r_sql_dbconnect"
int
r_sql_dbconnect(pghost, pgport, pgoptions, pgtty, pgdatabase, pglogin, pgpwd)
    const char *pghost;
    const char *pgport;
    const char *pgoptions;
    const char *pgtty;
    const char *pgdatabase;
    const char *pglogin;
    const char *pgpwd;
    
{
    static int init = 0;
    const char *pg_host = NULL;
    const char *pg_port = NULL;
    const char *pg_options = NULL;
    const char *pg_tty = NULL;
    const char *pg_database = NULL;
    const char *pg_login = NULL;
    const char *pg_pwd = NULL;
    DBI_conn * r_sql_conn;
    int conn_index = -1;
    int r_index;
    int i;
    char * ptr;

#ifdef DEBUG
    misc_warnx(FUNCTION_NAME ": %s/%s/%s/%s/%s/%s/%s",
               (pghost == NULL) ? "" : pghost,
               (pgport == NULL) ? "" : pgport,
               (pgoptions == NULL) ? "" : pgoptions,
               (pgtty == NULL) ? "" : pgtty,
               (pgdatabase == NULL) ? "" : pgdatabase,
               (pglogin == NULL) ? "" : pglogin,
               (pgpwd == NULL) ? "" : pgpwd);
#endif
    
    if (pghost != NULL && pghost[0] != '\0')
        pg_host = pghost;
    if (pgport != NULL && pgport[0] != '\0')
        pg_port = pgport;
    if (pgoptions != NULL && pgoptions[0] != '\0')
        pg_options = pgoptions;
    if (pgtty != NULL && pgtty[0] != '\0')
        pg_tty = pgtty;
    if (pgdatabase != NULL && pgdatabase[0] != '\0')
        pg_database = pgdatabase;
    if (pglogin != NULL && pglogin[0] != '\0')
        pg_login = pglogin;
    if (pgpwd != NULL && pgpwd[0] != '\0')
        pg_pwd = pgpwd;

    if (pg_database == NULL)
        pg_database = "template1";

        /* Look for the next free connection index */
        
    for ( r_index = 0; r_index < MAX_SQL_CONN; r_index++)
    {
        if (dbiconn[r_index].sql_conn == (DBI_conn *) NULL)
            break;
    }
    if (r_index < MAX_SQL_CONN)
        conn_index = r_index;
    else
    {
        misc_warnx(FUNCTION_NAME ": No free connection slots (%s)", pgdatabase);
        return(-1);
    }

    r_sql_conn = dbi_connect(pg_host, pg_port, pg_options, pg_tty, pg_database, pg_login, pg_pwd);

        /* check to see that the backend connection was successfully made */
    if (dbi_status(r_sql_conn) == CONNECTION_BAD) {
        misc_warnx(FUNCTION_NAME ": Connection to database '%s' failed.", pg_database);
        return(-2);
    }
        /* set the dbiconn structure */
    dbiconn[conn_index].sql_conn = r_sql_conn;
    for ( i = 0; i < PG_ARGS_COUNT; i++ )
        dbiconn[conn_index].pgargs[i] = NULL;
    
    if ( pg_host != NULL && (dbiconn[conn_index].pgargs[PG_HOST] = strdup(pg_host)) == NULL )
    {
        misc_warnx(FUNCTION_NAME ": strdup '%s' failed.", pg_host);
        return(-2);
    }
    if ( pg_port != NULL && (dbiconn[conn_index].pgargs[PG_PORT] = strdup(pg_port)) == NULL )
    {
        misc_warnx(FUNCTION_NAME ": strdup '%s' failed.", pg_port);
        return(-2);
    }
    if ( pg_options != NULL && (dbiconn[conn_index].pgargs[PG_OPTIONS] = strdup(pg_options)) == NULL )
    {
        misc_warnx(FUNCTION_NAME ": strdup '%s' failed.", pg_options);
        return(-2);
    }
    if ( pg_tty != NULL && (dbiconn[conn_index].pgargs[PG_TTY] = strdup(pg_tty)) == NULL )
    {
        misc_warnx(FUNCTION_NAME ": strdup '%s' failed.", pg_tty);
        return(-2);
    }
    if ( pg_database != NULL && (dbiconn[conn_index].pgargs[PG_DATABASE] = strdup(pg_database)) == NULL )
    {
        misc_warnx(FUNCTION_NAME ": strdup '%s' failed.", pg_database);
        return(-2);
    }
    if ( pg_login != NULL && (dbiconn[conn_index].pgargs[PG_LOGIN] = strdup(pg_login)) == NULL )
    {
        misc_warnx(FUNCTION_NAME ": strdup '%s' failed.", pg_login);
        return(-2);
    }
    if ( pg_pwd != NULL && (dbiconn[conn_index].pgargs[PG_PWD] = strdup(pg_pwd)) == NULL )
    {
        misc_warnx(FUNCTION_NAME ": strdup '%s' failed.", pg_pwd);
        return(-2);
    }
    dbiconn[conn_index].status = 0;
    
#ifdef DEBUG
    misc_warnx(FUNCTION_NAME ": connection index = %d", conn_index);
#endif

    return(conn_index);
    
} /* r_sql_dbconnect */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    r_sql_dbclose
 * DESCRIPTION: Close the database vonnection
 * ARGS:        connection index
 * RETURNS:     
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "r_sql_dbclose"
void *
r_sql_dbclose(conn_index)
  int conn_index;
{
    int i;
    
#ifdef DEBUG
    misc_warnx(FUNCTION_NAME ": connection index %d", conn_index);
#endif
    if (conn_index >= 0 && conn_index < MAX_SQL_CONN
        && dbiconn[conn_index].sql_conn != (DBI_conn *) NULL)
    {
        dbi_close(dbiconn[conn_index].sql_conn);
        dbiconn[conn_index].sql_conn = NULL;
        for ( i = 0; i < PG_ARGS_COUNT; i++ )
            if (dbiconn[conn_index].pgargs[i] != NULL)
                free(dbiconn[conn_index].pgargs[i]);
    }
    

} /* r_sql_dbclose */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    r_sql_init
 * DESCRIPTION: Initialise strucures for SQL query
 * ARGS:        connection index
 * RETURNS:     Result index ( 0 - MAX_DBI_RESULTS-1 )
 *              -1 == Asynchronous query in progress on this connection
 *              -2 == No free result slots
 * ----------------------------------------------------------------------
 */

/*+ Initialise strucures for SQL query +*/
#define FUNCTION_NAME "r_sql_init"
int
r_sql_init(conn_index)
  int conn_index;
{
    static result_init = 0;
    int i_index;

        /* make a connection to the database */
    if ( conn_index < 0 || conn_index > MAX_SQL_CONN
         || dbiconn[conn_index].sql_conn == NULL)
        return -1; /* initialise options */
    
        /* Initialize the results array */
    if ( result_init == 0 )
    {
        for (i_index = 0; i_index < MAX_DBI_RESULTS; i_index++)
            sql_res[i_index] = (DBI_result *) NULL;
        result_init = 1;
    
    }

        /* Look for the next free result index */
        
    for ( i_index = 0; i_index < MAX_DBI_RESULTS; i_index++)
    {
        if (sql_res[i_index] == (DBI_result *) NULL)
            break;
    }
    if (i_index >= MAX_DBI_RESULTS)
        return -2;

    return i_index;
    
} /* r_sql_init */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    r_sql_exec
 * DESCRIPTION: Execute an SQL query
 * ARGS:        connection index, result_index, query
 * RETURNS:     Depending of type of query
 *               - number of rows in set
 *               - number of rows effected
 *               - -1 == SQL ERROR (inc NON_FATAL)
 *               - -2 == No free result slots
 * ----------------------------------------------------------------------
 */

/*+ Execute an SQL query +*/
#define FUNCTION_NAME "r_sql_exec"
int
r_sql_exec(conn_index, result_index, query)
  int conn_index;
  int *result_index;
  const char * query;
{
    int i_index = r_sql_init(conn_index);
    
    if (i_index >= 0)
        *result_index = i_index;
    else
    {
        misc_warnx(FUNCTION_NAME ": No free result slots (%s)",query);
        return(i_index);
    }
         
        /* Execute the query string - */
#ifdef DEBUG
    misc_warnx(FUNCTION_NAME ": %d,%d,%s",conn_index, *result_index, query);
#endif
    sql_res[*result_index] = dbi_exec(dbiconn[conn_index].sql_conn, query);
    switch ((int) dbi_result_status(sql_res[*result_index]))
    {
        case DBI_RES_COMMAND_OK:
            return(dbi_cmd_tuples(sql_res[*result_index]));
            break;
        case PGRES_TUPLES_OK:
            return(dbi_numrows(sql_res[*result_index]));
            break;
        case PGRES_COPY_OUT:
        case PGRES_COPY_IN:
        case PGRES_NONFATAL_ERROR:
            misc_warnx(FUNCTION_NAME ": query failed - %s [%s]",
                       dbi_errormessage(dbiconn[conn_index].sql_conn), query);
            return(-2);
            break;
        case PGRES_EMPTY_QUERY:
        case PGRES_BAD_RESPONSE:
        case PGRES_FATAL_ERROR:
        default:
            misc_warnx(FUNCTION_NAME ": query failed - %s [%s]",
                       dbi_errormessage(dbiconn[conn_index].sql_conn), query);
            dbi_freeresult(sql_res[*result_index]);
            sql_res[*result_index] = NULL;
            return(-1);
    }
        /* Should not get to here */
    return(-1);
    
} /* r_sql_exec */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    r_sql_send
 * DESCRIPTION: Send an asynchronous SQL query
 * ARGS:        connection index, query
 * RETURNS:     Depending of type of query
 *               - 0  = suceessfully sent
 *               - -1 = illegal connection index.
 *               - -2 = failed to set non-blocking
 *               - -3 = connection busy.
 *               - -4 = failed send
 * ----------------------------------------------------------------------
 */

/*+ Send an asynchronous SQL query +*/
#define FUNCTION_NAME "r_sql_send"
int
r_sql_send(conn_index, query)
  int conn_index;
  const char * query;
{
    
        /* make a connection to the database */
    if ( conn_index < 0 || conn_index > MAX_SQL_CONN
         || dbiconn[conn_index].sql_conn == NULL)
        return -1;

    /* Is a request in progress */
    if ( dbiconn[conn_index].status )
    {
#       define TRIES_MAX   10
#       define RESULTS_MAX 10
        DBI_result * discard;
        int tries = 0;
        int results = 0;
        /* Cancel any existing queries */
        while ( tries < TRIES_MAX && results < RESULTS_MAX )
        {
            if ( r_sql_isbusy(conn_index) )
            {
                r_sql_cancel(conn_index);
                tries++;
                continue;
            } else
                discard = dbi_getresult(dbiconn[conn_index].sql_conn);
            if ( discard == NULL )
                break;
            results++;
        }
        /* Timeouts? */
        if ( tries == TRIES_MAX || results == RESULTS_MAX )
        {
            misc_warnx(FUNCTION_NAME ": A query is already in progress and could not be cancelled"
                       " - tries = %d/%d, results = %d/%d", tries, TRIES_MAX, results, RESULTS_MAX);
            return(-2);
        }
        dbiconn[conn_index].status = 0;
    }

    /* Set nonblocking mode on connection */
    if (dbi_setnonblocking(dbiconn[conn_index].sql_conn, TRUE)) 
    {
        misc_warnx(FUNCTION_NAME ": Failed to set non-blocking mode(%s)",query);
        return(-3);
    }
         
        /* Send the query string - */
#ifdef DEBUG
    misc_warnx(FUNCTION_NAME ": %d,%s",conn_index, query);
#endif
    if ( !dbi_sendquery(dbiconn[conn_index].sql_conn, query) )
    {
        misc_warnx(FUNCTION_NAME ": Failed to send query(%s)",query);
        return(-4);
    }
    
    /* Success */
    dbiconn[conn_index].status = 1;
    return(0);
    
} /* r_sql_send */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    r_sql_cancel
 * DESCRIPTION: Cancel the current query
 * ARGS:        connection index
 * RETURNS:     TRUE/FALSE
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "r_sql_cancel"
int
r_sql_cancel(conn_index)
  int conn_index;
{
    if ( conn_index >= 0 && conn_index < MAX_SQL_CONN
         && dbiconn[conn_index].sql_conn != (DBI_conn *) NULL )
        return(dbi_requestcancel(dbiconn[conn_index].sql_conn));
    else
        return(FALSE);
    
} /* r_sql_cancel */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    r_sql_isbusy
 * DESCRIPTION: Check the execution stations of this connection
 * ARGS:        connection index
 *              TRUE                    - is busy
 *              FALSE                   - not busy
 *              SQL_ISBUSY_ILLEGALCONN  - illegal connection index.
 *              SQL_ISBUSY_NOQUERY      - connection does not have a pending query.
 *              SQL_ISBUSY_BADSTATUS    - failed to check status
 * RETURNS:     TRUE/FALSE
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "r_sql_isbusy"
int
r_sql_isbusy(conn_index)
  int conn_index;
{
    /* Check connection index */
    if ( conn_index < 0 || conn_index > MAX_SQL_CONN
         || dbiconn[conn_index].sql_conn == NULL)
        return(SQL_ISBUSY_ILLEGALCONN);

    /* Check connection status */
    if ( !dbiconn[conn_index].status )
    {
        misc_warnx(FUNCTION_NAME ": No asynchronous query is in progress");
        return(SQL_ISBUSY_NOQUERY);
    }

        /* Set nonblocking mode on connection */
    if (dbi_consumeinput(dbiconn[conn_index].sql_conn) == 0) 
    {
        misc_warnx(FUNCTION_NAME ": Failed to consume input prior to checking query status");
        return(SQL_ISBUSY_BADSTATUS);
    }
         
        /* Check status of query - */
#ifdef DEBUG
    misc_warnx(FUNCTION_NAME ": BUSYCHECK: %d",conn_index);
#endif
    if ( dbi_isbusy(dbiconn[conn_index].sql_conn) )
        return(TRUE);
    else
        return(FALSE);
    
} /* r_sql_isbusy */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    r_sql_getresult
 * DESCRIPTION: Get a result from an asynchronous SQL query
 * ARGS:        connection indexresult_index
 * RETURNS:     Depending of type of query
 *               - number of rows in set (0-N)
 *               - number of rows effected (0-N)
 *               SQL_GETRESULT_ILLEGALCONN      - illegal connection index.
 *               SQL_GETRESULT_NOQUERY          - connection does not have a pending query.
 *               SQL_GETRESULT_BADSTATUS        - failed to check status
 *               SQL_GETRESULT_BUSY             - query is busy
 *               SQL_GETRESULT_NONFATAL         - non_fatal query error
 *               SQL_GETRESULT_FATAL            - fatal query error
 *               SQL_GETRESULT_END              - no further query results
 *               SQL_GETRESULT_UNKNOWN          - unknown error
 * ----------------------------------------------------------------------
 */

/*+ Send an asynchronous SQL query +*/
#define FUNCTION_NAME "r_sql_getresult"
int
r_sql_getresult(conn_index, result_index)
  int conn_index;
  int *result_index;
{
    int return_status = SQL_GETRESULT_UNKNOWN;
    int i_index;
    
    /* check query execution status */
    if ( r_sql_isbusy(conn_index) == TRUE)
        return(SQL_GETRESULT_BUSY);
    
        /* Get a result index */
    i_index = r_sql_init(conn_index);
    if (i_index >= 0)
        *result_index = i_index;
    else
    {
        misc_warnx(FUNCTION_NAME ": No free result slots");
        return(i_index);
    }

#ifdef DEBUG
    misc_warnx(FUNCTION_NAME ": GETRESULT: %d,%d",conn_index, *result_index);
#endif

    sql_res[*result_index] = dbi_getresult(dbiconn[conn_index].sql_conn);
    if ( sql_res[*result_index] == NULL )
    {
        return_status = 0;
        dbiconn[conn_index].status = 0;
    } else
        switch ((int) dbi_result_status(sql_res[*result_index]))
        {
            case DBI_RES_COMMAND_OK:
                return_status = dbi_cmd_tuples(sql_res[*result_index]);
                break;
            case PGRES_TUPLES_OK:
                return_status = dbi_numrows(sql_res[*result_index]);
                break;
            case PGRES_COPY_OUT:
            case PGRES_COPY_IN:
            case PGRES_NONFATAL_ERROR:
                misc_warnx(FUNCTION_NAME ": query failed - %s",
                           dbi_errormessage(dbiconn[conn_index].sql_conn));
                return_status = SQL_GETRESULT_NONFATAL;
                break;
            case PGRES_EMPTY_QUERY:
            case PGRES_BAD_RESPONSE:
            case PGRES_FATAL_ERROR:
            default:
                misc_warnx(FUNCTION_NAME ": query failed - %s",
                           dbi_errormessage(dbiconn[conn_index].sql_conn));
                dbi_freeresult(sql_res[*result_index]);
                sql_res[*result_index] = NULL;
                return_status = SQL_GETRESULT_FATAL;
        }

    return(return_status);
    
} /* r_sql_getresult */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    r_sql_value
 * DESCRIPTION: Locates field value of data set created by sql_exec()
 * ARGS:        result_index,row, fieldname
 * RETURNS:     field value (trailing whitespace trimmed)
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "r_sql_value"
char *
r_sql_value(result_index, row, fieldname)
  int *result_index;
  const int row;
  const char *fieldname;
{
    char *ptr;
    int i;
    
#ifdef DEBUG2
    misc_warnx(FUNCTION_NAME ": result_index = %d, row=%d, field=%s",
               *result_index, row, fieldname);
#endif

    ptr = dbi_result(sql_res[*result_index], row, fieldname);
    if (ptr == NULL)
        return(NULL);
    
    i = strlen(ptr) - 1;
    while (i >= 0 && ptr[i] == ' ')
        ptr[i--] = '\0';
    return(ptr);

} /* r_sql_value */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    r_sql_error
 * DESCRIPTION: Last DB error message
 * ARGS:        
 * RETURNS:     Last error message
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "r_sql_error"
char *
r_sql_error(conn_index)
  int conn_index;
{
    if ( conn_index >= 0 && conn_index < MAX_SQL_CONN
         && dbiconn[conn_index].sql_conn != (DBI_conn *) NULL )
        return(dbi_errormessage(dbiconn[conn_index].sql_conn));
    else
        return(NULL);
    
} /* r_sql_error */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    r_sql_getlastoid
 * DESCRIPTION: Return the oid of last insert, if last exec was an INSERT.
 *              Othersie return a NULL string.
 * ARGS:        
 * RETURNS:     OID of last insert
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "r_sql_getlastoid"
char *
r_sql_getlastoid(result_index)
  int *result_index;
{
    char *ptr;
    int i;

    ptr = dbi_getlastoid(sql_res[*result_index]);
    if (ptr == NULL)
        return(NULL);
    
    i = strlen(ptr) - 1;
    while (i >= 0 && ptr[i] == ' ')
        ptr[i--] = '\0';
    return(ptr);

} /* r_sql_getlastoid */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    r_sql_freeresult
 * DESCRIPTION: Free the result object
 * ARGS:        result_index
 * RETURNS:     
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "r_sql_freeresult"
void *
r_sql_freeresult(result_index)
  int *result_index;
{
#ifdef DEBUG
    misc_warnx(FUNCTION_NAME ": result_index = %d", *result_index);
#endif
    if (*result_index >= 0 && *result_index < MAX_DBI_RESULTS
        && sql_res[*result_index] != (DBI_result *) NULL)
    {
        dbi_freeresult(sql_res[*result_index]);
        sql_res[*result_index] = (DBI_result *) NULL;
    }
    

} /* r_sql_freeresult */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    sql_dbconnect
 * DESCRIPTION: Establish database connection options
 *               - initialy set to NULL, then ENV, then changed by
 *                 non-NULL args.
 * ARGS:        pghost, pgport, pgoptions, pgtty, pgdatabase
 * RETURNS:     void
 * ----------------------------------------------------------------------
 */

/*+ Set database connection options +*/
#define FUNCTION_NAME "sql_dbconnect"
int
sql_dbconnect(pghost, pgport, pgoptions, pgtty, pgdatabase, pglogin, pgpwd)
    const char *pghost;
    const char *pgport;
    const char *pgoptions;
    const char *pgtty;
    const char *pgdatabase;
    const char *pglogin;
    const char *pgpwd;
{
    int r_index;
    int conn_index = -1;
    int i;
    const char * local_args[PG_ARGS_COUNT];
    int diff;

    local_args[PG_HOST] = pghost;
    local_args[PG_PORT] = pgport;
    local_args[PG_OPTIONS] = pgoptions;
    local_args[PG_TTY] = pgtty;
    local_args[PG_DATABASE] = pgdatabase;
    local_args[PG_LOGIN] = pglogin;
    local_args[PG_PWD] = pgpwd;

        /* Look for an similar existing connection index */
        
    for ( r_index = 0; r_index < MAX_SQL_CONN; r_index++)
    {
        if (dbiconn[r_index].sql_conn == (DBI_conn *) NULL)
            continue;
        diff = 0;
        for ( i = 0; i < PG_ARGS_COUNT; i++ )
        {
            /* Both NULL, or both pointing to same var??? */
            if ( dbiconn[r_index].pgargs[i] == local_args[i] )
                continue;
            /* Identical strings */
            if ( dbiconn[r_index].pgargs[i] != NULL && local_args[i] != NULL
                 && !strcmp(dbiconn[r_index].pgargs[i], local_args[i]) )
                continue;
            /* Must be a difference */
            diff = 1;
            break;
        }
        
        if (diff)
            continue;
        conn_index = r_index;
        break;
    }
        /* If no connect yet, make one */
    if (conn_index == -1)
        conn_index = r_sql_dbconnect(pghost, pgport, pgoptions,
                                     pgtty, pgdatabase, pglogin, pgpwd);

    if ( def_conn_index == -1 )
        def_conn_index = conn_index;
    
    return(conn_index);
    
} /* sql_dbconnect */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    sql_dbclose
 * DESCRIPTION: Close the database vonnection
 * ARGS:        connection index
 * RETURNS:     
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "sql_dbclose"
void *
sql_dbclose()
{
    r_sql_dbclose(def_conn_index);
    def_conn_index = -1;
    
} /* sql_dbclose */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    sql_exec
 * DESCRIPTION: Execute an SQL query
 * ARGS:        query
 * RETURNS:     Depending of type of query
 *               - number of rows in set
 *               - number of rows effected
 *               - 0 (non-fatal)
 *               - -1 == ERROR
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "sql_exec"
int
sql_exec(query)
    const char * query;
{
    r_sql_freeresult(&def_result_index);
    def_result_index = -1;
    return(r_sql_exec(def_conn_index, &def_result_index, query));
} /* r_sql_exec */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    sql_value
 * DESCRIPTION: Locates field value of data set created by sql_exec()
 * ARGS:        row, fieldname
 * RETURNS:     field value (trailing whitespace trimmed)
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "sql_value"
char *
sql_value(row, fieldname)
    const int row;
    const char *fieldname;
{
    return(r_sql_value(&def_result_index, row, fieldname));
    
} /* sql_value */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    sql_error
 * DESCRIPTION: Last DB error message
 * ARGS:        
 * RETURNS:     Last error message
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "sql_error"
char *
sql_error()
{
    return(r_sql_error( (int) def_conn_index));
} /* sql_error */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    sql_lastoid
 * DESCRIPTION: Return the oid of last insert, if last exec was an INSERT.
 *              Othersie return a NULL string.
 * ARGS:        
 * RETURNS:     OID of last insert
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "sql_getlastoid"
char *
sql_getlastoid()
{
    return(r_sql_getlastoid(&def_result_index));
} /* r_sql_getlastoid */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    sql_begin_work
 * DESCRIPTION: Begin a transaction
 * ARGS:        
 * RETURNS:     
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "sql_begin_work"
int
sql_begin_work()
{

    /* Begin the transaction */
    if (sql_exec("begin work;") != 0) {
        misc_warnx(FUNCTION_NAME ": sql_begin_work work failed");
        return(-1);
    }
    return(0);
    
} /* sql_begin_work */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    sql_rollback_work
 * DESCRIPTION: Rollback a transaction
 * ARGS:        
 * RETURNS:     
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "sql_rollback_work"
int
sql_rollback_work()
{

    /* Rollback the transaction */
    if (sql_exec("rollback work;") != 0) {
        misc_warnx(FUNCTION_NAME ": sql_rollback_work work failed");
        return(-1);
    }
    return(0);
    
} /* sql_rollback_work */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    sql_commit_work
 * DESCRIPTION: Commit a transaction
 * ARGS:        
 * RETURNS:     
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "sql_commit_work"
int
sql_commit_work()
{

    /* Commit the transaction */
    if (sql_exec("commit work;") != 0) {
        misc_warnx(FUNCTION_NAME ": sql_commit_work failed");
        return(-1);
    }
    return(0);
    
} /* sql_commit_work */
#undef FUNCTION_NAME

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    sql_hide_metachars
 * DESCRIPTION: Escape single quotes in string (' -> '')
 * ARGS:        str
 * RETURNS:     pointer to converted string
 * ----------------------------------------------------------------------
 */

#define FUNCTION_NAME "sql_hide_metachars"
#define BUF_LEN 128
char *
sql_hide_metachars(str)
  char *str;
{
    static char *buf = NULL;
    static int buf_len = 0;
    int str_len;
    char *dst, *src;

        /* initialize the output buffer */
    if (buf == NULL) {
        buf = (char *) malloc(BUF_LEN + 1);
        buf_len = BUF_LEN;
    }
    memset(buf, 0, buf_len);
    
    str_len = strlen(str);
        /* Extend the buffer if necessary */
    if (str_len*2 > buf_len) {
        buf_len = str_len*2 + 1;
        realloc(buf, buf_len);
    }

    for (dst = buf, src = str; *src != '\0'; src++) {
        switch (*src)
        {
            case '\'':
                *dst++ = '\'';
                *dst++ = '\'';
                break;
            default:
                *dst++ = *src;
        }
    }
    *dst = '\0';
    
    return(buf);
    

} /* sql_hide_metachars */
#undef FUNCTION_NAME

