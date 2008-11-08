/*
 *     $Id: qmvhelp.cpp 2889 2006-06-11 11:47:27Z source $
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
 * Contributions to this code include shameless borrowing from Qt's
 * Designer help module.
 *
 */

/*
 * ======================================================================
 * DESCRIPTION: QmvHelp - Help Browser
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <qprinter.h>
#include <qpaintdevicemetrics.h>
#include <qsimplerichtext.h> 
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>
#include <qtooltip.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qapplication.h>
#include <qpainter.h>
#include "qmvhelp.h"

namespace QmvHelpIcons
{
#include "left1arrow.xpm"
#include "right1arrow.xpm"
#include "left2arrow.xpm"
#include "right2arrow.xpm"
#include "print.xpm"
#include "quit.xpm"
} // end QmvHelpIcons

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvhelp_version[] = "$Id: qmvhelp.cpp 2889 2006-06-11 11:47:27Z source $";

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

QmvHelp::QmvHelp( const QString& home, QWidget * parent, const char *name )
        : QDialog( parent, name, FALSE, 0 )
{
    bookmarkMenu = 0;
    help_browser = new QTextBrowser( this );
    help_browser->mimeSourceFactory()->setFilePath( home );
    help_browser->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    connect( help_browser, SIGNAL( textChanged() ),
	     this, SLOT( textChanged() ) );

    setCaption( tr( "Help Viewer" ) );
    
    
        // ------------------------------------------------------------
        // Top level layout 
        // ------------------------------------------------------------
    QVBoxLayout * vl_top = new QVBoxLayout( this, 1 );

        // Controls - layout
    QHBoxLayout * hl_tools = new QHBoxLayout( vl_top, 0, "controls_help" );

    
        // Create control buttons ----------------------------

    QPushButton * pushb;
    QPixmap home_icon( QmvHelpIcons::left2arrow_xpm );
    pushb = new QPushButton( this, "first_page" );
    pushb->setPixmap( home_icon );
    hl_tools->addWidget( pushb );
    pushb->setDefault( FALSE );
    pushb->setFocusPolicy( QWidget::NoFocus );
    connect( pushb, SIGNAL( clicked() ), this, SLOT( slotFirstPage() ) );
    pushb->setAccel( Key_Home );
    QWhatsThis::add( pushb, "<b>First Page:</b>.<br>"
                     "Return to the first page visited");

    QPixmap left_icon( QmvHelpIcons::left1arrow_xpm );
    pushb = new QPushButton( this, "Previous Page" );
    pushb->setPixmap( left_icon );
    hl_tools->addWidget( pushb );
    pushb->setDefault( FALSE );
    pushb->setFocusPolicy( QWidget::NoFocus );
    connect( pushb, SIGNAL( clicked() ), this , SLOT( slotPrevPage() ) );
    pushb->setAccel( Key_Prior );
    QWhatsThis::add( pushb, "<b>Previous Page:</b>.<br>"
                     "Move backward to the previous page visited");


    QPixmap right_icon( QmvHelpIcons::right1arrow_xpm );
    pushb = new QPushButton( this, "next_page" );
    pushb->setPixmap( right_icon );
    hl_tools->addWidget( pushb );
    pushb->setDefault( FALSE );
    pushb->setFocusPolicy( QWidget::NoFocus );
    connect( pushb, SIGNAL( clicked() ), this, SLOT( slotNextPage() ) );
    pushb->setAccel( Key_Next );
    QWhatsThis::add( pushb, "<b>Next Page:</b>.<br>"
                     "Move forward to the next page visited");

    QPixmap last_icon( QmvHelpIcons::right2arrow_xpm );
    pushb = new QPushButton( this, "last_page" );
    pushb->setPixmap( last_icon );
    hl_tools->addWidget( pushb );
    pushb->setDefault( FALSE );
    pushb->setFocusPolicy( QWidget::NoFocus );
    connect( pushb, SIGNAL( clicked() ), this, SLOT( slotLastPage() ) );
    pushb->setAccel( Key_End );
    QWhatsThis::add( pushb, "<b>Last Page:</b>.<br>"
                     "Move forward to the last page");

    QPixmap print_icon (QmvHelpIcons::print_xpm);
    pushb = new QPushButton( this, "print_frm" );
    pushb->setPixmap( print_icon );
    hl_tools->addWidget( pushb );
    pushb->setDefault( FALSE );
    pushb->setFocusPolicy( QWidget::NoFocus );
    connect( pushb, SIGNAL( clicked() ), this, SLOT( slotPrintReport() ) );
    pushb->setAccel( Key_Print );
    QWhatsThis::add( pushb, "<b>Print this document:</b>. "
                     " Print the current help document.");

    hl_tools->addStretch();
    
    banner =  new QLabel("Help Documentation", this, "Help Toolbar Label");
    hl_tools->addWidget(banner);
    
    hl_tools->addStretch();

    QPixmap quit_icon (QmvHelpIcons::quit_xpm);
    pushb = new QPushButton( this, "quit" );
    pushb->setPixmap( quit_icon );
    hl_tools->addWidget( pushb );
    pushb->setDefault( FALSE );
    pushb->setFocusPolicy( QWidget::NoFocus );
    connect( pushb, SIGNAL( clicked() ), this, SLOT( hide() ) );
    pushb->setAccel( CTRL+Qt::Key_W );
    QWhatsThis::add( pushb, "<b>Close the viewer:</b>. "
                     " Close the help viewer.");
 

    vl_top->addWidget( help_browser );
    resize( 650, 500 );
    setMaximumSize( QApplication::desktop()->width(), QApplication::desktop()->height() );

}

// Destructor

QmvHelp::~QmvHelp()
{
  
}
void QmvHelp::setSource( const QString & doc )
{
    help_browser->setSource( doc );
    qDebug("QmvHelp::setSource source=%s", help_browser->source().latin1());
    
}


void QmvHelp::setupBookmarkMenu()
{}

void QmvHelp::slotFirstPage()
{
    help_browser->home();
    if ( page_total > 0 )
        page_current = 1;
    else
        page_current = 0;
//    slotSetBannerText( QString("Page %1 of %2 ").arg( page_current ).arg( page_total ) );
}
void QmvHelp::slotPrevPage()
{
    help_browser->backward();
    if ( page_current > 1 )
        page_current--;
//    slotSetBannerText( QString("Page %1 of %2 ").arg( page_current ).arg( page_total ) );
}
void QmvHelp::slotNextPage()
{
    help_browser->forward();
    if ( page_current < page_total )
        page_current++;
//    slotSetBannerText( QString("Page %1 of %2 ").arg( page_current ).arg( page_total ) );
}
void QmvHelp::slotLastPage()
{
    help_browser->forward();
    page_current = page_total;
//    slotSetBannerText( QString("Page %1 of %2 ").arg( page_current ).arg( page_total ) );
}
void QmvHelp::slotPrintReport()
{
    QPrinter printer;
    printer.setFullPage(TRUE);
    if ( printer.setup() ) {
	QPainter p( &printer );
	QPaintDeviceMetrics metrics(p.device());
	int dpix = metrics.logicalDpiX();
	int dpiy = metrics.logicalDpiY();
	const int margin = 72; // pt
	QRect body(margin*dpix/72, margin*dpiy/72,
		   metrics.width()-margin*dpix/72*2,
		   metrics.height()-margin*dpiy/72*2 );
	QFont font("times", 10);
	QSimpleRichText richText( help_browser->text(), font, help_browser->context(),
                                  help_browser->styleSheet(),
				  help_browser->mimeSourceFactory(), body.height() );
	richText.setWidth( &p, body.width() );
	QRect view( body );
	int page = 1;
	do {
	    richText.draw( &p, body.left(), body.top(), view, colorGroup() );
	    view.moveBy( 0, body.height() );
	    p.translate( 0 , -body.height() );
	    p.setFont( font );
	    p.drawText( view.right() - p.fontMetrics().width( QString::number(page) ),
			view.bottom() + p.fontMetrics().ascent() + 5, QString::number(page) );
	    if ( view.top()  >= richText.height() )
		break;
	    printer.newPage();
	    page++;
	} while (TRUE);
    }
}

//--------------------------------------------------------------------------------
void QmvHelp::setPrinterName( const QString & name )
{
        //formViewer()->setPrinterName( name );
}

// --------------------------------------------------------------------------------
void QmvHelp::slotSetBannerText( const QString & text )
{
    banner->setText( QString( "%1" ).arg( text, -60 ) );
}


void QmvHelp::textChanged()
{
    if ( help_browser->documentTitle().isNull() )
        setCaption( tr( "Help: " ) + help_browser->context() );
    else
        setCaption( tr( "Help: " ) + help_browser->documentTitle() );
}

void QmvHelp::goHome()
{
    if ( !first_link.isNull() && !first_link_title.isNull() )
        showLink( firstLink(), firstLinkTitle() );
}

void QmvHelp::showLink( const QString & link, const QString & title )
{
    help_browser->setCaption( title );
    help_browser->setSource( link );

    if ( first_link.isNull() )
        first_link = link;
    if ( first_link_title.isNull() )
        first_link_title = title;
}

void QmvHelp::showBookmark( int id )
{
}

void QmvHelp::setupFileActions()
{}

void QmvHelp::setupGoActions()
{}


