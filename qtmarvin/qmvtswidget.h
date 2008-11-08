/*
 *     $Id: qmvtswidget.h 1644 2003-03-26 00:30:40Z source $
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

#ifndef QMVTSWIDGET_H
#define QMVTSWIDGET_H

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
#include <qlineedit.h>
#include <qmcdatenav.h>
#include <qdatetime.h>
#include <qdialog.h>

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvTSWidget : public QWidget
{
    Q_OBJECT
    
      public:
    
        // The modes of use.
    enum TSMode {
        DateMode = 1,
        TimeMode = 2,
        DateTimeMode = DateMode | TimeMode,
        DateNoText   = 4                        // use only the calendar widget
    };
    
    QmvTSWidget( QWidget * parent, const char * name = 0, uint mode = DateTimeMode );    // Constructor
    ~QmvTSWidget();  // Destructor
    
    QDateTime getDateTime() const;
    void setDate( const QDate & date );
    void setTime( const QTime & time );
    void setDateTime( const QDateTime & datetime );
          
      private slots:
    void setTime( const QString & time_str );
    void setDate( const QString & date_str );
    void setDate();
    void setTime();
    void showCalendar();
    void setDateByCalendar();
          
      private:
    uint use_mode;
    QLineEdit * date_edit;
    QLineEdit * time_edit;
    QmcDateNav * date_nav;
    QDialog * dl_datenav;
    QDateTime current_dt;
          
}; /* CLASS QmvTSWidget */

#endif /* QMVTSWIDGET_H */
// Local Variables:
// mode:C++
// End:
