/*
 *     $Id: qmvtuple.cpp 2122 2004-02-16 09:56:33Z source $
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
  \file   qmvtuple.cpp
  \brief  Source for QmvTuple

  QmvTuple is a base tupple class which consists of a QDict of
  QmvTupleAttribute indexed by attribute name.

  \author Rex McMaster, rmcm@compsoft.com.au
  \date 2000-11-02 */


/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include "qmvtuple.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvtuple_version[] = "$Id: qmvtuple.cpp 2122 2004-02-16 09:56:33Z source $";

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
//! Constructor
/*!
  The constructor registers the QmvClass parent class, and populates itself
  with relevant QmvTupleAttribute objects.

  \param parent The parent class.
*/
QmvTuple::QmvTuple(QmvClass *parent)
{
    if (!parent)
        qFatal("parent is null");
    
    parent_class = parent; // set private attribute

        // Use the parent class attribute metadata to build the tuple attributes
    const QmvMetaDict * meta = parent->attributeMetadata();
    QDictIterator<QmvAttribute> it_attribute(*meta);
    while (it_attribute.current())
    {
        QmvTupleAttribute *mta = new QmvTupleAttribute(this, it_attribute.current());
        const QString & ta = QString("");
        mta->load(ta);
            // then insert this QmvTupleAttribute into this QmvTuple
        insert(it_attribute.currentKey(), (const QmvTupleAttribute *) mta);
        ++it_attribute;
    }
    setAutoDelete( TRUE );
    
}

// ----------------------------------------------------------------------
//! Constructor - Copy
/*!
  The constructor registers the QmvClass parent class, and populates itself
  with relevant QmvTupleAttribute objects.

  \param parent The parent class.
*/
QmvTuple::QmvTuple( QmvTuple * t )
{
    parent_class = t->parent_class;
    QDictIterator<QmvTupleAttribute> it_attribute(*t);
    while (it_attribute.current() )
    {
        QmvTupleAttribute * old_ta = it_attribute.current();
        QmvTupleAttribute * new_ta = new QmvTupleAttribute( this, old_ta->metaAttribute() );
            // do not copy unique indexes or system attributes
        if ( old_ta->accessMethod() != QmvAttribute::SystemAccess )
        {
                // Text primary keys can be munged, otherwise clear them
                // Non-key attributes are copied.
            if ( old_ta->attributeName() == parent_class->primaryKey() ||
                 old_ta->attributeName() == parent_class->userKey() )
                if ( old_ta->attributeType() == "text" )
                    new_ta->update( QString( "%1_COPY" ).arg(old_ta->currentValue()) );
                else
                    new_ta->update( QString( "" ) );
            else
                new_ta->update( old_ta->currentValue() );
        }
        insert( it_attribute.currentKey(), (QmvTupleAttribute *) new_ta);
        ++it_attribute;
    }
    setAutoDelete( TRUE );
    
}

// ----------------------------------------------------------------------
//! Destructor
/*!

  The destructor removes the child tupleattributes, but does not
  remove the tuple from persistent storage.

*/
QmvTuple::~QmvTuple()
{

//          // Iterate over tuple attributes
//      QDictIterator<QmvTupleAttribute> it_mt(*this);
//      it_mt.toFirst();
//      while (it_mt.current())
//      {
//          delete it_mt.current();
//          ++it_mt;
//      }
}


//! Reload the data from persistent storage
/*!
  Restore the in-memory version of tuple data from the persisten data

  \param withforeign            TRUE/FALSE to include/exclude reload of foreign tuples.
  \return       TRUE/FALSE for success/failure.
*/
bool QmvTuple::reload( bool withforeign )
{
return TRUE;
}


//! Return the Parent Class
/*!
  Return a pointer to the parent class of this tuple.

*/

const QmvClass * QmvTuple::parentClass()
{
    return(parent_class);
}


// ----------------------------------------------------------------------
//! An attribute of this tuple by name
/*!
  Returns a tuple attribute 

  \param attname pointer to the name of the attribute
*/
QmvTupleAttribute * QmvTuple::tupleAttribute(const QString & attname) const
{
    if (this->find(attname))
        return(this->find(attname));
    else
        return(0);
}


// ----------------------------------------------------------------------
//! The value of a attribute
/*!
  Returns the current value of an attribute in this tuple.

  \param attname pointer to the name of the attribute
*/
const QString QmvTuple::attributeValue(const QString &attname) const
{
    if (this->find(attname))
        return(this->find(attname)->currentValue());
    else
        return(0);
}

// ----------------------------------------------------------------------
//! The value of primary key 
/*!
  Returns the value of the primary key of this tuple.

*/
const QString QmvTuple::primarykeyValue()
{
    if (parentClass()->primaryKey())
        return(attributeValue(parentClass()->primaryKey()));
    else
        return 0;
}
// ----------------------------------------------------------------------
//! The value of user key 
/*!
  Returns the value of the user key of this tuple.

*/
const QString QmvTuple::userkeyValue()
{
    if (parentClass()->userKey())
        return(attributeValue(parentClass()->userKey()));
    else
        return 0;
}

// ----------------------------------------------------------------------
//! The value of user label 
/*!
  Returns the value of the user label attribute of this tuple.
  The user label can be of the following forms:
   - a simple text string
   - a attribute name - will be replaced with the current value
   - a \ref QString argument comma-separated list where the first
     argument is a format string (see \ref QString::arg()) with
     parameters %1-%9, plus %+1-9, %++1-9 etc. The subsequent
     arguments can be simple text strings or attribute names.
*/
const QString QmvTuple::userLabelValue() const
{
    QStringList ul_list = parent_class->userLabelList();
    QString ulv;

        // null
    if ( ul_list.count() == 0 )
        return ulv;

        // the first arg may be a QString format list
    if ( tupleAttribute( ul_list[0] ) )
            ulv.append( attributeValue( ul_list[0] ));
    else
        ulv.append(ul_list[0] );

        // muliple arguments, so expand with arg()
    for ( int i = 1; i < ul_list.count(); i ++ )
    {
            // a kludge to get more than 9 arguments
        if ( i > 9 && i % 9 == 1 )
            ulv.replace( QRegExp("%\\+"), "%" );
        if ( tupleAttribute( ul_list[i] ) )
            ulv = ulv.arg( attributeValue( ul_list[i] ));
        else
            ulv = ulv.arg(ul_list[i] );
    }
        
    return(ulv);

}

//! Returns storage state of tuple data.
/*!
  Returns state of tuple data with respect to persistent storage (as
  determined at load time).

  \return TRUE/FALSE
*/
bool QmvTuple::isChanged()
{
    
        // Iterate over tuple attributes
    QDictIterator<QmvTupleAttribute> it_mt(*this);
    while (it_mt.current())
    {
        if (it_mt.current()->isChanged())
            return TRUE;
        ++it_mt;
    }
    return FALSE;
}

// ----------------------------------------------------------------------
//! change the value of the attribute
/*!

  Saves \em attvalue to the QmvTupleAttribute object indexed by \em attname.

  \param attname        Attribute name
  \param attvalue       New attribute value
*/
void QmvTuple::update(const QString & attname, const QString & attvalue)
{
    if (this->find(attname))
        this->find(attname)->update(attvalue);
}


//! Purge from persistent storage
/*!
  Remove the persistent image of this tuple. This d

  \return <ReturnValue>
*/
bool QmvTuple::purge()
{
    return(TRUE);   
}


// ----------------------------------------------------------------------
//! Save the current tuple. 
/*!
  Update it it it already exists, else insert it.

  \return TRUE for success, else FALSE
*/
bool QmvTuple::save()
{
    return(TRUE);   // if nothing to save, so return TRUE
}


// ----------------------------------------------------------------------
//! Initialize the tuple attributes. 
/*!
  Set to initial values

*/
void QmvTuple::init()
{
    ;
}




// ----------------------------------------------------------------------
//! Return error message. 
/*!
  Return the most recent error message;

  \Error Message
*/
QString QmvTuple::lastError()
{
    return QString("");
}


