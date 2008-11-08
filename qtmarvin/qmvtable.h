/*
 *     $Id: qmvtable.h 3020 2006-09-05 02:56:16Z source $
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

#ifndef QMVTABLE_H
#define QMVTABLE_H

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
#include <qobject.h>
#include <qtable.h>
#include <qstring.h>
#include <qwidget.h>
#include <qpixmap.h>
#include <qlineedit.h>
#include "qmvcombobox.h"
#include "qmultilineedit.h"

/*
 * ======================================================================
 * CLASS DEFINITION : QmvTable
 * ======================================================================
 */

class QmvTableToolTip;
class QmvTable : public QTable
{
    Q_OBJECT
    
      public:
        // Constructor
    QmvTable( QWidget * parent = 0, const char * name = 0 );
    QmvTable( int numRows, int numCols, QWidget * parent = 0, const char * name = 0 );
    ~QmvTable();  // Destructor
    void redisplayRow( int row, int col );
    void paintCell ( QPainter * p, int row, int col, const QRect & cr, bool selected );
    void paintFocus ( QPainter * p, const QRect & cr );
    bool saveMode() const;
    void setSaveMode( bool mode = true );
    
  public slots:
void editCompleted();
    void editCompleted( int row, int col );
    void slotSetToolTips( bool tooltips );
    void updateCornerWidget();

  private slots:
    void cellClicked( int row, int col, int button, const QPoint & mousePos );
    void realSingleClick();

      signals:
    void reloadRequest( int row, int col );
    void doubleClicked( int row, int col, int button, const QPoint & mousePos );
    void clicked( int row, int col, int button, const QPoint & mousePos );
    void statusMessage( const  QString & msg );
    
      protected:
    bool eventFilter( QObject *o, QEvent *e );
    void keyPressEvent( QKeyEvent *ke );
    void focusOutEvent( QFocusEvent *e );
    void focusInEvent( QFocusEvent *e );
    void contentsMousePressEvent( QMouseEvent * mre );
    void contentsMouseReleaseEvent( QMouseEvent * mre );
    void contentsMouseDoubleClickEvent( QMouseEvent * mpe );
    void activateNextCell();
    void setCellContentFromEditor( int row, int col);

      private:
    void init();
    bool init_flag;
    bool save_mode;
        // Variables for distinguishing single/double click
    bool is_current_cell;
    bool first_click;
    QVBox * corner_widget;
    QTimer *dc_timer;
    struct hold_mre 
    {
        QEvent::Type type;
        QPoint pos;
        QPoint globalPos;
        Qt::ButtonState button;
        Qt::ButtonState state;
    } hold_mre;

    QToolButton * tt_button;
    bool tooltips_flag;
    const QString getDetails( const int row, const int col, bool fk = false ) const;
    friend class QmvTableToolTip;
    friend class QmvTableWhatsThis;
          
}; /* CLASS QmvTable */

/*
 * ======================================================================
 * CLASS DEFINITION : QmvTableItem
 * ======================================================================
 */

class QmvTupleAttribute;

class QmvTableItem : public QTableItem
{
    
      public:
    enum EditorType { NoEdit, LineEdit, ComboBox, MultiLineEdit, ColorDialog, DateTime, CopyText };
        // Constructor
    QmvTableItem( QmvTable * table, EditType et, QmvTupleAttribute * tat, const QPixmap & p = QPixmap() );
    ~QmvTableItem();  // Destructor
    QWidget * createEditor() const;
    void setContentFromEditor ( QWidget * w );
    bool itemHasChanged( QmvTupleAttribute * newatt );
    bool refreshText();
    void paint( QPainter * p, const QColorGroup & cg,
                const QRect & cr, bool selected );
    void setColor() const;
    void setTexTCopy(const QString & key );
    QString setDateTime( const QDateTime & dt );
    QmvTupleAttribute * getTupleAttribute() const;
    
      private:
    QmvTable * parent_table;
    QGuardedPtr<QmvTupleAttribute> tupatt;
    EditorType editor_type;
    bool edit_prompting;
    uint dt_flags;
          
}; /* CLASS QmvTableItem */

#endif /* QMVTABLE_H */
// Local Variables:
// mode:C++
// End:
