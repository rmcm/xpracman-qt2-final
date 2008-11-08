/*
 *     $Id: qmvform.h 2889 2006-06-11 11:47:27Z source $
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

#ifndef QMVFORMEDITFORM_H
#define QMVFORMEDITFORM_H

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

#include <qdialog.h>
#include <qstring.h>
#include <qlayout.h>
/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

    //--------------------------------------------------------------------------------
    // Internal class to handle editing of static form fields
    //--------------------------------------------------------------------------------

class QLabel;

class QmvFormEditFields : public QDialog
{
    Q_OBJECT

      public:

    QmvFormEditFields( QWidget * parent, const char * name );
    ~QmvFormEditFields();

    void addEditorRow( QLabel * label, QWidget * editor, int row );

      private slots:

      void editCompleted();
    bool eventFilter( QObject *o, QEvent *e );

      protected:

    void keyPressEvent( QKeyEvent *e );

      private:
        
    QGridLayout * grid_editrow ;
        
}; /* CLASS QmvFormEditFields */

#endif /* QMVFORMEDITFORM_H */


// Local Variables:
// mode:C++
// End:
