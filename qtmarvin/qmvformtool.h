/*
 *     $Id: qmvformtool.h 3232 2006-12-15 01:18:27Z source $
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

#ifndef QMVFORMTOOL_H
#define QMVFORMTOOL_H

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

#include <qdialog.h>
#include <qintdict.h>
#include <qlabel.h>
#include <qdatetime.h>
#include "qmcdatenav.h"
#include "qmvclass.h"
#include "qmvtoolwindow.h"
#include "qmvroweditor.h"
#include "qmv_form_api.h"

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */
//class QmvApplication;


class QmvFormTool : public QmvToolWindow
{
    Q_OBJECT

      public:
    QmvFormTool( QmvRelationWidget * rlw, QmvApplication * parent = 0, const char * name = 0 );
    ~QmvFormTool();  // Destructor

    enum page {
        FirstPage = 0,
        StandardPage,
        LastPage
    };
    
public slots:

    // Reimplemented
    
virtual void init( const QString & name = QString::null );
    
protected slots:

    // Reimplemented 

virtual void slotSetBannerText( const QString & text );
    virtual void slotBack1();
    virtual void slotForward1();
    virtual void slotPrint();
    virtual void slotFax();
    virtual void slotEmail();

        // new
    void slotAddNewDetail( int id );
    void displayDetails();
    void popupEventDetails( QCanvasItem * pressed, const QPoint & pt, ButtonState button, ButtonState state );
    void eventLabelMoved( QCanvasItem * pressed, const QPoint & from, const QPoint & to );
    void eventLabelResized( QCanvasItem * pressed, const QRect & from, const QRect & to );
    void dataChangeRequested( QCanvasItem * pressed, const QPoint & pt, ButtonState button, ButtonState state );
    int findPatient( int patn_sequence = -1 );
        
      signals:
    void detailUpdated( int rowid );

      protected:

        // reimplemented
    virtual void buildBackgroundPixmap();
    
      private:
    QmvRelationWidget * rlw_form;
    QmvRelationWidget * rlw_fmdt;
    QmvSqlClass * reln_form;
    QmvSqlClass * reln_fmdt;
    formHeader * fh;
    QSize pagesize;
    
    QPopupMenu * popup_file;
    QPopupMenu * popup_print;
    QPopupMenu * popup_utilities;
    QPopupMenu * popup_help;

    QPixmap background;
    int current_page;
    QPtrDict<int> canvas_details;

    void initDisplayParameters();

}; /* CLASS QmvFormTool */

#endif /* QMVFORMTOOL_H */
// Local Variables:
// mode:C++
// End:
