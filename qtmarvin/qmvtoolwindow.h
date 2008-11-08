/*
 *     $Id: qmvtoolwindow.h 3138 2006-10-12 22:24:19Z source $
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

#ifndef QMVTOOLWINDOW_H
#define QMVTOOLWINDOW_H

/*
 * ======================================================================
 * DESCRIPTION: Base class for Tool Windows
 *
 * ======================================================================
 */

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */

#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qlabel.h>
#include <qaction.h>
#include <qguardedptr.h>
#include "qmvapplication.h"
#include "qmcdatenav.h"
#include "qmvclass.h"
#include "qmvcanvasview.h"
#include "qmvcanvasgrid.h"

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvToolWindow : public QMainWindow
{
    Q_OBJECT
    
      public:
    QmvToolWindow( QmvSqlClass * reln, QmvApplication * parent, const char * name );
    ~QmvToolWindow();  // Destructor

    enum AGNavigation
    {
        AGNavBack4,
        AGNavBack3,
        AGNavBack2,
        AGNavBack1,
        AGNavHome,
        AGNavForward1,
        AGNavForward2,
        AGNavForward3,
        AGNavForward4,
        AGNavPrint,
        AGNavDisplay1,
        AGNavDisplay2,
        AGNavDisplay3,
        AGNavZoomIn,
        AGNavZoomOut,
        AGNavFind
    };
    void setBaseQuery( const QString & query );
    const QString getBaseQuery() const;
    void setNavigationAction( AGNavigation id, const QString & text, const QIconSet & iset,
                              const QString & mtext, int accel,
                              bool enabled = TRUE );
    
    protected slots:

    virtual void init( const QString & query = QString::null );
    virtual void slotSetBannerText( const QString & text );
    virtual void slotBack4();
    virtual void slotBack3();
    virtual void slotBack2();
    virtual void slotBack1();
    virtual void slotHome();
    virtual void slotForward1();
    virtual void slotForward2();
    virtual void slotForward3();
    virtual void slotForward4();
    virtual void slotPrint();
    virtual void slotDisplay1();
    virtual void slotDisplay2();
    virtual void slotDisplay3();
    virtual void slotZoomIn();
    virtual void slotZoomOut();
    virtual void slotSetZoom( const int zoom = 0 );
    virtual void setMagFactor( const double min, const double max, const int steps);
    virtual void setMagCurrent( const int zoom = 0 );
    virtual void slotFind();
    void slotLineDown();
    void slotLineUp();
    void slotPageDown();
    void slotPageUp();

      protected:
    QToolBar * navigationToolBar();
    QToolBar * generalToolBar();
    QToolBar * otherToolBar();
    QPixmap background;
    QmvApplication * parentApp() const;
    virtual void clearCanvasView();
    virtual void setCanvasSize( QSize resize = QSize() );
    virtual void buildBackgroundPixmap();
    const QSize canvasSize() const;
    void setTransparentColor( QColor & col );
    const QColor & getTransparentColor() const;
    QmvCanvasView * canvasView() const;
    QCanvas * getCanvas() const;
    QmvIcons * stdicons;
    
      private:
    QmvSqlClass * appl_reln;
    QString base_query;                 //!< The base query string
    QmvApplication * parent_app;
    QActionGroup * actiongp_nav;
    QActionGroup * actiongp_other;
    QActionGroup * actiongp_gen;
    QIntDict<QAction> action_nav_idx;
    QToolBar * toolbar_nav;
    QToolBar * toolbar_other;
    QToolBar * toolbar_gen;
    
    QmvCanvasView * canvasview;
    QCanvas * canvas;
    QArray<double> mag_factor;          //!< Valid magnifications
    int mag_current;                    //!< The current magnification factor
    QGuardedPtr<QLabel> banner;
    QColor transparent;
    
    virtual void closeEvent( QCloseEvent* ce );
          
}; /* CLASS QmvToolWindow */

#endif /* QMVTOOLWINDOW_H */
// Local Variables:
// mode:C++
// End:
