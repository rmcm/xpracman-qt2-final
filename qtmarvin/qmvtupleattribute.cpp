/*
 *     $Id: qmvtupleattribute.cpp 2092 2004-02-09 05:36:33Z source $
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



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <iostream.h>
#include "qmvtupleattribute.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
static char qmvtupleattribute_version[] = "$Id: qmvtupleattribute.cpp 2092 2004-02-09 05:36:33Z source $";

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
  Constructor for tupleattribute

  \param       QmvClass *parent - the parent class
  \param       QmvAttribute *meta - the meta data for this tuple attribute
*/
// ----------------------------------------------------------------------
QmvTupleAttribute::QmvTupleAttribute(QmvTuple * parent, QmvAttribute * meta)
        : parenttuple(parent), metaattribute(meta), original_value(""), current_value(""),
          local_colour_group(false), colour_group(QApplication::palette().active())
{
}

// ----------------------------------------------------------------------
//! Copy Constructor
/*!
  Copy Constructor for tupleattribute

  \param       QmvClass *parent - the parent class
  \param       QmvTupleAttribute *ta - an existing object
*/
// ----------------------------------------------------------------------
QmvTupleAttribute::QmvTupleAttribute( QmvTuple * parent, QmvTupleAttribute * ta  )
        : parenttuple(parent), metaattribute(ta->metaAttribute()),
          original_value(""), current_value(ta->currentValue()),
          local_colour_group(false), colour_group(QApplication::palette().active())
{
}

    
// ----------------------------------------------------------------------
//! Destructor
/*!

*/
// ----------------------------------------------------------------------
QmvTupleAttribute::~QmvTupleAttribute()
{
}

// ----------------------------------------------------------------------
//! Return the parent tuple of this attribute.
/*!
  
  Return the parent tuple of this attribute.

*/
// ----------------------------------------------------------------------
QmvTuple * QmvTupleAttribute::parentTuple()
{
    return parenttuple;
}


// ----------------------------------------------------------------------
//! Return the meta-attribute class of this attribute.
/*!
  
  Return the meta-attribute class of this attribute.

*/
// ----------------------------------------------------------------------
QmvAttribute * QmvTupleAttribute::metaAttribute()
{
    return metaattribute;
}



