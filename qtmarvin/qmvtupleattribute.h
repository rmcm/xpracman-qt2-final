/*
 *     $Id: qmvtupleattribute.h 1904 2003-10-09 02:11:11Z source $
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

#ifndef QMVTUPLEATTRIBUTE_H
#define QMVTUPLEATTRIBUTE_H

/*
 * ======================================================================
 * DESCRIPTION: 
 *
 * ======================================================================
 */

/*!
  \file   qmvtupleattribute.h
  \brief  QmvTupleAttribute interface


  \author Rex McMaster, rmcm@compsoft.com.au
  \date   2000-11-02
*/


/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <qstring.h>
#include <qlist.h>
#include <qlistview.h>
#include <qguardedptr.h>
#include <qtextstream.h>
#include "qmvtuple.h"
#include "qmvattribute.h"

class QString;
class QmvAttribute;
class QmvTuple;


/*
 * ======================================================================
 * CLASS DEFINITION : QmvTupleAttribute
 *      - contains the real data
 * ======================================================================
 */

class QmvTupleAttribute : public QObject
{
    Q_OBJECT
    
      public:
    QmvTupleAttribute(QmvTuple *parent, QmvAttribute * meta);
    QmvTupleAttribute( QmvTuple *parent, QmvTupleAttribute * ta  );
    ~QmvTupleAttribute();

    QmvTuple * parentTuple();
    QmvAttribute * metaAttribute();
    const QmvAttribute * metaForeign() const;
        // The attributes attributes from persistent storage
    const QString attributeAccess() const;
    const QString attributeName() const;
    const QString attributeType() const;
    const QString attributeLength() const;
    const QString attributeAttributes() const;
    const QString attributeOperator() const;
    const QString attributeTitle() const;
    const QString attributeClassName() const;
    const QString attributeDisplen() const;
    const QString attributeBrowseOrder() const;
    const QString attributeViewOrder() const;
    const QString attributeInputMethod() const;
    const QString attributeDefault() const;
    const QString attributeHtmltagatt() const;
    const QString attributeUserEntry() const;
    const QString attributeStatus() const;
    const QString attributeTimestamp() const;
    const QString attributeSequence() const;

        // Some attribute-attribtues mapped to internal form

    const QmvAttribute::data_type dataType() const;
    const QmvAttribute::input_method inputMethod() const;
    const QmvAttribute::access_method accessMethod() const;
    const QStringList getLabelList() const;
    const QString getValueByLabel( const QString & label) const;
    const int getIndexByLabel( const QString & label) const;
    const QString getLabelByValue( const QString & value) const;
    const QString getUserLabelByValue( const QString & value) const;
    int extendList( const QString & search, bool labeltoo, bool replace, bool fuzzy );
    int addToList( const QString & value, const QString & label );
    QString formatTimestamp( const QString & ts ) const;
    void setColourGroup( QColorGroup cg);
    const bool isColourGroupSet() const;
    const QColorGroup colourGroup() const;
    
    void load(const QString & data);
    const QmvClass * loadForeignKey();
    void update(const QString & data);
    void restore();
    bool isChanged();
    void setSaved();
    bool isAlert() const;
    const QColor alertColour() const;
    const QString originalValue();
    const QString currentValue();
    bool writeXMLDeclaration( QTextStream & xml_ts );
    bool writeXMLData( QTextStream & xml_ts );
    void print(const QString & label);
    void print();

      private:
    QmvTuple *parenttuple;              //!< The parent QmvTuple of this attribute
    QmvAttribute *metaattribute;        //!< The meta data for this tuple attribute
    QGuardedPtr<QmvTuple> foreigntuple; //!< The foreign QmvTuple if this is a foreign key
    bool local_colour_group;
    QColorGroup colour_group;
    bool alert;                         //!< Set if alert is set.

//! Original, persistent value of the attribute
/*!
  
 */
    QString original_value;  // Data as loaded

//! Current, in-memory value of the attribute
/*!
  
 */
    QString current_value;    // possibly mutated data

}; /* CLASS QmvTupleAttribute */


#endif /* QMVTUPLEATTRIBUTE_H */
// Local Variables:
// mode:C++
// End:
