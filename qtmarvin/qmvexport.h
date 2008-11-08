/*
 *     $Id: qmvexport.h 763 2001-08-27 10:39:15Z source $
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

#ifndef QMVEXPORT_H
#define QMVEXPORT_H

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
#include <qstring.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include "qmvclass.h"

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvExport : public QObject
{
    Q_OBJECT
    
      public:
    QmvExport( QmvClass * relation, char * name );    // Constructor
    ~QmvExport();  // Destructor

    bool writeXMLDeclaration( QTextStream & xml_ts );
    bool writeXMLData( QTextStream & xml_ts );
    bool writeKugarDataFile( const QString & filename );
    bool writeCsvFile( const QString & filename );
    bool writePalmDBFiles( const QString & basename );
    void setInstaller( const QString & installer );
    QString getInstaller( ) const;
    
      private:
    QmvClass * export_relation;
    QString install_command;          // External installation program and arguments
          
}; /* CLASS QmvExport */

#endif /* QMVEXPORT_H */
// Local Variables:
// mode:C++
// End:
