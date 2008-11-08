/*
 *     $Id: qmvclass.cpp 2889 2006-06-11 11:47:27Z source $
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
 * DESCRIPTION: 
 *
 * ======================================================================
 */


/*!
  \file   qmvclass.cpp
  \brief  Source for the QmvClass class

  A class to manage persistent data class objects .

  \author Rex McMaster, rmcm@compsoft.com.au
  \date   2000-11-02
*/

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <stdlib.h>
#include <unistd.h>
#include <qmainwindow.h>
#include <qapplication.h>
#include <qdict.h>
#include <qarray.h>
#include <qtable.h>
#include <qmessagebox.h>
#include <qscrollview.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qdatetime.h>
#include "qmvclass.h"
/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
static char qmvclass_version[] = "$Id: qmvclass.cpp 2889 2006-06-11 11:47:27Z source $";

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
//! Constructor
/*!

  Constructs a data class object, using metadata known to the parent
  qmvDB object.

  \param db             Parent database object
  \param name           Data class name
  \param rellevel       Limit level of auto load of relations classes. Defaults to 0.
*/

QmvClass::QmvClass( QmvDB * db, const QString name, const int rellevel )
        : QObject( db, name ),
          database(db)
{

}

//! Copy Constructor
/*!
  Creates a copy of \em rel. Tuple list (loaded_tuples), query
  and query handle are not copied.

  \param rel    
*/

QmvClass::QmvClass( const QmvClass & rel )
        : QObject( rel.parent(), rel.name() )
{
    database = rel.database;
    class_name = rel.class_name;
    child_relations = rel.child_relations;
    class_metadata = rel.class_metadata;
    attribute_metadata = rel.attribute_metadata;
    foreign_metadata = rel.foreign_metadata;
    relation_metadata = rel.relation_metadata;
    foreign_relations = rel.foreign_relations;
    active_tuple_count = 0;
}

QmvClass::~QmvClass()
{
        // remove the foreign relation objects
    foreign_relations.setAutoDelete( TRUE );     // delete items when they are removed
    foreign_relations.clear();
        // remove the child relation objects
    child_relations.setAutoDelete( TRUE );
    child_relations.clear();
}

// ----------------------------------------------------------------------
//! Create persistent default metadata
/*!

  Create a set of meta data for a class. This includes meta-class,
  meta-attribute, meta-foreign and meta-relation datasets. The
  information is scrounged from the database system tables by the SQL
  function create_metadata().

  \return true on success and false if any errors
*/

const bool QmvClass::createMetadata( const QString name )
{

}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
QmvDB * QmvClass::dataBase() const
{
    return database;
}

//! Return pointer to attribute metadata for this class.
/*!

*/
QmvMetaDict * QmvClass::attributeMetadata()
{
    return & attribute_metadata;
}

//! Return pointer to foreign class metadata for this class.
/*!

*/
QmvMetaDict * QmvClass::foreignMetadata()
{
    return & foreign_metadata;
}

//! Return pointer to child relation class  metadata for this class.
/*!

*/
QmvMetaDict * QmvClass::relationMetadata()
{
    return & relation_metadata;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvClass::classMetaDataAttribute( const QString & attname) const
{
    QString str;
    QString *pt = class_metadata.find(attname);
    if (pt)
        str = *pt;

    return str;
}
// ----------------------------------------------------------------------
//! the name of this class
/*!

   A distinct name ( eg might map to an SQL table or view )

   \sa relationTitle

*/
// ----------------------------------------------------------------------
const QString QmvClass::relationName() const
{
    return classMetaDataAttribute("mtcl_name");
}

// ----------------------------------------------------------------------
//! the title of this class
/*!

  A descriptive title for this object.

  \sa relationName

*/
// ----------------------------------------------------------------------
const QString QmvClass::relationTitle() const
{
    return classMetaDataAttribute("mtcl_title");
}

// ----------------------------------------------------------------------
//! the serial key for this class
/*!

  A unique tuple key - a serial integer.

*/
// ----------------------------------------------------------------------
QString const QmvClass::serialKey() const
{
    QString serialname = QString("%1__sequence").arg(relationName());
    QmvAttribute * ptr = attribute_metadata.find( serialname );    
    if (ptr)
        return( serialname );
    

    return QString::null;
}

// ----------------------------------------------------------------------
//! the primary key for this class
/*!

  A unique tuple key.

*/
// ----------------------------------------------------------------------
const QString QmvClass::primaryKey() const
{
    return classMetaDataAttribute("mtcl_primary");
}

// ----------------------------------------------------------------------
//! the user key for this class
/*!

  A user tuple key.

*/
// ----------------------------------------------------------------------
const QString QmvClass::userKey() const
{
    QString * pk = class_metadata.find("mtcl_userkey");
    if (pk && pk->length())
        return(*pk);
    else
        return primaryKey();
}

// ----------------------------------------------------------------------
//! the application group for this class
/*!

  A user tuple key.

*/
// ----------------------------------------------------------------------
const QString QmvClass::groupName() const
{
    return classMetaDataAttribute("mtcl_group");
}

// ----------------------------------------------------------------------
//! the user label for this class
/*!

  A tuple label. If this is not specified, the name of the primary-key is
  used. If the label list is compound, just return the first member.

*/
// ----------------------------------------------------------------------
const QString QmvClass::userLabel() const
{
    QStringList ul_list = userLabelList();
    if ( ul_list.count() )
            // if multiple list members, the first is a format string
        if ( ul_list.count() == 1 )
            return ul_list[0];
        else
            return ul_list[1];
    else
        return primaryKey();
}

// ----------------------------------------------------------------------
//! the user label list for this class
/*!

  A tuple label list. If this is not specified, the name of the primary-key is
  used.

*/
// ----------------------------------------------------------------------
const QStringList QmvClass::userLabelList() const
{
    QStringList ul_list;
    QString ul = QString(*class_metadata.find("mtcl_userlabel"));
    ul.simplifyWhiteSpace();
    ul.replace( QRegExp(" *, *"), "," );
    ul_list = QStringList::split( QRegExp(","), ul );
    if ( !ul_list.count() )
        ul_list.append( primaryKey() );
    
    return ul_list;
}

// ----------------------------------------------------------------------
//! the user label value for a tuple
/*!

  A tuple label.

*/
// ----------------------------------------------------------------------
const QString QmvClass::userLabelValue( int row )
{
    const QmvTuple * ta = tupleAt( row );
    QString ulv;
    
    if ( ta )
        ulv = ta->userLabelValue();

    return ulv;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvClass::currentQuery() const
{
    return current_query;
}


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvClass::access() const
{
    return classMetaDataAttribute("mtcl_access");
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvClass::group() const
{
    return classMetaDataAttribute("mtcl_group");
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvClass::matrixOrder() const
{
    return classMetaDataAttribute("mtcl_matrix_order");
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvClass::orderBy() const
{
    return classMetaDataAttribute("mtcl_order_by");
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvClass::groupBy() const
{
    return classMetaDataAttribute("mtcl_group_by");
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvClass::queryLimit() const
{
    return classMetaDataAttribute("mtcl_query_limit");
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvClass::queryOffset() const
{
    return classMetaDataAttribute("mtcl_query_offset");
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
int QmvClass::loadForeignRelations( const QStringList & fkrel, const int limit )
{
    return 0;
}

// ----------------------------------------------------------------------
//! Return a QStringList of foreign relation names.
/*!
  Return a QStringList of the names of foreign relations associated with
  this relation. Only one relationname is reported where more than one foreign
  key references the foreign relation ( ie foregn relations objects are shared )

  \return QStringList of foreign relation names.
*/
// ----------------------------------------------------------------------
QStringList QmvClass::listForeignRelations()
{
    QStringList frl;
    QDictIterator<QmvClass> it(foreign_relations);
    while ( it.current() )
    {
        frl += it.currentKey();
        ++it;
    }
    return( frl );
}

// ----------------------------------------------------------------------
//! Return a QStringList of foreignKey attribute-names.
/*!
  Return a QStringList of the names of foreign-key attributes in this
  relation.

  \return QStringList of foreign-key names.
*/
// ----------------------------------------------------------------------
QStringList QmvClass::listForeignKeys()
{
    QStringList frl;
    QDictIterator<QmvAttribute> it(foreign_metadata);
    while ( it.current() )
    {
        frl += it.currentKey();
        ++it;
    }
    return( frl );
}

// ----------------------------------------------------------------------
//! the foreign relation object for relation \em name
/*!
  Returns 0 if a relation object for \em name is not found
  
  \param name   the name of the foreign relation
*/
// ----------------------------------------------------------------------
QmvClass * QmvClass::foreignRelation( const QString &name ) const
{
    QmvClass * fr = foreign_relations.find( name );
    if (fr)
        return fr;
    else
        return 0; 
}

// ----------------------------------------------------------------------
//! Locate a foreign relation attribute 
/*!
  Locate the value of a foreign relation attribute, \em fratt, for the
  given local attribute, \em attname. Return a null QString if not found.

  \param attname                the local attribute name
  \param fratt                  the foreign relation attribute
  \return                       the value of the foreign relation attribute or null
                                length QString.
*/
// ----------------------------------------------------------------------
const QString QmvClass::foreignRelationAttribute( const QString & attname, const QString & fratt ) const
{
        // find the attribute object for this attribute
    QmvAttribute * frel_att = foreign_metadata.find( attname );
    if ( !frel_att )
        return QString::null;
        // find the requested foreign attribute
    QString * frelatt = frel_att->find( fratt );
    if ( !frelatt )
        return QString::null;
    
    return *frelatt;
}

// ----------------------------------------------------------------------
//! Return the foreign relation name for this attribute
/*!
  Return the name of the foreign relation association with the attribute
  name \em attname. If \em attname is not a foreign key then return a
  zero-length string.

  \param attname                the name of an attribute        
  \return                       the name of the foreign relation
*/
// ----------------------------------------------------------------------
const QString QmvClass::foreignRelationName( const QString & attname ) const
{
    return foreignRelationAttribute( attname, "mtfn_other_class");
}

// ----------------------------------------------------------------------
//! Return the foreign key attribute for this attribute
/*!
 Return the name of the foreign key attribute in the foreign relation named
 referenced by \em attname.

  \param attname        the name of the foreign relation attribute
  \return               the name of the join attribute in the foreign relation
*/
// ----------------------------------------------------------------------
const QStringList QmvClass::foreignKey( const QString &attname ) const
{
    return QStringList().split( QRegExp(",\\s*"), foreignRelationAttribute( attname, "mtfn_key" ) );
}

// ----------------------------------------------------------------------
//! Return the foreign join attribute for this attribute
/*!
 Return the name of the join attribute in the foreign relation named
 referenced by \em attname

  \param attname        the name of the foreign relation attribute
  \return               the name of the join attribute in the foreign relation
*/
// ----------------------------------------------------------------------
const QStringList QmvClass::foreignJoin( const QString & attname ) const
{
    return QStringList().split( QRegExp(",\\s*"), foreignRelationAttribute( attname, "mtfn_join" ) );
}

// ----------------------------------------------------------------------
//! Return the foreign view attribute for this attribute
/*!
 Return the name of the view attribute in the foreign relation named 
 referenced by \em attname.

  \param attname        the name of the foreign relation attribute
  \return               the name of the view attribute in the foreign relation
*/
// ----------------------------------------------------------------------
const QString QmvClass::foreignView( const QString & attname ) const
{
    return foreignRelationAttribute( attname, "mtfn_view" );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvClass::selectForeignQuery( const QString & attname,
                                            QStringList localvalue, int local ) const
{
   
    QString query = QString( "select * from %1")
        .arg( foreignRelationName( attname ) );
    if ( local )
    {
        QStringList fkeys = foreignJoin( attname);
        QStringList::Iterator itk = fkeys.begin();
        QStringList::Iterator itv = localvalue.begin();
        QString conjunctive = "where";
        while( itk != fkeys.end() && itv != localvalue.end() )
        {
            query += QString( " %1 %2 = '%3'::%4")
                .arg( conjunctive )
                .arg( *itk )
                .arg( *itv )
                .arg( attribute( attname )->attributeType() );
            conjunctive = "and";
            ++itk;
            ++itv;
        }
    }
    
    qDebug("QmvClass::selectForeignQuery:%s", query.latin1() );
    
    return query;
    
}

// ----------------------------------------------------------------------
//! the title for a child relation
/*!
  Return the title for the child relation \em name within the context of this parent.

  \param name   
  \return QString title
*/
// ----------------------------------------------------------------------
QString const QmvClass::childTitle( const QString &name ) const
{
    QmvAttribute * at = relation_metadata.find(name);
    if ( at )
        return *at->find( "mtrl_title");
    else
        return 0;
}

// ----------------------------------------------------------------------
//! Return the QmvAttribute object of the named attribute.
/*!
  Return a reference to the QmvAttribute object associated with
  \em attname.

  \param attname        the attribute name
  \return               the QmvAttribute object
*/
// ----------------------------------------------------------------------
const QmvAttribute * QmvClass::attribute( const QString & attname ) const
{
    QmvAttribute * ptr = attribute_metadata.find( attname );
    return ptr;

}

// ----------------------------------------------------------------------
//! Return the attribute name associated with \em att_title
/*!
  Note that if more than one attribute has the title \em att_title, the
  first located attribute name is returned.
  
  \param att_title      Title of attribute
  \return               attribute name ( null if not found)
*/
// ----------------------------------------------------------------------
const QString QmvClass::attributeNamebyTitle( const QString & att_title )
{
    QDictIterator<QmvAttribute> it(attribute_metadata);
    for ( ; it.current(); ++it )
        if ( attAttValue( it.currentKey(), "mtat_title") == att_title )
            return( it.currentKey() );

    return QString("");
}

// ----------------------------------------------------------------------
//! Meta-attribute value
/*!

  Returns the value of the specified meta-attribute of the specified
  tuple attribute. attAttPrint will print a list of
  meta-attributes.

  \param attname        pointer to the attribute name
  \param attattname     pointer to the meta-attribute name

  \sa attAttPrint
  
*/
// ----------------------------------------------------------------------
QString QmvClass::attAttValue(const QString &attname, const QString &attattname)
{
        // extract the value of an attribute attribute
    QmvAttribute *att = attribute_metadata.find(attname);
    if (att)
    {
        QString *attatt = att->find(attattname);
        if (attatt)
            return(*attatt);
        else
            return(0);
    } else
        return(0);
}


// ----------------------------------------------------------------------
//! Sorted Attribute List
/*!
  Return a QmvAttList of attributes, sorted by \em index, and consisting of
  attributes with names in \em atnames.

  \param index          An attribute-attribute name for attributes sorting
                        ( eg attribute_browse_order or attibute_view_order )
  \param atnames        A \ref QStringList of attribute names

  \return               A sorted attribute list
  
*/
// ----------------------------------------------------------------------
QmvAttList QmvClass::sortedAttlist( QString const & index, QStringList & atnames )
{
        // The mtat class is required for meta-meta info
    QmvClass * reln_mtat = database->dbMetaRelation("mtat");
    if ( !reln_mtat )
        return QmvAttList(); // blank list;
    
    QmvAttribute::data_type index_dtype;
    bool sortflag = true;
    const QmvAttribute * at_index = reln_mtat->attribute(index);
    if ( at_index )
        index_dtype = at_index->dataType();
    else
    {
        sortflag = false;
        index_dtype = QmvAttribute::TypeUnknown;
    }
    
    
        // Determine the internal data type - required for sort
    QmvAttList::cfDataTypes cftype;
    switch ( index_dtype )
    {
        case QmvAttribute::Integer:
            cftype = QmvAttList::Integer;
            break;
            
        case QmvAttribute::Numeric:
            cftype = QmvAttList::Double;
            break;
            
        case QmvAttribute::Timestamp:
        case QmvAttribute::TypeUnknown:
        case QmvAttribute::Text:
            cftype = QmvAttList::String;
            break;
                
        default:
            cftype = QmvAttList::Unknown;
            break;
    }

        // Create a new list
    QmvAttList attlist( index, cftype );

        // Scan the attribute names

    for ( int i = 0; i < atnames.count(); i++ )
    {
        const QmvAttribute * at;
        at = attribute( atnames[i] );
        if ( at )
            attlist.append( at );
    }

        // sort the list
    if ( sortflag )
        attlist.sort();
    
        // return the list
    return( attlist );

}


// ----------------------------------------------------------------------
//! Sorted Attribute List
/*!
  Return a QmvAttList of attributes, sorted by \em index, and filter by \em filter.

  \param index          An attribute-attribute name for attributes sorting
                        ( eg attribute_browse_order or attibute_view_order )
  \param filter         One or more flags to be applied as filter - members of the
                        enum \ref AttlistFilter can be OR'd together.
*/
// ----------------------------------------------------------------------
QmvAttList QmvClass::sortedAttlist( QString const & index, int filter )
{
    QStringList atnames;
    
        // Scan the attribute meta data, and apply the filter(s)
    QDictIterator<QmvAttribute> it(attribute_metadata);
    for ( ; it.current(); ++it )
    {
        qDebug( "sortedAttlist:at = %s",
                attAttValue(it.currentKey(), QString("mtat_title")).latin1() );
            // Skip null and zero length
        if ( filter & NoBlank )
            if ( attAttValue( it.currentKey(), index ).length() == 0 ||
                 attAttValue( it.currentKey(), index ).isNull() )
                continue;
        if ( filter & NoZero )
            if ( attAttValue(it.currentKey(), index).toInt() == 0 )
                continue;
        if ( filter & NoNeg )
            if ( attAttValue(it.currentKey(), index).toInt() < 0 )
                continue;
        if ( filter & NoPos )
            if ( attAttValue(it.currentKey(), index).toInt() > 0 )
                continue;
        if ( filter & NoReadOnly )
            if ( attAttValue( it.currentKey(), "mtat_input_method") == "RO" )
                continue;
        if ( filter & NoSystem )
            if ( attAttValue( it.currentKey(), "mtat_input_method") == "SYSTEM" )
                continue;
            // Add this attribute to the list
        atnames += attAttValue( it.currentKey(), "mtat_name");
    }

    return( sortedAttlist( index, atnames ) );
}


// ----------------------------------------------------------------------
//! Editable Attributes
/*!
  Return a QmvAttList of editable attributes, sorted by mtat_view_order.

  /ref sortedAttlist()
  
*/
// ----------------------------------------------------------------------
QmvAttList QmvClass::edit_attlist()
{
    return(sortedAttlist( "mtat_view_order", NoBlank|NoReadOnly|NoSystem ) );
}

// ----------------------------------------------------------------------
//! Viewable Attributes
/*!
  Return a QmvAttList of viewable attributes, sorted by mtat_view_order.

  /ref sortedAttlist()
  
*/
// ----------------------------------------------------------------------
QmvAttList QmvClass::view_attlist()
{
    return(sortedAttlist( "mtat_view_order", NoBlank ) );
}

// ----------------------------------------------------------------------
//! \em StringList of attribute names
/*!
  Return a StringList of attribute names selected by \em group ( enum \em ListScope )

  \param group          enum \em ListScope
  \return               a StringList of attribute names
*/
// ----------------------------------------------------------------------
QStringList QmvClass::attributeList( const int group )
{
    QStringList attlist;
    QDictIterator<QmvAttribute> it(attribute_metadata);
    
    switch ( group )
    {
            // All attributes in relation
        case AttAll:
            while ( it.current() )
            {
                attlist += it.current()->attributeName();
                ++it;
            }
            break;
            
                // Viewable Attributes
        case AttViewable:
            while ( it.current() )
            {
                if ( attAttValue(it.currentKey(), "mtat_view_order").toInt() > 0 &&
                     attAttValue(it.currentKey(), "mtat_input_method") != QString( "SYSTEM" ) )
                    attlist += it.current()->attributeName();
                ++it;
            }
            
            break;
            
                // Editable attributes
        case AttEditable:
            while ( it.current() )
            {
                if ( attAttValue(it.currentKey(), "mtat_view_order").toInt() > 0 &&
                     attAttValue(it.currentKey(), "mtat_input_method") != QString( "RO" ) &&
                     attAttValue(it.currentKey(), "mtat_input_method") != QString( "SYSTEM" ) )
                    attlist += it.current()->attributeName();
                ++it;
            }
            break;
            
        default:
            break;
    }
    return ( attlist );
}

// ----------------------------------------------------------------------
//! Return an attribute list for SQL SELECT.
/*!
  This method supports virtual attributes via the EXPRESSION AS ALIAS forms.

  \return       A \ref QString attribute list for use with SQL SELECT
*/
// ----------------------------------------------------------------------
const QString QmvClass::SQLSelectList()
{
        // get a list of all attributes.
    QmvAttList atlist = sortedAttlist( "mtat_view_order", 0);

        // in case nothing is returned, default to SQL *
    if (atlist.count() < 1)
        return "*";
    
    QStringList sl;
    
    for ( QmvAttribute * at_ptr = atlist.first(); at_ptr != 0; at_ptr = atlist.next())
    {
        if ( at_ptr->inputMethod() == QmvAttribute::Virtual )
            sl << QString("%1 as %2").arg(at_ptr->virtualAttribute()).arg(at_ptr->attributeName());
        else
            sl << at_ptr->attributeName();
    }
    return sl.join(",");
}


// ----------------------------------------------------------------------
//! Load the child relations of this relation
/*!
 

  \param        rellevel       depth of load

*/
// ----------------------------------------------------------------------
void QmvClass::loadChildRelations( const int rellevel )
{

    child_relations.setAutoDelete( TRUE );
    child_relations.clear();
    QDictIterator<QmvAttribute> it(relation_metadata);
    
    while ( it.current() )
    {
            // look for an existing implementation of this class.
        QmvClass * existing_cls = database->dbClasses()->find( it.currentKey() );
        if ( existing_cls )
                // Copy
            child_relations.insert( it.currentKey(), new QmvClass( *existing_cls ));
        else
                // New
            child_relations.insert( it.currentKey(), new QmvClass( database,  it.currentKey(), rellevel));
        
        qDebug("loaded relation %s -> %s", relationName().latin1(), it.currentKey().latin1());
        ++it;
    }
}

// ----------------------------------------------------------------------
//! Return a list of child relation names
/*!
  <long-description>

  \return       QStringList of names of child relations.
*/
// ----------------------------------------------------------------------
QStringList QmvClass::listChildRelations()
{
            
    QStringList crl;
    QDictIterator<QmvAttribute> it(relation_metadata);
    while ( it.current() )
    {
        crl += it.currentKey();
        ++it;
    }
    return( crl );
}

// ----------------------------------------------------------------------
//! the child relation object for relation \em name
/*!
  Returns 0 if a relation object for \em name is not found
  
  \param name   the name of the child relation
*/
// ----------------------------------------------------------------------
QmvClass * QmvClass::childRelationByName( const QString &name ) const
{
    QmvClass * fr = child_relations.find( name );
    return fr;
}

// ----------------------------------------------------------------------
//! Locate a child relation attribute 
/*!
  Locate the value of a child relation attribute, \em chatt, for the
  given local attribute, \em attname. Return a null QString if not found.

  \param attname                the local attribute name
  \param chatt                  the child relation attribute
  \return                       the value of the child relation attribute or null
                                length QString.
*/
// ----------------------------------------------------------------------
const QString QmvClass::childRelationAttribute( const QString & attname, const QString & chatt ) const
{
        // find the attribute object for this attribute
    QmvAttribute * chrel_att = relation_metadata.find( attname );
    if ( !chrel_att )
        return QString();
        // find the requested child attribute
    QString * chrelatt = chrel_att->find( chatt );
    if ( !chrelatt )
        return QString();
    
    return *chrelatt;
}

// ----------------------------------------------------------------------
//! Return the child relation name for this attribute
/*!
  Return the name of the child relation association with the attribute
  name \em attname. If \em attname is not a child key then return a
  zero-length string.

  \param attname                the name of an attribute        
  \return                       the name of the child relation
*/
// ----------------------------------------------------------------------
const QString QmvClass::childRelationName( const QString & attname ) const
{
    return childRelationAttribute( attname, "mtrl_other_class");
}

// ----------------------------------------------------------------------
//! Return the child key attribute for this attribute
/*!
 Return a list of the name(s) of the child key attribute(s) in the child relation named
 referenced by \em attname.

  \param attname        the name of the child relation attribute
  \return               the name(s) of the join attribute(2) in the child relation
*/
// ----------------------------------------------------------------------
const QStringList QmvClass::childKey( const QString &attname ) const
{
    return QStringList().split( QRegExp(",\\s*"), childRelationAttribute( attname, "mtrl_key" ) );
}

// ----------------------------------------------------------------------
//! Return the child join attribute for this attribute
/*!
 Return the name of the join attribute in the child relation named
 referenced by \em attname

  \param attname        the name of the child relation attribute
  \return               the name of the join attribute in the child relation
*/
// ----------------------------------------------------------------------
const QStringList QmvClass::childJoin( const QString & attname ) const
{
    return QStringList().split( QRegExp(",\\s*"), childRelationAttribute( attname, "mtrl_join" ) );
}

// ----------------------------------------------------------------------
//! Prepare a select statement for child relation.
/*!
  Returns an SQL select statement for the child relation named \em childname,
  where the parent key values are in the QStringList \em localvalue in the form
  \em {subkey-value sep subkey-value ...}

  \param childname      name of child relation
  \param localvalue     QStringList of parent key values
  \param sep            the subkey separator.
  \return               Select string.
*/
// ----------------------------------------------------------------------
const QString QmvClass::selectChildQuery( const QString & childname,
                                          QStringList & localvalue, const QString & sep ) const
{
    QmvClass * reln = childRelationByName( childname );
    if ( !reln )
    {
        QmvException * exception
            = new QmvException( QString( "<h2>Failed to locate child relation</h2>"
                                         "<ul><li>Database name: <b>%1</b>"
                                         "<li>Parent name: <b>%2 (%3)</b>"
                                         "<li>Child name: <b>%4</b></ul>" )
                                .arg( dataBase()->dbTitle() )
                                .arg( relationTitle() )
                                .arg( relationName() )
                                .arg( childname ),
                                QmvException::Warning );
        throw( exception );
    }
    
    QStringList joinlist = childJoin( childname );
    
    QString query = QString( "select * from %1")
        .arg( childname );

    QStringList::Iterator join = joinlist.begin();
    QStringList::Iterator value = localvalue.begin();
    QString conjunctive = "where";
    while ( join != joinlist.end() && value != localvalue.end() )
    {
            //QString type = reln->attribute( *join )->attributeType();
            //
        QString vlist = *value;
        vlist = cleanForSQL(vlist);
        QStringList keyvalues = QStringList().split( sep, vlist );
        query.append( QString( " %1 %2 in ('%3')")
                      .arg( conjunctive )
                      .arg( *join )
                      .arg( keyvalues.join("','") ) );
        conjunctive = "and";
        ++join;
        ++value;
    }
    query.append( QString( " %1" ).arg( reln->orderBy() ) );
    
    qDebug("QmvClass::selectChildQuery:%s", query.latin1() );
    
    return query;
    
}

const QDict<QmvClass> *  QmvClass::classRelations()
{
    return & child_relations;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
QmvTuple * QmvClass::createTuple( QmvTuple * existing )
{
    QmvTuple * tup;
    if ( existing )
        tup = new QmvTuple( existing );
    else
        tup = new QmvTuple(this);
    
    connect( tup, SIGNAL( tupleChanged( QmvTuple *  ) ),
             SIGNAL( tupleChanged( QmvTuple * ) ) );
    return tup;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvClass::deleteTuple( QmvTuple * tup )
{
    tup->disconnect();
    delete tup;
}
// ----------------------------------------------------------------------
//! Open a null dataset of <nrows> rows
/*!

  Create a null dataset of nrows rows. Initialize required number of tuples.
  
  \param nrows          the size of the dataset in tuples
  \return               Returns the size of dataset in tuples, or error (<0)
  
*/
// ----------------------------------------------------------------------
int QmvClass::open(int nrows)
{
  int i;
  QmvTuple *mt;
  QmvTupleAttribute *mta;
  QString this_class_name = relationName();
  
  loaded_tuples.clear();
  for (i = 0; i < nrows; i++)
    {
      // Load into class tuple storage
      mt = createTuple();
      // Now the QmvTuple is ready to insert in the dataset of the class
      loaded_tuples.append(mt);
    }
  current_query = QString("");
  emit relationChanged( NewBlankSet, nrows );
  return(nrows);
}

// ----------------------------------------------------------------------
//! Open a persistent dataset
/*!

  Create a dataset consisting of existing persistent data which is
  specified by query, an SQL query.
  

  \param query          SQL query string
  \return               The number of tuples in dataset

*/
// ----------------------------------------------------------------------
int QmvClass::open(const QString & query)
{
    int nrows = 0;
    emit relationChanged( NewBlankSet, nrows );
    return( nrows );
}
 
// ----------------------------------------------------------------------
//! Open a persistent dataset
/*!

  Create a dataset consisting of existing persistent data which is
  specified by query, an SQL query.
  

  \param query          SQL query string
  \return               The number of tuples in dataset

*/
// ----------------------------------------------------------------------
int QmvClass::open(const QString & query, QProgressDialog * controller )
{
    int nrows = 0;
    emit relationChanged( NewBlankSet, nrows );
    return( nrows );
}
 
// ----------------------------------------------------------------------
//! Return the current query string;
/*!

  \return current query string.
*/
// ----------------------------------------------------------------------
const QString QmvClass::queryDescription()
{
    return current_query;
}

 
// ----------------------------------------------------------------------
//! The number of rows in current dataset
/*!

  Returns the number of rows in the current dataset

  \return The number of rows in dataset.
*/
// ----------------------------------------------------------------------
uint QmvClass::count() const
{
    return( active_tuple_count );
}
 
// ----------------------------------------------------------------------
//! Return the row number by primary key
/*!
  Find a row in the current dataset using the primary key value.

  \param pk     the primary key value
  \return       row number or -1 if not found
*/
// ----------------------------------------------------------------------
int QmvClass::rowByPK( const QString & pk )
{
    if ( primaryKey().length() < 1 || pk.length() < 1 )
        return -1;
    
    for ( int i = 0; i < count(); i++)
        if ( attributeValue( i, primaryKey() ) == pk )
            return i;

    return -1;
}

// ----------------------------------------------------------------------
//! Return the row number by serial key
/*!
  Find a row in the current dataset using the serial key value.

  \param sk     the serial key value
  \return       row number or -1 if not found
*/
// ----------------------------------------------------------------------
int QmvClass::rowBySK( const QString & sk )
{
    if ( serialKey().length() < 1 || sk.length() < 1 )
        return -1;
    
    for ( int i = 0; i < count(); i++)
        if ( attributeValue( i, serialKey() ) == sk )
            return i;

    return -1;
}

// ----------------------------------------------------------------------
//! Return a tuple from loaded set
/*!

  Find a tuple by row number (rows number 0 ... ( count() - 1 ) inclusive.

*/
// ----------------------------------------------------------------------
QmvTuple * QmvClass::tupleAt(int row) 
{
    if (row >= 0 && row < count())
        return loaded_tuples.at(row);
    else
        return 0;
}


//! Return a tuple attribute
/*!
  Return a tuple attribute from \em row with the name \em attname

  \param row                    row number in current set
  \param attname                attribute name
  \return                       QmvAttribute or 0
*/
// ----------------------------------------------------------------------
QmvTupleAttribute * QmvClass::tupleAttribute( int row, const QString &attname )
{
    if (row >= 0 && row < count())
        return( tupleAt(row)->tupleAttribute(attname));
    else
        return 0;
}

// ----------------------------------------------------------------------
//! The current value of specified tuple attribute.
/*!
  
  Returns the value of the specified tuple attribute in the specified row.

  \param row            tuple row number (0 -> ( count() - 1 ) )
  \param attname        pointer to attribute name

  \return attribute value
*/
// ----------------------------------------------------------------------
const QString QmvClass::attributeValue( int row, const QString &attname )
{
    if (row >= 0 && row < count())
         return( QString(tupleAt(row)->attributeValue(attname)) );
    else
        return QString();
}


// ----------------------------------------------------------------------
//! Commit the current dataset to persistent storage.
/*!

  Commits changed attributes (see QmvTupleAttribute) of the tuples
  of the current set to persistent storage. If an error occurs during
  the storage of a tuple, the commit-process is aborted, and the number
  of successfully commited tuples is returned.

  \return the number of rows committed to persistent storage
*/
// ----------------------------------------------------------------------
int QmvClass::save()
{
    int i = 0;
        // do not call this->save() or signals will be duplicated
    for (i = 0; i < count(); i++ )
        if ( loaded_tuples.at(i)->save() == FALSE )
            return(i);
    
    emit relationChanged( SetChanged, i );
    return(i);
}

// ----------------------------------------------------------------------
//! Save a tuple of current dataset to persistent storage;
/*!

  Commit the specified tuple of the current dataset to persistent
  storage.
 
  \param row            The specified tuple number in dataset (0 -> ( count() - 1 ) )
  \return Success: TRUE/FALSE */
// ----------------------------------------------------------------------
bool QmvClass::save(int row)
{
    if ( row < 0 || row >= count() )
        return (FALSE);

    QApplication::setOverrideCursor( Qt::waitCursor );
    bool status = loaded_tuples.at(row)->save();
    QApplication::restoreOverrideCursor();
    
    if ( status == FALSE )
        return (FALSE);

    emit relationChanged( SingleChanged, row );
    return (TRUE);
}

bool QmvClass::save( QmvTuple * tup )
{
    qDebug("QmvClass::save: %d", loaded_tuples.count() );
    
    int row = loaded_tuples.findRef( tup );
    if ( row == -1 )
        return (FALSE);

    return ( save( row ) );
}

// ----------------------------------------------------------------------
//! Update the value of an attribute (shared method)
/*!
  Update the value of the specified attribute of the specified tuple.
  
  \param row    the tuple row number (0 -> ( count() - 1 ) )
  \param name   the tuple attribute name
  \param value  the new value to be applied to attribute
*/
// ----------------------------------------------------------------------
void QmvClass::update(int row, const QString &name, const QString &value)
{
  if (row >= 0 && row < count())
        loaded_tuples.at(row)->update(name, value);
}


// ----------------------------------------------------------------------
//! Insert a new tuple into this dataset
/*!

  Extend the current dataset by one tuple.

  \param newtuple      A new tuple to be inserted
  \return               the position of the new tuple
*/
// ----------------------------------------------------------------------
int QmvClass::newTuple()
{
        // should never be GT -  just paranoid
    if ( active_tuple_count >= loaded_tuples.count() )
            // need to extend
    {
        QmvTuple * tup = createTuple();
        loaded_tuples.append( tup );
    }
    
    active_tuple_count++;
    return( active_tuple_count - 1 );
}



// ----------------------------------------------------------------------
//! Copy an existing tuple into this dataset
/*!

  Duplicate the tuple at \em row to a new tuple. If \em autosave is TRUE then
  save the copied tuple to persistent storage.
  
  \param row            Existing tuple row
  \param autosave       TRUE/FALSE
  \return               the position of the new tuple
*/
// ----------------------------------------------------------------------
int QmvClass::copyTuple( int row )
{
    if ( row >= active_tuple_count )
        return -1;
    
    QmvTuple *existing = tupleAt( row );
    if ( !existing )
        return (-1);
    
    QmvTuple *nt = createTuple( existing );
    if ( !nt )
        return (-1);

        // remove an unused tuple to maintain buffer size
    if ( active_tuple_count < loaded_tuples.count() )
        deleteTuple( loaded_tuples.take(active_tuple_count) );
    
    int newrow = row+1;
    loaded_tuples.insert( newrow, nt );
    return( newrow );
}

// ----------------------------------------------------------------------
//! Remove a tuple from this dataset
/*!

  Remove the tuple at "row" in the current dataset.

  \param row            Row number (0 -> ( count() - 1 ) )
  \param rm_persistent  TRUE/FALSE to also remove associated persistent storage
                        (default is FALSE)
  \return               True/False ( Success/Failure )

*/
// ----------------------------------------------------------------------
bool QmvClass::remove(int row, bool rm_persistent )
{
    if (row >= 0 && row < count())
    {
        QmvTuple *tup = loaded_tuples.at(row);
        return remove(tup, rm_persistent );
    } else
        return false;
    
}

// ----------------------------------------------------------------------
//! Remove a tuple from this dataset
/*!

  Remove the tuple from the current dataset.

  \param tuple          The tuple to remove
  \param rm_persistent  TRUE/FALSE to also remove associated persistent storage
                        (default is FALSE)
  \return               True/False ( Success/Failure )

*/
// ----------------------------------------------------------------------
bool QmvClass::remove(QmvTuple * tuple, bool rm_persistent )
{
    if ( rm_persistent && !tuple->purge() )
            return false;
    
    loaded_tuples.removeRef(tuple);
    deleteTuple( tuple );
    active_tuple_count--;
    emit relationChanged( SetReduced, 1 );
    return true;
}

// ----------------------------------------------------------------------
//! Write an XML version of the metadata
/*!
  Write an XML version of metadata of this class to the QTextStream
  object \em xml_ts.

  \param xml_ts         QTextStream object
  \return               True/False ( Success/Failure )
*/
// ----------------------------------------------------------------------
bool QmvClass::writeXMLDeclaration( QTextStream & xml_ts )
{
    xml_ts << "<table content=\"declaration\">" << endl;
    xml_ts << "  <name>" << relationName() << "</name>" << endl;
    xml_ts << "  <title>" << relationTitle() << "</title>" << endl;
    xml_ts << "  <attributelist>" << endl;
    
    QDictIterator<QmvAttribute> it(attribute_metadata);
    while ( it.current() )
    {
        xml_ts << "    ";
        it.current()->writeXMLDeclaration( xml_ts );
        xml_ts << endl;
        ++it;
    }
    xml_ts << "  </attributelist>" << endl;
    xml_ts << "</table>" << endl;

    return true;
}

// ----------------------------------------------------------------------
//! Write an XML version of the data in the current set..
/*!
  Write an XML version of the class data to the QTextStream
  object \em xml_ts.

  \param xml_ts         QTextStream object
  \return <ReturnValue>
*/
// ----------------------------------------------------------------------
bool QmvClass::writeXMLData( QTextStream & xml_ts)
{
    xml_ts << "<table content=\"data\">" << endl;
    xml_ts << "  <name>" << relationName() << "</name>" << endl;
    xml_ts << "  <title>" << relationTitle() << "</title>" << endl;
    xml_ts << "  <tuplelist>" << endl;

    for ( int i = 0; i < active_tuple_count; i++ )
        loaded_tuples.at(i)->writeXMLData( xml_ts, "    ", "\n" );
    xml_ts << "  </tuplelist>" << endl;
    xml_ts << "</table>" << endl;
    
    return true;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvClass::htmlLabel( const int row )
{
    QmvTuple * tup = tupleAt(row);
    QString label;
    if ( tup )
        label = tup->htmlLabel();

    return label;
}

    
// ----------------------------------------------------------------------
//! Print the current dataset
/*!

  Print the current dataset.

*/
// ----------------------------------------------------------------------
void QmvClass::print()
{
    for ( int i = 0; i < active_tuple_count; i++ )
        loaded_tuples.at(i)->print();
}

// ----------------------------------------------------------------------
//! Print an attribute of the current dataset.
/*!

  Print a single attribute of the current dataset.

  \param name   the attribute to print
*/
// ----------------------------------------------------------------------
void QmvClass::print(const QString &name)
{
    for ( int i = 0; i < active_tuple_count; i++ )
        loaded_tuples.at(i)->print(name);
}

// ----------------------------------------------------------------------
//! print a tuple from the current dataset.
/*!

  Print a single tuple from the current dataset.

  \param row    the specified tuple number  (0 -> ( count() - 1 ) )
*/
// ----------------------------------------------------------------------
void QmvClass::print(int row)
{
  if (row >= 0 && row < count() )
        loaded_tuples.at(row)->print();
}

// ----------------------------------------------------------------------
//! print an attribute of a single tuple.
/*!

  Print a single attribute of a single tuple of the current dataset.

  \param row    tuple number (0 -> ( count() - 1 ) )
  \param name   attribute name
*/
// ----------------------------------------------------------------------
void QmvClass::print(int row, const QString &name)
{
  if (row >= 0 && row < count())
        loaded_tuples.at(row)->print(name);
}


// ----------------------------------------------------------------------
//! Meta-attribute list
/*!

  Prints the names and values of meta-attributes of a tuple attribute.
  
*/
// ----------------------------------------------------------------------
void QmvClass::attAttPrint(const QString & attname)
{

    QmvAttribute *att = attribute_metadata.find(attname);
    if (att)
        att->print();
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvClass::attAttPrint()
{

    QDictIterator<QmvAttribute> it_ma(attribute_metadata);
    while( it_ma.current() )
    {
        this->attAttPrint( it_ma.currentKey() );
        ++it_ma;
    }
}

// ----------------------------------------------------------------------
//! Print to the Printer
/*!
  Print the current data set to a printer;

  \param pr             the QPrinter paint device
  \param attlist        list of attributes to include ( as columns ). A null list
                        will cause all viewaable attributes to be printed.

  \Error Message
*/
// ----------------------------------------------------------------------
bool QmvClass::printReport( QPrinter *printer,
                            const QStringList * attlist, const QString * rowlist )
{
    if ( !printer || (count() < 1) )
        return FALSE;
    
        // ------------------------------------------------------
        // Get an attribute list - from arg - rowlist,  or viewable
        // ------------------------------------------------------
    QStringList printable;
    if ( attlist )
        printable = *attlist;
    else
        printable = attributeList( AttViewable );

    int column_count = printable.count();
    
        // ------------------------------------------------------
        // Create - a StringList for the column headings
        //        - an array to hold the default column widths
        //        - an array to hold the maximum datalength in each column.
        //        - an attribute index sorted by "browse order"
        // ------------------------------------------------------
    QmvAttList atlist = sortedAttlist( "mtat_browse_order", NoBlank|NoSystem );
    QArray<int> column_widths( column_count );
    QArray<int> max_column_widths( column_count );
    
        // ------------------------------------------------------
        // Scan the attribute index and load widths and headings
        // ------------------------------------------------------
    int total_char_width = 0;           // in characters
    int total_realchar_width = 0;       // in characters
    const int min_col_width = 1;        // in characters
    const int min_wrap_col_width = 15;  // in characters
    
    int col_num = 0 ;
    for ( QmvAttribute * at_ptr = atlist.first();
          at_ptr != 0 && col_num < column_count;
          at_ptr = atlist.next())
    {
        qDebug("printReport:%s, mtat_title = %s",
               at_ptr->find("mtat_name")->latin1(),
               at_ptr->find("mtat_title")->latin1());
        QString * heading = at_ptr->find( "mtat_title");
        column_widths[col_num] = at_ptr->find("mtat_displen")->toInt();
        int hl = heading->length();
        if ( column_widths[col_num] < hl )
            column_widths[col_num] = hl;
        
        total_char_width += column_widths[col_num];
            // Scan the dataset for maximum data length
            // - initialise with heading width
        max_column_widths[col_num] = column_widths[col_num];
        for ( int i = 0; i < count(); i++)
        {
            int attvallen = attributeValue( i, *at_ptr->find("mtat_name" )).length();
            if ( attvallen > max_column_widths[col_num] )
                max_column_widths[col_num] = attvallen;
        }
            // apply minimum column width
        if ( max_column_widths[col_num] < min_col_width )
            max_column_widths[col_num] = min_col_width;
        total_realchar_width += max_column_widths[col_num];

        col_num++;
    }
    
   
        // ---------------------------------------
        // Prepare some layout and scaling metrics
        // ---------------------------------------
    const int margin = 10;                                      // Margins on all sides
    const int header_margin = 130;                              // size for header details
    const int trailer_margin = 60;                              // size for trailer details
    const int cell_padding = 2;                                 // cell padding
    QPaintDeviceMetrics metrics( printer );                     // need width/height of printer surface
    int avail_data_width  = metrics.width() - ( 2 * margin ) - ( column_count * (cell_padding * 2));
    int avail_data_height = metrics.height() - ( 2 * margin ) - header_margin - trailer_margin;
    const QString def_font = "Helvetica";
    const QString head_font = "Times";
    const int head_y_points = 14;
    
    const int def_y_points = 12;                                        // Default vertical pointsize
    const int min_y_points = 7;
    const float min_scaling = float(min_y_points) / float(def_y_points);
        // get approx y/x ratio
    QFont testfont( def_font, def_y_points );
    QFontMetrics fm( testfont );
    float def_yx_ratio = float(fm.height()) / float(fm.width("M"));     // Desired V/H font size ratio
    float x_points = float(def_y_points) / float(def_yx_ratio);         // Default horizontal pointsize
    int y_points = def_y_points;                                // Start with Default vertical pointsize
    float scaling = float(avail_data_width) / (float(total_char_width) * x_points);


    
        // ---------------------------------------
        // Scaling methodology - progressively..
        //      - scale the point size
        //      - reduce the column width to
        //        maximum data width
        //      - reduce the column width by wrapping
        //      - reduce the column count
        // Variables to be set;
        //      - scaling
        //      - total_char_width
        //      - column_widths
        //      - column_count
        // ---------------------------------------
    
    if ( scaling < 1.0 )
        if ( scaling >= min_scaling )
            x_points = avail_data_width / total_char_width;
        else // we need to vary the specified (in metadata) column widths
            while ( true )
            {
                    // use real data length ??
                float tmp_scaling = avail_data_width / (total_realchar_width*x_points);
                if ( tmp_scaling < 1.0 && tmp_scaling >= min_scaling )
                {
                    scaling = tmp_scaling;
                    x_points = avail_data_width / total_realchar_width;
                    column_widths = max_column_widths;
                    total_char_width = total_realchar_width;
                    qDebug("printReport:: using real data width method");
                    break;
                }
                
                    // Reduce with of widest colums - word wrap.
                    // - recalculate the column widths
                QArray<int> wrapped_column_widths( column_count );
                int col_num = 0;
                int total_wrapchar_width = 0;
                for ( QmvAttribute * at_ptr = atlist.first();
                      at_ptr != 0 && col_num < column_count;
                      at_ptr = atlist.next())
                {
                    if ( max_column_widths[col_num] > min_wrap_col_width )
                        wrapped_column_widths[col_num] = min_wrap_col_width;
                    else
                        wrapped_column_widths[col_num] = max_column_widths[col_num];
                    total_wrapchar_width += wrapped_column_widths[col_num];
                    col_num++;
                }
                tmp_scaling = avail_data_width / (total_wrapchar_width*x_points);
                if ( tmp_scaling < 1.0 && tmp_scaling >= min_scaling )
                {
                    scaling = tmp_scaling;
                    x_points = avail_data_width / total_wrapchar_width;
                    column_widths = wrapped_column_widths;
                    total_char_width = total_wrapchar_width;
                    qDebug("printReport:: using wrapped real data width method");
                    break;
                }

                    // drop some columns
                int new_column_count = 0;
                int tmp_width = 0;
                for (int i = 0; i < column_count; i++)
                {
                    tmp_width += wrapped_column_widths[i];
                    tmp_scaling = avail_data_width / (tmp_width*x_points);
                    if (tmp_scaling >= min_scaling)
                        continue;
                    tmp_width -= wrapped_column_widths[i];
                    tmp_scaling = avail_data_width / (tmp_width*x_points);
                    new_column_count = i;
                    break;
                }
                if ( new_column_count > 0 )
                {
                    column_count = new_column_count;
                    scaling = tmp_scaling;
                    total_char_width = tmp_width;
                    x_points = avail_data_width / total_char_width;
                    column_widths = wrapped_column_widths;
                    qDebug("printReport:: using wrapped real data width method, with column truncation");
                    break;
                }
                    
                
                    // failed
                qDebug("printReport:: failed to resolve layout");
                return (FALSE);
            }
    

        // After scaling, set the font pointsize
    y_points = int( x_points * def_yx_ratio );
    int total_data_width = int(total_char_width*x_points);      // total data width in points
    


        // ------------------------------------------------------
        // Open a painter
        // ------------------------------------------------------
    QPainter p;
    if( !p.begin( printer ) )
        return FALSE;
    p.setFont( QFont( def_font, y_points, QFont::Normal) );
    fm = p.fontMetrics();

        // --------------------------------------------------
        // Start printing
        // --------------------------------------------------
    int pagenum = 0;                                            // initialise page count
    int ypos = 0;
    int xpos = 0;
    int linespace = 0;
    QRect testrect;                                             // to determine print size
    QPointArray border_lines;                                   // to draw borders
    int table_width = ( column_count * 2 * cell_padding ) + total_data_width;
    for( int row = 0 ; row < count() ; row++ )
    {
            // header (first page or new page) ----------------------------------------
        if ( ypos == 0 || ypos > metrics.height() - margin - trailer_margin )
        {
            if ( ypos > 0 ) // Not the first page ------------------
            {
                    // closing horizontal line ---------------------
                border_lines.putPoints( border_lines.size(), 2, margin, ypos, margin + table_width, ypos);

                    // draw the left vertical border
                border_lines.putPoints( border_lines.size(), 2,
                                        margin, header_margin,
                                        margin, ypos);
                
                    // Vertical lines ------------------------------
                int right_col = margin;
                for ( int col_num = 0; col_num < column_count; col_num++ )
                {
                    right_col += int(column_widths[col_num] * x_points) + (2 * cell_padding);
                    border_lines.putPoints( border_lines.size(), 2,
                                            right_col, header_margin,
                                            right_col, ypos);
                }
                p.drawLineSegments( border_lines );

                    // Page number ---------------------------------
                p.save();
                p.setFont( QFont( head_font, head_y_points, QFont::Bold) );
                ypos = metrics.height() - trailer_margin;
                xpos = margin;
                p.drawText( xpos, ypos,
                            metrics.width() - (2 * margin ),
                            p.fontMetrics().lineSpacing(),
                            AlignCenter,
                            tr("Page: ") + QString::number( ++pagenum ) );
                p.restore();

                    // New Page ------------------------------------
                printer->newPage();
            }

                // ------------------------------------------------------------
                // Page header
                // ------------------------------------------------------------
            p.save();
            p.setFont( QFont( head_font, head_y_points, QFont::Bold) );
            int label_width = 100;

            ypos = 2*margin;
            xpos = margin;
            p.drawText( xpos, ypos,
                        avail_data_width,
                        p.fontMetrics().lineSpacing(),
                        AlignCenter|AlignTop,
                        tr("LISTING: ") + relationTitle(), -1, &testrect );
            ypos += ( testrect.height() + p.fontMetrics().lineSpacing());

            if ( ypos < header_margin)
            {
                p.drawText( xpos, ypos,
                            xpos + label_width + 30,
                            p.fontMetrics().lineSpacing(),
                            AlignLeft,
                            tr("Prepared on:"));
                p.save();
                p.setFont( QFont( def_font, def_y_points, QFont::Normal) );
                p.drawText( xpos + label_width + 30, ypos,
                            avail_data_width - (xpos + label_width + 30),
                            header_margin - ypos,
                            AlignLeft,
                            QDateTime().currentDateTime().toString(), -1, &testrect );
                p.restore();
                ypos += ( testrect.height() >
                          p.fontMetrics().lineSpacing()) ? testrect.height() : p.fontMetrics().lineSpacing();
            }
            
            if ( ypos < header_margin)
            {
                p.drawText( xpos, ypos, 
                            xpos + label_width + 30,
                            p.fontMetrics().lineSpacing(),
                            AlignLeft,
                            tr( "Rows in set:" ));
                p.drawText( xpos + label_width + 30, ypos, 
                            avail_data_width - (xpos + label_width + 30),
                            p.fontMetrics().lineSpacing(),
                            AlignLeft,
                            QString("").setNum(count()) );
                ypos += p.fontMetrics().lineSpacing();
            }
            
            if ( ypos < header_margin)
            {
                p.drawText( xpos, ypos, 
                            xpos + label_width + 30,
                            p.fontMetrics().lineSpacing(),
                            AlignLeft,
                            tr( "Filter:" ));
                p.save();
                p.setFont( QFont( def_font, def_y_points, QFont::Normal) );
                p.drawText( xpos + label_width + 30, ypos,
                            metrics.width() - margin  - (xpos + label_width + 30),
                            header_margin - ypos,
                            AlignLeft|WordBreak,
                            queryDescription(), -1, &testrect );
                p.restore();
                ypos += ( testrect.height() >
                          p.fontMetrics().lineSpacing()) ? testrect.height() : p.fontMetrics().lineSpacing();
            }
            
            
                // now for data
            p.restore();
            ypos = header_margin;	
            xpos = margin + cell_padding;

                // initialise the border point array with top line --------------------
            border_lines.setPoints( 2, margin, ypos, margin + table_width, ypos);

                // Draw the column headings ----------------------------------------
            int col_num = 0;
            int row_height = 0;
            int avail_height = metrics.height() - ypos - trailer_margin;
            for ( QmvAttribute * at_ptr = atlist.first(); at_ptr != 0; at_ptr = atlist.next())
            {
                    // allow for column truncation
                if (col_num >= column_count)
                    break;
                
                int colwid = int(column_widths[col_num] * x_points);
                qDebug("%d+%d ", xpos, colwid);

                QString heading = *at_ptr->find( "mtat_title" );

                    // Determine the printing space.
                testrect = p.boundingRect( xpos, ypos,
                                           colwid,
                                           avail_height,
                                           WordBreak|AlignCenter, heading);
                
                    // really print using determined values
                p.drawText( xpos, ypos,
                            colwid,
                            testrect.height(),
                            WordBreak|AlignCenter,
                            heading );
                xpos += (colwid + (2 * cell_padding));
                
                    // remember the deepest row.
                if ( testrect.height() > row_height )
                    row_height = testrect.height();
                
                col_num++;
            }

                // Increment by deepest row
            ypos = ypos + row_height;
            xpos = margin;
            
                // line under headings
            border_lines.putPoints( border_lines.size(), 2, margin, ypos, margin + table_width, ypos);
            
        } // end of header ------------------------------------------------------------
        
            // Print the data
        int col_num = 0;
        int row_height = 0;
        int xpos = margin + cell_padding;
        qDebug("printReport:: scaling=%f, point=%d", scaling, y_points);
            // Find the row height, so background can be painted
        for ( QmvAttribute * at_ptr = atlist.first(); at_ptr != 0; at_ptr = atlist.next())
        {
                // allow for column truncation
            if (col_num >= column_count)
                break;
            int colwid = int(column_widths[col_num] * x_points);
            qDebug("%d+%d ", xpos, colwid);
            QString celldata = attributeValue( row, *at_ptr->find( "mtat_name" ) );
            testrect = p.boundingRect( xpos, ypos,
                                       colwid, metrics.height(),
                                       WordBreak|AlignLeft, celldata);
                // remember the deepest row.
            if ( testrect.height() > row_height )
                row_height = testrect.height();
            col_num++;
        }
        if ( row % 2 )
            p.fillRect( margin, ypos, table_width, row_height,
                        QBrush( QColor(lightGray), Dense7Pattern) );
            // now write the data
        
        col_num = 0;
        row_height = 0;
        xpos = margin + cell_padding;
        for ( QmvAttribute * at_ptr = atlist.first(); at_ptr != 0; at_ptr = atlist.next())
        {
                // allow for column truncation
            if (col_num >= column_count)
                break;
            
            int colwid = int(column_widths[col_num] * x_points);
            qDebug("%d+%d ", xpos, colwid);
            QString celldata = attributeValue( row, *at_ptr->find( "mtat_name" ) );
            testrect = p.boundingRect( xpos, ypos,
                                       colwid, metrics.height(),
                                       WordBreak|AlignLeft, celldata);
            p.drawText( testrect,
                        WordBreak|AlignLeft,
                        celldata);
            xpos += (colwid + ( 2 * cell_padding));
                
                // remember the deepest row.
            if ( testrect.height() > row_height )
                row_height = testrect.height();
            col_num++;
        }


            // Increment by deepest row
        ypos = ypos + row_height;
    }
    
        // closing horizontal line ---------------------
    border_lines.putPoints( border_lines.size(), 2, margin, ypos, margin + table_width, ypos);
    
        // draw the left vertical border
    border_lines.putPoints( border_lines.size(), 2,
                            margin, header_margin,
                            margin, ypos);
    
        // Vertical lines ------------------------------
    int right_col = margin;
    for ( int col_num = 0; col_num < column_count; col_num++ )
    {
        right_col += int(column_widths[col_num] * x_points) + (2 * cell_padding);
        border_lines.putPoints( border_lines.size(), 2,
                                right_col, header_margin,
                                right_col, ypos);
    }
    
    qDebug("printReport:: border_lines = %d", border_lines.size() );
    p.drawLineSegments( border_lines );
    
        // Page number ---------------------------------
    ypos = metrics.height() - trailer_margin;
    xpos = margin;
    pagenum++;
    p.save();
    p.setFont( QFont( head_font, head_y_points, QFont::Bold) );
    p.drawText( xpos, ypos,
                metrics.width() - trailer_margin,
                p.fontMetrics().lineSpacing(),
                AlignCenter,
                tr("Page: ") + QString::number( pagenum ) + tr(" of ") + QString::number( pagenum ));
    p.restore();

        // send job to printer
    p.end();

    return TRUE;
}


// ----------------------------------------------------------------------
//! Return error message. 
/*!
  Return the most recent error message;

  \Error Message
*/
// --------------------------------------------------------------------------------
QString QmvClass::lastError()
{
    return QString("");
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QDateTime QmvClass::relStrToDateTime( const QString & str )
{
    QDateTime dt_null;
    
        // expecting NN D/Mi/H/D/W/F/M/Y AGO/AHEAD
    QStringList ts_parts = QStringList::split( QRegExp( "\\s+" ), str );
    if ( int i = ts_parts.count() != 3 )
        return dt_null;

        // value
    bool ok;
    int value = ts_parts[0].toInt(&ok, 10);
    if ( !ok )
        return dt_null;

        // Direction
    int dir;
    if ( ts_parts[2].lower().find("ago") == 0 )
        dir = -1;
    else if ( ts_parts[2].lower().find("ahead") == 0 )
        dir = 1;
    else
        return dt_null;
    
        // units
    QString units = ts_parts[1].lower();
    if ( units.find("s") == 0 )
        return QDateTime::currentDateTime().addSecs( dir * value);
    else if ( units.find("mi") == 0 )
        return QDateTime::currentDateTime().addSecs( dir * value * 60);
    else if ( units.find("h") == 0 )
        return QDateTime::currentDateTime().addSecs( dir * value * 60 * 60);
    else if ( units.find("d") == 0 )
        return QDateTime::currentDateTime().addDays( dir * value );
    else if ( units.find("w") == 0 )
        return  QDateTime::currentDateTime().addDays( dir * value * 7 );
    else if ( units.find("f") == 0 )
        return  QDateTime::currentDateTime().addDays( dir * value * 14 );
    else if ( units.find("mo") == 0 )
        return  QDateTime::currentDateTime().addDays( dir * value * 30 ); // approximation
    else if ( units.find("y") == 0 )
        return  QDateTime::currentDateTime().addDays( dir * value * 365 ); // approximation
    
    return dt_null;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QDateTime QmvClass::strToDateTime( const QString & str )
{
        // some standard symbols
    QString symbol = str.lower();
    if ( symbol == "today" )
        return QDateTime( QDate::currentDate(), QTime() );
    else if ( symbol == "now" )
        return QDateTime::currentDateTime();
    else if ( symbol == "tomorrow" )
        return QDateTime::currentDateTime().addDays(1);
    else if ( symbol == "yesterday" )
        return QDateTime::currentDateTime().addDays(-1);

        // check for relative - eg "1 week ago"
    QDateTime relative = relStrToDateTime( str );
    if (relative.isValid() )
        return relative;
    
        // expecting 27/02/2002[ 08:55:57.00 EST]
    QStringList ts_parts = QStringList::split( QRegExp( "\\s+" ), str );

    QDate date_part = QDate();
    QTime time_part = QTime();
    QString zone_part;
    

    if ( ts_parts.count() >= 2 )
    {
        QStringList t_parts = QStringList::split( QRegExp( ":" ), ts_parts[1] );
            // hh:mm:ss.ms
        if ( t_parts.count() == 3 )
            time_part = QTime( t_parts[0].toInt(), t_parts[1].toInt(),
                               t_parts[2].left(2).toInt(), t_parts[2].right(2).toInt() );
        else if ( t_parts.count() == 2 )
            time_part = QTime( t_parts[0].toInt(), t_parts[1].toInt(),
                               t_parts[2].left(2).toInt() );
        else
            return QDateTime();
    }
    if ( ts_parts.count() >= 1 )
    {
        QStringList d_parts = QStringList::split( QRegExp( "[/ -]" ), ts_parts[0] );
        if ( d_parts.count() == 3 )
        {
            int y, m, d;
            bool ok;
            y = d_parts[2].toInt(&ok);
            if (!ok) return QDateTime();
            m = d_parts[1].toInt(&ok);
            if (!ok) return QDateTime();
            d = d_parts[0].toInt(&ok);
            if (!ok) return QDateTime();
            date_part = QDate( y, m, d );
        } else
            return QDateTime();
    }
    return QDateTime( date_part, time_part );
}

// --------------------------------------------------------------------------------
//! Convert PostgreSQL Interval to minutes
/*!
  Convert a PostgreSQL interval string to an integer minute value

  \param str    PostgreSQL interval string
  \return       minutes since 1/1/0000
*/
// --------------------------------------------------------------------------------
int QmvClass::strToMinutes( const QString & str )
{
        // expecting:  <float> <string>
    QStringList items = QStringList::split( QRegExp( "\\s+" ), str );
    int minutes = 0;
    bool ok;
    int i = 0;
    while ( i < items.count() )
    {
        if ( items[i] == "@" )
            i++;
        if ( i >= items.count() )
            return -1;
        
        double value = items[i].toDouble(&ok);
        if ( !ok )
            return -1;
        i++;
        if ( i >= items.count() )
            return -1;
        QString unit = items[i];
        if ( unit.find( QRegExp( "min" ) ) >= 0 )
            minutes += int( value );
        if ( unit.find( QRegExp( "hour" ) ) >= 0  )
            minutes +=  int( value*60 );
        if ( unit.find( QRegExp( "day" ) )  >= 0 )
            minutes += int( value*60*24 );
        if ( unit.find( QRegExp( "mon" ) )  >= 0 )
            minutes += int( value*60*24*30 );   // approx
        if ( unit.find( QRegExp( "year" ) )  >= 0 )
            minutes += int( value*60*24*365 );  // approx
        i++;
    }
    
    return minutes;
}


// --------------------------------------------------------------------------------
//! Convert PostgreSQL Time to minutes
/*!
  Convert a PostgreSQL time string to an integer minute value

  \param str    PostgreSQL time string of form HH:MM:SS
  \return       minutes since midnight or -1 for error
*/
// --------------------------------------------------------------------------------
int QmvClass::timeToMinutes( const QString & str )
{
    QStringList items = QStringList::split( ":", str );
    if ( items.count() < 2 )
        return -1;

    bool ok;
    int hours = items[0].toInt(&ok);
    if ( !ok )
        return -1;
    int minutes = items[1].toInt(&ok);
    if ( !ok )
        return -1;
    return (hours *60 ) + minutes;
}
// --------------------------------------------------------------------------------
//! Convert \ref QDateTime to PostgreSQL timestamp
/*!

  \param        dt     
  \return       PostgreSQL timestamp
*/
// --------------------------------------------------------------------------------
QString QmvClass::qtDateTimeToPgTimestamp( const QDateTime & dt )
{
    if ( dt.isNull() )
        return QString("");
    
    return( QString( "%1-%2-%3 %4:%5" )
            .arg( dt.date().day() )
            .arg( dt.date().month() )
            .arg( dt.date().year())
            .arg( dt.time().hour() )
            .arg( dt.time().minute() ) );
}

// --------------------------------------------------------------------------------
//! Week number in year
/*!
  Return the number of the week in \ref QDate dt

  \param dt             Date
  \return               Day of week
*/
// --------------------------------------------------------------------------------
int QmvClass::weekOfYear( QDate & dt )
{
        // days in week 1 of year
    int wkone = 7 - QDate(dt.year(), 1, 1).dayOfWeek();
    int week = ( (dt.dayOfYear() - wkone) / 7 ) + 1;
    return week;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QString QmvClass::cleanForSQL( const QString & str )
{
    QString x_str = str.stripWhiteSpace();
    x_str.replace( QRegExp("\\\\"), "");        // remove escapes
    x_str.replace( QRegExp("'"), "\\'");        // escape single-quotes
    return x_str;
}

/*
 * ======================================================================
 * MEMBER FUNCTIONS: QmvSqlClass
 * ======================================================================
 */
//! Constructor
/*!

  Constructs a data class object, using metadata known to the parent
  qmvDB object.

  \param db             Parent database object
  \param name           Data class name
  \param rellevel       Limit level of auto load of relations classes. Defaults to 0.
*/
// ----------------------------------------------------------------------
QmvSqlClass::QmvSqlClass( QmvDB * db, const QString name, const int rellevel )
        : QmvClass( db, name, rellevel )
{
    database = db;
    qDebug("QmvSqlClass:TOP: %d %s", rellevel, name.latin1());
    
    int num_rows, i;
    QString * att;
    QmvAttribute * m_att;
    QString *att_name;
    QList<QString> * att_class_list = new QList<QString>( database->attmetaClass() );
    QList<QString> * att_attribute_list = new QList<QString>( database->attmetaAttribute() );
    QList<QString> * att_foreign_list = new QList<QString>( database->attmetaForeign() );
    QList<QString> * att_relation_list = new QList<QString>( database->attmetaRelation() );
    QString query;

        // Set the classname
    class_name = name;
    
        // Get metadata for this class ========================================
        // ====================================================================
    
    class_metadata.setAutoDelete( TRUE );     // delete items when they are removed
    class_metadata.clear();                     // Reset
    query = QString("select mtcl.* from mtcl"
                    " where mtcl_name = '%1'"
                    " and mtcl__status = 'A'").arg(name);
    num_rows = sql_exec(query);
    
    if(num_rows != 1)
    {
        qWarning("No meta-class data can be located for %s"
                 " - calling createMetadata(%s)", name.latin1(), name.latin1());
        if ( (createMetadata(name) == FALSE) || ((num_rows = sql_exec(query)) != 1) )
        {
            QmvException * exception
                = new QmvException( QString( "<h2>Unable to locate meta data.</h2>"
                                             "<ul><li>Database name: <b>%1</b>"
                                             "<li>Class: <b>%2</b></ul>" )
                                    .arg( dataBase()->dbTitle() )
                                    .arg( name ),
                                    QmvException::Warning );
            throw( exception );
        }
    }

        // Use the template of class metadata
    for (att = att_class_list->first();
         att != 0;
         att = att_class_list->next())
    {
        class_metadata.insert(*att, new QString(sql_value(0, *att)));
            // cout << " ... found " << *class_metadata[*att] << " at " << class_metadata[*att] << endl;
    }

    
        // Get metadata for attributes of this class ------------------------------
    attribute_metadata.setAutoDelete( TRUE );     // delete items when they are removed
    attribute_metadata.clear();

    query = QString("select mtat.* from mtat where mtat_class_name = '%1'"
                    " and mtat__status = 'A'"
                    " and (%2 & mtat_access::integer) != 0"
                    " order by mtat_view_order")
        .arg(name)
        .arg( dataBase()->dbAccess()->accessID() );
    num_rows = sql_exec(query);
    for (i = 0; i < num_rows; i++)
    {

        m_att = new QmvAttribute( this ); // Create a new attribute
        att_name = new QString(sql_value(i, "mtat_name"));
        attribute_metadata.insert((const QString &) *att_name,
                                  (const QmvAttribute *) m_att);
        for (att = att_attribute_list->first();
             att != 0;
             att = att_attribute_list->next())
        {
            m_att->insert((const QString &) *att,
                          new QString(sql_value(i, *att)));
        }
            // Attribute is loaded, so set the datatype and input method
        m_att->setdataType();
        m_att->setInputMethod();
        m_att->setAccessMethod();
        qDebug("QmvSqlClass:atname=%s, type=%d, method=%d\n",
               att_name->latin1(),
               m_att->dataType(),
               m_att->inputMethod() );
        
    }
        // bulk load alerts for attributes of this class ------------------------------

    int sql_result_index_mtal;
    QString query_mtal = QString("select mtal.* from mtal where mtal_mtat_name in"
                                 " (select mtat_name from mtat where mtat_class_name = '%1')")
        .arg(name);
    int num_alerts = r_sql_exec(database->dbConnectionID(), &sql_result_index_mtal, query_mtal);
    for (int i_mtal = 0; i_mtal < num_alerts; i_mtal++)
    {
        QmvAttribute * m_att = attribute_metadata.find( QString(r_sql_value(&sql_result_index_mtal,
                                                                            i_mtal, "mtal_mtat_name")) );
        if (!m_att)
            continue;
        
            // add the alert to the QmvAttribute object
            // trust the DB to supply the right enum
        m_att->addAlert((QString(r_sql_value(&sql_result_index_mtal, i_mtal, "mtal_priority")).toInt()),
                        static_cast<QmvAttribute::operator_types>
                        (QString(r_sql_value(&sql_result_index_mtal, i_mtal, "mtal_mtop_code")).toInt()),
                        QString(r_sql_value(&sql_result_index_mtal, i_mtal, "mtal_value")),
                        QString(r_sql_value(&sql_result_index_mtal, i_mtal, "mtal_colour")) );
    }
    r_sql_freeresult(&sql_result_index_mtal);
    
        // Get metadata for foreigns of this class ------------------------------
    foreign_metadata.setAutoDelete( TRUE );     // delete items when they are removed
    foreign_metadata.clear();

    query = QString("select mtfn.* from mtfn where mtfn_master_class = '%1'"
                    " and mtfn__status = 'A'").arg(name);
    int sql_result_index;
    num_rows = r_sql_exec(database->dbConnectionID(), &sql_result_index, query);
    for (i = 0; i < num_rows; i++)
    {
        QmvAttribute * fk_m_att = new QmvAttribute( this );
        QString * fk_att_name = new QString(r_sql_value(&sql_result_index, i, "mtfn_key"));
        
        foreign_metadata.insert((const QString &) *fk_att_name,
                                (const QmvAttribute *) fk_m_att);
            // load fk_m_att with values
        for (att = att_foreign_list->first();
             att != 0;
             att = att_foreign_list->next())
        {
            qDebug ("QmvClass: fk = %s", att->latin1());
            
            fk_m_att->insert((const QString &) *att,
                             new QString(r_sql_value(&sql_result_index, i, *att)));
        }
    }
        // Free the result index
    r_sql_freeresult(&sql_result_index);
    
        // Get metadata for relations of this class ------------------------------
    relation_metadata.setAutoDelete( TRUE );     // delete items when they are removed
    relation_metadata.clear();

    query = QString("select mtrl.* from mtrl where mtrl_master_class = '%1'"
                    " and mtrl__status = 'A'"
                    " order by mtrl_title" )
        .arg(name);
    num_rows = sql_exec(query);
    for (i = 0; i < num_rows; i++)
    {
        m_att = new QmvAttribute( this );
        att_name = new QString(sql_value(i, "mtrl_other_class"));
        relation_metadata.insert((const QString &) *att_name,
                                 (const QmvAttribute *) m_att);
        for (att = att_relation_list->first();
             att != 0;
             att = att_relation_list->next())
        {
            m_att->insert((const QString &) *att,
                          new QString(sql_value(i, *att)));
        }
    }

}

// ----------------------------------------------------------------------
//! Copy Constructor
/*!
  Creates a copy of \em rel. Tuple list (loaded_tuples), query
  and query handle are not copied.

  \param rel    
*/
// ----------------------------------------------------------------------
QmvSqlClass::QmvSqlClass( const QmvSqlClass & rel )
        : QmvClass( rel )
{

}


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
QmvSqlClass::~QmvSqlClass()
{  
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvSqlClass::queryDescription()
{

    return(currentQuery());
        //     Needs to be debugged
    QString querystring = currentQuery();
    QString plaintext;
    QStringList query = QStringList().split( QRegExp(" +"), querystring );
    for ( QStringList::Iterator it = query.begin(); it != query.end(); ++it )
    {
        if (!(*it).contains( QRegExp("^where$", FALSE) ))
            continue;
        ++it;

        while( it != query.end() )
        {
            QString title = attAttValue( (*it), "mtat_title");
            if ( title )
                plaintext += title;
            else
                plaintext += (*it);
            plaintext += " ";
            
            ++it;
        }
        
    }
    return plaintext;
}

// ----------------------------------------------------------------------
//! Create persistent default metadata
/*!

  Create a set of meta data for a class. This includes meta-class,
  meta-attribute, meta-foreign and meta-relation datasets. The
  information is scrounged from the database system tables by the SQL
  function create_metadata().

  \return true on success and false if any errors
*/
// ----------------------------------------------------------------------
const bool QmvSqlClass::createMetadata( const QString name )
{
    QString query;
    int i_status;
    
    sql_begin_work();
    query = QString("select create_metadata('%1')").arg(name);
    i_status = sql_exec(query);
    if (i_status == 1)
    {
        sql_commit_work();
        return(TRUE);
    } else {
        sql_rollback_work();
        return(FALSE);
    }
    
}

// ----------------------------------------------------------------------
//! Load foreign relation objects
/*!
  Creates and loads foreign relations for this relation.

  \param fkrel  A QStringList of Foreign relation names. A zero list causes
                all foreign relations to be initialised
  \param limit  limit to number of tupples loaded in each foreign relation. If
                limit = 0, then use fkatt->listLimit() which is derived from metadata.
  \return       Number of foreign relations loaded
*/
// ----------------------------------------------------------------------
int QmvSqlClass::loadForeignRelations( const QStringList & fkrel, const int limit )
{
    
    QmvDB * db = dataBase();
    
    QDictIterator<QmvAttribute> it( *foreignMetadata() );
    int loaded = 0;
    for ( ; it.current(); ++it )
    {
        QmvAttribute *fk_m_att = it.current();
        QString fk_att_name = it.currentKey();

            // get the classname
        QString * cln = fk_m_att->find("mtfn_other_class" );
        if ( !cln )
            continue;
        
            // Was a list specified, and is this relation in it?
        if ( fkrel.count() > 0 && fkrel.grep( QRegExp(QString("^%1$").arg(*cln)) ).count() == 0 )
            continue;

            // Is this foreign relation already loaded?
        if ( foreignRelation( *cln ) )
            continue;
        
       
            // Add a pointer to this foreign QmvAttribute in the foreign key QmvAttribute
            // of this tuple.
            // Create a new QmvSqlClass object as a foreign relation
        QmvAttribute * fk_att = attribute_metadata.find( fk_att_name );
        if ( fk_att )   // this should be found ie non-zero
        {
            qDebug("loadForeignRelations (%s)", fk_att->attributeName().latin1() );
                // Look for an existing foreign relation of this type
            QmvSqlClass * fk_rel = (QmvSqlClass *) db->dbForeignRelations()->find( *cln );
            qDebug("----> looking for %s in dbForeignRelations() ", cln->latin1());

            if ( !fk_rel )
            {
                qDebug("----> NOT FOUND ");
                    // look for an existing implementation of this class.
                fk_rel = (QmvSqlClass *) db->dbClasses()->find( *cln );
                qDebug("----> looking for %s in dbClasses()", cln->latin1());
            
                if ( fk_rel )
                {
                        // Copy existing class
                    qDebug("----> FOUND in dbClasses() - %x", fk_rel );
                    fk_rel = new QmvSqlClass( *fk_rel );
                } else {
                        // no existing classes of this type - create a new one.
                    fk_rel = new QmvSqlClass( db, *cln, 0 );
                    qDebug("----> NOT FOUND - made a new one - %x", fk_rel );
                }
                    // update the relevant foreign key attribute with foreign relation details
                    // and object pointer
            } else
                qDebug("----> FOUND - %x", fk_rel);
            
            qDebug("----> Registering foreign relation at - %x", fk_rel);
            fk_att->setForeign( fk_m_att, fk_rel );
                // Add to foreign relations list for this class
            foreign_relations.insert( *cln, fk_rel );

                // Add to the master foreign relations index
            db->addtoForeignRelationIndex( *cln, fk_rel );
        
                // Now load the foreign value list
            int newlimit = ( limit != 0 ) ? limit : fk_att->listLimit();
            if ( newlimit > 0 )
                fk_rel->open( QString("SELECT * FROM %1 ORDER BY %2 LIMIT %3 ")
                              .arg(fk_rel->relationName())
                              .arg(fk_rel->userLabel())
                              .arg( newlimit ) );

                // Create some utility pointers to relevant fields
            QString * mf_key = fk_m_att->find( "mtfn_key" );
            QString * mf_join = fk_m_att->find( "mtfn_join" );
            QString * mf_view = fk_m_att->find( "mtfn_view" );

                // Build a string of value/label pairs
            QString values;
            const QString sep1 = ";";
            const QString sep2 = ",";
            qDebug ("loadForeignRelations: setting fk pairs for %s (%d)",mf_join->latin1(), fk_rel->count());
            for ( int i = 0; i < fk_rel->count(); i++ )
            {
                qDebug (" (%d) %s = %s", i,
                        fk_rel->attributeValue( i, *mf_join ).latin1(),
                        fk_rel->attributeValue( i, *mf_view ).latin1() );
                values.append( QString("%1%2%3%4")
                               .arg( fk_rel->attributeValue( i, *mf_join ) )
                               .arg( sep2 )
                               .arg( fk_rel->attributeValue( i, *mf_view ) )
                               .arg( sep1 ) );
            }
            fk_att->setList( values, sep1, sep2 );
                // fk_att->print();
            loaded++;
        }
    }
    
    return loaded;
}


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvSqlClass::loadChildRelations( const int rellevel )
{

    QDictIterator<QmvAttribute> it(relation_metadata);
    
    while ( it.current() )
    {
        QmvSqlClass * new_cls;
        
            // look for an existing implementation of this class.
        QmvSqlClass * existing_cls = (QmvSqlClass *) database->dbClasses()->find( it.currentKey() );
        if ( existing_cls )
                // Copy
            new_cls = new QmvSqlClass( *existing_cls );
        else
                // New
            new_cls = new QmvSqlClass( database,  it.currentKey());

            // Insert into local index
        child_relations.insert( it.currentKey(), new_cls );
        
        qDebug("CHILD: loaded relation %s -> %s", relationName().latin1(), it.currentKey().latin1());
        if ( rellevel > 1 )
            new_cls->loadChildRelations( rellevel - 1 );
        
        ++it;
    }
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
QmvSqlTuple * QmvSqlClass::createTuple( QmvSqlTuple * existing )
{
    QmvSqlTuple * tup;
    if ( existing )
        tup = new QmvSqlTuple(existing);
    else
        tup = new QmvSqlTuple(this);
    
    connect( tup, SIGNAL( tupleChanged( QmvTuple *  ) ),
             SIGNAL( tupleChanged( QmvTuple * ) ) );
    return tup;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvSqlClass::deleteTuple( QmvSqlTuple * tup )
{
    tup->disconnect();
    delete tup;
}

    
// ----------------------------------------------------------------------
//! Open a persistent dataset
/*!

  Create a dataset consisting of existing persistent data which is
  specified by query, an SQL query. This method calls
  QmvSqlClass::open(const QString & query, QObject * controller; )
  with a NULL controller.

  \param query          SQL query string
  \return               The number of tuples in dataset

*/
// ----------------------------------------------------------------------
int QmvSqlClass::open(const QString & query)
{
    return( open( query, 0 ) );
}

// ----------------------------------------------------------------------
//! Open a persistent dataset
/*!

  Create a dataset consisting of existing persistent data which is
  specified by query, an SQL query.
  

  \param query          SQL query string
  \param controller     A QProgressDialog 
                        
  \return               The number of tuples in dataset
                        or error (<0) -1 = SQL error -2 = cancelled by controller.

*/
// ----------------------------------------------------------------------
int QmvSqlClass::open(const QString & query, QProgressDialog * controller )
{
    int i;
    int sql_result_index = -1;
    QString local_query = query;
  
    int nrows;
    if ( !local_query.isNull() && local_query.length() > 0 )
    {
            // expand wildcard attribute list
        local_query.replace( QRegExp("select *\\*", false ),
                             QString("SELECT %1").arg(SQLSelectList()) );
        
        if ( controller )
        {
      
            QApplication::setOverrideCursor( Qt::waitCursor );
                // TODO: checks could be calculated from cost estimate.
            int quickchecks = 10;
            const int checks = 100;
            int smallinterval = 200000;       // microseconds between early checks
            int interval = 1;                 // seconds between query checks
            int counter = 0;
            controller->reset();
            controller->setTotalSteps( quickchecks );
            controller->setProgress( counter );
            controller->setMinimumDuration (2000);
            int status = r_sql_send(database->dbConnectionID(), local_query);
            QApplication::restoreOverrideCursor();
            if ( status )
                return(-1);

            while ( TRUE )
            {
                nrows = r_sql_getresult( database->dbConnectionID(), &sql_result_index );
                if ( nrows >= 0 )
                    break;            // query completed
          
                    // If busy, continue loop, else SQL error.
                if ( nrows != SQL_GETRESULT_BUSY )
                    return(-1);

                    // If cancelled return
                if ( controller->wasCancelled() )
                {
                    r_sql_cancel( database->dbConnectionID() );
                    return(-2);
                }

                    // quick checks at the beginning 
                if ( counter < quickchecks)
                    usleep(smallinterval);
                else
                    sleep(interval);
          
                    // display progress to near the limit.
                if ( counter < ( controller->totalSteps() - 1 ) )
                    counter++;
                else {
                    counter = 1;
                    controller->setTotalSteps( checks );
                }
          
          
                controller->setProgress( counter );
                qApp->processEvents();
            }
            while( counter++ <= controller->totalSteps() )
                controller->setProgress( counter );
            controller->hide();
      
        } else {
            QApplication::setOverrideCursor( Qt::waitCursor );
            nrows = r_sql_exec(database->dbConnectionID(), &sql_result_index, local_query);
            QApplication::restoreOverrideCursor();
        }
        
    } else
        nrows = 0;
    

        // return if SQL error
    if ( nrows < 0 )
        return(-1);
  
    QmvSqlTuple *mt;
    QmvTupleAttribute *mta;
    QString this_class_name = relationName();
    for (i = 0; i < nrows; i++)
    {
            // Load into class tuple storage
        if ( i >= loaded_tuples.count() )
            mt = createTuple();
        else
            mt = (QmvSqlTuple *) loaded_tuples.at(i);
        
            /*
             * Load the new tuple with data:
             * Iterate over this->attribute_metadata which is keyed on tuple-attributes of this class.
             * and load the tupleattribute instance into the corresponding  QmvTupleAttribute of the
             * new tuple.
             */
        QDictIterator<QmvAttribute> it_attribute(attribute_metadata);
        while (it_attribute.current())
        {
            mta = mt->tupleAttribute(it_attribute.currentKey());
            char * val = r_sql_value(&sql_result_index, i, it_attribute.currentKey().latin1());
                //  attribute found ??
            if ( !val )
                return(-1);

            const QString & ta = QString( val );
          
            mta->load(ta);
            ++it_attribute;
        }
            // Now the QmvSqlTuple is ready to insert in the dataset of the class
        if ( i >= loaded_tuples.count() )
            loaded_tuples.append(mt);
    }
    active_tuple_count = nrows;
    current_query = query;


    if ( sql_result_index >= 0 )
    {
        r_sql_freeresult(&sql_result_index);
    }
    
    emit relationChanged( NewSet, active_tuple_count );
    return(active_tuple_count);
}
// ----------------------------------------------------------------------
//! Insert a new tuple into this dataset
/*!

  Extend the current dataset by one tuple.

  \param autosave       save the new tuple to persistent storage
  \return               the position of the new tuple
*/
// ----------------------------------------------------------------------
int QmvSqlClass::newTuple( bool autosave )
{
    QmvSqlTuple * nt = 0;
    if ( active_tuple_count < loaded_tuples.count() )
    {
            // reuse the next inactive tuple
        nt = (QmvSqlTuple *) loaded_tuples.at( active_tuple_count );
        nt->init();
    } else {
            // add a new tuple
        nt = createTuple();
        if ( !nt )
            return (-1);
        loaded_tuples.append( nt );
    }

    if ( autosave && !nt->save() )
        return (-1);
    
    active_tuple_count++;
    emit relationChanged( SetIncreased, 1 );
    return( active_tuple_count - 1 );
}

// ----------------------------------------------------------------------
//! Copy an existing tuple into this dataset
/*!

  Duplicate the tuple at \em row to a new tuple. If \em autosave is TRUE then
  save the copied tuple to persistent storage.
  
  \param row            Existing tuple row
  \param autosave       TRUE/FALSE
  \return               the position of the new tuple
*/
// ----------------------------------------------------------------------
int QmvSqlClass::copyTuple( int row, bool autosave )
{
        // cant copy invalid rows
    if ( row >= active_tuple_count )
        return(-1);
    
    QmvSqlTuple *existing = (QmvSqlTuple * )tupleAt( row );
    if ( !existing )
        return (-1);
    
    QmvSqlTuple *nt = createTuple( existing );
    if ( !nt )
        return (-1);
    
    if ( autosave && !nt->save() )
        return (-1);

        // remove an unused tuple (to maintain buffer size)
        // from immediately after the end of the active list
    if ( active_tuple_count < loaded_tuples.count() )
        deleteTuple( (QmvSqlTuple * ) loaded_tuples.take(active_tuple_count) );

    int newrow = row+1;
    loaded_tuples.insert( newrow, nt );
        // now have one more active tuple
    active_tuple_count++;
    emit relationChanged( SetIncreased, 1 );
    return(newrow);
}

// ----------------------------------------------------------------------
//! Return SQL error message.
/*!
  Return the most recent SQL error message;

  \Error Message
*/
// ----------------------------------------------------------------------
QString QmvSqlClass::lastError()
{
    return QString(r_sql_error( dataBase()->dbConnectionID() ));
}