// ----------------------------------------------------------------------
//! Write an XML version of the tuple attributes
/*!
  Write an XML version of the tuple attributes to the QTextStream object \em xml_ts
  using \em prefix and \em suffix

  \param xml_ts         the target QTextStream object
  \param prefix         attribute prefix
  \param suffix         attribute suffix
*/
void QmvTuple::writeXMLData( QTextStream & xml_ts, const QString & prefix, const QString & suffix )
{
    xml_ts << prefix << "<tuple>" << endl;
    QDictIterator<QmvTupleAttribute> it_ta(*this);
    while (it_ta.current())
    {
        xml_ts << prefix << "  ";
        it_ta.current()->writeXMLData( xml_ts );
        xml_ts << suffix;
        ++it_ta;
    }
    xml_ts << prefix << "</tuple>" << endl;
    
}


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTuple::htmlLabel()
{
    QString label;
    QTextOStream x_out(&label);

        // remove plurals
    QString title = parentClass()->relationTitle() ;
    title.replace( QRegExp("s$"), "" );
    
    x_out << "<table width=100% cellpadding=0 cellspacing=0>";
    x_out << QString("<tr><th bgcolor=yellow align=left colspan=2>%1 Details</th></tr>")
        .arg( title);

    QmvAttList attlist = parent_class->sortedAttlist( "mtat_view_order",
                                                      QmvClass::NoBlank|QmvClass::NoSystem|QmvClass::NoZero );

    for (QmvAttribute * at_ptr = attlist.first(); at_ptr != 0; at_ptr = attlist.next())
    {
        QString value = attributeValue(at_ptr->attributeName());
        switch ( at_ptr->inputMethod() )
        {
            case QmvAttribute::Colour:
                    // strip the alpha value (expecting form .. #AARRGGBB
                value.replace(1, 2, "");
                x_out << QString("<tr><td>%1:</td><td bgcolor=%2></td></tr>")
                    .arg(at_ptr->attributeTitle())
                    .arg(value);

                break;

            default:
                x_out << QString("<tr><td>%1:</td><td><strong>%2</strong></td></tr>")
                    .arg(at_ptr->attributeTitle())
                    .arg(value);
                break;
        }
    }
    
    x_out << "</table>";
    
    return label;
}

// ----------------------------------------------------------------------
//! Print the value of the tuple
/*!
  Prints the entire tuple, including the values of each attribute.

*/
void QmvTuple::print()
{
  QDictIterator<QmvTupleAttribute> it_mt(*this);
  while (it_mt.current())
    {
      it_mt.current()->print(it_mt.currentKey());
      ++it_mt;
    }
}


// ----------------------------------------------------------------------
//! Print the value of an attribute of this tuple
/*!


  \param name   Attribute name
*/
void QmvTuple::print(const QString &name)
{
  if (this->find(name))
    this->find(name)->print();
}

/*
 * ======================================================================
 * MEMBER FUNCTIONS: QmvSqlTuple
 * ======================================================================
 */

// ----------------------------------------------------------------------
//! Constructor
/*!
  The constructor registers the QmvClass parent class, and populates itself
  with relevant QmvTupleAttribute objects.

  \param parent The parent class.
*/
QmvSqlTuple::QmvSqlTuple(QmvSqlClass *parent)
        : QmvTuple( parent )
{
 
}

// ----------------------------------------------------------------------
//! Copy Constructor
/*!
  The constructor registers the QmvClass parent class, and populates itself
  with relevant QmvTupleAttribute objects.

  \param parent The parent class.
*/
QmvSqlTuple::QmvSqlTuple( QmvSqlTuple *tp)
        : QmvTuple( tp )
{
 
}

// ----------------------------------------------------------------------
//! Destructor
/*!

  The destructor removes the child tupleattributes, but does not
  remove the tuple from persistent storage.

*/
QmvSqlTuple::~QmvSqlTuple()
{

//          // Iterate over tuple attributes
//      QDictIterator<QmvTupleAttribute> it_mt(*this);
//      it_mt.toFirst();
//      while (it_mt.current())
//      {
//          delete it_mt.current();
//          ++it_mt;
//      }
}
 

//! Reload the data from persistent storage
/*!
  Restore the in-memory version of tuple data from the persisten data

  \param withforeign            TRUE/FALSE to include/exclude reload of foreign tuples.
  \return       TRUE/FALSE for success/failure.
*/
 
