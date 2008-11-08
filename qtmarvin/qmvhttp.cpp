/*
 *     $Id: qmvhttp.cpp 1121 2002-02-07 05:36:18Z source $
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
 * DESCRIPTION: 
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include "qmvhttp.h"
#include "qurlinfo.h"
#include <stdlib.h>

#include <qstringlist.h>
#include <qregexp.h>

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmvhttp_version[] = "$Id: qmvhttp.cpp 1121 2002-02-07 05:36:18Z source $";
//#define QMVHTTP_DEBUG
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

QmvHttp::QmvHttp()
    : QNetworkProtocol(), connectionReady( FALSE )
{
#ifdef QMVHTTP_DEBUG
    qDebug("QmvHttp::QmvHttp");
#endif
    commandSocket = new QSocket( this );

    connect( commandSocket, SIGNAL( hostFound() ),
	     this, SLOT( hostFound() ) );
    connect( commandSocket, SIGNAL( connected() ),
	     this, SLOT( connected() ) );
    connect( commandSocket, SIGNAL( connectionClosed() ),
	     this, SLOT( closed() ) );
    connect( commandSocket, SIGNAL( readyRead() ),
	     this, SLOT( readyRead() ) );
}

// Destructor

QmvHttp::~QmvHttp()
{ 
#ifdef QMVHTTP_DEBUG
    qDebug("QmvHttp::~QmvHttp");
#endif
    close();
    delete commandSocket;
}

void QmvHttp::operationPut( QNetworkOperation * op )
{
#ifdef QMVHTTP_DEBUG
    qDebug("QmvHttp::operationPut");
#endif
    QString cmd = "POST ";
    cmd += url()->encodedPathAndQuery();
    cmd += "\r\n";
    commandSocket->writeBlock( cmd.latin1(), cmd.length() );
}

void QmvHttp::operationGet( QNetworkOperation * op )
{
#ifdef QMVHTTP_DEBUG
    qDebug("QmvHttp::operationGet");
#endif
    QString cmd = QString( "GET %1 HTTP/1.1\r\n").arg(url()->encodedPathAndQuery());
    commandSocket->writeBlock( cmd.latin1(), cmd.length() );
    cmd = QString( "Host: %1:%2\r\n" )
        .arg(url()->host())
        .arg(url()->port() != -1 ? url()->port() : 80);
    commandSocket->writeBlock( cmd.latin1(), cmd.length() );
    cmd = QString( "Accept: */*\r\n" );
    commandSocket->writeBlock( cmd.latin1(), cmd.length() );
//      cmd = QString( "Connection: close\r\n" );
//      commandSocket->writeBlock( cmd.latin1(), cmd.length() );
    cmd = QString( "User-Agent: qmvhttp\r\n" );
    commandSocket->writeBlock( cmd.latin1(), cmd.length() );
    cmd = "\r\n";
    commandSocket->writeBlock( cmd.latin1(), cmd.length() );
        // reset the HTTP response buffer
    bytes_in = 0;
    response_buffer.resize(0);
}

bool QmvHttp::checkConnection( QNetworkOperation * op )
{
#ifdef QMVHTTP_DEBUG
    qDebug("QmvHttp::checkConnection for %s", url()->toString().latin1() );
#endif

    QSocket::State status = commandSocket->state();
    switch (status)
    {
            // Connection is open.--------------------------------------------
        case QSocket::Connection:
            if ( connectionReady )
            {
#ifdef QMVHTTP_DEBUG
                qDebug( "QmvHttp: connection ok!" );
#endif
                return TRUE;
            } else {
#ifdef QMVHTTP_DEBUG
                qDebug( "QmvHttp: connected but HTTP layer not ready" );
#endif
            }
            break;
            // Connection is open.--------------------------------------------
        case QSocket::HostLookup:
#ifdef QMVHTTP_DEBUG
            qDebug( "QmvHttp: Hostlookup in progress" );
#endif
            break;
            // Connection is open.--------------------------------------------
        case QSocket::Connecting:
#ifdef QMVHTTP_DEBUG
            qDebug( "QmvHttp: connection in progress" );
#endif
            break;
            // Connection is open.--------------------------------------------
        case QSocket::Idle:
#ifdef QMVHTTP_DEBUG
            qDebug( "QmvHttp: disconnected - will retry" );
#endif
            connectionReady = FALSE;
            commandSocket->connectToHost( url()->host(),
                                          url()->port() != -1 ? url()->port() : 80 );

            break;
        default:
#ifdef QMVHTTP_DEBUG
            qDebug( "QmvHttp: status (%d) unknown", status );
#endif
            break;
    }

    return FALSE;
}


void QmvHttp::close()
{
#ifdef QMVHTTP_DEBUG
    qDebug("QmvHttp::close");
#endif
    qDebug("QmvHttp::close");
    if ( !commandSocket->peerName().isEmpty() ) {
 	commandSocket->writeBlock( "quit\r\n", strlen( "quit\r\n" ) );
 	commandSocket->close();
    }
}

int QmvHttp::supportedOperations() const
{
#ifdef QMVHTTP_DEBUG
    qDebug("QmvHttp::supportedOperations");
#endif
    return OpGet | OpPut;
}

void QmvHttp::hostFound()
{
#ifdef QMVHTTP_DEBUG
    qDebug("QmvHttp::hostFound");
#endif
    if ( url() )
	emit connectionStateChanged( ConHostFound, tr( "Host %1 found" ).arg( url()->host() ) );
    else
	emit connectionStateChanged( ConHostFound, tr( "Host found" ) );
}

void QmvHttp::connected()
{
#ifdef QMVHTTP_DEBUG
    qDebug("QmvHttp::connected");
#endif
    if ( url() )
	emit connectionStateChanged( ConConnected, tr( "Connected to host %1" ).arg( url()->host() ) );
    else
	emit connectionStateChanged( ConConnected, tr( "Connected to host" ) );
    connectionReady = TRUE;
}

void QmvHttp::closed()
{
#ifdef QMVHTTP_DEBUG
    qDebug("QmvHttp::closed");
#endif
    if ( url() )
	emit connectionStateChanged( ConClosed, tr( "Connection to %1 closed" ).arg( url()->host() ) );
    else
	emit connectionStateChanged( ConClosed, tr( "Connection closed" ) );

    connectionReady = FALSE;
    emit finished( operationInProgress() );

}

void QmvHttp::readyRead()
{
#ifdef QMVHTTP_DEBUG
    qDebug("QmvHttp::readyRead");
#endif
    QByteArray s;
    s.resize( commandSocket->bytesAvailable() );
    commandSocket->readBlock( s.data(), commandSocket->bytesAvailable() );
        // still in response header
    if ( bytes_in == 0 && processGetResponse( &s ) < 0 )
            return;
    emit data( s, operationInProgress() );
    bytes_in += s.size();
    if ( content_length >= 0 && bytes_in == content_length )
    {
            // reset for next response
        bytes_in = 0;
        emit finished( operationInProgress() );
    }
    
}
// --------------------------------------------------------------------------------
//! Process the server response
/*!

  Parse the response headers. Returns -1 if an error, or the number of
  bytes remaining in \em s after the response headers. Resizes \em s
  to exclude the response header component. Sets \em content_length to
  value from \em Content-Length, or -1 if not found.

  \param s              Pointer to data containing response headers
  \return -1 if error else number of unprocessed bytes */

int QmvHttp::processGetResponse( QByteArray * s )
{
#ifdef QMVHTTP_DEBUG
    qDebug("QmvHttp::processGetResponse");
#endif
        // convert to QString to look for end-of-header
    QString tmp_qstring(*s);
    QRegExp rx_eoh("[\\r]*\\n[\\r]*\\n");
    int rx_len;
        // this will have to be ported to QT3 - the length attribute is
        // incorporated into .find() ??
    int eoh = rx_eoh.match( tmp_qstring, 0, &rx_len );

    content_length = -1;
    int datasize;
    if ( eoh >= 0)
    {// found the start of data
            // parse the headers by converting to QString
        QString header = tmp_qstring.left(eoh);
        QStringList response_list = QStringList().split( QRegExp("[\\r]*\\n"), header);
        for ( QStringList::Iterator it = response_list.begin(); it != response_list.end(); ++it )
        {
            QStringList response = QStringList().split( QRegExp("\\s*:\\s*"), (*it) );
            if ( response[0] == "Content-Length" )
            {
                content_length = response[1].toInt();
#ifdef QMVHTTP_DEBUG
                qDebug("QmvHttp::processGetResponse - content_length = %d", content_length);
#endif
                continue;
            }
        }
        
        eoh += rx_len; // eol and blank line
        datasize = s->size() - eoh;
            // remove the response header from the input buffer
        QByteArray tmp = s->copy(); // deep copy
        int offset = tmp.size() - datasize;
        memcpy( s->data(), tmp.data() + offset, datasize);
        s->resize( datasize );
        return datasize;
    
    } else if ( tmp_qstring.find( QRegExp("[\\r]*\\n") ) ) {
            // incomplete HTTP response header, so append to response buffer
        int size = response_buffer.size();
        response_buffer.resize( size + s->size() );
        memcpy( response_buffer.data() + size, s->data(), s->size() );
        return(0);
    } else {
            // not a response header - so treat as data, from non-persistent connection
        return(s->size());
    }

        // should not get to here.
    return(-1);
}
    
