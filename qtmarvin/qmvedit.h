/*
 *     $Id: qmvedit.h 354 2001-05-12 05:59:24Z source $
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

#ifndef QMVEDIT_H
#define QMVEDIT_H

/*
 * ======================================================================
 * DESCRIPTION: Editor classes
 *      QmvList, QmvItem, QmvEditor
 * ======================================================================
 */

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */

#include <qvbox.h>
#include <qlistview.h>
#include <qguardedptr.h>
#include "qmvclass.h"
#include "qmvtuple.h"
#include "qmvattlist.h"

static QColor backColor1( 226, 244, 216 );
static QColor backColor2( 235, 235, 235 );
static QColor selectedBack( 192, 192, 192 );

class QmvClass;
class QmvTuple;
class QmvTupleAttribute;
class QmvList;
class QmvItem;
class QmvEditor;
class QPainter;
class QColorGroup;
class QComboBox;
class QLineEdit;
class QPushButton;
class QHBox;
class QSpinBox;
class QLabel;
class QCloseEvent;
class QResizeEvent;

/*
 * ======================================================================
 * CLASS DEFINITION - QmvList
 * ======================================================================
 */

class QmvList : public QListView
{
    Q_OBJECT

public:
    QmvList(QmvEditor * editor);
    ~QmvList();

    void setPersistent(bool state = TRUE);
    void setupTuples();
    void setCurrentItem( QListViewItem *i );
    void updateHeading();
    void valueChanged( QmvItem *i );
    void refetchData();
    void setAttributeValue( QmvItem *i );
    void setCurrentAttribute( const QString &n );

    QmvEditor *Editor() const;

public slots:
    void updateEditorSize();
    void resetAttribute();
    bool eventFilter( QObject *o, QEvent *e );

private slots:

    void itemPressed( QListViewItem *i, const QPoint &p, int c );
    void toggleOpen( QListViewItem *i );
    void itemMenu(QListViewItem *i, const QPoint &p, int c );
    bool itemRemove();
    QmvItem *itemInsert();
    QmvItem *itemCopy();
    void currentSetMenu();
    bool printSet();
    bool exportSet();
    
protected:

    bool dataisPersistent();
    void resizeEvent( QResizeEvent *e );
    void paintEmptyArea( QPainter *p, const QRect &r );
    bool addQmvItem( QmvItem *&item, const QCString &name, QString &type );
    
private:
    QmvEditor *editor;
    bool dataispersistent;
    QPopupMenu *tuplemenu;
    QPopupMenu *relationmenu;
          
}; /* CLASS QmvList */

/*
 * ======================================================================
 * CLASS DEFINITION - QmvItem
 * ======================================================================
 */

class QmvItem : public QListViewItem
{
      public:

    QmvItem( QmvList *ls, QmvItem *after, QmvTuple *tuple, const QString &label );
    
    ~QmvItem();

    QmvTuple *shuttleTuple() const;
    QmvClass *shuttleClass() const;
    QmvTupleAttribute *shuttletupleAttribute() const;
    
    void paintCell( QPainter *p, const QColorGroup &cg, int column, int width, int alignment );
    void paintBranches( QPainter * p, const QColorGroup & cg, int w, int y, int h, GUIStyle s );
    void paintFocus( QPainter *p, const QColorGroup &cg, const QRect &r );

    virtual bool hasSubItems() const;
    virtual void createChildren();
    virtual void initChildren();
    virtual void deleteChildren();

    bool isOpen() const;
    void setOpen( bool b );

    virtual void showEditor();
    virtual void hideEditor();

    virtual void setValue( const QString &v );
    virtual void refreshValue();
    QString value() const;
    QString name() const;
    virtual void notifyValueChange();

    virtual void setChanged( bool update = TRUE );
    bool isChanged() const;

    virtual void placeEditor( QWidget *w );

    virtual void childValueChanged( QmvItem *child );

    void addChild( QmvItem *i );
    void removeChild( QmvItem *i );
    int childCount() const;
    QmvItem *child( int i ) const;

    virtual bool hasCustomContents() const;
    virtual void drawCustomContents( QPainter *p, const QRect &r );

    void updateBackColor();
    void setup();

    virtual QString currentItem() const;
    virtual int currentIntItem() const;
    virtual void setCurrentItem( const QString &s );
    virtual void setCurrentItem( int i );
    virtual int currentIntItemFromObject() const;
    virtual QString currentItemFromObject() const;

    void setFocus( QWidget *w );

    virtual void toggle();
    
      protected:
//! A state variable to register whether an item is editable.    
    bool allowedit;

//! A pointer to the QListView object of this item.    
    QmvList *listview;
//! The current value (second column) of this item.
    QString val;
//! A pointer the the QmvClass associated with this item.
    QGuardedPtr<QmvClass> shuttleclass;
    
//! A pointer to the QmvTuple associated with this item.
    QGuardedPtr<QmvTuple> shuttletuple;
    
//! A pointer to the QmvTupleAttribute associated with this item.
/*!    This may be 0 if the item is a parent QmvTuple
 */
    QGuardedPtr<QmvTupleAttribute> shuttletupleattribute;

      private:    
    QColor backgroundColor();
    void createResetButton();
    void updateResetButtonState();
    
    
      private:
//!  A state variable registering whether the item is currently open or closed.
    bool open;
    
//!  A local QList of pointers to visible child items
    QList<QmvItem> children;
    
//! Default background colour (???)
    QColor backColor;
//! A reset button to restore pre-edit value of this item.    
    QPushButton *resetButton;
};

/*
 * ======================================================================
 * CLASS DEFINITION - QmvTextItem
 * ======================================================================
 */

class QmvTextItem : public QObject,
                    public QmvItem
{
    Q_OBJECT

public:
    QmvTextItem( QmvList * cls, QmvItem *after,
                 QmvTupleAttribute *attribute, const QString &label = 0,
                 bool comment = FALSE, bool multiLine = FALSE, bool ascii = FALSE, bool a = FALSE );
    ~QmvTextItem();

    virtual void createChildren();
    virtual void initChildren();

    virtual void showEditor();
    virtual void hideEditor();

    virtual void setValue( const QString &v );
    virtual bool hasSubItems() const;
    virtual void childValueChanged( QmvItem *child );

    virtual void setChanged( bool update = TRUE );

private slots:

    virtual void setValue();
    void getText();

private:
    QLineEdit *lined();
    QGuardedPtr<QLineEdit> lin;
    QGuardedPtr<QHBox> box;
    QPushButton *button;
    bool withComment, hasMultiLines, asciiOnly, accel;

};

/*
 * ======================================================================
 * CLASS DEFINITION - QmvBoolItem
 * ======================================================================
 */

class QmvBoolItem : public QObject,
                    public QmvItem
{
    Q_OBJECT

public:
    QmvBoolItem( QmvList * cls, QmvItem *after,
                 QmvTupleAttribute *attribute, const QString &label = 0 );
    ~QmvBoolItem();

    virtual void createChildren();
    virtual void initChildren();
    
    virtual void showEditor();
    virtual void hideEditor();

    virtual void setValue( const QString &v );
    virtual void toggle();

private slots:

    virtual void setValue();

private:
    QComboBox *combo();
    QGuardedPtr<QComboBox> comb;
    QString str_true;
    QString str_false;
    QString lab_true;
    QString lab_false;
};


/*
 * ======================================================================
 * CLASS DEFINITION - QmvListItem
 * ======================================================================
 */

class QmvListItem : public QObject,
                    public QmvItem
{
    Q_OBJECT

public:
    QmvListItem( QmvList * cls, QmvItem *after,
                 QmvTupleAttribute *attribute, const QString &label = 0 );
    ~QmvListItem();

    virtual void createChildren();
    virtual void initChildren();
    
    virtual void showEditor();
    virtual void hideEditor();
    
    virtual void setValue( const QString &v );
    virtual void setValue( uint i );
    
    virtual QString currentItem() const;
    virtual int currentIntItem() const;
    virtual void setCurrentItem( const QString &s );
    virtual void setCurrentItem( int i );
    virtual int currentIntItemFromObject() const;
    virtual QString currentItemFromObject() const;
    virtual void toggle() {}

private slots:

    void setValue();

private:
    QComboBox *combo();
    QGuardedPtr<QComboBox> comb;
    int oldInt;
    bool editable;
    QString oldString;
};


/*
 * ======================================================================
 * CLASS DEFINITION - QmvEditor
 * ======================================================================
 */

class QmvEditor : public QVBox
{
    Q_OBJECT

public:
    QmvEditor( QWidget *parent, QmvClass * cls = 0);
    ~QmvEditor();

    QmvClass *shuttleClass() const;
    
    void clear();
    void setup();

    void emitWidgetChanged();
    void refetchData();

        //void closed( FormWindow *w );

    QmvList *tupleList() const;
        //FormWindow *formWindow() const;

    QString classOfCurrentTuple() const;
    QmvAttribute * metaObjectOfCurrentAttribute() const;

    void resetFocus();

signals:
    void hidden();

public slots:
    void setshuttleClass( QmvClass *c );

protected:
    void closeEvent( QCloseEvent *e );
    
private:
        // The current shuttle class
    QGuardedPtr<QmvClass> shuttlecls;
        // The tuple listview
    QGuardedPtr<QmvList> listview;

};


#endif /* QMVEDIT_H */
// Local Variables:
// mode:C++
// End:
