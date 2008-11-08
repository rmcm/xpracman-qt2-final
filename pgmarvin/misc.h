/*
 *     $Id: misc.h 1497 2003-01-08 08:29:25Z source $
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

 */

#ifndef MISC_H
#define MISC_H
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

enum std_error {
    USE_STDERR,
    USE_SYSLOG
};

/*
 * ----------------------------------------------------------------------
 * EXTERN VARIABLES:
 * ----------------------------------------------------------------------
 */

/*
 * ----------------------------------------------------------------------
 * FUNCTION PROTOTYPES:
 * ----------------------------------------------------------------------
 */
extern void set_std_err __P((enum std_error i));
extern void misc_warnx __P((char *fmt, ...));
extern void misc_errx __P((int eval, char *fmt, ...));
/*
 * ----------------------------------------------------------------------
 * #DEFINES:
 * ----------------------------------------------------------------------
 */



#endif /* MISC_H */
