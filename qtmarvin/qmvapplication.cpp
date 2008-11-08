/*
 *     $Id: qmvapplication.cpp 3232 2006-12-15 01:18:27Z source $
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
 * INCLUDES:
 * ======================================================================
 */

// Local includes
#include <pwd.h>
#include "qmvapplication.h"
#include "marvin_logo.xpm"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvapplication_version[] = "$Id: qmvapplication.cpp 3232 2006-12-15 01:18:27Z source $";

/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */

/*
 * ======================================================================
 * LOCAL FUNCTION PROTOTYPES:
 * ======================================================================
 */

/*
 * ======================================================================
 * LOCAL FUNCTIONS:
 * ======================================================================
 */

/*
 * ======================================================================
 * GLOBAL FUNCTIONS:
 * ======================================================================
 */

/*
 * ======================================================================
 * Constructor
 *
 * Create all child widgets of the QmvApplication Widget
 * ======================================================================
 */

QmvApplication::QmvApplication( const QString & dbtitle, const QmvAccess * dbaccess,
                                QWidget* parent,  const char* name )
        : QMainWindow( parent, name ),
          dbw_current(0), progress(0)

{
    db_access = dbaccess;
    application_dir = ".";
    application_name = "qmvapp";
    current_user_preference = "_default";
    printer = 0;
    signal_mapper = new QSignalMapper(this);
    connect( signal_mapper, SIGNAL( mapped( const QString &) ),
             this, SLOT( displayQuerySet( const QString &) ) );
    
    if ( !name )
        setName( "QmvApplication" );
    setCaption( tr( "QmvApplication"  ) );

        // initialise
    ot_window = 0;

        // ------------------------------------------------------------
        // make a central widget to contain the other widgets
        // ------------------------------------------------------------
    
    central = new QWidget( this, "central:Central Widget" );
    setCentralWidget( central );
    topgrid = new QGridLayout(central, 1, 1, 0, -1, "topgrid:Top QmvApplication Grid" );
    
        // ---- Icon Sets - central
    
    stdicons = new QmvIcons( this, "qmvapplication_icons" );

        // ------------------------------------------------------------
        // ---- Actions - File options
        // ------------------------------------------------------------

    actiongp_file = new QActionGroup( this, "actiongp_files" );
    setActionGroupColour( actiongp_file, new QColor("black") );
    
        // ------------------------------------------------------------
        // ---- Actions - View options
        // ------------------------------------------------------------

    actiongp_view = new QActionGroup( this, "actiongp_view" );
    setActionGroupColour( actiongp_view, new QColor("blue") );

        // ------------------------------------------------------------
        // ---- Actions - Print options
        // ------------------------------------------------------------

    actiongp_print = new QActionGroup( this, "actiongp_print" );
    setActionGroupColour( actiongp_print, new QColor("darkGreen") );

        // ------------------------------------------------------------
        // ---- Actions - Utilities options
        // ------------------------------------------------------------

    actiongp_utilities = new QActionGroup( this, "actiongp_utilities" );
    setActionGroupColour( actiongp_utilities, new QColor("red") );
    
        // ------------------------------------------------------------
        // ---- Actions - Preferences
        // ------------------------------------------------------------

    actiongp_preferences = new QActionGroup( this, "actiongp_preferences" );
    
        // ------------------------------------------------------------
        // ---- Actions - Help options
        // ------------------------------------------------------------

    actiongp_help = new QActionGroup( this, "actiongp_help" );
    setActionGroupColour( actiongp_help, new QColor("darkCyan") );
    QAction * ac;
    
        // ------------------------
        // About
        // ------------------------

    ac = addToActionGroup( "About",
                           QIconSet(),
                           "&About",
                           "About",
                           "<b>About:</b> About this application.",
                           0,
                           actiongp_help, "doc_help", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( about_qmvapplication() ) );
    

        // ------------------------
        // Help
        // ------------------------

    ac = addToActionGroup( "Help",
                           QIconSet(),
                           "&Help",
                           "Help",
                           "<b>Help:</b> Look for assistance .",
                           0,
                           actiongp_help, "doc_help", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( help_qmvapplication() ) );
    

        // ------------------------------------------------------------
        // ---- Tool bars -----------------------------------------
        // ------------------------------------------------------------

    QPalette t_pal;
    QLabel * t_lab;
    
        // create a file toolbar
    toolbar_file = new QToolBar( this, "toolbar_file" );
    toolbar_file->setHorizontalStretchable(FALSE);
    (void) new QLabel("File ", toolbar_file, "File_toolbar_label");
    
        // create a view toolbar
    toolbar_view = new QToolBar( this, "toolbar_view" );
    toolbar_view->setHorizontalStretchable(FALSE);
    QPixmap * x_pm = new QPixmap(44,22);
    x_pm->fill(toolbar_view, QPoint(0,0) );
    QIconSet view_icon( *stdicons->makeTextPixmap( x_pm,
                                                   tr("View"),
                                                   QFont("Helvetica", 18, QFont::Normal ),
                                                   getActionGroupColour(actiongp_view),
                                                   QRect(QPoint(2,2), QSize(40,20)) ) );
    action_rlsel = new QAction( "Panel Selector",
                                view_icon,
                                "Select a &Data Area",
                                CTRL+ALT+Key_1,
                                actiongp_view, "action_panels", FALSE );
    action_rlsel->setStatusTip("Select a data area from the full list");
    action_rlsel->setWhatsThis("Select a data area from the full list");
    connect( action_rlsel, SIGNAL( activated() ), this, SLOT( toggleViewList() ) );
    

        // create a print toolbar
    toolbar_print = new QToolBar( this, "toolbar_print" );
    toolbar_print->setHorizontalStretchable(FALSE);
    t_lab =  new QLabel("Print ", toolbar_print, "print_toolbar_label");
    t_pal = t_lab->palette();
    t_pal.setColor(QColorGroup::Foreground, getActionGroupColour(actiongp_print) );
    t_lab->setPalette(t_pal);
    t_lab->setFont( QFont("helvetica", 10, QFont::Normal ) );
    
        // create a utilities toolbar
    toolbar_utilities = new QToolBar( this, "toolbar_utilities" );
    toolbar_utilities->setHorizontalStretchable(FALSE);
    t_lab =  new QLabel("Utilities ", toolbar_utilities, "utilities_toolbar_label");
    t_pal = t_lab->palette();
    t_pal.setColor(QColorGroup::Foreground, getActionGroupColour(actiongp_utilities) );
    t_lab->setPalette(t_pal);
    t_lab->setFont( QFont("helvetica", 10, QFont::Normal ) );
    
        // create a spacer toolbar
    toolbar_spacer = new QToolBar( this, "toolbar_spacer");
    toolbar_spacer->setHorizontalStretchable(TRUE);
    
        // ------------------------------------------------------------
        // make a widgetstack to contain the databases, and
        // and relation selector
        // ------------------------------------------------------------
    stack_dbwin = new QWidgetStack( central, "stack_dbwin:database window stack" );
    topgrid->addWidget(stack_dbwin, 0, 0);
    stack_rlsel = new QWidgetStack( toolbar_view, "stack_rlsel:relation selector stack" );
    

        // Populate the main toolbar ----------------------------
        // -- first a database selector
    dbsel =  new QComboBox( TRUE, toolbar_file, "dbsel:database-selector" );
    
    qDebug("QmvApplication: dbsel height = %d", dbsel->height() );
    QWhatsThis::add( dbsel,
                     "<h3>Select a Database:</h3>"
                     "Select the active database to work with.\n\n"
                     " Either type the database name in the input field (it will auto-complete)"
                     " or use the selector button to display the full list of databases");

    dbw_current = addDB( dbtitle );

        // need to now select the database (ie after dbsel )
    if ( !dbw_current )
    {
        QmvException * exception
            = new QmvException( QString( "<h2>Unable to create database widget.</h2>"
                                         "<ul><li>Database title: <b>%1</b>" )
                                .arg( dbtitle ),
                                QmvException::Warning );
        throw( exception );
    }

        // Set these from the dbwidget
    database = dbw_current->dataBase();


    dbsel->setEditable( TRUE );
    dbsel->setInsertionPolicy( QComboBox::NoInsertion );
    dbsel->setAutoCompletion( TRUE );
    dbsel->insertStringList( database->otherDbTitles());
    connect( dbsel, SIGNAL( activated( const QString & ) ),
             SLOT( selectDB( const QString & ) ) );

    actiongp_file->addTo( toolbar_file );
    actiongp_print->addTo( toolbar_print );
    
        // Populate the view toolbar ----------------------------
        // -- first is a relation selector - a widget stack (see above)
    
        // -- second - the canned views
    actiongp_view->addTo( toolbar_view );

        // -- second - the canned utilities
    actiongp_utilities->addTo( toolbar_utilities );

        // ------------------------------------------------------------
        // Create menus ----------------------------
        // ------------------------------------------------------------
    
    popup_file = new QPopupMenu( this, "popup_file:File Menu" );
    popup_view = new QPopupMenu( this, "popup_view:View Menu" );
    popup_print = new QPopupMenu( this, "popup_print:Print Menu" );
    popup_utilities = new QPopupMenu( this, "popup_utilities:Utilities Menu" );
    popup_preferences = new QPopupMenu( this, "popup_preferences:Preferences Menu" );
    popup_help = new QPopupMenu( this, "popup_help:Help Menu" );
    
    menuBar()->insertItem( "&File", popup_file, MenuFile );
    menuBar()->insertItem( "&View", popup_view, MenuView );
    menuBar()->insertItem( "&Print", popup_print, MenuPrint );
    menuBar()->insertItem( "&Utilities", popup_utilities, MenuUtilities );
    menuBar()->insertItem( "P&references", popup_preferences, MenuPreferences );
    menuBar()->insertItem( "&Help", popup_help, MenuHelp );

        // File Menu
    actiongp_file->addTo( popup_file );
    popup_file->insertSeparator();
    popup_file->insertItem( "&Quit", qApp, SLOT(closeAllWindows()), CTRL+Key_Q );
    popup_file->insertItem( "&Restart", this, SLOT(restart()) );

        // View Menu
    actiongp_view->addTo( popup_view );
    
        // Print Menu
    actiongp_print->addTo( popup_print );
    
        // Utilities Menu
    actiongp_utilities->addTo( popup_utilities );
    
        // Preferences Menu
    actiongp_preferences->addTo( popup_preferences );
    popup_preferences->insertSeparator();
    loadPreferences();

        // Help Menu
    actiongp_help->addTo( popup_help );
    popup_help->insertItem( "What's &This",
                            this, SLOT(whatsThis()), Key_F1 );

    loadApplicationGroups();
    
    QPalette pal;
    
        // User Label
    label_user = new QLabel( this, "label_user");
    pal = label_user->palette();
    pal.setColor(QColorGroup::Foreground, QColor("black") );
    label_user->setPalette(pal);
    label_user->setFont( QFont("helvetica", 10, QFont::Normal ) );
    label_user->setAlignment(  AlignLeft | AlignBottom );
    setUserLabel();
    
        //label_user->setText( user_label );

        // Application Label
    label_app = new QLabel( this, "textlabel");
    pal = label_app->palette();
    pal.setColor(QColorGroup::Foreground, QColor("blue") );
    label_app->setPalette(pal);
    label_app->setFont( QFont("courier", 14, QFont::Bold ) );
    label_user->setAlignment(  AlignLeft | AlignBottom );
    label_app->setText( caption() );

        // Logo
    QPixmap logo = QPixmap(marvin_logo_xpm);
    logo.resize( 50,16 );

    id_user_label = menuBar()->insertItem( label_user, MenuUserLabel );
    id_app_label = menuBar()->insertItem( label_app, MenuSystemLabel );
    id_app_logo = menuBar()->insertItem( logo, this, SLOT( about_qmvapplication() ), 0, MenuLogo );


        // ------------------------------------------------------------
        // --- ACCELERATORS  ---------------------------------------
        // ------------------------------------------------------------

    QAccel * qmv_accel = new QAccel( this, "qmvapplication" );
    connect( qmv_accel, SIGNAL( activated( int ) ), this, SLOT(  panelFocus( int ) ) );
        // Top panel focus
    qmv_accel->insertItem( ALT+Key_1, Focus1 );
    qmv_accel->setWhatsThis( Focus1, "Focus to panel 1" );
        // Middle panel focus
    qmv_accel->insertItem( ALT+Key_2, Focus2 );
    qmv_accel->setWhatsThis( Focus2, "Focus to panel 2" );
        // Lower panel focus
    qmv_accel->insertItem( ALT+Key_3, Focus3 );
    qmv_accel->setWhatsThis( Focus3, "Focus to panel 3" );
        // SubLower panel focus
    qmv_accel->insertItem( ALT+Key_4, Focus4 );
    qmv_accel->setWhatsThis( Focus4, "Focus to panel 4" );
    

        // ------------------------------------------------------------
        // --- Add Status etc  ---------------------------------------
        // ------------------------------------------------------------
        // Create a label and a message in the status bar
        // The message is updated when buttons are clicked etc.
    
//     msg = new QLabel( statusBar(), "message" );
//     msg->setAlignment( AlignCenter );
//     QFont boldfont; boldfont.setWeight(QFont::Bold);
//     msg->setFont( boldfont );
//     statusBar()->addWidget( msg, 4, true );
//     QToolTip::add( msg, "Message area" );

        // Add a progress bar
    progbar = new QProgressBar( statusBar(), "progress" );
    progbar->setTotalSteps( 100 );
    progbar->setProgress( progress );
    statusBar()->addWidget( progbar , 0, true );
    QToolTip::add( progbar, "Progress bar" );

    

    statusBar()->message( "Ready", 2000 );

        // Display the current dbwidget
    selectDB( dbw_current->dbTitle() );
}


// --------------------------------------------------------------------------------
QmvApplication::~QmvApplication()
{
    if ( printer )
        delete printer;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvApplication::loadApplicationGroups()
{
        // unload current application groups

    ag_list.setAutoDelete(true);
    ag_list.clear();
    pm_list.setAutoDelete(true);
    pm_list.clear();
    tb_list.setAutoDelete(true);
    tb_list.clear();

    QmvSqlClass * reln_mtag = ( QmvSqlClass *) database->dbRelation( "mtag" );
    if ( !reln_mtag )
        return;
    QmvSqlClass * reln_mtad = ( QmvSqlClass *) database->dbRelation( "mtad" );
    if ( !reln_mtag )
        return;

    int accessid = dbw_current->dataBase()->dbAccess()->accessID();
    
        // Load Action groups
    reln_mtag->open( QString("select * from mtag where mtag_parent is null"
                    " and (%1 & mtag_access::integer) != 0")
                     .arg(accessid) );       // toplevel groups
    for (int g = 0; g < reln_mtag->count(); g++)
    {
        QString group_name = reln_mtag->attributeValue(g, "mtag_name");
        QString group_title = reln_mtag->attributeValue(g, "mtag_title");

        reln_mtad->open( QString("select * from mtad where mtad_parent = '%1'"
                    " and (%2 & mtad_access::integer) != 0")
                         .arg(group_name)
                         .arg(accessid) );
        
        if ( reln_mtad->count() < 1 )
            continue;
        
        for (int m = 0; m < reln_mtad->count(); m++ )
        {
            QString member_name = reln_mtad->attributeValue(m, "mtad_class");
            QString member_title = reln_mtad->attributeValue(m, "mtad_title");
            const QmvClass *cl = dbw_current->dataBase()->dbRelation(member_name);
            if (!cl)
                continue;
            QString member_icon = reln_mtad->attributeValue(m, "mtad_icon" );
            QPixmap pm;
            if ( member_icon.length() < 1 )
                member_icon = member_title.left(1).upper();
            else
                ;         // dummy - TODO: test for name of pixmap etc

            if ( pm.isNull() )
                pm = (*stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                                 member_icon,
                                                 QFont("Helvetica"),
                                                 Qt::red,
                                                 QRect() ) );

            addAction( member_name, member_title, group_name, group_title, QIconSet(pm) );
        }
        
        QActionGroup * ag = ag_list.find(group_name);
        if (!ag)
            continue;
        QToolBar * tb = tb_list.find(group_name);
        if (tb)
            ag->addTo( tb );
        QPopupMenu * pm = pm_list.find(group_name);
        if (pm)
        {
            menuBar()->insertItem( group_title, pm );
            ag->addTo( pm );
        }
    }

}


