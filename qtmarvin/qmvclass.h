/*
 *     $Id: qmvclass.h 2889 2006-06-11 11:47:27Z source $
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

#ifndef QMVCLASS_H
#define QMVCLASS_H

/*
 * ======================================================================
 * DESCRIPTION: 
 *
 * ======================================================================
 */
/*!
  \file   qmvclass.h
  \brief  Header for the QmvClass

  Defines the interface to the QmvClass class

  \author Rex McMaster, rmcm@compsoft.com.au
  \date   2000-11-02
*/

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <qstring.h>
#include <qdict.h>
#include <qlistview.h>
#include <qtextstream.h>
#include <qprogressdialog.h>
#include <qdatetime.h>
#include "qmvdb.h"
#include "qmvattlist.h"
#include "qmvattribute.h"
#include "qmvtupleattribute.h"

class QString;
class QListView;
class QListViewItem;
class QmvDB;
class QmvTuple;
class QmvSqlTuple;
class QmvList;
class QmvAttList;

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */
/*
 * ======================================================================
 * CLASS DEFINITIONS : QmvClass
 * ======================================================================
 */

typedef QDict<QmvAttribute> QmvMetaDict;

class QmvClass : public QObject
{
    Q_OBJECT
    
      public:
    enum ChangeType { NewBlankSet, NewSet, SetReduced, SetIncreased, SetChanged, SingleChanged };
    enum ListScope  { AttAll, AttViewable, AttEditable };
    enum AttlistFilter { NoBlank=1, NoReadOnly=2, NoForeignKey=4, NoSystem=8, NoZero=16, NoNeg=32, NoPos=64 };
        
    QmvClass( QmvDB * db, const QString name, const int rellevel = 0 );    // Constructor
    QmvClass( const QmvClass & rel ); // copy
    ~QmvClass();  // Destructor

    
    virtual const bool createMetadata(QString name);
    QmvDB * dataBase() const;
    QmvMetaDict * attributeMetadata();
    QmvMetaDict * foreignMetadata();
    QmvMetaDict * relationMetadata();
    const QString classMetaDataAttribute( const QString & attname) const;
    const QString relationName() const;
    const QString relationTitle() const;
    const QString serialKey() const;
    const QString primaryKey() const;
    const QString userKey() const;
    const QString userLabel() const;
    const QStringList userLabelList() const;
    const QString userLabelValue( int row );
    const QString currentQuery() const;
    const QString access() const;
    const QString group() const;
    const QString matrixOrder() const;
    const QString orderBy() const;
    const QString groupBy() const;
    const QString queryLimit() const;
    const QString queryOffset() const;
    const QString groupName() const;


    
    virtual const QString queryDescription();

    const QmvAttribute * attribute( const QString & attname ) const;
    const QString attributeNamebyTitle( const QString & att_title );
    
        // foreign relation methods
    virtual int loadForeignRelations( const QStringList & fkrel, int const limit = 0 );
    QStringList listForeignRelations();
    QStringList listForeignKeys();
    QmvClass * foreignRelation( const QString &name ) const;
    const QString foreignRelationAttribute( const QString & attname, const QString & fratt ) const;
    const QString foreignRelationName( const QString & attname ) const;
    const QStringList foreignKey( const QString & attname ) const;
    const QStringList foreignJoin( const QString & attname ) const;
    const QString foreignView( const QString & attname ) const;
    const QString selectForeignQuery( const QString & attname, QStringList localvalue,
                                      int local=0 ) const;
    
        // child relation methods
    virtual void loadChildRelations( const int rellevel = 0 );
    QStringList listChildRelations();
    QmvClass * childRelationByName( const QString &name ) const;
    const QString childRelationAttribute( const QString & childname, const QString & fratt ) const;
    const QString childTitle( const QString &name ) const;
    const QString childRelationName( const QString & attname ) const;
    const QStringList childKey( const QString & childname ) const;
    const QStringList childJoin( const QString & childname ) const;
    const QString childView( const QString & childname ) const;
    const QString selectChildQuery( const QString & childname, QStringList & localvalue,
                                    const QString & sep ) const;
    
    
    QString attAttValue(const QString &attname, const QString &attattname);
    QmvAttList edit_attlist();
    QmvAttList view_attlist();
    QmvAttList sortedAttlist(  QString const & index, QStringList & atnames );
    QmvAttList sortedAttlist(  QString const & index, int filter );
    QStringList attributeList( const int group );
    const QString SQLSelectList();

