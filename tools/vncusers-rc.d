#! /bin/sh
# Copyright (c) 1995-2003 SuSE Linux AG, Nuernberg, Germany.
# All rights reserved.
#
# Author: Kurt Garloff
# Please send feedback to http://www.suse.de/feedback/
#
# /etc/init.d/vncusers
#   and its symbolic link
# /(usr/)sbin/rcvncusers
#
# Template system startup script for some example service/daemon vncusers
#
# LSB compatible service control script; see http://www.linuxbase.org/spec/
# 
# Note: This template uses functions rc_XXX defined in /etc/rc.status on
# UnitedLinux (UL) based Linux distributions. If you want to base your 
# script on this template and ensure that it works on non UL based LSB 
# compliant Linux distributions, you either have to provide the rc.status
# functions from UL or change the script to work without them.
#
### BEGIN INIT INFO
# Provides:          vncusers
# Required-Start:    $syslog $remote_fs
# X-UnitedLinux-Should-Start: $time ypbind sendmail
# Required-Stop:     $syslog $remote_fs
# X-UnitedLinux-Should-Stop: $time ypbind sendmail
# Default-Start:     3 5
# Default-Stop:      0 1 2 6
# Short-Description: vncusers startup
# Description:       Start vncusers to allow vnc servers for specified users
### END INIT INFO
# 
# Any extensions to the keywords given above should be preceeded by 
# X-VendorTag- (X-UnitedLinux- for us) according to LSB.
# 
# Notes on Required-Start/X-UnitedLinux-Should-Start:
# * There are two different issues that are solved by Required-Start
#    and X-UnitedLinux-Should-Start
# (a) Hard dependencies: This is used by the runlevel editor to determine
#     which services absolutely need to be started to make the start of
#     this service make sense. Example: nfsserver should have
#     Required-Start: $portmap
#     Also, required services are started before the dependent ones.
#     The runlevel editor will warn about such missing hard dependencies
#     and suggest enabling. During system startup, you may expect an error,
#     if the dependency is not fulfilled.
# (b) Specifying the init script ordering, not real (hard) dependencies.
#     This is needed by insserv to determine which service should be
#     started first (and at a later stage what services can be started
#     in parallel). The tag X-UnitedLinux-Should-Start: is used for this.
#     It tells, that if a service is available, it should be started
#     before. If not, never mind.
# * When specifying hard dependencies or ordering requirements, you can 
#   use names of services (contents of their Provides: section)
#   or pseudo names starting with a $. The following ones are available
#   according to LSB (1.1):
#	$local_fs		all local file systems are mounted
#				(most services should need this!)
#	$remote_fs		all remote file systems are mounted
#				(note that /usr may be remote, so
#				 many services should Require this!)
#	$syslog			system logging facility up
#	$network		low level networking (eth card, ...)
#	$named			hostname resolution available
#	$netdaemons		all network daemons are running
#   The $netdaemons pseudo service has been removed in LSB 1.2.
#   For now, we still offer it for backward compatibility.
#   These are new (LSB 1.2):
#	$time			the system time has been set correctly	
#	$portmap		SunRPC portmapping service available
#   UnitedLinux extensions:
#	$ALL			indicates that a script should be inserted
#				at the end
# * The services specified in the stop tags 
#   (Required-Stop/X-UnitedLinux-Should-Stop)
#   specify which services need to be still running when this service
#   is shut down. Often the entries there are just copies or a subset 
#   from the respective start tag.
# * X-UnitedLinux-Should-Start/Stop are not part of LSB (as of 1.3)
#   but official Should-Start/Stop tags are in discussion (1.9).
#   insserv does support these as well.
# * X-UnitedLinux-Default-Enabled: yes/no is used at installation time
#   (%fillup_and_insserv macro in %post of many RPMs) to specify whether
#   a startup script should default to be enabled after installation.
#   It's not used by insserv.
#
# Note on runlevels:
# 0 - halt/poweroff 			6 - reboot
# 1 - single user			2 - multiuser without network exported
# 3 - multiuser w/ network (text mode)  5 - multiuser w/ network and X11 (xdm)
# 
# Note on script names:
# http://www.linuxbase.org/spec/refspecs/LSB_1.3.0/gLSB/gLSB/scrptnames.html
# A registry has been set up to manage the init script namespace.
# http://www.lanana.org/
# Please use the names already registered or register one or use a
# vendor prefix.


