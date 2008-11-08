/*
 *     $Id: qmvcombobox.cpp 2889 2006-06-11 11:47:27Z source $
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
 * DESCRIPTION: A QMV comboboxlist for dataentry
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
//#define QMVCOMBOBOX_DEBUG
#include "qmvcombobox.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvcombobox_version[] = "$Id: qmvcombobox.cpp 2889 2006-06-11 11:47:27Z source $";

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
QmvComboBox::QmvComboBox( QmvTupleAttribute * tat, bool rw, QWidget * parent, const char * name ) 
        : QComboBox( rw, parent, name ), tupatt(tat)
{
    setEditable(rw);
    init();
}

// --------------------------------------------------------------------------------
QmvComboBox::QmvComboBox( QmvTupleAttribute * tat, QWidget * parent, const char * name ) 
        : QComboBox( parent, name ), tupatt(tat)
{
    init();
}

// Destructor

// --------------------------------------------------------------------------------
QmvComboBox::~QmvComboBox()
{
}

// --------------------------------------------------------------------------------
//! Common constructor initialisation code.
/*!
 
*/
// --------------------------------------------------------------------------------
void QmvComboBox::init()
{
    setInsertionPolicy( NoInsertion );
    setAutoCompletion( TRUE );
    installEventFilter( this );
        // Handle changes in text-input line
    connect( lineEdit(), SIGNAL( returnPressed() ),
             this, SLOT( returnPressed() ) );
        // Selected options
    connect( this, SIGNAL( activated ( const QString & ) ),
             this, SLOT( slotActivated( const QString & ) ) );
}

// --------------------------------------------------------------------------------
//! Accept selection and finish editing
/*!
  Return the name of the attribute associated with this combobox

  \param       select string
*/
// --------------------------------------------------------------------------------
void QmvComboBox::slotActivated( const QString & selection )
{
        //lineEdit()->setText(selection);
        //returnPressed();
}

// --------------------------------------------------------------------------------
//! Return the name of the Attribute
/*!
  Return the name of the attribute associated with this combobox

  \return       attribute name
*/
// --------------------------------------------------------------------------------
const QString QmvComboBox::attributeName() const
{
    if (tupatt)
        return tupatt->metaAttribute()->attributeName();
    else
            throw( QmvException( QString( "<h3>QmvComboBox::attributeName()</h2>"
                                          "tupatt not set" ),
                                 QmvException::Critical) );
}

// --------------------------------------------------------------------------------
//! Return the value of the attribute.
/*!


  \return  Attribute value
*/
// --------------------------------------------------------------------------------
const QString QmvComboBox::attributeValue() const
{
    if (tupatt)
        return tupatt->currentValue();
    else
            throw( QmvException( QString( "<h3>QmvComboBox::attributeValue()</h2>"
                                          "tupatt not set" ),
                                 QmvException::Critical) );
}

// --------------------------------------------------------------------------------
//! Return a pointer to the parent object
/*!
  Return a pointer to the parent \ref QmvClass object

  \return       Parent object

*/
// --------------------------------------------------------------------------------

const QmvClass * QmvComboBox::parentRelation() const
{
    if (!tupatt)
        throw( QmvException( QString( "<h3>QmvComboBox::parentRelation()</h2>"
                                      "tupatt not set" ),
                             QmvException::Critical) );
    
    return(tupatt->parentTuple()->parentClass());
}


// --------------------------------------------------------------------------------
//! Return a pointer to the parent database object
/*!
  Return a pointer to the parent \ref QmvDB object

  \return       Parent Database object

*/
// --------------------------------------------------------------------------------
const QmvDB * QmvComboBox::dataBase() const
{
    return parentRelation()->dataBase();
}


// --------------------------------------------------------------------------------
//! Load the combolist.
/*!
  Load the list with the intrinsic \em list.

  \param list   
*/
// --------------------------------------------------------------------------------
void QmvComboBox::loadList( )
{
    loadList( tupatt->getLabelList() );
}


// --------------------------------------------------------------------------------
//! Load the combolist.
/*!
  Load the list with the supplied \em list.

  \param list   
*/
// --------------------------------------------------------------------------------
void QmvComboBox::loadList( const QStringList & list )
{
    clear();
    insertStringList( list );
    setCurrentLabel();
}

// --------------------------------------------------------------------------------
//! Replace the combolist.
/*!
  replace the list on the basis of supplied input, checking values and labels.

  \param input   
*/
// --------------------------------------------------------------------------------
void QmvComboBox::queryList( const QString & input )
{
        // refresh the list using "input" as search, in both value and label
    tupatt->extendList(input, true, true, true);
    loadList();
}


// --------------------------------------------------------------------------------
//! Update the combolist.
/*!
  update the list on the basis of supplied input, checking values and labels.

  \param input   
*/
// --------------------------------------------------------------------------------
void QmvComboBox::updateList( const QString & input )
{
        // the input comes from the lineedit widget. This may be a request to search
        // for a set of foreign values. The text may be a value or a label or part of
        // either.

        // getValueByLabel will also try search as value
        // and extend the list by items found.

    QString newlabel;
    QString newvalue;
    
        // First try as value - getLabelByValue() will search persistent storage
        //   if not found.
    newlabel = tupatt->getLabelByValue( input );
    if ( newlabel.isNull() )
            // try label
    {
        newvalue = tupatt->getValueByLabel( input );
        if ( !newvalue.isNull() )
            newlabel = tupatt->getLabelByValue( newvalue );
    } else {
            // nothing is an exact match
        newvalue = input;
            // replace the list using "input" as search, in both value and label
        tupatt->extendList(input, true, true, true);
    }
    
        // load newvalue into local list (unconditionally, since the list may have changed )
    QStringList newlist;
    newlist.append(newvalue);
    loadList(newlist);
    
        // set current item if possible and different to current input
    if ( !newlabel.isNull() && newlabel != input )
    {
        if ( int i = tupatt->getIndexByLabel( newlabel ) >= 0 )
            setCurrentItem( i );
        setEditText( newlabel );
    }
    
}

// --------------------------------------------------------------------------------
//! Set the current item in list
/*!
  Set the current item in the list, using the label associated
  with the current tupleattribute value.

*/
// --------------------------------------------------------------------------------
void QmvComboBox::setCurrentLabel()
{
        // this will also add to list if necessary
    QString label = tupatt->getLabelByValue( attributeValue() );
    int index = 0;
    if ( label.isNull() )
        return;
    else
        index = tupatt->getIndexByLabel( label ); 

    setCurrentItem( index );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvComboBox::popupList()
{
    popup();
}

//! The attribute value associated with the current selection
/*!
  Return the data value associated with the current selection.

  \return data-value
*/
// --------------------------------------------------------------------------------
const QString QmvComboBox::currentValue() const
{
    return  tupatt->getValueByLabel( currentText() );
}

// --------------------------------------------------------------------------------
//! Input complete.
/*!
  Cleanup at end of input. Use the input to update the current list, then
  emit "highlighted" signal.

*/
// --------------------------------------------------------------------------------
void QmvComboBox::returnPressed()
{
#ifdef QMVCOMBOBOX_DEBUG
    qDebug("QmvComboBox::inputCompleted():text=%s", currentText().latin1() );
#endif
    
    QString text = lineEdit()->text();
    
        // the user has input some text - check the value
    if ( text.length() > 0 )
    {
        updateList( text );
    }
    
    emit highlighted( text );
}

// --------------------------------------------------------------------------------
//! Event Filter
/*!
  \param o      Object generating the event
  \param e      The Event object
  \return       TRUE to stop event, FALSE to dispatch the event
*/
// --------------------------------------------------------------------------------
bool QmvComboBox::eventFilter( QObject *o, QEvent *e )
{
    
#ifdef QMVCOMBOBOX_DEBUG
    const char *name = o ? o->name() : "NO_OBJECT";
    const char *classname = o ? o->className() : "NO_CLASS";
    int etype = e ? e->type() : -9999;
    qDebug( "%-20.20s::eventFilter(%20.20s/%20.20s, %6d)", "QmvComboBox", name, classname, etype );
#endif

    if ( !o || !e )
  	return QComboBox::eventFilter( o, e );

        // add special handling  

    return QComboBox::eventFilter( o, e );    // standard event processing
}

// --------------------------------------------------------------------------------
//! Key Press handler.
/*!
  A reimplementation to handle ForeignKey verification and other data verification,

  \param ke     The current KeyEvent object
*/
// --------------------------------------------------------------------------------
void QmvComboBox::keyPressEvent( QKeyEvent *ke )
{

#ifdef QMVCOMBOBOX_DEBUG
    int ktype = ke ? ke->key() : -9999;
    qDebug( "%-20.20s::keyPressEvent::%6d", "QmvComboBox", ktype );
#endif

    if ( ke && ke->key() == Key_Escape )
        setEditText( tupatt->originalValue() );
    
    QComboBox::keyPressEvent( ke );
    return;
}


// --------------------------------------------------------------------------------
void QmvComboBox::focusInEvent( QFocusEvent *fe )
{
    
#ifdef QMVCOMBOBOX_DEBUG
    qDebug( "QmvComboBox::focusInEvent" );
#endif
    
    QComboBox::focusInEvent( fe );
}
// --------------------------------------------------------------------------------
void QmvComboBox::focusOutEvent( QFocusEvent *fe )
{
    
#ifdef QMVCOMBOBOX_DEBUG
    qDebug( "QmvComboBox::focusOutEvent" );
#endif
    
    QComboBox::focusOutEvent( fe );
}

// --------------------------------------------------------------------------------
void QmvComboBox::resizeEvent( QResizeEvent *re )
{
    
#ifdef QMVCOMBOBOX_DEBUG
    qDebug( "QmvComboBox::resizeEvent" );
#endif
    
    QComboBox::resizeEvent( re );
}