    const QDict<QmvClass> * classRelations();

    QmvTuple * createTuple( QmvTuple * existing = 0 );
    virtual void deleteTuple( QmvTuple * tup );
    int open( int rows );
    virtual int open(const QString &query);
    virtual int open(const QString & query, QProgressDialog * controller );
    uint count() const;
    int rowByPK( const QString & pk );
    int rowBySK( const QString & sk );
    QmvTuple * tupleAt(int row);
    QmvTupleAttribute * tupleAttribute( int row, const QString &attname);
    const QString attributeValue( int row, const QString &attname);

    int save();
    bool save(int row);
    bool save( QmvTuple * tup );
    void update(int row, const QString &name, const QString &value);
    virtual int newTuple();
    virtual int copyTuple( int row );
    bool remove(int row, bool rm_persistent = FALSE );
    bool remove(QmvTuple * tuple, bool rm_persistent = FALSE );
    bool writeXMLDeclaration( QTextStream & xml_ts );
    bool writeXMLData( QTextStream & xml_ts );
    const QString htmlLabel( const int row );
    void print();
    void print(const QString &name);
    void print(const int row);
    void print(const int row, const QString &name);
    void attAttPrint();
    void attAttPrint( const QString &attname );
    virtual QString lastError();
    static QDateTime relStrToDateTime( const QString & str );
    static QDateTime strToDateTime( const QString & str );
    static int strToMinutes( const QString & str );
    static int timeToMinutes( const QString & str );
    static QString qtDateTimeToPgTimestamp( const QDateTime & dt ) ;
    static int weekOfYear( QDate & dt );
    static QString cleanForSQL( const QString & str );
    
    bool printReport( QPrinter *printer,
                      const QStringList * attlist = 0, const QString * rowlist = 0 );
    
      signals:
    void relationChanged( int change, int count );
    void tupleChanged( QmvTuple * tup );
    
      protected:
    QmvDB * database;                   //!< Parent database
    QString class_name;                 //!< This class name
    QDict<QmvClass> child_relations;    //!< Loaded child relations - keyed by class_name
    QDict<QString> class_metadata;      //!< Class metadata - keyed by meta_class attribute name
    QmvMetaDict attribute_metadata;     //!< Attribute metadata - keyed by attribute_name
    QmvMetaDict foreign_metadata;       //!< Foreign-class metadata - keyed by foreign_key
    QmvMetaDict relation_metadata;      //!< Relation-class metadata - keyed by relation_other_class

    QString current_query;              //!< The query associated with the current set
    int query_handle;                   //!< A handle for persistent query result
    QList<QmvTuple> loaded_tuples;      //!< List of loaded tuples in this set.
    int active_tuple_count;             //!< The active tuple count - 0->(active_tuple_count-1)
    QDict<QmvClass> foreign_relations;  //!< Foreign key relations - indexed by fk attribute

    
}; /* CLASS QmvClass */

/*
 * ======================================================================
 * CLASS DEFINITIONS : QmvSqlClass
 * ======================================================================
 */

class QmvSqlClass : public QmvClass
{
    Q_OBJECT
    
        public:
    QmvSqlClass( QmvDB * db, const QString name, const int rellevel = 0 );    // Constructor
    QmvSqlClass( const QmvSqlClass & rel );  // copy constructor
    ~QmvSqlClass();  // Destructor

    virtual const QString queryDescription();
    
    virtual const bool createMetadata(QString name);
    virtual int loadForeignRelations( const QStringList & fkrel, int const limit = 0 );
    virtual void loadChildRelations( const int rellevel = 0 );
    QmvSqlTuple * createTuple( QmvSqlTuple * existing = 0 );
    virtual void deleteTuple( QmvSqlTuple * tup );
    virtual int open(const QString &query);
    virtual int open(const QString & query, QProgressDialog * controller );
    virtual int newTuple( bool autosave = true );
    virtual int copyTuple( int row, bool autosave = true );
    virtual QString lastError();

}; /* CLASS QmvSqlClass */

#endif /* QMVCLASS_H */
// Local Variables:
// mode:C++
// End:
