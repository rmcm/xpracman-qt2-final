/*
 *     $Id: qmvqueryconditionwidget.h 1877 2003-09-26 04:57:18Z source $
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

#ifndef QMVQUERYCONDITIONWIDGET_H
#define QMVQUERYCONDITIONWIDGET_H

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
#include <qlayout.h>
#include "qmvquerycondition.h"

class QmvSqlQuery;

/*
 * ======================================================================
 * CLASS DEFINITION QmvQueryConditionWidget
 * ======================================================================
 */

class QmvQueryConditionWidget : public QWidget
{
    Q_OBJECT

      public:
    QmvQueryConditionWidget( QmvQueryCondition * qry_condition,
                        QWidget * parent, const char * name = 0 );
    ~QmvQueryConditionWidget();
    bool eventFilter( QObject *o, QEvent *e );
    
  public slots:
    void atChanged( const QString & at_text );
    void update();

  private slots:
    void atActivated( int at_button );
    void atActivated( const QString & at_text );
    void opChanged( const QString & op_text );
    void opActivated( int op_button );
    void opActivated( const QString & op_text );
    void ptChanged( const QString & pt_text );
    void ptActivated( const QString & pt_text );

      signals:
    
      private:
    QmvSqlQuery * query_object;
    QmvQueryCondition * query_condition;
    QHBoxLayout * h_layout;
    QComboBox * cb_atlist;
    QComboBox * cb_oplist;
    QComboBox * cb_ptlist;
    QIntDict<QPushButton> comparator_buttons;
    QButtonGroup * bg_op;
    bool atValidate( const QString & at_text, bool trigger = FALSE );
    bool opValidate( const QString & op_text, bool trigger = FALSE );
    bool ptValidate( const QString & pt_text, bool trigger = FALSE );
    
}; /* CLASS QmvConditionWidget */



#endif /* QMVQUERYCONDITIONWIDGET_H */
// Local Variables:
// mode:C++
// End:
