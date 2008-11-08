/*
 *     $Id: qmvpopup.cpp 1837 2003-08-30 09:24:24Z source $
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
 * DESCRIPTION: Transient Popup Window
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <qpainter.h>
#include "qmvpopup.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmvpopup_version[] = "$Id: qmvpopup.cpp 1837 2003-08-30 09:24:24Z source $";
// #define QMVPOPUP_DEBUG

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

QmvPopup::QmvPopup ( const QString& contents, const QPoint& pos, QWidget * parent, const char * name )
        : QWidget( parent, name, WType_Popup | WDestructiveClose )
{
    
    const int shadowWidth = 6;   // also used as '5' and '6' and even '8' below
    const int vMargin = 8;
    const int hMargin = 12;

    setBackgroundMode( QWidget::NoBackground );
    QSimpleRichText* doc = new QSimpleRichText( contents, font() );
  
    doc->adjustSize();
    QRect r( 0, 0, doc->widthUsed(), doc->height() );
    int w = r.width() + 2*hMargin;
    int h = r.height() + 2*vMargin;
 
    resize( w + shadowWidth, h + shadowWidth ); 

    move( pos - rect().center());
     if (geometry().right() > QApplication::desktop()->width())
         move( QApplication::desktop()->width() - width(),
               y() );
     if (geometry().bottom() > QApplication::desktop()->height())
         move( x(),
               QApplication::desktop()->height() - height() );
     if ( x() < 0 )
         move( 0, y() );
     if ( y() < 0 )
         move( x(), 0 );
    show();

    QPainter p( this );
    p.setPen( QApplication::palette().normal().foreground() );
    p.drawRect( 0, 0, w, h );
    p.setPen( QApplication::palette().normal().mid() );
    p.setBrush( QColor( 255, 255, 240 ) );
    p.drawRect( 1, 1, w-2, h-2 );
    p.setPen( black );
    doc->draw( &p, hMargin, vMargin, r, colorGroup(), 0 );
    delete doc;
    p.drawPoint( w + 5, 6 );
    p.drawLine( w + 3, 6,
                w + 5, 8 );
    p.drawLine( w + 1, 6,
                w + 5, 10 );
    int i;
    for( i=7; i < h; i += 2 )
        p.drawLine( w, i,
                    w + 5, i + 5 );
    for( i = w - i + h; i > 6; i -= 2 )
        p.drawLine( i, h,
                    i + 5, h + 5 );
    for( ; i > 0 ; i -= 2 )
        p.drawLine( 6, h + 6 - i,
                    i + 5, h + 5 );

}

// --------------------------------------------------------------------------------
// Destructor
// --------------------------------------------------------------------------------
QmvPopup::~QmvPopup()
{
  
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvPopup::mousePressEvent( QMouseEvent* )
{
        //close();
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvPopup::mouseReleaseEvent( QMouseEvent* )
{
    close();
}
