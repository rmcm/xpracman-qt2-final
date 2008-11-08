/*
 *     $Id: qmvdb.h 2889 2006-06-11 11:47:27Z source $
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

#ifndef QMVDB_H
#define QMVDB_H

/*
 * ======================================================================
 * DESCRIPTION: DataBase class for marvin storage interface classes
 *
 * ======================================================================
 */

/*!
  \file   qmvdb.h
  \brief  Header file for QmvDB class.

  Contains class definition for QmvDB.

  \author Rex McMaster, rmcm@compsoft.com.au
  \date   2000-11-02
*/

/*
 * ======================================================================
 * INCLUDES:
 * ==========================================value============================
 */
#include <qstring.h>
#include <qlist.h>
#include <qdict.h>
#include <qptrdict.h>
#include <qlayout.h>
#include <qvbox.h>
#include <qstatusbar.h>
#include "qmvclass.h"
#include "qmvaccess.h"
#include "qmvrelationwidget.h"
#include "sql.h"
#include "qmvquerywidget.h"
#include "qmvexception.h"


class QString;
class QWidgetStack;
class QmvDB;
class QmvClass;
class QmvSqlClass;
class QmvDBWidget;
class QGroupBox;
class QVBox;
class QmvRelationWidget;

typedef QPtrDict<QmvRelationWidget> relnIndex;
typedef QIntDict<QWidgetStack> ListPanel;
typedef QIntDict<relnIndex> DictPanel;

/*
 * ======================================================================
 * CLASS DEFINITIONS : QmvDB
 * ======================================================================
 */
//! A persistent data object.
/*!
  
  QmvDB is a persistent data object, roughly equivilent to an SQL database. 

*/
class QmvDB : public QObject
{
    Q_OBJECT
  public:
    enum meta_group { MetaClass, MetaAttribute, MetaForeign, MetaRelation };
        // Constructor
    QmvDB(const QString& dbname, const QmvAccess * access,
          bool connect = TRUE, QWidget *parent = 0, const char *name = 0 );
    ~QmvDB();                                // Destructor
    
    const QString dbTitle() const;
    virtual bool open();
    virtual void close();
    const QString dbName() const;
    
    virtual bool loadMetameta();
    virtual int loadRelations( const QString &query_where = 0, const int rellevel = 0 );
    virtual int loadForeignRelations( const QStringList & rellist, int limit = 0 );


    const QList<QString> & attmetaClass() const;
    const QList<QString> & attmetaAttribute() const;
    const QList<QString> & attmetaForeign() const;
    const QList<QString> & attmetaRelation() const;
    const QmvClass * dbRelation( const QString & name ) const;
    QmvClass * dbMetaRelation( const QString & name ) const;
    
    
    QDict<QmvClass> const * dbClasses() const;
    QDict<QmvClass> const * dbForeignRelations() const;

    void addtoForeignRelationIndex( const QString & name, const QmvClass * relation );
    QmvClass * findchildRelation( QStringList & ftree ) const;
    QString findchildTitle( QStringList & ftree ) const;
    const QStringList sortedRelationTitles() const;
    const QString relationNameByTitle( const QString & title ) const;
    const int dbConnectionID() const;
    const QmvAccess * dbAccess() const;
    const QString dbAccessName() const;
    const QStringList otherDbTitles() const;
    const QStringList accessList() const;
    const QString otherDbName( const QString & title ) const;
    const QString otherDbTitle( const QString & name ) const;

      protected:
    QList<QString> att_meta_class;              // the meta-attributes of a class of this DB
    QList<QString> att_meta_attribute;          // the meta-attributes of an attribute
    QList<QString> att_meta_foreign;            // the meta-attributes of a foreign relationship
    QList<QString> att_meta_relation;           // the meta-attributes of a child relationship
    

    QDict<QString> location;                    //!< Location attributes for this database.
    QmvDBWidget * dbwidget;                     //!< A default QmvDBWidget widget associated with this database.
    QDict<QmvClass> db_metaclasses;             //!< A dictionary of the metadata relations in this database
    QDict<QmvClass> db_classes;                 //!< A dictionary of the relations in this database
    QDict<QmvClass> db_foreign_relations;       //!< A dictionary of the foreign relations in this database
    QDict<QString> relation_titles;             //!< A dictionary of relation titles
    int id_conn;                                //!< The database connection id for this object
    const QmvAccess * dbaccess;                 //!< the access object for this database - may be shared.

};
/*
 * ======================================================================
 * CLASS DEFINITIONS : QmvSqlDB
 * ======================================================================
 */


//! A persistent data object.
/*!
  
  QmvSqlDB is a persistent data object, roughly equivilent to an SQL database. 

*/
class QmvSqlDB : public QmvDB
{
    Q_OBJECT
  public:
        // Constructor
    QmvSqlDB( const QString & title, const QmvAccess * access,
             bool connect = TRUE, QWidget *parent = 0, const char *name = 0 );
    ~QmvSqlDB();                                // Destructor
    
    virtual bool open();
    virtual void close();
    virtual bool loadMetameta();
    virtual int  loadRelations( const QString &query_where, const int rellevel = 0 );
    virtual int loadForeignRelations( const QStringList & rellist, int limit = 0 );

/*! \name Database Demographics
  PostgreSQL connection options.
 */
    
//@{
    QString db_host;            //!< The host where this database is located.
    QString db_port;            //!< The TCP port on the database host.
    QString db_options;         //!< Postgres connection options.
    QString db_tty;             //!< Postgres tty option.
    QString db_name;            //!< The name of this database.
//@}
    
};

/*
 * ======================================================================
 * CLASS DEFINITIONS : QmvDBWidget
 * ======================================================================
 */

class QmvDBWidget : public QWidget
{
    Q_OBJECT
      public:
    QmvDBWidget( const QString & dbtitle, const QmvAccess * dbaccess,
                 QWidget * parent = 0, const char * name = 0,
                 bool disp_classlist = true, bool disp_query = true,
                 Orientation orientation = Qt::Horizontal );
    ~QmvDBWidget();

    const QString dbName() const;
    const QString dbTitle() const;
    const QmvDB * dataBase() const;
    QProgressBar * progressBar() const;
    QStatusBar * statusBar() const;
    
    void dbTree();
    void childTree( QmvClass * cls, QListViewItem * cl_item );
    void foreignTree( QmvClass * cls, QListViewItem * cl_item );
    const QString userPreference( const QString & attr, const QString & prefname = QString::null ) const;
    QmvRelationWidget * getRelationWidget( QmvSqlClass * cls );
    QmvRelationWidget * getRelationWidget( int panelid );
    QmvTuple * getSelectedTuple( const QString & relnam );
    const int getPanelID() const;

  public slots:
    void hierarchyVisibilityHandler( QWidget * relnw ) const;
    void panelVisibilityHandler( QWidget * relnw ) const;
    void focusPanel( int panelid );

      protected:
    int setPanel( QmvSqlClass * cls, int panelid = -1 );
    bool display_class_list;
    bool display_class_query;
    QmvDB * database;                   //!< The parent database.
    QListView * classlistview;          //!< A list of classes in this database.
    QWidgetStack * wstk_classeditors;   //!< A stack of class editors.
    QPtrDict<QGroupBox> dict_classeditors; //!< A dictionary of instantiated class editors.

  protected slots:
    
    void setRelationHierarchyByTitle( const QString & title );
    void setRelationHierarchyByName( const QString & reln );
    void selectChild( int id, const QString & relname, const QString & query );
    void registerSelectedTuple( int id, QmvTuple * tup );

      private:

    QWidget * original_parent;                  //<! the original parent, before adding to QWidgetStack
    QWidgetStack * hier_reln;                   //<! stack of relation hierarchies
    QGridLayout *hier_grid;
    QDict<QVBox> dict_reln;              //<! index of relation hierarchy splitters by reln name
    QVBox * hier_split;                     //<! the current splitter which holds list-panes.
    int max_list_panels;                        //<! limit to list-panels per hierarchy
    int current_panelid;                        //<! Panelid of current main panel displayed
    QPtrDict<ListPanel> hier_list_panel;        //<! Index of panel lists by hierarchy splitter
    QPtrDict<DictPanel> hier_dict_panel;        //<! Index of panel indexs by hierarchy splitter

    ListPanel * list_panels;                    //<! Ptr to index of wdgt stacks (list panels) by panel id
    DictPanel * dict_panels;                    //<! Ptr to index of relnIndex by panel id
    
    QIntDict<QmvTuple> selected_tuples;         //<! Index of selected tuples

};

    
/*
 * ======================================================================
 * CLASS DEFINITIONS : QmvSqlDBWidget
 * ======================================================================
 */

class QmvSqlDBWidget : public QmvDBWidget
{
    Q_OBJECT
      public:
    QmvSqlDBWidget( const QString & dbtitle, const QmvAccess * dbaccess,
                    QWidget * parent = 0, const char * name = 0,
                    bool disp_classlist = true, bool disp_query = true,
                    Orientation orientation = Qt::Horizontal);
    ~QmvSqlDBWidget();
    

      private:
    
};    

    
    
#endif /* QMVDB_H */

// Local Variables:
// mode:C++
// End:
