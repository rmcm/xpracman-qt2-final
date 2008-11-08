/*
 *     $Id: qmvicons.h 3214 2006-12-04 21:11:52Z source $
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

#ifndef QMVICONS_H
#define QMVICONS_H

/*
 * ======================================================================
 * DESCRIPTION: Interface for standard Icons management class
 *
 * ======================================================================
 */

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */

#include <qobject.h>
#include <qiconset.h>
#include <qintdict.h>
#include <qpushbutton.h>

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */


class QmvIcons : public QObject
{
    
    Q_OBJECT
    
      public:

    enum StdIconSet {
        BlankPageIcon,
        CloseIcon,
        DeleteIcon,
        DocAdminIcon,
        DocFaqIcon,
        DocKeyBindingsIcon,
        DocOverviewIcon,
        DocProgIcon,
        DocUserIcon,
        DocWhatIcon,
        ExportIcon,
        FilenewIcon,
        ImportIcon,
        Left1Icon,
        Left2Icon,
        NewPageIcon,
        PrintIcon,
        FaxIcon,
        EmailIcon,
        QuitIcon,
        Right1Icon,
        Right2Icon,
        TextPageIcon,
        HomeIcon
    };

    QmvIcons( QObject * parent = 0, const char * name = 0 );    // Constructor
    ~QmvIcons();  // Destructor
    const QIconSet getIconSet( const StdIconSet pixmap ) const;
    const QPixmap getPixmap( const StdIconSet pixmap ) const;
    QPixmap * makeTextPixmap( QPixmap *pm, const QString & text,
                            QFont font, const QColor colour, const QRect & rect );
    const QPixmap makeTextButton( const QPushButton * pb, const QString & text, const QColor & cl );
    
    private:
    QIntDict<QPixmap> pixmap_list;
          
}; /* CLASS QmvIcons */

#endif /* QMVICONS_H */
// Local Variables:
// mode:C++
// End:
