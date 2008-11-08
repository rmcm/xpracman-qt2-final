/*
 *     $Id: qmvimport.h 699 2001-08-05 05:53:47Z source $
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

#ifndef QMVIMPORT_H
#define QMVIMPORT_H

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

#include <qwidget.h>
#include <qwizard.h>
#include <qlistbox.h>
#include <qstring.h>
#include "qmvclass.h"

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvClass;

class QmvImport :  public QWizard
{
    Q_OBJECT
    
      public:
    QmvImport( QmvClass * reln = 0, QWidget * parent = 0, const char * name = 0 );    // Constructor
    ~QmvImport();  // Destructor
    void setRelation( QmvClass * reln );

  public slots:
    void imported( int i );
    
      private:
    QListBox * list_box;
    QmvClass * relation;
          
}; /* CLASS QmvImport */

#endif /* QMVIMPORT_H */
// Local Variables:
// mode:C++
// End:
