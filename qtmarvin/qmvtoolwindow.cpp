/*
 *     $Id: qmvtoolwindow.cpp 3138 2006-10-12 22:24:19Z source $
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
 * DESCRIPTION: Base class for application tool windows
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include "qmvtoolwindow.h"

namespace QmvToolWindowIcons
{
#include "left1arrow.xpm"
#include "right1arrow.xpm"
#include "left2arrow.xpm"
#include "right2arrow.xpm"
#include "print.xpm"
#include "days_1.xpm"
#include "days_5.xpm"
#include "days_7.xpm"
#include "month.xpm"
#include "today.xpm"
#include "agenda.xpm"
#include "zoomin.xpm"
#include "zoomout.xpm"
#include "find.xpm"
#include "quit.xpm"
} // end QmvToolWindowIcons

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmvtoolwindow_version[] = "$Id: qmvtoolwindow.cpp 3138 2006-10-12 22:24:19Z source $";
// #define QMVTOOLWINDOW_DEBUG

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

QmvToolWindow::QmvToolWindow( QmvSqlClass * reln, QmvApplication * parent, const char * name )
        : QMainWindow( parent, name, WType_TopLevel | WDestructiveClose | WGroupLeader )
{
    appl_reln = reln;
    parent_app = parent;
    if ( appl_reln )
    {        
        setBaseQuery( appl_reln->currentQuery() );
        if ( !name )
            setName( QString( "Application tool - %1" ).arg( appl_reln->relationTitle() ) );
    }
    setCaption( QObject::name() );

        // ------------------------------------------------------------
        // make a central widget to contain the other widgets
        // ------------------------------------------------------------
    
    QWidget * central = new QWidget( this, "central:Central Widget" );
    setCentralWidget( central );
    QGridLayout * topgrid = new QGridLayout( central, 1, 1, 0, -1, "topgrid: Top QmvToolWindow Grid" );
    
    stdicons = new QmvIcons( this, "qmvtoolwindow_icons" );
    
        // ------------------------------------------------------------
        // ---- Actions - Navigation options
        // ------------------------------------------------------------

    actiongp_nav = new QActionGroup( this, "actiongp_navigation" );
    QAction * ac;
    action_nav_idx.clear();
    
        // ------------------------------------------------------------
    ac = new QAction( "Back a longer way",
                      QPixmap( QmvToolWindowIcons::left2arrow_xpm ),
                      "Back a &longer way",
                      ALT+Key_Left,
                      actiongp_nav, "nav_back_4", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotBack4() ) );
    action_nav_idx.insert( AGNavBack4, ac );
    
        // ------------------------------------------------------------
    ac = new QAction( "Back a long way",
                      QPixmap( QmvToolWindowIcons::left2arrow_xpm ),
                      "Back a &long way",
                      CTRL+Key_Left,
                      actiongp_nav, "nav_back_3", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotBack3() ) );
    action_nav_idx.insert( AGNavBack3, ac );
    
        // ------------------------------------------------------------
    ac = new QAction( "Back a fair way",
                      QPixmap( QmvToolWindowIcons::left2arrow_xpm ),
                      "Back a &fair way",
                      SHIFT+Key_Left,
                      actiongp_nav, "nav_back_2", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotBack2() ) );
    action_nav_idx.insert( AGNavBack2, ac );
    
        // ------------------------------------------------------------
    ac = new QAction( "Back a short way",
                      QPixmap( QmvToolWindowIcons::left1arrow_xpm ),
                      "Back a &short way",
                      Key_Left,
                      actiongp_nav, "nav_back_1", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotBack1() ) );
    action_nav_idx.insert( AGNavBack1, ac );
    
        // ------------------------------------------------------------
    ac = new QAction( "Home",
                      QPixmap( QmvToolWindowIcons::agenda_xpm ),
                      "&Home",
                      Key_Home,
                      actiongp_nav, "nav_home", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotHome() ) );
    action_nav_idx.insert( AGNavHome, ac );
    
        // ------------------------------------------------------------
    ac = new QAction( "Forward a short way",
                      QPixmap( QmvToolWindowIcons::right1arrow_xpm ),
                      "Forward a &short way",
                      Key_Right,
                      actiongp_nav, "nav_forward_1", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotForward1() ) );
    action_nav_idx.insert( AGNavForward1, ac );
    
        // ------------------------------------------------------------
    ac = new QAction( "Forward a fair way",
                      QPixmap( QmvToolWindowIcons::right2arrow_xpm ),
                      "Forward a &fair way",
                      SHIFT+Key_Right,
                      actiongp_nav, "nav_forward_2", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotForward2() ) );
    action_nav_idx.insert( AGNavForward2, ac );
    
        // ------------------------------------------------------------
    ac = new QAction( "Forward a long way",
                      QPixmap( QmvToolWindowIcons::right2arrow_xpm ),
                      "Forward a &long way",
                      CTRL+Key_Right,
                      actiongp_nav, "nav_forward_3", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotForward3() ) );
    action_nav_idx.insert( AGNavForward3, ac );
    
        // ------------------------------------------------------------
    ac = new QAction( "Forward a longer way",
                      QPixmap( QmvToolWindowIcons::right2arrow_xpm ),
                      "Forward a &longer way",
                      ALT+Key_Right,
                      actiongp_nav, "nav_forward_4", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotForward4() ) );
    action_nav_idx.insert( AGNavForward4, ac );
    
        // ------------------------------------------------------------
    ac = new QAction( "Print",
                      QPixmap( QmvToolWindowIcons::print_xpm ),
                      "&Print",
                      Key_P,
                      actiongp_nav, "nav_print", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotPrint() ) );
    action_nav_idx.insert( AGNavPrint, ac );
    
        // ------------------------------------------------------------
    ac = new QAction( "Display 1",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                   tr("1"), QFont("Times"), QColor( black ), QRect() ),
                      "Display &1",
                      ALT+Key_1,
                      actiongp_nav, "nav_display1", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotDisplay1() ) );
    action_nav_idx.insert( AGNavDisplay1, ac );
    
        // ------------------------------------------------------------
    ac = new QAction( "Display 2",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                   tr("2"), QFont("Times"), QColor( black ), QRect() ),
                      "Display &2",
                      ALT+Key_2,
                      actiongp_nav, "nav_display2", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotDisplay2() ) );
    action_nav_idx.insert( AGNavDisplay2, ac );
    
        // ------------------------------------------------------------
    ac = new QAction( "Display 3",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                   tr("3"), QFont("Times"), QColor( black ), QRect() ),
                      "Display &3",
                      ALT+Key_3,
                      actiongp_nav, "nav_display3", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotDisplay3() ) );
    action_nav_idx.insert( AGNavDisplay3, ac );
    
        // ------------------------------------------------------------
    ac = new QAction( "Zoom In",
                      QPixmap( QmvToolWindowIcons::zoomin_xpm ),
                      "Zoom In",
                      Key_Plus,
                      actiongp_nav, "nav_zoomin", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotZoomIn() ) );
    action_nav_idx.insert( AGNavZoomIn, ac );
    
        // ------------------------------------------------------------
    ac = new QAction( "Zoom Out",
                      QPixmap( QmvToolWindowIcons::zoomout_xpm ),
                      "Zoom Out",
                      Key_Minus,
                      actiongp_nav, "nav_zoomout", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotZoomOut() ) );
    action_nav_idx.insert( AGNavZoomOut, ac );
    
        // ------------------------------------------------------------
    ac = new QAction( "Find",
                      QPixmap( QmvToolWindowIcons::find_xpm ),
                      "Find",
                      CTRL+Key_F,
                      actiongp_nav, "nav_find", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotFind() ) );
    action_nav_idx.insert( AGNavFind, ac );
    
    
    
        // ------------------------------------------------------------
        // ---- Actions - other
        // ------------------------------------------------------------

    actiongp_other = new QActionGroup( this, "actiongp_other" );
    
    
        // ------------------------------------------------------------
        // ---- Actions - general
        // ------------------------------------------------------------

    actiongp_gen = new QActionGroup( this, "actiongp_general" );

    ac = new QAction( "Close",
                      QPixmap( QmvToolWindowIcons::quit_xpm ),
                      "Close",
                      CTRL+Key_W,
                      actiongp_gen, "gen_quit", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( hide() ) );
    
        // ------------------------------------------------------------
        // ---- Tool bars -----------------------------------------
        // ------------------------------------------------------------
    
    toolbar_nav = new QToolBar( this, "toolbar_navigation" );
    toolbar_nav->setHorizontalStretchable(FALSE);
    actiongp_nav->addTo( toolbar_nav );
    
    toolbar_other = new QToolBar( this, "toolbar_other" );
    toolbar_other->setHorizontalStretchable(FALSE);
    actiongp_other->addTo( toolbar_other );

    QToolBar * toolbar_stretch = new QToolBar( this, "toolbar_stretch" );
    toolbar_stretch->setHorizontalStretchable( TRUE );
    
    toolbar_gen = new QToolBar( this, "toolbar_general" );
    toolbar_gen->setHorizontalStretchable(FALSE);
    actiongp_gen->addTo( toolbar_gen );

    banner = new QLabel( "Tool Window", toolbar_other, "banner" );
    
        // ----------------------------------------
        // Build the canvas
        // ----------------------------------------
                 
        // magnification scales
        // Start with a bigger canvas - this will be reduced
        // to the canvasview viewport when the canvasView is displayed.
    QSize canvas_size = QSize( 1024, 768 );
    setMagFactor(1.0, 3.0, 4);
    mag_current = 0;
    canvas = new QCanvas( canvas_size.width(), canvas_size.height() );
    canvas_size = canvas->size();
    canvasview = new QmvCanvasView( canvas, central );
    topgrid->addWidget( canvasview, 0, 0 );
    canvasview->setHScrollBarMode( QScrollView::AlwaysOn );
    canvasview->setVScrollBarMode( QScrollView::AlwaysOn );
    transparent = QColor( white );
    
        // prepare for display
    adjustSize();
    canvasview->adjustSize();

    init();
    show();
    adjustSize();
    updateGeometry();
    setFocus();

        // adjust canvas to the base magnification on parent size.
    setCanvasSize(parent_app->size());
}

// --------------------------------------------------------------------------------
// Destructor
// --------------------------------------------------------------------------------

QmvToolWindow::~QmvToolWindow()
{
  delete canvas;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QmvApplication * QmvToolWindow::parentApp() const
{
    return parent_app;
}

// --------------------------------------------------------------------------------
//! Initialize the display
/*!

  Initialize the display

  \param query  defines the general set.

*/
// --------------------------------------------------------------------------------
void QmvToolWindow::init( const QString & query )
{}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
const QSize QmvToolWindow::canvasSize() const
{
    return canvas->size();
}

