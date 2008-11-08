/*
 *     $Id: qmvrelationwidget.cpp 3336 2007-01-28 04:54:30Z source $
 * 
 *   Copyright 2001 X=X Computer Software Trust
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
#include <errno.h>
#include <qapplication.h>
#include <qaccel.h>
#include "qmvrelationwidget.h"
#include "qmvroweditor.h"
#include "qmvpopup.h"
#include "qmvbitgraph.h"

namespace QmvRelationWidgetIcons
{
#include "editcopy.xpm"
#include "editdelete.xpm"
#include "folder_new.xpm"
#include "fileopen.xpm"
#include "left1arrow.xpm"
#include "right1arrow.xpm"
        // from KDE-iceWM theme
#include "title_bg_lt.xpm"
#include "title_bg_rt.xpm"
#include "title_chrome.xpm"
#include "title_bg.xpm"
#include "maximize.xpm"
#include "minimize.xpm"
#include "restore.xpm"
#include "redball.xpm"
#include "reload.xpm"
#include "find.xpm"

} // end QmvRelationWidgetIcons

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvrelationwidget_version[] = "$Id: qmvrelationwidget.cpp 3336 2007-01-28 04:54:30Z source $";
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

QmvRelationWidget::QmvRelationWidget( QmvDBWidget * dbw,
                                      const QmvSqlClass & rel, QmvSqlClass * parent_rel,
                                      QmvRelationWidget * parent_relwid,
                                      const QStringList & headatts, const int index,
                                      QWidget * parent, const char * name )
        : QWidget( parent, name ),
          read_only_state(false),
          always_refresh_set(false)
{
    
    parent_dbw = dbw;
    parent_relation = parent_rel;
    parent_relation_widget = parent_relwid;
    current_row = -1;
    relation = new QmvSqlClass( rel );
    relation->loadChildRelations(0);
    stdicons = new QmvIcons( this, "new_icons" );

        // get refresh options
    if (sql_exec(QString("SELECT conf_value from conf"
                         " where conf_code = 'QMVRELATIONWIDGET_ALWAYS_REFRESH_SET_%1'")
                 .arg(QString(relation->relationName()).upper())) == 1 )
    {
        if (QString(sql_value(0, "conf_value" )).lower() == "yes")
        {
            always_refresh_set = true;
        }
    }

    id_index = index;
    relation_icons = new QmvIcons( this, "relationwidget-icons" );
 
        // --------------------------------------------------
        // A Popup Menu for the child relations
        // --------------------------------------------------

    current_child = -1; // browse
    popup_children = new QPopupMenu( this, "Child Options" );
    popup_children->setCheckable( TRUE );
    label_children = new QLabel( "Display", popup_children );
    label_children->setAlignment( Qt::AlignCenter );
    popup_children->insertItem( label_children, -1, 0 );
    popup_children->insertSeparator();
    connect( popup_children, SIGNAL( aboutToShow() ), this, SLOT( preparePopupChildMenu() ) );
    

        // ------------------------------------------------------------
        // Layout the components
        // ------------------------------------------------------------

    int title_height = 17;

        // Top Level layout for this widget
    QVBoxLayout * layout_top =  new QVBoxLayout( this, 1, -1, "layout_top" );
    QHBox * hbox_main =  new QHBox( this, "hbox_top" );
    layout_top->addWidget( hbox_main );
    
        // Some reusable pointers
    QPalette pal;
    QFrame * x_frame;

        // --------------------------------------------------
        // TopLevel container widget
        // --------------------------------------------------
        // Add a vertical box for title, tools and tableview
        // --------------------------------------------------
    vbox_main =  new QVBox( hbox_main, "top layout" );
    vbox_main->setFrameStyle( QFrame::Box | QFrame::Plain );
    vbox_main->setLineWidth(1);
    vbox_main->setMidLineWidth(0);
    vbox_main->setSpacing(0);
    hbox_main->setStretchFactor( vbox_main, 10 );
    
//    setFocusPolicy( QWidget::NoFocus);
    

        // --------------------------------------------------
        //  Horizontal Box for title and navigation buttons
        // --------------------------------------------------

    QToolTipGroup * tt_title = new QToolTipGroup( this, "Tool tips - titlebar" );
    hbox_title = new QHBox( vbox_main, "Title layout" );
    hbox_title->setFixedHeight( title_height );
    hbox_title->setLineWidth(0);
    hbox_title->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    hbox_title->setSpacing(0);

        // Next Tuple
    nav_left = new QFrame( hbox_title, "previous_tuple" );
    QPixmap nl( QmvRelationWidgetIcons::minimizeA_xpm );
    nav_left->setFixedSize( nl.size() );
    nav_left->setBackgroundPixmap( nl );
    nav_left->installEventFilter( this );
    QToolTip::add( nav_left, "Next row", tt_title, "Select the next row in the display set" );

        // Previous Tuple
    nav_right = new QFrame( hbox_title, "next_tuple" );
    QPixmap nr( QmvRelationWidgetIcons::maximizeA_xpm );
    nav_right->setFixedSize( nr.size() );
    nav_right->setBackgroundPixmap( nr );
    nav_right->installEventFilter( this );
    QToolTip::add( nav_right, "Previous Row", tt_title, "Select the previous row in the display set" );

        // Browse Mode
    nav_restore = new QFrame( hbox_title, "browse tuples" );
    QPixmap nb( QmvRelationWidgetIcons::restoreA_xpm );
    nav_restore->setFixedSize( nb.size() );
    nav_restore->setBackgroundPixmap( nb );
    nav_restore->installEventFilter( this );
    QToolTip::add( nav_restore, "Restore Browse", tt_title, "Restore Browse mode for this set" );

        // Title padding (left)
    x_frame = new QFrame( hbox_title, "Left title spacer" );
    x_frame->setFixedSize( QPixmap(QmvRelationWidgetIcons::title_bg_lt_xpm).size());
    x_frame->setBackgroundPixmap( QmvRelationWidgetIcons::title_bg_lt_xpm );
    hbox_title->setStretchFactor( x_frame, 10 );

        // Title
    label_title = new QFrame( hbox_title, "title" );
    label_title->setBackgroundPixmap(QmvRelationWidgetIcons::title_bg_xpm);
    QFont title_font( "helvetica", 10, QFont::Bold );
    label_title->setFont( title_font);
    label_title->installEventFilter(this);
    QToolTip::add( label_title, "Restore Browse", tt_title, "Click this title to restore Browse mode for this set" );

        // Make the title stretchable.
    hbox_title->setStretchFactor( label_title, 10 );

        // title padding (right)
    x_frame = new QFrame( hbox_title, "Right title spacer" );
    x_frame->setFixedSize( QPixmap(QmvRelationWidgetIcons::title_bg_rt_xpm).size());
    x_frame->setBackgroundPixmap( QmvRelationWidgetIcons::title_bg_rt_xpm );
    hbox_title->setStretchFactor( x_frame, 10 );

        // Chrome fill.

    x_frame = new QFrame( hbox_title, "Right Chrome" );
    x_frame->setBackgroundPixmap( QmvRelationWidgetIcons::title_chrome_xpm );
    hbox_title->setStretchFactor( x_frame, 10 );
    


        // --------------------------------------------------
        //  Horizontal Box for child tabbar
        // --------------------------------------------------

    hbox_tabbar = new QHBox( vbox_main, "TabBar layout" );

        // Add spacer
    x_frame = new QFrame( hbox_tabbar, "Right Chrome" );
    hbox_tabbar->setStretchFactor( x_frame, 10 );

    tab_children = new QmvTabBar( hbox_tabbar, "child relations" );
    QToolTip::add( tab_children, "Child MenuBar" );
    
        // grab the chrome pixmap just for its size
    QPixmap tab_bg(QmvRelationWidgetIcons::title_chrome_xpm);
    tab_bg.fill( backgroundColor().dark(130) );
    tab_children->setFixedHeight( tab_bg.height() );
    tab_children->setBackgroundPixmap( tab_bg );

        // get a list of relation titles for the child menu, and the tabset
    QStringList crl = relation->listChildRelations();

    QPixmap pm( QSize(8, 8));
    int tb_id;
    pm.fill( Qt::red );
    for ( QStringList::Iterator it = crl.begin();
          it != crl.end(); ++it )
    {
        
        QString blab = relation->childTitle( *it );
        
        tb_id = tab_children->addTab( blab );
        tab_children->setToolTip( tb_id, "Display " + blab );
        dict_children.insert( tb_id, new QString( *it ) );

            // Insert popup menu item
        pm.fill( Qt::blue );
        popup_children->insertItem( pm, "&" + relation->childTitle( *it ), tb_id, -1 );

    }
    if ( crl.count() == 0 )
        tab_children->hide();
    
    
        // Connect the button group to a handler
    connect( tab_children, SIGNAL( selected ( int ) ),
             this, SLOT( handleChildTabSelection( int ) ) );
    
        // --------------------------------------------------
        //  Horizontal Box label, QBE and tuple toolbuttons
        // --------------------------------------------------
    hbox_toolbar = new QHBox( vbox_main, "ToolBar layout" );
    hbox_toolbar->setFixedHeight( 44 );

        // --------------------------------------------------
        //  Label for read-only header
        // --------------------------------------------------
    tuple_display = new QTextView( "", "", hbox_toolbar, "tuple display" );
    hbox_toolbar->setStretchFactor( tuple_display, 20);
    tuple_display->hide();
    QToolTip::add( tuple_display, QString("%1 (Current Parent Row)").arg(relation->relationTitle()) );
    
        // --------------------------------------------------
        // A query for this relation
        // --------------------------------------------------
    
    qbe_relation = new QmvQueryWidget( relation,  hbox_toolbar, "qbe_relation" );
    
        // --------------------------------------------------
        // A widget for the tuple functions
        // --------------------------------------------------
    hb_tup_functions = new QHBox( hbox_toolbar, "tuple buttons" );
    hb_tup_functions->setSpacing(0);
    hb_tup_functions->setLineWidth(0);
    
    QToolButton * tf_tb;

        // Add new tuple
    tf_tb = new QToolButton( hb_tup_functions, "tuple_add" );
    tf_tb->setAutoRaise(TRUE);
    tf_tb->setIconSet( stdicons->getIconSet( QmvIcons::BlankPageIcon ) );
    tf_tb->setTextLabel("Add Row (Shift+Insert)");
    tf_tb->setAccel( SHIFT+Key_Insert );
    QWhatsThis::add(tf_tb , QString( "This button will add a new row to the <b>%1</b> panel."
                                     " (Shift+Insert)" )
                    .arg( relation->relationTitle() ) );
    connect( tf_tb, SIGNAL( clicked() ), this, SLOT( addToSet()) );
    
    
        // Mini-editor
    pb_miniedit = new QToolButton( hb_tup_functions, "mini_editor" );
    pb_miniedit->setAutoRaise(TRUE);
    pb_miniedit->setIconSet( stdicons->getIconSet( QmvIcons::TextPageIcon ) );
    pb_miniedit->setTextLabel("Mini Editor (Shift+F2)");
    pb_miniedit->setAccel( SHIFT+Key_F2 );
    QWhatsThis::add(pb_miniedit , QString( "This button will activate display of the mini editor"
                                           " with the current row of the <b>%1</b> panel."
                                           " (Shift+F2)" )
                    .arg( relation->relationTitle() ) );
    connect( pb_miniedit, SIGNAL( clicked() ), this, SLOT( showMiniEditor()) );
 
        // Delete current tuple
    tf_tb = new QToolButton( hb_tup_functions, "tuple_delete" );
    tf_tb->setAutoRaise(TRUE);
    tf_tb->setIconSet( QIconSet(QmvRelationWidgetIcons::editdelete_xpm) );
    tf_tb->setTextLabel("Delete Row (Shift+Delete)");
    tf_tb->setAccel( SHIFT+Key_Delete );
    QWhatsThis::add(tf_tb , QString( "This button will delete the current row"
                                     " from the <b>%1</b> panel."
                                     " (Shift+Delete)" )
                    .arg( relation->relationTitle() ) );
    connect( tf_tb, SIGNAL( clicked() ), this, SLOT( deleteFromSet()) );


        // Print current set
    tf_tb = new QToolButton( hb_tup_functions, "tuple_print" );
    tf_tb->setAutoRaise(TRUE);
    tf_tb->setIconSet( stdicons->getIconSet( QmvIcons::PrintIcon ) );
    tf_tb->setTextLabel("Print Set (Ctrl+Shift+Alt+P)");
    tf_tb->setAccel( CTRL+SHIFT+ALT+Key_P );
    QWhatsThis::add(tf_tb , QString( "<h3>Print the current set:</h3>"
                                     "This button will print the current set of rows"
                                     " from the <b>%1</b> panel."
                                     " (Ctrl+Shift+Alt+P)" )
                    .arg( relation->relationTitle() ) );
    connect( tf_tb, SIGNAL( clicked() ), this, SLOT( formSet()) );

        // Export current set
    tf_tb = new QToolButton( hb_tup_functions, "tuple_export" );
    tf_tb->setAutoRaise(TRUE);
    tf_tb->setIconSet( stdicons->getIconSet( QmvIcons::ExportIcon ) );
    tf_tb->setTextLabel("Export Set");
    QWhatsThis::add(tf_tb , QString( "This button will export the current set of rows"
                                     " from the <b>%1</b> relation to an external destination." )
                    .arg( relation->relationTitle() ) );
    connect( tf_tb, SIGNAL( clicked() ), this, SLOT( exportSet()) );

        // Import to current set
    tf_tb = new QToolButton( hb_tup_functions, "tuple_import" );
    tf_tb->setAutoRaise(TRUE);
    tf_tb->setIconSet( stdicons->getIconSet( QmvIcons::ImportIcon ) );
    tf_tb->setTextLabel("Import Set");
    QWhatsThis::add(tf_tb , QString( "This button will import a set of rows"
                                     " from an external source into the <b>%1</b> panel." )
                    .arg( relation->relationTitle() ) );
    connect( tf_tb, SIGNAL( clicked() ), this, SLOT( importSet()) );

        // Copy tuple
    tf_tb = new QToolButton( hb_tup_functions, "tuple_copy" );
    tf_tb->setAutoRaise(TRUE);
    tf_tb->setIconSet( QIconSet( QmvRelationWidgetIcons::editcopy_xpm ) );
    tf_tb->setTextLabel("Copy Row (Ctrl+Shift+Alt+C)");
    tf_tb->setAccel( CTRL+SHIFT+ALT+Key_C );
    QWhatsThis::add(tf_tb , QString( "This button will copy an existing row to the <b>%1</b> panel."
                                     " (Ctrl+Shift+Alt+C)" )
                    .arg( relation->relationTitle() ) );
    connect( tf_tb, SIGNAL( clicked() ), this, SLOT( copyToSet()) );

        // Refresh table
    tf_tb = new QToolButton( hb_tup_functions, "table_refresh" );
    tf_tb->setAutoRaise(TRUE);
    tf_tb->setIconSet( QIconSet( QmvRelationWidgetIcons::reload_xpm ) );
    tf_tb->setTextLabel("Refresh (F5)");
    tf_tb->setAccel( Key_F5 );
    QWhatsThis::add(tf_tb , QString( "This button will refresh the <b>%1</b> panel."
                                     " (F5)" )
                    .arg( relation->relationTitle() ) );
    connect( tf_tb, SIGNAL( clicked() ), this, SLOT( refreshAll()) );

        // Toggle query
    tf_tb = new QToolButton( hb_tup_functions, "toggle_query" );
    tf_tb->setAutoRaise(TRUE);
    tf_tb->setIconSet( QIconSet( QmvRelationWidgetIcons::redball_xpm ) );
    tf_tb->setTextLabel("Hide/Show the Query Toolbar (F9)");
    tf_tb->setAccel( Key_F9 );
    QWhatsThis::add(tf_tb , QString( "This button will toggle the query toolbar."
                                     " (F9)" )
                    .arg( relation->relationTitle() ) );
    connect( tf_tb, SIGNAL( clicked() ), this, SLOT( toggleQueryWidgetVisible()) );

        // Child relations
    toolbt_children = new QToolButton( hb_tup_functions, "child relations" );
    toolbt_children->setAutoRaise(TRUE);
    toolbt_children->setIconSet( QIconSet(QmvRelationWidgetIcons::fileopen_xpm ) );
    toolbt_children->setTextLabel( QString("More ...  (Alt+%1)").arg(id_index+1));
    toolbt_children->setAccel( CTRL+SHIFT+ALT+Key_C );
    QWhatsThis::add(toolbt_children , QString( "This button will display a menu of options"
                                               " for other folders associated with the <b>%1</b> panel"
                                               " (Alt+%2)" )
                    .arg( relation->relationTitle() )
                    .arg(id_index+1) );
    toolbt_children->setPopup( popup_children );
    toolbt_children->setPopupDelay(500);
    connect( toolbt_children, SIGNAL( clicked() ), this, SLOT( toggleChildSelection()) );
    toolbt_children->hide();
    
        // Spacer.
    QLabel * toolbar_spacer = new QLabel( "", hb_tup_functions, "toolbar spacer" );
    hb_tup_functions->setStretchFactor( toolbar_spacer, 10 );

        // --------------------------------------------------
        //  a tableview for relation -
        // --------------------------------------------------
    table_relation = new QmvTable( vbox_main, rel.relationTitle() );

        // set table characteristics
    table_relation->setSorting( -1 );
    table_relation->horizontalHeader()->setClickEnabled( FALSE,  -1 );
    table_relation->verticalHeader()->setClickEnabled( TRUE,  -1 );

    vbox_main->setStretchFactor( hbox_toolbar, 5 );
    vbox_main->setStretchFactor( table_relation, 10 );
    vbox_main->adjustSize();
    
        // ----------------------------------------------------------------------------
        // Connect Query widget to relation display
        // - no need for slot to use argument, as the relation object is common.
        // ----------------------------------------------------------------------------
    connect( qbe_relation, SIGNAL( relationChanged() ),
             this, SLOT( relationQuery() ) );
    
        // Help information
    
    QWhatsThis::add( table_relation, QString( "<h3>%1:</h3>"
                                              " A matrix of rows selected into the <b>%2</b> panel.")
                     .arg( relation->relationTitle() ) 
                     .arg( relation->relationTitle() ) );
    
        // ----------------------------------------------------------------------------
        // Construct an attribute list to use for headers.
        // ----------------------------------------------------------------------------
    setHeaderAttributeList( headatts );
    
    sync_editors = true;

    connect( table_relation, SIGNAL( currentChanged( int, int ) ),
             this, SLOT( currentCellChanged( int, int ) ) );
    connect( table_relation, SIGNAL( clicked( int, int, int, const QPoint & ) ),
             this, SLOT( cellClicked( int, int, int, const QPoint &  ) ) );
    connect( table_relation->verticalHeader(), SIGNAL( clicked( int ) ),
             this, SLOT( showMiniEditor( int  ) ) );
    connect( table_relation, SIGNAL( reloadRequest( int, int ) ),
             this, SLOT( refreshRow( int ) ) );
    connect( table_relation, SIGNAL( reloadRequest( int, int ) ),
             this, SLOT( refreshPropagate() ) );

        // If top level relation, and there is no load limit
        // then preload the default set.
    if ( id_index == 0 && relation->queryLimit().toInt() == 0 )
    {
        QmvSqlQuery * qobj = qbe_relation->getQueryObject();
        if ( qobj ) {
            QProgressDialog controller( QString( "Loading records..." ),
                                        "Cancel", 100,
                                        this, "progress", TRUE );
            qobj->setProgressDialog( &controller );
            qobj->updateRelationSet();
            qobj->setProgressDialog(0);

        }
    }
    
    if ( id_index == 0)
        setQueryWidgetVisible( true, false );
    else
        setQueryWidgetVisible( false, false );
    setTitle();
    adjustSize();
    updateGeometry();

    setFocusProxy(qbe_relation);
    setReadOnly( FALSE );
    slotSetNavigationButtons();
}

// Destructor

// --------------------------------------------------------------------------------
QmvRelationWidget::~QmvRelationWidget()
{
    delete relation;
    dict_children.setAutoDelete( TRUE );
    dict_children.clear();
    list_editors.setAutoDelete( TRUE );
    list_editors.clear();
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::hideEditPanel()
{
    hide();
    panel_label->show();
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::showEditPanel()
{
    panel_label->hide();
    show();
}

// --------------------------------------------------------------------------------

QmvSqlClass * QmvRelationWidget::getRelation() const
{
    return relation;
}

// --------------------------------------------------------------------------------
QmvSqlClass * QmvRelationWidget::getParentRelation() const
{
    return parent_relation;
}


// --------------------------------------------------------------------------------
QmvRelationWidget * QmvRelationWidget::getParentRelationWidget() const
{
    return parent_relation_widget;
}


// --------------------------------------------------------------------------------
QmvQueryWidget *  QmvRelationWidget::getQueryWidget() const
{
    return qbe_relation;
}

// --------------------------------------------------------------------------------
bool QmvRelationWidget::selectChildRelationWidget( const QString & childname )
{
    int i;
        // button 0 is "none"
    for ( int i = 1; i < dict_children.count(); i++ )
        if ( *dict_children[i] == childname )
        {
            selectChild( i );
            return true;
        }
    
    return false;
}

// --------------------------------------------------------------------------------
int QmvRelationWidget::getCurrentRow() const
{
    return current_row;
}

// --------------------------------------------------------------------------------
//! Move to and select next row
/*!

  Also disable/enable navigation buttons as required

*/
// --------------------------------------------------------------------------------
void QmvRelationWidget::slotNextRow()
{
    if ( current_row >= relation->count() - 1 )
        return;
    
    current_row++;
    rowClicked( current_row );
    if ( read_only_state )
        setTupleDisplay();

    slotSetNavigationButtons();
}

// --------------------------------------------------------------------------------
//! Move to and select previous row
/*!

  Also disable/enable navigation buttons as required

*/
// --------------------------------------------------------------------------------
void QmvRelationWidget::slotPreviousRow()
{
    if ( current_row <= 0 )
        return;
    
    current_row--;
    rowClicked( current_row );
    if ( read_only_state )
        setTupleDisplay();

    slotSetNavigationButtons();
}

// --------------------------------------------------------------------------------
//! Set navigation button access
/*!

   Disable non-relevant navigation buttons

*/
// --------------------------------------------------------------------------------
void QmvRelationWidget::slotSetNavigationButtons()
{
    if ( current_row <= 0 )
    {
        nav_right->setEnabled(FALSE);
        nav_right->setBackgroundPixmap( QmvRelationWidgetIcons::maximizeD_xpm );
    } else {
        nav_right->setEnabled(TRUE);
        nav_right->setBackgroundPixmap( QmvRelationWidgetIcons::maximizeA_xpm );
    }
    

    if ( current_row >= relation->count() - 1 )
    {
        nav_left->setEnabled(FALSE);
        nav_left->setBackgroundPixmap( QmvRelationWidgetIcons::minimizeD_xpm );
    } else {
        nav_left->setEnabled(TRUE);
        nav_left->setBackgroundPixmap( QmvRelationWidgetIcons::minimizeA_xpm );
    }
}

// --------------------------------------------------------------------------------
//! Return a list of key values for parent->child relations
/*!
   in the currently selected parent set.

  \param child_name     the name of the child relation
  \param sep            the separator to use between members of compound keys
  \return               a QStringList of key values lists.
*/
// --------------------------------------------------------------------------------
QStringList QmvRelationWidget::getCurrentSelection( QString & child_name, const QString & sep ) const
{
    QStringList list;   // the destination list - outer=keylist,inner=valuelist
    QStringList keys = relation->childKey( child_name);
    for ( QStringList::Iterator it = keys.begin(); it != keys.end(); ++it )
    {
        QStringList values;
        values.clear();
            // interate over the selections and get the values of the current key
        for ( int isel = 0; isel < table_relation->numSelections(); isel++ )        // selections
            for ( int irow = table_relation->selection( isel ).topRow();            // rows in selection
                  irow <= table_relation->selection( isel ).bottomRow(); irow++ )
            {
                    // qDebug("getCurrentSelection:sel=%d,row=%d,value=%s/%s",
                    // isel,irow,(*it).latin1(),relation->attributeValue( irow, *it ).latin1());
                values += relation->attributeValue( irow, *it );
            }
        
            // put key values into a string before adding to stringlist
        list += values.join( sep );
    }

    return list;
}


// --------------------------------------------------------------------------------
void QmvRelationWidget::setTitle()
{
        // build a title from relation-title and current set data
    int i = getCurrentRow() + 1;
    int j = relation->count();

    QString title = QString("%1 ").arg( relation->relationTitle() );
    
    if ( i > j || i == 0 )
        title.append( QString( "(%1)" ).arg( j ) );
    else
        title.append( QString ( "(%1 of %2)" ).arg(i).arg(j) );
    

        // Get the metrics of the title
    int height = label_title->height();
    QRect textrect = label_title->fontMetrics().boundingRect(title);
    int width = textrect.width() + 8;
    if ( width < 100 )
        width = 100;
    
    label_title->setFixedWidth( width );
    
    QPixmap t_pm(QmvRelationWidgetIcons::title_bg_xpm);
    QPixmap pm( label_title->size() );
    for ( int i = 0; i < width; i += t_pm.width() )
        bitBlt( &pm, QPoint(i, 0), &t_pm, t_pm.rect() );
    
    QPainter pt;
    
    pt.begin(&pm);
    pt.setFont( label_title->font() );
    pt.setPen( white );
    pt.drawText( (width - textrect.width())/2, (height - textrect.height())/2, textrect.width(), textrect.height(),
                 Qt::AlignCenter, title );
    pt.end();

    label_title->setBackgroundPixmap( pm );    
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::setHeaderAttributeList( const QStringList & headatts )
{
    QStringList header_attributes = headatts;

    clearTable();
    attlist_filter = QmvClass::NoBlank|QmvClass::NoSystem|QmvClass::NoZero;
    QmvAttList attlist = relation->sortedAttlist( "mtat_view_order", attlist_filter );
    int ncols = attlist.count();
    table_relation->setNumCols( ncols );

    for (QmvAttribute * at_ptr = attlist.first(); at_ptr != 0; at_ptr = attlist.next())
    {
        int col = attlist.at();
        if ( col < 0 || col > (ncols - 1) )
            continue;
        display_attributes.insert( col, at_ptr->find( "mtat_name" ) ); // attname reference list
        int displen = at_ptr->attributeDisplen().toInt();
        if ( displen > 20 )
            displen = 20;
        table_relation->horizontalHeader()->setLabel( col,
                                                      (col < headatts.count()) ? headatts[col] : at_ptr->attributeTitle() );

        table_relation->setColumnWidth( col, table_relation->horizontalHeader()->fontMetrics().width("M") * displen );
        if ( displen < 1 )
            table_relation->setColumnStretchable( col, TRUE );
    }
    
    table_relation->adjustSize();
    table_relation->updateGeometry();
    
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRelationWidget::hideColumn( int col, bool hide )
{
    if ( hide )
        table_relation->hideColumn( col );
    else
        table_relation->showColumn( col );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRelationWidget::hideRow( int row, bool hide )
{
    if ( hide )
        table_relation->hideRow( row );
    else
        table_relation->showRow( row );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRelationWidget::setColumnStretchable( int col, bool stretch )
{
    table_relation->setColumnStretchable( col, stretch );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRelationWidget::setRowStretchable( int row, bool stretch )
{
    table_relation->setRowStretchable( row, stretch );
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::clearTable()
{
    
    parent_dbw->statusBar()->message( tr("Clearing display ...") );
    parent_dbw->progressBar()->setTotalSteps(table_relation->numCols());
    parent_dbw->progressBar()->reset();
        // Clear the current display
    for ( int r = 0; r < table_relation->numRows(); r++ )
    {
        if ( r > 10 )
            parent_dbw->progressBar()->setProgress(r);
        for ( int c = 0; c < table_relation->numCols(); c++ )
        {
            table_relation->editCompleted( r, c );       // update pending edits.
            table_relation->clearCell( r, c );
            table_relation->clearCellWidget( r, c );
        }
    }

    table_relation->setNumRows(0);
    current_row = -1;
    table_relation->setCurrentCell( -1, -1 );
    slotSetNavigationButtons();
    table_relation->update();
    parent_dbw->progressBar()->reset();
    parent_dbw->statusBar()->clear();
}



// --------------------------------------------------------------------------------
void QmvRelationWidget::relationQuery()
{
    displayList();
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::relationQuery( int changetype, int count )
{
    switch( changetype )
    {
            // NewBlankSet, NewSet, SetReduced, SetIncreased, SetChanged, SingleChanged
        case QmvClass::SetIncreased:
        case QmvClass::SetReduced:
            displayList();
            break;
        default:
            break;
            
    }
    
}

// --------------------------------------------------------------------------------
//! Select the current row of the table
/*!
  
   Clear and current selections, then select the current row. Also select the parent
   row if there is a parent. Ensure visibility.
   
*/
// --------------------------------------------------------------------------------
void QmvRelationWidget::selectCurrentRow()
{
        // skip if empty
    if ( table_relation->numRows() < 1 )
    {
        current_row = -1;
        return;
    }
    
        // select the current row, after clearing current selection
    current_row = table_relation->currentRow();
    int ns = table_relation->numSelections();
    
    table_relation->clearSelection();
    QTableSelection curr_row_sel = QTableSelection();
    curr_row_sel.init( current_row, 0 );
    curr_row_sel.expandTo( current_row, table_relation->numCols() - 1 );
    int curr_sel_id = table_relation->addSelection( curr_row_sel );

        // announce the selection
    emit tupleSelected( id_index, relation->tupleAt(current_row) );

        // if there is a parent, then select the parent row.
    QmvRelationWidget * parent = getParentRelationWidget();
    if ( parent )
        parent->selectCurrentRow();

    setTitle();    
    slotSetNavigationButtons();

    ensureCurrentRowVisible();
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::toggleChildSelection()
{
    if ( current_child < 0 || current_child > (popup_children->count() - 1) )
        return;
    else {
        current_child = -1;
        parent_dbw->focusPanel( id_index );
    }
    
}


// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
void QmvRelationWidget::showTitleBar( bool state )
{
    if ( state )
        hbox_title->show();
    else
        hbox_title->hide();
}

    
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
void QmvRelationWidget::showToolBar( bool state )
{
    if ( state )
        hbox_toolbar->show();
    else
        hbox_toolbar->hide();
}

// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
void QmvRelationWidget::showTabBar( bool state )
{
    if ( state )
        hbox_tabbar->show();
//        tab_children->show();
    else
        hbox_tabbar->hide();
//        tab_children->hide();
}

    
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRelationWidget::preparePopupChildMenu()
{
    popup_children->removeItem( popup_children->idAt(0));
        // the docs are unclear about this
    if ( !label_children )
        label_children = new QLabel("QLabels are autodeleted", popup_children );
    
    label_children->setText( relation->userLabelValue( getCurrentRow() ) );
    
    popup_children->insertItem( label_children, -1, 0 );
    
    for (int i = 0; i < popup_children->count(); i++ )
        popup_children->setItemChecked( popup_children->idAt( i ), FALSE );

        // set the active item
    if ( current_child >= 0 && current_child < popup_children->count() )
    {
        popup_children->setItemChecked( current_child, TRUE );
        popup_children->setActiveItem( current_child );
    }
    
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRelationWidget::popupChildMenu( const QPoint & pos )
{
    if ( relation->count() < 1 || popup_children->count() < 1)
        return;
    QPoint mpos;
    if ( pos.isNull() )
        mpos = QCursor::pos();
    else
        mpos = pos;

        // use the return value -
        // (the signals in the popup disturb the order of panel resizing)

    int i = popup_children->exec( mpos, current_child );
    if ( i >= 0 )
        tab_children->setCurrentTab(i);
    
//        handleChildTabSelection(i);
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::handleChildMenuSelection( int id )
{
        // let the tab widget handler handle the details.
    handleChildTabSelection( id );
     
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::handleChildTabSelection( int id )
{
    if ( popup_children->count() < 1 )
        return;

        // only act if valid id.
    if ( dict_children.find(id) )
    {
            // toggle the selection if already set.
        if ( current_child == id )
        {  
            toggleChildSelection();
            tab_children->setTabActive( id, FALSE );
        } else {
            current_child = id;
            if ( id >= 0 )
                selectChild( current_child );
        }
    }
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::selectChild( int id_selected )
{
    QString * ptr = dict_children[id_selected];
    if ( !ptr )
        return;

    QString child_name = *ptr;
        // if namelength zero, then close children
    if ( child_name.length() == 0 )
    {
            // a null child-name should focus this panel.
        emit rowSelected( id_index, child_name, "" );
        return;
    }
        // select the current row
    selectCurrentRow();
    if ( current_row < 0 )
        return;
    
        // pass the signal on with a select string
    QString sep = "|";
    QStringList chkval = getCurrentSelection( child_name, sep );
    QString query;
    
    try
    {
        query = relation->selectChildQuery( child_name, chkval, sep );
    }
    catch ( QmvException * exception )
    {
        QMessageBox::Icon icon;
        switch ( exception->eStatus() )
        {
            case QmvException::Critical:
                icon = QMessageBox::Critical;
                break;
            case QmvException::Warning:
                icon = QMessageBox::Warning;
                break;
            case QmvException::Information:
            default:
                icon = QMessageBox::Information;
                break;
        }
        QMessageBox mbox( QString( "%1: Warning - Child relation not available" )
                          .arg( "QmvRelationWidget::selectChild()" ),
                          exception->eMessage(),
                          icon,
                          QMessageBox::Ok,
                          QMessageBox::NoButton,
                          QMessageBox::NoButton);
        mbox.exec();
        delete exception;
        return;
    }
    if ( query.length() > 0 )
        emit rowSelected( id_index, child_name, query );
    ensureCurrentRowVisible();
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::setBaseQuery( const QString & query )
{
    qbe_relation->setBaseQuery( query );
}


// --------------------------------------------------------------------------------
//! getFullQuery
/*!
  Get the composite query - base (immutable) query plus the user filter

*/
// --------------------------------------------------------------------------------
QString  QmvRelationWidget::getCompositeQuery() const
{
    QmvSqlQuery * qobj = qbe_relation->getQueryObject();
    if ( qobj )
        return qobj->sqlQuery();
    else
        return QString();
}


// --------------------------------------------------------------------------------
void QmvRelationWidget::currentCellChanged( int row, int col )
{
    return;
    
    if ( current_row != row )
        selectCurrentRow();

}

    
// --------------------------------------------------------------------------------
//! Respond to click in cell.
/*!
  Respond to mouse click in cell, selecting the row as a common response. Middle
  button click will also activate the current child panel. Right button click
  will popup the child selection menu.

  \param row    
  \param col    
  \param button 
  \param pos    
*/
// --------------------------------------------------------------------------------
void QmvRelationWidget::cellClicked( int row, int col, int button, const QPoint & pos )
{
    
    if ( row < 0 )
        return;
    
    QString reln = relation->relationName();
    int sequence = relation->attributeValue( row, relation->primaryKey() ).toInt();
    int old_row = current_row;

        // select the row
    selectRow( row );

        // update the position displays, if changed
    if ( current_row != old_row )
    {
        setTitle();
        slotSetNavigationButtons();
    }

        // Different actions depend on mouse buttons - TODO: assess need for this.
    
    switch ( button )
    {
                // Middle button -> access the current row
        case Qt::MidButton:

            emit rowAccessed( reln, sequence );
            return;
            break;

                // right button -> menu
                // don't pass pos, as this is relative to the widget connected to this slot.
                // - popupChildMenu() will default to QCursor::pos().
        case Qt::RightButton:
            popupChildMenu();
            return;
            break;
            
        case Qt::LeftButton:
        default:
            return;
            break;
    }
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::rowClicked( int row )
{
    selectRow( row );
    emitClicked( row );
    QString reln = relation->relationName();
    int sequence = relation->attributeValue( row, relation->primaryKey() ).toInt();
    emit rowAccessed( reln, sequence );


    setTitle();
}


// --------------------------------------------------------------------------------
//! Select the specified row.
/*!
  Select the row (\em row) of the table. Use \ref setCurrentCell() to handle the details.

  \param row    
*/
// --------------------------------------------------------------------------------
void QmvRelationWidget::selectRow( int row )
{
    current_row = row;
    table_relation->setFocus(); // make sure the table has kb focus
    table_relation->setCurrentCell( current_row, table_relation->currentColumn() );
    selectCurrentRow();
}


// --------------------------------------------------------------------------------
void QmvRelationWidget::colClicked( int col )
{
        //
}


//! Return a pointer to the QmvTupleAttribute for the specified cell.
/*!
  Return a pointer to the QmvTupleAttribute for the \emrow, \em col cell.

  \param row            Row
  \param col            Column
  \return               QmvTupleAttribute pointer or 0
*/

// --------------------------------------------------------------------------------
QmvTupleAttribute * QmvRelationWidget::getAttribute( int row, int col )
{
    if ( row < 0 )
        return 0;
    if ( col < 0 || col > display_attributes.count() )
        return 0;
    
    return relation->tupleAttribute( row, *display_attributes[col] );
    
}

    

// --------------------------------------------------------------------------------
void QmvRelationWidget::emitClicked( int row )
{
    
        // handle any local stuff here
    
    QString child_name;
    QString query;

        // If browse mode then don't look for children

    if ( current_child >= 0 && dict_children.count() > 0 )
    {
        QString * ptr = dict_children[current_child];
        if ( ptr )
        {
            child_name = *ptr;
            if ( child_name.length() > 0 )
            {
                    // non-zero child name - setup for child relation
                    // else the signal should focus the current pane.
                QString sep = "|";
                QStringList chkval = getCurrentSelection( child_name, sep );
                try
                {
                    query = relation->selectChildQuery( child_name, chkval, sep );
                }
                catch ( QmvException * exception )
                {
                    QMessageBox::Icon icon;
                    switch ( exception->eStatus() )
                    {
                        case QmvException::Critical:
                            icon = QMessageBox::Critical;
                            break;
                        case QmvException::Warning:
                            icon = QMessageBox::Warning;
                            break;
                        case QmvException::Information:
                        default:
                            icon = QMessageBox::Information;
                            break;
                    }
                    QMessageBox mbox( QString( "%1: Warning - Child relation not available" )
                                      .arg( "QmvRelationWidget::selectChild()" ),
                                      exception->eMessage(),
                                      icon,
                                      QMessageBox::Ok,
                                      QMessageBox::NoButton,
                                      QMessageBox::NoButton);
                    mbox.exec();
                    delete exception;
                    return;
                }
            }
            

        } else
            return;
    }
    
        // pass the signal on with primary key value
    emit rowSelected( id_index, child_name, query );
    
}

// --------------------------------------------------------------------------------
//! Display the current dataset
/*!
  Rebuild the display table with the current dataset. If \em query exists, then refersh
  the dataset from persistent storage

  \param query  
*/
// --------------------------------------------------------------------------------
void QmvRelationWidget::displayList( const QString & query )
{

    clearTable();

    if ( !relation )
    {
        QMessageBox::warning( this, "QmvRelationWidget: Relation List",
                              "RELATION cannot be found");
        return;
    }
        // message
    parent_dbw->statusBar()->message( tr("Displaying records ...") );

        // new query - so load current set.
    if ( query.length() > 0 )
    {
    
        QProgressDialog controller( QString( "Loading records..." ),
                                    "Cancel", 100,
                                    this, "progress", TRUE );
        
        int status = relation->open( query, &controller );
        controller.cancel();
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
                parent_dbw->statusBar()->clear();
                return;
                break;
            case -2:
                QMessageBox::warning( 0, "warning",
                                      tr("<b>%1:</b><br>"
                                         "The data query has been cancelled as requested.")
                                      .arg( relation->relationTitle() ),
                                      "OK", 0 );
                parent_dbw->statusBar()->clear();
                return;
                break;
            default:
                break;
        }
    }

    table_relation->verticalHeader()->setUpdatesEnabled( FALSE );
    table_relation->viewport()->setUpdatesEnabled( FALSE );
    table_relation->setUpdatesEnabled( FALSE );
    
        // set left margin - the alert-pixmap width depends on number of columns
        // Alert pixmap attributes
    int max_alert_rows = 3;
    QSize alert_cell(3, 3);
    int alert_spacing = 1;
    int alert_border = 1;
        // Calculate the space required.
    int pm_width = (table_relation->numCols()/max_alert_rows + 1) * (alert_cell.width()+alert_spacing)
        - alert_spacing
        + (alert_border*2);
    
    table_relation->setLeftMargin(table_relation->verticalHeader()->fontMetrics().width("33333") + pm_width + 5 );
    
        // Load the table with data
    table_relation->setNumRows( relation->count() );
    alert_map.setAutoDelete(true);
    alert_map.clear();
    parent_dbw->progressBar()->setTotalSteps(relation->count());
    parent_dbw->progressBar()->reset();
    for (int row = 0; row < relation->count(); row++)
    {
        if ( row > 10 )
            parent_dbw->progressBar()->setProgress(row);
        displayRow( row );
    }
    
    
        // limit extent of adjustment - expensive
    if ( relation->count() <= 20 )
        for (int row = 0; row < relation->count(); row++)
            table_relation->adjustRow( row );

    current_row = ( table_relation->numRows() > 0 ) ? 0 : -1;
    int current_col = ( table_relation->numCols() > 0 ) ? 0 : -1;
    table_relation->setCurrentCell( current_row, current_col );
    
    table_relation->verticalHeader()->setUpdatesEnabled( TRUE );
    table_relation->viewport()->setUpdatesEnabled( TRUE );
    table_relation->setUpdatesEnabled( TRUE );
    table_relation->verticalHeader()->update();
    table_relation->updateCornerWidget();
    table_relation->viewport()->update();
    table_relation->update();

        // get focus for this panel
    emit rowSelected( id_index, "", "" );

    if ( current_row >= 0 )
        selectRow( current_row );
    setTitle();
    
    parent_dbw->progressBar()->reset();
    parent_dbw->statusBar()->clear();
    
}

// --------------------------------------------------------------------------------
bool QmvRelationWidget::displayRow( int row )
{
//    alert_map.insert( row, new QmvBitGraph( table_relation->numCols(), table_relation, "altertmap") );
    
    bool alert = false;
    QmvBitGraph * bg = new QmvBitGraph(display_attributes.count(), this, "bitgraph-alerts" );
    for ( int col = 0; col < display_attributes.count(); col++ )
    {
        bg->slotSetBit( col, getAttribute( row, col )->alertColour() );
        alert |= displayCell( row, col );
    }

    QPixmap pm_alert = bg->pixmap(QSize(3,3), QSize(0,3),
                                  table_relation->verticalHeader()->backgroundColor(),
                                  QColor(Qt::gray),
                                  1, 1);
    
        // is there an icon already?
    QIconSet * ic = table_relation->verticalHeader()->iconSet(row);
    if (!ic)
        table_relation->verticalHeader()->setLabel( row, QIconSet(pm_alert, QIconSet::Small),
                                                    QString("%1").arg(row+1).rightJustify(3));
    else
        ic->setPixmap( pm_alert, QIconSet::Small);
        

    return alert;
}
// --------------------------------------------------------------------------------
bool QmvRelationWidget::displayCell( int row, int col )
{
    QmvTableItem * it = (QmvTableItem *) table_relation->item( row, col );

    QmvTupleAttribute * at = getAttribute( row, col );
        // Skip if nothing to be done.
    if ( it && !it->itemHasChanged( at ) )
        return at->isAlert();
    
    QmvAttribute::input_method  im = at->inputMethod();
    QmvAttribute::access_method am = at->accessMethod();
    QmvAttribute::data_type     dt = at->dataType();
    QmvTableItem::EditType      et = QmvTableItem::OnTyping;
    
    switch (im) //InputUnknown, Direct, List
    {
        case QmvAttribute::CopyText:
        case QmvAttribute::InputUnknown:
        case QmvAttribute::Direct:
        case QmvAttribute::List:
            et = QmvTableItem::OnTyping;
            break;
    }       
    switch (am)
    {
        case QmvAttribute::ReadOnly:
            et = QmvTableItem::Never;
            break;
            
    }
    
    table_relation->clearCellWidget( row, col );
        // new replaces old (auto deleted)
    QmvTableItem * item = new QmvTableItem( table_relation, et, at );
    item->setWordWrap( TRUE );
    table_relation->setItem( row, col, item );
    
    return at->isAlert();
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::refreshList( )
{
    table_relation->update();
    table_relation->adjustSize();
    table_relation->updateGeometry();
    adjustSize();
    updateGeometry();
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::refreshRowByAttribute( const QString & value, const QString & attr )
{
    for ( int i = 0; i < relation->count(); i++ )
        if ( value == relation->attributeValue( i, attr ) )
            refreshRow(i);
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::refreshRowBySequenceId( int seq )
{
    for ( int i = 0; i < relation->count(); i++ )
        if ( seq == relation->attributeValue( i, relation->primaryKey() ).toInt() )
        {
            refreshRow(i);
            return;
        }
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::refreshAll()
{
    for ( int i = 0; i < relation->count(); i++ )
    {
        refreshRow(i);
    }
}


// --------------------------------------------------------------------------------
//! Progagate refresh to whole set
/*!
  Refresh the entire set if this panel is configured to respond in this way.

*/
// --------------------------------------------------------------------------------
void QmvRelationWidget::refreshPropagate()
{
    if (always_refresh_set)
    {
        refreshAll();
    }
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::refreshRow( int row )
{
    if ( ! relation->tupleAt( row ) )
        return;
    
    relation->tupleAt( row )->reload();
    for ( int col = 0; col < display_attributes.count(); col++ )
        refreshCell( row, col );
    
}
// --------------------------------------------------------------------------------
void QmvRelationWidget::refreshCell( int row, int col )
{
    QmvTableItem * ti = (QmvTableItem *) table_relation->item( row, col );
    if ( ti )
        if ( ti->refreshText() )
            table_relation->updateCell( row, col );
    
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::ensureCurrentRowVisible( )
{
    table_relation->ensureCellVisible( current_row, table_relation->currentColumn() );
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRelationWidget::showMiniEditor( int row )
{
    rowClicked( row );
    showMiniEditor();
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRelationWidget::showMiniEditor()
{
    
    selectCurrentRow();
    if ( current_row < 0 )
        return;

//     QmvRowEditor * re = new QmvRowEditor( relation, true, this, "row editor" );
//     re->load(current_row );
//     re->exec();
//     delete re;
//         // Refresh this row in the main table
//     displayRow( current_row );
//     return;

        // ----------------------------------------------------------------------------
        // Build a popup single row editor (MODAL)
        // ----------------------------------------------------------------------------

    QDialog * dialog_editrow;
    dialog_editrow = new QDialog( this, "single row edit", TRUE, 0);
    dialog_editrow->setCaption( relation->relationTitle() );
    QAccel * ac_editrow = new QAccel( dialog_editrow );
    ac_editrow->connectItem( ac_editrow->insertItem( CTRL+Key_W ), dialog_editrow, SLOT(close()) );

    QVBoxLayout * vl_editrow = new QVBoxLayout( dialog_editrow, 2 );
    
    QHBoxLayout * hl_editrow = new QHBoxLayout( vl_editrow, 0, "controls_editrow" );
    QLabel * testlabel = new QLabel( relation->relationTitle(), dialog_editrow );
    testlabel->setFocusPolicy( QWidget::NoFocus );
    hl_editrow->addWidget(testlabel);
    hl_editrow->addStretch( 10 );

        // QWhatsThis
    QToolButton * wtb = QWhatsThis::whatsThisButton(dialog_editrow);
    wtb->setFocusPolicy( QWidget::NoFocus );
    hl_editrow->addWidget(wtb);
    wtb->setAccel( Key_F1 );
    
        // Control: close button
    QToolButton * close_editrow = new QToolButton( dialog_editrow,
                                                   "close_editrow");
    close_editrow->setFocusPolicy( QWidget::NoFocus );
    close_editrow->setIconSet( stdicons->getIconSet(QmvIcons::QuitIcon) );
    close_editrow->setTextLabel( "Close the mini editor" );
    close_editrow->setAutoRaise(TRUE);
    QWhatsThis::add( close_editrow, "Close the mini editor");
    hl_editrow->addWidget(close_editrow);
    connect( close_editrow, SIGNAL( clicked() ), dialog_editrow, SLOT( accept() ) );
    close_editrow->setAccel( CTRL+Key_W );
    
        // Table: the single row editor
    table_editrow = new QmvTable( dialog_editrow, "relation_row" );
    vl_editrow->addWidget( table_editrow );
    
    table_editrow->setSorting( -1 );
    table_editrow->horizontalHeader()->setClickEnabled( FALSE,  -1 );
    table_editrow->verticalHeader()->setClickEnabled( FALSE,  -1 );
    table_editrow->setTopMargin(0);
    table_editrow->horizontalHeader()->hide();
    table_editrow->setShowGrid(true);
    table_editrow->slotSetToolTips(false);

    int n_att = table_relation->horizontalHeader()->count();
    table_editrow->setNumCols( 1 );
    table_editrow->setNumRows( n_att );

        // Copy the attribute labels from the main table.
    int label_width = 0;
    for ( int i_col = 0; i_col < n_att; i_col++ )
    {
        QString lab = table_relation->horizontalHeader()->label( i_col);
            // rotated 90 deg.
        table_editrow->verticalHeader()->setLabel( i_col, lab );
        int labwid =  table_editrow->verticalHeader()->fontMetrics().width( lab );
        label_width = ( labwid > label_width ) ? labwid : label_width;
    }
    table_editrow->setLeftMargin( (label_width < 200) ? label_width : 200 );
    table_editrow->setMinimumWidth( table_editrow->minimumWidth() +
                                    table_editrow->verticalHeader()->width() );
    table_editrow->adjustColumn( 0 );
    table_editrow->updateCornerWidget();
    dialog_editrow->adjustSize();
    
        // load the data
    
    miniEditRow( current_row );

        // resize
    int tabwid = table_editrow->verticalHeader()->width()
        + table_editrow->columnWidth(0)
        + 20;   // ?? why??
    if ( tabwid > QApplication::desktop()->width() )
        tabwid = QApplication::desktop()->width();
    dialog_editrow->resize( tabwid, dialog_editrow->height() );
    dialog_editrow->updateGeometry();

    connect( table_editrow, SIGNAL( reloadRequest( int, int ) ),
               this, SLOT( miniEditRefresh( ) ) );
    
        // Go for it
    dialog_editrow->exec();
    delete dialog_editrow;

        // Refresh this row in the main table
    displayRow( current_row );
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::miniEditRefresh()
{
    
    int max_colwidth = 200; //arbitrary

    table_editrow->setColumnWidth( 0, max_colwidth );
    for ( int col = 0; col < display_attributes.count(); col++ )
        miniEditCell( current_row, col );
    
}
// --------------------------------------------------------------------------------
void QmvRelationWidget::miniEditRow( int row )
{
    
    int max_colwidth = 200; //arbitrary
    

    table_editrow->setColumnWidth( 0, max_colwidth );
    for ( int col = 0; col < display_attributes.count(); col++ )
        miniEditCell( row, col );
    
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::miniEditCell( int row, int col )
{
        // NOTE: row and col refer to row in main table
    
    QmvTableItem *it = (QmvTableItem *) table_editrow->item( col, 0 );    

    QmvTupleAttribute * at = getAttribute( row, col );
        // Skip if nothing to be done.
    if ( !at || (it && !it->itemHasChanged( at )) )
        return;

    QmvAttribute::input_method  im = at->inputMethod();
    QmvAttribute::access_method am = at->accessMethod();
    QmvAttribute::data_type     dt = at->dataType();
    QmvTableItem::EditType et;
            
    switch (im) //InputUnknown, Direct, List
    {
        case QmvAttribute::CopyText:
        case QmvAttribute::InputUnknown:
        case QmvAttribute::Direct:
        case QmvAttribute::List:
            et = QmvTableItem::OnTyping;
            break;
    }
    switch (am)
    {
        case QmvAttribute::ReadOnly:
            et = QmvTableItem::Never;
            break;
                
    }

    table_editrow->clearCellWidget( col, 0 );
    QmvTableItem * item = new QmvTableItem( table_editrow, et, at );
    item->setWordWrap( TRUE );
        // col is used for row, as this is a vertically aligned table.
    table_editrow->setItem( col, 0, item );
    table_editrow->adjustRow( col );
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::relationMenu( int section )
{
    if (!menu_relation)
        return;

    setMenu( table_relation->verticalHeader(), section, menu_relation );
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::clear()
{
    clearTable();
}

// --------------------------------------------------------------------------------
const void QmvRelationWidget::setMenu( const QHeader * hd, int section, QPopupMenu * menu ) const
{
    QPoint mousePos = QPoint( hd->geometry().left() + hd->sectionPos(section) - hd->offset(),
                              hd->geometry().top() );
    
    menu->popup( hd->mapToGlobal(mousePos ));
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::addToSet( )
{
    addToSet(true);
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::addToSet(bool broadcast )
{
    if ( !relation )
        return;

    int new_row = -1;
        // if we have a parent, then establish the new tuple(s) as child(ren).
    if ( getParentRelation() )
    {
        QString my_name = getRelation()->relationName();
        QStringList my_key = getParentRelation()->childJoin( my_name );
        QString sep = "|";
        QStringList parentlist_bykeys = getParentRelationWidget()->getCurrentSelection( my_name, sep );
        if ( parentlist_bykeys.count() < 1 )
        {
            QMessageBox::warning(0, "Add Row:Warning",
                                 tr("<b>%1:</b><br><br>No parent row was selected<br>")
                                 .arg(relation->relationTitle()),
                                 "OK", 0 );
            return;
        }
        
            // add records for each parent -
            // the parent list is the second level, key list is the first,
            // so get the first parent valuelist, and use it for the iteration loop limit.
        QStringList valuelist = QStringList().split( sep, parentlist_bykeys[0] );

        for ( int iparent = 0; iparent < valuelist.count(); iparent++ )
        {
                // a new child tuple for each selected parent row
                // do not auto-save, since data needs to be massaged with FK first.
            int row = relation->newTuple(FALSE);
            if ( row < 0 )
            {
                QMessageBox::warning(0, "Add Row:Warning",
                                     tr("<b>%1:</b><br><br>Row-creation failed<br><b>%2</b>")
                                     .arg(relation->relationTitle())
                                     .arg(relation->lastError()),
                                     "OK", 0 );
                return;
            }

            for ( int ikey = 0; ikey < my_key.count(); ikey++ )
            {
                QStringList parentvaluelist = QStringList().split( sep, parentlist_bykeys[ikey] );
                relation->update( row, my_key[ikey], parentvaluelist[iparent] );
            }
            
            
            if ( !relation->save( row ) )
            {
                QMessageBox::warning(0, "Add Row:Warning",
                                     tr("<b>%1:</b><br><br>The data-save failed<br><b>%2</b>")
                                     .arg(relation->relationTitle())
                                     .arg(relation->lastError()),
                                     "OK", 0 );
                return;
            }
                // first inserted row will become the current row.
            if ( iparent == 0 )
                new_row = row;
            
        }
            // remember the new row - its position may change in reload
        QString sk = relation->attributeValue( new_row, relation->serialKey() );
            // force a reload - since there is a parent, the new row(s) will be included.
        displayList( relation->queryDescription() );
            // find the new position
        new_row = relation->rowBySK( sk );
        
    } else {
            // no parent - so add a single record.
        int row = relation->newTuple(TRUE);
        if ( row < 0 )
        {
            QMessageBox::warning(0, "Add Row:Warning",
                                 tr("<b>%1:</b><br><br>The data-save failed<br><b>%2</b>")
                                 .arg(relation->relationTitle())
                                 .arg(relation->lastError()),
                                 "OK", 0 );
            return;
        }
        new_row = row;
            // no parent - so do not force a reload ( the new rows may not be in the set)
        displayList(  );
            // this is only relevant for parents - since children reload query
        table_relation->setCurrentCell( new_row, 0 );
        table_relation->ensureCellVisible( new_row, 0 );
        selectCurrentRow();
    }

    if ( new_row >= 0 )
    {
        QString reln = relation->relationName();
        int sequence = relation->attributeValue( new_row, relation->primaryKey() ).toInt();
            // this will select the current row - then it will be available
            // to processes responding to the subsequent rowAdded() signal.
        selectRow( new_row );
        if (broadcast)
            emit rowAdded( reln, sequence );        
        refreshRow( new_row );
    }
    
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::copyToSet( )
{
    if ( !relation )
        return;

    if ( current_row < 0 )
    {
        QMessageBox::warning( 0, "copyToSet: Warning",
                              tr("Select a row to indicate the row to copy."),
                              "OK", 0 );
        return;
    }
    
        // Emit warning signal
    QString reln = relation->relationName();
    int sequence = relation->attributeValue( current_row, relation->primaryKey() ).toInt();
    emit rowAboutToBeCopied( reln, sequence );

        // make the copy
    int row = relation->copyTuple( current_row, TRUE);
    if ( row < 0 )
    {
        QMessageBox::warning(0, "Copy Row:Warning",
                             tr("<b>%1:</b><br><br>The data-save failed<br><b>%2</b>")
                             .arg(relation->relationTitle())
                             .arg(relation->lastError()),
                             "OK", 0 );
        return;
    }
    int new_row = row;

        // handle redisplay, based on presence of parent
    if ( getParentRelation() )
    {
            // remember the new row - its position may change in reload
        QString sk = relation->attributeValue( new_row, relation->serialKey() );
            // force a reload - since there is a parent, the new row(s) will be included.
        displayList( relation->queryDescription() );
            // find the new position
        new_row = relation->rowBySK( sk );
    } else {
            // no parent - so do not force a reload ( the new rows may not be in the set)
        displayList();
        table_relation->setCurrentCell( new_row, 0 );
        table_relation->ensureCellVisible( new_row, 0 );
        selectCurrentRow();
    }

        // cleanup
    reln = relation->relationName();
    sequence = relation->attributeValue( new_row, relation->primaryKey() ).toInt();
        // this will select the current row - then it will be available
        // to processes responding to the subsequent rowAdded() signal.
    selectRow( new_row );
    emit rowAdded( reln, sequence );        
    refreshRow( new_row );
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::deleteFromSet( )
{
    if ( !relation )
        return;

    if ( current_row < 0 )
    {
        QMessageBox::warning( 0, "deleteFromSet: Warning",
                              tr("Select a row to indicate the row to delete."),
                              "OK", 0 );
        return;
    }
    
        
        // delete a  record
    int row = current_row;
    
    QString reln = relation->relationName();
    int sequence = relation->attributeValue( row, relation->primaryKey() ).toInt();
    
    switch( QMessageBox::information( this, "deleteFromSet:Remove",
				      tr("Are you sure that you want to delete this record \n\n %1 : %2")
                                      .arg( relation->attributeValue( row, relation->userKey() ) )
                                      .arg( relation->attributeValue( row, relation->userLabel() ) ),
				      "Yes", "Cancel",
				      0, 1 ) )
    {
        case 0:
            emit rowAboutToBeDeleted( reln, sequence );
            if ( !relation->remove( row, TRUE ) )
            {
                QMessageBox::warning(0, "Delete Row:Warning",
                                     tr("<b>%1:</b><br><br>The delete failed<br><b>%2</b>")
                                     .arg(relation->relationTitle())
                                     .arg(relation->lastError()),
                                     "OK", 0 );
                return;
            }

            break;
        case 1:
        default: // just for sanity
            break;
    }
    emit tupleSelected( id_index, 0 ); // unselect
    displayList();

    current_row = ( row < relation->count() ) ? row : relation->count() - 1;
    selectRow( current_row );
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::printSet( )
{
    if ( !relation )
        return;

    QPrinter printer;
    printer.setOutputFileName( relation->relationName() + ".ps");
    printer.setOutputToFile(FALSE);

    if ( relation->count() < 1)
    {
        QMessageBox::warning( 0, "PrintSet:warning",
                              tr("<b>%1:</b><br><br>Printing failed - nothing to print.")
                              .arg(relation->relationTitle()),
                              "OK", 0 );
        return;
    }
    
    
        // printer dialog
    if ( printer.setup(this) )
    {	

    QApplication::setOverrideCursor( Qt::waitCursor );
    bool pstatus = relation->printReport( &printer );
    QApplication::restoreOverrideCursor();
    if ( pstatus )
        {
            QMessageBox::information(0, "PrintSet:information",
                                     tr("<b>%1:</b><br><br>Printing has completed.")
                                     .arg(relation->relationTitle()),
                                     "OK", 0 );
            return;
        } else {
            QMessageBox::warning( 0, "PrintSet:warning",
                                tr("<b>%1:</b><br><br>Printing failed.")
                                .arg(relation->relationTitle()),
                                "OK", 0 );
            return;
        }
    } else {
	
        QMessageBox::information(0, "PrintSet:information",
                                 tr("<b>%1:</b><br><br>Printing has been aborted.")
                                 .arg(relation->relationTitle()),
                                 "OK", 0 );
        return;
    }
}
// --------------------------------------------------------------------------------
void QmvRelationWidget::exportSet()
{

    
    QmvExport * xp = new QmvExport( getRelation(), "qmvrelationwidget" );
    xp->setInstaller( "transfer_to_syncdir %f" );
    xp->writePalmDBFiles( relation->relationName() + "_export" );
    delete xp;

    return;
      
          // the rest should be reviewed
//      QString filename = "export.xml";
    
//      QFile fe( filename );
//      if ( !fe.open( IO_WriteOnly ) )
//      {
//          QMessageBox::critical( 0, QString( "Fatal Error - File open" ),
//                                 QString( tr("Cannot open the export file <%1> for writing") )
//                                 .arg( filename ) );
//          return;
//      }
    
//      QTextStream te( &fe );
//      QmvExport * xp = new QmvExport( getRelation(), "qmvrelationwidget" );
//      xp->writeXMLDeclaration( te );
//      xp->writeXMLData( te );
//      fe.close();
                             
//  //      xp->writeXMLDeclaration( QString("%1.kud").arg( getRelation()->relationName() ));
//        delete xp;
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::importSet()
{
    // insert a row in preparation
    // TODO: addToSet() may have added more than one row - check instead of just using the first.
        // Add a new row to set, but don't broadcast
    addToSet(false);
    QString sk = relation->attributeValue( getCurrentRow(), relation->serialKey() );
    
    // call import script with following arguments
    //  1: this class name
    //  2 - N : primary key value of recipient rows
    QString installer = QString("%1 %2 %3 %4")
                        .arg( "import_set" )
                        .arg( getRelation()->dataBase()->dbName())
                        .arg( getRelation()->relationName() )
                        .arg( sk );;

    // A system call which returns success/failure
    int status = system(installer);
    
    QString status_header = QString("Importing rows");
    if ( status )
    {
        // need to remove the inserted row
        // - dont advertise the deletion, it's private
          relation->remove( current_row, true );
        QMessageBox mbox( status_header,
                          QString( "Failed to import records using;<br><br>"
                                   " <em>%1</em>" )
                          .arg( installer ),
                          QMessageBox::Warning,
                          QMessageBox::Ok,
                          QMessageBox::NoButton,
                          QMessageBox::NoButton);
        mbox.exec();
            // refresh current set
        displayList( relation->queryDescription() );
        return;
    }
    
    QMessageBox mbox( status_header,
                       QString( "%1 records have been imported using;<br><br>"
                                " <em>%2</em>")
                      .arg( 1 )
                      .arg( installer ),
                      QMessageBox::Information,
                      QMessageBox::Ok,
                      QMessageBox::NoButton,
                      QMessageBox::NoButton);
    mbox.exec();
        
      // refresh current set
    displayList( relation->queryDescription() );
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::formSet( )
{
    formSet( QString( "" ) );
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::formSet( const QString & form_code )
{
        // refresh the display
    refreshAll();
    
    if ( !reln_form )
        reln_form = new QmvSqlClass( getRelation()->dataBase(), "form" );

    if ( !form_code.length() )
        reln_form->open( QString( "select * from form where form_classname ~ '^%1'" )
                         .arg( getRelation()->relationName() ) );
    else
        reln_form->open( QString( "select * from form where form_code = '%1'::text").arg( form_code) );

    if ( ! form_select )
        form_select = new QmvSelect( reln_form, this );

        // Were any rows located?
    if ( reln_form->count() < 1 )
    {
        printSet();
        return;
    }
        // if more than 1 row, popup selector
    int row;
    if ( reln_form->count() == 1 )
        row = 0;
    else
        row = form_select->exec();

        // Get the form_code
    QString form;
    if ( row >= 0 && row < reln_form->count() )
        form = reln_form->attributeValue( row, "form_code" );
    else
        return; // cancelled.
    
    if ( !form_viewer )
        form_viewer = new QmvForm( getRelation(), this, "printform" );

    if ( !form_viewer->initForm( form ) )
    {
        QMessageBox::information(0, "FormSet:information",
                                 tr("<b>%1:</b><br><br>This form <b>%2</b> cannot be initialised.")
                                 .arg( getRelation()->relationTitle() )
                                 .arg( form ),
                                 "OK", 0 );
        return;
    }

    form_viewer->setCaption( QString("%1").arg( getRelation()->relationTitle() ));
        // set printer
    form_viewer->setPrint(
        parent_dbw->userPreference( "PrinterProgramForm"),
        parent_dbw->userPreference( "PrinterOptionsForm"),
        parent_dbw->userPreference( "PrinterNameForm") );
        // set fax
    form_viewer->setFax(
        parent_dbw->userPreference( "FaxProgramName"),
        parent_dbw->userPreference( "FaxProgramFormat"),
        parent_dbw->userPreference( "FaxFileName"),
        QStringList(""));
    form_viewer->setEmail(
        parent_dbw->userPreference( "EmailProgramName"),
        parent_dbw->userPreference( "EmailProgramFormat"),
        parent_dbw->userPreference( "EmailFileName"),
        QStringList(""));
 
    if ( parent_dbw->userPreference("ViewFormFirst").lower() == "yes" )
        form_viewer->viewForm();
    else
        form_viewer->printForm();

}

// --------------------------------------------------------------------------------
void QmvRelationWidget::setReadOnly( bool state )
{
    if ( read_only_state == state )
        return;
    
    read_only_state = state;
    if ( read_only_state )
    {
        table_relation->hide();
        if ( qbe_visible )
            qbe_relation->hide();
        hb_tup_functions->hide();
        setTupleDisplay();
        tuple_display->adjustSize();
        tuple_display->show();
        adjustSize();
        updateGeometry();
    } else {
            // Standard Browse mode
        table_relation->show();
        if ( qbe_visible )
            qbe_relation->show();
        hb_tup_functions->show();
        tuple_display->hide();
    }
    setTitle();
    
        // TODO: change widget display
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRelationWidget::toggleQueryWidgetVisible()
{
    setQueryWidgetVisible(!qbe_visible, true);
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRelationWidget::setQueryWidgetVisible( bool visible, bool refresh )
{
    qbe_visible = visible;
    if ( qbe_visible ) {
        qbe_relation->show();
        qbe_relation->setActive(true);
    } else {
            // when hidden, deactivate the conditions
        qbe_relation->hide();
        qbe_relation->setActive(false);
    }
        // if not a top-level panel, then redisplay the set
    if ( id_index > 0 && refresh ) {
        qbe_relation->getQueryObject()->updateRelationSet();
    }
    
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::setTupleDisplay()
{
    int row = getCurrentRow();
    tuple_display->setText( relation->userLabelValue( row ) );
    return;
}

// --------------------------------------------------------------------------------
bool QmvRelationWidget::eventFilter( QObject *o, QEvent *e )
{
#ifdef QMVRELATIONWIDGET_DEBUG
    const char *name = o ? o->name() : "NO_OBJECT";
    const char *classname = o ? o->className() : "NO_CLASS";
    int etype = e ? e->type() : -9999;
    qDebug( "%-20.20s::eventFilter(%20.20s/%20.20s, %6d)", "QmvRelationWidget", name, classname, etype );
#endif
    
    if ( !o || !e )
        return QWidget::eventFilter( o, e );
    
    if ( e->type() == QEvent::MouseButtonPress )
        if (o == nav_left )
            slotNextRow();
        else if ( o == nav_right )
            slotPreviousRow();
        else if ( o == label_title || o == nav_restore )
            toggleChildSelection();             // focus this panel to full size
    
    return QWidget::eventFilter( o, e );
}

// --------------------------------------------------------------------------------
void QmvRelationWidget::mousePressEvent( QMouseEvent * mpe )
{         
    QWidget::mousePressEvent( mpe );
    
}
    
// --------------------------------------------------------------------------------
//! Add a toolbutton to tuple toolgroup.
/*!

  \param name   object name
  \return       pointer to new toolbutton
*/
// --------------------------------------------------------------------------------
QToolButton * QmvRelationWidget::addTupleButton(char * name)
{
    
        // Add new tuple
    QToolButton * tf_tb = new QToolButton( hb_tup_functions, name );
    tf_tb->setAutoRaise(TRUE);

    return tf_tb;
}
