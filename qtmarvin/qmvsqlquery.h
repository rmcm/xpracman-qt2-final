/*
 *     $Id: qmvsqlquery.h 3181 2006-11-07 09:15:12Z source $
 * 
 *   Copyright 2000 X=X Computer Software Trust
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

#ifndef QMVSQLQUERY_H
#define QMVSQLQUERY_H

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
#include <qdict.h>
#include <qlist.h>
#include <qintdict.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qptrdict.h>
#include <qbuttongroup.h>
#include <qtooltip.h>
#include <qspinbox.h>
#include <qprogressdialog.h>
#include <qguardedptr.h>

#include "qmvicons.h"

class QmvAttribute;
class QmvQueryCondition;
class QmvClass;


/*
 * ======================================================================
 * CLASS DEFINITION   QmvSqlQuery
 * ======================================================================
 */


//! An Sql Query Object
/*!
  Is the parent for one or more QmvQueryCondition objects

  \param cls    assoicated QmvClass object to query
  \param parent parent object
  \param name   name of this query object 
*/
class QmvSqlQuery : public QObject
{
    Q_OBJECT
    
 public:
    enum conjunction_types { And, Or, Not, Xor };
//! 
/*!\enum conjunction_types
  Conjunction types

*/
   
/*!   \var conjunction_types    And
 *                              And this condition 
 */
/*!   \var conjunction_types    Or
 *                              Or this condition
 */
/*!   \var conjunction_types    Not
 *                              Not this condition
 */
/*!   \var conjunction_types    Xor
 *                              TODO::IMPLEMENT - currently implented as Or
 */
    
        // Constructor
    QmvSqlQuery(  QmvClass * cls, const QString name = 0 );
        // Destructor
    ~QmvSqlQuery();

    void setRelation( QmvClass * cls );
    QmvClass * getRelation();
    void setBaseQuery( const QString & query );
    const QString getBaseQuery() const;
    const QString getOrderby() const;
    const int getOffset() const;
    const int getLimit() const;
    
    const QStringList & getAttributeTitles() const;
    int getCurrentOrderbyPosition() const;
    QmvAttribute * getAttribute( const QString & title ) const;
    int getListPosition( const QString & title ) const;
    const QString getTitleAt( int index ) const;
    int addCondition( QmvQueryCondition * newcond, int pos = -1 );
    int addCondition( QmvQueryCondition * newcond,
                       QmvQueryCondition * after);
    QmvQueryCondition * addCondition( int pos = -1);
    
    bool removeCondition( QmvQueryCondition * newcond );
    bool removeCondition( int pos );
    void clear();
    const uint count() const;
    
    int countConditions() const;
    QmvQueryCondition * getCondition( const int pos );
    int getCondition( const QmvQueryCondition * cond  );
    QString sqlQuery();
    int countEstimate();
    QString sqlQueryConditions();
    const bool getQueryStatus() const;
    const QString queryErrorMessage() const;
    void setProgressDialog( QProgressDialog * pd );
    
  signals:
    
    void queryChanged();
    void listChanged();
    void relationChanged( QmvClass * cls );
    void queryStatus( int status, QmvClass * cls );
    
  public slots:
    void setOrderby( const QString & new_orderby );
    void setOffset( int new_offset );
    void setLimit( int new_limit );
    void rebuildList();
    void noticeChanges( QmvQueryCondition * who, int type, bool reload = TRUE );
    bool updateRelationSet();
    
 private:
    QmvClass * sql_relation;
    QStringList attribute_titles;
    QDict<QmvAttribute> attribute_index;
    QDict<int> title_index;
    QList<QmvQueryCondition> querylist;
    QPtrDict<conjunction_types> conjunctive_index;
    QString base_query;
    QString query_orderby;
    int query_limit;
    int query_offset;
    QGuardedPtr<QProgressDialog> controller;
    bool query_status;
    QString query_errormessage;
    
}; /* CLASS QmvSqlQuery */

#include "qmvquerycondition.h"
#include "qmvattribute.h"
#include "qmvclass.h"


#endif /* QMVSQLQUERY_H */
// Local Variables:
// mode:C++
// End:

