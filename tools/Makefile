#     $Id: Makefile 2920 2006-07-11 00:42:56Z source $
# 
#   Copyright 2005 X=X Computer Software Trust
#    		  Kangaroo Ground Australia 3097
# 
# 
#  This is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License published by
#  the Free Software Foundation; either version 2, or (at your option)
#  any later version.
#  
#  This software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this software; see the file COPYING.  If not, write to
#  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
#  
#  Report problems and direct all questions to:
#  
#      Rex McMaster, rmcm@compsoft.com.au
#  
# 
.PHONY	: all
.PHONY	: help

help:
		@echo "targets: all vnc backups dymoprint"

all:           	vnc backups

vnc:
		@echo "installing vncusers scripts"
		install --backup -D -c -m 0775 -o root -g wheel \
			vncusers-rc.d /etc/rc.d/vncusers
		if test -f "/etc/sysconfig/vncusers" ; then \
			echo "/etc/sysconfig/vncusers already exists, not copying"; \
		else \
			install --backup -D -c -m 0644 -o root -g wheel \
				vncusers-sysconfig /etc/sysconfig/vncusers; \
		fi
		install --backup -D -c -m 0755 -o root -g wheel \
			vncusers-sbin /usr/local/sbin/vncusers
		@echo "adding vncusers to runlevel 5"
		/sbin/insserv -d /etc/rc.d/vncusers
		@echo "You can add users to list in /etc/sysconfig/vncusers"

backups:
		@echo "installing backups scripts"
		install --backup -D -c -m 0775 -o root -g wheel \
			pg_admin.sh /usr/local/sbin/pg_admin.sh
		install --backup -D -c -m 0775 -o root -g wheel \
			xdump /usr/local/sbin/xdump
		@echo "You can activate backup scripts from cron"
		@echo " 48 21 * * * /usr/local/sbin/pg_admin.sh"
		@echo " 23 23 * * * /usr/local/sbin/xdump /home /export /"
dymoprint:
		@echo "Installing dymo print filters"
		install --backup -D -c -m 0775 -o root -g wheel \
			ps2dymo /usr/local/sbin/ps2dymo
		install --backup -D -c -m 0775 -o root -g wheel \
			txt2dymo /usr/local/sbin/txt2dymo
