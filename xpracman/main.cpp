/*
 *     $Id: main.cpp 2892 2006-06-11 12:16:11Z source $
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
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
extern "C"
{
#include <errno.h>
#include <signal.h>
}

#include <qapplication.h>
#include <qstring.h>
#include <qmessagebox.h>
#include "xpracman.h"
#include <qmv.h>

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

#ifndef QMVAPPLICATIONDIR
#define QMVAPPLICATIONDIR "/usr/local/share/xpracman"
#endif

/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */
QString XpracManDir;
/*
 * ======================================================================
 * MEMBER FUNCTIONS:
 * ======================================================================
 */


void messageHandler( QtMsgType type, const char *msg )
{
//      switch ( type )
//      {
//          case QtDebugMsg:
//                  //fprintf( stderr, "Debug: %s\n", msg );
//              break;
//          case QtWarningMsg:
//              fprintf( stderr, "Warning: %s\n", msg );
//              break;
//          case QtFatalMsg:
//              fprintf( stderr, "Fatal: %s\n", msg );
//              abort();                        // dump core on purpose
//    }
}



int main( int argc, char **argv )
{
    QApplication::setColorSpec( QApplication::CustomColor );
    QApplication * a = new QApplication( argc, argv );
    
    QString appname( "xpracman" );
    QString applabel( "XpracMan" );
    ::XpracManDir = getenv("PRAC_DIR");
    if ( !XpracManDir.length() )
        XpracManDir = QMVAPPLICATIONDIR;
    
    QString dbname = getenv( "PGDATABASE" );
    QString dbaccessname = getenv( "MARVINACCESS" );
    QString dbhost = getenv( "PGHOST" );
    QString dbport = getenv( "PGPORT" );
    
    int debug = 0;
    int wapp = 800;
    int happ = 600;
    bool maximize = false;
    
    for ( int i = 1; i < a->argc(); i++ )
    {
        if ( QString( a->argv()[i] ) == "--debug" )
        {
            debug = 1;
            continue;
        }
        if ( QString( a->argv()[i] ) == "--maximize" )
        {
            maximize = true;
            continue;
        }
        qDebug( "main: argument # %d <%s>", i, a->argv()[i] );
        if ( QString( a->argv()[i] ) == "--pracdir" )
        {
            ++i;
            XpracManDir = QString( a->argv()[i] );
            continue;
        }
        if ( QString( a->argv()[i] ) == "--database" )
        {
            ++i;
            dbname = QString( a->argv()[i] );
            continue;
        }
        if ( QString( a->argv()[i] ) == "--database-host" )
        {
            ++i;
            dbhost = QString( a->argv()[i] );
            continue;
        }
        if ( QString( a->argv()[i] ) == "--database-port" )
        {
            ++i;
            dbport = QString( a->argv()[i] );
            continue;
        }
        if ( QString( a->argv()[i] ) == "--accessdb" )
        {
            ++i;
            dbaccessname = QString( a->argv()[i] );
            continue;
        }
        if ( QString( a->argv()[i] ) == "--applabel" )
        {
            ++i;
            applabel = QString( a->argv()[i] );
            continue;
        }
        
        QMessageBox::critical( 0, QString( "%1: Illegal argument:fatal" ).arg( appname ),
                               QString( "An unknown argument was detected.\n\n %1\n\n"
                                        "This application will now exit." )
                               .arg( QString( a->argv()[i] ) ) );
        exit(1);
        
    }

        // Start a splash screen asap
    
    QProgressDialog splash( QString("Starting %1...").arg(applabel), "Abort", 100, 0, "progress", TRUE );
    splash.setProgress( 10 );
    qApp->processEvents();
    if ( splash.wasCancelled() )
        exit(1);
    
    if ( !debug )
        qInstallMsgHandler( messageHandler );

        // ------------------------------------------------------------
        // Settle on an access database name
        // ------------------------------------------------------------
    if ( dbaccessname.isNull() || dbaccessname.length() < 1 )
        if ( dbname.isNull() || dbname.length() < 1 )
            dbaccessname = QString("marvin_access");
        else
            dbaccessname = dbname;

        // ------------------------------------------------------------
        // Create a QmvAccess object
        // ------------------------------------------------------------
    splash.setProgress( 20 );
    splash.setLabelText("Checking access rights...");
    qApp->processEvents();
    if ( splash.wasCancelled() )
        exit(1);
    
    QmvAccess * dbaccess;
    try {
        dbaccess = new QmvAccess( QString("%1:%2:%3").arg(dbaccessname).arg(dbhost).arg(dbport), 0, "db-access" );
    }
    catch ( QmvException * exception )
    {
        splash.cancel();
        QMessageBox::Icon icon;
        switch ( exception->eStatus() )
        {
            case QmvException::Critical:
                icon = QMessageBox::Critical;
                break;
            case QmvException::Warning:
                icon = QMessageBox::Warning;
                break;
            case QmvException::Information:
            default:
                icon = QMessageBox::Information;
                break;
        }
        QMessageBox mbox( QString( "%1: Fatal Error - No Access Service available" ).arg( appname ),
                          exception->eMessage(),
                          icon,
                          QMessageBox::Ok,
                          QMessageBox::NoButton,
                          QMessageBox::NoButton);
        mbox.exec();
        delete exception;
        exit(1);
    }
    

        // ------------------------------------------------------------
        // Create a QmvApplication object
        // ------------------------------------------------------------
    splash.setProgress( 40 );
    splash.setLabelText("Connecting to database...");
    qApp->processEvents();
    if ( splash.wasCancelled() )
        exit(1);
    
    QString dbtitle;
    if ( dbname.isNull() || dbname.length() < 1 )
        if ( dbaccess->dbTitleList().count() > 0 )
                // take the first database in list
            dbtitle = *(dbaccess->dbTitleList().begin());
        else
        {
            splash.cancel();
            QMessageBox mbox( QString( "%1: Fatal Error - Database not found" ).arg( appname ),
                              QString( "Unable to determine the name of the database.\n\n"
                                       " You need to either set PGDATABASE,\\"
                                       " or use the command-line argument < --database NAME >"),
                              QMessageBox::Critical,
                              QMessageBox::Ok,
                              QMessageBox::NoButton,
                              QMessageBox::NoButton);
            mbox.exec();
            exit(1);
        }
    else
    {
        dbtitle = dbaccess->dbTitleByName( dbname );
        if ( dbtitle.isNull() || dbtitle.length() < 1 )
        {
            splash.cancel();
            QMessageBox mbox( QString( "%1: Fatal Error -  Database not accessible (title)" ).arg( appname ),
                              QString( "Unable to determine details of the database <b>\"%1\"</b>.<BR><BR>"
                                       " It may be that you do not have access to this database."
                                       " Check with your administrator.").arg( dbname),
                              QMessageBox::Critical,
                              QMessageBox::Ok,
                              QMessageBox::NoButton,
                              QMessageBox::NoButton);
            mbox.exec();
            exit(1);
        }
    }

        // ------------------------------------------------------------
        // manage a pid file
        // ------------------------------------------------------------

    QString homeappdir = QString( "%1/.%2" ).arg( getenv("HOME")).arg(appname);
    QDir dir_homeappdir( homeappdir ); 
    if ( !dir_homeappdir.exists() )
    {
        if ( !dir_homeappdir.mkdir( homeappdir ) )
        {
            splash.cancel();
            QMessageBox mbox( QString( "%1: Fatal Error - configuration" ).arg( appname ),
                              QString( "Could not create the directory:<br>%1").arg(homeappdir),
                              QMessageBox::Critical,
                              QMessageBox::Ok,
                              QMessageBox::NoButton,
                              QMessageBox::NoButton);
            mbox.exec();
            exit(1);
        }
    }
        // look for existing pid file.
    QString pidfile = QString("%1/%2-%3.pid")
        .arg(homeappdir).arg(dbname).arg( getenv("DISPLAY"));
    QFile f_pidfile( pidfile);
    if ( f_pidfile.open(IO_ReadOnly) )
    {  
        QTextStream t_pidfile( &f_pidfile);
        int pid = -1;
        t_pidfile >> pid;
        int k = kill(pid, 0);
        qDebug("pid=%d, kill=%d, errno = %d", pid, k, errno);
        
        if ( pid > 0 && !(kill(pid, 0) == -1 && errno == ESRCH) )
        {
            splash.cancel();
            QMessageBox mbox( QString( "%1: Fatal Error - startup" ).arg( appname ),
                              QString( "%1 is already running(pid=%2).").arg( appname ).arg(pid),
                              QMessageBox::Critical,
                              QMessageBox::Ok,
                              QMessageBox::NoButton,
                              QMessageBox::NoButton);
            mbox.exec();
            exit(1);
        }
        f_pidfile.close();
    }
    if ( !f_pidfile.open(IO_WriteOnly|IO_Truncate) )
    {  
        splash.cancel();
        QMessageBox mbox( QString( "%1: Fatal Error - startup" ).arg( appname ),
                          QString( "Cannot open PID for writing:<br>%1").arg(pidfile),
                          QMessageBox::Critical,
                          QMessageBox::Ok,
                          QMessageBox::NoButton,
                          QMessageBox::NoButton);
        mbox.exec();
        exit(1);
    } else {
        QTextStream t_pidfile( &f_pidfile);
        int pid = getpid();
        t_pidfile << pid << endl;
        f_pidfile.close();
    }
  
        // ------------------------------------------------------------
        // Build an application object
        // ------------------------------------------------------------
    splash.setProgress( 70 );
    splash.setLabelText("Building interface...");
    qApp->processEvents();
    if ( splash.wasCancelled() )
        exit(1);
    XpracMan * xpracman;
    try {
        xpracman = new XpracMan( dbtitle, dbaccess, 0, appname  );;
    }
    catch ( QmvException * exception )
    {
        QMessageBox::Icon icon;
        switch ( exception->eStatus() )
        {
            case QmvException::Critical:
                icon = QMessageBox::Critical;
                break;
            case QmvException::Warning:
                icon = QMessageBox::Warning;
                break;
            case QmvException::Information:
            default:
                icon = QMessageBox::Information;
                break;
        }
        splash.cancel();
        QMessageBox mbox( QString( "%1: Fatal Error - failed to build a database application" )
                          .arg( appname ),
                          exception->eMessage(),
                          icon,
                          QMessageBox::Ok,
                          QMessageBox::NoButton,
                          QMessageBox::NoButton);
        mbox.exec();
        delete exception;
        exit(1);
    }
    xpracman->setCaption( applabel );
    xpracman->setAppLabel( applabel );
    a->setMainWidget( xpracman );
    xpracman->setMaximumSize( QApplication::desktop()->width(), QApplication::desktop()->height() );
    xpracman->setMinimumSize( wapp, happ );
    if ( maximize )
        xpracman->resize( QApplication::desktop()->width(), QApplication::desktop()->height() );
    xpracman->updateGeometry();
    xpracman->show();
    a->connect( a, SIGNAL(lastWindowClosed()), a, SLOT(quit()) );

        // ready for main window.
    splash.cancel();
    
    int status;
    try
    {
        status = a->exec();
    }
    catch ( QmvException * exception )
    {
        QMessageBox::Icon icon;
        switch ( exception->eStatus() )
        {
            case QmvException::Critical:
                icon = QMessageBox::Critical;
                break;
            case QmvException::Warning:
                icon = QMessageBox::Warning;
                break;
            case QmvException::Information:
            default:
                icon = QMessageBox::Information;
                break;
        }
        QMessageBox mbox( QString( "%1: Error - This program exited unexpectedly" ).arg( appname ),
                          exception->eMessage(),
                          icon,
                          QMessageBox::Ok,
                          QMessageBox::NoButton,
                          QMessageBox::NoButton);
        mbox.exec();
        delete exception;
        QFile::remove(pidfile);
        exit(1);
    }
    
    QFile::remove(pidfile);
        // Look for Restart request
    if ( status == QmvApplication::ReStart )
    {
        delete a;
        extern char **environ;
        return execve( argv[0], argv, environ );
    }
    delete a;
    return status;
}
