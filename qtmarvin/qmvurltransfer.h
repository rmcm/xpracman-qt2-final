/*
 *     $Id: qmvurltransfer.h 1151 2002-02-20 07:39:39Z source $
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

#ifndef QMVURLTRANSFER_H
#define QMVURLTRANSFER_H

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
#include <qcstring.h>
#include <qimage.h>
#include <qprogressdialog.h>
#include <qurloperator.h>
#include <qnetworkprotocol.h>
/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvUrlTransfer : public QObject
{
    Q_OBJECT
    
      public:
    QmvUrlTransfer( const QString & url, QWidget *parent = 0, const char *name = 0 );
    ~QmvUrlTransfer();  // Destructor

    const QByteArray & load();
    const QString statusDetail() const;

      signals:
    void loadCompleted();
    
  private slots:
    void loadStart( QNetworkOperation * res );
    void loadProgress( int bytes_done, int bytes_total, QNetworkOperation * res );
    void loadFinished( QNetworkOperation * res );
    void data( const QByteArray &data_buf, QNetworkOperation * res );
    void cancel();
    
      private:
    QString remote;
    QUrlOperator * op;
    int image_progress;
    QByteArray dest_buffer;
    int data_size;
    bool op_status;
    QString status_detail;
          
}; /* CLASS QmvUrlTransfer */

#endif /* QMVURLTRANSFER_H */
// Local Variables:
// mode:C++
// End:
