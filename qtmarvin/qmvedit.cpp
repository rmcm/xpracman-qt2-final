/*
 *     $Id: qmvedit.cpp 870 2001-10-07 04:44:14Z source $
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

/*
 * ======================================================================
 * DESCRIPTION: 
 *
 * ======================================================================
 */

/*!
  \file   qmvedit.cpp
  \brief  A class to edit class data

  <long description>

  \author Source Code
  \date   2000-12-03
*/


/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */

#include <stdlib.h>
#include <iostream.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qapplication.h>
#include <qheader.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaccel.h>
#include <qstring.h>
#include <qpopupmenu.h>
#include <qcombobox.h>
#include <qlistbox.h>
#include <qprinter.h>
#include "qmvedit.h"
#include "asciivalidator.h"

#include "resetproperty.xpm"
/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvedit_version[] = "$Id: qmvedit.cpp 870 2001-10-07 04:44:14Z source $";

/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */

//  static QString to_string( const QVariant &v, bool accel )
//  {
//      if ( !accel )
//  	return v.toString();
//      return QAccel::keyToString( v.toInt() );
//  }


/*
 * ======================================================================
 * MEMBER FUNCTIONS: QmvList
 * ======================================================================
 */
QmvList::QmvList(QmvEditor * editor)
        : QListView(editor), editor(editor)
{
    viewport()->setBackgroundMode( PaletteBackground );
    setResizePolicy( QScrollView::Manual );
    
    if (editor->shuttleClass())
    {
        qDebug("qmvlist:classname=%s",  editor->shuttleClass()->relationName().latin1());
        addColumn( tr( editor->shuttleClass()->relationTitle() ) );
        QString ctlabel;
        ctlabel.sprintf("%d rows",editor->shuttleClass()->count());
        addColumn( tr( ctlabel) );
        
    } else {
        addColumn( "No Class Selected" );
        addColumn( "0 Rows" );
    }
    tuplemenu = new QPopupMenu(this, "item menu");
    tuplemenu->insertItem("Item Menu");
    tuplemenu->insertSeparator();
    tuplemenu->insertItem("&New", this, SLOT(itemInsert()));
    tuplemenu->insertItem("&Copy", this, SLOT(itemCopy()));
    tuplemenu->insertItem("&Delete", this, SLOT(itemRemove()));

    relationmenu = new QPopupMenu(this, "relation menu");
    relationmenu->insertItem("Relation Menu");
    relationmenu->insertSeparator();
    relationmenu->insertItem("&Print", this, SLOT(printSet()));
    relationmenu->insertItem("&Export", this, SLOT(exportSet()));
    
    
    QToolTip::add( header(), "Click for codes Utilites-Menu");
    
    connect( header(), SIGNAL( sizeChange( int, int, int ) ),
	     this, SLOT( updateEditorSize() ) );
    disconnect( header(), SIGNAL( clicked( int ) ),
		this, SLOT( changeSortColumn( int ) ) );
    connect( this, SIGNAL( pressed( QListViewItem *, const QPoint &, int ) ),
	     this, SLOT( itemPressed( QListViewItem *, const QPoint &, int ) ) );
    connect( this, SIGNAL( rightButtonPressed( QListViewItem *, const QPoint &, int ) ),
	     this, SLOT( itemMenu( QListViewItem *, const QPoint &, int ) ) );
    connect( header(), SIGNAL( clicked( int ) ),
	     this, SLOT( currentSetMenu() ) );
    connect( this, SIGNAL( doubleClicked( QListViewItem * ) ),
	     this, SLOT( toggleOpen( QListViewItem * ) ) );
    setSorting( -1 );
    setHScrollBarMode( AlwaysOff );
}


QmvList::~QmvList()
{}


//! Setup the list with tuple data
/*!
  Load the data from the associated relation, and diplay
  in the listview.

*/
void QmvList::setupTuples()
{
    if ( !editor->shuttleClass())
        return;

    QmvItem *item = 0;
    QmvClass *cls = editor->shuttleClass();
    QString key  = cls->userKey();
    if (!key)
        key = cls->primaryKey();
        
        // Add tuples in current set to the list,
        // using primary key and descriptive label

    for (int row = 0; row < cls->count(); row++)
    {
        QmvTuple * tp = cls->tupleAt(row);
        if (!tp)
            continue;
        
        item = new QmvItem(this, item, tp,
                               cls->attributeValue(row, key));
    }
}

// Reimplemented 
void QmvList::setCurrentItem( QListViewItem *i )
{
    if ( currentItem() )
	( (QmvItem*)currentItem() )->hideEditor();
    QListView::setCurrentItem( i );
    ( (QmvItem*)currentItem() )->showEditor();
}

//! Set the headings
/*!
  Display relation title and loaded-set count in the listview
  headings.

*/
void QmvList::updateHeading()
{
    setColumnText( 0, tr( editor->shuttleClass()->relationTitle() ) );
    QString ctlabel;
    ctlabel.sprintf("%d rows",editor->shuttleClass()->count());
    setColumnText( 1, tr( ctlabel) );
}



//! Handle changes to tuple.
/*!

  This method handles, at the tuple level, changes to the tuple.
  (see \ref QmvItem::notifyValueChange for changes to attributes of tuples)
  
  This method is called when attributes of this tuple have changed. 

  \param state  TRUE/FALSE
  \sa           QmvItem::notifyValueChange
  
*/
void QmvList::setPersistent( bool state )
{
    dataispersistent = state;
}

//! Return persistent state
/*!
  The current state of the data with respect to persistent storage.

  \return TRUE/FALSE
*/
bool QmvList::dataisPersistent()
{
    return dataispersistent;
}

void QmvList::refetchData()
{}

void QmvList::setAttributeValue( QmvItem *i )
{}

void QmvList::setCurrentAttribute( const QString &n )
{}

QmvEditor * QmvList::Editor() const
{
    return editor;
}

void QmvList::updateEditorSize()
{
    QSize s( header()->sectionPos(1) + header()->sectionSize(1), height() );
    QResizeEvent e( s, size() );
    resizeEvent( &e );
    viewport()->repaint( s.width(), 0, width() - s.width(), height(), FALSE );
}

void QmvList::resetAttribute()
{
    if ( !currentItem() )
	return;
    QmvItem *i = (QmvItem*)currentItem();
    i->setValue(i->shuttletupleAttribute()->originalValue());
    i->setChanged(TRUE);
}
// Reimplemented 
bool QmvList::eventFilter( QObject *o, QEvent *e )
{
    if ( !o || !e )
	return TRUE;

    QmvItem *i = (QmvItem*)currentItem();
    if ( o != this &&e->type() == QEvent::KeyPress )
    {
	QKeyEvent *ke = (QKeyEvent*)e;
	if ( ( ke->key() == Key_Up || ke->key() == Key_Down ) &&
	     ( o != this || o != viewport() ) &&
	     !( ke->state() & ControlButton ) )
        {
	    QApplication::sendEvent( this, (QKeyEvent*)e );
	    return TRUE;
	} else
            if ( ( !o->inherits( "QLineEdit" ) ||
                   ( o->inherits( "QLineEdit" ) && ( (QLineEdit*)o )->isReadOnly() ) ) &&
                 i && i->hasSubItems() )
            {
                if ( !i->isOpen() &&
                     ( ke->key() == Key_Plus ||
                       ke->key() == Key_Right ))
                    i->setOpen( TRUE );
                else
                    if ( i->isOpen() &&
                         ( ke->key() == Key_Minus ||
                           ke->key() == Key_Left ) )
                        i->setOpen( FALSE );
            } else
                if ( ( ke->key() == Key_Return || ke->key() == Key_Enter ) && o->inherits( "QComboBox" ) )
                {
                    QKeyEvent ke2( QEvent::KeyPress, Key_Space, 0, 0 );
                    QApplication::sendEvent( o, &ke2 );
                    return TRUE;
                }
    }
    
    return QListView::eventFilter( o, e );
}

// ???Replaces pressed() (via new connect())
void QmvList::itemPressed( QListViewItem *item, const QPoint &p, int c )
{   
    if ( !item )
        return;
    QmvItem *pi = (QmvItem*)item;
        // Attributes cannot be pressed - TODO: change this if implementing subattributes.
    if ( pi->shuttletupleAttribute() )
        return;

        // This is a tuple press, update the persistent storage
    if (!dataisPersistent() && pi->isOpen())
    {
            // Save via class to emit signal
        if ( !pi->shuttleClass()->save( pi->shuttleTuple() ) )
        {
            setPersistent(FALSE);
            QMessageBox::warning(0, "QmvList:Warning_save",
                                 tr("The data-save failed.\n\n %1")
                                 .arg(pi->shuttleTuple()->lastError()),
                                 "OK", 0 );
        } else
        {
            setPersistent(TRUE);
            for (int i = 0; i < pi->childCount(); ++i )
            {
                QmvItem *ci = pi->child(i);
                ci->refreshValue();
                ci->setChanged(FALSE);
            }
            
        }
        pi->setText( 0, pi->shuttleTuple()->userkeyValue());
        pi->setText( 1, pi->shuttleTuple()->userlabelValue());
    }
    
        // if first column (0) and in left margin (ie +/- icon)
    if ( c == 0 && viewport()->mapFromGlobal( p ).x() < 20 )
        toggleOpen( item );
}   

void QmvList::toggleOpen( QListViewItem *i )
{
    if ( !i )
        return;
    QmvItem *pi = (QmvItem*)i;
        // is a parent (tuple) item
    if ( !pi->shuttletupleAttribute() ) {
        pi->setOpen( !pi->isOpen() );
    } else {
        pi->toggle();
    }
}

void QmvList::itemMenu(QListViewItem *i, const QPoint &p, int c )
{
    if (!tuplemenu)
        return;
    
    tuplemenu->popup(p);
}

bool QmvList::itemRemove()
{
    if ( !currentItem() || !editor->shuttleClass())
        return(FALSE);

    QmvItem *ci = (QmvItem *)currentItem();
    if (ci->shuttletupleAttribute())
    {
        QMessageBox::warning( this, "Tuple:Remove",
                                  tr("You cannot remove a row attribute, only whole rows.\n\n %1 : %2")
                                  .arg(ci->text(0))
                                  .arg(ci->text(1)),
                                  "OK", 0);
        return(FALSE);
    }
    
    
    QmvClass * cls = ci->shuttleClass();
    QmvTuple * tp = ci->shuttleTuple();
    
    
    switch( QMessageBox::information( this, "Tuple:Remove",
				      tr("Are you sure that you want to delete this item? \n\n %1 : %2")
                                      .arg(ci->shuttleTuple()->userkeyValue())
                                      .arg(ci->shuttleTuple()->userlabelValue()),
				      "Yes", "Cancel",
				      0, 1 ) )
    {
        case 0:
                // Closing will remove the children.
            ci->setOpen(FALSE);
            delete ci;
            cls->remove(tp, TRUE);
            updateHeading();
            break;
        case 1:
        default: // just for sanity
                //ce->ignore();
            break;
    }
}

QmvItem * QmvList::itemInsert()
{
    if ( !editor->shuttleClass())
        return(0);
    
    QmvItem *ci;
    if (currentItem())
        ci = (QmvItem *)currentItem();
    else
        ci = 0;
    
    QmvItem *item = 0;
    QmvClass *cls = editor->shuttleClass();
    QString key  = cls->userKey();
        
        // Add tuples in current set to the list,
        // using primary key and descriptive label

    int newrow = cls->newTuple();
    if (newrow < 0)
        return(0);
    QmvTuple * newtuple = cls->tupleAt(newrow);
    QString tmp = cls->relationTitle();
        // Close the current item to reduce confusion
    if (ci && ci->isOpen())
        ci->setOpen(FALSE);
    
    item = new QmvItem(this, ci, newtuple, newtuple->userkeyValue());
    
    item->setText( 0, newtuple->userkeyValue());
    item->setText( 1, newtuple->userlabelValue());
    item->setOpen(TRUE);
    updateHeading();

    return(item);
}

QmvItem * QmvList::itemCopy()
{
    if ( !editor->shuttleClass())
        return(0);
    
    QmvItem *ci;
    if (currentItem())
        ci = (QmvItem *)currentItem();
    else
        ci = 0;
    
    QmvItem *item = 0;
    QmvClass *cls = editor->shuttleClass();
    QString key  = cls->userKey();
    QmvTuple * oldtuple = ci->shuttleTuple();
        
        // Add tuples in current set to the list,
        // using primary key and descriptive label

    int newrow = cls->newTuple();
    if (newrow < 0)
        return(0);
    QmvTuple * newtuple = cls->tupleAt(newrow);
    
        // Close the current item to reduce confusion
    if (ci && ci->isOpen())
        ci->setOpen(FALSE);

        // Copy the editable attributes from the tuple of the current item
    QmvAttList atlist = cls->edit_attlist();
    for (QmvAttribute * at_ptr = atlist.first(); at_ptr != 0; at_ptr = atlist.next())
        newtuple->update( *at_ptr->find("mtat_name"),
                          oldtuple->attributeValue(*at_ptr->find("mtat_name")));

        // Create a new item with the newtuple.
    item = new QmvItem(this, ci, newtuple, newtuple->userkeyValue());
    
    item->setText( 0, newtuple->userkeyValue());
    item->setText( 1, newtuple->userlabelValue());
    item->setOpen(TRUE);
    updateHeading();
    
    return(item);
}

void QmvList::currentSetMenu()
{
    if (!relationmenu)
        return;
    QPoint mousePos = QPoint( header()->geometry().left(),
                              header()->geometry().top() );
    relationmenu->popup( mapToGlobal(mousePos) );
}

bool QmvList::printSet()
{
    QmvClass *cls = editor->shuttleClass();
    if ( !cls || (cls->count() < 1) )
        return FALSE;
    
    QPrinter printer;
    printer.setOutputFileName( cls->relationName() + ".ps");
    printer.setOutputToFile(FALSE);
    
        // printer dialog
    if ( printer.setup(this) )
    {	
        if ( cls->printReport( &printer ) )
        {
            QMessageBox::information(0, "printSet:information",
                                     tr("Printing has completed.\n\n %1")
                                     .arg(cls->relationTitle()),
                                     "OK", 0 );
            return TRUE;
        } else {
            QMessageBox::warning( 0, "printSet:warning",
                                tr("Printing failed.\n\n %1")
                                .arg(cls->relationTitle()),
                                "OK", 0 );
            return FALSE;
        }
    } else {
	
        QMessageBox::information(0, "printSet:information",
                                 tr("Printing has been aborted.\n\n %1")
                                 .arg(cls->relationTitle()),
                                 "OK", 0 );
        return FALSE;
    }
}

bool QmvList::exportSet()
{}

// Reimplemented 
void QmvList::resizeEvent( QResizeEvent *e )
{
    QListView::resizeEvent( e );
    QSize vs = viewportSize( 0, contentsHeight() );

    int os = header()->sectionSize( 1 );
    int ns = vs.width() - header()->sectionSize( 0 );
    if ( ns < 16 )
	ns = 16;
	
    header()->resizeSection( 1, ns );
    header()->repaint( header()->width() - header()->sectionSize( 1 ),
                       0, header()->sectionSize( 1 ), header()->height() );

    int elipsis = fontMetrics().width("...") + 10;
    viewport()->repaint( header()->sectionPos(1) + os - elipsis, 0, elipsis, viewport()->height(), FALSE );
    if ( currentItem() )
	( ( QmvItem* )currentItem() )->showEditor();
}


// Reimplemented 
void QmvList::paintEmptyArea( QPainter *p, const QRect &r )
{
    p->fillRect( r, backColor2 );
}

bool QmvList::addQmvItem( QmvItem *&item, const QCString & name, QString & type )
{}



/*
 * ======================================================================
 * MEMBER FUNCTIONS: QmvItem - BASE class (for tuples)
 * ======================================================================
 */

// This base constructor is for tuples 
QmvItem::QmvItem( QmvList *ls, QmvItem *after, QmvTuple *tuple, const QString &label)
        : QListViewItem(ls, after), listview(ls)
{
        // database hooks
    shuttleclass = ls->Editor()->shuttleClass();
    shuttletuple = tuple;
    shuttletupleattribute = 0; // ie this is a tuple , not an attribute.
    
    setSelectable( FALSE );
    open = FALSE;
    resetButton = 0;
    
    
    setText( 0, shuttletuple->userkeyValue());
    setText( 1, shuttletuple->userlabelValue());

        // Editing not allowed on base - is tuple
    allowedit = FALSE;
}

QmvItem::~QmvItem()
{
    
}
//! Return the QmvTuple Object
/*!
  Return a pointer to the QmvTuple object associated with this item. This is
  either the parent tuple of the attribute, or the tuple, associated with this item.

*/
QmvTuple *QmvItem::shuttleTuple() const
{
    return shuttletuple;
}

//! Return the QmvClass Object
/*!
  Return a pointer to the QmvClass object associated with thie item.

*/
QmvClass *QmvItem::shuttleClass() const
{
    return shuttleclass;
}


//! Return the QmvTupleAttribute object
/*!
  Return a pointer to the  QmvTupleAttribute object associated with this item.
  Returns 0 if this item is a "tuple" item. This can be used to distinguish between a
  tuple (parent) item and an attribute (child) item.

*/
QmvTupleAttribute *QmvItem::shuttletupleAttribute() const
{
    return shuttletupleattribute;
}


// Reimplemented 
void QmvItem::paintCell( QPainter *p, const QColorGroup &cg,
                                 int column, int width, int alignment )
{
    QListView *listview = listView();
    
    QColorGroup g( cg );
    g.setColor( QColorGroup::Base, backgroundColor() );
    g.setColor( QColorGroup::Foreground, Qt::black );
    g.setColor( QColorGroup::Text, Qt::black );
    
    int indent = 0;
    if ( column == 0 ) {
        indent = 20 + (shuttletupleattribute  ? 20 : 0 );
        p->fillRect( 0, 0, width, height(), backgroundColor() );
        p->save();
        p->translate( indent, 0 );
    }

    if ( isChanged() && column == 0 ) {
        p->save();
        QFont f = p->font();
        f.setBold( TRUE );
        p->setFont( f );
    }

    if ( !hasCustomContents() || column != 1 ) {
        QListViewItem::paintCell( p, g, column, width - indent, alignment  );
    } else {
        p->fillRect( 0, 0, width, height(), backgroundColor() );
        drawCustomContents( p, QRect( 0, 0, width, height() ) );
    }
    
    if ( isChanged() && column == 0 )
        p->restore();
    if ( column == 0 )
        p->restore();
        // Draw +/- open/close control icon on tuple items
    if ( !shuttletupleattribute && column == 0 ) {
        p->save();
        p->setPen( cg.foreground() );
        p->setBrush( cg.base() );
        p->drawRect( 5, height() / 2 - 4, 9, 9 );
        p->drawLine( 7, height() / 2, 11, height() / 2 );
        if ( !isOpen() )
            p->drawLine( 9, height() / 2 - 2, 9, height() / 2 + 2 );
        p->restore();
    }
    
    p->save();
    p->setPen( QPen( cg.dark(), 1 ) );
    p->drawLine( 0, height() - 1, width, height() - 1 );
    p->drawLine( width - 1, 0, width - 1, height() );
    p->restore();

    if ( listview->currentItem() == this && column == 0 &&
         !listview->hasFocus() && !listview->viewport()->hasFocus() )
        paintFocus( p, cg, QRect( 0, 0, width, height() ) );


}

// Reimplemented 
void QmvItem::paintBranches( QPainter * p, const QColorGroup & cg,
				  int w, int y, int h, GUIStyle s )
{
    QColorGroup g( cg );
    g.setColor( QColorGroup::Base, backgroundColor() );
    QListViewItem::paintBranches( p, g, w, y, h, s );
}

// Reimplemented 
void QmvItem::paintFocus( QPainter *p, const QColorGroup &cg, const QRect &r )
{
    p->save();
    QApplication::style().drawPanel( p, r.x(), r.y(), r.width(), r.height(), cg, TRUE, 1 );
    p->restore();
}

//! Determine if this item has subitems
/*!

  Indicate where this item has subitems.

  \return TRUE/FALSE
*/

bool QmvItem::hasSubItems() const
{
        // attribute subitems not yet implemented
    return FALSE;
}

//! Create the child of this item
/*!
  If this item is a tuple item, then the children will be tuple attributes.
  
  TODO: The type of the QmvTupleAttribute should determine the type of item
  to create. Currently this just creates a QmvTextItem.

*/
void QmvItem::createChildren()
{
        // This will be a tuple item, so children will be tuple attributes
    
    QmvItem *item = 0;
    QmvTuple *tup = shuttleTuple();
    QmvClass *cls = tup->parentClass();
    
        // The list of editable attributes
    QmvAttList atlist = cls->edit_attlist();
        // Add the tuple attributes as children
    for (QmvAttribute * at_ptr = atlist.last(); at_ptr != 0; at_ptr = atlist.prev())
    {
        QmvTextItem * itext;
        QmvBoolItem * iboolean;
        QmvListItem * ilist;
        
        qDebug("datatype %s = %d", at_ptr->find("mtat_name")->latin1(),  at_ptr->dataType());

        switch ( at_ptr->inputMethod() )
        {
            case QmvAttribute::List:
                ilist = new QmvListItem( listview,
                                         this,
                                         tup->tupleAttribute(*at_ptr->find("mtat_name")));
                ilist->setChanged(FALSE);
                addChild( ilist );
                break;

                
            case QmvAttribute::Direct:
            default:
                
                switch ( at_ptr->dataType() )
                {
                    case QmvAttribute::Integer:
                    case QmvAttribute::Numeric:
                    case QmvAttribute::Timestamp:
                    case QmvAttribute::TypeUnknown:
                    case QmvAttribute::Text:
        
                        itext = new QmvTextItem( listview,
                                                 this,
                                                 tup->tupleAttribute(*at_ptr->find("mtat_name")));
                        itext->setChanged(FALSE);
                        addChild( itext );
                        break;

                
                    default:
                        iboolean = new QmvBoolItem( listview,
                                                    this,
                                                    tup->tupleAttribute(*at_ptr->find("mtat_name")));
                        iboolean->setChanged(FALSE);
                        addChild( iboolean );
                        break;
                }
                break;
        }
        
            
    }
    
}

//! Initialise the values of child items.
/*!
  Set the values of child items to the values found in
  QmvTupleAttribute::currentValue().

*/
void QmvItem::initChildren()
{

    QmvItem *item = 0;
    for (int i = 0; i < childCount(); ++i )
    {
        item = child(i);
        item->setValue(item->shuttletupleattribute->currentValue());
    }
    
}

void QmvItem::deleteChildren()
{
    children.setAutoDelete( TRUE );
    children.clear();
    children.setAutoDelete( FALSE );
}


//! Is this item open?
/*!
  Indicates whether this item is open or closed. 

  \return       TRUE/FALSE
*/

// Reimplemented 
bool QmvItem::isOpen() const
{
    return open;
}

//! Set the item to be open or closed
/*!
  Sets the item to be open or closed. Note that this item may
  also be a leaf item; ie without children - in this case nothing
  should happen.

  \param b      TRUE/FALSE
*/
// Reimplemented 
void QmvItem::setOpen( bool b )
{
    
        // State consistent already
    if ( b == open )
	return;
    open = b;

        // Add the children
    if ( open ) {
        createChildren();
        initChildren();
    } else {
        deleteChildren();
    }
    
    qApp->processEvents();
    listview->updateEditorSize();
}

// No editor at tuple level
void QmvItem::showEditor()
{
    if (allowedit)
    {
        createResetButton();
        resetButton->parentWidget()->show();
    }
    
}

void QmvItem::hideEditor()
{
    if (allowedit)
    {
        createResetButton();
        resetButton->parentWidget()->hide();
    }
    
}

void QmvItem::setValue( const QString &v )
{   
    val = v;
}       

QString QmvItem::value() const
{       
    return val;
}

void QmvItem::refreshValue()
{
    if (shuttletupleAttribute())
        setValue(shuttletupleAttribute()->currentValue());
}


QString QmvItem::name() const
{}

//! Propagate data changes
/*!

  If this is a tuple item, set change flag in list, otherwise
  if this is an attribute item, then set flag for changes to be
  propagated to the in-memory (QmvTupleAttribute) storage  object.

*/
void QmvItem::notifyValueChange()
{
        // tuple needs to know about all changes
    listview->setPersistent( FALSE );
        // Is this the parent tuple
    if ( !shuttletupleattribute ) {
	setChanged(FALSE);
	if ( hasSubItems() )
	    initChildren();
    } else {
	childValueChanged( this );
            // Dont update the QmvTupleAttribute yet
            // - otherwise it will update on every keystroke.
	setChanged( FALSE );
    }
}

void QmvItem::setChanged( bool update )
{
    repaint();
    if ( update )
    {
        // copy the changes back to the QmvTupleAttribute
        shuttletupleAttribute()->update(value());
    }
    updateResetButtonState();
}   
bool QmvItem::isChanged() const
{
    
    if (shuttletupleAttribute())
        return shuttletupleAttribute()->isChanged();
    else
        return shuttleTuple()->isChanged();
}       


void QmvItem::placeEditor( QWidget *w )
{
    createResetButton();
    QRect r = listview->itemRect( this );
    if ( r == QRect( 0, 0, -1, -1 ) )
	listview->ensureItemVisible( this );
    r = listview->itemRect( this );
    r.setX( listview->header()->sectionPos( 1 ) );
    r.setWidth( listview->header()->sectionSize( 1 ) - 1 );
    r.setWidth( r.width() - resetButton->width() - 2 );
    r = QRect( listview->viewportToContents( r.topLeft() ), r.size() );
    w->resize( r.size() );
    listview->moveChild( w, r.x(), r.y() );
    resetButton->parentWidget()->resize( resetButton->sizeHint().width() + 10, r.height() );
    listview->moveChild( resetButton->parentWidget(), r.x() + r.width() - 8, r.y() );
    resetButton->setFixedHeight( r.height() - 3 );
}

void QmvItem::childValueChanged( QmvItem *child )
{}

//! Add a child item to the internal list.
/*!
  Add a child to the internal list of dependent children (QmvItem::children).
  

  \param i      
*/

void QmvItem::addChild( QmvItem *i )
{
    children.append( i );
}

void QmvItem::removeChild( QmvItem *i )
{
    children.remove(i);
}

int QmvItem::childCount() const
{
    return children.count();
}

QmvItem * QmvItem::child( int i ) const
{
    return ( (QmvItem*)this )->children.at( i );
}


/*!  If the contents of the item is not displayable with a text, but
  you want to draw it yourself (using drawCustomContents()), return
  TRUE here.
*/  

bool QmvItem::hasCustomContents() const 
{       
    return FALSE;
}  
void QmvItem::drawCustomContents( QPainter *, const QRect & )
{       
}

void QmvItem::updateBackColor()
{
    if ( listView()->firstChild() == this ) {
        backColor = backColor1;
        return;
    }
    
    QListViewItemIterator it( this );
    --it;
    if ( it.current() ) {
        if ( ( ( QmvItem*)it.current() )->backColor == backColor1 )
            backColor = backColor2;
        else
            backColor = backColor1;
    } else {
        backColor == backColor1;
    }
}   

// Reimplemented 
void QmvItem::setup()
{
    QListViewItem::setup();
    setHeight( QListViewItem::height() + 2 );
}

QString QmvItem::currentItem() const
{}

int QmvItem::currentIntItem() const
{}

void QmvItem::setCurrentItem( const QString &s )
{}

void QmvItem::setCurrentItem( int i )
{}

int QmvItem::currentIntItemFromObject() const
{}

QString QmvItem::currentItemFromObject() const
{}

void QmvItem::setFocus( QWidget *w )
{
    w->setFocus();
}


void QmvItem::toggle()
{
}

QColor QmvItem::backgroundColor()
{
    updateBackColor();
    return backColor;
}   
void QmvItem::createResetButton()
{
    if ( resetButton ) {
	resetButton->parentWidget()->lower();
	return;
    }
    QHBox *hbox = new QHBox( listview->viewport() );
    hbox->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    hbox->setLineWidth( 1 );
    resetButton = new QPushButton( hbox );
    resetButton->setPixmap( QPixmap(resetproperty_xpm));
    resetButton->setFixedWidth( resetButton->sizeHint().width() );
    hbox->layout()->setAlignment( Qt::AlignRight );
    listview->addChild( hbox );
    hbox->hide();
    QObject::connect( resetButton, SIGNAL( clicked() ),
		      listview, SLOT( resetAttribute() ) );
    QToolTip::add( resetButton, QmvEditor::tr( "Reset attribute to its default value" ) );
    QWhatsThis::add( resetButton,
                     QmvEditor::tr( "Click this button to reset the Attribute"
                                           "to its default value" ) );
    updateResetButtonState();
}

void QmvItem::updateResetButtonState()
{
    if ( !resetButton )
	return;
    if ( shuttletupleAttribute() )
	resetButton->setEnabled( isChanged() );
//      else
//  	resetButton->setEnabled(  );
}

/*
 * ======================================================================
 * MEMBER FUNCTIONS: QmvTextItem
 * ======================================================================
 */
QmvTextItem::QmvTextItem( QmvList * cls, QmvItem *after, QmvTupleAttribute *attribute, const QString &label,
                          bool comment, bool multiLine, bool ascii, bool a)
    : QmvItem(cls, after, attribute->parentTuple(), label), withComment( comment ),
      hasMultiLines( multiLine ), asciiOnly( ascii ), accel( a )
{
    lin = 0;
    box = 0;
    shuttletupleattribute = attribute;
    setText( 0, shuttletupleattribute->attributeTitle());
    setText( 1, shuttletupleattribute->currentValue());

    allowedit = TRUE;
}

QmvTextItem::~QmvTextItem()
{
    delete (QLineEdit*)lin;
    lin = 0;
    delete (QHBox*)box;
    box = 0;
}

void QmvTextItem::createChildren()
{
}

void QmvTextItem::initChildren()
{
        
}

void QmvTextItem::showEditor()
{
    QmvItem::showEditor();
    if ( !lin ) {
	lined()->blockSignals( TRUE );
	lined()->setText( value() );
	lined()->blockSignals( FALSE );
    }
    QWidget* w;
    if ( hasMultiLines )
	w = box;
    else
	w= lined();

    placeEditor( w );
    if ( !w->isVisible() || !lined()->hasFocus() ) {
	w->show();
	setFocus( lined() );
    }
}

void QmvTextItem::hideEditor()
{
    QmvItem::hideEditor();
    QWidget* w;
    if ( hasMultiLines )
	w = box;
    else
	w = lined();

    w->hide();
}

void QmvTextItem::setValue( const QString & v )
{
    if ( ( !hasSubItems() || !isOpen() )
	 && value() == v )
	return;
    if ( lin ) {
	lined()->blockSignals( TRUE );
	int oldCursorPos;
	oldCursorPos = lin->cursorPosition();
	lined()->setText( v );
	lin->setCursorPosition( oldCursorPos );
	lined()->blockSignals( FALSE );
    }
    setText( 1, v);
    QmvItem::setValue( v );
}

//! Determine if this item has subitems
/*!

  Indicate where this item has subitems.

  \return TRUE/FALSE
*/

bool QmvTextItem::hasSubItems() const
{
        // Not yet implemented
    return FALSE;
}

//! Update in-memory storage with changed vaue
/*!
  Copy the new data vale to the QmvTupleAttribute object. Note
  that this does not update the persistant storage.
  \param child */

void QmvTextItem::childValueChanged( QmvItem *child )
{
        // copy the changes back to the QmvTupleAttribute
    shuttletupleAttribute()->update(child->value());
}

void QmvTextItem::setChanged( bool update)
{
    QmvItem::setChanged( update );
    if ( withComment && childCount() > 0 )
	( (QmvTextItem*)QmvItem::child( 0 ) )->lined()->setEnabled( shuttletupleattribute->isChanged() );
}

void QmvTextItem::setValue()
{
    setText( 1, lined()->text() );
    QString v;
    if ( accel )
	v = QAccel::stringToKey( lined()->text() );
    else
	v = lined()->text();
    QmvItem::setValue( v );
    notifyValueChange();
}

void QmvTextItem::getText()
{
    QString txt = QString("Hello there"); //TextEditor::getText( listview, value().toString() );
    if ( !txt.isEmpty() ) {
	setText( 1, txt );
	QmvItem::setValue( txt );
	notifyValueChange();
	lined()->blockSignals( TRUE );
	lined()->setText( txt );
	lined()->blockSignals( FALSE );
    }
}

QLineEdit * QmvTextItem::lined()
{
    if ( lin )
	return lin;
    if ( hasMultiLines ) {
	box = new QHBox( listview->viewport() );
	box->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	box->setLineWidth( 2 );
	box->hide();
    }

    lin = 0;
    if ( hasMultiLines )
	lin = new QLineEdit( box );
    else
	lin = new QLineEdit( listview->viewport() );

    if ( asciiOnly )
	lin->setValidator( new AsciiValidator( lin, "ascii_validator" ) );
    if ( !hasMultiLines ) {
	lin->hide();
    } else {
	button = new QPushButton( tr("..."), box );
	button->setFixedWidth( 20 );
	connect( button, SIGNAL( clicked() ),
		 this, SLOT( getText() ) );
	lin->setFrame( FALSE );
    }
    connect( lin, SIGNAL( returnPressed() ),
	     this, SLOT( setValue() ) );
    connect( lin, SIGNAL( textChanged( const QString & ) ),
	     this, SLOT( setValue() ) );
    lin->installEventFilter( listview );
    return lin;
}

/*
 * ======================================================================
 * CLASS DEFINITION - QmvBoolItem
 * ======================================================================
 */
QmvBoolItem::QmvBoolItem( QmvList * cls, QmvItem *after, QmvTupleAttribute *attribute, const QString &label)
    : QmvItem(cls, after, attribute->parentTuple(), label)
{
    comb = 0;
    
    shuttletupleattribute = attribute;
    setText( 0, shuttletupleattribute->attributeTitle());
    setText( 1, shuttletupleattribute->currentValue());

    str_true = QString( "0" );
    str_false = QString( "1" );
    lab_true = tr( "True" );
    lab_false = tr( "False" );
    
    allowedit = TRUE;
}

QmvBoolItem::~QmvBoolItem()
{
    delete (QComboBox*)comb;
    comb = 0;
}

QComboBox *QmvBoolItem::combo()
{
    if ( comb )
	return comb;
    comb = new QComboBox( FALSE, listview->viewport() );
    comb->hide();
    comb->insertItem( lab_false );
    comb->insertItem( lab_true );
    connect( comb, SIGNAL( activated( int ) ),
	     this, SLOT( setValue() ) );
    comb->installEventFilter( listview );
    return comb;
}

void QmvBoolItem::createChildren()
{
}

void QmvBoolItem::initChildren()
{
        
}

void QmvBoolItem::showEditor()
{
    QmvItem::showEditor();
    if ( !comb ) {
	combo()->blockSignals( TRUE );
	if ( value() == str_true )
	    combo()->setCurrentItem( 0 );
	else
	    combo()->setCurrentItem( 1 );
	combo()->blockSignals( FALSE );
    }
    placeEditor( combo() );
    if ( !combo()->isVisible()  || !combo()->hasFocus() ) {
	combo()->show();
	setFocus( combo() );
    }
}

void QmvBoolItem::hideEditor()
{
    QmvItem::hideEditor();
    combo()->hide();
}

void QmvBoolItem::setValue( const QString & v )
{
    if ( ( !hasSubItems() || !isOpen() )
	 && value() == v )
	return;

    if ( comb ) {
	combo()->blockSignals( TRUE );
	if ( v == str_true )
	    combo()->setCurrentItem( 1 );
	else
	    combo()->setCurrentItem( 0 );
	combo()->blockSignals( FALSE );
    }
    
    setText( 1, v );
    QmvItem::setValue( v );
}

void QmvBoolItem::toggle()
{
    if ( value() == str_false )
        setValue( str_true );
    else
        setValue( str_true );

    setValue();
}

void QmvBoolItem::setValue()
{
    if ( !comb )
	return;
    setText( 1, combo()->currentText() );
    if ( combo()->currentItem() == 0 )
        QmvItem::setValue( str_true );
    else
        QmvItem::setValue( str_false );
    notifyValueChange();
}
/*
 * ======================================================================
 * MEMBER FUNCTIONS: QmvListItem
 * ======================================================================
 */

QmvListItem::QmvListItem( QmvList * cls, QmvItem *after, QmvTupleAttribute *attribute,
                          const QString &label = 0 )

    : QmvItem(cls, after, attribute->parentTuple(), label)
{
    comb = 0;
    oldInt = -1;
    
    shuttletupleattribute = attribute;
    setText( 0, shuttletupleattribute->attributeTitle());
    setText( 1, shuttletupleattribute->currentValue());
    
    allowedit = TRUE;
}


QmvListItem::~QmvListItem()
{
    delete (QComboBox*)comb;
    comb = 0;
}


QComboBox *QmvListItem::combo()
{
    if ( comb )
	return comb;
    comb = new QComboBox( FALSE, listview->viewport() );
    comb->hide();

    connect( comb, SIGNAL( activated( int ) ),
	     this, SLOT( setValue() ) );
    comb->installEventFilter( listview );
    combo()->blockSignals( TRUE );
    combo()->clear();
    combo()->listBox()->setColumnMode(1);

        // Load from foreign class of this attribute.
    const QString & att = shuttletupleattribute->attributeName();
    
    QmvClass * fk_class = shuttleclass->foreignRelation( att );

    if (fk_class)
        for (int i = 0; i < fk_class->count(); i++)
            comb->insertItem( fk_class->attributeValue( i, shuttleclass->foreignView( att ) ), i );
    
    combo()->blockSignals( FALSE );
    return comb;
}

void QmvListItem::createChildren()
{
}

void QmvListItem::initChildren()
{}

void QmvListItem::showEditor()
{
    QmvItem::showEditor();
    if ( !comb )
        combo();
    
    placeEditor( combo() );
    if ( !combo()->isVisible()  || !combo()->hasFocus() ) {
	combo()->show();
	setFocus( combo() );
    }
}

void QmvListItem::hideEditor()
{
    QmvItem::hideEditor();
    combo()->hide();
}

void QmvListItem::setValue( const QString &v )
{
    if ( !comb )
	return;
    setText( 1, combo()->currentText() );
    QmvItem::setValue( combo()->currentText() );
    notifyValueChange();
    oldInt = currentIntItem();
    oldString = currentItem();
}

void QmvListItem::setValue( uint i )
{
    
}

void QmvListItem::setValue(  )
{
    
}

//! Return the text of selected item
/*!


  \return       text of selected item
*/
QString QmvListItem::currentItem() const
{
    return ( (QmvListItem *)this )->combo()->currentText();
}

//! Get the index of seleced item
/*!
  

  \return       index of selected item
*/
int QmvListItem::currentIntItem() const
{
    return ( (QmvListItem *)this )->combo()->currentItem();
}

// TODO: are either of these methods (setCurrentItem) used??
//! Set the selected list item
/*!
  Set the selected item in the list to \em s

  \param s      String to find in list
*/
void QmvListItem::setCurrentItem( const QString &s )
{
    if ( comb && currentItem().lower() == s.lower() )
 	return;

    if ( !comb )
	combo();
        
    for ( uint i = 0; i < combo()->listBox()->count(); ++i ) {
	if ( combo()->listBox()->item( i )->text().lower() == s.lower() ) {
	    setCurrentItem( i );
	    break;
	}
    }
}

//! Set the selected list item
/*!
  Set the selected item in the list to index \em i

  \param i      index of list item to select
*/
void QmvListItem::setCurrentItem( int i )
{
    if ( comb && i == combo()->currentItem() )
	return;

    if ( !comb )
	combo();
    
    combo()->setCurrentItem( i );
    setText( 1, combo()->currentText() );
    oldInt = currentIntItem();
    oldString = currentItem();
}

int QmvListItem::currentIntItemFromObject() const
{
    return oldInt;
}

QString QmvListItem::currentItemFromObject() const
{
    return oldString;
}

/*
 * ======================================================================
 * MEMBER FUNCTIONS: QmvEditor
 * ======================================================================
 */

QmvEditor::QmvEditor( QWidget *parent, QmvClass * cls )
    : QVBox( parent, "editor", WStyle_Customize | WStyle_NormalBorder | WStyle_Title |
			WStyle_StaysOnTop | WStyle_Tool |WStyle_MinMax | WStyle_SysMenu )
{
    listview = new QmvList( this );
    if (cls)
        setshuttleClass(cls);
    else
    {
        setCaption( tr( "Tuple Editor" ) );
        shuttlecls = 0;
    }
    qApp->processEvents();
    listview->updateEditorSize();
    
}

// --------------------------------------------------------------------------------
QmvEditor::~QmvEditor()
{
}

//! Return the current relation (shuttleclass)
/*!
  Return a pointer to the relation associated with this editor

*/
// --------------------------------------------------------------------------------
QmvClass * QmvEditor::shuttleClass() const
{
    return shuttlecls;
}

//! Set the associated relation
/*!
  Associate an relation object with this editor object

  \param c      a pointer to a QmvClass object
*/
// --------------------------------------------------------------------------------
void QmvEditor::setshuttleClass (QmvClass *c)
{
        // Null
    if (!c)
    {
        setCaption( tr("Editor - blank") );
        clear();
        shuttlecls = 0;
        return;
    }
        // Setup a new class
    shuttlecls = c;
    setCaption( tr("Editor (%1)").arg(c->relationName()) );
    listview->viewport()->setUpdatesEnabled( FALSE );
    listview->setUpdatesEnabled( FALSE );
    clear();
    listview->viewport()->setUpdatesEnabled( TRUE );
    listview->setUpdatesEnabled( TRUE );
    setup();
    listview->updateHeading();
    listview->triggerUpdate();
    
}
    
//! Clear the listview object
/*!
  Clear the editors listview.

*/
// --------------------------------------------------------------------------------
void QmvEditor::clear()
{
    listview->setContentsPos( 0, 0 );
    listview->clear();
}

//! Initialise this editor.
/*!
  Enable updates, process events and resize the widget

*/
// --------------------------------------------------------------------------------
void QmvEditor::setup()
{
    listview->viewport()->setUpdatesEnabled( FALSE );
    listview->setupTuples();
    listview->viewport()->setUpdatesEnabled( TRUE );
    qApp->processEvents();
    listview->updateEditorSize();
}

//! Reload the current dataset.
/*!
  

*/
// --------------------------------------------------------------------------------
void QmvEditor::refetchData()
{
    listview->refetchData();
}

// --------------------------------------------------------------------------------
void QmvEditor::emitWidgetChanged()
{

}

// --------------------------------------------------------------------------------
void QmvEditor::closeEvent( QCloseEvent *e )
{
    emit hidden();
    e->accept();
}

// --------------------------------------------------------------------------------
QmvList *QmvEditor::tupleList() const
{
    return listview;
}

// --------------------------------------------------------------------------------
QString QmvEditor::classOfCurrentTuple() const
{
}

// --------------------------------------------------------------------------------
QmvAttribute * QmvEditor::metaObjectOfCurrentAttribute() const
{
   
}

// --------------------------------------------------------------------------------
void QmvEditor::resetFocus()
{
    if ( listview->currentItem() )
	( (QmvItem*)listview->currentItem() )->showEditor();
}

