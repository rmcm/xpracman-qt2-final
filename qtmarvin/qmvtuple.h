/*
 *     $Id: qmvtuple.h 2037 2004-01-19 07:44:00Z source $
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

#ifndef QMVTUPLE_H
#define QMVTUPLE_H

/*
 * ======================================================================
 * DESCRIPTION: 
 *
 * ======================================================================
 */

/*!
  \file   qmvtuple.h
  \brief  QmvTuple Class interface



  \author Rex McMaster, rmcm@compsoft.com.au
  \date   2000-11-02
*/

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <iostream.h>
#include <qdict.h>
#include <qlist.h>
#include <qlistview.h>
#include <qtextstream.h>
#include "qmvtupleattribute.h"
#include "qmvclass.h"

class QmvTupleAttribute;
class QmvClass;
class QmvSqlClass;

/*
 * ======================================================================
 * CLASS DEFINITION : QmvTuple: Base tuple class
 *      - dictionary key: attribute_name
 *      -            
 * ======================================================================
 */
//! Tuple Class
/*!
  Contains the persistent data tuple. Is indexed by attribute names
  of the constituent tuple attributes.

*/
class QmvTuple
    : public QObject,
      public QDict<QmvTupleAttribute>
{
    Q_OBJECT
      public:
    QmvTuple(QmvClass *parent);
    QmvTuple( QmvTuple *tup );
    ~QmvTuple();

    const QmvClass * parentClass();
    QmvTupleAttribute * tupleAttribute(const QString & attname) const;
    const QString attributeValue(const QString & attname) const;
    const QString primarykeyValue();
    const QString userkeyValue();
    const QString userLabelValue() const;
    virtual bool reload( const bool withforeign = FALSE );
    bool isChanged();
    void update(const QString & attname, const QString & attvalue);
    virtual bool purge();
    virtual bool save();
    virtual void init();
    virtual QString lastError();

    void writeXMLData( QTextStream & xml_ts, const QString & prefix, const QString & suffix );

    const QString htmlLabel();
    
    void print();         // for debugging
    void print(const QString & attname);

      signals:
    virtual void tupleChanged( QmvTuple * tp );
    
      protected:    
    QmvClass *parent_class;        // parent class

}; /* CLASS QmvTuple */
/*
 * ======================================================================
 * CLASS DEFINITION : QmvSqlTuple
 *      - contains the persistent data
 *      - dictionary key: attribute_name
 *      -            
 * ======================================================================
 */
//! Tuple Class
/*!
  Contains the persistent data tuple. Is indexed by attribute names
  of the constituent tuple attributes.

*/
class QmvSqlTuple
    : public QmvTuple
{
      public:
    QmvSqlTuple(QmvSqlClass *parent);
    QmvSqlTuple( QmvSqlTuple * tup );
    ~QmvSqlTuple();

    virtual bool reload( const bool withforeign = FALSE );
    virtual bool purge();
    virtual bool save();
    virtual void init();
    virtual QString lastError();


}; /* CLASS QmvSqlTuple */

#endif /* QMVTUPLE_H */
// Local Variables:
// mode:C++
// End:
