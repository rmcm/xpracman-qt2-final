/*
 *     $Id: qmvmultilineedit.h 1865 2003-09-14 22:27:45Z source $
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

#ifndef QMVMULTILINEEDIT_H
#define QMVMULTILINEEDIT_H

/*
 * ======================================================================
 * DESCRIPTION: Multiline editor widget
 *
 * ======================================================================
 */

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */

#include <qmultilineedit.h>
#include <qstring.h>
#include <qintdict.h>
#include <qguardedptr.h>
#include "qmvexception.h"
#include "qmvdb.h"
#include "qmvclass.h"

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvDB;
class QmvClass;
class QmvTupleAttribute;

class QmvMultiLineEdit : public QMultiLineEdit
{
    Q_OBJECT
    
      public:
    QmvMultiLineEdit(QmvTupleAttribute * tat,
                     QWidget * parent=0, const char * name=0);    // Constructor
    ~QmvMultiLineEdit();  // Destructor

    enum SaveMode {
        AlwaysSave,
        NeverSave,
        SaveIfChanged
    };
    
    public slots:
    void editCompleted( const QString & text );
    bool save();
    bool save( SaveMode mode );

      protected:
    QmvComboBox * selectList();
    void checkForeign();
    void keyPressEvent( QKeyEvent *e );
    void focusInEvent( QFocusEvent *e );
    void focusOutEvent( QFocusEvent *e );
    void resizeEvent( QResizeEvent *re );
    bool eventFilter( QObject *o, QEvent *e );

      private:
    QmvTupleAttribute * tupatt;
    QGuardedPtr<QmvComboBox> cb_list;
    bool cb_init;                               //<! Flag for combobox initialisation

}; /* CLASS QmvMultiLineEdit */

#endif /* QMVMULTILINEEDIT_H */
// Local Variables:
// mode:C++
// End:
