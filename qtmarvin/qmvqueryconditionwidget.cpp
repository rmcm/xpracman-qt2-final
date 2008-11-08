/*
 *     $Id: qmvqueryconditionwidget.cpp 3206 2006-11-18 02:35:14Z source $
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
 * DESCRIPTION: QmvQueryConditionWidget
 *
 *      A widget for a single sql query condition object
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include "qmvqueryconditionwidget.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmvqueryconditionwidget_version[] = "$Id: qmvqueryconditionwidget.cpp 3206 2006-11-18 02:35:14Z source $";

/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */

/*
 * ======================================================================
 * MEMBER FUNCTIONS: QmvQueryConditionWidget Class
 * ======================================================================
 */

// Constructor

QmvQueryConditionWidget::QmvQueryConditionWidget( QmvQueryCondition * qry_condition,
                                                  QWidget * parent, const char * name )
        : QWidget( parent, name ), query_condition(qry_condition)
{
        // Get a new condition object and associate it with a condition new widget
    setMaximumHeight(22);    
    h_layout = new QHBoxLayout( this, -1, 0, "condition-h-layout" );

        // Create combobox for attribute names,
        // - load it with attribute_titles
        // - add it to index.

    cb_atlist = new QComboBox( false, this, "attributes_combo" );
    cb_atlist->insertStringList( query_condition->getAttributeTitles(), 0 );
    cb_atlist->clearEdit();
    QWhatsThis::add( cb_atlist,
                     QString("<h3>Attributes List:</h3> Select the attribute(eg <b>%1</b>) to use when selecting rows.")
                     .arg(query_condition->getAttTitle()));
    h_layout->addWidget( cb_atlist );
    int std_height = cb_atlist->sizeHint().height();
    
    cb_oplist = new QComboBox( false, this, "operators" );
    h_layout->addWidget( cb_oplist );

    for ( int i = QmvQueryCondition::Matches; i <= QmvQueryCondition::All; i++ )
        cb_oplist->insertItem (query_condition->getOpLabel((QmvQueryCondition::operator_types) i), i );
        // select matches for default
    cb_oplist->setCurrentItem( QmvQueryCondition::Like );
    query_condition->setOpValue( QmvQueryCondition::Like  );
    QWhatsThis::add( cb_oplist,
                     "<h3>Query operator:</h3> Select the type of filter to use when selecting rows."
                     "<ul>"
                     "<li><b>Matches:</b> case-sensitive; matches from start of field "
                     "<li><b>Like:</b> case-insensitive ; matches from start of field "
                     "<li><b>Contains:</b> case-insensitive ; matches anywhere in field "
                     "<li><b>Equals:</b> exact match; case-sensitive"
                     "<li><b>GT</b> - Greater-than"
                     "<li><b>LT</b> - Less-than"
                     "<li><b>GE</b> - Greater-than or equal-to"
                     "<li><b>LE</b> - Less-then or equal-to"
                     "<li><b>Not</b> - Does not match"
                     "<li><b>All</b> - Matches all fields."
                     "</ul>");

        // Create combobox for attribute query values,
        // - add it to index.
    cb_ptlist = new QComboBox( true, this, "value_combo" );
    cb_ptlist->setInsertionPolicy( QComboBox::AtTop );
    cb_ptlist->setAutoCompletion( TRUE );
    h_layout->addWidget( cb_ptlist );
    QStringList at_titles = query_condition->getAttributeTitles();
    QString help = QString("<h3>Search Pattern:</h3> Enter the pattern"
                           " for the <b>attribute (eg %1)</b> which you want rows to match.")
        .arg(query_condition->getAttTitle());
    if (at_titles.count() > 1 )
        help.append( QString(" You can also specify patterns for more than one attribute,"
                             " separated by a comma.<br>"
                             "For example: <b>%1,%2</b>")
                     .arg(at_titles[0])
                     .arg(at_titles[1]) );
    QWhatsThis::add( cb_ptlist, help );
    setFocusProxy( cb_ptlist );
    
        //  update widget from querycondition
    update();
    
        // Connect condition-item widgets to change-methods.

    connect( cb_atlist, SIGNAL( highlighted( const QString & ) ),
             this, SLOT( atActivated( const QString & ) ) );

    connect( cb_oplist, SIGNAL( highlighted( const QString & ) ),
             this, SLOT( opActivated( const QString & ) ) );
    
    connect( cb_ptlist, SIGNAL( highlighted( const QString & ) ),
             this, SLOT( ptActivated( const QString &) ) );

        // notice pattern changes, to keep text in sync with advanced query widget.
    connect( cb_ptlist, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( ptChanged( const QString &) ) );

        // Install event filters for atttribute and operator list
        // to use ENTER as search trigger
        // (cb_ptlist is editable, so ENTER is noticed internally

    cb_atlist->installEventFilter(this);
    cb_oplist->installEventFilter(this);
    cb_ptlist->lineEdit()->installEventFilter(this);
}