// --------------------------------------------------------------------------------
//! Set the base query string
/*!

  \param query  
*/
// --------------------------------------------------------------------------------
void QmvToolWindow::setBaseQuery( const QString & query )
{
    base_query = query;
}

// --------------------------------------------------------------------------------
//! Get the base query string
/*!

  \param query  
*/
// --------------------------------------------------------------------------------
const QString QmvToolWindow::getBaseQuery() const
{
    return base_query;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvToolWindow::slotSetBannerText( const QString & text )
{

        // need to check banner, as it may have been deleted.

    if ( banner )
        banner->setText(text);
}

// --------------------------------------------------------------------------------
//! Set attribute of Navigation Actions.
/*!
  If an argument is not null, then set it, otherwise leave at current value.
  
  \param id     
  \param text   
  \param iset   
  \param mtext  
  \param accel  
*/
// --------------------------------------------------------------------------------
void QmvToolWindow::setNavigationAction( AGNavigation id, const QString & text, const QIconSet & iconset,
                                              const QString & mtext, int accel, bool enabled )
{
    QAction * ac = action_nav_idx.find( id );
    if ( ! ac )
        return;
    if ( text.length() )
        ac->setText( text );
    if ( !iconset.pixmap().isNull() )
        ac->setIconSet( iconset );
    if ( mtext.length() )
        ac->setMenuText( mtext );
    if ( accel )
        ac->setAccel( accel );
    ac->setEnabled( enabled );
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QToolBar *  QmvToolWindow::navigationToolBar()
{
    return toolbar_nav;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QToolBar *  QmvToolWindow::generalToolBar()
{
    return toolbar_gen;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QToolBar *  QmvToolWindow::otherToolBar()
{
    return toolbar_other;
}


// --------------------------------------------------------------------------------
//! Clear the canvasview
/*!
  Call this method to clear the canvas, rather than calling QmvCanvasView::clear()
  directly, as local pointers need to be cleared as well.

*/
// --------------------------------------------------------------------------------
void QmvToolWindow::clearCanvasView()
{
    canvasview->clear();
}

// --------------------------------------------------------------------------------
//! Resize the canvas
/*!

  Set the canvas size to current magnigication, based on mag=1 as viewport().
  
*/
// --------------------------------------------------------------------------------
void QmvToolWindow::setCanvasSize( QSize new_size )
{
        // resize whole window if requested and within desktop
    QSize sz;
    if ( new_size.isValid() ) {
        if (new_size.width() > QApplication::desktop()->width() )
            new_size.setWidth( QApplication::desktop()->width() );
        if (new_size.width() < QApplication::desktop()->height() )
            new_size.setHeight( QApplication::desktop()->height() );
        resize(new_size);
        setFixedSize(size());
        sz = size(); // debug
        updateGeometry();        // notify the layout manager
        sz = size();             // debug
        sz = canvasview->size(); //debug
    }
    
        // resize the canvas to 
    sz = canvasview->viewport()->size();
    canvas->resize( sz.width(), static_cast<int> ( sz.height() * mag_factor[mag_current]) );
    sz = canvas->size(); //debug
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvToolWindow::setTransparentColor( QColor & col )
{
    transparent = col;
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
const QColor & QmvToolWindow::getTransparentColor() const
{
    return transparent;
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QmvCanvasView * QmvToolWindow::canvasView() const
{
    return canvasview;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QCanvas * QmvToolWindow::getCanvas() const
{
    return canvas;
}


// --------------------------------------------------------------------------------
//! 
/*!

*/
// --------------------------------------------------------------------------------
void QmvToolWindow::buildBackgroundPixmap( )
{
    background.resize( canvas->size() );
    background.fill( transparent );
    QPainter p( &background );
}

// --------------------------------------------------------------------------------
// Navigation methods slots
// --------------------------------------------------------------------------------

void QmvToolWindow::slotBack4(){ }
void QmvToolWindow::slotBack3(){ }
void QmvToolWindow::slotBack2(){ }
void QmvToolWindow::slotBack1(){ }
void QmvToolWindow::slotHome(){ }
void QmvToolWindow::slotForward1(){ }
void QmvToolWindow::slotForward2(){ }
void QmvToolWindow::slotForward3(){ }
void QmvToolWindow::slotForward4(){ }
void QmvToolWindow::slotPrint(){ }
void QmvToolWindow::slotDisplay1(){ }
void QmvToolWindow::slotDisplay2(){ }
void QmvToolWindow::slotDisplay3(){ }

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvToolWindow::slotZoomIn()
{
    slotSetZoom( mag_current + 1);
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvToolWindow::slotZoomOut()
{
    slotSetZoom( mag_current - 1 );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvToolWindow::slotSetZoom( const int zoom )
{
    if (zoom == mag_current)
    {
        return;
    }
    
    setMagCurrent(zoom);
    setCanvasSize();
    init();
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvToolWindow::setMagFactor( const double min, const double max, const int steps )
{
    qDebug("setNagFactor::%f,%f,%d", min, max, steps);
    // some arbitary limits
    if (min < 1.0 || max > 4.0 || min >= max
        || steps < 1 || steps > 10 ) return;
    
    // set the array
    bool ok = mag_factor.resize(steps);
    double increment = (max - min) / (steps - 1);
    for ( int i = 0; i < steps; i++ ) {
        mag_factor[i] = min + ( i * increment );
    }
    mag_current = 0;
    for (int i = 0; i < steps; i++)
        qDebug("setMagFactor::%d, %f", i, mag_factor[i]);
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvToolWindow::setMagCurrent( const int zoom )
{
    if ( zoom < 0  )
        mag_current = 0;
    else if ( zoom > ( mag_factor.size() - 1) )
        mag_current = mag_factor.size() - 1;
    else
        mag_current = zoom;

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvToolWindow::slotFind()
{
        // for reimplementation
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvToolWindow::slotLineDown()
{
    canvasview->verticalScrollBar()->addLine();
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvToolWindow::slotLineUp()
{
    canvasview->verticalScrollBar()->subtractLine();
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvToolWindow::slotPageDown()
{
    canvasview->verticalScrollBar()->addPage();
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvToolWindow::slotPageUp()
{
    canvasview->verticalScrollBar()->subtractPage();
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvToolWindow::closeEvent( QCloseEvent* ce )
{
   
    ce->ignore();
    hide();
}
