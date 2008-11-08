/*
 *     $Id: qmvattribute.cpp 3020 2006-09-05 02:56:16Z source $
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
 TODO: label_list/value_dict/value_dict. The maintenace should be tightly
 bound. 
 */

/*
 * ======================================================================
 * DESCRIPTION: 
 *
 * ======================================================================
 */

/*!
  \file   qmvattribute.cpp
  \brief  Source for QmvAttribute class

  A class to hold attribute metadata information, which describes tuple
  attributes (see QmvTupleAttribute).

  \author Rex McMaster, rmcm@compsoft.com.au
  \date   2000-11-06
*/


/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <iostream.h>
#include <qregexp.h>
#include "qmvattribute.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvattribute_version[] = "$Id: qmvattribute.cpp 3020 2006-09-05 02:56:16Z source $";

/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */

/*
 * ======================================================================
 * MEMBER FUNCTIONS:
 * ======================================================================
 */


// ----------------------------------------------------------------------
//!  Constructor
/*!

  Constructor for QmvAttribute

*/
// ----------------------------------------------------------------------
QmvAttribute::QmvAttribute( QmvClass * reln )
        : parent(reln), type(TypeUnknown), input(InputUnknown), autosortlist(true), limit_list(0),
          multiline(-1), access(AccessUnknown), edit_prompting(false),
          foreign_metadata(0), foreign_relation(0)
{
    label_dict.setAutoDelete( true );
    value_dict.setAutoDelete( true );
    alert_list.setAutoDelete( true );

}


// ----------------------------------------------------------------------
//! Destructor
/*!

  Destructor for QmvAttribute

*/
// ----------------------------------------------------------------------
QmvAttribute::~QmvAttribute()

{
    alert_list.clear();
}

// ----------------------------------------------------------------------
//! Return a pointer to the parent \ref QmwClass Object
/*!

*/
// ----------------------------------------------------------------------
QmvClass * QmvAttribute::parentRelation() const
{
    return parent;
}

// ----------------------------------------------------------------------
//! Return the attribute named
/*!

 Return the attribute named in the argument.

 \ar    attribute name

*/

// ----------------------------------------------------------------------
const QString QmvAttribute::attributeAttribute(const QString & att) const
{
    QString attval;
    QString * attptr = find(att);
    if (attptr)
        attval = *attptr;
    return attval;
}

// ----------------------------------------------------------------------
// Hardwired wrappers for attributeAttribute()
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeAccess() const
{
return attributeAttribute("mtat_access");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeName() const
{
return attributeAttribute("mtat_name");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeType() const
{
return attributeAttribute("mtat_type");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeLength() const
{
return attributeAttribute("mtat_length");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeAttributes() const
{
return attributeAttribute("mtat_attributes");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeOperator() const
{
return attributeAttribute("mtat_operator");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeTitle() const
{
return attributeAttribute("mtat_title");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeClassName() const
{
return attributeAttribute("mtat_class_name");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeDisplen() const
{
return attributeAttribute("mtat_displen");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeBrowseOrder() const
{
return attributeAttribute("mtat_browse_order");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeViewOrder() const
{
return attributeAttribute("mtat_view_order");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeInputMethod() const
{
return attributeAttribute("mtat_input_method");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeDefault() const
{
return attributeAttribute("mtat_default");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeHtmltagatt() const
{
return attributeAttribute("mtat_htmltagatt");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeUserEntry() const
{
return attributeAttribute("mtat__user_entry");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeStatus() const
{
return attributeAttribute("mtat__status");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeTimestamp() const
{
return attributeAttribute("mtat__timestamp");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::attributeSequence() const
{
return attributeAttribute("mtat__sequence");
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvAttribute::virtualAttribute() const
{
    return virtual_att;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QmvAttribute::data_type QmvAttribute::dataType() const
{
    return type;
}
// ----------------------------------------------------------------------
const bool QmvAttribute::autoSortList() const
{
    return autosortlist;
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvAttribute::setAutoSortList( bool enable )
{
    autosortlist = enable;
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QmvAttribute::input_method QmvAttribute::inputMethod() const
{
    return input;
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QmvAttribute::access_method QmvAttribute::accessMethod() const
{
    return access;
}

// ----------------------------------------------------------------------
//! get the edit-prompt flag
// ----------------------------------------------------------------------
const bool QmvAttribute::getEditPrompting() const
{
    return edit_prompting;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const int QmvAttribute::listLimit() const
{
    return limit_list;
}


// ----------------------------------------------------------------------
//! Set the internal data type
/*!
  Map the persistant storage (SQL) datatype to an internal datatype (enum \ref data_type)

  \param val    
*/
// ----------------------------------------------------------------------
void QmvAttribute::setdataType( const data_type val )
{
    
    if ( val != TypeUnknown )
    {
        type = val;
        return;
    }
    
    QString * strtype = find("mtat_type");
    if ( !strtype )
        return;

    if ( strtype->contains("text", FALSE) || strtype->contains("char", FALSE) )
    {
        type = Text;
        return;
    }
    
    if ( strtype->contains("int", FALSE) )
    {
        type = Integer;
        return;
    }
    if ( strtype->contains("numeric", FALSE) )
    {
        type = Numeric;
        return;
    }
    if ( strtype->contains("timestamp", FALSE) )
    {
        type = Timestamp;
        return;
    }
    if ( strtype->contains("boolean", FALSE) )
    {
        type = Boolean;
        return;
    }
}

// ----------------------------------------------------------------------
//! Set the input method for this attribute.
/*!
  Set the input method to a member of the \em input_method enum.

  \param val    Input method
*/
// ----------------------------------------------------------------------
void QmvAttribute::setInputMethod( const input_method val )
{
    if ( val != InputUnknown )
    {
        input = val;
        return;
    }
   
    QString * im = find( "mtat_input_method" );
    if ( !im )
        return;

        // Virtual attributes
        // - an attribute which does not exist in the base table - eg computed attributes
    
    if ( im->contains( QRegExp("^\\s*VIRTUAL\\s*=") ) )
    {
        virtual_att = QString(*im);
        virtual_att.replace( QRegExp("^\\s*VIRTUAL\\s*="), QString() );
        input = Virtual;
    }

        // CopyText method
        // - contents of this field determine content of another field
        // - eg postcode -> suburb and suburb->postcode

    if ( im->contains( QRegExp("^\\s*COPYTEXT\\s*=") ) )
    {
        QString tmpsel = QString(*im);
        tmpsel.replace( QRegExp("^\\s*COPYTEXT\\s*="), QString() );
        input = CopyText;
        QStringList x_str = QStringList::split( QChar(SLSeparator), tmpsel  );
            // need to find the database to build a query object.
        if ( parent && x_str.count() == 4 )
        {
//            copy_text = new copyTextMeta;
            copy_text.name = x_str[0];
            copy_text.key = x_str[1];
            copy_text.src = x_str[2];
            copy_text.dst = x_str[3];
            copy_text.reln = (QmvSqlClass *) parent->dataBase()->dbRelation( copy_text.name );
        }
    }

        // DateTime family
    if ( im->contains( QRegExp("^\\s*DATETIME\\s*") ) )
        input = DateTime;
    if ( im->contains( QRegExp("^\\s*DATEONLY\\s*") ) )
        input = DateOnly;
    if ( im->contains( QRegExp("^\\s*TIMEONLY\\s*") ) )
        input = TimeOnly;
    if ( im->contains( QRegExp("^\\s*DATENOTEXT\\s*") ) )
        input = DateNoText;
    
    
        // Colour selector
    if ( im->contains( QRegExp("^\\s*COLOUR\\s*") ) )
    {
        input = Colour;
    }
    
        // Embedded tag - List limit
        // - this does not set an imput method, just a limit.
    limit_list = 0;       // arbitrary limit
    if ( im->contains( QRegExp("^\\s*FSL\\s*=\\s*[0-9]*\\s*$") ) )
    {
        QString tmpsel = QString(*im);
        tmpsel.replace( QRegExp("^\\s*FSL\\s*="), QString() );
        limit_list = tmpsel.toInt();
    }
        // Embedded tag - List
    if ( im->contains( QRegExp("^\\s*SELECT\\s*=") ) )
    {
        setAutoSortList(FALSE);
        QString tmpsel = QString(*im);
        tmpsel.replace( QRegExp("^\\s*SELECT\\s*="), QString() );
        setList( tmpsel, ";", "," );
        input = List;
    }

        // Embedded tag - multiline
    if ( im->contains( QRegExp("^\\s*\\(TEXTAREA|MULTILINE\\)\\s*=") ) )
    {
        QString tmpsel = QString(*im);
        tmpsel.replace( QRegExp("^\\s*\\(TEXTAREA|MULTILINE\\)\\s*="), QString() );
        multiline = tmpsel.toInt();
        setList( tmpsel, ";", "," );
        input = MultiLine;
    }

        // Foreign relation
        // NOTE:: the foreignmetadata is probably not loaded when this method is
        //        called from qmvclass constructor. So this setting is not definitive.
        //        See setForeign().
    if ( metaForeign() && input == InputUnknown )
    {
        input = List;
    }
        

}

// ----------------------------------------------------------------------
//! Set the access method for this attribute.
/*!
  Set the access method to a member of the \em access_method enum.

  \param val    Access method
*/
// ----------------------------------------------------------------------
void QmvAttribute::setAccessMethod( const access_method val )
{
    if ( val != AccessUnknown )
    {
        access = val;
        return;
    }
   
    QString * am = find( "mtat_input_method" );
    if ( !am )
        return;

    QStringList tokens = QStringList::split( QRegExp("\\s+"), *am );
    
    for ( QStringList::Iterator it = tokens.begin(); it != tokens.end(); ++it )
    {
        
            // Embedded tag - System
        if ( (*it) == "SYSTEM")
        {
            access = SystemAccess;
            return;
        }

            // Embedded tag - Readonly
        if ( (*it) == "RO" || (*it).contains( QRegExp("^\\s*VIRTUAL\\s*=") ))
        {
            access = ReadOnly;
            return;
        }

            // Embedded tag - Writeonly
        if ( (*it) == "WO")
        {
            access = WriteOnly;
            return;
        }
    
            // Embedded tag - ReadWrite
        if ( (*it) == "RW")
        {
            access = ReadWrite;
            return;
        }

            // Edit-prompting
        if ( (*it) == "RWP" )
        {
            access = ReadWrite;
            edit_prompting = true;
            return;
        }

    }
    

}

// ----------------------------------------------------------------------
//! Set flag for edit prompting
/*!


  \param do_prompt      true/false
*/
// ----------------------------------------------------------------------
void QmvAttribute::setEditPrompting( bool do_prompt )
{
    edit_prompting = do_prompt;
}

// ----------------------------------------------------------------------
//! Set the limit for in-memory foreign list
/*!


  \param listlimit      
*/
// ----------------------------------------------------------------------
void QmvAttribute::setListLimit( int listlimit )
{
    limit_list = listlimit;
}



// ----------------------------------------------------------------------
//! Return the label list/
/*!
  Returns a QStringList of labels.

  \return       label list
*/
// ----------------------------------------------------------------------
const QStringList QmvAttribute::getLabelList() const
{
    return label_list;
}

// ----------------------------------------------------------------------
//! Return the value associated with the label
/*!

  \param        label  
  \return       value
*/
// ----------------------------------------------------------------------
const QString QmvAttribute::getValueByLabel( const QString & label ) const
{
    QString * ptr = label_dict.find( label );
    qDebug("QmvAttribute::getValueByLabel:label=%s value=%s", label.latin1(), (ptr)?(*ptr).latin1():"" );
    if ( ptr )
        return *ptr;
    else
        return QString::null;
    
}


// ----------------------------------------------------------------------
//! Return the QmvComboBox index of label
/*!
  Return the index in the QmvComboBox list of the supplied \em label.

  \param        label  
  \return       index
*/
// ----------------------------------------------------------------------
const int QmvAttribute::getIndexByLabel( const QString & label ) const
{
    int index = -1;
    
    int i = 0;
    
    for ( QValueListConstIterator<QString> it = QmvAttribute::label_list.begin();
          it != QmvAttribute::label_list.end(); ++it )
    {
        if ( (*it) == label )
        {
            index = i;
            break;
        }
        i++;
    }
    qDebug("QmvAttribute::getIndexByLabel:label=%s index=%d", label.latin1(), index );
    
    return index;
    
}

// ----------------------------------------------------------------------
//! Extend the value list.
/*!

  Extend the value list's set with the results of \param search. This
  is only valid for foreign keys. If \param labeltoo is TRUE (default
  is FALSE), then extend the search into the label attribute of the
  foreign table. If \param replace is TRUE (default is FALSE), then
  clear the list before loading, else append the the existing list
  (duplicates are ignored). .

  \param search 
  \param labeltoo       
  \param replace        
  \return       ths number of matches in the list

*/

// ----------------------------------------------------------------------
int QmvAttribute::extendList( const QString & search, bool labeltoo, bool replace, bool fuzzy )
{
    int row = 0;

    if ( search.isNull() || !search.length() )
        return 0;

        // Need definition and implementation of foreign class
    if ( metaForeign() && foreignRelation() )
    {
        if ( replace )
            clearList();

            // Does this already exist? If so, then if not a fuzzy match
            // then return
        QString * ptr = value_dict.find( search );
        if ( ptr && !fuzzy )
            return 1;
        
        QString * fkey  = metaForeign()->find( "mtfn_key" );
        QString * fjoin = metaForeign()->find( "mtfn_join" );
        QString * fview = metaForeign()->find( "mtfn_view" );
        QString query = QString( "select * from %1 where %2 ~* '%3'" )
            .arg(foreignRelation()->relationName())
            .arg( *fjoin )
            .arg( search );
        
            // also search foreign_view?
        if ( labeltoo )
            query.append( QString( " or %1 ~* '%2'" )
                          .arg( *fview )
                          .arg( search ) );
        if (foreignRelation()->queryLimit().toInt() > 0 )
            query.append( QString( " limit %1" ).arg( foreignRelation()->queryLimit() ) );
                          
        int count = foreignRelation()->open( query );
        
            // load value/label pairs into list
            // - note use fview for label - the userlabelvalue is contains markup
            //   and may be very long, so unsuitable for comboboxes
        for ( row = 0; row < count; row++ )
            addToList( foreignRelation()->attributeValue( row, *fjoin ),
                       foreignRelation()->attributeValue( row, *fview ) );
    }
    return row;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QmvAttribute::copyTextMeta & QmvAttribute::copyText() const
{
        return copy_text;
}

// ----------------------------------------------------------------------
//! Return the \em label associated with \em value
/*!


  \param        value  
  \return       label
*/
// ----------------------------------------------------------------------
const QString QmvAttribute::getLabelByValue( const QString & value )
{
    QString * ptr = value_dict.find( value );
    qDebug("QmvAttribute::getLabelByValue:value=%s label=%s", value.latin1(), (ptr)?(*ptr).latin1():"" );
    if ( !ptr )
    {
            // Try extending the list
        extendList( value, FALSE, FALSE, FALSE );
            // then retry
        ptr = value_dict.find( value );
        qDebug("QmvAttribute::getLabelByValue:value=%s label=%s", value.latin1(), (ptr)?(*ptr).latin1():"" );
    }
    
    if ( ptr )
        return *ptr;
    else
        return QString::null;
}


// ----------------------------------------------------------------------

//! Build an SQL query for the Foreign Tuple.
/*!
  Using \e value, return an SQL select statement for the foreign tuple
  reference by \e value

  \param value          the foreign key
  \return <ReturnValue> SQL select statement.
*/
// ----------------------------------------------------------------------
const QString QmvAttribute::fkQueryString( const QString & value ) const
{
    QString * fkey  = metaForeign()->find( "mtfn_key" );
    QString * fjoin = metaForeign()->find( "mtfn_join" );
    QString * fview = metaForeign()->find( "mtfn_view" );
    if ( fkey && fjoin && fview && foreignRelation() )
        return  QString( "select * from %1 where %2 = '%3'" )
            .arg(foreignRelation()->relationName())
            .arg( *fjoin )
            .arg( QmvClass::cleanForSQL(value) );
    else
        return QString::null;
}

// ----------------------------------------------------------------------
//! Return the \em user label associated with \em value
/*!

  \param        value  Foreign Key value
  \return       label  Associated tuple label.
*/
const QString QmvAttribute::getUserLabelByValue( const QString & value )
{
    QString query = fkQueryString( value );

    QString label;
    if ( query.length() && foreignRelation() && foreignRelation()->open( query ) == 1 )
        label = foreignRelation()->userLabelValue(0);

    return label;
}


// ----------------------------------------------------------------------
//! Clear the value/label lists
/*!
  

*/
void QmvAttribute::clearList()
{
    
    label_dict.setAutoDelete( TRUE );
    label_dict.clear();
    value_dict.setAutoDelete( TRUE );
    value_dict.clear();
    label_list.clear();
    
}

// ----------------------------------------------------------------------
//! Set the value list
/*!
  Set the value list for this attribute to the value/label pairs specified in \em values.
  The QString \em values consists of pairs separated by \em sep1. Within each pair, the
  value and label are separated by \em sep2.

  \param values         List of value/label pairs
  \param sep1           Inter-pair separator (pairs)
  \param sep2           Intra-pair separator (value/label withing pair)
*/
void QmvAttribute::setList( const QString & values, const QString & sep1, const QString & sep2 )
{

    clearList();

    QStringList pairs = QStringList::split( sep1, values );
    int index = 0;
    for ( int i = 0; i < pairs.count(); i++ )
    {
        QStringList itempair = QStringList::split( sep2, pairs[i] );
        label_list += itempair[1];
        label_dict.insert( itempair[1], new QString( itempair[0] ) );
        value_dict.insert( itempair[0], new QString( itempair[1] ) );
    }
    if ( autosortlist )
        label_list.sort();

}

// ----------------------------------------------------------------------
//! Add value/label pair to combo list.
/*!
  Add a new value/label pair to the list if if does not already exist.
  Sort and rebuild the indexes.

  \param value  
  \param label  
  \return <ReturnValue>
*/
bool QmvAttribute::addToList( const QString & value, const QString & label )
{
    QString x_value = value;
    QString x_label = label;
    
        // no null values;
    if ( x_value.length() < 1 )
        return -1;
    
        // if no label, then use the value as a label
    if ( x_label.length() < 1 )
        x_label = x_value;

        // if any of this exists, then replace
    while ( QString * ptr = label_dict.find( x_label ) )
    {
        QString copy_value = QString( *ptr );
        label_dict.remove( x_label );
        value_dict.remove( copy_value );
    }
    while ( QString * ptr = value_dict.find( x_value ) )
    {
        QString copy_label = QString( *ptr );
        value_dict.remove( x_value );
        label_dict.remove( copy_label );
    }
        
    qDebug("QmvAttribute::addToList(%s, %s):%s->%s", value.latin1(), label.latin1(), x_value.latin1(), x_label.latin1() );
    
    label_dict.insert( x_label, new QString( x_value ) );
    value_dict.insert( x_value, new QString( x_label ) );
        // rebuild label_list - simple append not valid
    label_list.clear();
    QDictIterator<QString> itl( label_dict );
    while ( itl.current() )
    {
        label_list += itl.currentKey();
        ++itl;
    }
        // always sort - since the list is not built in any order
    label_list.sort();


    return true;
}

         
// ----------------------------------------------------------------------
const QmvAttribute * QmvAttribute::metaForeign() const
{
    return foreign_metadata;
}

// ----------------------------------------------------------------------
QmvClass * QmvAttribute::foreignRelation() const
{
    return foreign_relation;
}

// ----------------------------------------------------------------------
const QmvClass * QmvAttribute::foreignRelation( const QString fk_value ) const
{
    if ( !foreignRelation() )
        return 0;
    
    QString query = fkQueryString( fk_value );

    if ( query.length() && foreignRelation()->open( query ) == 1 )
        return foreignRelation();
    else
        return 0;
}

// ----------------------------------------------------------------------
//! Set the foreign metadata
/*!
  Set the pointer to the metadata describing the foreign relationship of
  this attribute.

  \param fkmeta         a pointer to the metadata of the fk relation

*/
void QmvAttribute::setForeign( QmvAttribute const * fkmeta, QmvClass * fkrel )
{
    foreign_metadata = fkmeta;
    foreign_relation = fkrel;
    input = List;               // use list for input method

}

// ----------------------------------------------------------------------
//! Write an XML version of the metadata
/*!
  Write an XML version of key metadata of this attribute to the QTextStream
  object \em xml_ts.

  \param xml_ts         QTextStream object
  \return               True/False ( Success/Failure )
*/
bool QmvAttribute::writeXMLDeclaration( QTextStream & xml_ts )
{
    
    xml_ts << "<attribute>";
    xml_ts << "<name>" << attributeName() << "</name>";
    xml_ts << "<type>" << attributeType() << "</type>";
    xml_ts << "<Title>" << attributeTitle() << "<Title/>";
    xml_ts << "</attribute>";

    return true;
}

// ----------------------------------------------------------------------
//! Write an XML version of the attribute data \em data_str.
/*!
  Write an XML version of the attribute data \em data_str the QTextStream
  object \em xml_ts.

  \param xml_ts         QTextStream object
  \paran data_str       The attribute data.
  \return <ReturnValue>
*/
bool QmvAttribute::writeXMLData( QTextStream & xml_ts, const QString & data_str )
{
    xml_ts << "<data attribute=\"" << attributeName() << "\">" << data_str << "</data>";

    return true;
}

// ----------------------------------------------------------------------
//! print all metadata for an attribute
/*!

  Print all the meta-data for the attribute

*/
void QmvAttribute::print()
{
    QDictIterator<QString> it_mt(*this);
    while (it_mt.current())
    {
        cout << it_mt.currentKey() << "|" << *it_mt.current() << endl;
        ++it_mt;
    }
        // print the labellist
    cout << "QmvAttribute::print():Label-List:" << endl;
    QStringList tmp = getLabelList();
    int index = 0;
    for ( QStringList::Iterator it = tmp.begin(); it != tmp.end(); ++it )
        cout << index++
             << ":"
             << (*it)
             << endl;
    
    cout << "QmvAttribute::print()::Data type " << dataType()  << endl;
    cout << "QmvAttribute::print()::Input Method " << inputMethod()  << endl;
    cout << "QmvAttribute::print()::Access Method " << accessMethod()  << endl;
        // Foreign metatdata and foreign relation
    cout << "QmvAttribute::print()::Foreign Metadata " << (int) metaForeign()  << endl;
    cout << "QmvAttribute::print()::Foreign Relation " << (int) foreignRelation()  << endl;

        // label dictionary
    cout << "QmvAttribute::print():Label-dict:" << endl;
    index = 0;
    QDictIterator<QString> itl( label_dict );
    while ( itl.current() )
    {
        cout << index++
             << ":"
             << itl.currentKey().latin1()
             << " -> "
             << itl.current()->latin1()
             << endl;
        ++itl;
    }

        // iterator for value dictionary
    cout << "QmvAttribute::print():Value-dict:" << endl;
    index = 0;
    QDictIterator<QString> itv( value_dict );
    while ( itv.current() )
    {
        cout << index++
             << ":"
             << itv.currentKey().latin1()
             << " -> "
             << itv.current()->latin1()
             << endl;
        ++itv;
    }
        
}


// ----------------------------------------------------------------------
//! Print a single meta-attribute
/*!
  
  Print a single meta-datum for this attribute.

  \param name   the meta-attribute to print
*/
// ----------------------------------------------------------------------
void QmvAttribute::print(QString name)
{
    QString *value = this->find(name);
    
    if(value)
        cout << name << "|" << value << endl;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvAttribute::addAlert( const int priority, const operator_types op, const QString & value, const QString & colour )
{
    alertItem * al = new alertItem;
    al->priority = priority;
    al->op = op;
    al->value = value;
    al->colour = colour;
    
        // convert to presentation form #XXXXXX
    bool ok;
    uint rgba = al->colour.replace( QRegExp("#"), "").toUInt( &ok, 16);
    if (ok)
    {
        rgba %= 0x1000000; // strip alpha value
        al->colour = QString::number(rgba, 16).rightJustify(6, '0');
        al->colour.prepend("#");
    }
            
    alert_list.inSort(al);
}


// ----------------------------------------------------------------------
//! Test for alert
/*!
  test the \em compare value with registered alerts. Return an invalid
  \ref QColor if no alerts match, or a valid \ref QColor if an alert
  is set.

  \param compare        value to test
  \return <ReturnValue> QColor - valid if set, else invalid
*/
// ----------------------------------------------------------------------
QColor QmvAttribute::alert( const QString & compare )
{
    if ( type == Timestamp )
    {
        return alert( QmvClass::strToDateTime( compare ));
    }

        // lists/fk without alerts.
    if ( input == List && alert_list.count() < 1 )
        return(QColor("yellow"));
    
    for ( int i = 0; i < alert_list.count(); i++ )
    {
        alertItem * ai = alert_list.at(i);
        switch (ai->op)
        {
            case Matches:
                if ( compare.find( QRegExp(ai->value)) == 0 )
                    return QColor(ai->colour);
                break;
            case Like:
                if ( (compare.lower()).find( QRegExp(ai->value.lower())) == 0 )
                    return QColor(ai->colour);
                break;
            case Contains:
                if ( (compare.lower()).find( QRegExp(ai->value.lower())) >= 0 )
                    return QColor(ai->colour);
                break;
            case Equals:
                if ( compare == ai->value )
                    return QColor(ai->colour);
                break;
            case GreaterThan:
                if ( compare > ai->value )
                    return QColor(ai->colour);
                break;
            case LessThan:
                if ( compare < ai->value )
                    return QColor(ai->colour);
                break;
            case GreaterThanOrEqual:
                if ( compare >= ai->value )
                    return QColor(ai->colour);
                break;
            case LessThanOrEqual:
                if ( compare <= ai->value )
                    return QColor(ai->colour);
                break;
            case NotEqual:
                if ( compare != ai->value )
                    return QColor(ai->colour);
                break;
            case All: 
                return QColor(ai->colour);   
                break;
        }
    }
    return QColor();
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
QColor QmvAttribute::alert( const QDateTime & compare )
{
    QDateTime value;

    for ( int i = 0; i < alert_list.count(); i++ )
    {
        alertItem * ai = alert_list.at(i);
        value = QmvClass::strToDateTime( ai->value );
        switch (ai->op)
        {
            case Matches:
            case Like:
            case Contains:
            case Equals:
                if ( compare == value )
                    return QColor(ai->colour);
                break;
            case GreaterThan:
                if ( compare > value )
                    return QColor(ai->colour);
                break;
            case LessThan:
                if ( compare < value )
                    return QColor(ai->colour);
                break;
            case GreaterThanOrEqual:
                if ( compare < value )
                    return QColor(ai->colour);
                break;
            case LessThanOrEqual:
                if ( compare <= value )
                    return QColor(ai->colour);
                break;
            case NotEqual:
                if ( compare != value )
                    return QColor(ai->colour);
                break;
            case All: 
                return QColor(ai->colour);   
                break;
        }
    }
    return QColor();
}
