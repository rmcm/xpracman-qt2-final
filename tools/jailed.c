/*
 *     $Id: jailed.c 1207 2002-03-16 02:10:15Z source $
 * 
 *   Copyright 2001 X=X Computer Software Trust
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

/*
 * ======================================================================
 * DESCRIPTION: 
 *
 * ======================================================================
 */

/*
 * A chroot wrapper for /bin/sh. This must be compiled with a specific
 * user details. Replace USERNAME with user logname.
 *
 * To compile:
 *
 *      cc "-DUSERHOME=\"/var/users/USERNAME\"" -o jailed-USERNAME jailed.c 
 *
 * To install:
 *
 *      install -c -o root -g USERGROUP -m 04750 jailed-USERNAME /usr/local/sbin/jailed-USERNAME
 *
 * Create a user account (eg keepout );
 * 1. Build a jailed shell for keepout
 * 
 *      gmake USERHOME=/var/users/keepout jailed-keepout 
 * 
 * 2. Create the user
 * 
 *   Give the keepout user a distinct group (eg
 *   jailed - create a new entry in /etc/group )
 * 
 *      # as root
 *      echo 'jailed:*:999:' >> /etc/group
 *      user add -m -b /var/users -g jailed keepout
 * 
 * 3. Install the jailed shell and make it the login shell for keepout
 * 
 *   You should now have a usershell called jailed-keepout. Install this
 *   in /usr/local/sbin 
 * 
 *      # as root ( ie sudo tcsh first)
 *      install -c -o root -g jailed -m 04750 jailed-keepout /usr/local/sbin/jailed-keepout
 *      usermod -g jailed -s /usr/local/sbin/jailed-keepout keepout
 *      
 * 
 * 4. Populate the keepout homedir. 
 * 
 *      # as root ( should be already root from 2 above)     
 * 
 *      cd ~keepout
 *      mkdir -p etc bin usr/lib usr/bin usr/libexec
 *      cp -p /bin/sh bin
 *      cp -p /usr/bin/scp usr/bin
 *      cp -p /usr/lib/libc.so.12 usr/lib
 *      cp -p /usr/libexec/ld.elf_so usr/libexec
 *      grep keepout /etc/master.passwd > etc/passwd.keepout
 *      pwd_mkdb -d ~keepout etc/passwd.keepout
 * 
 * You should now be able to su to keepout;
 * 
 *     su - keepout
 *
 * To add scp access;
 *
 *      
 * 
 * and you should be able to scp files to the keepout homedir. When
 * running as keepout (eg vi su or ssh) you should be unable to see
 * outside the keepout home directory.
 * 
 *
 *
 *
 *
 *
 */
/*
 * ======================================================================
 * INCLUDES:
 * ====================================================================== */
#include <stdio.h>
#include <unistd.h>

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char * ENV[] =
{ "HOME=/", "PATH=/bin:/usr/bin", "SHELL=/bin/sh", "TERM=vt220", (char *)0 };


#ifndef USERHOME
#define USERHOME "/var/users/jailed"
#endif

/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */

/*
 * ======================================================================
 * LOCAL FUNCTION PROTOTYPES:
 * ======================================================================
 */

/*
 * ======================================================================
 * LOCAL FUNCTIONS:
 * ======================================================================
 */

/*
 * ======================================================================
 * GLOBAL FUNCTIONS:
 * ======================================================================
 */


int main(int argc, char ** argv)
{
    int i;
    
#define SH "/bin/sh"
    
    if ( strlen( argv[0] ) < strlen(SH) )
    {
        fprintf( stderr,"internal error - "
                 SH
                 " to long for argv[0]\n");
        exit(1);
    }
    argv[0] = SH;
    
#ifdef DEBUG     
    for (i=0; i< argc; i++)
        fprintf(stderr, " %d = %s\n", i, argv[i]);
    fprintf(stderr, "euid=%d, uid=%d\n", geteuid(), getuid() );
#endif 
    if (chdir(USERHOME)||
        chroot(".")||
        chdir("/"))
        return 1;
    setuid(getuid());
#ifdef DEBUG
    fprintf(stderr, "euid=%d, uid=%d\n", geteuid(), getuid() );
#endif 
    execve(argv[0], argv, ENV);
    perror(argv[0]);
    return 1;
}
