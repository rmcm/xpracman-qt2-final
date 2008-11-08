/*
 *     $Id: qmvpopup.h 1837 2003-08-30 09:24:24Z source $
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

#ifndef QMVPOPUP_H
#define QMVPOPUP_H

/*
 * ======================================================================
 * DESCRIPTION: A simple popup message box
 *
 * ======================================================================
 */

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <qwidget.h>
#include <qsimplerichtext.h>
#include <qapplication.h>

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvPopup : public QWidget
{
    Q_OBJECT
    
      public:
    QmvPopup( const QString& contents, const QPoint& pos, QWidget * parent = 0, const char * name = 0);
    ~QmvPopup();  // Destructor

      protected:
    void mousePressEvent( QMouseEvent*);
    void mouseReleaseEvent( QMouseEvent*);
    
      private:
          
}; /* CLASS QmvPopup */

#endif /* QMVPOPUP_H */
// Local Variables:
// mode:C++
// End:
