/*
 *     $Id: qmvquerycondition.cpp 3211 2006-12-04 00:53:56Z source $
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
#include "qmvquerycondition.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmvquerycondition_version[] = "$Id: qmvquerycondition.cpp 3211 2006-12-04 00:53:56Z source $";

/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */


//--------------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------------
QmvQueryCondition::QmvQueryCondition( QmvSqlQuery * parent,
                                      QmvAttribute * meta,
                                      const char * name,
                                      operator_types op,
                                      const QString & val)
        : QObject(parent, name), parent_query(parent),
          meta_attributes(meta), c_operator(op), c_value(val), active_status(true)
{

        // load attribute titles
    if ( parent_query )
    {
        attribute_titles = (( QmvSqlQuery * )parent_query)->getAttributeTitles();
            // load default attribute if needed.
        if ( attribute_titles.count() && !meta )
            setAttValue( parent_query->getAttribute(attribute_titles[0] ) );
    }
    
    if (meta_attributes)
        c_att_title = meta_attributes->attributeTitle();
    
        // ------------------------------------------------------------
        // -- operator labels
        // ------------------------------------------------------------

    oplabel.insert( Matches,                new QString("Matches") );
    oplabel.insert( Like,                   new QString("Like") );
    oplabel.insert( Contains,               new QString("Contains") );
    oplabel.insert( Equals,                 new QString("Equals") );
    oplabel.insert( GreaterThan,            new QString("GT") );
    oplabel.insert( LessThan,               new QString("LT") );
    oplabel.insert( GreaterThanOrEqual,     new QString("GE") );
    oplabel.insert( LessThanOrEqual,        new QString("LE") );
    oplabel.insert( NotEqual,               new QString("Not") );
    oplabel.insert( All,                    new QString("All") );
    
        // ------------------------------------------------------------
        // -- operator button labels
        // ------------------------------------------------------------

    opbtlab.insert( Matches,                new QString("M") );
    opbtlab.insert( Like,                   new QString("L") );
    opbtlab.insert( Contains,               new QString("C") );
    opbtlab.insert( Equals,                 new QString("=") );
    opbtlab.insert( GreaterThan,            new QString("GT") );
    opbtlab.insert( LessThan,               new QString("LT") );
    opbtlab.insert( GreaterThanOrEqual,     new QString("GE") );
    opbtlab.insert( LessThanOrEqual,        new QString("LE") );
    opbtlab.insert( NotEqual,               new QString("Not") );
    opbtlab.insert( All,                    new QString("All") );
    
        // ------------------------------------------------------------
        // -- operator values
        // ------------------------------------------------------------

    opvalue.insert( Matches,                new QString("~") );
    opvalue.insert( Like,                   new QString("~*") );
    opvalue.insert( Contains,               new QString("~*") );
    opvalue.insert( Equals,                 new QString("=") );
    opvalue.insert( GreaterThan,            new QString(">") );
    opvalue.insert( LessThan,               new QString("<") );
    opvalue.insert( GreaterThanOrEqual,     new QString(">=") );
    opvalue.insert( LessThanOrEqual,        new QString("<=") );
    opvalue.insert( NotEqual,               new QString("<>") );
    opvalue.insert( All,                    new QString("all") );

}


//--------------------------------------------------------------------------------
// Destructor
//--------------------------------------------------------------------------------
QmvQueryCondition::~QmvQueryCondition()
{
}
 

//--------------------------------------------------------------------------------
//!  Set the attribute (private)
/*!

  \param att    
*/
//--------------------------------------------------------------------------------
void QmvQueryCondition::setAttValue( QmvAttribute * att )
{
    meta_attributes = att;
}


//--------------------------------------------------------------------------------
//!  Set the attribute title
/*!

  \param att    
*/
//--------------------------------------------------------------------------------
void QmvQueryCondition::setAttTitle( const QString & title, bool trigger )
{
    c_att_title = title;
    emit queryChanged( this, AttComponent, trigger );
}


//--------------------------------------------------------------------------------
//!  Set the operator
/*!

  \param op     
*/
//--------------------------------------------------------------------------------
void QmvQueryCondition::setOpValue( operator_types op, bool trigger )
{
    c_operator = op;
    emit queryChanged( this, OperatorComponent, trigger );
}

 

//--------------------------------------------------------------------------------
//! Set the value
/*!

  \param val    
*/
//--------------------------------------------------------------------------------
void QmvQueryCondition::setPatternValue( const QString & val, bool trigger )
{
    if ( val.isNull() )
        c_value = QString("");
    else
        c_value = val;

    emit queryChanged( this, PatternComponent, trigger );
}

 

//--------------------------------------------------------------------------------
//!  Return the current attribute title
/*!

*/
//--------------------------------------------------------------------------------
const QString QmvQueryCondition::getAttTitle() const
{
    return c_att_title;
}


//--------------------------------------------------------------------------------
//!  Return the current operator
/*!

*/
//--------------------------------------------------------------------------------
const QmvQueryCondition::operator_types QmvQueryCondition::getOpValue() const
{
    return c_operator;
}

 

//--------------------------------------------------------------------------------
//! Return the current pattern value
/*!

*/
//--------------------------------------------------------------------------------
const QString QmvQueryCondition::getPatternValue() const
{
    return c_value;
}

 

//--------------------------------------------------------------------------------
//! Get an operator label
/*!

  \param op     
*/
//--------------------------------------------------------------------------------
const QString & QmvQueryCondition::getOpLabel( operator_types op ) const
{
    return *oplabel.find( op );
}



//--------------------------------------------------------------------------------
//! Get an operator button label
/*!

  \param op     
*/
//--------------------------------------------------------------------------------
const QString & QmvQueryCondition::getOpBtLab( operator_types op ) const
{
    return *opbtlab.find( op );
}



//--------------------------------------------------------------------------------
//!  Get an operator value
/*!

  \param op     
*/
//--------------------------------------------------------------------------------
const QString & QmvQueryCondition::getOpValue( operator_types op ) const
{
    return *opvalue.find( op );
}




//--------------------------------------------------------------------------------
//! Return the comparison operator
/*!

  \return <ReturnValue>
*/
//--------------------------------------------------------------------------------
QmvQueryCondition::operator_types QmvQueryCondition::getOperator() const
{
    return c_operator;
}

 

//--------------------------------------------------------------------------------
//! Return the comparison value
/*!

  \return <ReturnValue>
*/
//--------------------------------------------------------------------------------
QString QmvQueryCondition::getValue() const
{
    return c_value;
}


//--------------------------------------------------------------------------------
//! Get the attribute title list
/*!

*/
//--------------------------------------------------------------------------------
const QStringList & QmvQueryCondition::getAttributeTitles() const
{
    return attribute_titles;
}

//--------------------------------------------------------------------------------
//! Get the list position of \em title
/*!

  \param title  
  \return  list position
*/
//--------------------------------------------------------------------------------
int QmvQueryCondition::getListPosition( const QString & title ) const
{
    return attribute_titles.findIndex( title );
}

//--------------------------------------------------------------------------------
//! Get list title at \em pos
/*!

  \param pos    
*/
//--------------------------------------------------------------------------------
const QString & QmvQueryCondition::getListTitle( int pos ) const
{
    if ( pos >= 0 && pos < attribute_titles.count() )
        return attribute_titles[pos];

    return QString::null;
}


//--------------------------------------------------------------------------------
//! Return the current status of this condition 
/*!
  - TRUE if active, FALSE if deactivated

  \return <ReturnValue>
*/
//--------------------------------------------------------------------------------
bool QmvQueryCondition::active() const
{
    return active_status;
}

void QmvQueryCondition::setActive( bool status )
{
    active_status = status;
}

//--------------------------------------------------------------------------------
//! Return the SQL query condition
/*!
  Return an SQL condition of the form

  \em "<conjunctiion> ATTRIBUTE <operator> <value>::DATATYPE"

  using the supplied paramters. The parameters are stored.
  
  \param cj     Conjunctive term
  \param op     Comparison operator
  \param val    Comparison value
  \return       SQL query condition.
*/
//--------------------------------------------------------------------------------
const QString QmvQueryCondition::sqlCondition( QmvAttribute * at,
                                               operator_types op,
                                               const QString & val )
{
    
        // set local values
    setAttValue( at );
    setOpValue( op );
    setPatternValue( val);

    return ( sqlCondition() );
    
}



//--------------------------------------------------------------------------------
//! Return the SQL query condition
/*!
  Using currently stored parameters.

  \return <ReturnValue>
*/
//--------------------------------------------------------------------------------
const QString QmvQueryCondition::sqlCondition()
{
    if (!meta_attributes || !active())
        return QString::null;

        // is this a virtual attribute
    QString attribute_name;
    if ( meta_attributes->inputMethod() == QmvAttribute::Virtual )
        attribute_name = QString("%1")
            .arg(meta_attributes->virtualAttribute());
    else
        attribute_name = meta_attributes->attributeName();
    
        // Clean the pattern string
    QString tmp_pattern = c_value;
    tmp_pattern = QmvClass::cleanForSQL(tmp_pattern);

        // If this is a compound string, then parse the components first
    QString sep = ",";
    tmp_pattern = tmp_pattern.replace( QRegExp("\\s*,\\s*"), sep);
    int pos_sep = tmp_pattern.find( sep );
    QString supplementary;
    if ( pos_sep >= 0 )
    {
        supplementary = tmp_pattern;
        supplementary.remove(0, pos_sep+1);
        tmp_pattern = tmp_pattern.left(pos_sep);
    }
    
        // Build the final string
    
    if ( c_operator == All )
        str_condition = QString("");
    else {
            // format string - has 4 variable positions;
            //   1. attribute name
            //   2. operator
            //   3. pattern
            //   4. dataype cast
        QString format;
        
            // NULLs are different ------------------------------
        if ( tmp_pattern.length() == 0 )
        {
            if ( c_operator == Equals )
                    // Equals looks for nulls, else no condition.
                str_condition
                    = QString( "%1 is null" )
                    .arg( attribute_name );
            else
                str_condition = QString("");
        } else {
                // other than null ------------------------------
            
            switch ( c_operator )
            {
                    // matches and like anchored to start of line
                case Matches:
                case Like:
                    if ( meta_attributes->attributeType() == "timestamp" )
                        format = QString(" date_trunc('day',(%1)) %2 date_trunc('day','^%3'::%4)");
                    else
                        if ( meta_attributes->attributeType() == "text" )
                            format = QString(" (%1) %2 '^%3'::%4");
                        else
                            format = QString( " (%1) %2 '%3'::%4");
                    break;
                        // within string
                case Contains:
                    format = QString(" (%1) %2 '.*%3'::%4");
                    break;
                default:
                    format = QString(" (%1) %2 '%3'::%4");
                    break;
            }
            str_condition
                = QString( format )
                .arg( attribute_name )
                .arg( getOpValue(c_operator))
                .arg(tmp_pattern)
                .arg( meta_attributes->attributeType() );
        }
        
    }
    
        // now append any supplementary components
        // (chained conditions - separated by ","
    if ( supplementary.length() > 0 )
    {
        int att_pos = getListPosition( getAttTitle() );
        if ( att_pos >= 0 && att_pos < attribute_titles.count() )
        {
            QString str_conj = "AND";
            if ( supplementary.left(1) == "|" ) {
                str_conj = "OR";
                supplementary.remove(0,1);
            }

            if (str_condition.length() < 1) {
                str_conj = "";
            }
            
            QmvQueryCondition * next_attribute
                = new QmvQueryCondition(parent_query,
                                        parent_query->getAttribute(attribute_titles[att_pos+1]),
                                        "supplementary",
                                        getOpValue(),
                                        supplementary);
            if (next_attribute)
            {
                    // the next condition will recurse along supplementary list
                QString str_next = next_attribute->sqlCondition();
                if ( str_next.length() > 0 ) {
                    str_condition.append( QString(" %1 %2").arg(str_conj).arg( str_next ));
                }    
                delete next_attribute;
            }
        }
    }
    
    return str_condition;
    
}       // sqlCondition