bool QmvSqlTuple::reload( bool withforeign )
{
    
    QString query;
    const QString & cn = parent_class->relationName();
    const QString & pk = parent_class->serialKey();
    const QString & pkv = this->attributeValue(pk);
    int rows;

    if (pkv)  // This tuple exists in persistent storage.
    {
            
        query = QString("SELECT %1 FROM %2 WHERE %3 = '%4'::%5")
            .arg(parent_class->SQLSelectList())
            .arg(cn)
            .arg(pk)
            .arg(pkv)
            .arg(parent_class->attAttValue(pk, QString("mtat_type")));
        rows = sql_exec(query);
        if (rows != 1)
        {
            qDebug("QmvSqlTuple::reload():lastError = <%s>", lastError().latin1() );
            return(FALSE);
        }

            // reload tuple from persistent storage
        QDictIterator<QmvTupleAttribute> it_mt(*this);
        it_mt.toFirst();
        while (it_mt.current())
        {
            it_mt.current()->load( QString(sql_value(0, it_mt.currentKey().latin1())) );
            ++it_mt;
        }
    }

    return(TRUE);
    
}

//  bool QmvSqlTuple::loadForeign( const QString & att_name )
//  {
//      QmvTupleAttribute * 
bool QmvSqlTuple::purge()
{
    
    QString query;
    const QString & cn = parent_class->relationName();
    const QString & pk = parent_class->serialKey();
    const QString & pkv = this->attributeValue(pk);
    int rows;

    if (pkv)  // This tuple exists in persistent storage.
    {
            
        query = QString("DELETE FROM %1 WHERE %2 = '%3'::%4")
            .arg(cn)
            .arg(pk)
            .arg(pkv)
            .arg(parent_class->attAttValue(pk, QString("mtat_type")));
        rows = sql_exec(query);
        if (rows  < 0 )
        {
            qDebug("QmvSqlTuple::purge():lastError = <%s>", lastError().latin1() );
            return(FALSE);
        }
        else
            return(TRUE);
    }

    return(TRUE);
    
}


// ----------------------------------------------------------------------

