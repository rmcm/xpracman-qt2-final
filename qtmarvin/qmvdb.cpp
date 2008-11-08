/*
 *     $Id: qmvdb.cpp 3177 2006-11-07 09:11:52Z source $
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
/*!
  \file   qmvdb.cpp
  \brief  Source for the QmvDB Class

  \author Rex McMaster, rmcm@compsoft.com.au
  \date 2000-11-02 */

/*
 * ======================================================================
 * DESCRIPTION:  Base class for marvin interface classes
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <iostream.h>
#include <qapplication.h>
#include <qlist.h>
#include <qwidgetstack.h>
#include <qlayout.h>
#include <qgroupbox.h> 
#include <qwhatsthis.h>
#include <qvaluelist.h>
#include <qlabel.h>
#include "sql.h"
#include "qmvdb.h"
#include "qmvapplication.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvdb_version[] = "$Id: qmvdb.cpp 3177 2006-11-07 09:11:52Z source $";
/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */

/*
 * ======================================================================
 * MEMBER FUNCTIONS: - QmvDB
 * ======================================================================
 */

// ----------------------------------------------------------------------
//! Constructor
/*!
  
  Load the meta-meta-data from the persisent storage. This data is
  used to specify the metadata of constituent classes.

  \param name   The database name
*/

QmvDB::QmvDB(const QString& dbtitle, const QmvAccess * access,
             bool connect, QWidget *parent, const char *name )
        : QObject( parent, name )
{

    dbaccess = access;
    id_conn = -1;
    
    location.insert( "title",  new QString(dbtitle) );
    location.insert( "name",  new QString( dbaccess->dbNameByTitle( dbtitle ) ) );
    location.insert( "host",   new QString( dbaccess->dbHostByTitle( dbtitle ) ) );
    location.insert( "port",   new QString( dbaccess->dbPortByTitle( dbtitle ) ) );
    location.insert( "options",   new QString( dbaccess->dbOptionsByTitle( dbtitle ) ) );
    location.insert( "tty",   new QString( dbaccess->dbTtyByTitle( dbtitle ) ) );
    location.insert( "login",   new QString( dbaccess->dbLoginByTitle( dbtitle ) ) );
    location.insert( "pwd",   new QString( dbaccess->dbPwdByTitle( dbtitle ) ) );
        
    if ( connect && open() )
        loadMetameta();
    
}


// ----------------------------------------------------------------------
//! Destructor
/*!
  Destructor

*/

QmvDB::~QmvDB()
{
    close();
}

const QString QmvDB::dbTitle() const
{
    QString *title = location.find( "title" );
    if ( title )
        return *title;
    else
        return QString();
}

// ----------------------------------------------------------------------
//! Open a connection to the persisent database.
/*!
  

  \return TRUE/FALSE
*/
bool QmvDB::open()
{
    return(TRUE);
    
}
// ----------------------------------------------------------------------
void QmvDB::close()
{
    ;
}

//! Return the name of the database
/*!
  Return the name of the database used in connection dialog.

  \return       database name.
*/
// ----------------------------------------------------------------------
const QString QmvDB::dbName() const
{
    return ( *location.find("name") );
}

// ----------------------------------------------------------------------
//! Load the meta metadata
/*!
  Load the metadata for the metadata classes.

  \return TRUE/FALSE
*/
bool QmvDB::loadMetameta()
{
    return(TRUE);
}


// ----------------------------------------------------------------------
//! Load the Class Objects
/*!
  Load the QmvClass objects for the classes of this database.
  \param query_where    The class list can be limited with this
                        \em where clause. Note that this string must begin with the word
                        \em where as it is simply appended to the select statement. (default = null)
  \param relevel        limit to recursive loading of relation classes.
                        (defaults to 0)
  
  \return       Number of database classes loaded.
*/
int QmvDB::loadRelations( const QString & query_where, const int rellevel )
{
    return (0);
}


// ----------------------------------------------------------------------
//! Load foreign relation objects
/*!
  Creates and loads foreign relations for this relation.

  \param rellist        A QStringList of Foreign relation names. A zero list causes
                        all local relations to have foreign relations initialised
  \param limit          limit to number of tupples loaded in each foreign relation.
  \return               Number of foreign relations loaded
*/
int QmvDB::loadForeignRelations( const QStringList & rellist, int limit )
{
    return (0);
}

// ----------------------------------------------------------------------
const QList<QString> & QmvDB::attmetaClass() const
{
    return ( att_meta_class );
}
// ----------------------------------------------------------------------
const QList<QString> & QmvDB::attmetaAttribute() const
{
    return ( att_meta_attribute );
}
// ----------------------------------------------------------------------
const QList<QString> & QmvDB::attmetaForeign() const
{
    return ( att_meta_foreign );
}
// ----------------------------------------------------------------------
const QList<QString> & QmvDB::attmetaRelation() const
{
    return ( att_meta_relation );
}
// ----------------------------------------------------------------------
const QmvClass * QmvDB::dbRelation( const QString & name ) const
{
    return db_classes.find( name );
}
// ----------------------------------------------------------------------
QmvClass * QmvDB::dbMetaRelation( const QString & name ) const
{
    return db_metaclasses.find( name );
} 
// ----------------------------------------------------------------------
QDict<QmvClass> const * QmvDB::dbClasses() const
{
    return ( & db_classes );
}
// ----------------------------------------------------------------------
QDict<QmvClass> const * QmvDB::dbForeignRelations() const
{
        return ( & db_foreign_relations );
}


// --------------------------------------------------------------------------------
void QmvDB::addtoForeignRelationIndex( const QString & name, const QmvClass * relation )
{
    db_foreign_relations.insert( name, relation );
}


// --------------------------------------------------------------------------------
//! Locate a child relation
/*!
  based on its family tree, expressed in a QStringList (ordered L->R, child->ancestors)

  \param ftree  The family tree in a QStringList.
  
*/
QmvClass * QmvDB::findchildRelation( QStringList & ftree ) const
{
        // Get the top of tree (oldest ancestor) name
    QStringList::Iterator class_name = ftree.fromLast();
    if ( class_name == ftree.end() )
        return 0; // nothing found in list

        // Oldest ancestor object
    QmvClass * cls = dbClasses()->find( *class_name );

    qDebug("findchildRelation (%d) %s of %s", ftree.count(),
           (*class_name).latin1(), ftree.join(":").latin1());
    
    while ( class_name != ftree.begin()  )
    {
        --class_name;
        cls = cls->classRelations()->find( *class_name );
    qDebug("findchildRelation (%d) %s", ftree.count(), (*class_name).latin1());
        if (!cls)
            return 0;
    }
    return (cls);
}

// --------------------------------------------------------------------------------
//! The title of a child relation
/*!
   in its particular context. This is determined from the parent of the child.

  \param ftree  
  \return QString title
*/
QString QmvDB::findchildTitle( QStringList & ftree ) const
{
    if ( ftree.count() < 1 )
        return 0;
    
        // Get the top of tree (oldest ancestor) name
    QStringList::Iterator class_name = ftree.fromLast();

        // Oldest ancestor object
    QmvClass * cls = dbClasses()->find( *class_name );
    if ( !cls )
        return 0;

    qDebug("findchildTitle (%d) %s of %s", ftree.count(),
           (*class_name).latin1(), ftree.join(":").latin1());

    if ( ftree.count() == 1 )
        return cls->relationTitle();
    
    --class_name;
    while ( class_name != ftree.begin()  )
    {
        cls = cls->classRelations()->find( *class_name );
        qDebug("findchildTitle (%d) %s", ftree.count(), (*class_name).latin1());
        --class_name;
        if (!cls)
            return 0;
    }
        // we should now be at the second last generation
    
    return ( cls->childTitle(*class_name) );
}

// --------------------------------------------------------------------------------
//! Return a sorted QStringList of relation titles
/*!


  \return QStringList of relation titles
  \sa   relationNameByTitle
*/
const QStringList QmvDB::sortedRelationTitles() const
{
    QStringList rel_list;
    
        // Prepare the relation list
    QDictIterator<QmvClass> clli( *dbClasses() );
    while ( clli.current() )
    {
        rel_list += clli.current()->relationTitle();
        ++clli;
    }

    rel_list.sort();
    return rel_list;
}

// --------------------------------------------------------------------------------
//! Find a relation name by Title
/*!

  \param title  
  \return Relation name
  \sa sortedRelationTitles
*/
const QString QmvDB::relationNameByTitle( const QString & title ) const
{
    QString * ptr = relation_titles.find( title );
    if ( ptr )
        return *ptr;
    else
        return QString();
}

// --------------------------------------------------------------------------------
//! the DB connection index
/*!
  Return the DB connection index for this object. This index is used to
  reference this QmvDB object's connection to persistent storage in a
  multi-database environment.

  \return The connection index.
  \sa QmvSqlClass::QmvSqlClass
*/
const int QmvDB::dbConnectionID() const
{
    int i = id_conn;
    return i;
}

// --------------------------------------------------------------------------------
//! The QmvAccess access object
/*!
  Describes the access to relations attributes and other databases.
  \return reference to access object
*/
const QmvAccess * QmvDB::dbAccess() const
{
    return dbaccess;
}

// --------------------------------------------------------------------------------
//! A List of database titles
/*!
  Returns a QStringList of titles of other accessable databases.

  \return Db title list.
*/
const QStringList QmvDB::otherDbTitles() const
{
    return dbAccess()->dbTitleList();
}

// --------------------------------------------------------------------------------
//! The user access classification
/*!
  Returns a QStringList of user access groups for the current user.

  \return Access groups
*/
const QStringList QmvDB::accessList() const
{
    return dbAccess()->accessList();
}

// --------------------------------------------------------------------------------
//! Returns a database name.
/*!
  Returns the database name associated with the supplied \em title .
  This information is extracted from the QmvAccess object.

  \param title  
  \return database name
*/
const QString QmvDB::otherDbName( const QString & title ) const
{
    return dbAccess()->dbNameByTitle( title );
}

// --------------------------------------------------------------------------------
//! Returns a database title.
/*!
  Returns the database title associated with the supplied \em name .
  This information is extracted from the QmvAccess object.

  \param name
  \return database title
*/
const QString QmvDB::otherDbTitle( const QString & name ) const
{
    return dbAccess()->dbTitleByName( name );
}

// --------------------------------------------------------------------------------
//! Returns the name of the Access-Control database.
/*!
  Returns the name of the database associated with the QmvAccess object.

  \param title  
  \return database name
*/
const QString QmvDB::dbAccessName() const
{
    return dbAccess()->dbName();
}

/*
 * ======================================================================
 * MEMBER FUNCTIONS: - QmvSqlDB
 * ======================================================================
 */

// ----------------------------------------------------------------------
//! Constructor
/*!
  
  Load the meta-meta-data from the persisent storage. This data is
  used to specify the metadata of constituent classes.

  \param name   The database name
  \param title  A descriptive title
*/

QmvSqlDB::QmvSqlDB( const QString& title, const QmvAccess * access,
                    bool connect, QWidget *parent, const char *name )
        : QmvDB( title, access, connect, parent, name )
{
    
    if ( connect && open() )
        loadMetameta();
}


// ----------------------------------------------------------------------
//! Destructor
/*!
  Destructor

*/

QmvSqlDB::~QmvSqlDB()
{
    location.setAutoDelete(TRUE);
    location.clear();
    relation_titles.setAutoDelete(TRUE);
    relation_titles.clear();
    close();
}

// ----------------------------------------------------------------------
//! Open a connection to the persisent database.
/*!
  

  \return TRUE/FALSE
*/

bool QmvSqlDB::open()
{
    if (id_conn >= 0)
        return(TRUE);
    
    QDictIterator<QString> it(location);
    while ( it.current() ) 
    {
        qDebug( "%s -> %s", it.currentKey().latin1(), it.current()->latin1() );
        ++it;
    }

    id_conn = sql_dbconnect( location.find("host")->latin1(),
                             location.find("port")->latin1(),
                             location.find("options")->latin1(),
                             location.find("tty")->latin1(),
                             location.find("name")->latin1(),
                             location.find("login")->latin1(),
                             location.find("pwd")->latin1());

    if (id_conn >= 0)
        return(TRUE);
    else
        return(FALSE);
    
}

// ----------------------------------------------------------------------
void QmvSqlDB::close()
{
    if (id_conn >= 0)
        r_sql_dbclose( id_conn );
    
    id_conn = -1;
}


// ----------------------------------------------------------------------
//! Load the meta metadata
/*!
  Load the metadata for the metadata classes.

  \return TRUE/FALSE
*/
bool QmvSqlDB::loadMetameta()
{
    bool ret_value = TRUE;
    int num_rows, i;
    
    
        // delete items when they are removed
    att_meta_class.setAutoDelete( TRUE );
    att_meta_attribute.setAutoDelete( TRUE );
    att_meta_foreign.setAutoDelete( TRUE );
    att_meta_relation.setAutoDelete( TRUE );

        // Clear the lists;
    att_meta_class.clear();
    att_meta_attribute.clear();
    att_meta_foreign.clear();
    att_meta_relation.clear();
    db_classes.clear();


        // Access is allowed to all metadata

    
        // ------------------------------------------------------------
        // Register the meta-class attributes
        // ------------------------------------------------------------
    int sql_result_index;
    num_rows = r_sql_exec( dbConnectionID(), &sql_result_index,
                           QString( "select mtat_name, mtat_access from mtat"
                                    " where mtat_class_name = 'mtcl'"
                                    " and mtat__status = 'A'"
                                    " order by mtat_view_order;") );
    if (num_rows < 0)
    {
        qDebug( "A fatal error occured when attempting to fetch metadata" );
        return(FALSE);
    }
    for (i = 0; i < num_rows; i++)
        att_meta_class.append( new QString(r_sql_value( &sql_result_index, i, "mtat_name") ) );
    r_sql_freeresult( &sql_result_index );

        // ------------------------------------------------------------
        // Register the meta-attribute attributes
        // ------------------------------------------------------------
    num_rows = r_sql_exec( dbConnectionID(), &sql_result_index,
                           QString( "select mtat_name, mtat_access from mtat"
                                    " where mtat_class_name = 'mtat'"
                                    " and mtat__status = 'A';" ) );
    for (i = 0; i < num_rows; i++)
        att_meta_attribute.append(new QString(r_sql_value( &sql_result_index, i, "mtat_name" ) ) );
    r_sql_freeresult( &sql_result_index );
    
        // ------------------------------------------------------------
        // Register the meta-foreign attributes
        // ------------------------------------------------------------
    num_rows = r_sql_exec( dbConnectionID(), &sql_result_index,
                           QString( "select mtat_name, mtat_access from mtat"
                                    " where mtat_class_name = 'mtfn'"
                                    " and mtat__status = 'A';") );
    for (i = 0; i < num_rows; i++)
        att_meta_foreign.append(new QString(r_sql_value( &sql_result_index, i, "mtat_name" ) ) );
    r_sql_freeresult( &sql_result_index );
    
        // ------------------------------------------------------------
        // Register the meta-relation attributes
        // ------------------------------------------------------------
    num_rows = r_sql_exec( dbConnectionID(), &sql_result_index,
                           QString( "select mtat_name, mtat_access from mtat"
                                    " where mtat_class_name = 'mtrl'"
                                    " and mtat__status = 'A';") );
    for (i = 0; i < num_rows; i++)
        att_meta_relation.append(new QString(r_sql_value( &sql_result_index, i, "mtat_name" ) ) );
    r_sql_freeresult( &sql_result_index );


        // ----------------------------------------------------------------------
        // Load a set of QmvSQLClass objects for the meta data, which are not
        // visible on the relation list.
        // ----------------------------------------------------------------------
    
    QmvSqlClass * newcls;
    newcls = new QmvSqlClass( this, "mtcl", 0 );
    db_metaclasses.insert( "mtcl", newcls );
    newcls = new QmvSqlClass( this, "mtat", 0 );
    db_metaclasses.insert( "mtat", newcls );
    newcls = new QmvSqlClass( this, "mtfn", 0 );
    db_metaclasses.insert( "mtfn", newcls );
    newcls = new QmvSqlClass( this, "mtrl", 0 );
    db_metaclasses.insert( "mtrl", newcls );
    
    return(ret_value);
}


// --------------------------------------------------------------------------------
//! Load the Class Objects
/*!
  Load the QmvSqlClass objects for the classes of this database.
  \param query_where    The class list can be limited with this
                        \em where clause. Note that this string must begin with the word
                        \em where as it is simply appended to the select statement. (default = null)
  \param relevel        limit to recursive loading of relation classes.
                        (defaults to 0)
  
  \return       Number of database classes loaded.
*/
int QmvSqlDB::loadRelations( const QString & query_where, const int rellevel )
{

        // ------------------------------------------------------------
        // Limit access to mtcl records based on accessID
        // ------------------------------------------------------------
    
    int accessid = dbAccess()->accessID();
    QString query = QString( "select mtcl_name, mtcl_access from mtcl"
                             " where (%1 & mtcl_access::integer) != 0"
                             " and mtcl__status = 'A'"
                             "%2%3;" )
        .arg( accessid )
        .arg( (query_where.length() > 0) ? " and " : "" )
        .arg(query_where);

    int sql_result_index;
    int n_classes = r_sql_exec(dbConnectionID(), &sql_result_index, query );

        // Load the root level classes in the list into this QmvDB.
    for (int i = 0; i < n_classes; i++)
    {
        
        QString class_name = QString(r_sql_value( &sql_result_index, i, "mtcl_name"));
        if (db_classes.find( class_name ))
            continue;
        
        QmvSqlClass * newcls = new QmvSqlClass( this, class_name, 0 );
        qDebug("Class (Root) # %d = %s : %s", i,
               newcls->relationName().latin1(),
               newcls->relationTitle().latin1());
        
        db_classes.insert( class_name, newcls );
        
        relation_titles.insert( newcls->relationTitle(),
                                new QString( newcls->relationName() ) );
    }
    r_sql_freeresult( &sql_result_index );

    if ( rellevel > 0 )
    {
            // Prepare the foreign relations
        QDictIterator<QmvClass> it( db_classes );
        while ( it.current() )
        {
                // Build the foreign relation objects for each root class
            it.current()->loadForeignRelations( QStringList() );
            ++it;
        }

            // load the child relations
        it.toFirst();
        while ( it.current() )
        {
                // Build the foreign relation objects for each root class
            it.current()->loadChildRelations( rellevel );
            ++it;
        }
    }
    
    
        // Cleanup
    r_sql_freeresult(&sql_result_index);
        
    return(n_classes);
}


// --------------------------------------------------------------------------------
//! Load foreign relation objects
/*!
  Creates and loads foreign relations.

  \param rellist        A QStringList of Foreign relation names. A zero list causes
                        all local relations to have foreign relations initialised
  \param limit          limit to number of tupples auto loaded in each foreign relation.
  \return               Number of foreign relations loaded
*/
int QmvSqlDB::loadForeignRelations( const QStringList & rellist, int limit )
{
    int loaded = 0;
        // Prepare the foreign relations
    QDictIterator<QmvClass> it( db_classes );
    while ( it.current() )
    {
            // Was a list specified, and is this relation in it?
        if ( rellist.count() > 0
             && rellist.grep( QRegExp(QString("^%1$").arg(it.currentKey()) ) ).count() == 0 )
            continue;

            // Build the foreign relation objects for each root class
        loaded += it.current()->loadForeignRelations( QStringList(), limit );
        ++it;
    }

    return loaded;
}


/*
 * ======================================================================
 * MEMBER FUNCTIONS: - QmvDBWidget
 * ======================================================================
 */

//! Constructor
/*!
  Construct a database view widget, with a QListView object and a QWidgetList
  of QmvEditor objects. Load the QListView objects with the current class list.

  \param dbtitle        The database title - TODO ?? remove??
  \parem dbaccess       A QmvAccess object for this database widget
  \param parent         Widget parent
  \param name           name of this object
  \param orientation    Whether to display in table or listview ????
*/
QmvDBWidget::QmvDBWidget( const QString & dbtitle, const QmvAccess * dbaccess,
                          QWidget * parent, const char * name ,
                          bool disp_classlist, bool disp_query,
                          Orientation orientation )
        : QWidget(parent, name)

{
    original_parent = parent;
        // these may be redundant
    display_class_list = disp_classlist;
    display_class_query = disp_query;
    
    QApplication::setOverrideCursor( Qt::waitCursor );
    
        // Create a database object
    database = new QmvSqlDB( dbtitle, dbaccess, TRUE, this, dbtitle );
    if ( !database )
        throw( QString("Failed to instantiate database object"), QmvException::Critical );

    qDebug("XpracMan(): database_name = %s", database->dbName().latin1());

    
    QApplication::restoreOverrideCursor();

        // load the relations 
    database->loadRelations();
    
        // load the foreign relations for the primary database classes
    database->loadForeignRelations( QStringList() );

    
        // ------------------------------------------------------------
        // Layout  ----------------------------
        //    - three widgetstacks - parent(first) and child (second)
        //                           grandchild (third)
        // -------------------------------------------------------------
    
    hier_grid = new QGridLayout( this, 1, 1, 0, -1, "Top QmvDBWidget Layout" );
    hier_reln = new QWidgetStack( this, "hier_reln:Relation Hierarchies" );
    hier_grid->addWidget( hier_reln, 0, 0 );
    
        // Connect to hierarchy visibility handler
    connect( hier_reln, SIGNAL( aboutToShow( QWidget * ) ),
             this, SLOT( hierarchyVisibilityHandler( QWidget * ) ) );

    hier_split = 0;
    list_panels = 0;
    dict_panels = 0;
    max_list_panels = 5;
    adjustSize();
    updateGeometry();
}

//! Destructor
/*!


*/
QmvDBWidget::~QmvDBWidget()
{}

// --------------------------------------------------------------------------------
//! The title of the assoicated database
/*!

  \return       database title
*/

const QString QmvDBWidget::dbTitle() const
{
    return database->dbTitle();
}



// --------------------------------------------------------------------------------
//! The name of the assoicated database
/*!

  \return       database name
*/

const QString QmvDBWidget::dbName() const
{
    return database->dbName();
}



// --------------------------------------------------------------------------------
//! Return QmvDB object
/*!
  Return the database object for this widget.
  
\return         Database object
*/