# Check for missing binaries (stale symlinks should not happen)

VNCUSERS_BIN=/usr/local/sbin/vncusers
test -x $VNCUSERS_BIN || {
    echo "$VNCUSERS_BIN not installed"; exit 5
}
# Source SuSE config

## Set defaults
export VNCUSERS_LIST=guest
export VNCUSERS_OFFSET=300
export VNCUSERS_GEOMETRY="-geometry 1016x700"
export VNCUSERS_DEPTH="-depth 24"

## Load configuration file
VNCUSERS_SYSCONFIG=/etc/sysconfig/vncusers
test -f $VNCUSERS_SYSCONFIG && . $VNCUSERS_SYSCONFIG

## Get the requested action
VNCACTION=$1
shift 1

## Use supplied user list if present
if test $# -gt 0 ; then
    VNCUSERS_LIST=$*
fi

# Source LSB init functions
# providing start_daemon, killproc, pidofproc, 
# log_success_msg, log_failure_msg and log_warning_msg.
# This is currently not used by UnitedLinux based distributions and
# not needed for init scripts for UnitedLinux only. If it is used,
# the functions from rc.status should not be sourced or used.
#. /lib/lsb/init-functions

# Shell functions sourced from /etc/rc.status:
#      rc_check         check and set local and overall rc status
#      rc_status        check and set local and overall rc status
#      rc_status -v     be verbose in local rc status and clear it afterwards
#      rc_status -v -r  ditto and clear both the local and overall rc status
#      rc_status -s     display "skipped" and exit with status 3
#      rc_status -u     display "unused" and exit with status 3
#      rc_failed        set local and overall rc status to failed
#      rc_failed <num>  set local and overall rc status to <num>
#      rc_reset         clear both the local and overall rc status
#      rc_exit          exit appropriate to overall rc status
#      rc_active        checks whether a service is activated by symlinks
#      rc_splash arg    sets the boot splash screen to arg (if active)
. /etc/rc.status

# Reset status of this service
rc_reset

# Return values acc. to LSB for all commands but status:
# 0	  - success
# 1       - generic or unspecified error
# 2       - invalid or excess argument(s)
# 3       - unimplemented feature (e.g. "reload")
# 4       - user had insufficient privileges
# 5       - program is not installed
# 6       - program is not configured
# 7       - program is not running
# 8--199  - reserved (8--99 LSB, 100--149 distrib, 150--199 appl)
# 
# Note that starting an already running service, stopping
# or restarting a not-running service as well as the restart
# with force-reload (in case signaling is not supported) are
# considered a success.

case "$VNCACTION" in
    start)
	echo -n "Starting vncusers "
	## Start daemon with startproc(8). If this fails
	## the return value is set appropriately by startproc.
	$VNCUSERS_BIN start $VNCUSERS_LIST

	# Remember status and be verbose
	rc_status -v
	;;
    stop*)
	echo -n "Shutting down vncusers "
	## Stop daemon with killproc(8) and if this fails
	## killproc sets the return value according to LSB.

	$VNCUSERS_BIN ${VNCACTION} $VNCUSERS_LIST

	# Remember status and be verbose
	rc_status -v
	;;
    restart*)
	## Stop the service and regardless of whether it was
	## running or not, start it again.
	$VNCUSERS_BIN ${VNCACTION} $VNCUSERS_LIST

	# Remember status and be quiet
	rc_status
	;;
    *)
	echo "Usage: $0 {start|stop|stop-clean|restart|restart-clean}"
	exit 1
	;;
esac
rc_exit
