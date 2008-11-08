/*
 *     $Id: qmvapplwindow.h 220 2001-01-31 07:50:34Z source $
 * 
 *   Copyright 2000 X=X Computer Software Trust
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

#ifndef QMVAPPLWINDOW_H
#define QMVAPPLWINDOW_H

/*
 * ======================================================================
 * DESCRIPTION: Interface specification for marvin editor's application
 *              windows.
 * ======================================================================
 */

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */

#include <qmainwindow.h>
#include <qlist.h>

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QMultiLineEdit;
class QToolBar;
class QPopupMenu;
class QWorkspace;
class QPopupMenu;

class QmvApplWindow : public QMainWindow
{
    Q_OBJECT
  public:
    QmvApplWindow(QWidget* parent, const char* name, int wflags);    // Constructor
    ~QmvApplWindow();  // Destructor
          
  signals:
    void message(const QString&, int );
    
  private:
          
}; /* CLASS QmvApplWindow */

#endif /* QMVAPPLWINDOW_H */
// Local Variables:
// mode:C++
// End:
