/*
 *     $Id: qmvurltransfer.cpp 1179 2002-03-05 09:22:02Z source $
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
#include <unistd.h>
#include <qapplication.h>
#include <qtimer.h>
#include "qmvurltransfer.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmvurltransfer_version[] = "$Id: qmvurltransfer.cpp 1179 2002-03-05 09:22:02Z source $";
//#define QMVURLTRANSFER_DEBUG
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

QmvUrlTransfer::QmvUrlTransfer( const QString & url, QWidget *parent, const char *name )
        : QObject( parent, name )
{
#ifdef QMVURLTRANSFER_DEBUG
    qDebug("QmvUrlTransfer::QmvUrlTransfer");
#endif
    remote = url;
    op = new QUrlOperator( remote );
    connect( op, SIGNAL( start( QNetworkOperation * ) ),
             this, SLOT( loadStart( QNetworkOperation *) ) );
    connect( op, SIGNAL( finished( QNetworkOperation * ) ),
             this, SLOT( loadFinished( QNetworkOperation * ) ) );
    connect( op, SIGNAL( data ( const QByteArray &, QNetworkOperation * )  ),
             this, SLOT( data( const QByteArray &, QNetworkOperation * ) ) );
    status_detail = "";
}

// Destructor

QmvUrlTransfer::~QmvUrlTransfer()
{

#ifdef QMVURLTRANSFER_DEBUG
    qDebug("QmvUrlTransfer::~QmvUrlTransfer");
#endif    
  
}

// --------------------------------------------------------------------------------
const QByteArray & QmvUrlTransfer::load()
{

#ifdef QMVURLTRANSFER_DEBUG
    qDebug("QmvUrlTransfer::load");
#endif    

        // "http://localhost:9999/axis-cgi/jpg/image.cgi?resolution=320x240"
    dest_buffer.resize(0);
    op_status = true;
    op->get();
    if ( !op_status )
    {
        dest_buffer.resize(0);   
        return dest_buffer;
    }
    
        // Loop while the image loads
    image_progress = 0;
    QTimer timer(this);
    connect( &timer, SIGNAL(timeout()), SLOT(cancel()) );
    timer.start( 10000, TRUE );
    while( image_progress < 100 && op_status )
    {
        qApp->processEvents(10);
    }
        // timeout
    if ( image_progress < 100 )
    {
        op_status = false;
        dest_buffer.resize(0);   
        return dest_buffer;
    }
    
    timer.stop();
#ifdef QMVURLTRANSFER_DEBUG
    qDebug("QmvUrlTransfer::load - GET");
#endif    
    return dest_buffer;
}

// --------------------------------------------------------------------------------
void QmvUrlTransfer::loadStart( QNetworkOperation * res )
{
    if ( !res )
        return;

#ifdef QMVURLTRANSFER_DEBUG
    qDebug("QmvUrlTransfer::loadStart");
#endif    
    image_progress = 0;
}

// --------------------------------------------------------------------------------
void QmvUrlTransfer::loadProgress( int bytes_done, int bytes_total, QNetworkOperation * res )
{
    if ( !res )
        return;

#ifdef QMVURLTRANSFER_DEBUG
    qDebug("QmvUrlTransfer::loadProgress");
#endif    
    data_size = bytes_done;
        // this may be false - ?? multiple res objects??
    image_progress = (bytes_total*100)/bytes_done;
}

// --------------------------------------------------------------------------------
void QmvUrlTransfer::loadFinished( QNetworkOperation * res )
{
    if ( !res )
        return;
    
#ifdef QMVURLTRANSFER_DEBUG
    QNetworkProtocol::Operation o = res->operation();
    QNetworkProtocol::State s = res->state();
    int e = res->errorCode();
    QString pd = res->protocolDetail();
    qDebug( "QmvUrlTransfer::loadFinished(): state = %d, total=%d ", s, dest_buffer.size() );
#endif
    
    op_status = false;
    switch ( res->state() )
        {
            case QNetworkProtocol::StFailed:
            case QNetworkProtocol::StStopped:
                status_detail = res->protocolDetail();
                qWarning( "loadFinished:: %s", status_detail.latin1() );
                op_status = false;
                break;
            default:
                op_status = true;
                break;
        }
    image_progress = 100;
}

// --------------------------------------------------------------------------------
void QmvUrlTransfer::data( const QByteArray &data_buf, QNetworkOperation * res )
{
    if ( !res )
        return;
#ifdef QMVURLTRANSFER_DEBUG
    QNetworkProtocol::Operation o = res->operation();
    QNetworkProtocol::State s = res->state();
    QString pd = res->protocolDetail();
    qDebug( "QmvUrlTransfer::data(): %d new bytes", data_buf.size() );
#endif    
    
    if ( res->operation() == QNetworkProtocol::OpGet )
    {
        int size = dest_buffer.size();
        dest_buffer.resize( size + data_buf.size() );
        memcpy( dest_buffer.data() + size, data_buf.data(), data_buf.size() );
    }

}

// --------------------------------------------------------------------------------
const QString QmvUrlTransfer::statusDetail() const
{

#ifdef QMVURLTRANSFER_DEBUG
    qDebug("QmvUrlTransfer::statusDetail");
#endif    
    return status_detail;
}

// --------------------------------------------------------------------------------
void QmvUrlTransfer::cancel()
{
    op_status = false;
}

