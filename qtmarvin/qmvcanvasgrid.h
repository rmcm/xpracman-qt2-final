/*
 *     $Id: qmvcanvasgrid.h 1896 2003-10-03 07:41:09Z source $
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

#ifndef QMVCANVASGRID_H
#define QMVCANVASGRID_H

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
#include <qimage.h>
#include <qcanvas.h>

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvCanvasGrid : public QCanvasRectangle
{
      public:
    QmvCanvasGrid( QCanvas * c, QSize sz, QColor tp );    // Constructor
    ~QmvCanvasGrid();  // Destructor
    void clearGrid();
    void setOpaqueFactor( int opf );
    void setTransparentColor( QColor tp );
    const int opaqueFactor() const;
    const QColor transparentColor() const;
    void fill( const QRgb fc );
    void drawText( const QPointArray & pa,
                   const QPen & pen, const QFont font, QString text, int w, int h );
    void drawText( const QRect & rect,
                   const QPen & pen, const QFont font, QString text );
    void drawPixmap( const QPoint & origin, const QPixmap & pixmap, const QRect & bound );
    void drawEllipses( const QPointArray & pa, int w, int h, const QPen & pen );
    void setUpdateMode( bool mode = true );
    void updateGridImage( bool resetmode = false );
    void updateGridImage( const QPixmap & pm, bool usemask=FALSE );
    void save( const QString & fname ) const;
      protected:
    void drawShape( QPainter & );
    bool hit( const QPoint & pt ) const;
          
      private:
    int image_depth;
    bool update_mode;           //!< update mode to cache updates via PM<->Image transfers.
    QImage work_grid;           //!< The grid image
    QImage img_grid_tmp;        //!< a temp image for conversions
    QPixmap work_pixmap;        //!< pixmap for drawing on grid
    QColor transparent;         //!< The colour used for the transparancy mask.
    int opaque_factor;          //!< opaqueness of foreground ( 0x0->0xff )
          
}; /* CLASS QmvCanvasGrid */

#endif /* QMVCANVASGRID_H */
// Local Variables:
// mode:C++
// End:
