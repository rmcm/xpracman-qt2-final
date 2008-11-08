/*
 *     $Id: qmvmultilineedit.cpp 3121 2006-10-02 11:52:49Z source $
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
 * DESCRIPTION: Multi line editor base on QMultiLineEdit.
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
//#define QMVMULTILINEEDIT_DEBUG
#include "qmvpopup.h"
#include "qmvroweditor.h"
#include "qmvmultilineedit.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvmultilineedit_version[] = "$Id: qmvmultilineedit.cpp 3121 2006-10-02 11:52:49Z source $";

/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */

/*
 * ======================================================================
 * MEMBER FUNCTIONS:
 * ======================================================================
 */

// Constructor
// --------------------------------------------------------------------------------
QmvMultiLineEdit::QmvMultiLineEdit( QmvTupleAttribute * tat, QWidget * parent, const char * name ) 
        : QMultiLineEdit( parent, name ), tupatt(tat), cb_list(0), cb_init(false)
{
    
#ifdef QMVMULTILINEEDIT_DEBUG    
    qDebug( "QmvMultiLineEdit::constructor" );
#endif

    setText( tupatt->currentValue() );
    setWordWrap( QmvMultiLineEdit::WidgetWidth );
    
    int displen;
    int maxwidth = 60;  // arbitary TODO: do something more intelligent
    int defwidth = 20; // arbitary TODO: do something more intelligent
    
    displen = tupatt->metaAttribute()->attributeDisplen().toInt();
    if ( displen == 0 )
        displen = text().length();
    if ( displen < 1 )
        displen = defwidth;
    if ( displen > maxwidth )
        displen = maxwidth;
            
    setWrapColumnOrWidth( displen );

    setFocusPolicy(QWidget::WheelFocus);
    
        // this is not activated when mode is "OnTyping"

    switch ( tupatt->metaAttribute()->inputMethod() )
    {
            
        case QmvAttribute::List:
                //editor_type = ComboBox;
            setReadOnly(TRUE);
            
            if ( tupatt->metaAttribute()->inputMethod() == QmvAttribute::List )
            {
                    // display the label instead of value
                QString label = tupatt->getLabelByValue( tupatt->currentValue() );
                if ( !label.isNull() && label.length() )
                    setText( label );
            }
            
            cb_list = selectList();
            break;
            
        case QmvAttribute::ReadOnly:
            setReadOnly(TRUE);
                //editor_type = NoEdit;
            break;
            
        case QmvAttribute::CopyText:
        case QmvAttribute::InputUnknown:
        case QmvAttribute::Direct:
        default:
                //editor_type = MultiLineEdit;
            break;
    }
    setCursorPosition(0,0);
    selectAll();
   
    installEventFilter( this );
    setFocusPolicy(QWidget::StrongFocus);
}

// Destructor
// --------------------------------------------------------------------------------
QmvMultiLineEdit::~QmvMultiLineEdit()
{
    
#ifdef QMVMULTILINEEDIT_DEBUG
    qDebug( "QmvMultiLineEdit::destructor" );
#endif

}

//! Event Filter
/*!


  \param o      Object generating the event
  \param e      The Event object
  \return       TRUE to stop event, FALSE to dispatch the event
*/
// --------------------------------------------------------------------------------
bool QmvMultiLineEdit::eventFilter( QObject *o, QEvent *e )
{
        // just pass thru to relevant filter.
    
#ifdef QMVMULTILINEEDIT_DEBUG
    const char *name = o ? o->name() : "NO_OBJECT";
    const char *classname = o ? o->className() : "NO_CLASS";
    int etype = e ? e->type() : -9999;
    qDebug( "%-20.20s::eventFilter(%20.20s/%20.20s, %6d)", "QmvMultiLineEdit", name, classname, etype );
#endif

    if ( !o || !e )
  	return QMultiLineEdit::eventFilter( o, e );

        // add special handling
    
    return QMultiLineEdit::eventFilter( o, e );    // standard event processing
}

// --------------------------------------------------------------------------------
//! Key Press handler.
/*!
  A reimplementation to handle ForeignKey verification and other data verification,

  \param ke     The current KeyEvent object
*/
// --------------------------------------------------------------------------------
void QmvMultiLineEdit::keyPressEvent( QKeyEvent *ke )
{

#ifdef QMVMULTILINEEDIT_DEBUG    
    int ktype = ke ? ke->key() : -9999;
    qDebug( "%-20.20s::keyPressEvent::%6d", "QmvMultiLineEdit", ktype );
#endif

        // A hack to get the initiating keystroke into the combobox
        // and replace the existing contents.
    if ( cb_list && cb_init && ( ke->text()[ 0 ].isPrint()) )
    {
        cb_list->setEditText( ke->text() );
        cb_init = FALSE;
        ke->accept();
        return;
    }
        
        // CTRL_F/F3 for FK lookups
    if ( (ke->key() == Key_F &&  (ke->state() & ControlButton )) ||
         ke->key() == Key_F3 )
    {
        if ( cb_list )
        {
            QString newtext = cb_list->currentText();
            cb_list->queryList( newtext );
        }
        
        ke->accept();
        return;
    }

        // Revert to original value
    if ( (ke->key() == Key_Z && (ke->state() & ControlButton )) || ke->key() == Key_Escape)
    {
        setText( tupatt->originalValue() );
        if ( ke->key() == Key_Escape )
            ke->ignore();
        else
            ke->accept();
        return;
    }

        // Force an immediate save, stay in field
    if (ke->key() == Key_S && (ke->state() & ControlButton ) )
    {
        save( SaveIfChanged );
        ke->accept();
        return;
    }

        // TAB or RETURN to end the edit.
        // - make sure the event is passed on
    if ( ke->key() == Key_Tab || ke->key() == Key_Return || ke->key() == Key_Enter )
        ke->ignore();

        // If we get to here then pass it on
    if ( !cb_list )
        QMultiLineEdit::keyPressEvent( ke );
    
    return;
}


// --------------------------------------------------------------------------------
void QmvMultiLineEdit::focusInEvent( QFocusEvent *fe )
{
    
#ifdef QMVMULTILINEEDIT_DEBUG
    qDebug( "QmvMultiLineEdit::focusInEvent" );
#endif
    
    QMultiLineEdit::focusInEvent( fe );
}
// --------------------------------------------------------------------------------
void QmvMultiLineEdit::focusOutEvent( QFocusEvent *fe )
{
    
#ifdef QMVMULTILINEEDIT_DEBUG
    qDebug( "QmvMultiLineEdit::focusOutEvent" );
#endif
    
    QMultiLineEdit::focusOutEvent( fe );
}

// --------------------------------------------------------------------------------
void QmvMultiLineEdit::resizeEvent( QResizeEvent *re )
{
    
#ifdef QMVMULTILINEEDIT_DEBUG
    qDebug( "QmvMultiLineEdit::resizeEvent" );
#endif
    
    QMultiLineEdit::resizeEvent( re );
    if ( cb_list )
        cb_list->resize( re->size() );
}


//! Verify Foreign Keys
/*!
  Verify the current test as a valid foreign key. If there is more than one match then
  display a select list.

*/
// --------------------------------------------------------------------------------
void QmvMultiLineEdit::checkForeign()
{

#ifdef QMVMULTILINEEDIT_DEBUG
    qDebug( "QmvMultiLineEdit::checkForeign()" );
#endif
    
    QString newvalue = text();
    
        // is this a list ?
    if ( tupatt->metaAttribute()->inputMethod() != QmvAttribute::List )
        return;

    cb_list->updateList( text() );
}

// --------------------------------------------------------------------------------
QmvComboBox * QmvMultiLineEdit::selectList()
{

#ifdef QMVMULTILINEEDIT_DEBUG
    qDebug( "QmvMultiLineEdit::selectList()" );
#endif

    if ( !cb_list )
    {
        cb_init = TRUE;
        
            // add a combobox
        cb_list = new QmvComboBox( tupatt, TRUE, this, "combo-list" );
        setFocusProxy( cb_list );

            // get the result
        connect( cb_list, SIGNAL( activated( const QString & ) ),
                 this, SLOT( editCompleted( const QString & ) ) );
    
            // load the attibute value list
        cb_list->loadList();
    
        cb_list->setEditable( TRUE );
        cb_list->setInsertionPolicy( QComboBox::NoInsertion );
        cb_list->setAutoCompletion( FALSE );
        if ( cb_list->lineEdit() )
        {
            cb_list->lineEdit()->selectAll();
            cb_list->lineEdit()->setCursorPosition(0);
        }

    }
    return cb_list;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvMultiLineEdit::editCompleted( const QString & text  )
{
    QMultiLineEdit::setText( text );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
bool QmvMultiLineEdit::save()
{
    
#ifdef QMVMULTILINEEDIT_DEBUG
    qDebug( "QmvMultiLineEdit::save()" );
#endif
    
        // smart save
    return save( SaveIfChanged );
}

// --------------------------------------------------------------------------------
bool QmvMultiLineEdit::save( SaveMode mode )
{
    
#ifdef QMVMULTILINEEDIT_DEBUG
    qDebug( "QmvMultiLineEdit::save(mode)" );
#endif
    
    QString newvalue = text();
    QmvAttribute::input_method im;
    QmvAttribute * at = tupatt->metaAttribute();
        // no attribute
    if ( !at )
        return(false);
    
    im = at->inputMethod();
    
    if ( im == QmvAttribute::List )
    {
        QString newlabel;
            // first expect that the data is label
        newlabel = text();
        newvalue = tupatt->getValueByLabel( newlabel );
        if ( newvalue.isNull() )
                // try the data as a value
        {
            newvalue = text();
            newlabel = tupatt->getLabelByValue( newvalue );
        }
        
            // if value found, refresh the visible label (it may have been entered as a value )
        if ( !newlabel.isNull() )
            setText( newlabel );
    }   
    
    
    tupatt->update( newvalue );

    if ( mode == NeverSave )
        return(TRUE);
    
    if ( tupatt->isChanged() || mode == AlwaysSave )
            // there is a core happening here sometimes, do some checking
        if ( !tupatt->parentTuple() )
        {
            cout << "ERROR: QmvMultiLineEdit::save() tupatt->parentTuple() is null" << endl;
            return(false);
        }
    
        if ( !tupatt->parentTuple()->save() )
        {
// DISABLE THIS: it is probably too close to the action, and triggers a recursive call to this method.
//            
//             if ( tupatt->parentTuple() && tupatt->parentTuple()->parentClass() ) {
//             	QString sql_error = tupatt->parentTuple()->lastError();
//                 if (sql_error.length() < 1 )
//                     sql_error = "Unknown Error Message";
//                 QMessageBox::warning(0, "Save value:Warning",
//                                      tr("The save failed<br><b>%1</b>")
//                                      .arg(sql_error),
//                                      "OK", 0 );
//            }
            tupatt->restore();
            setText( tupatt->currentValue() );
            if ( im == QmvAttribute::List )
                setText( tupatt->getLabelByValue( tupatt->currentValue() ) );
            return(FALSE);
        }
    
    return(TRUE);
}