// ----------------------------------------------------------------------
// The attributes attributes from persistent storage
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QmvAttribute * QmvTupleAttribute::metaForeign() const
{
    return (metaattribute) ? metaattribute->metaForeign() : 0 ;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeAccess() const
{
    return metaattribute->attributeAccess();
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeName() const
{
    return metaattribute->attributeName();
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeType() const
{
    return metaattribute->attributeType();
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeLength() const
{
    return metaattribute->attributeLength();
}
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeAttributes() const
{
    return metaattribute->attributeAttributes();
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeOperator() const
{
    return metaattribute->attributeOperator();
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeTitle() const
{
    return metaattribute->attributeTitle();
}
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeClassName() const
{
    return metaattribute->attributeClassName();
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeDisplen() const
{
    return metaattribute->attributeDisplen();
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeBrowseOrder()const
{
    return metaattribute->attributeBrowseOrder();
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeViewOrder() const
{
    return metaattribute->attributeViewOrder();
}
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeInputMethod()const
{
    return metaattribute->attributeInputMethod(); 
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeDefault() const
{
    return metaattribute->attributeDefault();
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeHtmltagatt() const
{
    return metaattribute->attributeHtmltagatt();
}
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeUserEntry() const
{
    return metaattribute->attributeUserEntry();
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeStatus() const
{
    return metaattribute->attributeStatus();
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeTimestamp() const
{
    return metaattribute->attributeTimestamp();
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::attributeSequence() const
{
    return metaattribute->attributeSequence();
}

// ----------------------------------------------------------------------
// Some attribute-attribtues mapped to internal form
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QmvAttribute::data_type QmvTupleAttribute::dataType() const
{
    return metaattribute->dataType();
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QmvAttribute::input_method QmvTupleAttribute::inputMethod() const
{
    return metaattribute->inputMethod();
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QmvAttribute::access_method QmvTupleAttribute::accessMethod() const
{
    return metaattribute->accessMethod();
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QStringList QmvTupleAttribute::getLabelList() const
{
    return metaattribute->getLabelList();
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::getValueByLabel( const QString & label) const
{
    return metaattribute->getValueByLabel( label );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const int QmvTupleAttribute::getIndexByLabel( const QString & label) const
{
    return metaattribute->getIndexByLabel( label );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::getLabelByValue( const QString & value) const
{
    return metaattribute->getLabelByValue( value );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::getUserLabelByValue( const QString & value) const
{
    return metaattribute->getUserLabelByValue( value );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
int QmvTupleAttribute::extendList( const QString & search, bool labeltoo, bool replace, bool fuzzy )
{
    return metaattribute->extendList( search, labeltoo, replace, fuzzy );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
int QmvTupleAttribute::addToList( const QString & value, const QString & label )
{
    return metaattribute->addToList( value, label );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
QString QmvTupleAttribute::formatTimestamp( const QString & ts ) const
{
    int displen = attributeDisplen().toInt();
    if ( ts.isNull()
         || displen < 1
         || ts.length() < displen
             //|| !ts[0].isDigit()
         )
        return ts;

    return ts.left( displen );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTupleAttribute::setColourGroup( QColorGroup cg)
{
    colour_group = cg;
    local_colour_group = true;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const bool QmvTupleAttribute::isColourGroupSet() const
{    
        //QColorGroup cg = QApplication::palette().active();
    return local_colour_group;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const QColorGroup QmvTupleAttribute::colourGroup() const
{    
        //QColorGroup cg = QApplication::palette().active();
    return colour_group;
}

// ----------------------------------------------------------------------
//! Is an alert set?
/*!
  Return whether an alert was set by the last dataload (\ref load())

  \return true/false
*/
// ----------------------------------------------------------------------
bool QmvTupleAttribute::isAlert() const
{    
    return alert;
}

// ----------------------------------------------------------------------
const QColor QmvTupleAttribute::alertColour() const
{
    return metaattribute->alert(current_value);
}
// ----------------------------------------------------------------------
//! Initialise this tupleattribute with new data.
/*!
  Sets both original_value and current_value.

  \param data   pointer to QString object
*/
// ----------------------------------------------------------------------
void QmvTupleAttribute::load(const QString & data)
{
    original_value = data;

        // Conversions based on datatype
    switch ( dataType() )
    {
        case QmvAttribute::Timestamp:
            if ( data.length() > 0 )
                original_value = formatTimestamp( data );
            break;

        default:
            break;
    }
    
        // conversions based on input method
    switch ( inputMethod() )
    {
        case QmvAttribute::Colour:
                // convert to presentation form #XXXXXX
            bool ok;
            original_value.replace( QRegExp("#"), "").toUInt( &ok, 16);
            if (ok)
            {
                original_value = original_value.rightJustify(8, '0');
                original_value.prepend("#");
            }
            break;

//         case QmvAttribute::List:
//             colour_group.setColor( QColorGroup::Base, QColor("yellow") );
//             setColourGroup(colour_group);
//             break;
            
        default:
            break;
    }

    current_value = original_value;
    QColor alert_colour = alertColour();
    alert  = alert_colour.isValid();
    if ( alert )
    {
        colour_group.setColor( QColorGroup::Base, alert_colour );
        setColourGroup(colour_group);
    } else
        local_colour_group = false;
            
}

// ----------------------------------------------------------------------
//! Load the foreign tuple
/*!
  Return a \ref QmvClass object loaded with foreign tuple if this attribute
  is a foreign key.

*/
// ----------------------------------------------------------------------
const QmvClass * QmvTupleAttribute::loadForeignKey()
{
    const QmvClass * fk_class = metaattribute->foreignRelation( currentValue() );

    return fk_class;
}

// ----------------------------------------------------------------------
//! Update the current value
/*!     Change the current value - this does not save the data in
  persistent storage. Refer to QmvTuple::save() for details of saving
  tuples to persistent storage.
  

  \param data   the data for new attribute value
*/
// ----------------------------------------------------------------------
void QmvTupleAttribute::update(const QString & data)
{
        current_value = data;
}

// ----------------------------------------------------------------------
//! Restore the current value
/*!     Restore the current value - this does not save the data in
  persistent storage. Refer to QmvTuple::save() for details of saving
  tuples to persistent storage.

*/
// ----------------------------------------------------------------------
void QmvTupleAttribute::restore()
{
  current_value = original_value;
}

// ----------------------------------------------------------------------
//! Has the attribute value changed?
/*!

   Determine if the in-memory attribute value matches the value in
   persistent storage.

  \return TRUE if changed, else FALSE
*/
// ----------------------------------------------------------------------
bool QmvTupleAttribute::isChanged()
{
    if (current_value == original_value)
        return(FALSE);
    else
        return(TRUE);
}

// ----------------------------------------------------------------------
//! Set the original value to current value
/*!
  Set the original value to the current value. This is called by \ref QmvTuple::save()
  after sucessfull save to persistent storage.

*/

// ----------------------------------------------------------------------
void QmvTupleAttribute::setSaved()
{
    original_value = current_value;
}

// ----------------------------------------------------------------------
//! The original value of the attribute
/*!
  Returns the value of the attribute in persistent storage.


*/
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::originalValue()
{
  return(original_value);
}


// ----------------------------------------------------------------------
//! The current value of the attribute
/*!
  Returns the current value of the attribute in memory.

*/
// ----------------------------------------------------------------------
const QString QmvTupleAttribute::currentValue()
{
    current_value.latin1(); // DEBUG
    return(current_value);
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
bool QmvTupleAttribute::writeXMLDeclaration( QTextStream & xml_ts )
{
    return metaattribute->writeXMLDeclaration( xml_ts );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
bool QmvTupleAttribute::writeXMLData( QTextStream & xml_ts )
{
    return metaattribute->writeXMLData( xml_ts, current_value );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//! Print the original and current attribute values.
/*!

   Prints the original and current attribute values to stdout - used
   for debugging.

  \param        
*/
// ----------------------------------------------------------------------
void QmvTupleAttribute::print()
{
  cout << original_value << "|" << current_value << endl;
}

// ----------------------------------------------------------------------
//! Print the original and current attribute values with a label.
/*!

   Prints the original and current attribute values to stdout - used
   for debugging. Prepend a label to each line.

  \param label A label to prefix output.
*/
// ----------------------------------------------------------------------
void QmvTupleAttribute::print(const QString & label)
{
    cout << label << "|" << original_value << "|" << current_value << endl;
}

