/*
 *     $Id: qmvcanvasgrid.cpp 2889 2006-06-11 11:47:27Z source $
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
 * DESCRIPTION: A canvas item which presents a grid
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <qpainter.h>
#include "qmvcanvasgrid.h"

//#define QMVCANVASGRID_DEBUG

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmvcanvasgrid_version[] = "$Id: qmvcanvasgrid.cpp 2889 2006-06-11 11:47:27Z source $";

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

QmvCanvasGrid::QmvCanvasGrid( QCanvas * c, QSize sz, QColor tp )
        : QCanvasRectangle( c ),
          transparent(tp), opaque_factor(0xff), image_depth(8)
{

    setSize( sz.width(), sz.height() );
    work_grid.create( size(), image_depth );
    work_pixmap.resize( size() );
    work_pixmap.fill( transparent );
    updateGridImage( work_pixmap );
    update_mode = true;
}

// Destructor

QmvCanvasGrid::~QmvCanvasGrid()
{
  
}

// --------------------------------------------------------------------------------
void QmvCanvasGrid::clearGrid()
{
    QPixmap pm( size() );
    pm.fill( transparent );
    updateGridImage( pm );
}

// --------------------------------------------------------------------------------
void QmvCanvasGrid::setOpaqueFactor( int opf )
{
    opaque_factor = opf & 0xff;
}

// --------------------------------------------------------------------------------
void QmvCanvasGrid::setTransparentColor( QColor tp )
{
    transparent = tp;
}

// --------------------------------------------------------------------------------
const int QmvCanvasGrid::opaqueFactor() const
{
    return opaque_factor;
}

// --------------------------------------------------------------------------------
const QColor QmvCanvasGrid::transparentColor() const
{
    return transparent;
}

// --------------------------------------------------------------------------------
void QmvCanvasGrid::fill( const QRgb fc )
{
    work_pixmap.fill( fc );
    updateGridImage( work_pixmap );
}

// --------------------------------------------------------------------------------
void QmvCanvasGrid::drawText( const QPointArray & pa,
                              const QPen & pen, const QFont font, QString text, int w, int h )
{

    QPainter pt;
    pt.begin(&work_pixmap);
    
    pt.setPen( pen );
    pt.setFont( font );
    
        // we want the centre at pa.point
    int x, y;
    for ( int i = 0; i < pa.count(); i++ )
    {
        x = pa.point(i).x() - w/2;
        y = pa.point(i).y() - h/2;
        pt.drawText( x, y, w, h,
                     AlignHCenter | AlignVCenter,
                     text, text.length() );
    }
    pt.end();
    
    updateGridImage( work_pixmap );
}

// --------------------------------------------------------------------------------
void QmvCanvasGrid::drawText( const QRect & rect,
                              const QPen & pen, const QFont font, QString text )
{
    QPainter pt;
    pt.begin(&work_pixmap);
    pt.setPen( pen );
    pt.setFont( font );
    
    pt.drawText( rect,
                 WordBreak|AlignCenter,
                 text );
    pt.end();
    
    updateGridImage( work_pixmap );
}

// --------------------------------------------------------------------------------
void QmvCanvasGrid::drawPixmap( const QPoint & origin, const QPixmap & pixmap, const QRect & bound )
{
    QPainter pt;
    pt.begin(&work_pixmap);
    pt.drawPixmap ( origin, pixmap, bound );
    pt.end();
    
    updateGridImage( work_pixmap );
}

// --------------------------------------------------------------------------------
void QmvCanvasGrid::drawEllipses( const QPointArray & pa,
                                  int w, int h, const QPen & pen )
{

    QPainter pt;
    pt.begin(&work_pixmap);
    pt.setPen( pen );
    
        // we want the centre at pa.point
    for ( int i = 0; i < pa.count(); i++ )
        pt.drawEllipse( pa.point(i).x() - w/2,
                        pa.point(i).y() - h/2, w, h );
    pt.end();
    
    updateGridImage( work_pixmap );
        
}

// --------------------------------------------------------------------------------
//! Set the update mode
/*!
  Flag to determine if the QImage \ref work_grid is updated from the QPixmap
  \ref work_pixmap when updateGridImage( const QPixmap & pm, bool usemask ) is called.

  \param mode   
*/
// --------------------------------------------------------------------------------
void QmvCanvasGrid::setUpdateMode( bool mode )
{
    update_mode = mode;
}


// --------------------------------------------------------------------------------
//! Update the internal image cache
/*!
  Alternative update method, which forces an update even if \ref update_mode is false.
  If \i resetmode is true, then update_mode is reinstated.

  \param resetmode      TRUE=set update_mode to TRUE
*/
// --------------------------------------------------------------------------------
void QmvCanvasGrid::updateGridImage( bool resetmode )
{
        // remember the current update mode and then set updates
    bool hold_current = update_mode;
    update_mode = true;
    updateGridImage( work_pixmap );

        // reinstate the update mode, or ??
    if ( resetmode )
        update_mode = true;
    else
        update_mode = hold_current;
}


// --------------------------------------------------------------------------------
//! Update the internal image cache
/*!

  If \ref update_mode is set, then update the internal \ref QImage and
  \QPixmap objects from \em pm. This method also sets the transparency
  mask using the current transparent color ( \ref transparentColor()
  ). This method is call by \ref clearGrid(), \ref fill(), \ref
  drawText(), and \ref drawEllipses(). It can also be called directly.

  \param pm */
// --------------------------------------------------------------------------------
void QmvCanvasGrid::updateGridImage( const QPixmap & pm, bool usemask )
{
    if ( !update_mode )
    {
        work_pixmap = pm;
        return;
    }
    
        // Prepare a copy of grid image
    img_grid_tmp = pm.convertToImage();
    if ( img_grid_tmp.isNull() )        // failed
        return;
    bool alpha = img_grid_tmp.hasAlphaBuffer();
        // convert to 8bit depth - not many colors required, and it is
        // easier to operate on a color table.
        // The pixmap cannot be used directly as it's depth defaults to video display depth.

    if ( img_grid_tmp.depth() != image_depth )
        work_grid = img_grid_tmp.convertDepth( image_depth,
                                               ThresholdDither || ThresholdAlphaDither || AvoidDither);
    else
        work_grid = img_grid_tmp;
    
        // Prepare the image - create mask.
    if ( !usemask )
    {
        work_grid.setAlphaBuffer( TRUE );
    
            // Walk the colour table.
        for ( int i = 0; i < work_grid.numColors(); i++ )
            if ( QColor( work_grid.color( i ) ) == transparent )
                work_grid.setColor( i, work_grid.color(i) & 0x00ffffff );
            else
                work_grid.setColor( i, work_grid.color(i) | opaque_factor * 0x1000000 );
    }
    
    
        // Transfer back to the main pixmap, which is then used in reimplemented drawShape()
    work_pixmap.convertFromImage( work_grid, QPixmap::Color );
}


// reimplemented
// --------------------------------------------------------------------------------
void QmvCanvasGrid::drawShape( QPainter & p )
{
//    p.drawImage( int( x() ), int( y() ), work_grid, 0, 0, -1, -1, OrderedAlphaDither );
    p.drawPixmap( int( x() ), int( y() ), work_pixmap );
}

// --------------------------------------------------------------------------------
bool QmvCanvasGrid::hit( const QPoint & pt ) const
{
    int ix = pt.x() - int( x() );
    int iy = pt.y() - int (y() );
    if ( !work_grid.valid( ix , iy ) )
	return FALSE;
    QRgb pixel = work_grid.pixel( ix, iy );
    return qAlpha( pixel ) != 0;
}

// Debugging - save to jpeg
// --------------------------------------------------------------------------------
void QmvCanvasGrid::save( const QString & fname ) const
{
    
#ifdef QMVCANVASGRID_DEBUG
    grid.save( QString( "%1.xpm" ).arg( fname ), "XPM" );
#endif
    
}
