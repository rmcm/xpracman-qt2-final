/*
 *     $Id: qtkugar.h 605 2001-07-03 22:17:41Z source $
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
    // 
    // sections copied from
    // report.h

#ifndef QTKUGAR_H
#define QTKUGAR_H

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


#include <qmainwindow.h>
#include <qfile.h>
#include <qmessagebox.h>
#include "mreportviewer.h"

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QtKugar : public QMainWindow
{
    Q_OBJECT

      public:
    QtKugar();    // Constructor
    ~QtKugar();  // Destructor
          
    void renderReport();
    void setReportTemplate(const QString & report_data );
    void setReportData(const QString & report_template );

    QSize sizeHint() const;
    
  private slots:
//      void slotFileQuit();
//      void slotPrint();
//      void slotViewToolBar();
//      void slotViewStatusBar();
//      void slotPreferedTemplate(const QString &);
  
      private:
    MReportViewer *rptviewer;	// The viewer widget.
          
}; /* CLASS QtKugar */

#endif /* QTKUGAR_H */
// Local Variables:
// mode:C++
// End:
