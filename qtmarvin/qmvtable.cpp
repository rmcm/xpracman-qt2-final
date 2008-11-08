/*
 *     $Id: qmvtable.cpp 3208 2006-11-18 02:40:38Z source $
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
//#define QMVTABLE_DEBUG
#include <qcolordialog.h>
#include <qtimer.h>
#include "qmvtable.h"
#include "qmvmultilineedit.h"
#include "qmvtswidget.h"
#include "qmvpopup.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
static char qmvtable_version[] = "$Id: qmvtable.cpp 3208 2006-11-18 02:40:38Z source $";

/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */


/*
 * ======================================================================
 * Local class: QmvTableToolTip
 * ======================================================================
 */
    
class QmvTableToolTip : public QToolTip
{
    
      public:
    QmvTableToolTip( QmvTable * parent_table, QToolTipGroup * g = 0)
            : QToolTip( parent_table->viewport(), g ), table(parent_table)
        {};
    virtual ~QmvTableToolTip(){};

      protected:      
    void maybeTip ( const QPoint & mousePos )
        {
            if (!table || !table->tooltips_flag) return;
            QPoint p = table->viewportToContents(mousePos);
            int row = table->rowAt( p.y() );
            int col = table->columnAt( p.x() );
            QString str = table->getDetails( row, col, false );
            if ( str.length() < 1 ) return;
            QRect rect = table->cellGeometry( row, col );
            rect.moveTopLeft( table->contentsToViewport(rect.topLeft()) );
            tip( rect, str );
        };

      private:
    QmvTable * table;
};


/*
 * ======================================================================
 * Local class: QmvTableWhatsThis
 * ======================================================================
 */
    
class QmvTableWhatsThis : public QWhatsThis
{
      public:
    QmvTableWhatsThis( QmvTable * parent_table)
            : QWhatsThis( parent_table->viewport() ), table(parent_table)
        {};
    virtual ~QmvTableWhatsThis(){};

      protected:      
    QString text( const QPoint & mousePos )
        {
            if (!table) return QString::null;
            QPoint p = table->viewportToContents(mousePos);
            int row = table->rowAt( p.y() );
            int col = table->columnAt( p.x() );
            return table->getDetails( row, col, true );
        };

      private:
    QmvTable * table;
};
/*
 * ======================================================================
 * MEMBER FUNCTIONS: QmvTable
 * ======================================================================
 */

// Constructors

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
QmvTable::QmvTable( QWidget * parent, const char * name ) 
        : QTable( parent, name ),
          init_flag(false), save_mode(true), tooltips_flag(true), tt_button(0),
          corner_widget(0), dc_timer(0)
{
    init();
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
QmvTable::QmvTable( int numRows, int numCols, QWidget * parent, const char * name )
        : QTable( numRows, numCols, parent, name ),
          init_flag(false), save_mode(true), tooltips_flag(true), tt_button(0),
          corner_widget(0), dc_timer(0)
{
    init();
}

// Destructor

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
QmvTable::~QmvTable()
{
    if (dc_timer) delete dc_timer;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTable::init()
{
        // only run once
    if (init_flag) return;
    init_flag = true;
    
    setFocusPolicy(QWidget::WheelFocus);
    setFocus();

    connect( this, SIGNAL( clicked( int, int, int, const QPoint & ) ),
             this, SLOT( cellClicked( int, int, int, const QPoint &  ) ) );
    
        // double clicks are always preceded by mousepress and release
        // so use a timer to wait for a following double click
    dc_timer = new QTimer( this, "double click timer" );
    connect( dc_timer, SIGNAL( timeout() ), this, SLOT( realSingleClick() ) );

        // Create a local QWhatsThis object
    new QmvTableWhatsThis( this );
        // Create a local QToolTip object
    new QmvTableToolTip( this, 0 );

        // add a corner widget for controls
    if ( corner_widget) delete corner_widget;
    corner_widget = new QVBox(this);
    corner_widget->setFrameStyle( QFrame::Box | QFrame::Plain );
    corner_widget->setLineWidth(2);
    corner_widget->setMidLineWidth(0);
    corner_widget->setSpacing(0);

        // add box for control buttons to the corner widget
    QHBox * cw_controls = new QHBox( corner_widget, "control buttons");
    corner_widget->setStretchFactor( cw_controls, 0 );

            // Add Tooltips Toolbutton
    QWhatsThis::whatsThisButton(cw_controls);
    tt_button = new QToolButton(cw_controls, "Tooltips toggle");
    tt_button->setFocusPolicy( QWidget::NoFocus );
    QmvIcons stdicons( this, "new_icons" );
    QFont std_font = QFont("Helvetica", 12, QFont::Bold);
    QPixmap std_pm =  stdicons.getPixmap( QmvIcons::BlankPageIcon );
    QColor std_colour = QColor(Qt::blue);
    QIconSet tooltips_icon(
        *(stdicons.makeTextPixmap( new QPixmap(std_pm ), tr("T"), std_font, std_colour, QRect() ) ));
    tt_button->setIconSet( tooltips_icon );
    tt_button->setTextLabel( "Activate/Deactivate foreign-key tool-tips" );
    tt_button->setToggleButton(true);
    tt_button->setOn(true);
    tooltips_flag = true;
    QWhatsThis::add( tt_button, "<h3>Tool tips:</h3>"
                     "<ol><li>Pressed: Display more information in tooltips</li>"
                     "<li>Raised: Disable tooltips</li>"
                     "</ol>");
    connect( tt_button, SIGNAL( toggled( bool ) ), this, SLOT( slotSetToolTips( bool ) ) );
    tt_button->setAccel( CTRL+ALT+Key_T );
    
    updateCornerWidget();
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTable::slotSetToolTips( bool tooltips )
{
    tooltips_flag = tooltips;
    if( tooltips_flag)
    {
        tt_button->setOn(true);
        emit statusMessage( tr("Foreign-key tooltips are now active.") );
    } else {
        tt_button->setOn(false);
        emit statusMessage( tr("Foreign-key tooltips have been disabled.") );
    }
}


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTable::updateCornerWidget()
{
        // Adjust the headers if necessary
    if ( verticalHeader()->isVisible()
         && verticalHeader()->width() < corner_widget->minimumWidth() )
        verticalHeader()->setFixedWidth( corner_widget->minimumHeight() );
    if ( horizontalHeader()->isVisible()
         && horizontalHeader()->height() < corner_widget->minimumHeight() )
        horizontalHeader()->setFixedHeight( corner_widget->minimumHeight() );

    int w = 0,h = 0;
    if ( verticalHeader()->isVisible() )
        w = verticalHeader()->width();
    if ( horizontalHeader()->isVisible() )
        h = horizontalHeader()->height();
    QSize corner_size( w, h  );
    corner_widget->setFixedSize( corner_size );
    corner_widget->resize(corner_size);
}


// ----------------------------------------------------------------------
//! Reimplement General Event Filter
/*!
  Reimplemented from \ref QTable, API effected by mapping;

  - ALT-RETURN to a single left click
  - RETURN to "activateNextCell() method.
  
  \param o      Object
  \param e      Event
  \return Status of local handler
*/
// ----------------------------------------------------------------------
bool QmvTable::eventFilter( QObject *o, QEvent *e )
{
#ifdef QMVTABLE_DEBUG
    const char *name = o ? o->name() : "NO_OBJECT";
    const char *classname = o ? o->className() : "NO_CLASS";
    int etype = e ? e->type() : -9999;
//    qDebug( "%-20.20s::eventFilter(%20.20s/%20.20s, %6d)", "QmvTable", name, classname, etype );
#endif
    // test disabing this eventfilter while tracing crash - Issue#324
    // return QTable::eventFilter( o, e );
    if ( !o || !e )
  	return true;

    if ( e->type() == QEvent::KeyPress )
    {
        QKeyEvent *ke = (QKeyEvent *) e;

            // keyboard row select.
        if ( ke->key() == Key_Return || ke->key() == Key_Enter )
            if  ( ke->state() & AltButton )
            {
                    // the mouse point is a fake - TODO: decide what to make it.
                emit clicked( currentRow(), currentColumn(), 1, QPoint(0,0) );
                return true;
            } else {
                activateNextCell();
                return true;
            }
        
    }

        // focus out - do nothing
        // TODO:  popup a reminder to do something
    if ( e->type() == QEvent::FocusOut )
        return true;
    
    return QTable::eventFilter( o, e );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTable::keyPressEvent( QKeyEvent *ke )
{
#ifdef QMVTABLE_DEBUG
    int ktype = ke ? ke->key() : -9999;
    qDebug( "%-20.20s::keyPressEvent::%6d", "QmvTable", ktype );
#endif

        // finish and move to next cell
    if ( ke->key() == Key_Return || ke->key() == Key_Enter )
        activateNextCell();

        // Finish
    if ( ke->key() == Key_Escape )
    {
        editCompleted();
        ke->accept();   // finish here.
    }
    
        // navigation, in-place editing etc
    QTable::keyPressEvent ( ke );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTable::focusOutEvent( QFocusEvent *e )
{
    QTable::focusOutEvent( e );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTable::focusInEvent( QFocusEvent *e )
{
    QTable::focusInEvent( e );
}

// ----------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTable::contentsMousePressEvent( QMouseEvent * mre )
{
        // are we moving to a new cell?
    is_current_cell = ( rowAt(mre->y()) == currentRow() )
        && (columnAt(mre->x()) == currentColumn() );
    
        // The order of events is PRESS/RELEASE/DOUBLECLICK/RELEASE
        // Flag the press, to differentiate the susequent release events.
    first_click = true;
    QTable::contentsMousePressEvent( mre );
}

// ----------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTable::contentsMouseReleaseEvent( QMouseEvent * mre )
{
        // remember the mouse event - used later in realSingleClick()
    hold_mre.type = mre->type();
    hold_mre.pos = mre->pos();
    hold_mre.globalPos = mre->globalPos();
    hold_mre.button = mre->button();
    hold_mre.state = mre->state();
    
       // single or double clicked ... currently is a single click, but
        // there may be a double click in the pipeline, so set the timer
        // Only left-double-click is relevant.
    if ( first_click )
    {
        if (mre->button() == Qt::LeftButton )
            dc_timer->start(QApplication::doubleClickInterval(), true);
        else
            realSingleClick();
    }
    
        // reset
    first_click = false;

    return;
}

// --------------------------------------------------------------------------------
//! DoubleClick handler
/*!
  Reimplementation to better separate doubleclick and singleclick

  \param mre    The mouse event
*/
// --------------------------------------------------------------------------------
void QmvTable::contentsMouseDoubleClickEvent( QMouseEvent * mre )
{
        // stop the singleclick timer, a double click arrived.
    dc_timer->stop();

        // Call the standard handlers
    QTable::contentsMouseDoubleClickEvent( mre );
    QTable::contentsMouseReleaseEvent( mre );
}


// --------------------------------------------------------------------------------
//! Single Click Event Handler.
/*!
  A separate handler for single clicks, called from a QTimer after the first left
  press, unless a double click intervenes.

*/
// --------------------------------------------------------------------------------
void QmvTable::realSingleClick()
{
        // Build an event from the stored press event and dispatch to the standard
        // QTable handler.
    QMouseEvent * mre = new QMouseEvent( hold_mre.type, hold_mre.pos, hold_mre.globalPos, hold_mre.button, hold_mre.state);
    QTable::contentsMouseReleaseEvent( mre );
    delete mre;

        // QTable::contentsMouseReleaseEvent() filters out non-left buttonclicks.
        // so send the signal now.
    if ( mre->button() != LeftButton )
        emit clicked( rowAt(hold_mre.pos.y()),
                      columnAt(hold_mre.pos.x()),
                      hold_mre.button,
                      mapFromGlobal( hold_mre.globalPos ) );
    
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTable::setCellContentFromEditor( int row, int col)
{
        // this would be invoked it cell content was replaceable and typing commenced.
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTable::activateNextCell()
{
    
    int col = currentColumn() + 1;
    int row = currentRow();

    bool eot = false;
    
    if ( col >= numCols() )
        if ( row >= numRows() )
            col--;
        else
        {
            col = 0;
            if ( row < (numRows() - 1) )
                row++;
            else
                eot = true;
        }

    setCurrentCell( row, col );
        // if we are in the last cell of table, the edit will not
        // automatically end the edit.
    if ( eot )
        editCompleted();
    
    ensureCellVisible( row, col );
}

// ----------------------------------------------------------------------
//! 
/*!
  <long-description>

  \param row    
  \param col    
*/
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTable::cellClicked( int row, int col, int button, const QPoint & mousePos)
{
    if ( !cellWidget( row, col ) )
        setFocus();
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTable::redisplayRow( int row, int col )
{
        // the args (row and col) refer to the cell, not the QmvClass row
    emit reloadRequest( row, col );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTable::editCompleted()
{
    editCompleted( currentRow(), currentColumn() );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTable::editCompleted( int row, int col )
{
    endEdit( row, col, TRUE, FALSE );
}


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTable::paintCell ( QPainter * p, int row, int col,
                          const QRect & cr, bool selected )
{
    QTable::paintCell ( p, row, col, cr, selected );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTable::paintFocus ( QPainter * p, const QRect & cr )
{
    QTable::paintFocus( p, cr );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
bool QmvTable::saveMode() const
{
    return save_mode;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTable::setSaveMode( bool mode )
{
    save_mode = mode;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
const QString QmvTable::getDetails( const int row, const int col, bool fk ) const
{
    QmvTableItem * tab_item = (QmvTableItem *) item( row, col );
    if ( !tab_item )
        return QString::null;
    
    QmvTupleAttribute * tup_att = tab_item->getTupleAttribute();
    if ( !tup_att )
        return QString::null;

        // simple details
    if (! fk )
        return tup_att->parentTuple()->userLabelValue();

        // Details of FK.
    QmvSqlClass * fk_class = ( QmvSqlClass *) tup_att->loadForeignKey();
    if ( !fk_class )
        return tup_att->parentTuple()->userLabelValue();
    
    return QString("<b>%1</b><br><hr>%2")
        .arg(tup_att->parentTuple()->userLabelValue())
        .arg(fk_class->htmlLabel(0));
}

/*
 * ======================================================================
 * MEMBER FUNCTIONS: QmvTableItem
 * ======================================================================
 */
// Constructor

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
QmvTableItem::QmvTableItem( QmvTable * table, EditType et, QmvTupleAttribute * tat, const QPixmap & p ) 
        : QTableItem( table, et, tat->currentValue() ),
          parent_table(table), tupatt(tat), editor_type(MultiLineEdit),
          dt_flags(0), edit_prompting(false)
{
    switch ( tupatt->metaAttribute()->inputMethod() )
    {
        case QmvAttribute::InputUnknown:
        case QmvAttribute::Direct:
            editor_type = MultiLineEdit;
            break;
            
        case QmvAttribute::List:
                // Don't translate  value to label here - too slow
            editor_type = ComboBox;
            break;
            
        case QmvAttribute::ReadOnly:
            editor_type = NoEdit;
            break;
            
        case QmvAttribute::Colour:
            editor_type = ColorDialog;
            break;

                // Datetime family
        case QmvAttribute::DateTime:
            dt_flags = QmvTSWidget::DateTimeMode;
        case QmvAttribute::DateOnly:
            dt_flags = QmvTSWidget::DateMode;
        case QmvAttribute::TimeOnly:
            dt_flags = QmvTSWidget::TimeMode;
        case QmvAttribute::DateNoText:
            dt_flags = QmvTSWidget::DateMode | QmvTSWidget::DateNoText;
            editor_type = DateTime;
            break;

                // Combined text copy - without FK
        case QmvAttribute::CopyText:
            editor_type = CopyText;
            break;
            
        default:
            editor_type = MultiLineEdit;
            break;
    }

        // Check for edit-prompting
    edit_prompting = tupatt->metaAttribute()->getEditPrompting();

    
        // protect the editor
    setReplaceable( FALSE );
        // TODO do something with the pixmap
}

// ----------------------------------------------------------------------
// Destructor

QmvTableItem::~QmvTableItem()
{
  
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
QWidget* QmvTableItem::createEditor() const
{
    QWidget * w;
    
    switch ( editor_type )
    {
        case NoEdit:
            w = 0;
            break;

        case ColorDialog:
            setColor();
            w = 0;
            break;

        case DateTime:
            w  = new QmvTSWidget( parent_table->viewport(), "testing", dt_flags );
                //w->setFixedHeight( 22 );
             (( QmvTSWidget * ) w)->setDateTime( QmvClass::strToDateTime( tupatt->currentValue()) );
            break;

        case CopyText:
        case LineEdit:
        case ComboBox:
        case MultiLineEdit:
        default:
            w = new QmvMultiLineEdit( tupatt, parent_table->viewport(), "qmvtableitem-editor" );
            break;
    }

    return w;
}

// ----------------------------------------------------------------------
//! Copy the cell editor content to QmvTableItem
/*!
  This function is called when the editor (\em w ) has completed.
  The persistent storage is updated, and the display text is updated. 

  \param w      
*/
// ----------------------------------------------------------------------
void QmvTableItem::setContentFromEditor( QWidget * w )
{
    QString newvalue;
    if (!w)
        return;

    QString oldvalue = tupatt->currentValue();
    if ( edit_prompting && !oldvalue.isNull() && oldvalue.length() > 0 &&
         QMessageBox::information( parent_table, "setContentFromEditor:Save",
                                   QString("Are you sure that you want to alter this value \n\n %1 : %2")
                                   .arg( "old" )
                                   .arg( "new" ),
                                   "Yes", "Cancel",
                                   0, 1 ) != 0 ) {
        return;
    }
        
    QmvMultiLineEdit::SaveMode mode = QmvMultiLineEdit::SaveIfChanged;
    
    switch ( editor_type )
    {
        case DateTime:
            newvalue = setDateTime( (( QmvTSWidget * ) w)->getDateTime() );
            break;

        case CopyText:
            newvalue = ( (QmvMultiLineEdit *) w)->text();
            setTexTCopy(newvalue);      // do the replacements
                // update if mode is set.
            newvalue = text();          // setTexTCopy updates if possible.
            if ( parent_table->saveMode() )
                tupatt->parentTuple()->save();
            parent_table->updateCell( row(), col() );
            break;
            
        case LineEdit:
        case ComboBox:
        case MultiLineEdit:
        default:
            if ( parent_table->saveMode() )
                mode = QmvMultiLineEdit::SaveIfChanged;
            else
                mode = QmvMultiLineEdit::NeverSave;
            
            ( (QmvMultiLineEdit *) w)->save( mode ); // attempt a save
            newvalue = ( (QmvMultiLineEdit *) w)->text(); // perhaps the save altered the value
    }
            
        // return the value to the tableitem

    setText( newvalue );
    parent_table->redisplayRow( row(), col() );
    
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
bool QmvTableItem::itemHasChanged( QmvTupleAttribute * newatt )
{
    if ( !tupatt || !newatt )
        return false;

    QString curval = text();
    
        // If a list, then convert from label to value.
    if ( tupatt->metaAttribute()->inputMethod() == QmvAttribute::List )
        curval =  tupatt->getValueByLabel( curval );
    
        // compare
    if ( newatt->currentValue() == curval )
        return false;
    
    return true;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
bool QmvTableItem::refreshText()
{
    if ( tupatt->currentValue() != text() )
    {
        setText( tupatt->currentValue() );
        return TRUE;
    } else
        return FALSE;
    
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTableItem::paint( QPainter * p, const QColorGroup & cg,
                          const QRect & cr, bool selected )
{
    
    if ( !tupatt )
        return;

        // Save colour group
    QColorGroup cg_new(cg);
        // Look for local tupleattribute colour group.
    if ( tupatt->isColourGroupSet() )
        cg_new = tupatt->colourGroup();
    
    QColor colour;
    
        // Use colour in place of text.
    if ( editor_type == ColorDialog )
    {
        QString str = tupatt->currentValue();
        bool ok;
        QRgb rgba = str.replace( QRegExp("#"), "").toUInt( &ok, 16);
        if ( !ok )
            rgba = QColor().rgb();

            // strip the alpha value
        rgba %= 0x1000000;
        
        colour.setRgb( rgba );
        cg_new.setColor( QColorGroup::Base, colour );
        cg_new.setColor( QColorGroup::Text, colour );
        cg_new.setColor( QColorGroup::Highlight, colour );
        cg_new.setColor( QColorGroup::HighlightedText, colour );
    }
    
        

    
    QTableItem::paint( p, cg_new, cr, selected );
    
//      p->setFont( forig );

}

// ----------------------------------------------------------------------
//! Set a colour value
/*!
  A pseudo-editor using QColorDialog.

*/
// ----------------------------------------------------------------------
void QmvTableItem::setColor() const
{
    QString str = tupatt->currentValue();
    bool ok;
    QRgb rgba = str.replace( QRegExp("#"), "").toUInt( &ok, 16);
    if ( !ok )
        rgba = QColor().rgb();

        // allow specification of alpha value and colour
    QRgb new_rgba = QColorDialog::getRgba( rgba, &ok, parent_table, "colordialog" );
    if ( ok )
    {
        QString col_str;
        col_str.setNum( new_rgba, 16 );
        col_str = col_str.rightJustify(8, '0');
        col_str.prepend("#");
        tupatt->update( col_str );
        if ( parent_table->saveMode() )
            tupatt->parentTuple()->save();
        parent_table->updateCell( row(), col() );
    }
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void QmvTableItem::setTexTCopy( const QString & key )
{
    const QmvAttribute::copyTextMeta & copytext = tupatt->metaAttribute()->copyText();
    QmvSqlClass * x_reln = copytext.reln;
    if (!x_reln)
    {
        x_reln = (QmvSqlClass *) tupatt->parentTuple()->parentClass()->dataBase()->dbRelation( copytext.name );
        if (!x_reln)
            return;         // no search object
    }
    
    
    x_reln->open( QString ( "select * from %1 where %2 ~* '%3' LIMIT %4" )
                         .arg(copytext.name)
                         .arg(copytext.key)
                         .arg(key)
                         .arg(x_reln->queryLimit()) );
    int count = x_reln->count();
    if ( count <= 0 )
    {
            // nothing found, or an error
            // use the text entered.
        tupatt->update(key);
        setText(key);
        return;
    }
    
    
    int x_row = 0;
    if ( count > 1 )
    {
        QStringList labels;
        labels.append(copytext.key);
        labels.append(copytext.src);
  
        QmvSelect ct_select( x_reln, labels, parent_table, "copy selector" );
        x_row = ct_select.exec();
    } else
        x_row = 0;
    if ( x_row >= 0 && x_row < count )
    {
            // copy
        tupatt->parentTuple()->update( copytext.dst, x_reln->attributeValue(x_row, copytext.src) );
        tupatt->update(x_reln->attributeValue(x_row, copytext.key));
        setText( x_reln->attributeValue(x_row, copytext.key) );
    } else {
           // use the text entered.
        tupatt->update(key);
        setText(key);
    }
    
    return;    

}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
QString QmvTableItem::setDateTime( const QDateTime & dt )
{
    QString dt_str;
    if ( dt.date().isValid() )
        dt_str = QString( "%1/%2/%3" )
            .arg( dt.date().day() )
            .arg( dt.date().month() )
            .arg( dt.date().year() );
            
    if ( dt.time().isValid() )
        dt_str.append( QString( " %1")
                       .arg( dt.time().toString() ) );
            
    tupatt->update( dt_str );
    if ( parent_table->saveMode() )
        tupatt->parentTuple()->save();
    parent_table->updateCell( row(), col() );

    return dt_str;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
QmvTupleAttribute * QmvTableItem::getTupleAttribute() const
{
    
    return tupatt;
}

