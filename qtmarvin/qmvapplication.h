/*
 *     $Id: qmvapplication.h 3336 2007-01-28 04:54:30Z source $
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

#ifndef QMVAPPLICATION_H
#define QMVAPPLICATION_H

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
#include <iostream.h>
#include <stdlib.h>
#include <unistd.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qprinter.h>
#include <qapplication.h>
#include <qkeycode.h>
#include <qwindowsstyle.h>
#include <qplatinumstyle.h>
#include <qmotifstyle.h>
#include <qmotifplusstyle.h>
#include <qcdestyle.h>
#include <qsgistyle.h>
// Standard Qt widgets

#include <qtoolbar.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlcdnumber.h>
#include <qmultilineedit.h>
#include <qlineedit.h>
#include <qlist.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qslider.h>
#include <qtooltip.h>
#include <qspinbox.h>
#include <qstatusbar.h>
#include <qwhatsthis.h>
#include <qtoolbutton.h>
#include <qvbox.h>
#include <qtable.h>
#include <qwidgetstack.h>
#include <qprogressbar.h>
#include <qsplitter.h>
#include <qlistview.h>
#include <qheader.h>
#include <qtextview.h>
#include <qfiledialog.h>
#include <qaccel.h>
#include <qmetaobject.h>
#include <qiconview.h>
#include <qobjectlist.h>
#include <qtextbrowser.h>
#include <qaction.h>

#include <qmainwindow.h>
#include <qlistview.h>
#include <qmv.h>
#include <qintdict.h>
#include <qwidgetstack.h>
#include <qdatetime.h>
#include <qsignalmapper.h>

/*
 * ======================================================================
 * DEFINITIONS - typedefs, enums, structures etc
 * ======================================================================
 */

extern QString QmvApplicationDir;

class QLabel;
class QCheckBox;
class QProgressBar;
class QTabWidget;
class QGroupBox;
class QButtonGroup;
class QRadioButton;

class QMultiLineEdit;
class QPopupMenu;
class QmcDateNav;
class QListView;
class QListViewItem;
class QTable;
class QTableItem;

class QmvApplication : public QMainWindow
{
    Q_OBJECT

      public:
    enum exitstatus {
        Normal=0,
        ReStart
    };

    enum MenuBarItems {
        MenuFile,
        MenuView,
        MenuPrint,
        MenuUtilities,
        MenuPreferences,
        MenuHelp,
        MenuUserLabel,
        MenuSystemLabel,
        MenuLogo
    };
    
    enum XpmAccelerators {
        Focus1,
        Focus2,
        Focus3,
        Focus4,
        ViewFK,
        SelectRow
    };
    
    QmvApplication( const QString & dbtitle, const QmvAccess * dbaccess,
                    QWidget *parent = 0, const char *name = 0 );    // Constructor
    ~QmvApplication(); // Destructor
    const QString dbAccessName() const;
    const QmvAccess * dbAccess() const;

    void loadApplicationGroups();
    void setAppPath( const QString & path );
    const QString & getAppPath() const;
    void setAppName( const QString & path );
    const QString & getAppName() const;
    void setActionGroupColour( QActionGroup * ag, const QColor * colour );
    const QColor getActionGroupColour( QActionGroup * ag ) const;
    void setUserLabel( const QString & label = QString::null );
    const QString & getUserLabel() const;
    void setAppLabel( const QString & label );
    void setAppLogo( const QPixmap & logo );
    const QString userPreference( const QString & attr, const QString & prefname = QString::null ) const;
    QProgressBar * progressBar() const;

public slots:

void toggleViewList();
    void setViewList( bool status = true );
    int restart();
    virtual void loadPreferences();
    void setCurrentPreferences( int pref );
    void setCurrentPreferences( const QString & prefname );
    bool savePreferences();
    bool editPreferences();
    bool deletePreferences();
    bool addPreferences();

        // This is called when a new panel is created.
    virtual void newListPanel( QmvRelationWidget * rlp );
    void addRowToSetTrigger( QString & reln, int sequence );
    void deleteRowFromSetTrigger( QString & reln, int sequence );
    void accessRowTrigger( QString & reln, int sequence );
    QString sessionFolder();
    
    protected slots:
    QAction * addToActionGroup( const QString & label,
                                const QIconSet & icon,
                                const QString & menulabel,
                                const QString & statustip,
                                const QString & whatsthislabel,
                                int accellkey,
                                QActionGroup * action_group,
                                char * name,
                                bool toggle = FALSE);
    QmvDBWidget * addDB(  const QString & name );
    void selectDB( const QString & title );
    QmvRelationWidget * displayQuerySet( const QString & relname );
    QmvRelationWidget * displayQuerySet( const QString & relname, const QString query );
    QmvRelationWidget * displayChildSet( const QString & parent, const QString child );

    bool formSet( QmvSqlClass * reln, const QString form_code );
    bool formReport( const QString & reln_name, const QString & query,
                     const QString & form_code );
    void panelFocus( int id );
    void viewForeign();
    void selectRow();

    virtual void help_qmvapplication();
    virtual void about_qmvapplication();
    void displaySchema();
    void objectTree();
    
      signals:
    void relationActivated( const QString & title );
    
      protected:
    QSignalMapper * signal_mapper;
    void closeEvent( QCloseEvent* );
    bool saveGeometry( QWidget * w );
    bool restoreGeometry( QWidget * w );
    QString application_dir;
    QString application_name;
    QString user_label;
    const QmvAccess * db_access;
    QComboBox * dbsel;
    QGridLayout *topgrid;
    QmvIcons * stdicons;
    QActionGroup * actiongp_file;
    QActionGroup * actiongp_view;
    QActionGroup * actiongp_print;
    QActionGroup * actiongp_utilities;
    QActionGroup * actiongp_preferences;
    QActionGroup * actiongp_help;
    QPtrDict<QColor> ag_colour;
    
    QPopupMenu * popup_file;
    QPopupMenu * popup_view;
    QPopupMenu * popup_print;
    QPopupMenu * popup_utilities;
    QPopupMenu * popup_preferences;
    QPopupMenu * popup_help;

    QDict<QActionGroup> ag_list;
    QDict<QPopupMenu> pm_list;
    QDict<QToolBar> tb_list;
    QActionGroup * addActionGroup( const QString & group, const QString & title );
    const QStringList actionGroupList() const;
    QActionGroup * actionGroup( const QString & name ) const;
    
    QDict<QAction> ac_list;
    QAction * addAction( const QString & ac_name,
                         const QString & ac_title,
                         const QString & gp_name,
                         const QString & gp_title,
                         const QIconSet & icon);
    const QStringList actionList() const;
    QAction * action( const QString & name ) const;
    
    QToolBar *toolbar_file;
    QToolBar *toolbar_view;
    QToolBar *toolbar_print;
    QToolBar *toolbar_utilities;
    QToolBar *toolbar_spacer;
    QLabel * label_user;
    QLabel * label_app;
    int id_user_label;
    int id_app_label;
    int id_app_logo;
    
    QPrinter *printer;
    QWidget *central;
    QAction * action_rlsel;
    QWidgetStack * stack_dbwin;
    QWidgetStack * stack_rlsel;
    
    QDict<QmvDBWidget> dict_dbname;
    QDict<QmvDBWidget> dict_dbtitle;
    
    QLabel     *msg;
    QProgressBar *progbar;
    int progress;
    const QmvDB *database;

    void objectTree( QObject * parent, QListViewItem * whl_parent = 0 );
    QDialog * ot_window;
    QListView *ot_listview;

    QDict< QDict<QString> > user_preferences;    //!< user settings - multiple sets.
    QIntDict<QString> preferences_idx;           //!< index to preference names.
    QString current_user_preference;
    
      private:
    QString doc_root;
    QmvDBWidget * dbw_current;
    int rellist_row;                            //!< The last row selected in relation selector.
};


#endif /* QMVAPPLICATION_H */
// Local Variables:
// mode:C++
// End:



