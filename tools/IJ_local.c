/*     $Id: IJ_local.c 21 2000-01-10 22:00:29Z source $ */

/*   Copyright 1998 X=X Computer Software Trust */
/*    		  Kangaroo Ground Australia 3097 */


/*  This is free software; you can redistribute it and/or modify */
/*  it under the terms of the GNU General Public License published by */
/*  the Free Software Foundation; either version 2, or (at your option) */
/*  any later version. */
 
/*  This software is distributed in the hope that it will be useful, */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*  GNU General Public License for more details. */
 
/*  You should have received a copy of the GNU General Public License */
/*  along with this software; see the file COPYING.  If not, write to */
/*  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. */
 
/*  Report problems and direct all questions to: */
 
/*      Rex McMaster, rmcm@compsoft.com.au */
 

 
/*  Revision Log */
  

#include <stdio.h>

#define BUFF_LEN 512
#define PEEK_LEN 20

main(argc,argv)	/* execv a program */
int argc;
char *argv[];
{
        int     i,i_peek;
        char    peek_buff[PEEK_LEN];
        int     width,length;
        char    buff[BUFF_LEN];
        FILE*   psfilter;
        extern int opterr;
        extern char *optarg;
        char c;

	opterr = 0;

/* Arguments to filter from lpr
HP_if
-w132
-l66
-i0
-n
lcs
-h
host
*/
        while ((c = getopt(argc,argv,"w:l:i:nh")) != EOF) {
                switch (c) {
                case 'w':
			width = atoi(optarg);
                        break;
                case 'l':
			length = atoi(optarg);
                        break;
                case '?': 
                        fprintf(stderr, "usage: %s [b]\n", argv[0]);
                        fprintf(stderr, 
                                "\tb: background (def. foreground)\n",
                                argv[0]);
                        return(1); 
                }
        }

        /* Check for Postscript %! */
        
        i_peek = read(0, peek_buff, PEEK_LEN);
        
        if (i_peek >= 2 && peek_buff[0] == '%' && peek_buff[1] == '!')
          {
            /* send to postscript filter */
            psfilter = popen("/usr/local/bin/psfilter -ij", "w");
            fprintf(psfilter, "%*.*s", i_peek, i_peek, peek_buff);
            while ((i = read(0,buff,BUFF_LEN)) > 0) {
              fprintf(psfilter, "%*.*s", i, i, buff);
            }
            pclose(psfilter);
          }
        else
          {
            if (width > 96) { /* Small font + Portrait */
              i = sprintf(buff,"\033(s16.66H");
            } else {   /* Standard font + Portrait */
              i = sprintf(buff,"\033(s12H");
            }
            
            write(1,buff,strlen(buff));
            write(1,peek_buff,i_peek);
            while ((i = read(0,buff,BUFF_LEN)) > 0) {
              if (write(1,buff,i) < 0) {
                perror("error writing to output");
                return(-1);
              }
            }
            write(1,"\033E",2);
          }
/*      write(1,"\014",1);*/
        return(0);

}