//! Save the current tuple. 
/*!
  \fn  QmvSqlTuple::save()
  Update it it it already exists, else insert it.

  \return TRUE for success, else FALSE
*/
bool QmvSqlTuple::save()
{

    QString sql_setlist;
    QString sql_att_list;
    QString sql_val_list;
    QString attnam, attval;
    const QString x_holder( "__@@__@@__@@__");
    
    QString attatt = "mtat_type";
    
        // Iterate over tuple attributes and build a "SET var=val" list
        // from changed attributes.
    QDictIterator<QmvTupleAttribute> it_mt(*this);
    while (it_mt.current())
    {
        if (it_mt.current()->isChanged() && it_mt.current()->inputMethod() != QmvAttribute::Virtual )
        {
            if (sql_setlist.length())
                sql_setlist.append(",");
            if (sql_att_list.length())
                sql_att_list.append(",");
            if (sql_val_list.length())
                sql_val_list.append(",");
                // attribute name and value
            attnam = it_mt.currentKey();
            attval = it_mt.current()->currentValue();
            attval = QmvClass::cleanForSQL(attval);
            attval.replace( QRegExp("%"), x_holder );

                // timestamps do not handle '' - use null
            QString newvalue;
            if (attval.length() == 0 || attval.lower() == "null")
                newvalue = "null";
            else
                newvalue = QString("'%1'").arg(attval);
            
                // Build a set-list
            sql_setlist += QString("%1=%2::%3")
                .arg(attnam)
                .arg(newvalue)       // current data value - null adjusted
                .arg(parent_class->attAttValue(attnam, QString("mtat_type")));
                // build an attribute list
            sql_att_list += QString("%1")
                .arg(attnam);
                // build a values list
            sql_val_list += QString("'%1'::%2")
                .arg(attval)       // current data value
                .arg(parent_class->attAttValue(attnam, QString("mtat_type")));
        }
            //it_mt.current()->print();
        ++it_mt;
    }
        // --------------------------------------------------------------------------------
        // Build a query string, the form of which depends on the current state
        // of the tuple. The tuple is then stored, and recovered to update the values
        // of all attributes.
        // --------------------------------------------------------------------------------
    
    QString query;
    QString new_serial_pkv;    // new primary key where exists specific SQL insert function
    
    const QString & cn = parent_class->relationName();
    const QString & pk = parent_class->serialKey();
    QString pkv = this->attributeValue(pk);
    int rows;
    int have_used_list = 0;

        // ------------------------------------------------------------------
        // If there is a currently a serial key, then the tuple must already
        // exist in persistent storage, otherwise try to insert it.
        // ------------------------------------------------------------------
    if ( pkv.length() < 1 )
    {

            // initialise to defaults
            // First try for specific insert function - inserts via views cannot
            // be located via oid's. Hence the first method to try is to look
            // for an "insert function".
        query = QString( "SELECT %1_insert_row() as new_sequence" ).arg( cn );
        rows = sql_exec(query);
        if ( rows == 1 )
        {
            pkv = QString(sql_value(0, "new_sequence" ) );
                
        } else { // else use generic insert method

            if (sql_setlist.length())       // something specific to insert
            {
                query = QString("INSERT INTO %1(%2) VALUES(%3)")
                    .arg(cn)
                    .arg(sql_att_list)
                    .arg(sql_val_list);
                query.replace( QRegExp(x_holder), "%" );
                rows = sql_exec(query);
                if ( rows != 1 )
                    return(FALSE);
                have_used_list = 1;
            } else {
            
                query = QString( "INSERT INTO %1 default values" ).arg( cn );
                rows = sql_exec(query);
                if ( rows != 1 )
                    return(FALSE);
            }
            
                // recover pkv
            query = QString("SELECT %1 FROM %2 where oid = %3")
                .arg(pk)
                .arg(cn)
                .arg(sql_getlastoid());
            rows = sql_exec(query);
            if ( rows != 1 )
            {
                return(FALSE);
            }
            
            pkv = QString(sql_value(0, pk ) );
        
        }
        
            // Now recover the new record
        
        query = QString("SELECT * FROM %1 where %2 = %3")
            .arg(cn)
            .arg(pk)
            .arg(pkv);
        if (sql_exec(query) == 1)
        {    // successfully recovered, so apply to tuple
            QDictIterator<QmvAttribute> it_attribute(*parent_class->attributeMetadata());
            while (it_attribute.current())
            {
                QmvTupleAttribute * mta = tupleAttribute(it_attribute.currentKey());
                const QString & ta = QString(sql_value(0, it_attribute.currentKey().latin1()));
                mta->load(ta);
                ++it_attribute;
            }
        } else
            return(FALSE);
 

    }
    
        // ------------------------------------------------------------
        // By now should have persistent tuple.
        // - update it if the values have not been used in the INSERT.
        // ------------------------------------------------------------
    if ( pkv.length() > 0 )
    {       
        if (sql_setlist.length() && !have_used_list )       // something to change
        {
            query = QString("UPDATE %1 SET %2 WHERE %3 = '%4'::%5")
                .arg(cn)
                .arg(sql_setlist)
                .arg(pk)
                .arg(pkv)
                .arg(parent_class->attAttValue(pk, QString("mtat_type")));
            query.replace( QRegExp(x_holder), "%" );
            rows = sql_exec(query);
            qDebug("QmvSqlTuple::save():rows = <%d>", rows);
                // updating views returns 0 even if underlying update returns 1
            if ( rows < 0 )
            {
                return(FALSE);
            }
        }
            // ------------------------------------------------------------------
            // No serial key, so irrecoverble
            // ------------------------------------------------------------------
    } else
        return(FALSE);

        // ------------------------------------------------------------------
        // Reload tuple
        // ------------------------------------------------------------------
    if ( reload() )
    {
        it_mt.toFirst();
        while (it_mt.current())
        {
            it_mt.current()->setSaved();
            ++it_mt;
        }
        emit tupleChanged( this );
        return(TRUE);
    } else
        return(FALSE);
      
}

// --------------------------------------------------------------------------------
//! Iniialize this tuple.
/*!


*/
// -------------------------------------------------------------------------------
void QmvSqlTuple::init()
{
        // reload tuple from persistent storage
    QDictIterator<QmvTupleAttribute> it_mt(*this);
    it_mt.toFirst();
    while (it_mt.current())
    {
        it_mt.current()->update( "" );
        it_mt.current()->setSaved();
        ++it_mt;
    }
}


//! Return SQL error message.
/*!
  Return the most recent SQL error message;

  \Error Message
*/
QString QmvSqlTuple::lastError()
{
    return QString(sql_error());
}

