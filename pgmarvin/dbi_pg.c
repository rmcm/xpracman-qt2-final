/*
 * $Id: dbi_pg.c 917 2001-10-14 21:59:23Z source $ 
 *   
 * Copyright 1998 X=X Computer Software Trust 
 * Kangaroo Ground Australia 3097 
 *   
 *   
 * This is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License published by 
 * the Free Software Foundation; either version 2, or (at your option) 
 * any later version. 
 *   
 * This software is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 *   
 * You should have received a copy of the GNU General Public License 
 * along with this software; see the file COPYING.  If not, write to 
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *   
 * Report problems and direct all questions to: 
 *   
 * Rex McMaster, rmcm@compsoft.com.au 
 *   
 *   
 */


#ifdef __cplusplus
extern "C"
{
#endif

/*
 * ----------------------------------------------------------------------
 * DESCRIPTION:  The PRAC interface to a PostgreSQL database. A series of
 *standardised functions used by PRAC client software. These are
 *based on the PHP interface to the PostgreSQL database.
 * ----------------------------------------------------------------------
 */



/*
 * ----------------------------------------------------------------------
 * INCLUDES:
 * ----------------------------------------------------------------------
 */

#include <sys/errno.h>
#include "dbi_pg.h"

/*
 * ----------------------------------------------------------------------
 * DEFINES:
 * ----------------------------------------------------------------------
 */

/*
 * ----------------------------------------------------------------------
 * LOCAL VARIABLES:
 * ----------------------------------------------------------------------
 */
    
static char dbi_pg_version[] = "$Id: dbi_pg.c 917 2001-10-14 21:59:23Z source $";

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


/*************************************************************
 * Open a connection to a database
 *************************************************************/
DBI_conn *
dbi_connect(const char *db_host,
            const char *db_port,
            const char *db_options,
            const char *db_tty,
            const char *db_name,
            const char *db_login,
            const char *db_pwd)
{
  DBI_conn *conn;
  conn = PQsetdbLogin(db_host, db_port, db_options, db_tty, db_name, db_login, db_pwd);
  if (PQstatus(conn) == CONNECTION_BAD)
    {
      return((DBI_conn *) NULL);
    }
  else
    {
      return(conn);
    }
}
         
/*************************************************************
 * Close an existing connection to a database
 *************************************************************/
void
dbi_close(DBI_conn *conn)
{
  PQfinish(conn);
}
         
/*************************************************************
 * Reset an existing connection to a database
 *************************************************************/
void
dbi_reset(DBI_conn *conn)
{
  PQreset(conn);
}
         
/*************************************************************
 * Set tracing on an existing connection to a database
 *************************************************************/
void
dbi_trace(DBI_conn *conn,
          FILE* debug_port)
{
  PQtrace(conn, debug_port);
}
         
/*************************************************************
 * Set tracing on an existing connection to a database
 *************************************************************/
void
dbi_untrace(DBI_conn *conn)
{
  PQuntrace(conn);
}
         
/*************************************************************
 * Returns the host name of the connection. 
 *************************************************************/
char *
dbi_host(DBI_conn *conn)
{
  return(PQhost(conn));
}
         
/*************************************************************
 * Returns the port of the connection. 
 *************************************************************/
char *
dbi_port(DBI_conn *conn)
{
  return(PQport(conn));
}
         
/*************************************************************
 *Returns the options used in the connection. 
 *************************************************************/
char *
dbi_options(DBI_conn *conn)
{
  return(PQoptions(conn));
}
         
/*************************************************************
 *Returns the tty of the connection. 
 *************************************************************/
char *
dbi_tty(DBI_conn *conn)
{
  return(PQtty(conn));
}
         
/*************************************************************
 *Returns the database name of the connection.
 *************************************************************/
char *
dbi_dbname(DBI_conn *conn)
{
  return(PQdb(conn));
}
         
/*************************************************************
 *Returns the status of the connection.
 *************************************************************/
DBI_status
dbi_status(DBI_conn *conn)
{
  return(PQstatus(conn));
}
         
/*************************************************************
 *Returns the status of the query.
 *************************************************************/
DBI_ExecStatusType
dbi_result_status(DBI_result *result_id)
{
  return(PQresultStatus(result_id));
}
         
/*************************************************************
 *Returns the status of the command status of last query.
 *************************************************************/
char *
dbi_cmd_status(DBI_result *result_id)
{
  return(PQcmdStatus(result_id));
}
         
/*************************************************************
 *Returns the number of rows affected by the SQL command.
 *************************************************************/
int
dbi_cmd_tuples(DBI_result *result_id)
{
  return(atoi(PQcmdTuples(result_id)));
}
         
/*************************************************************
 * Submit a query and return a result pointer
 *************************************************************/
DBI_result *
dbi_exec(DBI_conn *conn,
         const char *query)
{
  DBI_result *result;
  result = PQexec(conn, query);
  if (PQresultStatus(result) <= PGRES_TUPLES_OK)
    {
      return(result);
    }
  else
    {
      PQclear(result);
      return((DBI_result *) NULL);
    }
}
         
/*************************************************************
 * Extract a tuple/field value from a result pointer.
 *************************************************************/
char *
dbi_result(DBI_result *result_id,
           const int tup_num,
           const char *field_id)
{
  int field_num;

  field_num = dbi_atoi_field(result_id, field_id);
  return(PQgetvalue(result_id, tup_num, field_num));
}
         
/*************************************************************
 *Free the storage associated with the DBI_result
 *************************************************************/
void
dbi_freeresult(DBI_result *result_id)
{
      PQclear(result_id);
}
         
/*************************************************************
 * Returns a string with the object id of the tuple inserted
 if the last query is an INSERT command. Otherwise, returns
 an empty string. 
 *************************************************************/
char *
dbi_getlastoid(DBI_result *result_id)
{
  return(PQoidStatus(result_id));
}
         
/*************************************************************
 * Returns the number of tuples (instances) in the query resul
 *************************************************************/
int
dbi_numrows(DBI_result *result_id)
{
  return(PQntuples(result_id));
}
         
/*************************************************************
 *Returns the number of fields (attributes) in the query result
 *************************************************************/
int
dbi_numfields(DBI_result *result_id)
{
  return(PQnfields(result_id));
}
         
/*************************************************************
 *Returns the field (attribute) index associated with the
 given field name.
 *************************************************************/
int
dbi_fieldnum(DBI_result *result_id,
             char* field_name)
{
  return(PQfnumber(result_id, field_name));
}
         
/*************************************************************
 *Returns the field (attribute) name associated with the given
 field index. Field indices start at 0.
 *************************************************************/
char *
dbi_fieldname(DBI_result *result_id,
              int field_num)
{
  return(PQfname(result_id, field_num));
}
         
/*************************************************************
 *Returns the field type associated with the given field index.
 *************************************************************/
Oid
dbi_fieldtype(DBI_result *result_id,
              int field_num)
{
  return(PQftype(result_id, field_num));
}
         
/*************************************************************
 *Returns the size in bytes of the field associated with the
 given field index. If the size returned is -1, the field is
 a variable length field. Field indices
     start at 0.
 *************************************************************/
int
dbi_fieldsize(DBI_result *result_id,
              int field_num)
{
  return(PQfsize(result_id, field_num));
}
         
/*************************************************************
 *Returns the length of a field (attribute) in bytes. If the
 field is a struct varlena, the length returned here does not
 include the size field of the varlena,
     i.e., it is 4 bytes less. 
 *************************************************************/
int
dbi_fieldprtlen(DBI_result *result_id,
                int tup_num,
                char *field_id)
{
  
  return(PQgetlength(result_id, tup_num, dbi_atoi_field(result_id, field_id)));
}
         
/*************************************************************
 * Returns the command status associated with the last query command.
 *************************************************************/
char *
dbi_errormessage(DBI_conn *conn)
{
  return(PQerrorMessage(conn));
}

/*************************************************************
 * Returns the field number of field string, where field-string
 may be a field name or a string version of field number.
 *************************************************************/
int
dbi_atoi_field(DBI_result *result_id,
               const char * field_id)
{
  int field_num;
  char *ptr = '\0';
  extern int errno;

  field_num = (int) strtol(field_id, &ptr, 10);
  if (field_num == 0 && ptr != '\0')
    {
      field_num = PQfnumber(result_id, field_id);
    }
  return(field_num);
}

/*************************************************************
 *************************************************************
 * Asynchronous Query Processing Functions
 *************************************************************
 *************************************************************/

/********************************************************************************
 * Sets the state of the connection to non-blocking

 int PQsetnonblocking(PGconn *conn, int arg)

 this function will ensure that calls to PQputline, PQputnbytes,
 PQsendQuery and PQendcopy will not block but instead return an error
 if they need to be called again.

 When a database connection has been set to non-blocking mode and
 PQexec is called, it will temporarily set the state of the connection
 to blocking until the PQexec completes.
 ********************************************************************************/
int
dbi_setnonblocking(DBI_conn *conn, int arg)
{
    return PQsetnonblocking(conn, arg);
}

/********************************************************************************
 * Returns the blocking status of the database connection

 int PQisnonblocking(const PGconn *conn)

 Returns TRUE if the connection is set to non-blocking mode, FALSE if
 blocking.
 ********************************************************************************/
int
dbi_isnonblocking(const DBI_conn *conn)
{
    return PQisnonblocking(conn);
}

/********************************************************************************
 * Submit a query to Postgres without waiting for the result(s). TRUE
 * is returned if the query was successfully dispatched, FALSE if not
 * (in which case, use PQerrorMessage to get more information about
 * the failure).

 int PQsendQuery(PGconn *conn, const char *query);

 After successfully calling PQsendQuery, call PQgetResult one or more
 times to obtain the query results. PQsendQuery may not be called
 again (on the same connection) until PQgetResult has returned NULL,
 indicating that the query is done.
 ********************************************************************************/
int
dbi_sendquery(DBI_conn *conn, const char *query)
{
    return PQsendQuery(conn, query);
}

/********************************************************************************
 * Wait for the next result from a prior PQsendQuery, and return
 * it. NULL is returned when the query is complete and there will be
 * no more results.

 PGresult *PQgetResult(PGconn *conn);

 PQgetResult must be called repeatedly until it returns NULL,
 indicating that the query is done. (If called when no query is
 active, PQgetResult will just return NULL at once.) Each non-null
 result from PQgetResult should be processed using the same PGresult
 accessor functions previously described. Don't forget to free each
 result object with PQclear when done with it. Note that PQgetResult
 will block only if a query is active and the necessary response data
 has not yet been read by PQconsumeInput.
 ********************************************************************************/
DBI_result *
dbi_getresult(DBI_conn *conn)
{
    return PQgetResult(conn);
}

/********************************************************************************
 * If input is available from the backend, consume it.

 int PQconsumeInput(PGconn *conn);

 PQconsumeInput normally returns 1 indicating "no error", but returns
 0 if there was some kind of trouble (in which case PQerrorMessage is
 set). Note that the result does not say whether any input data was
 actually collected. After calling PQconsumeInput, the application may
 check PQisBusy and/or PQnotifies to see if their state has changed.

 PQconsumeInput may be called even if the application is not prepared
 to deal with a result or notification just yet. The routine will read
 available data and save it in a buffer, thereby causing a select(2)
 read-ready indication to go away. The application can thus use
 PQconsumeInput to clear the select condition immediately, and then
 examine the results at leisure.
 ********************************************************************************/
int
dbi_consumeinput(DBI_conn *conn)
{
    return PQconsumeInput(conn);
}

/********************************************************************************
 * Returns 1 if a query is busy, that is, PQgetResult would block
 * waiting for inpint ut. A 0 return indicates that PQgetResult can be
 * called with assurance of not blocking.

 int PQisBusy(DBI_conn *conn);

 PQisBusy will not itself attempt to read data from the backend;
 therefore PQconsumeInput must be invoked first, or the busy state
 will never end.
 
 ********************************************************************************/
int
dbi_isbusy(DBI_conn *conn)
{
    return PQisBusy(conn);
}

/********************************************************************************
 * Attempt to flush any data queued to the backend, returns 0 if
 * successful (or if the send queue is empty) or EOF if it failed for
 * some reason.

 int PQflush(PGconn *conn);

 PQflush needs to be called on a non-blocking connection before
 calling select to determine if a responce has arrived. If 0 is
 returned it ensures that there is no data queued to the backend that
 has not actually been sent. Only applications that have used
 PQsetnonblocking have a need for this.
 ********************************************************************************/
int
dbi_flush(DBI_conn *conn)
{
    return PQflush(conn);
}

/********************************************************************************
 *  Obtain the file descriptor number for the backend connection
 *  socket. A valid descriptor will be >= 0; a result of -1 indicates
 *  that no backend connection is currently open.

 int PQsocket(const PGconn *conn);

 PQsocket should be used to obtain the backend socket descriptor in
 preparation for executing select(2). This allows an application using
 a blocking connection to wait for either backend responses or other
 conditions. If the result of select(2) indicates that data can be
 read from the backend socket, then PQconsumeInput should be called to
 read the data; after which, PQisBusy, PQgetResult, and/or PQnotifies
 can be used to process the response.

 Non-blocking connections (that have used PQsetnonblocking) should not
 use select until PQflush has returned 0 indicating that there is no
 buffered data waiting to be sent to the backend.
 ********************************************************************************/
int
dbi_socket(const DBI_conn *conn)
{
    return PQsocket(conn);
}

/********************************************************************************
 * Request that Postgres abandon processing of the current query.
 
 int PQrequestCancel(PGconn *conn);

 The return value is 1 if the cancel request was successfully
 dispatched, 0 if not. (If not, PQerrorMessage tells why not.)
 Successful dispatch is no guarantee that the request will have any
 effect, however. Regardless of the return value of PQrequestCancel,
 the application must continue with the normal result-reading sequence
 using PQgetResult. If the cancellation is effective, the current
 query will terminate early and return an error result. If the
 cancellation fails (say, because the backend was already done
 processing the query), then there will be no visible result at all.

 Note that if the current query is part of a transaction, cancellation
 will abort the whole transaction.

 PQrequestCancel can safely be invoked from a signal handler. So, it
 is also possible to use it in conjunction with plain PQexec, if the
 decision to cancel can be made in a signal handler. For example, psql
 invokes PQrequestCancel from a SIGINT signal handler, thus allowing
 interactive cancellation of queries that it issues through
 PQexec. Note that PQrequestCancel will have no effect if the
 connection is not currently open or the backend is not currently
 processing a query.
 ********************************************************************************/
int
dbi_requestcancel(DBI_conn *conn)
{
    return PQrequestCancel(conn);
}


#ifdef __cplusplus
}
#endif