// --------------------------------------------------------------------------------
void QmvApplication::setAppPath( const QString & path )
{
    application_dir = path;
}

// --------------------------------------------------------------------------------
const QString & QmvApplication::getAppPath() const
{
    return application_dir;
}

// --------------------------------------------------------------------------------
//! Add an actiongroup
/*!
  Adds a new actiongroup with the label \e name and returns a pointer to the
  new actiongroup. If the actiongroup label already exists, then does not create
  a new actiongroup, but returns a pointer to the existing actiongroup

  \param name   
*/
// --------------------------------------------------------------------------------
QActionGroup * QmvApplication::addActionGroup( const QString & group, const QString & title )
{
    QActionGroup * ag = ag_list.find(group);
    if (!ag)
    {
        ag = new QActionGroup( this, group );
        ag_list.insert( group, ag );
        QPopupMenu * pm = new QPopupMenu( this );
        pm_list.insert( group, pm );
        QToolBar * tb = new QToolBar( this );
        QWhatsThis::add(tb, title);
        QToolTip::add(tb, title);
        new QLabel(title, tb, title);
        tb_list.insert( group, tb );
    }
    return ag;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
const QStringList QmvApplication::actionGroupList() const
{
    QStringList agl;
    QDictIterator<QActionGroup> it(ag_list);
    while ( it.current() )
    {
        agl += it.currentKey();
        ++it;
    }
    return( agl );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QActionGroup * QmvApplication::actionGroup( const QString & name ) const
{
        //
    return ag_list.find(name);
}

// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
QAction * QmvApplication::addAction( const QString & ac_name,
                                     const QString & ac_title,
                                     const QString & gp_name,
                                     const QString & gp_title,
                                     const QIconSet & icon)
{
    QActionGroup * ag = addActionGroup( gp_name, gp_title );
    if (!ag)
        return 0;
    if ( icon.isNull() )
        ;
    
    QAction * ac = new QAction( ac_title, icon, ac_title, 0, ag, ac_title, false );
    ac->setStatusTip(ac_title);
    ac->setWhatsThis(ac_title);
    ac_list.insert( ac_name, ac );
    signal_mapper->setMapping( ac, ac_name );
    connect( ac, SIGNAL( activated() ), signal_mapper, SLOT( map() ) );
    return ac;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
const QStringList QmvApplication::actionList() const
{
        //
    QStringList acl;
    QDictIterator<QAction> it(ac_list);
    while ( it.current() )
    {
        acl += it.currentKey();
        ++it;
    }
    return( acl );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QAction * QmvApplication::action( const QString & name ) const
{
        //
    return ac_list.find(name);
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// void QmvApplication::()
// {
//         //
// }

// --------------------------------------------------------------------------------
void QmvApplication::setActionGroupColour( QActionGroup * ag, const QColor * colour )
{
    QColor * x_colour = ag_colour.find( ag );
    if ( !x_colour )
        ag_colour.insert( ag, colour );
    else
        ag_colour.replace( ag, colour );
}

// --------------------------------------------------------------------------------
const QColor QmvApplication::getActionGroupColour( QActionGroup * ag ) const
{
    QColor found("black"); // default
    
    QColor * colour = ag_colour.find( ag );
    if ( colour )
        found = *colour;

    return found;
}

// --------------------------------------------------------------------------------
void QmvApplication::setUserLabel( const QString & label )
{
    if ( label.isNull() || !label.length() )
    {
        struct passwd *passwdInfo;
        passwdInfo = getpwuid ( getuid() );
        if ( !passwdInfo )
            return;

        QString user = passwdInfo->pw_gecos;
        user.replace( QRegExp( ",.*"), "" );
        if ( user.length() == 0 )
            user = passwdInfo->pw_name;
        user.append( QString( " (%1)  [%2]" )
                     .arg(passwdInfo->pw_name)
                     .arg( userPreference( "PreferenceLabel" ) ) );
        user_label =  QString ( "%1" ).arg( user, -60 );
    } else
        user_label = label;
    
    label_user->setText( user_label );
}

// --------------------------------------------------------------------------------
const QString & QmvApplication::getUserLabel() const
{
    return user_label;
}

// --------------------------------------------------------------------------------
void QmvApplication::setAppName( const QString & name )
{
    application_name = name;
}

// --------------------------------------------------------------------------------
const QString & QmvApplication::getAppName() const
{
    return application_name;
}

// --------------------------------------------------------------------------------
void QmvApplication::setAppLabel( const QString & label )
{
    
    if ( !label_app )
        return;
    
    label_app->setText( label );
}

// --------------------------------------------------------------------------------
//! Set the application logo
/*!

  \param logo   a \QPixmap
*/
// --------------------------------------------------------------------------------
void QmvApplication::setAppLogo( const QPixmap & logo )
{
    QMenuItem * ptr = menuBar()->findItem( id_app_logo );
    if ( !ptr )
        id_app_logo = menuBar()->insertItem( logo );
    else
        menuBar()->changeItem( id_app_logo, logo );
}

// --------------------------------------------------------------------------------
//! Return the value of a user preference variable.
/*!

  Find the value of a user preference attribte \em attr in the
  preference group \em prefname.
  
  \param attr           the attribute name
  \param prefname       the preference group name.
  \return               the value or QString::null if not found

*/
// --------------------------------------------------------------------------------
const QString QmvApplication::userPreference( const QString & attr,  const QString & prefname ) const
{
    if ( attr.isNull() || attr.length() < 1 )
        return QString::null;
    QDict<QString> * cup;
    if ( prefname.isNull() || !prefname.length() )
        cup = user_preferences.find( current_user_preference );
    else
        cup = user_preferences.find( prefname );
    
    if ( !cup )
        return QString::null;
    
    QString * val = cup->find( attr );
    if (val)
        return *val;
    else
        return QString::null;
}

// --------------------------------------------------------------------------------
const QString QmvApplication::dbAccessName() const
{
    return db_access->dbName();
}

// --------------------------------------------------------------------------------
const QmvAccess * QmvApplication::dbAccess() const
{
    return db_access;
}

//! Create and configure a QAction object.
/*!
  Create a QAction object and configure tool-tips, iconset, menu entrires and accelerator key.

  \param action_group           A pointer to the parent action group QActionGroup object.
  \param label                  A QString label
  \param icon                   Icon for action
  \param menulabel              A QString menu label
  \param statustip              A QString status tip
  \param whatsthislabel         A QString WhatsThis label
  \param accelkey               An accelerator key.
  \param name                   The object name.
  \param toggle                 Toggle action?
*/
// --------------------------------------------------------------------------------
QAction * QmvApplication::addToActionGroup( const QString & label,
                                            const QIconSet & icon,
                                            const QString & menulabel,
                                            const QString & statustip,
                                            const QString & whatsthislabel,
                                            int accelkey,
                                            QActionGroup * action_group,
                                            char * name,
                                            bool toggle)
{
    
    QAction * newaction = new QAction( label,
                                       icon,
                                       menulabel,
                                       accelkey,
                                       action_group, name, toggle );
    newaction->setStatusTip(statustip);
    newaction->setWhatsThis(whatsthislabel);
    
    return  newaction;
}


// --------------------------------------------------------------------------------
QmvDBWidget * QmvApplication::addDB(  const QString & title )
{

        // no nulls
    if ( title.length() < 1 )
        return 0;

    QmvDBWidget * dbn = dict_dbtitle.find( title );

        // dont confuse with duplicates
    if ( dbn )
        return 0;
    
    int index = dict_dbtitle.count();
    QmvDBWidget * dbw = new QmvDBWidget( title,
                                         dbAccess(),
                                         this, title );
    if ( !dbw )
        return 0;


        // make a relation selector

    QComboBox * relsel =  new QComboBox( TRUE, this, "relation-selector" );
    qDebug("addDB: relsel height = %d", relsel->height() );
    relsel->setInsertionPolicy( QComboBox::NoInsertion );
    relsel->setAutoCompletion( TRUE );
    relsel->insertItem( "", 0 );
    relsel->insertStringList( dbw->dataBase()->sortedRelationTitles(), -1 );
    QWhatsThis::add( relsel,
                     "<h3>Select a Relation:</h3>"
                     "Select the relation (data category) to view."
                     " Either type the relation name in the input field (it will auto-complete)"
                     " or use the selector button to display the full list of relations");

        // connect combo relation selector to dbwidget
    connect( relsel, SIGNAL( activated( const QString & ) ),
             dbw,    SLOT( setRelationHierarchyByTitle( const QString & ) ) );
        // also connect hardwired buttons to dbwidget
    connect( this, SIGNAL( relationActivated( const QString & ) ),
             dbw,    SLOT( setRelationHierarchyByTitle( const QString & ) ) );

        // Put the dbwidget and the rel-selector into widget stacks with
        // the same index.
    
    stack_dbwin->addWidget( dbw, index );
    stack_rlsel->addWidget( relsel, index );
    relsel->setFocus();
    setViewList( TRUE );
    
        // add layout manager
    QGridLayout * dbgrid = new QGridLayout(stack_dbwin, 1, 1, 0, -1, "dbgrid:QmvDBWidget Grid" );
    dbgrid->addWidget( dbw, 0, 0);

        // Reference the dbwidget by db name and by db title.
    
    dict_dbtitle.insert( title, dbw );
    dict_dbname.insert( dbw->dbName(), dbw );

    return dbw;
}

// --------------------------------------------------------------------------------
//! Toggle the visibility of the panel list selector
/*!
  

*/
// --------------------------------------------------------------------------------
void QmvApplication::toggleViewList()
{

    QStringList rellist = database->sortedRelationTitles();
    QmvSelect selrel( rellist, this, "relation selector" );
    selrel.setCaption( tr( "Select a data area" ) );
    if ( rellist_row >= 0 )
        selrel.setCurrentRow( rellist_row );
    
    int row = selrel.exec();
    if ( row < 0 || row >rellist.count() )
        return;
    
    rellist_row = row;

        // look for the dbwidget by the title of the current database
    QmvDBWidget * dbw = dict_dbtitle.find( database->dbTitle() );
    if ( !dbw )
        return;

    emit relationActivated( rellist[rellist_row] );
    

        // --------------------------------------------------
    return;
    
    if ( stack_rlsel->isVisible() )
        setViewList( FALSE );
    else
        setViewList( TRUE );
}

// --------------------------------------------------------------------------------
//! Set the visibility of the panel list selector
/*!
  

*/
// --------------------------------------------------------------------------------
void QmvApplication::setViewList( bool status )
{
    if ( status )
    {
        stack_rlsel->show();
        action_rlsel->setOn(TRUE);
    } else {
        stack_rlsel->hide();
        action_rlsel->setOn(FALSE);
    }
}

// --------------------------------------------------------------------------------
void QmvApplication::selectDB(  const QString & title )
{
    QmvDBWidget * dbw = dict_dbtitle.find( title );
    if ( !dbw )
    {
        if ( !database )
            return;
        
        dbw = addDB( title );
        if ( !dbw )
            return;
    }

    int index = stack_dbwin->id( dbw );
    stack_dbwin->raiseWidget( index );
    stack_rlsel->raiseWidget( index ); // this is in the toolbar, so already layout managed
    
    database = dbw->dataBase();         // change the current database
    setCaption( tr( title  ) );         // change the title
    stack_dbwin->adjustSize();
    stack_dbwin->updateGeometry();
    stack_rlsel->adjustSize();
    
    stack_dbwin->show();                // make an appearance

    for ( int i = 0; i < dbsel->count(); i++ )
        if ( dbsel->text( i ) == title )
        {
            dbsel->setCurrentItem(i);
            break;
        }
    
    dbw_current = dbw;
}

// --------------------------------------------------------------------------------
//! Display a relation widget
/*!
  Search for an existing QmvRelationWidget for \em relnam, make it visible. This is
  a convenience version of the next method.

  \param relname        The name of the relation
  \return               A pointer to the QmvRelationWidget object
*/
QmvRelationWidget *  QmvApplication::displayQuerySet( const QString & relname )
{
    return displayQuerySet( relname, "" );
}

// --------------------------------------------------------------------------------
//! Display a relation widget
/*!
  Search for an existing QmvRelationWidget for \em relnam, make it visible, and
  if \em query is of non-zero length, use it to load an associated set.

  \param relname        The name of the relation
  \param query          A query defining a set
  \return               A pointer to the QmvRelationWidget object
*/
QmvRelationWidget *  QmvApplication::displayQuerySet( const QString & relname, const QString query )
{
        // look for the dbwidget by the title of the current database
    QmvDBWidget * dbw = dict_dbtitle.find( database->dbTitle() );
    if ( !dbw )
        return 0;
    
        // find the relation object
    QmvSqlClass * reln_ptr = ( QmvSqlClass *) database->dbRelation( relname );
    if ( !reln_ptr )
        return 0;

        // find the title of the relation.
    QString reln_title = reln_ptr->relationTitle();
    if ( !reln_title.length() )
        return 0;

        // Activate the widget
    emit relationActivated( reln_title ) ;
    ( (QComboBox *) stack_rlsel->visibleWidget())->setEditText( reln_title );
    

        // find the dislay object
    QmvRelationWidget * rlw = dbw->getRelationWidget( reln_ptr );
    if ( !rlw )
        return 0;
    
    // Load the query set
    if ( query.length() )
        rlw->displayList( query );

    rlw->getQueryWidget()->setFocus();

    return rlw;
}


// --------------------------------------------------------------------------------
//! Display a parent-child relationship
/*!
  Search for an existing QmvRelationWidget for \em parent, make it visible, and
  display the QmvRelationWidget \em child associated with the selection in \em parent

  \param parent         The name of the parent relation.
  \param child          The name of the child relation.

  \return               A pointer to the parent QmvRelationWidget object
*/
QmvRelationWidget *  QmvApplication::displayChildSet( const QString & parent, const QString child )
{

        // find the dislay object
    QmvRelationWidget * parent_rlw = displayQuerySet( parent, "");
    if ( !parent_rlw )
        return 0;
    
    if ( !parent_rlw->selectChildRelationWidget( child  ) )
        return 0;

    return parent_rlw;
}

// --------------------------------------------------------------------------------
void QmvApplication::newListPanel( QmvRelationWidget * nlp )
{
        // for reimplementation
}


// --------------------------------------------------------------------------------
void QmvApplication::addRowToSetTrigger( QString & reln, int sequence )
{
        // for reimplementation
}

// --------------------------------------------------------------------------------
void QmvApplication::deleteRowFromSetTrigger( QString & reln, int sequence )
{
        // for reimplementation
}

// --------------------------------------------------------------------------------
void QmvApplication::accessRowTrigger( QString & reln, int sequence )
{
        // for reimplementation
}

// --------------------------------------------------------------------------------
bool QmvApplication::formSet( QmvSqlClass * reln, const QString form_code )
{
        // Setup a form viewer, and prepare the report.
    static QmvForm * form_viewer;
    if ( form_viewer )
        delete form_viewer;
    
    form_viewer = new QmvForm( reln, this );

    if ( !form_viewer->initForm( form_code ) )
    {
        QMessageBox::information(0, "formSet:information",
                                 tr("<b>%1:</b><br><br>This form <b>%2</b> cannot be initialised.")
                                 .arg( reln->relationTitle() )
                                 .arg( form_code ),
                                 "OK", 0 );
        delete form_viewer;
        return(FALSE);
    }
    form_viewer->setTemplate();
    form_viewer->setData();
    form_viewer->renderForm();
    form_viewer->show();
    form_viewer->raise();
    form_viewer->adjustSize();
    form_viewer->updateGeometry();
    
    return(TRUE);
    }

// --------------------------------------------------------------------------------
bool QmvApplication::formReport( const QString & reln_name, const QString & query,
                                 const QString & form_code )
{
    QmvSqlClass * relation;
    QmvSqlClass * reln_existing = (QmvSqlClass *) database->dbRelation( reln_name );
    if ( reln_existing )
        relation = new QmvSqlClass( *reln_existing );
    else
        return(FALSE);
    
    QProgressDialog controller( "Loading records...", "Cancel", 100,
                                this, "progress", TRUE );

        // Load the data
    int status = relation->open( query, &controller );
    controller.cancel();
    bool return_status = false;
    
    switch ( status )
    {
        case -1:
            QMessageBox::warning( 0, "warning",
                                  tr("<b>%1:</b><br>"
                                     "The data query failed. The following message"
                                     " may help to explain the failure.<br><br>"
                                     "SQL: %2")
                                  .arg( relation->relationTitle() )
                                  .arg( relation->lastError() ),
                                  "OK", 0 );
            return_status = false;
            break;
        case -2:
            QMessageBox::warning( 0, "warning",
                                  tr("<b>%1:</b><br>"
                                     "The data query has been cancelled as requested.")
                                  .arg( relation->relationTitle() ),
                                  "OK", 0 );
            return_status = false;
            break;
        default:
                // No rows;
            if ( relation->count() < 1 )
                return_status = false;
            else
                // Setup a form viewer, and prepare the report.
                return_status = formSet( relation, form_code );
            break;
    }

    delete relation;
    return(return_status);
}

// --------------------------------------------------------------------------------
int QmvApplication::restart()
{
   
    switch( QMessageBox::information( this, "QmvApplication: Restart",
				      "Are you sure that you want to restart? ",
				      "Yes", "Cancel",
				      0, 1 ) ) {
        case 0:
            break;
        case 1:
        default: // just for sanity
            return -1;
            break;
    }
        // close all databases
    
    QDictIterator<QmvDBWidget> it(dict_dbname);
    while ( it.current() )
    {
       ( ( QmvDB *) it.current()->dataBase())->close();
        ++it;
    }
    qApp->exit(ReStart);
    return(0);
    
    extern char **environ;
    return execve( qApp->argv()[0], qApp->argv(), environ );
}

// --------------------------------------------------------------------------------
//! Load user preferences
/*!
  Load from user preference files - system (getAppPath() ) and user ($HOME/.getAppName() )
  in file of pattern userpreferences[_NAME] ).

*/
void QmvApplication::loadPreferences()
{
    user_preferences.clear();
    preferences_idx.setAutoDelete(TRUE);
    preferences_idx.clear();
    int pref_idx = 0;

        // ------------------------------------------------------------
        // Load default preferences 
        // ------------------------------------------------------------
    QString default_prefname( "_default" );
    QDict<QString> * ddef = new QDict<QString>;
    user_preferences.insert( default_prefname, ddef );
    preferences_idx.insert( pref_idx, new QString( default_prefname ) );
    
    ddef->insert( "PreferenceLabel", new QString("Default") );
    ddef->insert( "ViewFormFirst", new QString("no") );
    ddef->insert( "PrinterName", new QString("lp") );
    ddef->insert( "PrinterNameInvoice", new QString("lp") );
    ddef->insert( "PrinterNameStatement", new QString("lp") );
    ddef->insert( "PrinterNameForm", new QString("lp") );
    
        // ------------------------------------------------------------
        // Search path for preference files - from system then user sites.
        // ------------------------------------------------------------
    
    QIntDict<QString> updirlist;
    int dir_idx = 0;
    if ( getAppPath() )
    {
        updirlist.insert( dir_idx, new QString( getAppPath() ) );
        dir_idx++;
    }
         
    QString homepath = QString( "%1/.%2" ).arg( QDir::homeDirPath() ).arg( application_name );
    updirlist.insert( dir_idx, new QString( homepath ) );


        // ------------------------------------------------------------
        // walk through the search path
        // ------------------------------------------------------------
    QDir rcd;
    rcd.setFilter( QDir::Files );
    rcd.setSorting( QDir::Name );
    for ( int ndir = 0; ndir < updirlist.count(); ndir++ )
    {
            // Open directory
            qDebug("QmvApplication::loadPreferences: dir = %s", updirlist[ndir]->latin1() );
        if ( !rcd.cd( *updirlist[ndir] ) )
            continue;

            // Scan file names
        const QFileInfoList *list = rcd.entryInfoList();
        QFileInfoListIterator it( *list );
        while( QFileInfo * fi = it.current() )
        {
            qDebug("QmvApplication::loadPreferences: file = %s", fi->fileName().latin1() );

            QString prefix = "^userpreferences_*";
            QString suffix = "[a-zA-Z0-9]*$";
            
                // not a match
            if ( !fi->fileName().contains( QRegExp( prefix + suffix )  ) )
            {
                ++it;
                continue;
            }

                // extract the prefname from the filename
            QString prefname =  fi->fileName();
            QFile pref_user( QString( "%1/%2" ).arg( *updirlist[ndir]).arg(prefname) );
            prefname.replace( QRegExp( prefix ), "" );
            if ( !prefname.length() )
                prefname = default_prefname;
            
            qDebug("QmvApplication::loadPreferences: file = %s, prefname = %s", pref_user.name().latin1(),prefname.latin1() );

                // open the preference file and read the contents
            if ( pref_user.open( IO_ReadOnly ) )
            {
                    // look for existing prefname dictionary,
                    // else create a new one.
                QDict<QString> * dloc = user_preferences.find( prefname );
                if ( !dloc )
                {
                    dloc = new QDict<QString>();
                    user_preferences.insert( prefname, dloc );
                    pref_idx++;
                    preferences_idx.insert( pref_idx, new QString( prefname ) );
                }
                
                    // read the file
                QTextStream ts( &pref_user );
                while ( !ts.eof() )
                {
                    QString inputstr = ts.readLine();
                        // skip comments
                    if ( inputstr.contains( QRegExp( "^\\s#" ) ) )
                        continue;

                    int eqpos = inputstr.find("=");
                    if ( eqpos < 0 ) continue;
                        // use leftmost "=" as separator (treat other "="'s as text)
                    dloc->insert( inputstr.mid(0, eqpos).stripWhiteSpace(),
                                  new QString( inputstr.mid(eqpos+1).stripWhiteSpace() ) );
                    
//                     QStringList tokens = QStringList::split( QRegExp( "=" ), inputstr );
//                         // add to dict if 2 tokens
//                     if ( tokens.count() == 2 )
//                         dloc->insert( tokens[0].stripWhiteSpace(),
//                                       new QString( tokens[1].stripWhiteSpace() ) );
                }
                pref_user.close();
            } else {
                qDebug("QmvApplication::loadPreferences: failed to open %s/%s", rcd.currentDirPath().latin1(),pref_user.name().latin1() );
            }
            
            ++it;
        }
    }
        // ------------------------------------------------------------
        // setup menu
        // ------------------------------------------------------------
    popup_preferences->clear();
    popup_preferences->setCheckable(TRUE);
    for( int i = 0; i < preferences_idx.count(); i++ )
    {
        QString label = userPreference( "PreferenceLabel", *preferences_idx[i] );
        if ( label.isNull() || !label.length() )
            label = *preferences_idx[i];
        popup_preferences->insertItem( label, i );
        qDebug("QmvApplication::loadPreferences:menu %d:%s/%s",i,preferences_idx[i]->latin1(), current_user_preference.latin1() );
        
        if ( *preferences_idx[i] == current_user_preference )
            popup_preferences->setItemChecked( i, TRUE );
    }
    
    connect( popup_preferences, SIGNAL( activated( int ) ),  SLOT( setCurrentPreferences( int ) ) );
    popup_preferences->insertSeparator();
    int k = popup_preferences->insertItem( "&Reload all preferences", this, SLOT( loadPreferences() ) );
    k = popup_preferences->insertItem( "&Edit current preferences", this, SLOT( editPreferences() ) );
    popup_preferences->setItemEnabled( k ,FALSE);
    k = popup_preferences->insertItem( "&Delete current preferences", this, SLOT( deletePreferences() ) );
    popup_preferences->setItemEnabled( k ,FALSE);
    k = popup_preferences->insertItem( "&Add new preference", this, SLOT( addPreferences() ) );
    popup_preferences->setItemEnabled( k ,FALSE);
    


#ifdef DEBUG
    for ( int i = 0; i < preferences_idx.count(); i++ )
    {
        QDict<QString> * ptr = user_preferences.find( *preferences_idx[i] );
        if ( ptr )
        {
            QDictIterator<QString> it( *ptr );
            while( it.current() )
            {
                qDebug( "QmvApplication::loadPreferences %d-%s : %s = %s", i,preferences_idx[i]->latin1(), it.currentKey().latin1(), it.current()->latin1() );
                ++it;
            }
        }
            
    }
#endif    
}

// --------------------------------------------------------------------------------
void QmvApplication::setCurrentPreferences( int pref )
{
    if ( pref < 0 | pref >= preferences_idx.count() )
        return;
    setCurrentPreferences( *preferences_idx[pref] );
    
}
// --------------------------------------------------------------------------------
void QmvApplication::setCurrentPreferences( const QString & prefname )
{
    if ( prefname.isNull() || !prefname.length() || !user_preferences.find( prefname ) )
        return;
    
    current_user_preference = prefname;
    for( int i = 0; i < preferences_idx.count(); i++ )
        if ( *preferences_idx[i] == current_user_preference )
            popup_preferences->setItemChecked( i, TRUE );
        else
            popup_preferences->setItemChecked( i, FALSE );

    setUserLabel();
}

// --------------------------------------------------------------------------------
bool QmvApplication::savePreferences()
{
        // TODO:
    return true;
    
}


// --------------------------------------------------------------------------------
bool QmvApplication::editPreferences()
{
        // TODO:
    return true;
}


// --------------------------------------------------------------------------------
bool QmvApplication::deletePreferences()
{
        // TODO:
    return true;
}


// --------------------------------------------------------------------------------
bool QmvApplication::addPreferences()
{
        // TODO:
    return true;
}

// --------------------------------------------------------------------------------
QString QmvApplication::sessionFolder()
{
    QString qmvhomedir = QString("%1/.%2").arg( QDir::homeDirPath() ).arg( application_name );
    QDir d(qmvhomedir); 
    if ( !d.exists() )
        {
            if ( !d.mkdir( qmvhomedir ) )
            {
                qWarning("Could not create directory \"%s\"",
                         qmvhomedir.latin1() );
                return( QString::null );
            }
        }
    return qmvhomedir;
}


// --------------------------------------------------------------------------------
bool QmvApplication::saveGeometry( QWidget * w )
{
// This stuff is on design-hold - should it be stored in DB?
    
//         // save geometry
//     
//     QString sessionrc = sessionFileName();
//     if ( sessionrc.isNull() || !sessionrc.length() )
//         return(FALSE);
//     
//     QFile f( sessionrc );
//     
//     if ( !f.open( IO_WriteOnly ) )
//     {
//         QMessageBox::critical( 0, QString( "Fatal Error - File open" ),
//                                QString( QObject::tr("Cannot open <%1> for writing") ).arg(sessionrc) );
//         return(FALSE);
//     }
//     QTextStream t( &f );
// 
//     t << "<!--" << endl << endl;
//     t << "# Marvin sessionrc" << endl;
//     t << "# This file is automatically generated at the end of a session," << endl;
//     t << "# and contains settings to be restored at the beginning of the" << endl;
//     t << "# next session. Any manual changes will be overwritten." << endl << endl;
//     t << "-->" << endl << endl;
//     t << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>" << endl;
//     t << endl;
//     t << "<!DOCTYPE SESSIONRC [" << endl;
//     t << "<!ELEMENT SESSIONRC (ATTRIBUTE+) >" << endl;
//     t << "<!ELEMENT ATTRIBUTE (NAME, SETTINGLIST) >" << endl; 
//     t << "<!ELEMENT SETTINGLIST (SETTING+)>" << endl; 
//     t << "<!ELEMENT SETTING (NAME?, (VALUE|(SETTING+)))>" << endl; 
//     t << "<!ELEMENT NAME (#PCDATA) >" << endl;
//     t << "<!ELEMENT VALUE (#PCDATA) >" << endl;
//     t << "<!ELEMENT ATTRIBUTE (#PCDATA) >" << endl;
//     t << "]>" << endl;
//     t << endl;
// 
//     t << "<SESSIONRC>" << endl;
//     t << "    <ATTRIBUTE>" << endl;
//     t << "        <NAME>mainwindow</NAME>" << endl; 
//     t << "        <SETTINGLIST>" << endl; 
//     t << "          <SETTING><NAME>Xpos</NAME><VALUE>" << w->x() << "</VALUE></SETTING>" << endl;
//     t << "          <SETTING><NAME>Ypos</NAME><VALUE>" << w->y() << "</VALUE></SETTING>" << endl;
//     t << "          <SETTING><NAME>Width</NAME><VALUE>" << w->width() << "</VALUE></SETTING>" << endl;
//     t << "          <SETTING><NAME>Height</NAME><VALUE>" << w->height() << "</VALUE></SETTING>" << endl;
//     t << "        </SETTINGLIST>" << endl; 
//     t << "    </ATTRIBUTE>" << endl;
//     t << "</SESSIONRC>" << endl;
//     
//     
// //      t << "(session-info \"main\"" << endl;
// //      t << "(  position " << w->x() << " " << w->y() << ")" << endl;
// //      t << "(  size " << w->width() << " " << w->height() << " ))" << endl;
// 
//     f.close();
    return true;
}


// --------------------------------------------------------------------------------
bool QmvApplication::restoreGeometry( QWidget * w )
{
// This stuff is on design-hold - should it be stored in DB?
        // restore geometry
    
//     QString sessionrc = sessionFileName();
//     if ( sessionrc.isNull() || !sessionrc.length() )
//         return(FALSE);
//     
//     QFile f( sessionrc );
// //    QXmlInputSource source( f );
// //    QXmlSimpleReader reader;
//     
    return true;
}


// --------------------------------------------------------------------------------
void QmvApplication::closeEvent( QCloseEvent* ce )
{
   
    switch( QMessageBox::information( this, "QmvApplication: Quit",
				      "Are you sure that you want to quit? ",
				      "Yes", "Cancel",
				      0, 0 ) ) {
        case 0:
            saveGeometry( this );
            ce->accept();
            break;
        case 1:
        default: // just for sanity
            ce->ignore();
            break;
    }
} /* closeEvent */

// --------------------------------------------------------------------------------
QProgressBar * QmvApplication::progressBar() const
{
    return progbar;
}



// --------------------------------------------------------------------------------
//! About QmvApplication
/*!
  Information - brief

*/
// --------------------------------------------------------------------------------
void QmvApplication::about_qmvapplication()
{
    QString label = caption();
    if ( label.isNull() || !label.length() )
        label = "QMVAPPLICATION";
    
    QMessageBox::about( this,
                        QString( "About %1" ).arg( label ),
                        QString( "<b>%1</b> is a tool "
                                 "for accessing databases.<br><br>"
                                 "<em>%2</em>")
                        .arg( label )
                        .arg( qmvapplication_version ) );
} /* about_qmvapplication */

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvApplication::help_qmvapplication()
{
    QString label = caption();
    if ( label.isNull() || !label.length() )
        label = "QMVAPPLICATION";
    
    QMessageBox::information( this,
                              QString( "About %1" ).arg( label ),
                              QString( "<b>%1</b> is a tool "
                                       "for accessing databases.<br><br>"
                                       "<em>%2</em>")
                              .arg( label )
                              .arg( qmvapplication_version ) );
} /* help_qmvapplication */


void QmvApplication::displaySchema()
{
    static QDialog * dialog_schema;
    if ( dialog_schema )
    {
        dialog_schema->show();
        return;
    }
    
    dialog_schema = new QDialog( this, "displayschema", FALSE, 0);
    
    dialog_schema->setCaption( database->dbTitle() );
    QVBoxLayout * vl_schema = new QVBoxLayout( dialog_schema, 2 );
    
    QHBoxLayout * hl_schema = new QHBoxLayout( vl_schema, 0, "controls_schema" );
    QLabel * testlabel = new QLabel( database->dbTitle(), dialog_schema );
    hl_schema->addWidget(testlabel);
        // Control: close button
    QPushButton * close_schema = new QPushButton( "Close the schema display window",
                                                  dialog_schema,
                                                  "close_schema");
    close_schema->setPixmap( stdicons->getPixmap( QmvIcons::QuitIcon ) );
    QWhatsThis::add( close_schema, "<b>Close the schema window:</b>.");
    hl_schema->addStretch( 10 );
    hl_schema->addWidget(close_schema);
    connect( close_schema, SIGNAL( clicked() ), dialog_schema, SLOT( hide() ) );

        // load the image
    QListBox * qlb_schema = new QListBox( dialog_schema, "schema_qlabel" );
    vl_schema->addWidget( qlb_schema );
    QImageIO iio;
    QPixmap  schema_xpm;
    iio.setFileName( QString( "%1/marvin_schema.png" ).arg ( getAppPath() ) );
    if ( iio.read() )                 // ok
    {
        schema_xpm = iio.image();           // convert to pixmap
        qlb_schema->insertItem( schema_xpm );
        dialog_schema->show();
    } else {
        delete dialog_schema;
        dialog_schema = 0;
    }
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvApplication::panelFocus( int id )
{
        // look for the dbwidget by the title of the current database
    QmvDBWidget * dbw = dict_dbtitle.find( database->dbTitle() );
    if ( !dbw )
            return;

    int panel;
    
    switch ( id )
    {
        case Focus3:
            panel = 2;
            break;
        case Focus2:
            panel = 1;
            break;
        case Focus1:
        default:
            panel = 0;
            break;
    }
    dbw->focusPanel( panel );
    QmvRelationWidget * rlw = dbw->getRelationWidget( panel );
    if ( rlw )
        rlw->popupChildMenu();
    
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvApplication::viewForeign()
{
    ;
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvApplication::selectRow()
{
    ;
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvApplication::objectTree( )
{
#ifdef QMVAPPLICATION_DEBUG
    objectTree(this, 0);
    if ( ot_window )
    {
        ot_window->adjustSize();
        ot_window->show();
    }
    
#endif
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvApplication::objectTree( QObject * parent, QListViewItem * whl_parent )
{
#ifdef QMVAPPLICATION_DEBUG

    static int indent = 2;

    if ( !ot_window )
    {
        ot_window = new QDialog( this, "Object Tree", FALSE, 0);
        ot_window->setCaption( "Object Tree" );
        QVBoxLayout * vl_objectdump = new QVBoxLayout( ot_window, 2 );
        ot_listview = new QListView(ot_window);
        vl_objectdump->addWidget( ot_listview);
        
        QWhatsThis::add(ot_listview, "<b>Object Tree</b>"
                        " Tree view of the current object hierarchy");
        ot_listview->addColumn("Object");
        ot_listview->addColumn("Label");
        ot_listview->setRootIsDecorated( TRUE );
        ot_listview->setAllColumnsShowFocus( TRUE );
    }
    
    if (whl_parent == 0)
    {
        if (ot_listview)
            ot_listview->clear();   
        parent = this->topLevelWidget();
    }
    
    QObjectList * l = parent->queryList(0,0,TRUE,FALSE);
    QObjectListIt it( *l );             // iterate over the buttons
    QObject * obj;
    QListViewItem * whl_object;
    int i = 0;
    it.toLast();
    while ( (obj=it.current()) != 0 ) { // for each found object...
        QString str_object = obj->className();
        QColor bg_color; // not used yet
        bg_color.setNamedColor("white");
        if ( obj->isWidgetType() )
        {
            QWidget * w = ( QWidget * )obj;
            str_object.append (QString( " (%1:%2  %3:%4  %5:%6 %7:%8)" )
                               .arg( w->width() )
                               .arg( w->height() )
                               .arg( w->minimumWidth() )
                               .arg( w->minimumHeight() )
                               .arg( w->maximumWidth() )
                               .arg( w->maximumHeight() )
                               .arg( w->minimumSizeHint().width() )
                               .arg( w->minimumSizeHint().height() ) );
        } else if ( obj->inherits("QLayout") )
        {
            QLayout * w = ( QLayout * )obj;
            str_object.append (QString( " (%1:%2  %3:%4  %5:%6:%7)" )
                               .arg( w->minimumSize().width() )
                               .arg( w->minimumSize().height() )
                               .arg( w->maximumSize().width() )
                               .arg( w->maximumSize().height() )
                               .arg( w->resizeMode() )
                               .arg( w->expanding() )
                               .arg( w->isTopLevel() ) );
            bg_color.setNamedColor("lightGray");
        }
        
        if (indent == 2)    
            whl_object = new QListViewItem( ot_listview,
                                            str_object,
                                            QString(obj->name()));
        else
            whl_object = new QListViewItem( whl_parent,
                                            str_object,
                                            QString(obj->name()));

        
        indent+=2;
        objectTree(obj, whl_object);
        indent-=2;
        --it;
    }
    delete l;

#endif
}