// ----------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------
QmvQueryConditionWidget::~QmvQueryConditionWidget()
{
    comparator_buttons.setAutoDelete( TRUE );
    comparator_buttons.clear();
}

// --------------------------------------------------------------------------------
//! Event Filter
/*!
  \param o      Object generating the event
  \param e      The Event object
  \return       TRUE to stop event, FALSE to dispatch the event
*/
// --------------------------------------------------------------------------------
bool QmvQueryConditionWidget::eventFilter( QObject *o, QEvent *e )
{
    
#ifdef QMVQUERYCONDITIONWIDGET__DEBUG
    const char *name = o ? o->name() : "NO_OBJECT";
    const char *classname = o ? o->className() : "NO_CLASS";
    int etype = e ? e->type() : -9999;
    qDebug( "%-20.20s::eventFilter(%20.20s/%20.20s, %6d)", "QmvQueryConditionWidget", name, classname, etype );
#endif

    if ( !o || !e )
  	return QWidget::eventFilter( o, e );

    if ( e->type() == QEvent::KeyPress )
    {
        QKeyEvent *ke = (QKeyEvent *) e;

            // ENTER triggers a new query to be formed.
        if ( ke->key() == Key_Return || ke->key() == Key_Enter )
        {
            if ( o == cb_atlist )
                atValidate( cb_atlist->currentText(), true );
            else if ( o == cb_oplist )
                opValidate( cb_oplist->currentText(), true );
            else if ( o == cb_ptlist->lineEdit() )
                ptValidate( cb_ptlist->currentText(), true );
        }
    }
        

        // add special handling  

    return QWidget::eventFilter( o, e );    // standard event processing
}


//--------------------------------------------------------------------------------
//! Validate QmvAttribute changes
/*!
  Check the requested change and update the current attribute in the associated
  \ref QmvQueryCondition if ok. Trigger a new query if \em trigger is TRUE

  \param at_text        text entered
  \param                trigger (TRUE/FALSE)
*/
//--------------------------------------------------------------------------------
bool QmvQueryConditionWidget::atValidate( const QString & at_text, bool trigger )
{
            // do we have a match - we may have an edit in progress.
    for ( int i = 0; i < cb_atlist->count(); i++ )
    {
        if ( cb_atlist->text( i ) != at_text )
            continue;
            // only update if there is a match
        query_condition->setAttTitle( at_text, trigger );
        return TRUE;
    }
    return FALSE;
}

//--------------------------------------------------------------------------------
//! Validate Operator changes
/*!
  Check the requested change and update the current operator in the associated
  \ref QmvQueryCondition if ok. Trigger a new query if \em trigger is TRUE

  \param op_text        Position selected in combo list.
  \param                trigger (TRUE/FALSE)
*/
//--------------------------------------------------------------------------------
bool QmvQueryConditionWidget::opValidate( const QString & op_text, bool trigger )
{
        // do we have a match - we may have an edit in progress.
    for ( int i = 0; i < cb_oplist->count(); i++ )
    {
        if ( cb_oplist->text( i ) != op_text )
            continue;
            // only update if there is a match
        query_condition->setOpValue( (QmvQueryCondition::operator_types) i, trigger );
        return TRUE;
    }
    return FALSE;
 }

//--------------------------------------------------------------------------------
//! Validate Pattern changes
/*!
  Check the requested change and update the current pattern in the associated
  \ref QmvQueryCondition if ok. Trigger a new query if \em trigger is TRUE

  \param pt_text        Text enteredin combo list.
  \param                trigger (TRUE/FALSE)
*/
//--------------------------------------------------------------------------------
bool QmvQueryConditionWidget::ptValidate( const QString & pt_text, bool trigger )
{
        // no checks (yet)
    query_condition->setPatternValue( pt_text, trigger );
    return TRUE;
}

//--------------------------------------------------------------------------------
//! Handle QmvAttribute changes
/*!
  Update the current attribute in the associated QmvQueryCondition

  \param at_text      Text edited in combo list.
*/
//--------------------------------------------------------------------------------
void QmvQueryConditionWidget::atChanged( const QString & at_text )
{
    atValidate( at_text, FALSE );
}

//--------------------------------------------------------------------------------
//! Handle QmvAttribute activation
/*!
  Update the current attribute in the associated QmvQueryCondition, then emit
  a signal to announce the change.

  \param at_text      text selected in combo list.
*/
//--------------------------------------------------------------------------------
void QmvQueryConditionWidget::atActivated( const QString & at_text )
{
    if ( !atValidate( at_text, false ) )
        cb_atlist->setEditText( cb_atlist->text(cb_atlist->currentItem() ) );
}

//--------------------------------------------------------------------------------
//! Handle Attribute activation
/*!
  Update the current attribute in the associated QmvQueryCondition

  \param at_button      The index of the attribute button clicked.
*/
//--------------------------------------------------------------------------------
void QmvQueryConditionWidget::atActivated( int at_button )
{
    query_condition->setAttTitle( cb_atlist->text( cb_atlist->currentItem() ), false );
}

//--------------------------------------------------------------------------------
//! Handle Operator changes
/*!
  Update the current comparison-operator in the associated QmvQueryCondition.

  \param op_text      The text of the operator text widget
*/
//--------------------------------------------------------------------------------
void QmvQueryConditionWidget::opChanged( const QString & op_text )
{
    opValidate( op_text, FALSE );
}

//--------------------------------------------------------------------------------
//! Handle Operator activation
/*!
  Update the current comparison-operator in the associated QmvQueryCondition

  \param op_button      The index of the operator button clicked.
*/
//--------------------------------------------------------------------------------
void QmvQueryConditionWidget::opActivated( int op_button )
{
    query_condition->setOpValue( (QmvQueryCondition::operator_types) op_button, false );
}

//--------------------------------------------------------------------------------
//! Handle Operator activation
/*!
  Update the current comparison-operator in the associated QmvQueryCondition

  \param op_text      The text of the operator text widget
*/
//--------------------------------------------------------------------------------
void QmvQueryConditionWidget::opActivated( const QString & op_text )
{
    if ( !opValidate( op_text, false ) )
        cb_oplist->setEditText( cb_oplist->text( cb_oplist->currentItem() ) );
}

//--------------------------------------------------------------------------------
//! Handle Pattern changes
/*!
  Update the current pattern text in the associated QmvQueryCondition.

  \param pt_text      The text of the new search pattern
*/
//--------------------------------------------------------------------------------
void QmvQueryConditionWidget::ptChanged(  const QString & pt_text )
{
    ptValidate( pt_text, false );
}

//--------------------------------------------------------------------------------
//! Handle Pattern activation
/*!
  Update the current pattern text in the associated QmvQueryCondition

  \param pt_text      The text of the new search pattern
*/
//--------------------------------------------------------------------------------
void QmvQueryConditionWidget::ptActivated(  const QString & pt_text )
{
    ptValidate( pt_text, false );
}

//--------------------------------------------------------------------------------
//! Update condition controls from query-condition object
/*!

*/
//--------------------------------------------------------------------------------
void QmvQueryConditionWidget::update()
{
        // attribute control
    int user_label_pos = query_condition->getListPosition( query_condition->getAttTitle() );
    if ( user_label_pos < 0 )
        cb_atlist->setCurrentItem(0);
    else
        cb_atlist->setCurrentItem( user_label_pos );
    
        // operator control
    cb_oplist->setCurrentItem( query_condition->getOperator() );
        // value control
    cb_ptlist->setEditText( query_condition->getValue() );
}
