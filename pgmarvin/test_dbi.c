/*
 * $Id: test_dbi.c 361 2001-05-15 21:50:55Z source $ 
 *   
 * Copyright 2000 X=X Computer Software Trust 
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

/* Test file the PRAC interface to PostgreSQL database.
*/

#include <stdio.h>
#include "dbi_pg.h"

void
exit_nicely(PGconn* conn)
{
  dbi_close(conn);
  exit(1);
}

int
test_dbi()

{
  char *pghost, *pgport, *pgoptions, *pgtty, *pglogin, *pgpassword;
  char* dbName;
  int nFields;
  int i,j;

/*  FILE *debug; */

  PGconn* conn;
  PGresult* res;

  /* begin, by setting the parameters for a backend connection
     if the parameters are null, then the system will try to use
     reasonable defaults by looking up environment variables
     or, failing that, using hardwired constants */
  pghost = NULL;  /* host name of the backend server */
  pgport = NULL;  /* port of the backend server */
  pgoptions = NULL; /* special options to start up the backend server */
  pgtty = NULL;     /* debugging tty for the backend server */
  dbName = "prac";
  pglogin = NULL;
  pgpassword = NULL;

  /* make a connection to the database */
  conn = dbi_connect(pghost, pgport, pgoptions, pgtty, dbName, pglogin, pgpassword);

  /* check to see that the backend connection was successfully made */
  if (dbi_status(conn) == CONNECTION_BAD) {
    fprintf(stderr,"Connection to database '%s' failed.", dbName);
    fprintf(stderr,"%s",dbi_errormessage(conn));
    exit_nicely(conn);
  }

  /*  debug = fopen("/tmp/trace.out","w");  */
  /*   PQtrace(conn, debug);  */
  
  /* start a transaction block */

  res = dbi_exec(conn,"BEGIN");
  if (dbi_result_status(res) != PGRES_COMMAND_OK) {
    fprintf(stderr,"BEGIN command failed");
    dbi_freeresult(res);
    exit_nicely(conn);
  }
  /* should dbi_freeresult PGresult whenever it is no longer needed to avoid
     memory leaks */
  dbi_freeresult(res);

  /* fetch instances from the pg_database, the system catalog of databases*/
  res = dbi_exec(conn,"DECLARE myportal CURSOR FOR select * from prov where prov__sequence = 2");
  if (dbi_result_status(res) != PGRES_COMMAND_OK) {
    fprintf(stderr,"DECLARE CURSOR command failed");
    dbi_freeresult(res);
    exit_nicely(conn);
  }
  dbi_freeresult(res);
  
  res = dbi_exec(conn,"FETCH ALL in myportal");
  if (dbi_result_status(res) != PGRES_TUPLES_OK) {
    fprintf(stderr,"FETCH ALL command didn't return tuples properly");
    dbi_freeresult(res);
    exit_nicely(conn);
  }
  
  /* first, print out the attribute names */
  nFields = dbi_numfields(res);
  for (i=0; i < nFields; i++) {
    printf("%-15s",dbi_fieldname(res,i));
  }
  printf("\n");
  
  /* next, print out the instances */
  for (i=0; i < dbi_numrows(res); i++) {
    char *j_str;
    for (j=0  ; j < nFields; j++) {
      j_str = dbi_fieldname(res,j);
      printf("%-15s", dbi_result(res,i,j_str));
    }
    printf("\n");
  }

  dbi_freeresult(res);

  /* close the portal */
  res = dbi_exec(conn, "CLOSE myportal");
  dbi_freeresult(res);

  /* end the transaction */
  res = dbi_exec(conn, "END");
  dbi_freeresult(res);

  /* close the connection to the database and cleanup */
  dbi_close(conn);

/*   fclose(debug); */
}
