/*
 *     $Id: qmvroweditor.h 1569 2003-02-23 12:01:06Z source $
 * 
 *   Copyright 2002 X=X Computer Software Trust
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

#ifndef QMVROWEDITOR_H
#define QMVROWEDITOR_H

/*
 * ======================================================================
 * DESCRIPTION: A simple row browser/editor
 *
 * ======================================================================
 */

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */

#include <qdialog.h>
#include <qtoolbutton.h>
#include <qpushbutton.h>
#include <qsignalmapper.h>
#include <qhbox.h>
#include "qmvclass.h"

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvRowEditor :  public QDialog
{
    Q_OBJECT
    
      public:
    QmvRowEditor(QmvSqlClass * parent_rel, bool save = true,
                 QWidget * parent = 0, const char * name = 0 );    // Constructor
    ~QmvRowEditor();  // Destructor
    void setAttList( const QmvAttList & al );
    void initTable();
    void setReturnButton( QString label, int index, QString text );
    bool saveMode() const;
    void setSaveMode( bool save = true );
    bool load( int tuple_row = 0 );

    public slots:
    void handleReturn( int index );
    void slotRefreshSet();
    void slotNextRow();
    void slotPreviousRow();

      private:
    QmvSqlClass * relation;
    bool save_mode;
    QmvTable * table_editrow;
    QmvAttList atlist;
    QHBox * return_buttons;
    QSignalMapper * return_signals;
    int current_row;
    
          
}; /* CLASS QmvRowEditor */

#endif /* QMVROWEDITOR_H */
// Local Variables:
// mode:C++
// End:
