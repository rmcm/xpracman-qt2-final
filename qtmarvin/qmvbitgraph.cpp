/*
 *     $Id: qmvbitgraph.cpp 1907 2003-10-13 01:06:10Z source $
 * 
 *   Copyright 2003 X=X Computer Software Trust
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

/*!
  \file   qmvbitgraph.cpp
  \brief  A 2D display graph of a bitmap

  A 2D display of a bitmap, with setable colours for each bit.

  \author "Rex McMaster"
  \date   2003-10-02
*/


/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <math.h>
#include <qpainter.h>
#include "qmvbitgraph.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmvbitgraph_version[] = "$Id: qmvbitgraph.cpp 1907 2003-10-13 01:06:10Z source $";
// #define QMVBITGRAPH_DEBUG

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

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QmvBitGraph::QmvBitGraph( const uint count, QObject *parent, const char *name )
{
    bool status = p_bit_map.resize(count);
    int size = p_bit_map.size();
}

// Destructor

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QmvBitGraph::~QmvBitGraph()
{
    p_colour_map.setAutoDelete(true);
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvBitGraph::slotSetBit( const uint pos, const QColor & colour )
{
    int count = p_bit_map.size();
    
    if ( pos >= p_bit_map.size() )
        p_bit_map.resize(pos-1);

    p_bit_map.setBit( pos, colour.isValid() ? false : true );
    QColor * c = p_colour_map.find(pos);
    if (!c)
        p_colour_map.insert(pos, new QColor(colour) );
    else
        *c = colour;
}

// --------------------------------------------------------------------------------
//! Prepare a pixmap from the bitmap
/*!
  Prepare a \ref QPixmap from the internal bitmap using the supplied parameters.

  \param cell           the size of the display cell in pixels
  \param layout         the layout of the grid cells - rows by columns
  \param bg             the background colour
  \param fr             the frame colour
  \param border         the border size in pixels, defaults to 1 pixel
  \param spacing        the cell-spacing - defaults to 1 pixel
  \return <ReturnValue> the resulting pixmap.
*/
// --------------------------------------------------------------------------------
QPixmap QmvBitGraph::pixmap( const QSize cell, const QSize layout, const QColor bg,
                             const QColor fr, const int border, const int spacing ) const
{
        // set the display cell size
    QPixmap pm_cell(1, 1); // default to 1x1
    if ( !cell.isEmpty() )
        pm_cell.resize(cell);
    pm_cell.fill(bg);
    return pixmap( pm_cell, true, layout, bg, fr, border, spacing );
}

// --------------------------------------------------------------------------------
//! Prepare a pixmap from the bitmap
/*!
  Prepare a \ref QPixmap from the internal bitmap using the supplied parameters.

  \param cell           the pixmap to display
  \param fill           fill the pixmap with the bitmap colour (default = TRUE )
  \param layout         the layout of the grid cells - rows by columns
  \param bg             the background colour
  \param fr             the frame colour
  \param border         the border size in pixels, defaults to 1 pixel
  \param spacing        the cell-spacing - defaults to 1 pixel
  \return <ReturnValue> the resulting pixmap.
*/
// --------------------------------------------------------------------------------
QPixmap QmvBitGraph::pixmap( QPixmap cell, bool fill, const QSize layout, const QColor bg,
                             const QColor fr, const int border, const int spacing ) const
{
        // determine the default size
        // - square, extending width if necessary.
    int cols = static_cast<int> (sqrt(p_bit_map.size()));
    int rows = cols;
    while ( (cols*rows) < p_bit_map.size() )
        cols++;

        // either/both height or width of layout() may be 0 (ie unspecified)
    if ( layout.width() > 0 )
    {
        cols = layout.width();
        while ( (cols*rows) < p_bit_map.size() )
            rows++;
    }
    if ( layout.height() > 0 )
    {
        rows = layout.height();    
        while ( (cols*rows) < p_bit_map.size() )
            cols++;
    }

    if (cols <= 0 || rows <= 0)
        return QPixmap();
    
        // Create a pixmap and fill it with background colour
    QPixmap pm( ((cell.width() + spacing)*cols) - spacing + (border*2),
                ((cell.height() + spacing)*rows) - spacing + (border*2) );
    pm.fill(fr); // the frame shows through
    
    QPainter pt;
    pt.begin(&pm);

    QPen pen( Qt::gray, 2, Qt::SolidLine );
    QFont font("helvetica", 8);    
    pt.setPen( pen );
    pt.setFont( font );
    
        // we want to scan the TopLeft points of cells
    int w = cell.width();    // cell width (less border and spacing)
    int h = cell.height();   // cell height (less border and spacing)
    int x, y;
    for ( int i = 0; i < p_bit_map.size(); i++ )
    {
        if ( fill )
        {
            QColor * colour = p_colour_map.find(i);
            if (!colour )
                continue;       // leave background exposed.
            cell.fill((colour->isValid()) ? *colour : bg );
        }

        x = (i % cols)*(w + spacing) + border;
        y = (i / cols)*(h + spacing) + border;
        pt.drawPixmap ( x, y, cell);
        
    }
        //pm.save("bigraph.xpm", "XPM");
    pt.end();
    
    return pm;
}

