/*
 *     $Id: qmvimage.cpp 1898 2003-10-03 07:47:07Z source $
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
 * DESCRIPTION: Image Handler - capture,display and processing
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <iostream.h>
#include "qmvimage.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmvimage_version[] = "$Id: qmvimage.cpp 1898 2003-10-03 07:47:07Z source $";
//#define QMVIMAGE_DEBUG
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

QmvImage::QmvImage()
        : QImage()
{
#ifdef QMVIMAGE_DEBUG
    qDebug("QmvImage::QmvImage()");
#endif
    getimg = 0;
}

QmvImage::QmvImage( const QSize & size, int depth, int numColors )
        : QImage( size, depth, numColors )
{
#ifdef QMVIMAGE_DEBUG
    qDebug("QmvImage::QmvImage(%dx%d,%d,%d)",size.width(),size.height(),depth,numColors);
#endif
    getimg = 0;
}

QmvImage::QmvImage( const QmvImage & image )
        : QImage( image )
{
#ifdef QMVIMAGE_DEBUG
    qDebug("QmvImage::QmvImage(copy)");
#endif
    getimg = 0;
}

// Destructor

QmvImage::~QmvImage()
{
#ifdef QMVIMAGE_DEBUG
    qDebug("QmvImage::~QmvImage");
#endif 
}

// --------------------------------------------------------------------------------
bool QmvImage::loadFromURL( const QString & url, const char * format )
{
#ifdef QMVIMAGE_DEBUG
    qDebug("QmvImage::loadFromURL");
#endif
    if ( !url.length() )
        return false;

    if ( url != url_current || !getimg )
    {
        if (getimg)
            delete getimg;
        getimg = new QmvUrlTransfer( url );
        url_current = url;
    }
    
    QByteArray ba = getimg->load();
    if ( !ba.size() )
        return false;
    
    bool status = loadFromData( ba, format );
    return status;
    
    
}
