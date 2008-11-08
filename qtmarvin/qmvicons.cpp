/*
 *     $Id: qmvicons.cpp 3214 2006-12-04 21:11:52Z source $
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
 * DESCRIPTION: Manage standard icons
 *
 * ======================================================================
 */

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */

#include <qpainter.h>
#include "qmvicons.h"



/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvicons_version[] = "$Id: qmvicons.cpp 3214 2006-12-04 21:11:52Z source $";

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

QmvIcons::QmvIcons( QObject * parent, const char * name )
        : QObject( parent, name )
{

#include "blankpage.xpm"
#include "book.xpm"
#include "close.xpm"    
#include "delete.xpm"
#include "export.xpm"
#include "filenew.xpm"
#include "import.xpm"
#include "left1arrow.xpm"
#include "left2arrow.xpm"
#include "newpage.xpm"
#include "print.xpm"
#include "fax.xpm"
#include "email.xpm"
#include "quit.xpm"
#include "right1arrow.xpm"
#include "right2arrow.xpm"
#include "textpage.xpm"
#include "whatsthis.xpm"
#include "home.xpm"

    pixmap_list.insert( BlankPageIcon,      new QPixmap( blankpage_xpm ) );
    pixmap_list.insert( CloseIcon,          new QPixmap( close_xpm ) );
    pixmap_list.insert( DeleteIcon,         new QPixmap( delete_xpm ) );
    pixmap_list.insert( ExportIcon,         new QPixmap( export_xpm ) );
    pixmap_list.insert( FilenewIcon,        new QPixmap( filenew_xpm ) );
    pixmap_list.insert( ImportIcon,         new QPixmap( import_xpm ) );
    pixmap_list.insert( Left1Icon,          new QPixmap( left1arrow_xpm ) );
    pixmap_list.insert( Left2Icon,          new QPixmap( left2arrow_xpm ) );
    pixmap_list.insert( NewPageIcon,        new QPixmap( newpage_xpm ) );
    pixmap_list.insert( PrintIcon,          new QPixmap( print_xpm ) );
    pixmap_list.insert( FaxIcon,            new QPixmap( fax_xpm ) );
    pixmap_list.insert( EmailIcon,          new QPixmap( email_xpm ) );
    pixmap_list.insert( QuitIcon,           new QPixmap( quit_xpm ) );
    pixmap_list.insert( Right1Icon,         new QPixmap( right1arrow_xpm ) );
    pixmap_list.insert( Right2Icon,         new QPixmap( right2arrow_xpm ) );
    pixmap_list.insert( TextPageIcon,       new QPixmap( textpage_xpm ) );
    pixmap_list.insert( HomeIcon,           new QPixmap( home_xpm ) );

        // Documentation pixmaps
    pixmap_list.insert( DocOverviewIcon,
                        makeTextPixmap( new QPixmap( blankpage_xpm ),
                                        tr("O"), QFont("Times"), QColor(darkGreen), QRect() ));
    pixmap_list.insert( DocFaqIcon,
                        makeTextPixmap( new QPixmap( blankpage_xpm ),
                                        tr("F"), QFont("Times"), QColor(darkGreen), QRect() ));
    pixmap_list.insert( DocUserIcon,
                        makeTextPixmap( new QPixmap( blankpage_xpm ),
                                        tr("U"), QFont("Times"), QColor(darkGreen), QRect() ));
    pixmap_list.insert( DocKeyBindingsIcon,
                        makeTextPixmap( new QPixmap( blankpage_xpm ),
                                        tr("K"), QFont("Times"), QColor(darkGreen), QRect() ));
    pixmap_list.insert( DocAdminIcon,
                        makeTextPixmap( new QPixmap( blankpage_xpm ),
                                        tr("A"), QFont("Times"), QColor(darkGreen), QRect() ));
    pixmap_list.insert( DocProgIcon,
                        makeTextPixmap( new QPixmap( blankpage_xpm ),
                                        tr("P"), QFont("Times"), QColor(darkGreen), QRect() ));
                        
    pixmap_list.insert( DocWhatIcon, new QPixmap( whatsthis_xpm ) );
    
}


// Destructor

QmvIcons::~QmvIcons()
{
    pixmap_list.setAutoDelete( TRUE );
    pixmap_list.clear();
}

const QIconSet QmvIcons::getIconSet( const StdIconSet pixmap ) const
{
    if ( pixmap_list.find( pixmap ) )
        return QIconSet( *pixmap_list[pixmap]);
    else
        return QIconSet();
}
const QPixmap QmvIcons::getPixmap( const StdIconSet pixmap ) const
{
    if ( pixmap_list.find( pixmap ) )
        return *pixmap_list[pixmap];
    else
        return QPixmap();
}

QPixmap * QmvIcons::makeTextPixmap( QPixmap * pm, const QString & text,
                                  QFont ft, const QColor cl, const QRect & rect )
{
        // if null text rect, then default to 16x16, at 2,2
    QRect text_rect = rect;
    if ( text_rect.isNull() )
        text_rect = QRect( QPoint(2,2), QSize(16,16) );

//     ft.setPixelSize( text_rect.height() - 4 );
//     ft.setWeight( QFont::Black );
    
    QPainter pt( pm );
    pt.setFont( ft );
    pt.setPen( cl );
    pt.drawText( text_rect, Qt::AlignCenter, text );

    return ( pm );
}

const QPixmap QmvIcons::makeTextButton( const QPushButton * pb, const QString & text, const QColor & cl  )
{

    int margin = 2;
    int padding = 2;

        // get reduced rect for pixmap
    QRect pr = pb->rect();
    pr.setRect( pr.left() - margin,
                pr.top() - margin,
                pr.width() - (2*margin),
                pr.height() - (2*margin) );


    QPixmap pm = QPixmap ( pr.size() );
    pm.fill( pb, pr.topLeft() );

    QFont ft = QFont("Helvetica");
    ft.setPixelSize( pr.height() - (3*padding) );
    
        // Start painting
    QPainter pt;
    pt.begin( &pm );
    pt.setFont( ft );
    pt.setPen( cl );
    QRect brt = pt.boundingRect( pr, AlignCenter, text, -1 );
    brt.moveCenter( pr.center() );
    pt.drawText( brt, AlignCenter, text );
    pt.end();
    
    return pm;
}
