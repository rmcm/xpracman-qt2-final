/*
 *     $Id: qmvcanvasview.cpp 3080 2006-09-27 09:05:02Z source $
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
#include <qapplication.h>
#include <qpainter.h>
#include "qmvcanvasview.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmvcanvasview_version[] = "$Id: qmvcanvasview.cpp 3080 2006-09-27 09:05:02Z source $";

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

QmvCanvasView::QmvCanvasView( QCanvas * cv, QWidget * parent, const char * name, WFlags f )
        : QCanvasView( cv, parent, name, f )
{
        // double clicks are always preceded by mousepress and release
        // so use a timer to wait for a following double click
    dc_timer = new QTimer( this, "double click timer" );
    connect( dc_timer, SIGNAL( timeout() ), this, SLOT( realSingleClick() ) );
}

// Destructor

QmvCanvasView::~QmvCanvasView()
{
  
}

// --------------------------------------------------------------------------------
void QmvCanvasView::clear()
{
    
    QCanvasItemList list = canvas()->allItems();
    QCanvasItemList::Iterator it = list.begin();
    for (; it != list.end(); ++it) {
	if ( *it )
	    delete *it;
    }
}

// --------------------------------------------------------------------------------
void QmvCanvasView::contentsMousePressEvent( QMouseEvent * mpe )
{
        // reset the moving-flag
    moving = false;
    resizing = false;
    doubleclicked = false;
    button = mpe->button();
    buttonstate = mpe->state();
    rubber = false;
    
    if ( mpe->type() == QEvent::MouseButtonDblClick )
        doubleclicked = true;
    
    QCanvasItemList l = canvas()->collisions( mpe->pos() );
    for ( QCanvasItemList::Iterator it = l.begin(); it != l.end(); ++it)
    {
//  	if ( (*it)->rtti() == imageRTTI )
//          {
//   	    ImageItem *item= (ImageItem*)(*it);
//   	    if ( !item->hit( mpe->pos() ) )
//                   continue;
//  	}
	pressed = *it;
	pressed_point = mpe->globalPos();
        pressed_rect = pressed->boundingRect();
//        if ( (pressed_rect.y() + pressed_rect.height() - mpe->y() ) < 6)
        if ( mpe->button() == LeftButton && mpe->state() == ControlButton)
        {
            QApplication::setOverrideCursor(sizeVerCursor);
            resizing = true;
            rubber = false;
        } else
            QApplication::setOverrideCursor(pointingHandCursor);
        moving_point = pressed_point;
	return;
    }
    pressed = 0;
}


// --------------------------------------------------------------------------------
void QmvCanvasView::contentsMouseReleaseEvent( QMouseEvent * mre )
{
    QApplication::restoreOverrideCursor();
    
        // remember the press event - used later in realSingleClick()
    mre_pos = mre->globalPos();
    mre_button = mre->button();
    mre_state = mre->state();

        // other, non object clicks can also be interesting
        //  - eg for controls within the canvas
    if ( !pressed ) {
        realSingleClick();
        return;
    }
    
    if ( pressed && mre->globalPos() == moving_point )
    {
            // dragging
        if ( moving )    
            emit moved( pressed, mapFromGlobal( pressed_point ), mapFromGlobal( moving_point ) );
            // resizing
        else if ( resizing )
        {
            if ( rubber )
            {
                    // turn rubberband off
                QPainter p( viewport() );
                p.setPen( QColor(255,255,255) );
                p.setRasterOp(NotROP);
                p.drawRect( new_rect );
                rubber = false;
            }
            new_rect = pressed_rect;
            new_rect.setHeight( pressed_rect.height() + (moving_point.y() - pressed_point.y() ) );
            new_rect.setWidth( pressed_rect.width() + (moving_point.x() - pressed_point.x() ) );
            
                
            emit resized( pressed, pressed_rect, new_rect );
        } else {
                // single or double clicked ... currently is a single click, but
                // there may be a double click in the pipeline, so set the timer
            dc_timer->start(QApplication::doubleClickInterval(), true);
            return;
        }
            // reset the pressed pointer.
        pressed = 0;
    }
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvCanvasView::realSingleClick()
{
    emit clicked( pressed, mapFromGlobal( mre_pos ), mre_button, mre_state );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvCanvasView::contentsMouseMoveEvent( QMouseEvent * mme )
{
    if ( pressed && button == LeftButton )
    {
        if (resizing )
                // rubber-band to indicate resize
        {
                // don't move above top of item.
            if ( mapFromGlobal( moving_point).y() >  pressed_rect.y() )
            {   
                QPainter p( viewport() );
                p.setPen( QColor(255,255,255) );
                p.setRasterOp(NotROP);
                if ( rubber )
                    p.drawRect( new_rect );
                new_rect = pressed_rect;
                new_rect.setHeight( pressed_rect.height() + (moving_point.y() - pressed_point.y() ) );
                new_rect.setWidth( pressed_rect.width() + (moving_point.x() - pressed_point.x() ) );
                p.drawRect( new_rect );
                rubber = true;
            }
        } else
        {
            moving = true;   
            pressed->moveBy( mme->globalPos().x() - moving_point.x(),
                             mme->globalPos().y() - moving_point.y() );
            canvas()->update();
        }
        QPoint vpo = viewportToContents( QPoint(0,0) );
        ensureVisible( mme->pos().x(), mme->pos().y(), 5, 5 );
	moving_point = mme->globalPos() - ( viewportToContents( QPoint(0,0) ) - vpo );
    }
}

// --------------------------------------------------------------------------------
void QmvCanvasView::contentsMouseDoubleClickEvent( QMouseEvent * mre )
{
        // stop the timer, a double click arrived.
    dc_timer->stop();
    emit doubleClicked( pressed, mapFromGlobal( mre->globalPos() ), mre->button(), mre->state() );
}

// --------------------------------------------------------------------------------
void QmvCanvasView::mouseMoveEvent( QMouseEvent * mme )
{
    ensureVisible( mme->pos().x(), mme->pos().y(), 50, 50 );
}


// --------------------------------------------------------------------------------
void QmvCanvasView::update()
{
    QCanvasView::update();
    viewport()->update();
}
