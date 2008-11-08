/*
 *     $Id: qmvwkspwindow.cpp 1490 2002-12-27 23:42:12Z source $
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
 * DESCRIPTION: Methods for Marvin Editor's workspace window.
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include "qmvwkspwindow.h"
#include "qmvapplwindow.h"
#include <qworkspace.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qkeycode.h>
#include <qmovie.h>
#include <qmultilineedit.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qprinter.h>
#include <qapplication.h>
#include <qpushbutton.h>
#include <qaccel.h>
#include <qtextstream.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qwhatsthis.h>
#include <qobjectlist.h>
#include <qvbox.h>
#include <qsplitter.h>

#include "print.xpm"
#include "filenew.xpm"
#include "untested.xpm"


/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvwkspwindow_version[] = "$Id: qmvwkspwindow.cpp 1490 2002-12-27 23:42:12Z source $";

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

// Constructor

//! Constructor
/*!
  Prepare an workspace window, with a default database list application.

*/

QmvWkspWindow::QmvWkspWindow( const QmvAccess * dbaccess )
{
    access = dbaccess;
    dblist = dbaccess->dbTitleList();
    
    qDebug("database count = %d", dblist.count());

        // ---- Pixmaps -------------------------------------------
    QPixmap untested_icon (untested_xpm);
    
        // ---- Tool bars -----------------------------------------

        // create a main toolbar
    QToolBar *tools = new QToolBar( this, "tools:toolbar" );
    tools->setHorizontalStretchable(FALSE);
    
        // creat a central spacer toolbar
    QToolBar *tools_spacer = new QToolBar( this, "tools_spacer:Spacer toolbar");
    tools_spacer->setHorizontalStretchable(TRUE);
    (void) new QLabel("Maintenance", tools_spacer, "Toolbar Label");
    
        // create a toolbar for the scratch window
    QToolBar *tools_scratch = new QToolBar( this, "tools_scratch:Scratch Toolbar" );
    tools_scratch->setHorizontalStretchable(FALSE);
    
    QToolButton * toolb;  // general pointer
   
        // Populate the main toolbar ----------------------------
 

        // Create menus ----------------------------
    QPopupMenu * popup_file;
    QPopupMenu * popup_view;
    QPopupMenu * popup_help;
    QPopupMenu * popup_conf;
    
    popup_file = new QPopupMenu( this, "popup_file:File Menu" );
    popup_view = new QPopupMenu( this, "popup_view:View Menu" );
    windowsMenu = new QPopupMenu( this, "windowsMenu:Window Menu"); // class variable
    popup_help = new QPopupMenu( this, "popup_help:Help Menu" );
    popup_conf = new QPopupMenu( this, "popup_conf:Configuration Menu" );

    
    menuBar()->insertItem( "&File", popup_file );
    menuBar()->insertItem( "&View", popup_view );
    menuBar()->insertItem( "&Windows", windowsMenu );
    menuBar()->insertItem( "&Configuration", popup_conf );
    menuBar()->insertItem( "&Help", popup_help );
    
        // File Menu ----------------------------
    popup_file->insertSeparator();
    popup_file->insertItem( "&Quit", qApp, SLOT(closeAllWindows()), CTRL+Key_Q );

        // View Menu ----------------------------
    popup_view->insertItem( "&Database List", this, SLOT( databaseList()) );
    popup_view->insertItem( "Object &Tree", this, SLOT( ObjectTree()), CTRL+Key_T );

        // Windows Menu ----------------------------
    windowsMenu->setCheckable( TRUE );
    connect( windowsMenu, SIGNAL( aboutToShow() ),
	     this, SLOT( windowsMenuAboutToShow() ) );


        // Help Menu ----------------------------
    popup_help->insertItem( untested_icon, "&About Marvin Editor",
                            this, SLOT(about_marvin()));
    popup_help->insertSeparator();
    popup_help->insertItem( "What's &This",
                            this, SLOT(whatsThis()), SHIFT+Key_F1 );

    
        // Create Workspace ----------------------------
    QVBox* vb = new QVBox( this, "vb:toplevel wksp Vbox" );
    vb->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    ws = new QWorkspace( vb );
    setCentralWidget( vb );

        // Create Database listview --------------------------------------------
    databaseList();
    
        // initialise
    dbcurrent = 0;
    viewdbcurrent = 0;
    default_reln_level = 10;

    statusBar()->message( "Ready", 2000 );
}


// Destructor

QmvWkspWindow::~QmvWkspWindow()
{
}

//! Return the QmvAccess object
/*!


*/
const QmvAccess * QmvWkspWindow::dbAccess() const
{
    return access;
}


//! Create a Database View Widget
/*!
  Determine the QmvSqlDB object from the QListViewItem. Call 
  QmvWkspWindow::dbWidget with recursion limit set to 10.
  
  \param dbitem
  \sa QmvWkspWindow::dbWidget(QListViewItem *dbitem)
*/
void QmvWkspWindow::dbWidget(QListViewItem *dbitem)
{
    if (!dbitem)
        return;

    dbWidget( QString( dbitem->text(0) ), default_reln_level );       // set limit to recursion
}

//! Create a Database View Widget
/*!
  Open the QmvSqlDB object and load metadata. Prepare the class list.

  \param db             the database object
  \param rellevel       limit to recursive load of relation classes.
                        (default = 0)
*/
void QmvWkspWindow::dbWidget( const QString & dbname, const int rellevel )
{
    
    QApplication::setOverrideCursor( Qt::waitCursor );
    
    QPixmap untested_icon (untested_xpm);
    QmvApplWindow * wdbc = newWindow( tr("%1 Database").arg(dbname), untested_icon);
    viewdbcurrent = new QmvSqlDBWidget( dbAccess()->dbTitleByName( dbname ),
                                        dbAccess(),
                                        wdbc );
    wdbc->setCentralWidget(viewdbcurrent);
    wdbc->adjustSize();
    wdbc->show();

    QApplication::restoreOverrideCursor();
}

//! Create a new application window for this workspace.
/*!
  Create the applicationa window, and connect the statusbar.

  \param caption        
  \param pixmap 
*/
QmvApplWindow * QmvWkspWindow::newWindow( const QString & caption, const QPixmap & pixmap)
{
    QmvApplWindow * w = new QmvApplWindow(ws, caption.latin1(), WDestructiveClose );
    connect( w, SIGNAL( message(const QString&, int) ),
             statusBar(), SLOT( message(const QString&, int )) );
    w->setCaption(caption);
    w->setIcon( QPixmap(pixmap) );
    return(w);
}


void QmvWkspWindow::about_marvin()
{
    
    QMessageBox::about( this,
                        "About Marvin the Editor",
                        "<b>Marvin</b> is a tool "
                        "for editing and PostgreSQL databases");
}

//! Prepare window list.
/*!
  

*/
void QmvWkspWindow::windowsMenuAboutToShow()
{
    windowsMenu->clear();
    int cascadeId = windowsMenu->insertItem("&Cascade", ws, SLOT(cascade() ) );
    int tileId = windowsMenu->insertItem("&Tile", ws, SLOT(tile() ) );
    if ( ws->windowList().isEmpty() ) {
	windowsMenu->setItemEnabled( cascadeId, FALSE );
	windowsMenu->setItemEnabled( tileId, FALSE );
    }
    windowsMenu->insertSeparator();
    QWidgetList windows = ws->windowList();
    for ( int i = 0; i < int(windows.count()); ++i ) {
	int id = windowsMenu->insertItem(windows.at(i)->caption(),
					 this, SLOT( windowsMenuActivated( int ) ) );
	windowsMenu->setItemParameter( id, i );
	windowsMenu->setItemChecked( id, ws->activeWindow() == windows.at(i) );
    }
}

void QmvWkspWindow::windowsMenuActivated( int id )
{
    QWidget* w = ws->windowList().at( id );
    if ( w )
	w->setFocus();
}

void QmvWkspWindow::databaseList()
{
    if (db_window.isNull())
    {
        QPixmap untested_icon (untested_xpm);
        db_window = newWindow( tr("Databases"), untested_icon);
        viewdblist = new QListView(db_window);
        db_window->setCentralWidget(viewdblist);
    
        QWhatsThis::add(viewdblist, "<b>Database List</b>"
                        " A list of databases currently available. Click on a row to"
                        " activate access to that database");
        viewdblist->addColumn( "Database name" );
        viewdblist->setRootIsDecorated( TRUE );
        viewdblist->setAllColumnsShowFocus( TRUE );

        for ( QStringList::Iterator it = dblist.begin(); it != dblist.end(); ++it )
            new QListViewItem( viewdblist, *(it) );
    }
    db_window->adjustSize();
    db_window->show();
            // connect database select to 
    connect(viewdblist, SIGNAL(clicked(QListViewItem *)),
            this, SLOT( dbWidget(QListViewItem *) ));
        
 
}

/*
 * Prepare a widget tree (debugging) 
 */
void QmvWkspWindow::ObjectTree( )
{
    dumpObjectTree(this, 0);
    if ( !ot_window.isNull() )
    {
        ot_window->adjustSize();
        ot_window->show();
    }
    
}

void QmvWkspWindow::dumpObjectTree( QObject * parent, QListViewItem * whl_parent )
{

    static int indent = 2;

    if ( ot_window.isNull() )
    {
        QPixmap untested_icon (untested_xpm);
        ot_window = newWindow( tr("Object Tree"), untested_icon);
        ot_listview = new QListView(ot_window);  
        ot_window->setCentralWidget(ot_listview);
        QWhatsThis::add(ot_listview, "<b>Object Tree</b>"
                        " Tree view of the current object hierarchy");
        ot_listview->addColumn("Class");
        ot_listview->addColumn("Widget");
        ot_listview->setRootIsDecorated( TRUE );
        ot_listview->setAllColumnsShowFocus( TRUE ); 
    }
    
    if (whl_parent == 0)
    {
        ot_listview->clear();   
        parent = this->topLevelWidget();
    }
    
    QObjectList * l = parent->queryList(0,0,TRUE,FALSE);
    QObjectListIt it( *l );             // iterate over the buttons
    QObject * obj;
    QListViewItem * whl_object;
    int i = 0;
    while ( (obj=it.current()) != 0 ) { // for each found object...
        ++it;
        if (indent == 2)
            whl_object = new QListViewItem(ot_listview,
                                           QString(obj->className()),
                                           QString(obj->name()));
        else
            whl_object = new QListViewItem(whl_parent,
                                           QString(obj->className()),
                                           QString(obj->name()));
	qDebug( "%*d: %s::%s", indent, ++i, obj->className(), obj->name());
        indent+=2;
        dumpObjectTree(obj, whl_object);
        indent-=2;
    }
    delete l;

}

