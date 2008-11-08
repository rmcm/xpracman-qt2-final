/*
 *     $Id: qmvaccess.cpp 2889 2006-06-11 11:47:27Z source $
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
 * DESCRIPTION: Access Object
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <unistd.h>
#include <pwd.h>
#include <qregexp.h>
#include "sql.h"
#include "qmvaccess.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvaccess_version[] = "$Id: qmvaccess.cpp 2889 2006-06-11 11:47:27Z source $";

/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */

/*
 * ======================================================================
 * MEMBER FUNCTIONS:
 * ======================================================================
 */

// Constructor

QmvAccess::QmvAccess(const QString & accdbname, QObject *parent, const char *name )
{
    struct passwd * pwent = getpwuid( getuid() );
    
    logname = QString( pwent->pw_name );
    QStringList db_parts = QStringList::split( QRegExp( ":" ), accdbname );
    dbname = "";
    dbhost = "";
    dbport = "";
    if ( db_parts.count() > 0 )
        dbname = db_parts[0];
    if ( db_parts.count() > 1 )
        dbhost = db_parts[1];
    if ( db_parts.count() > 2 )
        dbport = db_parts[2];
    
    accessid = 0;
    dblist.clear();
    acclist.clear();
    
    int constat = r_sql_dbconnect( dbhost.latin1(), dbport.latin1(), "", "", dbname.latin1(), logname.latin1(), "" );
    if (constat >= 0)
    {   // successful connection

            // ------------------------------------------------------------
            // Get the user access information
            // ------------------------------------------------------------
        int sql_result_index;
        QString query = QString( "select * from mvac_user where username = '%1'").arg(logname); 
        int user_count = r_sql_exec( constat, &sql_result_index, query );
        if ( user_count == 1 )
        {  // found a user record
            accessid= QString( r_sql_value( &sql_result_index, 0, "perms") ).toInt();

            query = QString( "select * from mvac_database;" );
                
            int sql_result_index;  // masks user index
            int db_count = r_sql_exec( constat, &sql_result_index, query );
            for ( int db = 0; db < db_count; db++ )
            {
                qDebug("QmvAccess::QmvAccess:db=%s(%d) user=%d",
                       r_sql_value( &sql_result_index, db, "db_desc"),
                       QString( r_sql_value( &sql_result_index, db, "perms" ) ).toInt(),
                       accessid);
                
                    // is this db accessible?
                if ( !( QString( r_sql_value( &sql_result_index, db, "perms" ) ).toInt()
                        & accessid ) )
                    continue;
                    
                QString * db_desc = new QString(r_sql_value( &sql_result_index, db, "db_desc"));
                QString * db_name = new QString(r_sql_value( &sql_result_index, db, "db_name"));
                QString * db_host = new QString(r_sql_value( &sql_result_index, db, "db_host"));
                if ( db_host->length() == 0 )
                    *db_host = dbhost;
                QString * db_port = new QString(r_sql_value( &sql_result_index, db, "db_port"));
                if ( db_port->length() == 0 )
                    *db_port = dbport;
                QString * db_options = new QString(r_sql_value( &sql_result_index, db, "db_options"));
                QString * db_tty = new QString(r_sql_value( &sql_result_index, db, "db_tty"));
                QString * db_login = new QString(r_sql_value( &sql_result_index, db, "db_login"));
                QString * db_pwd = new QString(r_sql_value( &sql_result_index, db, "db_pwd"));

                    // list of titles
                dblist += *db_desc;
                    // index of titles by name
                db_title_index.insert( *db_name, db_desc );
                QDict<QString> * db_connection =  new QDict<QString>();
                    
                db_connection->insert( "name", db_name );
                db_connection->insert( "host", db_host );
                db_connection->insert( "port", db_port );
                db_connection->insert( "options", db_options );
                db_connection->insert( "tty", db_tty );
                db_connection->insert( "login", db_login );
                db_connection->insert( "pwd", db_pwd );

                    // Index of connections by title
                db_connection_index.insert( *db_desc, db_connection );
                    
            }
            r_sql_freeresult( &sql_result_index );
            dblist.sort();
            r_sql_freeresult(&sql_result_index);
        } else  {
            r_sql_freeresult(&sql_result_index);
            r_sql_dbclose(constat);
            QmvException * exception
                = new QmvException( QString( "<h2>Failed to locate User Credentials</h2>"
                                             "<ul><li>Database name: <b>%1</b>"
                                             "<li>Host name: <b>%2</b>"
                                             "<li>Port: <b>%3</b>"
                                             "<li>User name: <b>%4</b></ul>" )
                                    .arg(dbname)
                                    .arg(dbhost)
                                    .arg(dbport)
                                    .arg(logname),
                                    QmvException::Critical );
            throw( exception );
        } // end of user block

            // ------------------------------------------------------------
            // Get the access labels
            // ------------------------------------------------------------
        query = QString( "select * from mvac_label");
        int label_count = r_sql_exec( constat, &sql_result_index, query );
        for ( int i = 0; i < label_count; i++ )
            acc_label.insert( QString(r_sql_value( &sql_result_index, i, "perms")).toInt(),
                              new QString(r_sql_value( &sql_result_index, i, "label")) );
        r_sql_freeresult(&sql_result_index);


    } else {
        QmvException * exception
            = new QmvException(QString( "<h2>Failed to locate the Access Database:</h2>"
                                        "<ul><li>Database name: <b>%1</b>"
                                        "<li>Host name: <b>%2</b>"
                                        "<li>Port: <b>%3</b>"
                                        "<li>User name: <b>%4</b></ul>" )
                               .arg(dbname)
                               .arg(dbhost)
                               .arg(dbport)
                               .arg(logname),
                               QmvException::Critical );
        
        throw( exception );
    } // end of connection block
    r_sql_dbclose(constat);
    
}
            

// Destructor

QmvAccess::~QmvAccess()
{
    QDictIterator< QDict<QString> > it( db_connection_index );
    while ( it.current() )
    {
        it.current()->setAutoDelete( TRUE );
        it.current()->clear();
        ++it;
    }
    
    db_connection_index.setAutoDelete( TRUE );
    db_connection_index.clear();
    db_title_index.setAutoDelete( TRUE );
    db_title_index.clear();
    acc_label.setAutoDelete( TRUE );
    acc_label.clear();
}

const int QmvAccess::accessID() const
{
    return accessid;
}

const QStringList QmvAccess::dbTitleList() const
{
    return dblist;
}

const QStringList QmvAccess::accessList() const
{
    return acclist;
}

const QString QmvAccess::dbAttributeByTitle( const QString & title, const QString & attr ) const
{
    qDebug("QmvAccess::dbAttributeByTitle title=%s attr=%s", title.latin1(), attr.latin1() );

//          // Debugging
//      QDictIterator< QDict<QString> > it( db_connection_index );
//      while ( it.current() )
//      {
//          qDebug("QmvAccess::dbAttributeByTitle:db_connection_index = %s", it.currentKey().latin1() );
//          QDictIterator<QString> it2( *(it) );
//          while ( it2.current() )
//          {
//              qDebug("QmvAccess::dbAttributeByTitle:db_connection att=%s val=%s",
//                     it2.currentKey().latin1(),
//                     it2.current()->latin1() );
//              ++it2;
//          }
//          ++it;
//      }
//          // End debugging
    
    QDict<QString> * ptr = db_connection_index.find(title);
    if (!ptr)
        return QString();
   
    QString * ptr2 = ptr->find( attr );
    if (! ptr2 )
        return QString();

    qDebug("QmvAccess::dbAttributeByTitle FOUND=%s", ptr2->latin1() );
    return( *ptr2 );
}

const QString QmvAccess::dbNameByTitle( const QString & title ) const
{
    return dbAttributeByTitle( title, "name" );
}

const QString QmvAccess::dbHostByTitle( const QString & title ) const
{
     return dbAttributeByTitle( title, "host" );
}

const QString QmvAccess::dbPortByTitle( const QString & title ) const
{
     return dbAttributeByTitle( title, "port" );
}

const QString QmvAccess::dbOptionsByTitle( const QString & title ) const
{
     return dbAttributeByTitle( title, "options" );
}

const QString QmvAccess::dbTtyByTitle( const QString & title ) const
{
     return dbAttributeByTitle( title, "tty" );
}

const QString QmvAccess::dbLoginByTitle( const QString & title ) const
{
     return dbAttributeByTitle( title, "login" );
}

const QString QmvAccess::dbPwdByTitle( const QString & title ) const
{
     return dbAttributeByTitle( title, "pwd" );
}

const QString QmvAccess::dbTitleByName( const QString & name ) const
{
    QString * ptr = db_title_index.find( name );
    if (ptr)
        return *ptr;
    else
        return QString::null;
}

const QString QmvAccess::dbName() const
{
    return dbname;
}



