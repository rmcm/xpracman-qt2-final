/*
 *     $Id: qmvexport.cpp 1508 2003-01-29 01:55:31Z source $
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
#include <stdlib.h>
#include "qmvexport.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvexport_version[] = "$Id: qmvexport.cpp 1508 2003-01-29 01:55:31Z source $";

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

QmvExport::QmvExport( QmvClass * relation, char * name )
        : QObject( relation, name )
{
    export_relation = relation;
    
}

// Destructor

QmvExport::~QmvExport()
{
  
}

bool QmvExport::writeXMLDeclaration( QTextStream & xml_ts )
{
    export_relation->writeXMLDeclaration( xml_ts );
    return true;
}

bool QmvExport::writeXMLData( QTextStream & xml_ts )
{
    export_relation->writeXMLData( xml_ts );
    return true;
}

bool QmvExport::writeKugarDataFile( const QString & filename )
{
    QFile f( filename );
    if ( !f.open( IO_WriteOnly ) )
    {
        QMessageBox::critical( 0, QString( "Fatal Error - File open" ),
                               QString( tr("Cannot open <%1> for writing") ).arg(filename) );
        return(FALSE);
    }
    QTextStream t( &f );
    QmvAttList atlist = export_relation->sortedAttlist( "mtat_browse_order",
                                                        QmvClass::NoBlank|QmvClass::NoSystem );


        // Header
    t << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl << endl;
    t << "<!DOCTYPE KugarData [" << endl;
    
    t << "  <!ELEMENT KugarData (Row* )>" << endl;
    t << "  <!ATTLIST KugarData" << endl;
    t << "    Template CDATA #REQUIRED>" << endl;
    t << endl;
    t << "  <!ELEMENT Row EMPTY>" << endl;
    t << "  <!ATTLIST Row " << endl;
    for ( QmvAttribute * at_ptr = atlist.first(); at_ptr != 0; at_ptr = atlist.next() )
        t << "    " << *at_ptr->find( "mtat_name" ) << "  CDATA #REQUIRED" << endl;
    t << "]>" << endl << endl;

        // Data
    QString std_template = QString("%1.kut").arg( export_relation->relationName() );
    t << "<KugarData Template=\"" << std_template << "\">" << endl;

    for( int row = 0 ; row < export_relation->count() ; row++ )
    {
        t << "  <row ";
        for ( QmvAttribute * at_ptr = atlist.first(); at_ptr != 0; at_ptr = atlist.next() )
        {
            t << *at_ptr->find( "mtat_name" ) << "=";
            t << "\"" << export_relation->attributeValue( row, *at_ptr->find( "mtat_name" ) ) << "\" ";
        }
        
        t << ">" << endl;
    }
    t << "</KugarData>" << endl;
    f.close();
    return(TRUE);
}
bool QmvExport::writeCsvFile( const QString & filename )
{

    return true;  
}
//! Export to Palm-DB format CVS/INFO files.
/*!
  Prepare info and cvs files from current dataset. These files can be used by the
  palm-db-tool \em cvs2pdb to create a PDB database file.

  \param basename       the basename for the export files ( .info and .cvs are appended)
  \return       TRUE/FALSE to indicate success.
*/
// --------------------------------------------------------------------------------
bool QmvExport::writePalmDBFiles( const QString & basename )
{
        
        //------------------------------------------------------------
        // the metadata file
        //------------------------------------------------------------

        // setup a datastream to the export file
    QString file_metadata = basename;
    file_metadata.append(".info");
    QFile fmeta( file_metadata );
    if ( !fmeta.open( IO_WriteOnly ) )
    {
        QMessageBox::critical( 0, QString( "Fatal Error - File open" ),
                               QString( tr("Cannot open <%1> for writing") ).arg( file_metadata ) );
        return(FALSE);
    }
    QTextStream tmeta( &fmeta );

        // get a list of attributes to export
    QmvAttList atlist = export_relation->sortedAttlist( "mtat_browse_order",
                                                        QmvClass::NoBlank|QmvClass::NoSystem );

        // start to build the meta file.
    tmeta << "title " << "\"" << export_relation->relationTitle() << "\"" << endl;

        // iterate over the attribute list to build the field specifications;
        // - skipping attributes without name or title (????)
        // - build a list of "acceptable" attributes - "fields"
        // - build a db "view" list = "viewall"
        // - build a list of individual view lists = "viewlist"
    
    QStringList fields;
    QString viewall;
    QStringList viewlists;
    QString rowlabelfld =  export_relation->userLabel();
    const QmvAttribute * lab_ptr = export_relation->attribute( export_relation->userLabel() );
    
    for ( QmvAttribute * at_ptr = atlist.first(); at_ptr != 0; at_ptr = atlist.next() )
    {
        QString * fieldptr = at_ptr->find( "mtat_title" );
        if ( !fieldptr )
            fieldptr = at_ptr->find( "mtat_name" );
        if ( !fieldptr )
            continue;
        fields += at_ptr->attributeName();
        viewall.append( QString( " \"%1\" 80" ).arg( at_ptr->attributeTitle() ) );
            // ad a view if this attribute is different from the class label attribute
        if ( at_ptr != lab_ptr )
            viewlists += QString(" \"%1\" \"%2\" 80 \"%3\" 80")
                .arg( at_ptr->attributeTitle() )
                .arg( lab_ptr->attributeTitle() )
                .arg( at_ptr->attributeTitle() );
        
        QString datatype;
        switch ( at_ptr->dataType() )
        { //TypeUnknown, Text, Integer, Numeric, Timestamp, Boolean
            case QmvAttribute::Integer:
                datatype = "integer";
                break;
            case QmvAttribute::Boolean:
                datatype = "boolean";
                break;
            case QmvAttribute::Timestamp:
                    //case QmvAttribute::Date:
                datatype = "date";
                break;
                    //case QmvAttribute::Time:
                    //datatype = "time";
                    //break;
            case QmvAttribute::TypeUnknown:
            case QmvAttribute::Text:
            case QmvAttribute::Numeric:
            default:
                datatype = "string";
                break;
                
        }
        QString fstr = QString( "field \"%1\" %2" )
            .arg( at_ptr->attributeTitle() )
            .arg( datatype );        
        tmeta << fstr << endl;
    }
    
    tmeta << "view \"All Viewlists\" " << viewall << endl;
    QStringList::Iterator it = viewlists.begin();
    while ( it != viewlists.end() )
    {
        tmeta << "view "
              << *it
              << endl;
        ++it;
    }
    
    tmeta << "option backup false" << endl;
    tmeta << "extended off" << endl;
    fmeta.close();
    

        //------------------------------------------------------------
        // the data file
        //------------------------------------------------------------
    QString file_data = basename;
    file_data.append(".csv");
    QFile fdata( file_data );
    if ( !fdata.open( IO_WriteOnly ) )
    {
        QMessageBox::critical( 0, QString( "Fatal Error - File open" ),
                               QString( tr("Cannot open <%1> for writing") ).arg( file_data ) );
        return(FALSE);
    }
    QTextStream tdata( &fdata );

    for( int row = 0 ; row < export_relation->count() ; row++ )
    {
        QStringList::Iterator it = fields.begin();
        while ( it != fields.end() )
        {
            tdata << "\"" << export_relation->attributeValue( row, *it ) << "\"";
            ++it;
            if ( it != fields.end() )
                tdata << ",";
        }
        tdata << endl;
    }
    
    
    fdata.close();

    QString installer = getInstaller();
    installer.replace( QRegExp("%f"), basename );
    QString status_header = QString( "%1 to Palm DB" ).arg( export_relation->relationTitle() );
    
    int status = system( installer );
    if ( status )
    {
    
        QMessageBox mbox( status_header,
                          QString( "Failed to export %1 records using;<br><br>"
                                   " <em>%2</em>" )
                          .arg( export_relation->count() )
                          .arg( installer ),
                          QMessageBox::Warning,
                          QMessageBox::Ok,
                          QMessageBox::NoButton,
                          QMessageBox::NoButton);
        mbox.exec();
        return(FALSE);
    }
    
    QMessageBox mbox( status_header,
                      QString( "%1 records have been exported using;<br><br>"
                               " <em>%2</em>")
                      .arg( export_relation->count() )
                      .arg( installer ),
                      QMessageBox::Information,
                      QMessageBox::Ok,
                      QMessageBox::NoButton,
                      QMessageBox::NoButton);
    mbox.exec();
        
    return(TRUE);
    
}

// --------------------------------------------------------------------------------
void QmvExport::setInstaller( const QString & installer )
{
    install_command = installer;
}



// --------------------------------------------------------------------------------
QString QmvExport::getInstaller( ) const
{
    return install_command;
    
}

