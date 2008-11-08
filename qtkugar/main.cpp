/*
 *     $Id: main.cpp 2888 2006-06-11 08:02:04Z source $
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
#include <qapplication.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qfile.h>
#include "qtkugar.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */


/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */
QString QmvApplicationDir;
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
    QApplication a( argc, argv );
    
    QString appname( "QtKugar" );
    QString report_data;
    QString report_template;
    
    for ( int i = 1; i < a.argc(); i++ )
    {
        if ( QString( a.argv()[i] ) == "--nodebug" )
        {
            qInstallMsgHandler( messageHandler );
            continue;
        }
        qDebug( "main: argument # %d <%s>", i, a.argv()[i] );
        if ( QString( a.argv()[i] ) == "--data" )
        {
            ++i;
            report_data = QString( a.argv()[i] );
            continue;
        }
        if ( QString( a.argv()[i] ) == "--template" )
        {
            ++i;
            report_template = QString( a.argv()[i] );
            continue;
        }
        
        QMessageBox::critical( 0, QString( "%1: Illegal argument:fatal" ).arg( appname ),
                               QString( "An unknown argument was detected.\n\n %1\n\n"
                                        "This application will now exit." )
                               .arg( QString( a.argv()[i] ) ) );
        return(1);
        
    }

        // ------------------------------------------------------------
        // Check arguments
        // ------------------------------------------------------------
    if ( report_data.isNull() || report_data.length() < 1 )
    {
        QMessageBox::critical( 0, QString( "%1: Fatal Error - No data available" ).arg( appname ),
                          QString( "No Data specified") );
        return(1);
    }
    
    if ( report_template.isNull() || report_template.length() < 1 )
    {
        QMessageBox::critical( 0, QString( "%1: Fatal Error - No Template available" ).arg( appname ),
                          "No Template specified");
        return(1);
    }

    QtKugar * rptviewer = new QtKugar();
    
    rptviewer->setReportData( report_data );
    rptviewer->setReportTemplate( report_template );
    rptviewer->setCaption( "QtKugar" );
    a.setMainWidget( rptviewer );
    rptviewer->renderReport();
    rptviewer->adjustSize();
    rptviewer->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );

    a.exec();
    
}
