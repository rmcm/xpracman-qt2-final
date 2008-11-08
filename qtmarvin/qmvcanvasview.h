/*
 *     $Id: qmvcanvasview.h 1837 2003-08-30 09:24:24Z source $
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

#ifndef QMVCANVASVIEW_H
#define QMVCANVASVIEW_H

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

#include <qcanvas.h>
#include <qtimer.h>

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvCanvasView : public QCanvasView
{
    Q_OBJECT

      public:
    QmvCanvasView( QCanvas * cv = 0, QWidget * parent = 0, const char * name = 0, WFlags f = 0 );
    ~QmvCanvasView();  // Destructor
    void clear();

  public slots:
    void update();
    
  private slots:
    void realSingleClick();
    
      protected:
    void contentsMousePressEvent( QMouseEvent * mpe );
    void contentsMouseReleaseEvent( QMouseEvent * mre );
    void contentsMouseMoveEvent( QMouseEvent * mme );
    void contentsMouseDoubleClickEvent( QMouseEvent * mpe );
    void mouseMoveEvent( QMouseEvent * mme );

      signals:
    void status( const QString & st );
    void doubleClicked( QCanvasItem * pressed, const QPoint & pressed_point,
                  ButtonState button, ButtonState state );
    void clicked( QCanvasItem * pressed, const QPoint & pressed_point,
                  ButtonState button, ButtonState state );
    void moved( QCanvasItem * pressed, const QPoint & from, const QPoint & to );
    void resized( QCanvasItem * pressed, const QRect & from, const QRect & to );

      private:
    QTimer *dc_timer;
    QPoint mre_pos;
    ButtonState mre_button;
    ButtonState mre_state;
    QCanvasItem * pressed;
    QRect pressed_rect;
    QRect new_rect;
    QPoint pressed_point;
    QPoint moving_point;
    bool moving;
    bool doubleclicked;
    bool resizing;
    bool rubber;
    ButtonState button;
    ButtonState buttonstate;
          
}; /* CLASS QmvCanvasView */

#endif /* QMVCANVASVIEW_H */
// Local Variables:
// mode:C++
// End:
