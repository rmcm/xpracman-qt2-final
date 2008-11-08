/*
 *     $Id: qmvimage.h 1897 2003-10-03 07:42:46Z source $
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

#ifndef QMVIMAGE_H
#define QMVIMAGE_H

/*
 * ======================================================================
 * DESCRIPTION: Image Display
 *
 * ======================================================================
 */

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <qimage.h>
#include "qmvurltransfer.h"
/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvImage : public QImage
{
    
      public:
    QmvImage();    // Constructor
    QmvImage( const QSize & size, int depth, int numColors = 0 );    // Constructor
    QmvImage( const QmvImage & image );    // Constructor
    ~QmvImage();  // Destructor
    bool loadFromURL( const QString & url, const char * format="JPEG" );
         
      private:
    QmvUrlTransfer * getimg;
    QString url_current;
          
}; /* CLASS QmvImage */

#endif /* QMVIMAGE_H */
// Local Variables:
// mode:C++
// End:
