/*
 *     $Id: qmvhttp.h 1113 2002-02-05 12:18:09Z source $
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

#ifndef QMVHTTP_H
#define QMVHTTP_H

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
#include <qsocket.h>
#include <qapplication.h>
#include <qstring.h>
#include <qurlinfo.h>
#include <qnetworkprotocol.h>
#include <qurloperator.h>

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvHttp : public QNetworkProtocol
{
    Q_OBJECT

      public:
    QmvHttp();    // Constructor
    virtual ~QmvHttp();  // Destructor
    virtual int supportedOperations() const;

      protected:
    virtual void operationGet( QNetworkOperation *op );
    virtual void operationPut( QNetworkOperation *op );

    QSocket *commandSocket;
    bool connectionReady, passiveMode;

          
      private:
    bool checkConnection( QNetworkOperation *op );
    void close();
    int bytes_in;
    QByteArray response_buffer;
    int content_length;
    int processGetResponse( QByteArray * s );

  protected slots:
    void hostFound();
    void connected();
    void closed();
    void readyRead();
          
}; /* CLASS QmvHttp */

#endif /* QMVHTTP_H */
// Local Variables:
// mode:C++
// End:
