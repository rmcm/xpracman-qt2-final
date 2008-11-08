/*
 *     $Id: qmvaccess.h 1491 2003-01-08 03:40:27Z source $
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

#ifndef QMVACCESS_H
#define QMVACCESS_H

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
#include <qstring.h>
#include <qwidget.h>
#include <qstringlist.h>
#include <qdict.h>
#include <qintdict.h>
#include "qmvexception.h"
/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvAccess : public QObject
{
    Q_OBJECT
      public:
    enum AccessLevels { Guest = 1, User = 2, SuperVisor = 4, Administrator = 31 };
        // Constructor
    QmvAccess(const QString & accdbname, QObject *parent = 0, const char *name = 0 );
    ~QmvAccess();  // Destructor
    const int accessID() const;
    const QStringList dbTitleList() const;
    const QStringList accessList() const;
    const QString dbAttributeByTitle( const QString & title, const QString & attr ) const;
    const QString dbNameByTitle( const QString & title ) const;
    const QString dbHostByTitle( const QString & title ) const;
    const QString dbPortByTitle( const QString & title ) const;
    const QString dbOptionsByTitle( const QString & title ) const;
    const QString dbTtyByTitle( const QString & title ) const;
    const QString dbLoginByTitle( const QString & title ) const;
    const QString dbPwdByTitle( const QString & title ) const;
    const QString dbTitleByName( const QString & name ) const;
    const QString dbName() const;
    
      private:
    QString logname;
    QString dbname;
    QString dbhost;
    QString dbport;
    
    int accessid;
    QStringList dblist;
    QDict< QDict<QString> > db_connection_index;         //<! Index of connection dictionaries
    QDict<QString> db_title_index;
    QStringList acclist;
    QIntDict<QString> acc_label;
          
}; /* CLASS QmvAccess */

#endif /* QMVACCESS_H */
// Local Variables:
// mode:C++
// End:
