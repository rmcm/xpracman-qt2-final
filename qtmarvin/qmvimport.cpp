/*
 *     $Id: qmvimport.cpp 870 2001-10-07 04:44:14Z source $
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
#include "qmvimport.h"
#include <qimage.h>
#include <qurloperator.h> 
/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvimport_version[] = "$Id: qmvimport.cpp 870 2001-10-07 04:44:14Z source $";

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

QmvImport::QmvImport( QmvClass * reln, QWidget * parent, const char * name )
        : QWizard( parent, name, TRUE, 0 )
{
    list_box = new QListBox( this );
    setCaption( tr( "Import" ) );
        // ------------------------------------------------------------
        // Top level layout - add a groupbox, and manage it manually
        // ------------------------------------------------------------
    QHBoxLayout * hl_top = new QHBoxLayout( this, 1 );
    hl_top->addWidget( list_box );
    
    if ( reln )
        setRelation( reln );

    connect( list_box, SIGNAL( imported( int ) ),
             this, SLOT( imported( int ) ) );
    
    
}

// Destructor

QmvImport::~QmvImport()
{
  
}

void QmvImport::setRelation( QmvClass * reln )
{
    relation = reln;
    list_box->clear();
    for ( int i = 0; i < relation->count(); i++)
        list_box->insertItem( relation->attributeValue( i, relation->userLabel() ));


    QUrlOperator op("http://localhost:9999/axis-cgi/jpg/image.cgi?resolution=320x240");
    op.get();

    QImage image;
    QImageIO iio;
    QPixmap  pixmap;
    iio.setFileName( "import.jpeg" );
    if ( iio.read() )                 // ok
    {
        pixmap = iio.image();           // convert to pixmap
        list_box->insertItem( pixmap );
    }
    
    

}

void QmvImport::imported( int i )
{
    done( i );
}
