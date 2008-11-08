/*
 *     $Id: qmvroweditor.cpp 2907 2006-07-02 08:31:02Z source $
 * 
 *   Copyright 2002 X=X Computer Software Trust
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
 * DESCRIPTION:  A simple row browser/editor - suitable for popups.
 *
 * ======================================================================
 */


/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include "qmvroweditor.h"

namespace QmvRowEditorIcons
{
#include "left1arrow.xpm"
#include "right1arrow.xpm"
} // end QmvRowEditorIcons

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmvroweditor_version[] = "$Id: qmvroweditor.cpp 2907 2006-07-02 08:31:02Z source $";
// #define QMVROWEDITOR_DEBUG

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

QmvRowEditor::QmvRowEditor(QmvSqlClass * rel, bool save,
                           QWidget * parent, const char * name)
            : QDialog( parent, name, TRUE, 0 )
{
    relation = rel;
    setSaveMode( save );
    setCaption( relation->relationTitle() );
    QmvIcons * stdicons = new QmvIcons( this, "new_icons" );

        // Layout
    QVBoxLayout * vl_editrow = new QVBoxLayout( this, 2 );
    
    QHBoxLayout * hl_editrow = new QHBoxLayout( vl_editrow, 0, "controls_editrow" );
    QLabel * testlabel = new QLabel( relation->relationTitle(), this );
    testlabel->setFocusPolicy( QWidget::NoFocus );
    hl_editrow->addWidget(testlabel);
    hl_editrow->addStretch( 10 );

        // QWhatsThis
    QToolButton * wtb = QWhatsThis::whatsThisButton(this);
    wtb->setFocusPolicy( QWidget::NoFocus );
    hl_editrow->addWidget(wtb);
    wtb->setAccel( Key_F1 );

        // Control: close button
    QToolButton * close_editrow = new QToolButton( this,
                                                   "close_editrow");
    close_editrow->setFocusPolicy( QWidget::NoFocus );
    close_editrow->setIconSet( stdicons->getIconSet(QmvIcons::QuitIcon) );
    close_editrow->setTextLabel( "Close the mini editor" );
    close_editrow->setAutoRaise(TRUE);
    QWhatsThis::add( close_editrow, "Close the mini editor");
    hl_editrow->addWidget(close_editrow);
    connect( close_editrow, SIGNAL( clicked() ), this, SLOT( reject() ) );
    close_editrow->setAccel( CTRL+Key_W );
    
        // Table: the single row editor
    table_editrow = new QmvTable( this, "relation_row" );
    table_editrow->setSaveMode( save_mode );
    vl_editrow->addWidget( table_editrow );
    
    table_editrow->setSorting( -1 );
    table_editrow->horizontalHeader()->setClickEnabled( FALSE,  -1 );
    table_editrow->verticalHeader()->setClickEnabled( FALSE,  -1 );
    table_editrow->setTopMargin(0);
    table_editrow->horizontalHeader()->hide();
    table_editrow->setShowGrid(TRUE);
    table_editrow->setFocusPolicy(QWidget::StrongFocus);

        // connect redisplay signal

    connect( table_editrow, SIGNAL( reloadRequest( int, int ) ),
               this, SLOT( slotRefreshSet( ) ) );

        // default to editiable attributes only
    setAttList( relation->edit_attlist() );

        // --------------------------------------------------
        //  Container for Title and navigation buttons
        // --------------------------------------------------
    return_buttons = new QHBox( this, "Title-navigator" );
    vl_editrow->addWidget( return_buttons );
    return_buttons->setFrameStyle( QFrame::Panel | QFrame::Sunken );
//    return_buttons->hide();          // disable for now.

        // Create a signal mapper
    return_signals = new QSignalMapper( this );
    connect( return_signals, SIGNAL( mapped(int) ),
             this, SLOT( handleReturn(int) ));

    adjustSize();
    updateGeometry();
    setFocusProxy(table_editrow);
    setFocus();
}

// Destructor

QmvRowEditor::~QmvRowEditor()
{
  
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRowEditor::handleReturn( int index )
{
    done(index);
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRowEditor::setReturnButton( QString label, int index, QString text )
{
    QPushButton * pb = new QPushButton( label, return_buttons );
    QWhatsThis::add(pb , text );
    return_signals->setMapping( pb, index );
    connect( pb, SIGNAL( clicked() ), return_signals, SLOT( map() ) );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRowEditor::setAttList( const QmvAttList & al )
{
    atlist = al;
    initTable();
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRowEditor::initTable()
{
    int n_att = atlist.count();
    table_editrow->setNumCols( 1 );
    table_editrow->setNumRows( n_att );

        // Insert the attribute labels

    int label_width = 0;
    int row = 0;
    for (QmvAttribute * at_ptr = atlist.first(); at_ptr != 0; at_ptr = atlist.next())
    {
        QString lab = at_ptr->attributeTitle();
        table_editrow->verticalHeader()->setLabel( row, lab );
        int labwid =  table_editrow->verticalHeader()->fontMetrics().width( lab );
        label_width = ( labwid > label_width ) ? labwid : label_width;

        row++;
        
    }

        // add some slack
    label_width += 20;
    
    table_editrow->setLeftMargin( (label_width < 200) ? label_width : 200 );
    table_editrow->setColumnStretchable( 0, TRUE );
    table_editrow->setMinimumWidth( 400 );
    
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
bool QmvRowEditor::saveMode() const
{
    return save_mode;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRowEditor::setSaveMode( bool save )
{
    save_mode = save;
}


// --------------------------------------------------------------------------------
//! Load the edit table with tuple data from \em row
/*!
  

  \param tuple_row      The row number to load
  \param save           Whether an edit or display panel
  \return               true/false for success/failure of load
*/
// --------------------------------------------------------------------------------
bool QmvRowEditor::load( int tuple_row )
{

    QmvTuple * tup = relation->tupleAt( tuple_row );
    if ( !tup )
        return false;

    int table_row = 0;
    
    for (QmvAttribute * at = atlist.first(); at != 0; at = atlist.next())
    {
        QmvTableItem *it = (QmvTableItem *) table_editrow->item( table_row, 0 );

        QmvTupleAttribute * tat = tup->tupleAttribute( at->attributeName() );

        QmvAttribute::input_method  im = at->inputMethod();
        QmvAttribute::access_method am = at->accessMethod();
        
        QmvAttribute::data_type     dt = at->dataType();
        QmvTableItem::EditType et;
         
        switch (im) //InputUnknown, Direct, List
        {
            case QmvAttribute::InputUnknown:
            case QmvAttribute::Direct:
            case QmvAttribute::List:
                et = QmvTableItem::OnTyping;
                break;
        }
        switch (am)
        {
            case QmvAttribute::ReadOnly:
                et = QmvTableItem::Never;
                break;
             
        }

        table_editrow->clearCellWidget( table_row, 0 );
        QmvTableItem * item = new QmvTableItem( table_editrow, et, tat );
        item->setWordWrap( TRUE );
        table_editrow->setItem( table_row, 0, item );

        table_row++;
    }

    adjustSize();
    updateGeometry();
    
        // this will also grab focus.
    table_editrow->setCurrentCell(0, 0);
    current_row = tuple_row;
    
    return true;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRowEditor::slotRefreshSet()
{
    load( current_row );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRowEditor::slotNextRow()
{
       if ( current_row >= relation->count() - 1 )
        return;
    
    current_row++;
    load( current_row );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvRowEditor::slotPreviousRow()
{

    if ( current_row <= 0 )
        return;
    
    current_row--;
    load( current_row );
}