const QmvDB * QmvDBWidget::dataBase() const
{
    return database;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QProgressBar * QmvDBWidget::progressBar() const
{
    if (original_parent)
        return ((QmvApplication *) original_parent)->progressBar();
    else
        return 0;
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QStatusBar * QmvDBWidget::statusBar() const
{
    if (original_parent)
        return ((QmvApplication *) original_parent)->statusBar();
    else
        return 0;
}

// --------------------------------------------------------------------------------
void QmvDBWidget::setRelationHierarchyByTitle( QString const & title )
{
    QString reln = database->relationNameByTitle( title );
    if ( reln.isNull() || !reln.length() )
        return;
    setRelationHierarchyByName( reln );
}

// --------------------------------------------------------------------------------
void QmvDBWidget::setRelationHierarchyByName( QString const & reln )
{

        // Does this hierarchy exist? - if not then create it
    hier_split = dict_reln.find( reln );
    if ( !hier_split )
    {    
        hier_split = new QVBox( hier_reln, reln + " - hierachy" );
        dict_reln.insert( reln, hier_split );
        hier_list_panel.insert( hier_split, new ListPanel );
        hier_dict_panel.insert( hier_split, new DictPanel );
        list_panels = hier_list_panel[hier_split];
        dict_panels = hier_dict_panel[hier_split];
    
            // then build the top panel
        selectChild( -1, reln, "");
    } else {
        list_panels = hier_list_panel[hier_split];
        dict_panels = hier_dict_panel[hier_split];
    }
    
    hier_reln->raiseWidget( hier_split );
    hier_reln->adjustSize();
    hier_reln->updateGeometry();
    qApp->processEvents();
}


// --------------------------------------------------------------------------------
//! Activate/Create a relation panel
/*!
  Activate a relation panel with the requested relation on the top of the stack.
  If \em panel is -1, then create a new panel. If \em cls is not null, then look
  for a QmvRelationWidget for \em cls on the panel stack, and create one if none
  exists. Return the panel id.

  \param cls                    Pointer to a QmvSqlClass object
  \param panelid                Existing panel ID, or -1 to create a new panel.
  \return                       the panel ID
*/

int QmvDBWidget::setPanel( QmvSqlClass * cls, int panelid )
{

    if (!cls)
        return -1;
    
    int panel = panelid;
    if ( panel < 0 )    
    { // New panel requested
        
        int panel_count = list_panels->count();
        if ( panel_count >= max_list_panels )
        {
            QMessageBox::warning( 0, "setPanel:warning",
                                  tr("Maximum Panel count (%1) has been reached.")
                                  .arg( panel_count ),
                                  "OK", 0 );
            return -1;
        }
        panel = panel_count;

            // Create a new widgetstack for the QmvRelationWidget objects
        QWidgetStack * new_ws = new QWidgetStack( hier_split, QString( "Panel-%1").arg(panel) );
        hier_split->adjustSize();
        hier_split->updateGeometry();
        
            // Connect the widgetstack to a panel visibility handler
        connect( new_ws, SIGNAL( aboutToShow( QWidget * ) ),
                 this, SLOT( panelVisibilityHandler( QWidget * ) ) );
            
            // Register this widgetstack in the panel list
        list_panels->insert( panel, new_ws );
            // Insert a new relation dictionaries into the index of relation dictionaries ( type relnIndex)
        dict_panels->insert( panel, new relnIndex );
    }

    QWidgetStack * this_panel = list_panels->find(panel);
    relnIndex * this_panel_dict = dict_panels->find(panel);
    
    if ( !this_panel || !this_panel_dict )
        return -1;

        // ------------------------------------------------------------
        // determine parent info
        // ------------------------------------------------------------
    QmvSqlClass * par_rel;
    QmvRelationWidget * par_rel_wid;
    QWidgetStack * parent_panel = 0;
    if ( panel > 0 )
    {    
            // there is a parent
        parent_panel = list_panels->find(panel-1);
        if ( !parent_panel )
            return -1;
        par_rel_wid  = ( QmvRelationWidget *) parent_panel->visibleWidget();
        if ( !par_rel_wid )
            return -1;
        par_rel = par_rel_wid->getRelation();
        if ( !par_rel )
            return -1;
            // make the parent widget read-only
        par_rel_wid->setReadOnly( TRUE );
        
    } else {
            // no parent
        par_rel = 0;
        par_rel_wid = 0;
    }
    
        // ------------------------------------------------------------
        // create relation list if not found
        // ------------------------------------------------------------

        // look for existing widget for this relation
    QmvRelationWidget *list  = this_panel_dict->find( cls );
    if ( !list )
    {  // list for this relation not found, so make a new one
            
        list = new QmvRelationWidget( this,
                                      *cls,
                                      par_rel,
                                      par_rel_wid,
                                      QStringList(),
                                      panel,
                                      this_panel,
                                      cls->relationName() );

            // if this is the last panel available, then hide the tab-bar
        if ( panel == ( max_list_panels - 1 ) )
            list->showTabBar(FALSE);
        
            // add to end of widgetstack
        this_panel->addWidget( list, this_panel_dict->count() );
            // add to index
        this_panel_dict->insert( cls, list );

            // connect rowselected in list to child handler
        connect( list, SIGNAL( rowSelected( int, const QString &, const QString & ) ),
                 this, SLOT( selectChild( int, const QString &, const QString & ) ) );

            // connect tupleselected in list to local tuple register
        connect( list, SIGNAL( tupleSelected( int, QmvTuple * ) ),
                 this, SLOT( registerSelectedTuple( int, QmvTuple * ) ) );
        if ( original_parent )
        {
                // Notify parent of new list panel
            ((QmvApplication *) original_parent)->newListPanel( list );
                // Connect rowAdded to the QmvApplication trigger 
            connect( list, SIGNAL( rowAdded( QString &, int ) ),
                     original_parent, SLOT( addRowToSetTrigger( QString &, int ) ) );
                // Connect rowDeleted to the QmvApplication trigger 
            connect( list, SIGNAL( rowAboutToBeDeleted( QString &, int ) ),
                     original_parent, SLOT( deleteRowFromSetTrigger( QString &, int ) ) );
                // Connect rowDeleted to the QmvApplication trigger 
            connect( list, SIGNAL( rowAccessed( QString &, int ) ),
                     original_parent, SLOT( accessRowTrigger( QString &, int ) ) );
        }
            
            
    }
        // Make the current relation widget read-write
    list->setReadOnly( FALSE );
        
        // raise to top of widget stack (unconditionally - this was previously
        // conditional upon not already being on top - but the widgetstack may
        // have been hidden and the raiseWidget() is required to trigger the
        // geometry adjustment)
    this_panel->raiseWidget( list );
    this_panel->show();
    this_panel->adjustSize();
    this_panel->updateGeometry();
    
    hier_reln->adjustSize();
    hier_reln->updateGeometry();
    
        // hide child panels
    focusPanel( panel );
    return panel;
}

//--------------------------------------------------------------------------------
//! A visibility handler
/*!
  Prepare a QmvRelationWidget object hierarchy for visibility.

  \param relnw  
*/
void QmvDBWidget::hierarchyVisibilityHandler( QWidget * hierw ) const
{

        // Extract the int-dict of QWidgetStacks (of QmvRelationWidgets)
    ListPanel *lp = hier_list_panel[ (QVBox *) hierw];     

        // Walk the panel list (each has a widgetstack), and reinstate the current row
        // of the visible QmvRelationWidget.
    for ( int i = 0; i < lp->count(); i++ )
    {
        QmvRelationWidget * rel = (QmvRelationWidget *) list_panels->find(i)->visibleWidget();
        if ( rel )
            rel->selectCurrentRow();  // reinstate current row
    }
}


//--------------------------------------------------------------------------------
//! A visibility handler
/*!
  Prepare a QmvRelationWidget object for visibility within a panel.

  \param relnw  
*/
void QmvDBWidget::panelVisibilityHandler( QWidget * relnw ) const
{
        // reinstate current row
    ( (QmvRelationWidget *) relnw)->selectCurrentRow();
}


// ----------------------------------------------------------------------------------------------------
//! Focus the panel with id \e panel
/*!
Hide child panels, and unregister their selected tuples.

  \param panel  the panel id
*/
// ----------------------------------------------------------------------------------------------------
void QmvDBWidget::focusPanel( int panel )
{
    if ( panel < 0 )
        return;
    
    QVBox * sp = (QVBox *)hier_reln->visibleWidget();
    QWidgetStack * this_panel = list_panels->find(panel);
    
        // hide child panels
    for ( int i = panel + 1; i < list_panels->count(); i++ )
    {
        QWidgetStack * i_panel = list_panels->find(i);
        if ( i_panel )
            i_panel->hide();
            // no selected tuple when hidden.
        if ( selected_tuples[i] )
            selected_tuples.remove(i);
    }

        // set non-fixed height for current panel
    this_panel->setMinimumHeight(0);
    this_panel->setMaximumHeight(32767);

        // set widget to readwrite
    ( ( QmvRelationWidget *) this_panel->visibleWidget())->setReadOnly( FALSE );

        // If there is a parent, set the parent-panel to minimum size.
        // - this is done afer the current panel is resized (r/w)
    
    QWidgetStack * parent_panel = 0;
    if ( panel > 0 )
        parent_panel = list_panels->find(panel-1);
    if ( sp && parent_panel )
    {
        QmvRelationWidget * parent_visible = ( QmvRelationWidget *) parent_panel->visibleWidget();
        int height = parent_visible->minimumSizeHint().height();
        parent_panel->setFixedHeight(80); //parent_visible->minimumSizeHint().height());
    }
    
        // Focus the panel
    if ( panel < list_panels->count() && this_panel )
        this_panel->visibleWidget()->setFocus();
    
}

 

// ----------------------------------------------------------------------------------------------------
//! Select a child panel
/*!
  Select and configure and child panel

  \param id             The ID of the parent panel
  \param relname        the name of the child relation
  \param query          a query, which if non-null will define the child relation.
*/
void QmvDBWidget::selectChild( int id, const QString & relname, const QString & query )
{
        // ----------------------------------------
        // locate the child relation
        // ----------------------------------------
    QmvSqlClass * cls = 0;
    if ( relname.length() )
        cls = (QmvSqlClass *) database->dbClasses()->find(relname);
    if ( !cls )
    {
            // if relname not found, then focs to the panel id. This provides
            // a method for a panel to close its children, by sending a null relname.
        focusPanel(id);
        return;
    }
    
        // ----------------------------------------
        // locate the panel for the child relation
        // ----------------------------------------
    int panel_count = list_panels->count();
    int panel = id + 1;
    
    if ( panel >= panel_count )
            // a new panel is needed
        panel = setPanel( cls, -1 );
    else
            // the panel already exists.
        panel = setPanel( cls, panel );
    
    if ( panel < 0 )
        return;
    
        // ----------------------------------------
        // Locate the list widget for the child relation
        // ----------------------------------------
    QmvRelationWidget * list_reln = dict_panels->find(panel)->find( cls );
    if ( !list_reln )
    {
        QMessageBox::warning( 0, "selectChild:warning",
                              tr("Cannot locate the panel for class named \n\n"
                                 "<b>%1 - %2</b>.")
                              .arg( relname )
                              .arg( cls->relationTitle() ),
                              "OK", 0 );
        return;
    } 

    list_reln->setBaseQuery( query );
    list_reln->adjustSize();
    list_reln->updateGeometry();
    
        // If query exists, then apply it, including any user-applied filters
    if ( !query.isNull() && query.length() > 0 )
        list_reln->displayList( list_reln->getCompositeQuery() );

        // raise to top of stack.
    QWidgetStack * ws_panel = list_panels->find(panel);
    ws_panel->raiseWidget( list_reln );
    ws_panel->show();
    ws_panel->adjustSize();
    ws_panel->updateGeometry();
    list_reln->setFocus();
    
}

// --------------------------------------------------------------------------------
//! register selected tuples
/*!
  Register the \ref QmvTuple \em tup for the panel with index \id , after removing
  existing revistrations for \em id. If \em tup is a null pointer, the effect is to
  leave the panel \em id without a selected tuple.

  \param id     
  \param tup    
*/
// --------------------------------------------------------------------------------
void QmvDBWidget::registerSelectedTuple( int id, QmvTuple * tup )
{
    if ( selected_tuples[id] )
        selected_tuples.remove(id); // no selected tuple when hidden.
    if ( tup )
        selected_tuples.insert( id, tup );
}

// --------------------------------------------------------------------------------
QmvRelationWidget * QmvDBWidget::getRelationWidget( QmvSqlClass * cls )
{
    if ( dict_panels )
        for ( int i = 0; i < max_list_panels && i < dict_panels->count(); i++ )
            if ( dict_panels->find(i)->find( cls ) )
                return dict_panels->find(i)->find( cls );
    return 0;
}

// --------------------------------------------------------------------------------
//! Find the \ref QmvRelationWidget visible on panel \e panelid
/*!

  \param panelid

  \return pointer to \re QmvRelationWidget
*/
// --------------------------------------------------------------------------------
QmvRelationWidget * QmvDBWidget::getRelationWidget( int panelid )
{
    QWidgetStack * ws = list_panels->find(panelid);
    if ( ws )
        return ( QmvRelationWidget *) ws->visibleWidget();
    
    return 0;
}


// --------------------------------------------------------------------------------
QmvTuple * QmvDBWidget::getSelectedTuple( const QString & relnam )
{
        // look for a selected tuple with parent name matching
    for ( int i = 0; i < max_list_panels; i++ )
        if ( !selected_tuples[i] )
            continue;
        else
            if ( selected_tuples[i]->parentClass()->relationName() == relnam )
                return selected_tuples[i];

    return 0;
}

// --------------------------------------------------------------------------------
//! Return the current panel ID
/*!


  \return index of current panelid
*/
// --------------------------------------------------------------------------------
const int QmvDBWidget::getPanelID() const
{
    return current_panelid;
}

// --------------------------------------------------------------------------------
void QmvDBWidget::dbTree()
{
    

    static QDialog * dbtree_window;

    if ( !dbtree_window )
    {
        dbtree_window = new QDialog( this, "DB Tree", FALSE, 0);
        dbtree_window->setCaption( "DB Tree" );
            // A class list
            // -- make this the first widget in the VBox
        QVBoxLayout * vb_clss =  new QVBoxLayout( dbtree_window, 2 );
        classlistview = new QListView( dbtree_window, "classlistview");
        vb_clss->addWidget( classlistview );
        classlistview->addColumn( tr("Name") );
        classlistview->addColumn( tr("Description") );
        classlistview->setRootIsDecorated( TRUE );
        classlistview->setAllColumnsShowFocus( TRUE );
        QWhatsThis::add(classlistview, "<b>Class List:</b>"
                        " A list of classes in the <b>%1</b> database.");
    }
    classlistview->clear();
    
        // Prepare the class tree using base (primary list )
    QDictIterator<QmvClass> clli( *database->dbClasses() );
    while ( clli.current() )
    {
        QListViewItem * lvclass = new QListViewItem( classlistview,
                                                     clli.currentKey(),
                                                     clli.current()->relationTitle() );
        QListViewItem * lvchild = new QListViewItem( lvclass,
                                                     "Child Relations",
                                                     QString( "%1")
                                                     .arg(clli.current()->classRelations()->count()) );
        childTree( clli.current(), lvchild );
        QListViewItem * lvforeign = new QListViewItem( lvclass,
                                                       "Foreign Relations",
                                                       QString( "%1" )
                                                       .arg(clli.current()->listForeignRelations().count()));
        foreignTree( clli.current(), lvforeign);
        ++clli;
    }
    
    dbtree_window->show();
}

    
// --------------------------------------------------------------------------------
void QmvDBWidget::childTree( QmvClass * cls, QListViewItem * cl_item )
{
    QStringList chlr = cls->listChildRelations();
    QStringList::Iterator itchlr = chlr.begin();
    while ( itchlr != chlr.end() )
    {
        QString crl_name = *itchlr;
        QmvClass * crcl;
        if ( crl_name.length() && (crcl = cls-> childRelationByName( crl_name )  ) )
            childTree( crcl, new QListViewItem( cl_item, 
                                                QString( "%1(%2)").arg( crl_name ).arg( (int) crcl ),
                                                crcl->relationTitle() ) );
        else
            (void) new QListViewItem( cl_item, crl_name, "Not loaded" );
        
        ++itchlr;

    }
}

// --------------------------------------------------------------------------------
void QmvDBWidget::foreignTree( QmvClass * cls, QListViewItem * cl_item )
{
    QStringList frlk = cls->listForeignKeys();
    QStringList::Iterator itfrlk = frlk.begin();
    while ( itfrlk != frlk.end() )
    {
        QString frl_key = *itfrlk;
        QString frl_name = cls->foreignRelationName( frl_key );
        QmvClass * frcl;
        if ( frl_name.length() && (frcl = cls->foreignRelation( frl_name )  ) )
            foreignTree( frcl, new QListViewItem( cl_item, frl_key,
                                                  QString( "%1(%2):%3")
                                                  .arg( frl_name )
                                                  .arg( (int) frcl )
                                                  .arg(frcl->relationTitle() ) ) );
        else
            (void) new QListViewItem( cl_item, frl_key, "Not loaded" );
        
        ++itfrlk;
    }
}

// --------------------------------------------------------------------------------
//! Interface to QmvApplication userpreference method.
/*!
  Get current setting for userpreference \em attr in the \em prefname dataset.

  \param attr           attribute name
  \param prefname       preference dataset
  \return               the value of the attribute in the set.
*/
// --------------------------------------------------------------------------------
const QString QmvDBWidget::userPreference( const QString & attr,  const QString & prefname ) const
{
    if ( original_parent )
        return ( (QmvApplication *) original_parent)->userPreference( attr, prefname );
    else
        return QString::null;
}


/*
 * ======================================================================
 * MEMBER FUNCTIONS: - QmvSqlDBWidget
 * ======================================================================
 */


QmvSqlDBWidget::QmvSqlDBWidget( const QString & dbtitle, const QmvAccess * dbaccess,
                                QWidget * parent, const char * name,
                                bool disp_classlist, bool disp_query,
                                Orientation orientation )
        : QmvDBWidget( dbtitle, dbaccess, parent, name, disp_classlist, disp_query, orientation )
{
    return;   
}

QmvSqlDBWidget::~QmvSqlDBWidget()
{}



