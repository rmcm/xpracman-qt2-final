/*
 *     $Id: qmvbitgraph.h 1907 2003-10-13 01:06:10Z source $
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

#ifndef QMVBITGRAPH_H
#define QMVBITGRAPH_H

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
#include <qobject.h>
#include <qbitarray.h>
#include <qintdict.h>
#include <qcolor.h>
#include <qpixmap.h>

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvBitGraph : public QObject
{
    Q_OBJECT

      public:
    QmvBitGraph( uint count, QObject *parent = 0, const char *name = 0 );    // Constructor
    ~QmvBitGraph();  // Destructor

    QPixmap pixmap( const QSize cell = QSize(1,1), const QSize layout = QSize(0,0), const QColor bg = QColor(),
                    const QColor fr = QColor(), const int border = 0, const int spacing = 0 ) const;
    QPixmap pixmap( QPixmap cell, bool fill = true, const QSize layout = QSize(0,0), const QColor bg = QColor(),
                    const QColor fr = QColor(), const int border = 0, const int spacing = 0 ) const;
                 
    public slots:
    void slotSetBit( const uint pos, const QColor & colour );

      signals:
    void changed( const uint pos );
    
      private:
    QBitArray p_bit_map;                //!< the core bitarray
    QIntDict<QColor> p_colour_map;      //!< colour map
//     QSize p_cell;                       //!< the cell size (pixels)
//     int p_border;                       //!< border thickness (pixels)
//     int p_spacing;                      //!< cell spacing (pixels)
//     QSize p_layout;                     //!< if either height or width = 0, then no-preference.
          
}; /* CLASS QmvBitGraph */

#endif /* QMVBITGRAPH_H */
// Local Variables:
// mode:C++
// End:
