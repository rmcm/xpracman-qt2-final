/*
 *     $Id: qmvtabbar.h 1640 2003-03-24 08:09:56Z source $
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

#ifndef QMVTABBAR_H
#define QMVTABBAR_H

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
#include <qwidget.h>
#include <qlist.h>
#include <qlayout.h>
#include <qhbox.h>
#include <qiconset.h>
#include <qarray.h>

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvTabBar : public QWidget
{
    Q_OBJECT

      public:
    QmvTabBar( QWidget * parent = 0, const char * name = 0,
               QBoxLayout::Direction direction = QBoxLayout::LeftToRight);    // Constructor
    ~QmvTabBar();  // Destructor

    int addTab( const QString & label, int index = -1 );
    int addTab( const QIconSet & iconset, int index = -1 );
    void removeTab( int index );
    int count() const;
    void setTabActive( int index, bool state = TRUE );
    bool isTabActive( int index );
    void setTabEnabled( int index, bool state = TRUE );
    bool isTabEnabled( int index );
    void setTabVisible( int index, bool state = TRUE );
    bool isTabVisible( int index );
    void setToolTip( int index, const QString & text );

      signals:
    void selected( int index );

      public slots:
    void setCurrentTab( int index );
    
      private:
    void keyPressEvent( QKeyEvent *e );
    void focusInEvent( QFocusEvent *e );
    void focusOutEvent( QFocusEvent *e );
    void resizeEvent( QResizeEvent *re );
    bool eventFilter( QObject *o, QEvent *e );
    int addTab( const QSize & size, int index = -1 );
    void drawShadeText( QPainter *p, QRect rect, const char *text, const QColor top, const QColor bottom, int sw = 2 );

    QBoxLayout * layout_top;

    class QmvTabDetails : public QWidget
    {
          public:
        QmvTabDetails( QWidget *parent, QIconSet iconset, bool enabled = true, bool active = false )
                : QWidget ( parent, "tab widget" )
            {
                is = iconset;
                es = enabled;
                as = active;
            }

          public:
        bool setTabEnabled( bool state ) { es = state; }
        bool setTabActive( bool state ) { as = state; }
        bool isTabEnabled() const { return es; }
        bool isTabActive() const { return as; }
        bool isTabVisible() const { return isVisible(); }
        QIconSet iconset() const { return is; }

          private:
        QIconSet is;
        bool es;
        bool as;
    };
    struct tab_details
    {
        QWidget tab;
        QIconSet is;
        bool enabled;
        bool active;
    };
    QList<QmvTabDetails> tab_list;
    
          
}; /* CLASS QmvTabBar */

#endif /* QMVTABBAR_H */
// Local Variables:
// mode:C++
// End:
