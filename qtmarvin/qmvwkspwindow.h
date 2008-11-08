/*
 *     $Id: qmvwkspwindow.h 339 2001-04-29 23:38:35Z source $
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

#ifndef QMVWKSPWINDOW_H
#define QMVWKSPWINDOW_H

/*
 * ======================================================================
 * DESCRIPTION: Interface specification for marvin editor's workspace
 *              window.
 * ======================================================================
 */

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */

#include <qmainwindow.h>
#include <qlist.h>
#include <qmv.h>

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
class QmvApplWindow;


class QmvWkspWindow : public QMainWindow
{
    Q_OBJECT

      public:

    QmvWkspWindow( const QmvAccess * dbaccess ); 
    ~QmvWkspWindow();
    const QmvAccess * dbAccess() const;
    void dumpObjectTree( QObject * parent = 0, QListViewItem * whl_parent = 0 );
    
  private slots:
 
    void dbWidget( QListViewItem *dbitem );
    void dbWidget( const QString & dbname, const int rellevel = 0 );
    QmvApplWindow * newWindow( const QString & caption,
                               const QPixmap & icon );
    void about_marvin();
    void windowsMenuAboutToShow();
    void windowsMenuActivated( int id );
    void databaseList();
    void ObjectTree();
          
      private:

    QWorkspace *ws;
    const QmvAccess * access;
    QStringList dblist;
    QListView *viewdblist;
    QmvDB *dbcurrent;
    QmvDBWidget *viewdbcurrent;
    int default_reln_level;             //!< the default limit to recursion of class relations.
    QPopupMenu *windowsMenu;

    QGuardedPtr<QmvApplWindow> db_window;
    QGuardedPtr<QmvApplWindow> ot_window;
    QListView *ot_listview;
          
}; /* CLASS QmvWkspWindow */

#endif /* QMVWKSPWINDOW_H */
// Local Variables:
// mode:C++
// End:
