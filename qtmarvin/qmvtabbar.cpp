/*
 *     $Id: qmvtabbar.cpp 1979 2003-11-27 08:01:48Z source $
 * 
 *   Copyright 2003 X=X Computer Software Trust
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
#include <qpainter.h>
#include <qpixmap.h>
#include <qpen.h>
#include <qtooltip.h>
#include "qmvtabbar.h"

namespace QmvTabBarIcons
{
        // from KDE-iceWM theme
#include "title_bg_lt.xpm"
#include "title_bg_rt.xpm"
#include "title_bg.xpm"
#include "title_chrome.xpm"
#include "maximize.xpm"
#include "minimize.xpm"
} // end QmvTabBarIcons


/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmvtabbar_version[] = "$Id: qmvtabbar.cpp 1979 2003-11-27 08:01:48Z source $";
// #define QMVTABBAR_DEBUG

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

QmvTabBar::QmvTabBar( QWidget * parent, const char * name, QBoxLayout::Direction direction )
        : QWidget( parent, name )
{
    tab_list.setAutoDelete( true );
          // Top Level layout for this widget
    layout_top =  new QBoxLayout( this, direction, 0, -1, "layout_top" );
//     QWidget
//     layout_top->insertWidget(idx, td );

 
}

// Destructor

QmvTabBar::~QmvTabBar()
{
    tab_list.clear();
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
int QmvTabBar::addTab( const QString & label, int index )
{

    QRect textrect = fontMetrics().boundingRect(label);
    int hpad = 8;
    QRect fullrect(textrect.topLeft(), QSize( textrect.width() + hpad*2, height() ) );
    fullrect.moveTopLeft( QPoint(0, 0) );
    textrect.moveCenter( fullrect.center() );

    QIconSet is;
    QPainter pt;

    QPixmap pm_template( fullrect.size() );
    pm_template.fill(backgroundColor());
    const QPixmap * t_pm = backgroundPixmap();
    if ( t_pm && !t_pm->isNull() )
        for ( int i = 0; i < fullrect.width(); i += t_pm->width() )
            bitBlt( &pm_template, QPoint(i, 0), t_pm, t_pm->rect() );

    pt.begin(&pm_template);
    
    pt.end();

    QPixmap pm;

        // Normal.
    pm = pm_template;
    pm.fill( backgroundColor() );
    pt.begin(&pm);
    pt.setFont( font() );
    pt.setPen( colorGroup().text() );
    pt.drawText( textrect, Qt::AlignCenter, label );
    pt.end();
    is.setPixmap( pm, QIconSet::Small, QIconSet::Normal );
    
        // Disabled.
    pm = pm_template;
    pm.fill( backgroundColor() );
    pt.begin(&pm);
    pt.setFont( font() );
    pt.setPen( gray );
    pt.drawText( textrect, Qt::AlignCenter, label );
    pt.end();
    is.setPixmap( pm, QIconSet::Small, QIconSet::Disabled );
    
        // Active.
    pm = pm_template;
    pm.fill( colorGroup().highlight() );
    pt.begin(&pm);
    pt.setFont( font() );
    pt.setPen( colorGroup().highlightedText() );
    pt.drawText( textrect, Qt::AlignCenter, label );
    pt.end();
    is.setPixmap( pm, QIconSet::Small, QIconSet::Active );
    
    int idx = addTab( is, index );

    return idx;
}


// --------------------------------------------------------------------------------
//! Add a tab to the bar.
/*!
  Add a tab with the \ref QIconSet iconset at the index \e index. If \r index
  is < 0, then append the tab to the end of the bar, else insert at position
  \e index.

  \param iconset        
  \param index  
  \return <ReturnValue>
*/
// --------------------------------------------------------------------------------
int QmvTabBar::addTab( const QIconSet & iconset, int index )
{
    QmvTabDetails * td = new QmvTabDetails( this, iconset );
    if ( !td )
        return -1;

        // handle mouse presses
    td->installEventFilter(this);

        // Get the normal pixmap as a size template and burn it on tab
    QPixmap pm = (td->iconset()).pixmap( QIconSet::Small, QIconSet::Normal );
    td->setFixedSize( pm.size() );
    td->setBackgroundPixmap( pm );

        // Either append or insert
    if ( index < 0 || index > tab_list.count() )
        tab_list.append( td );
    else 
        tab_list.insert( index, td );
    
    int idx = tab_list.find( td );

        // Insert into the layout.
    layout_top->insertWidget(idx, td );

    return idx;
}

// --------------------------------------------------------------------------------
//! Remove a tab from the bar
/*!

  \param index  
*/
// --------------------------------------------------------------------------------
void QmvTabBar::removeTab( int index )
{
    QmvTabDetails * td = tab_list.at(index);
    if (!td)
        return;
    tab_list.remove(td);
}

// --------------------------------------------------------------------------------
//! Return the number of tabs in the bar
/*!

  \return <ReturnValue>
*/
// --------------------------------------------------------------------------------
int QmvTabBar::count() const
{
    return tab_list.count();
}

// --------------------------------------------------------------------------------
//! Set the enabled state of the tab
/*!


  \param index  
  \param state  
*/
// --------------------------------------------------------------------------------
void QmvTabBar::setTabEnabled( int index, bool state )
{
    QmvTabDetails * td = tab_list.at(index);
    if (!td)
        return;

    if ( state )
        td->setBackgroundPixmap( (td->iconset()).pixmap( QIconSet::Automatic, QIconSet::Normal ) );
    else {
        td->setTabEnabled( FALSE );
        td->setTabActive( FALSE );
        td->setBackgroundPixmap( (td->iconset()).pixmap( QIconSet::Automatic, QIconSet::Disabled ) );
        
    }
    td->setTabEnabled( state );
}

// --------------------------------------------------------------------------------
//! Set the active state of the tab
/*!
  Set the tab at \e index to be active if \e state is TRUE (and the tab is not
  disabled), else set it to be inactive

  \param index          index of tab
  \param state          TRUE/FALSE
*/
// --------------------------------------------------------------------------------
void QmvTabBar::setTabActive( int index, bool state )
{
    QmvTabDetails * td = tab_list.at(index);
    if (!td)
        return;

    if ( !isTabEnabled( index ) )
        return;
    
    if (state)
    {
        td->setBackgroundPixmap( (td->iconset()).pixmap( QIconSet::Automatic, QIconSet::Active ) );
        emit selected(index);
    } else
        td->setBackgroundPixmap( (td->iconset()).pixmap( QIconSet::Automatic, QIconSet::Normal ) );

    td->setTabActive( state );
    
}

// --------------------------------------------------------------------------------
//! Set the tab to be visible
/*!

  Set the tab at \e index to be visible if \e state is TRUE, else hide the tab.

  \param index          index of tab
  \param state          TRUE/FALSE
*/
// --------------------------------------------------------------------------------
void QmvTabBar::setTabVisible( int index, bool state )
{
    QmvTabDetails * td = tab_list.at(index);
    if (!td)
        return;
    
    if ( state )
        td->show();
    else
        td->hide();
}

// --------------------------------------------------------------------------------
//! Returns the active state of the tab
/*!


  \param index  
  \return <ReturnValue>
*/
// --------------------------------------------------------------------------------
bool QmvTabBar::isTabActive( int index )
{
    
    QmvTabDetails * td = tab_list.at(index);
    if (td)
        return td->isTabActive();
    else
        return false;

}

// --------------------------------------------------------------------------------
//! Returns the enabled state of the tab
/*!
  

  \param index  
  \return <ReturnValue>
*/
// --------------------------------------------------------------------------------
bool QmvTabBar::isTabEnabled( int index )
{
    QmvTabDetails * td = tab_list.at(index);
    if (td)
        return td->isTabEnabled();
    else
        return false;

}

// --------------------------------------------------------------------------------
//! Returns the visible state of the tab
/*!

  \param index  
  \return <ReturnValue>
*/
// --------------------------------------------------------------------------------
bool QmvTabBar::isTabVisible( int index )
{
    QmvTabDetails * td = tab_list.at(index);
    if (td)
        return td->isTabVisible();
    else
        return false;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTabBar::setCurrentTab( int index )
{
    setTabActive( index, TRUE );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTabBar::setToolTip( int index, const QString & text )
{
    QmvTabDetails * td = tab_list.at(index);
    if (td)
        QToolTip::add( td, text );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTabBar::keyPressEvent( QKeyEvent *e )
{;}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTabBar::focusInEvent( QFocusEvent *e )
{;}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTabBar::focusOutEvent( QFocusEvent *e )
{;}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTabBar::resizeEvent( QResizeEvent *re )
{;}

// --------------------------------------------------------------------------------
//! Reimplemented
/*!
  Handle mouse presses

  \param o      
  \param e      
  \return <ReturnValue>
*/
// --------------------------------------------------------------------------------
bool QmvTabBar::eventFilter( QObject *o, QEvent *e )
{
#ifdef QMVTABBAR_DEBUG
    const char *name = o ? o->name() : "NO_OBJECT";
    const char *classname = o ? o->className() : "NO_CLASS";
    int etype = e ? e->type() : -9999;
    qDebug( "%-20.20s::eventFilter(%20.20s/%20.20s, %6d)", "QmvTabBar", name, classname, etype );
#endif
    
    if ( !e )
        return TRUE;
    
    if ( e->type() == QEvent::MouseButtonPress )
    {
            // Look for a tab that matches the pressed object
        int index = tab_list.find( (QmvTabDetails *) o );
        
            // If we found a tab, then reset the tabs.
        if ( index >= 0 )
        {
            for ( int i = 0; i < count(); i++ )
                if ( i == index )
                    setTabActive( i, TRUE );
                else
                    setTabActive( i, FALSE );
            
        }
    }
    
    return QWidget::eventFilter( o, e );

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTabBar::drawShadeText( QPainter *p, QRect rect, const char *text,
                    const QColor top, const QColor bottom, int sw )
{
    if ( !p->isActive() )
        return;

    p->setPen( bottom );
    p->drawText( rect, Qt::AlignCenter, text );
    p->setPen( QPen( top, 3) );
    rect.moveTopLeft( rect.topLeft() - QPoint( sw,sw ) );
    p->drawText( rect, Qt::AlignCenter, text );
}
