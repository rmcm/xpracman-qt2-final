/*
 *     $Id: qmvquerywidget.h 3211 2006-12-04 00:53:56Z source $
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

#ifndef QMVQUERYWIDGET_H
#define QMVQUERYWIDGET_H

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

#include <qframe.h>
#include <qlayout.h>
#include "qmvqueryconditionwidget.h"

class QmvIcons;
class QmvQueryConditionWidget;

/*
 * ======================================================================
 * CLASS DEFINITION - QmvQueryWidget
 * ======================================================================
 */


//! A standardised, single-condition query Widget derived from QmvSqlQuery
/*!
 * Using the attributes;
  
  \arg A conjunctive term
  \arg A comparison operator
  \arg A comparison value

  
*/

class QmvQueryWidget : public QFrame
{
    Q_OBJECT

      public:
    QmvQueryWidget( QmvSqlQuery * qryobj, QWidget * parent = 0, const char * name = 0 );
    QmvQueryWidget( QmvClass * cls, QWidget * parent = 0, const char * name = 0 );
    QmvQueryWidget( QmvQueryWidget * qw, QWidget * parent = 0, const char * name = 0 );
    ~QmvQueryWidget();

    QmvSqlQuery * getQueryObject() const;
    void clearQueryObject();
    void initSimple();
    void initAdvanced();
    void setBaseQuery( const QString & query );
    const QString getBaseQuery() const;
    QDialog * getAdvancedDialog() const;
    
      signals:
    void relationChanged();
    
  public slots:
    void noticeQueryStatus( int status, QmvClass * cls );
    void orderbyChanged( int pos );
    void orderbyChanged( const QString & ob_text );
    void simpleQueryChanged( const QString & text );
    void updateRelationSet();
    void setActive( bool );

  private slots:
    void advancedQuery();
    
      private:
    bool new_query_object;                      //!< is query object created or supplied to constructor
    QmvSqlQuery * query_object;                 //!< The base query object
    
    QComboBox * cb_order_list;
    QGuardedPtr<QDialog> dialog_advqry;
    QSpinBox * sb_query_limit;
    QSpinBox * sb_query_offset;
    QmvIcons * query_icons;
    QToolTipGroup * query_ttg;
    QGridLayout * g_layout;
    QPushButton * createWhereButton( const QString & label, QWidget * parent, const char * name = 0);
    void createOrderByComboBox( QWidget * parent, const char * name );
    QPtrDict<QmvQueryConditionWidget> condition_list;
    QmvQueryConditionWidget * addCondition( QWidget * parent, int row );
    int initConditions( QWidget * parent, int minsize = 0 );
    QmvQueryConditionWidget * getCondition( int row ) const;
    QProgressDialog * controller;
    
}; /* CLASS QmvQueryWidget */


#endif /* QMVQUERYWIDGET_H */
// Local Variables:
// mode:C++
// End:

