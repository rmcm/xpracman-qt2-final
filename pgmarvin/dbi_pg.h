/*
 * $Id: dbi_pg.h 1497 2003-01-08 08:29:25Z source $ 
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

#ifndef DBI_PG_H
#define DBI_PG_H


/*
 * ----------------------------------------------------------------------
 * DESCRIPTION: 
 *
 * MARVIN interface to PGSQL database. A series of standardised
 * functions used by MARVIN client software.
 * ----------------------------------------------------------------------
 */

/*
 * ----------------------------------------------------------------------
 * INCLUDES:
 * ----------------------------------------------------------------------
 */

/* For cxref */
#include <sys/cdefs.h>
#if defined(__FEATURETEST__)
#include <sys/featuretest.h>
#endif
#include <libpq-fe.h>
/*
 * ----------------------------------------------------------------------
 * DEFINITIONS - typedefs, enums, structures etc
 * ----------------------------------------------------------------------
 */

/*
 * ----------------------------------------------------------------------
 * EXTERN VARIABLES:
 * ----------------------------------------------------------------------
 */

/*
 * ----------------------------------------------------------------------
 * INTERFACE FUNCTIONS:
 * ----------------------------------------------------------------------
 */

/* dbi_pg.c */

extern PGconn *dbi_connect __P((const char *db_host, const char *db_port, const char *db_options,
                                const char *db_tty, const char *db_name, const char *db_login,
                                const char *db_pwd));
extern void dbi_close __P((PGconn *conn));
extern void dbi_reset __P((PGconn *conn));
extern void dbi_trace __P((PGconn *conn, FILE *debug_port));
extern void dbi_untrace __P((PGconn *conn));
extern char *dbi_host __P((PGconn *conn));
extern char *dbi_port __P((PGconn *conn));
extern char *dbi_options __P((PGconn *conn));
extern char *dbi_tty __P((PGconn *conn));
extern char *dbi_dbname __P((PGconn *conn));
extern ConnStatusType dbi_status __P((PGconn *conn));
extern ExecStatusType dbi_result_status __P((PGresult *result_id));
extern char *dbi_cmd_status __P((PGresult *result_id));
extern int dbi_cmd_tuples __P((PGresult *result_id));
extern PGresult *dbi_exec __P((PGconn *conn, const char *query));
extern char *dbi_result __P((PGresult *result_id, const int tup_num, const char *field_id));
extern void dbi_freeresult __P((PGresult *result_id));
extern char *dbi_getlastoid __P((PGresult *result_id));
extern int dbi_numrows __P((PGresult *result_id));
extern int dbi_numfields __P((PGresult *result_id));
extern int dbi_fieldnum __P((PGresult *result_id, char *field_name));
extern char *dbi_fieldname __P((PGresult *result_id, int field_num));
extern Oid dbi_fieldtype __P((PGresult *result_id, int field_num));
extern int dbi_fieldsize __P((PGresult *result_id, int field_num));
extern int dbi_fieldprtlen __P((PGresult *result_id, int tup_num, char *field_id));
extern char *dbi_errormessage __P((PGconn *conn));
extern int dbi_atoi_field __P((PGresult *result_id, const char *field_id));
extern int dbi_setnonblocking __P((PGconn *conn, int arg));
extern int dbi_isnonblocking __P((const PGconn *conn));
extern int dbi_sendquery __P((PGconn *conn, const char *query));
extern PGresult *dbi_getresult __P((PGconn *conn));
extern int dbi_consumeinput __P((PGconn *conn));
extern int dbi_isbusy __P((PGconn *conn));
extern int dbi_flush __P((PGconn *conn));
extern int dbi_socket __P((const PGconn *conn));
extern int dbi_requestcancel __P((PGconn *conn));

/* END dbi_pg.c */

/*
 * ----------------------------------------------------------------------
 * #DEFINES:
 * ----------------------------------------------------------------------
 */


/*** Map from libpq-fe.h ***/
#define DBI_conn PGconn
#define DBI_status ConnStatusType
#define DBI_ExecStatusType ExecStatusType
#define DBI_result PGresult
#define DBI_RES_COMMAND_OK PGRES_COMMAND_OK
#define DBI_RES_TUPLES_OK PGRES_TUPLES_OK



#endif							/* DBI_PG_H */
