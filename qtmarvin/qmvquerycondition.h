/*
 *     $Id: qmvquerycondition.h 1878 2003-09-27 03:31:13Z source $
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

#ifndef QMVQUERYCONDITION_H
#define QMVQUERYCONDITION_H

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
#include <qstring.h>
#include <qintdict.h>
#include <qmvsqlquery.h>

/*
 * ======================================================================
 * CLASS DEFINITION - QmvQueryCondition
 * ======================================================================
 */

class QmvAttribute;

//! A standardised query condition (clause)
/*!
 * Using the attributes;
  
  \arg A comparison operator
  \arg A comparison value

  A standardised query condition of the form
\par
  \em "ATTRIBUTE <operator> <value>::DATATYPE"
\par
  can be prepared
  
*/

class QmvQueryCondition : public QObject
{
    Q_OBJECT

      public:    
    enum component_types { AttComponent, PatternComponent, OperatorComponent };
    enum operator_types { Matches, Like, Contains, Equals, GreaterThan, LessThan,
                          GreaterThanOrEqual, LessThanOrEqual, NotEqual, All };
 
//! 
/*!\enum operator_types
  Comparison operator types

*/
   
/*!   \var operator_types       Matches
 *                              case sensitive, anchored to start of target
 */
/*!   \var operator_types       Like
 *                              case insensitive, anchored to start of target
 */
/*!  \var operator_types        Contains          
 *                              case insensitive, anywhere in target
 */
/*!  \var operator_types        Equals            
 *                              exact match
 */
/*!  \var operator_types        GreaterThan       
 *                              numerically or .. , greater that target
 */
/*!  \var operator_types        LessThan          
 *                              numerically or .. , less that target
 */
/*!  \var operator_types        GreaterThanOrEqual 
 *                              numerically or .. , >= target
 */
/*!  \var operator_types        LessThanOrEqual   
 *                              numerically or .. , <= target
 */
/*!  \var operator_types        NotEqual          
 *n                             ot equal to target
 */
/*!  \var operator_types        All
 *                              matches every target
 */
   
    QmvQueryCondition( QmvSqlQuery * parent,
                       QmvAttribute * meta = 0,
                       const char * name = 0,
                       operator_types op = Equals,
                       const QString & val = QString::null );
    ~QmvQueryCondition();


    void setAttTitle( const QString & title, bool trigger = FALSE );
    void setAttValue( QmvAttribute * att );
    void setOpValue( operator_types op, bool trigger = FALSE );
    void setPatternValue( const QString & val, bool trigger = FALSE );
    const QString getAttTitle() const;
    const operator_types getOpValue() const;
    const QString getPatternValue() const;
    

    const QString & getOpLabel( operator_types op ) const;
    const QString & getOpBtLab( operator_types op ) const;
    const QString & getOpValue( operator_types op ) const;
    operator_types getOperator() const;
    QString getValue() const;
    
    const QStringList & getAttributeTitles() const;
    int getListPosition( const QString & title ) const;
    const QString & getListTitle( int pos ) const;
    bool active() const;
    void setActive( bool status = TRUE );
    
    const QString sqlCondition( QmvAttribute * at,
                                operator_types op,
                                const QString & val );
    const QString sqlCondition();

      signals:
    void queryChanged( QmvQueryCondition * qc, int type, bool trigger );
    
      private:
    QmvSqlQuery * parent_query;
    QStringList attribute_titles;
    QIntDict<QString> oplabel;
    QIntDict<QString> opbtlab;
    QIntDict<QString> opvalue;
    QString c_att_title;
    operator_types c_operator;
    QString c_value;
    QmvAttribute * meta_attributes;
    QString str_condition;
    bool active_status;
    
}; /* CLASS QmvQueryCondition */

#include "qmvattribute.h"

#endif /* QMVQUERYCONDITION_H */
// Local Variables:
// mode:C++
// End:
