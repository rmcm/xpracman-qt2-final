/*     $Id: HP_local.c 22 2000-02-02 23:13:30Z source $ */

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
  
/*   $Log$
 *   Revision 1.5  2000/02/02 23:13:30  source
 *   add indent handling
 *
 *   Revision 1.4  1998/12/22 03:58:51  source
 *   * HP_local.c: Set line spacing in small-font, portrait
 *
 *   Revision 1.3  1998/12/21 04:27:01  source
 *   Add 3rd width-based format
 *
 *   Revision 1.2  1998/06/01 03:52:21  source
 *   * HP_local.c: Bug fix in PS detection.
 *
 *   Revision 1.1  1998/05/30 03:52:26  source
 *   Initial revision
 * */

#include <stdio.h>

#define BUFF_LEN 512
#define PEEK_LEN 20

main(argc,argv)	/* execv a program */
int argc;
char *argv[];
{
        int     i,i_peek;
        char    peek_buff[PEEK_LEN];
        int     width,length,indent=-1;
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
                case 'i':
			indent = atoi(optarg);
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
            psfilter = popen("/usr/local/bin/psfilter", "w");
            fprintf(psfilter, "%*.*s", i_peek, i_peek, peek_buff);
            while ((i = read(0,buff,BUFF_LEN)) > 0) {
              fprintf(psfilter, "%*.*s", i, i, buff);
            }
            pclose(psfilter);
          }
        else
          {
            if (width > 132) { /* Small font + Landscape */
              if (indent < 0) indent = 0;
              i = sprintf(buff,"\033E\033&a%dL\033&k2G\033&l1o5.45c66F\033(s16.66H", indent);
            } else if (width > 96) { /* Small font + Portrait */
              if (indent < 0) indent = 1;
              i = sprintf(buff,"\033E\033&a%dL\033&k2G\033&l0o7.27c66F\033(s16.66H", indent);
            } else {   /* Standard font + Portrait */
              if (indent < 0) indent = 4;
              i = sprintf(buff,"\033E\033&a%dL\033&k2G\033&l0o7.27c66F\033(s12h4T", indent);
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
