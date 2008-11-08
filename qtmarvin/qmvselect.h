/*
 *     $Id: qmvselect.h 2411 2005-01-30 23:13:53Z source $
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

#ifndef QMVSELECT_H
#define QMVSELECT_H

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
#include <qlistview.h>
#include <qstring.h>
#include "qmvclass.h"

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvClass;

class QmvSelect :  public QDialog
{
    Q_OBJECT
    
      public:
    QmvSelect( const QStringList & options, QWidget * parent = 0, const char * name = 0 );    // Constructor
    QmvSelect( const QList<QStringList> & values, const QStringList & labels, int sortby = 0,
               QWidget * parent = 0, const char * name = 0 );    // Constructor
    QmvSelect( const QList< QList<QPixmap> > & pixmaps, const QList<QStringList> & values, const QStringList & labels, int sortby = 0,
               QWidget * parent = 0, const char * name = 0 );    // Constructor
    QmvSelect( QmvClass * reln = 0, QWidget * parent = 0, const char * name = 0 );    // Constructor
    QmvSelect( QmvClass * reln, QStringList & list, QWidget * parent = 0, const char * name = 0 );    // Constructor
    ~QmvSelect();  // Destructor
    
    int setCurrentRow( int row );
    
      signals:
    void escapePressed();

      protected:
    void keyPressEvent( QKeyEvent *ev );

  protected slots:
    void highlighted( QListViewItem * item );
    void selected( QListViewItem * item );
    void accept();
    void reject();
    
  private slots:
    void cancelSelect();
    
      private:
    int current_item;
    int index_column;
    int num_rows;
    int num_cols;
    int sort_by;
    QStringList label_list;
    QList<QStringList> value_list;
    QList< QList<QPixmap> > pixmap_list;
    QList<QString> col_string;
    QListView * list_view;
    QmvClass * relation;

    void init();

}; /* CLASS QmvSelect */

#endif /* QMVSELECT_H */
// Local Variables:
// mode:C++
// End:
