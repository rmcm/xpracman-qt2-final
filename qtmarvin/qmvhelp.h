/*
 *     $Id: qmvhelp.h 1256 2002-04-16 03:08:08Z source $
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
 * Contributions to this code include shameless borrowing from Qt's
 * Designer help module.
 *

 */

#ifndef QMVHELP_H
#define QMVHELP_H

/*
 * ======================================================================
 * DESCRIPTION: 
 *
 * ======================================================================
 */
/*!
  \file   qmvhelp.h
  \brief  Header for the QmvHelp class

  Defines the interface to the QmvHelp class

  \author Rex McMaster, rmcm@compsoft.com.au
  \date   2001-03-15
*/

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */


#include <qdialog.h>
#include <qtextbrowser.h>
#include <qmap.h>
#include <qlabel.h>
#include <stdlib.h>

class QTextBrowser;
class QPopupMenu;

class QmvHelp : public QDialog
{
    Q_OBJECT

    
      public:
    QmvHelp( const QString& home, QWidget * parent = 0, const char *name = 0 );
    ~QmvHelp();

    void setSource( const QString& );

    void setupBookmarkMenu();
    QTextBrowser *viewer() const { return help_browser; }

  private slots:
    void setPrinterName( const QString & name );
    void slotFirstPage();
    void slotPrevPage();
    void slotNextPage();
    void slotLastPage();
    void slotPrintReport();
    void slotSetBannerText( const QString & text );
    void textChanged();
    void goHome();
    void showLink( const QString &link, const QString &title );
    void showBookmark( int id );
    const QString & firstLink() const { return first_link; }
    const QString & firstLinkTitle() const { return first_link_title; }

      private:
    void setupFileActions();
    void setupGoActions();

      private:
    QTextBrowser* help_browser;
    QLabel * banner;
    int page_total;
    int page_current;
    bool print_status;
    QString first_link;
    QString first_link_title;

    QPopupMenu *bookmarkMenu;
    QMap<int, QString> bookmarks;

};

#endif /* QMVHELP_H */
// Local Variables:
// mode:C++
// End:
