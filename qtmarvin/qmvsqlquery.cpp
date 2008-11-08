/*
 *     $Id: qmvsqlquery.cpp 3181 2006-11-07 09:15:12Z source $
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

/*
 * ======================================================================
 * DESCRIPTION: A Query-By-Example widget
 *
 * ======================================================================
 */

/*!
  \file   qmvsqlquery.cpp
  \brief  Classes to build, store and edit SQL queries.

  SQL query conditions are handled in the form

  <conjunctiion> ATTRIBUTE <operator> <value>::DATATYPE
  
  \author Rex McMaster
  \date   2001-01-30
*/


/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <qwhatsthis.h>
#include "qmvsqlquery.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvsqlquery_version[] = "$Id: qmvsqlquery.cpp 3181 2006-11-07 09:15:12Z source $";

/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */

/*
 * ======================================================================
 * MEMBER FUNCTIONS: QmvSqlQuery Class
 * ======================================================================
 */


//--------------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------------

QmvSqlQuery::QmvSqlQuery( QmvClass * cls, const QString name )
        : QObject ( cls, name)
{
    setRelation( cls );
        // defaults
    setOffset( getRelation()->queryOffset().toInt() );
    setLimit( getRelation()->queryLimit().toInt() );
    setOrderby( getRelation()->orderBy() );
    controller = 0;
    
        // get a list of attribute titles and index them
    QmvAttList atlist = getRelation()->sortedAttlist( "mtat_view_order",
                                                      QmvClass::NoBlank|QmvClass::NoSystem );

    for ( QmvAttribute * at_ptr = atlist.first(); at_ptr != 0; at_ptr = atlist.next())
    {

#ifdef QMVSQLQUERY_DEBUG
        qDebug("printReport:%s, mtat_title = %s",
               at_ptr->find("mtat_name")->latin1(),
               at_ptr->find("mtat_title")->latin1());
#endif
        
            // load the label
        QString *title = at_ptr->find( "mtat_title");
        if ( !title )
            continue;
        
        attribute_titles += *title;
            // index a pointer to the QmvAttribute object
        attribute_index.insert( *title , at_ptr );
    }
}


//--------------------------------------------------------------------------------
// Destructor
//--------------------------------------------------------------------------------
QmvSqlQuery::~QmvSqlQuery()
{
    attribute_index.setAutoDelete( TRUE );
    attribute_index.clear();
}



//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void QmvSqlQuery::setRelation( QmvClass * cls )
{
    sql_relation = cls;
}



//--------------------------------------------------------------------------------
//! Return pointer to the database object
/*!
   Return a pointer to the \ref QmvClass database object associated with this
   query object.

*/
//--------------------------------------------------------------------------------
QmvClass * QmvSqlQuery::getRelation()
{
    return sql_relation;
}



//--------------------------------------------------------------------------------
//! Set the Base SQL query string. 
/*!
  Set the current base query, which is included as a component of all
  queries associated with this object.

  \param query  
*/
//--------------------------------------------------------------------------------
void QmvSqlQuery::setBaseQuery( const QString & query )
{
    base_query = query;
}



//--------------------------------------------------------------------------------
//! Return the base query.
/*!

*/
//--------------------------------------------------------------------------------
const QString QmvSqlQuery::getBaseQuery() const
{
    return base_query;
}



//--------------------------------------------------------------------------------
//! Return the \emorder-by clause.
/*!

  \return order by clause
*/
//--------------------------------------------------------------------------------
const QString QmvSqlQuery::getOrderby() const
{
    return query_orderby;
}



//--------------------------------------------------------------------------------
//! Return the query offset
/*!

  \return query offset
*/
//--------------------------------------------------------------------------------
const int QmvSqlQuery::getOffset() const
{
    return query_offset;
}



//--------------------------------------------------------------------------------
//! Return the query limit
/*!

  \return Row limit to query set.
*/
//--------------------------------------------------------------------------------
const int QmvSqlQuery::getLimit() const
{
    return query_limit;
}



//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void QmvSqlQuery::setOrderby( const QString & new_orderby )
{
    if ( new_orderby.isNull() || !new_orderby.length() )
        query_orderby = getRelation()->orderBy();
    else
        query_orderby = new_orderby;

    query_orderby.replace( QRegExp( "order +by" ), "" );
}



//--------------------------------------------------------------------------------
//! Set the query offset
/*!

  \param new_offset     
*/
//--------------------------------------------------------------------------------
void QmvSqlQuery::setOffset( int new_offset )
{
    query_offset = new_offset;
}



//--------------------------------------------------------------------------------
//! Set the query limit
/*!

  \param new_limit      
*/
//--------------------------------------------------------------------------------
void QmvSqlQuery::setLimit( int new_limit )
{
    query_limit = new_limit;
}


//--------------------------------------------------------------------------------
//! Set a progress dialog
/*!
  Set the associated progres dialog to \em pd

  \param pd     
*/
//--------------------------------------------------------------------------------
void QmvSqlQuery::setProgressDialog( QProgressDialog * pd )
{
    controller = pd;
}


//--------------------------------------------------------------------------------
//! Rebuild the condition list
/*!
  Rebuild and redisplay the condition list. This is usually in response to a change int the
  component conditions list.

  \ref QmvSqlQuery::addCondition and friends

*/
//--------------------------------------------------------------------------------
void QmvSqlQuery::rebuildList()
{
// ???
}



//--------------------------------------------------------------------------------
//! Coordinate condition changes
/*!
  Slot to trigger an undate of the data set associated with the query.

  \param who    The condition which has changed
  \param type   The part of the query condition which has changed.
*/
//--------------------------------------------------------------------------------
void QmvSqlQuery::noticeChanges( QmvQueryCondition * who, int type, bool reload )
{
    switch (type)
    {
        case QmvQueryCondition::AttComponent:
            who->setAttValue( getAttribute( who->getAttTitle() ) );
            break;
            
        case QmvQueryCondition::PatternComponent:
        case QmvQueryCondition::OperatorComponent:
            break;
            
        default:
            return;
    }

    if (reload)
        emit queryChanged();
}




//--------------------------------------------------------------------------------
//! Update the data set
/*!
  Rebuild the resultant dataset.

  \return  TRUE/FALSE
*/
//--------------------------------------------------------------------------------
bool QmvSqlQuery::updateRelationSet()
{
    int status = getRelation()->open( sqlQuery(), controller );
    if ( controller )
        controller->reset();
    
    if ( status < 0 )
    {
        query_errormessage = getRelation()->lastError();
        query_status = false;
    } else {
        query_errormessage = "";
        query_status = true;
        emit relationChanged( getRelation() );
    }
    
    emit queryStatus( status, getRelation() );        
    return query_status;
}

//--------------------------------------------------------------------------------
//! Return the current query status
/*!
  Return the status of the most recent query (\ref updateRelationSet()

  \return  TRUE/FALSE
*/
//--------------------------------------------------------------------------------
const bool QmvSqlQuery::getQueryStatus() const
{
    return query_status;
}

//--------------------------------------------------------------------------------
//! Return the current error message
/*!
  Return the status message associated with the most recent query (\ref updateRelationSet() )

  \return  Status Message
*/
//--------------------------------------------------------------------------------
const QString QmvSqlQuery::queryErrorMessage() const
{
    return query_errormessage;
}



//--------------------------------------------------------------------------------
//! Return list of attribute titles
/*!
  Return the list of table attributes in the \ref QmvClass associated with this
  query.

*/
//--------------------------------------------------------------------------------
const QStringList & QmvSqlQuery::getAttributeTitles() const
{
    return attribute_titles;
}



//--------------------------------------------------------------------------------
//! Return the index of order attribute..
/*!

  \return The index of the order attribute
*/
//--------------------------------------------------------------------------------
int QmvSqlQuery::getCurrentOrderbyPosition() const
{
    return getListPosition( getOrderby() );
}




//--------------------------------------------------------------------------------
//! Return the Attribute object of \em title
/*!
  Return a pointer to the \ref QmvAttribute object associated with \em title.

  \param title
  \return pointer
*/
//--------------------------------------------------------------------------------
QmvAttribute * QmvSqlQuery::getAttribute( const QString & title ) const
{
    return attribute_index.find( title );
}




//--------------------------------------------------------------------------------
//! Return the index of the Attribute object of \em title
/*!
  Return the index of the \ref QmvAttribute object associated with \em title.

  \param title  
  \return index
*/
//--------------------------------------------------------------------------------
int QmvSqlQuery::getListPosition( const QString & title ) const
{
    return attribute_titles.findIndex( title );
}



//--------------------------------------------------------------------------------
//! Return the title of the Attribute object at \em index
/*!
  Return the title of the \ref QmvAttribute object at  \em index

  \param title  
  \return index
*/
//--------------------------------------------------------------------------------
const QString QmvSqlQuery::getTitleAt( int index ) const
{
    if ( index < 0 || index >= attribute_titles.count() )
        return QString::null;
    
    return attribute_titles[index];
}



//--------------------------------------------------------------------------------
//! Add a condition
/*!
  Add a new QmvQueryCondition to the list at position \em pos

  \param newcond        pointer to new QmvQueryCondition
  \param after          position for insertion

  \return               index of new condition
*/
//--------------------------------------------------------------------------------
int QmvSqlQuery::addCondition( QmvQueryCondition * cond, int pos )
{
    if ( querylist.findRef( cond ) >= 0 )
        removeCondition( cond );
    
    if ( pos >= 0 && pos < querylist.count() )
        querylist.insert( pos, cond );
    else
        querylist.append( cond );

    if ( !conjunctive_index.find(cond) )
    {
        connect( cond, SIGNAL( queryChanged( QmvQueryCondition *, int, bool ) ),
                 this, SLOT( noticeChanges( QmvQueryCondition *, int, bool ) ) );
        conjunction_types * cj = new conjunction_types;
        *cj = QmvSqlQuery::And;
        conjunctive_index.insert( cond, cj );
    }

    emit listChanged();
    
    return querylist.findRef( cond );
}


//--------------------------------------------------------------------------------
//! Add a condition
/*!
  Add a new QmvQueryCondition to the list, after condition \em after

  \param cond        pointer to new QmvQueryCondition
  \param after          position after this condition

  \return               index of new condition
*/
//--------------------------------------------------------------------------------
int QmvSqlQuery::addCondition( QmvQueryCondition * cond, QmvQueryCondition * after )
{
    int pos = querylist.findRef( after );
    if ( pos >= 0 )
        pos++;
    
    pos = addCondition( cond, pos );
    return pos;
}



//--------------------------------------------------------------------------------
//! Create a condition 
/*!
  Add a new QmvQueryCondition to the list at position \em pos

  \param cond        pointer to new QmvQueryCondition
  \param after          position for insertion

  \return               index of new condition
*/
//--------------------------------------------------------------------------------
QmvQueryCondition * QmvSqlQuery::addCondition( int pos )
{
    QmvQueryCondition * newcond = new QmvQueryCondition( this );
    addCondition( newcond, pos );
    return newcond;
}



//--------------------------------------------------------------------------------
//! Remove a condition
/*!
  Remove the QmvQueryCondition condition pointed to by \em cond

  \param cond
  \return TRUE/FALSE as status.
*/
//--------------------------------------------------------------------------------
bool QmvSqlQuery::removeCondition( QmvQueryCondition * cond )
{
    if ( !cond )
        return FALSE;
    
    querylist.setAutoDelete(FALSE);
    bool stat = querylist.removeRef(cond);
    if ( stat )
        emit listChanged();
    return stat;
}



//--------------------------------------------------------------------------------
//! Remove a condition
/*!
  Remove the QmvQueryCondition condition at index \em pos

  \param    pos
  \return TRUE/FALSE as status.
*/
//--------------------------------------------------------------------------------
bool QmvSqlQuery::removeCondition( int pos )
{
    return removeCondition( querylist.at( pos ) );
}



//--------------------------------------------------------------------------------
//! Remove all conditions
/*!
  Remove all the conditions from the list.

*/
//--------------------------------------------------------------------------------
void QmvSqlQuery::clear()
{
    querylist.setAutoDelete(FALSE);
    querylist.clear();
    emit listChanged();
}




//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
const uint QmvSqlQuery::count() const
{
    return querylist.count();
}



//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
int QmvSqlQuery::countConditions() const
{
    return querylist.count();
}



//--------------------------------------------------------------------------------
//! Return a QmvQueryCondition condition
/*!
  Returns a pointer to the QmvQueryCondition at \em pos

  \param pos    Index of condition
*/
//--------------------------------------------------------------------------------
QmvQueryCondition * QmvSqlQuery::getCondition( const int pos )
{
    if ( pos < 0 || pos >= querylist.count() )
        return 0;

    return querylist.at( pos );
}



//--------------------------------------------------------------------------------
//! Return a QmvQueryCondition index
/*!
  Returns a n index to the QmvQueryCondition \em cond

  \param pos    Index of condition
*/
int QmvSqlQuery::getCondition( const QmvQueryCondition * cond  )
{
    return querylist.findRef( cond );

}

//--------------------------------------------------------------------------------
//! Return a consolidated query condition
/*!
  Returns a consolidated SQL \em where condition.

  \return QString where condition
*/
//--------------------------------------------------------------------------------
QString QmvSqlQuery::sqlQuery()
{
  
        // Build the final string
    QString str_query = getBaseQuery();

        // The base query can be set externally - eg for child subsets,
        // otherwise use a default
    if ( str_query.length() == 0 )
        str_query = QString("SELECT * FROM %1 WHERE TRUE ")
            .arg(sql_relation->relationName());
    else
    {
            // strip after WHERE clause - more conditions to come
        str_query.replace( QRegExp( "order +by .*$" ), "" );
        str_query.replace( QRegExp( "group +by .*$" ), "" );
        str_query.replace( QRegExp( "having .*$" ), "" );
    }
    
    
    int cond_count = querylist.count();

        // Add the conditions
    str_query.append( sqlQueryConditions() );
    
        // add the order-by
    if ( getOrderby().length() )
        str_query.append( QString(" ORDER BY %1").arg( getOrderby() ) );

        // add the offset and limit
    if ( getLimit() > 0 )
        str_query.append( QString(" LIMIT %1 OFFSET %2").arg( getLimit() ).arg( getOffset() ) );
    
    return str_query;
}

//--------------------------------------------------------------------------------
//! Return the sum of query conditions
/*!
  Returns a consolidated SQL \em where condition from the user-supplied filters

  \return QString where condition
*/
//--------------------------------------------------------------------------------
QString QmvSqlQuery::sqlQueryConditions()
{
    QString str_query_conditions;
    int cond_count = querylist.count();

        // Add the conditions
    for (int i = 0; i < cond_count; i++)
    {
        QmvQueryCondition * cond = querylist.at(i);
        if ( !cond || !cond->active() )
            continue;
        conjunction_types * cj = conjunctive_index.find(cond);
        QString str_cj;
        switch ( *cj )
        {
            case QmvSqlQuery::And:
                str_cj = QString("AND");
                break;
            case QmvSqlQuery::Not:
                str_cj = QString("NOT");
                break;
            case QmvSqlQuery::Xor:
                    // TODO
                break;
            case QmvSqlQuery::Or:
                str_cj = QString("OR");
                break;
            default:
                return 0;
                break;
        }
        
        QString cond_str = cond->sqlCondition();
        if ( !cond_str.isNull() && cond_str.length() )
            str_query_conditions.append( QString(" %1 %2").arg(str_cj).arg(cond_str) );
    }
    
    return str_query_conditions;
}

//--------------------------------------------------------------------------------
//! Return the estimated count associated with current query
/*!
  Runs a select count(*) with the current query to estimate the number of rows
  to be returned.

  \return int count (returns -1 if error)
*/
//--------------------------------------------------------------------------------

int QmvSqlQuery::countEstimate()
{
    QString str_query = sqlQuery();
    str_query.replace( QRegExp( "^.* from ", false ), "SELECT count(*) as count_estimate FROM " );
    str_query.replace( QRegExp( " order by +.*$", false ), "" );
    int status = sql_exec(str_query);
    if (status < 1) {
        return -1;
    }

    bool ok = false;
    int count =  QString(sql_value(0, "count_estimate")).toInt(&ok);
    if (ok) {
        return count;
    } else {
        return -1;
    }
}

