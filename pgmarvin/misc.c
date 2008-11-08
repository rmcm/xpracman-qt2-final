/*
 *     $Id: misc.c 917 2001-10-14 21:59:23Z source $
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
 *  Revision Log
 *   
 *   $Log$
 *   Revision 1.3  2001/10/14 21:59:23  source
 *   add versioning variables
 *
 *   Revision 1.2  2000/11/05 07:51:20  source
 *   message_control_id(), timestamp(): transfer to hl7tool::hl7:
 *
 *   Revision 1.1  2000/10/23 03:38:19  source
 *   Version transferred from hl7tool
 *
 *   Revision 1.5  2000/10/04 22:59:57  source
 *   Remove cxref header
 *
 *   Revision 1.4  2000/09/28 02:21:23  source
 *   set_std_err(): new function
 *
 *   Revision 1.3  2000/09/28 02:04:49  source
 *   standardize format
 *
 *   Revision 1.2  2000/09/19 11:07:51  source
 *   syslog option
 *
 *   Revision 1.1  2000/09/17 06:34:31  source
 *   first cut
 *

 */


/*
 * ----------------------------------------------------------------------
 * DESCRIPTION: Miscellaneous functions
 *
 * ----------------------------------------------------------------------
 */



/*
 * ----------------------------------------------------------------------
 * INCLUDES:
 * ----------------------------------------------------------------------
 */

#include <syslog.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "misc.h"

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
    
static char misc_version[] = "$Id: misc.c 917 2001-10-14 21:59:23Z source $";
static enum std_error local_std_error; /*+ Error destination +*/
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
 * FUNCTION:    set_std_err
 * DESCRIPTION: Set the static global variable std_error
 * ARGS:        i
 * RETURNS:     
 * ----------------------------------------------------------------------
 */

void
set_std_err(i)
enum std_error i;
{
    local_std_error = i;
    
} /* set_std_err */

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    misc_warnx
 * DESCRIPTION: 
 * ARGS:        format ...
 * RETURNS:     
 * ----------------------------------------------------------------------
 */

void
misc_warnx(char *fmt, ...)      /*+ Wrapper for warnx +*/
{
    va_list ap;

    va_start(ap, fmt);

    if (local_std_error == USE_SYSLOG) {
        vsyslog(LOG_INFO, fmt, ap);
    } else {
        vfprintf(stderr, fmt, ap);
        fprintf(stderr, "\n");
        fflush(stderr);
    }
    va_end(ap);

} /* misc_warnx */

/*
 * ----------------------------------------------------------------------
 * FUNCTION:    misc_errx
 * DESCRIPTION: 
 * ARGS:        format ...
 * RETURNS:     
 * ----------------------------------------------------------------------
 */

void
misc_errx(int eval, char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);

    if (local_std_error == USE_SYSLOG) {
        vsyslog(LOG_INFO, fmt, ap);
    } else {
        vfprintf(stderr, fmt, ap);
        fprintf(stderr, "\n");
        fflush(stderr);
    }
    va_end(ap);

    exit(eval);

} /* misc_errx */

