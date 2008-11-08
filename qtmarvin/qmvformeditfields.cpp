/*
 *     $Id: $
 * 
 *   Copyright 2006 X=X Computer Software Trust
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
 * DESCRIPTION: Preare Form-based Report
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <qdialog.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qfocusdata.h>
#include "qmvformeditfields.h"
#include "qmvmultilineedit.h"
namespace QmvFormEditFieldsIcons
{
#include "quit.xpm"
} // end QmvFormIcons

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvform_version[] = "$Id:$";

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

//--------------------------------------------------------------------------------        
// Constructor
//--------------------------------------------------------------------------------        
QmvFormEditFields::QmvFormEditFields( QWidget * parent, const char * name )
            : QDialog( parent, name, TRUE )
{
            
    setCaption( name );
    QmvIcons * stdicons = new QmvIcons( this, "new_icons" );

        // --------------------------------------------------
        // layout
        // --------------------------------------------------
    QVBoxLayout * vl_editrow = new QVBoxLayout( this, 2 );
    
    QHBoxLayout * hl_editrow = new QHBoxLayout( vl_editrow, 0, "controls_editrow" );
    QLabel * x_label = new QLabel( name, this );
    x_label->setFocusPolicy( QWidget::NoFocus );
    hl_editrow->addWidget(x_label);
    hl_editrow->addStretch( 10 );

        // --------------------------------------------------
        // QWhatsThis
        // --------------------------------------------------
    QToolButton * wtb = QWhatsThis::whatsThisButton(this);
    wtb->setFocusPolicy( QWidget::NoFocus );
    wtb->setAccel( Key_F1 );
    hl_editrow->addWidget(wtb);

        // --------------------------------------------------
        // Control: close button
        // --------------------------------------------------
    QToolButton * close_editrow = new QToolButton( this, "close_editrow");
    close_editrow->setFocusPolicy( QWidget::NoFocus );
    close_editrow->setIconSet( stdicons->getIconSet(QmvIcons::QuitIcon) );
    close_editrow->setTextLabel( "Close the mini editor" );
    close_editrow->setAutoRaise(TRUE);
    QWhatsThis::add( close_editrow, "Close the mini editor");
    hl_editrow->addWidget(close_editrow);
    connect( close_editrow, SIGNAL( clicked() ), this, SLOT( reject() ) );
    close_editrow->setAccel( CTRL+Key_W );

        // --------------------------------------------------
        // the field layout (can grow)
        // --------------------------------------------------
    grid_editrow = new QGridLayout( vl_editrow, 1, 2);

                
        // --------------------------------------------------
        //  Container for Title and navigation buttons
        // --------------------------------------------------
    QHBox * return_buttons = new QHBox( this, "return buttons" );
    vl_editrow->addWidget( return_buttons );
    return_buttons->setFrameStyle( QFrame::Panel | QFrame::Sunken );

        // OK/Save button
    QPushButton * pb = new QPushButton( "C&ontinue", return_buttons, "continue button" );
    pb->setFocusPolicy( QWidget::StrongFocus );
    QWhatsThis::add(pb , "Press this button to continue with printing" );
    connect( pb, SIGNAL( clicked() ), this, SLOT( accept() ) );

        // Cancel button
    pb = new QPushButton( "&Cancel", return_buttons, "cancel button" );
    pb->setFocusPolicy( QWidget::StrongFocus );
    QWhatsThis::add(pb , "Press this button to cancel printing" );
    connect( pb, SIGNAL( clicked() ), this, SLOT( reject() ) );

}

//--------------------------------------------------------------------------------        
//--------------------------------------------------------------------------------        
QmvFormEditFields::~QmvFormEditFields() 
{}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------        
void QmvFormEditFields::addEditorRow( QLabel * label, QWidget * editor, int row )
{
                
        // add label to layout
    grid_editrow->addWidget( label, row, 0 );

        // Add editor to the layour
    grid_editrow->addWidget( editor, row, 1 );
        // make sure that TABS and ENTERS navigate properly
    editor->installEventFilter(this);
    
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------        
void QmvFormEditFields::editCompleted()
{
    focusNextPrevChild(true);
}

// --------------------------------------------------------------------------------
//! Event Filter
/*!
  \param o      Object generating the event
  \param e      The Event object
  \return       TRUE to stop event, FALSE to dispatch the event
*/
// --------------------------------------------------------------------------------
bool QmvFormEditFields::eventFilter( QObject *o, QEvent *e )
{
    
    if ( !o || !e )
  	return QWidget::eventFilter( o, e );

        // add special handling  
    if ( e->type() == QEvent::KeyPress )
    {
        QKeyEvent *ke = (QKeyEvent *) e;

            // ENTER triggers a new query to be formed.
        if ( ke->key() == Key_Tab || ke->key() == Key_Return || ke->key() == Key_Enter )
        {
            if (QString(o->className()) == "QmvMultiLineEdit") {
                ((QmvMultiLineEdit *) o)->save();
            }
            focusNextPrevChild(true);
            return true;
        }
    }

    if ( e->type() == QEvent::FocusOut )
    {
        if (QString(o->className()) == "QmvMultiLineEdit") {
            ((QmvMultiLineEdit *) o)->save();
        }
    }
    


    return QWidget::eventFilter( o, e );    // standard event processing
}


// --------------------------------------------------------------------------------
//! Key Press handler.
/*!
  A reimplementation to handle ForeignKey verification and other data verification,

  \param ke     The current KeyEvent object
*/
// --------------------------------------------------------------------------------
void QmvFormEditFields::keyPressEvent( QKeyEvent *ke )
{


        // TAB or RETURN to end the edit.
        // - make sure the event is passed on
    if ( ke->key() == Key_Tab || ke->key() == Key_Return || ke->key() == Key_Enter )
    {
        focusNextPrevChild(true);
        ke->accept();
        return;
    }
    
    return;
}


