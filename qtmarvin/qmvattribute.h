/*
 *     $Id: qmvattribute.h 3020 2006-09-05 02:56:16Z source $
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

#ifndef QMVATTRIBUTE_H
#define QMVATTRIBUTE_H

/*
 * ======================================================================
 * DESCRIPTION: 
 *
 * ======================================================================
 */
/*!
  \file   qmvattribute.h
  \brief  QmvAttribute Interface

  <long description>

  \author Rex McMaster, rmcm@compsoft.com.au
  \date   2000-11-02
*/

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */

#include <qstring.h>
#include <qstringlist.h>
#include <qdict.h>
#include <qtextstream.h>
#include <qlist.h>
#include <qcolor.h>
#include <qdatetime.h>

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */
//!  contains attribute meta information
/*!
 *      - contains attribute meta information
 *              - NOT data (see QmvTuple)
 *      - dictionary key: attribute_name of meta_attribute

*/

class QmvClass;
class QmvSqlClass;

class QmvAttribute
    : public QDict<QString>
{
      public:
    QmvAttribute( QmvClass * reln );    // Constructor
    ~QmvAttribute();  // Destructor

    QmvClass * parentRelation() const;
    const QString attributeAttribute(const QString & att) const;
        // Hardwired wrappers for attributeAttribute()
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

    enum data_type { TypeUnknown, Text, Integer, Numeric, Timestamp, Boolean };
    enum input_method { InputUnknown, Direct, MultiLine, List, Colour,
                        DateTime, DateOnly, DateNoText, TimeOnly, CopyText, Virtual };
    enum access_method { AccessUnknown, Invisible, ReadOnly, WriteOnly, ReadWrite, SystemAccess };
    enum special_chars {
        SLSeparator = 0x3B        // ";"
    };
        // CopyText structure
    struct copyTextMeta
    {
        copyTextMeta() 
                : reln(0) {}
        
        QString name;           // name of relation
        QString key;            // search key
        QString src;            // data source field
        QString dst;            // destination field in the current tuple
        QmvSqlClass *reln;      // source relation object
    };
    
    const QString virtualAttribute() const;
    const data_type dataType() const;
    const bool autoSortList() const;
    void setAutoSortList( bool enable = TRUE );
    const input_method inputMethod() const;
    const access_method accessMethod() const;
    const bool getEditPrompting() const;
    const int listLimit() const;
    void setdataType( const data_type val = TypeUnknown );
    void setInputMethod( const input_method val = InputUnknown );
    void setAccessMethod( const access_method val = AccessUnknown );
    void setEditPrompting( const bool do_prompt );
    void setListLimit( int listlimit );
    
    const QStringList getLabelList() const;
    const QString getValueByLabel( const QString & label ) const;
    const int getIndexByLabel( const QString & label ) const;
    const QString getLabelByValue( const QString & value ) ;
    void clearList();
    void setList( const QString & values, const QString & sep1, const QString & sep2 );
    int extendList( const QString & search, bool labeltoo = FALSE,
                    bool replace = FALSE, bool fuzzy = FALSE );
    bool addToList( const QString & value, const QString & label );
    const QString fkQueryString( const QString & value ) const;
    const QString getUserLabelByValue( const QString & value );
    const copyTextMeta & copyText() const;
    const QString copyTextDestination();
    
    const QmvAttribute * metaForeign() const;
    QmvClass * foreignRelation() const;
    const QmvClass * foreignRelation( const QString fk_value ) const;
        
    void setForeign( QmvAttribute const * fkmeta,  QmvClass * fkrel );
    bool writeXMLDeclaration( QTextStream & xml_ts );
    bool writeXMLData( QTextStream & xml_ts, const QString & data_str );
    void print( QString name );
    void print();
    
    enum operator_types { Matches, Like, Contains, Equals, GreaterThan, LessThan,
                          GreaterThanOrEqual, LessThanOrEqual, NotEqual, All };

    void addAlert( const int priority, const operator_types op, const QString & value, const QString & colour );
    QColor alert( const QString & compare );
    QColor alert( const QDateTime & compare );
    
      private:
    QmvClass * parent;
    data_type type;
    input_method input;
    bool edit_prompting;
    bool autosortlist;
    int limit_list;
    int multiline;
    access_method access;
    QString virtual_att;                        //<! virtual attribute - eg computed value
    QDict<QString> label_dict;
    QDict<QString> value_dict;
    QDict<int> index_dict;
    QStringList label_list;
    
    QmvAttribute const * foreign_metadata;      //<! meta data for the foreign relation if exists
    QmvClass * foreign_relation;                //<~ instantiated foreign relation object

    copyTextMeta copy_text;
    
    struct alertItem
    {
        int priority;
        operator_types op;
        QString value;
        QString colour;
    };
    
    typedef QList<alertItem>             QmvAlertListBase;
    typedef QListIterator<alertItem>     QmvAlertListIterator;

    class Q_EXPORT QmvAlertList : public QmvAlertListBase
    {
          public:
        QmvAlertList( bool deepCopies=TRUE ) { dc = deepCopies; del_item = deepCopies; }
        QmvAlertList( const QmvAlertList & );
        ~QmvAlertList()                  { clear(); }
        QmvAlertList& operator=( const QmvAlertList & );

          private:
        int compareItems( QCollection::Item s1, QCollection::Item s2 )
            {
                int i = ((const alertItem*)s1)->priority - ((const alertItem*)s2)->priority;
                if ( i > 0 )
                    return 1;
                else if ( i < 0 )
                    return -1;
                else return 0;
            }

        bool dc;

    };
    
    QmvAlertList alert_list;                    //!< list of alerts
    
}; /* CLASS QmvAttribute */

#include "qmvclass.h"

#endif /* QMVATTRIBUTE_H */
// Local Variables:
// mode:C++
// End:
