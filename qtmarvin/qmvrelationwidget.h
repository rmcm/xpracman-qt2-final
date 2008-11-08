/*
 *     $Id: qmvrelationwidget.h 3336 2007-01-28 04:54:30Z source $
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

#ifndef QMVRELATIONWIDGET_H
#define QMVRELATIONWIDGET_H

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
#include <qgroupbox.h>
#include <qhbox.h>
#include <qwhatsthis.h>
#include <qtooltip.h>
#include <qheader.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>
#include <qlayout.h>
#include <qprinter.h>
#include <qstringlist.h>
#include <qlistview.h>
#include <qtable.h>
#include <qradiobutton.h>
#include <qlist.h>
#include <qtoolbutton.h>
#include <qtextstream.h>
#include <qprogressdialog.h>
#include <qtextview.h>
#include <qvbox.h>

#include "qmvtable.h"
#include "qmvicons.h"
#include "qmvclass.h"
#include "qmvtupleattribute.h"
#include "qmvsqlquery.h"
#include "qmvexport.h"
#include "qmvform.h"
#include "qmvselect.h"
#include "qmvimport.h"
#include "qmvtabbar.h"
#include "qmvbitgraph.h"

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvSqlClass;
class QmvTupleAttribute;
class QmvTable;
class QmvQueryWidget;
class QmvTuple;
class QmvForm;
class QmvSelect;
class QmvImport;
class QmvDBWidget;

class QmvRelationWidget : public QWidget
{
    Q_OBJECT
    
      public:
        // Constructor
    QmvRelationWidget( QmvDBWidget* dbw,
                       const QmvSqlClass & rel, QmvSqlClass * parent_rel,
                       QmvRelationWidget * parent_relwid,
                       const QStringList & headatts, const int index,
                       QWidget * parent = 0, const char * name = 0 );
        // Destructor
    ~QmvRelationWidget();

    QmvSqlClass * getRelation() const;
    QmvSqlClass * getParentRelation() const;
    QmvRelationWidget * getParentRelationWidget() const;
    QmvQueryWidget * getQueryWidget() const;
    bool selectChildRelationWidget( const QString & childname );
    void setBaseQuery( const QString & query );
    QString getCompositeQuery() const;
    int getCurrentRow() const;
    QStringList getCurrentSelection( QString & child_name, const QString & sep ) const;
    void selectCurrentRow();
    void setTitle();
    void displayList( const QString & query = 0 );
    void setHeaderAttributeList( const QStringList & headatts );
    void hideColumn( int col, bool hide );
    void hideRow( int row, bool hide );
    void setColumnStretchable( int col, bool stretch );
    void setRowStretchable( int row, bool stretch );
    void clearTable();
    const void setMenu( const QHeader * hd, int section, QPopupMenu * menu ) const;
    void clear();
    QmvTupleAttribute * getAttribute( int row, int col );
    QToolButton * addTupleButton( char * name = 0);
    
public slots:

    void slotPreviousRow();
    void slotNextRow();
    void slotSetNavigationButtons();
    void toggleChildSelection();
    void showTitleBar( bool state = true );
    void showToolBar( bool state = true );
    void showTabBar( bool state = true );
    void preparePopupChildMenu();
    void popupChildMenu( const QPoint & pos = QPoint() );
    void handleChildMenuSelection( int id );
    void handleChildTabSelection( int id );
    void hideEditPanel();
    void showEditPanel();
    void relationQuery();
    void relationQuery( int changetype, int count );
    bool displayRow( int row );
    bool displayCell( int row, int col );
    void refreshList();
    void refreshRowByAttribute( const QString & value, const QString & attr );
    void refreshRowBySequenceId( int seq );
    void refreshAll();
    void refreshPropagate();
    void refreshRow( int row );
    void refreshCell( int row, int col );
    void ensureCurrentRowVisible();
    void showMiniEditor( int row );
    void showMiniEditor();
    void miniEditRefresh();
    void miniEditRow( int row );
    void miniEditCell( int row, int col );
    void relationMenu( int section );
    void addToSet( );
    void addToSet( bool broadcast );
    void copyToSet();
    void deleteFromSet();
    void printSet();
    void exportSet();
    void importSet();
    void formSet();
    void formSet( const QString & form_code );
    void setReadOnly( bool state );
    void setTupleDisplay();
    void toggleQueryWidgetVisible();
    void setQueryWidgetVisible( bool visible, bool refresh );
        
private slots:
    void selectChild( int button );
    void emitClicked( int row );
    void currentCellChanged( int row, int col );
    void cellClicked( int row, int col, int button, const QPoint & pos );
    void rowClicked( int row );
    void selectRow ( int row );
    void colClicked( int col );
    bool eventFilter( QObject *o, QEvent *e );
    
      signals:
    void rowAdded( QString & reln, int sequence );
    void rowAboutToBeCopied( QString & reln, int sequence );
    void rowAboutToBeDeleted( QString & reln, int sequence );
    void rowAccessed( QString & reln, int sequence );
    void rowSelected( int id, const QString & reln, const QString & query );
    void tupleSelected( int id, QmvTuple * tup );

      protected:

    void mousePressEvent( QMouseEvent * mpe );

      private:
    QmvIcons * stdicons;
    QmvSqlClass * relation;
    QmvQueryWidget * qbe_relation;
    bool qbe_visible;
    QmvDBWidget * parent_dbw;
    QmvSqlClass * parent_relation;
    QmvRelationWidget * parent_relation_widget;
    QGuardedPtr<QmvSqlClass> reln_form;
    QGuardedPtr<QmvSelect> form_select;
    QGuardedPtr<QmvForm> form_viewer;
    QGuardedPtr<QmvImport> import_data;
    
    int id_index;                           //!< Location identifier used in signals
    QIntDict<QString> display_attributes;   //!< Index of displayed attributes, by column
    QPtrDict<QString> dict_itemkey;         //!< QListItem index -> primary key by item
    QPushButton * panel_label;
    QPushButton * panel_close;
    QFrame * gb_top;
    
    QVBox * vbox_main;                          //!< main container.
    int tf_std_height;
    QmvTable * table_relation;              //!< Table for relation matrix
    int attlist_filter;                     //!< Attribute list filter
    QmvTable * table_editrow;               //!< table for editing a single row
    QToolButton * pb_miniedit;              //<! toggle button for miniedit
    bool sync_editors;                      //<! flag to prevent update loop between editors
    QPopupMenu * menu_relation;
    QmvIcons * relation_icons;
    QToolTipGroup * child_ttg;
    QmvTabBar * tab_children;               //!< Child select list
    QTextView * tuple_display;              //!< Summary display of parent tuple
    QHBox * hbox_title;                     //!< Title with navigation
    QHBox * hbox_toolbar;                   //!< Box of QBE and tuple buttons
    QHBox * hbox_tabbar;                    //!< Box for the tabbar
    QHBox * hb_tup_functions;               //!< Box for tuple buttons (within hbox_toolbar)
    
    const QPixmap childButton( QPushButton * pb, const QString & text,
                               const QString & tt_text );
    QList<QWidget> list_editors;                //!< Index of edit widgets for each column
    QIntDict<QmvBitGraph> alert_map;            //!< list of column alert maps
    QIntDict<QString> dict_children;
    QToolButton * toolbt_children;
    QPopupMenu * popup_children;
    QGuardedPtr<QLabel> label_children;
    int current_child;
    int current_row;                            //!< the current row - pressed in vertical header.
    QFrame * nav_left;
    QFrame * nav_right;
    QFrame * nav_restore;
    QFrame * label_title;
    bool read_only_state;                       //!< current state of display (true=single row; false=multirow)
    bool always_refresh_set;                    //!< Do certain events always cause refresh from persistent store?
    
}; /* CLASS QmvRelationWidget */

#endif /* QMVRELATIONWIDGET_H */
// Local Variables:
// mode:C++
// End:

