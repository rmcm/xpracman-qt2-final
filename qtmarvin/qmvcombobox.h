/*
 *     $Id: qmvcombobox.h 2889 2006-06-11 11:47:27Z source $
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

#ifndef QMVCOMBOBOX_H
#define QMVCOMBOBOX_H

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
#include <qcombobox.h>
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

class QmvComboBox : public QComboBox
{
    Q_OBJECT
    
      public:
        // Constructor
    QmvComboBox( QmvTupleAttribute * tat, bool rw, QWidget * parent=0, const char * name=0 );
    QmvComboBox( QmvTupleAttribute * tat, QWidget * parent=0, const char * name=0 );
    ~QmvComboBox();  // Destructor

    void init();
    const QString attributeName() const;
    const QString attributeValue() const;
    const QmvClass * parentRelation() const;
    const QmvDB * dataBase() const;
    const QString currentValue() const;
    bool eventFilter( QObject *o, QEvent *e );
    
  public slots:
    void slotActivated( const QString & selection );
    void loadList();
    void loadList( const QStringList & list );
    void setCurrentLabel();
    void popupList();
    void queryList( const QString & input );
    void updateList( const QString & input );
    void returnPressed();

      protected:
    void keyPressEvent( QKeyEvent *e );
    void focusInEvent( QFocusEvent *e );
    void focusOutEvent( QFocusEvent *e );
    void resizeEvent( QResizeEvent *re );
    
      private:
    QGuardedPtr<QmvTupleAttribute> tupatt;
    QIntDict<QString> fk_values;
          
}; /* CLASS QmvComboBox */

#endif /* QMVCOMBOBOX_H */
// Local Variables:
// mode:C++
// End:
