/*
 *     $Id: sql.h 1498 2003-01-08 08:30:26Z source $
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
 */

#ifndef SQL_H
#define SQL_H
#ifdef __cplusplus
extern "C"
{
#endif
    
/*
 * ----------------------------------------------------------------------
 * DESCRIPTION: 
 *
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

/*
 * ----------------------------------------------------------------------
 * DEFINITIONS - typedefs, enums, structures etc
 * ----------------------------------------------------------------------
 */
#define TRUE    1
#define FALSE   0
#define SQL_GETRESULT_ILLEGALCONN       -1 /* illegal connection index. */
#define SQL_GETRESULT_NOQUERY           -2 /* connection does not have a pending query. */
#define SQL_GETRESULT_BADSTATUS         -3 /* failed to check status */
#define SQL_GETRESULT_BUSY              -4 /* query is busy */
#define SQL_GETRESULT_NONFATAL          -5 /* non_fatal query error */
#define SQL_GETRESULT_FATAL             -6 /* fatal query error */
#define SQL_GETRESULT_END               -7 /* no further query results */
#define SQL_GETRESULT_UNKNOWN           -8 /* unknown error */

    
#define SQL_ISBUSY_ILLEGALCONN -1 /* illegal connection index. */
#define SQL_ISBUSY_NOQUERY -2 /* connection does not have a pending query. */
#define SQL_ISBUSY_BADSTATUS -3 /* failed to check status */
/*
 * ----------------------------------------------------------------------
 * EXTERN VARIABLES:
 * ----------------------------------------------------------------------
 */

/*
 * ----------------------------------------------------------------------
 * FUNCTION PROTOTYPES: make sql.cproto
 * ----------------------------------------------------------------------
 */

/* sql.c */

    extern int r_sql_dbconnect __P((const char *pghost,
                                    const char *pgport,
                                    const char *pgoptions,
                                    const char *pgtty,
                                    const char *pgdatabase,
                                    const char *pglogin,
                                    const char *pgpwd));
    extern void *r_sql_dbclose __P((int conn_index));
    extern int r_sql_init __P((int conn_index));
    extern int r_sql_exec __P((int conn_index, int *result_index, const char *query));
    extern int r_sql_send __P((int conn_index, const char *query));
    extern int r_sql_cancel __P((int conn_index));
    extern int r_sql_getresult __P((int conn_index, int *result_index));
    extern char *r_sql_value __P((int *result_index, const int row, const char *fieldname));
    extern char *r_sql_error __P((int conn_index));
    extern char *r_sql_getlastoid __P((int *result_index));
    extern void *r_sql_freeresult __P((int *result_index));
    extern int sql_dbconnect __P((const char *pghost,
                                  const char *pgport,
                                  const char *pgoptions,
                                  const char *pgtty,
                                  const char *pgdatabase,
                                  const char *pglogin,
                                  const char *pgpwd));
    extern void *sql_dbclose __P((void));
    extern int sql_exec __P((const char *query));
    extern char *sql_value __P((const int row, const char *fieldname));
    extern char *sql_error __P((void));
    extern char *sql_getlastoid __P((void));
    extern int sql_begin_work __P((void));
    extern int sql_rollback_work __P((void));
    extern int sql_commit_work __P((void));
    extern char *sql_hide_metachars __P((char *str));
    
/*
 * ----------------------------------------------------------------------
 * #DEFINES:
 * ----------------------------------------------------------------------
 */


#define SQL_MAX_QUERY_LEN   2048

    
#ifdef __cplusplus
}
#endif

#endif /* SQL_H */
