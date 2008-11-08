/*
 *     $Id: qmvform.cpp 3336 2007-01-28 04:54:30Z source $
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
 * DESCRIPTION: Preare Form-based Report
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qstatusbar.h>
#include <qdict.h>
#include "qmvformeditfields.h"
#include "qmvform.h"
#include "qmvroweditor.h"
#include "qmvmultilineedit.h"
#include "qinputdialog.h"
namespace QmvFormIcons
{
#include "left1arrow.xpm"
#include "right1arrow.xpm"
#include "left2arrow.xpm"
#include "right2arrow.xpm"
#include "print.xpm"
#include "fax.xpm"
#include "email.xpm"
#include "quit.xpm"
} // end QmvFormIcons

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvform_version[] = "$Id: qmvform.cpp 3336 2007-01-28 04:54:30Z source $";

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

QmvForm::QmvForm( QmvSqlClass * relation, QWidget * parent, const char * name )
        : QDialog( parent, name, TRUE, 0 )
{
    setRelation( relation );
    print_status = false;
    reln_form = new QmvSqlClass( form_data->dataBase(), "form" );
    reln_fmdt = new QmvSqlClass( form_data->dataBase(), "fmdt" );

    setCaption( tr( "Form Viewer" ) );
    
        // ------------------------------------------------------------
        // Top level layout 
        // ------------------------------------------------------------
    QVBoxLayout * vl_top = new QVBoxLayout( this, 1 );
        // Icons
    QmvIcons * stdicons = new QmvIcons( this, "controls_icons" );

        // Controls - layout
    QHBox * hl_tools = new QHBox( this, "controls_form" );
    vl_top->addWidget( hl_tools );
    hl_tools->setSpacing(0);
    hl_tools->setLineWidth(0);
    
        // Create control buttons ----------------------------
    QToolButton * fc_tb;

        // First page
    fc_tb = new QToolButton( hl_tools, "formcontrol_firstpage");
    fc_tb->setAutoRaise( true );
    fc_tb->setIconSet( stdicons->getIconSet( QmvIcons::Left2Icon ) );
    fc_tb->setTextLabel("First Page (Home)");
    fc_tb->setAccel( Key_Home );
    QWhatsThis::add( fc_tb, QString("<b>First:</b>.<br>"
                     "Go to the first page") );
    connect( fc_tb, SIGNAL( clicked() ), this, SLOT( slotFirstPage() ) );

        // Previous Page
    fc_tb = new QToolButton( hl_tools, "formcontrol_previouspage");
    fc_tb->setAutoRaise( true );
    fc_tb->setIconSet( stdicons->getIconSet( QmvIcons::Left1Icon ) );
    fc_tb->setTextLabel("Previous Page(Prior)");
    fc_tb->setAccel( Key_Prior );
    QWhatsThis::add( fc_tb, QString("<b>Previous:</b>.<br>"
                     "Go to the previous page") );
    connect( fc_tb, SIGNAL( clicked() ), this, SLOT( slotPrevPage() ) );
    
        // Next Page
    fc_tb = new QToolButton( hl_tools, "formcontrol_nextpage");
    fc_tb->setAutoRaise( true );
    fc_tb->setIconSet( stdicons->getIconSet( QmvIcons::Right1Icon ) );
    fc_tb->setTextLabel("Next Page(Next)");
    fc_tb->setAccel( Key_Next );
    QWhatsThis::add( fc_tb, QString("<b>Next:</b>.<br>"
                     "Go to the next page") );
    connect( fc_tb, SIGNAL( clicked() ), this, SLOT( slotNextPage() ) );
    
        // Final Page
    fc_tb = new QToolButton( hl_tools, "formcontrol_lastpage");
    fc_tb->setAutoRaise( true );
    fc_tb->setIconSet( stdicons->getIconSet( QmvIcons::Right2Icon ) );
    fc_tb->setTextLabel("Final Page(End)");
    fc_tb->setAccel( Key_End );
    QWhatsThis::add( fc_tb, QString("<b>Final:</b>.<br>"
                     "Go to the final page") );
    connect( fc_tb, SIGNAL( clicked() ), this, SLOT( slotLastPage() ) );
    
        // Print form
    fc_tb = new QToolButton( hl_tools, "formcontrol_print");
    fc_tb->setAutoRaise( true );
    fc_tb->setIconSet( stdicons->getIconSet( QmvIcons::PrintIcon ) );
    fc_tb->setTextLabel("Print (P)");
    fc_tb->setAccel( Key_P );
    QWhatsThis::add( fc_tb, QString("<b>Print:</b>.<br>"
                     "Print Page(s)") );
    connect( fc_tb, SIGNAL( clicked() ), this, SLOT( slotPrintReport() ) );

        // Fax form
    fc_tb = new QToolButton( hl_tools, "formcontrol_fax");
    fc_tb->setAutoRaise( true );
    fc_tb->setIconSet( stdicons->getIconSet( QmvIcons::FaxIcon ) );
    fc_tb->setTextLabel("Fax (F)");
    fc_tb->setAccel( Key_F );
    QWhatsThis::add( fc_tb, QString("<b>Fax:</b>.<br>"
                     "Fax Page(s) to a particular destination") );
    connect( fc_tb, SIGNAL( clicked() ), this, SLOT( slotFaxReport() ) );

        // Email form
    fc_tb = new QToolButton( hl_tools, "formcontrol_email");
    fc_tb->setAutoRaise( true );
    fc_tb->setIconSet( stdicons->getIconSet( QmvIcons::EmailIcon ) );
    fc_tb->setTextLabel("Email (E)");
    fc_tb->setAccel( Key_E );
    QWhatsThis::add( fc_tb, QString("<b>Email:</b>.<br>"
                     "Email Page(s) to a particular address") );
    connect( fc_tb, SIGNAL( clicked() ), this, SLOT( slotEmailReport() ) );

            // page counter - stretch
    banner =  new QLabel("   Forms - 0/0", hl_tools, "Form Toolbar Label");
    hl_tools->setStretchFactor( banner, 10 );
    

        // Whats this
    fc_tb = QWhatsThis::whatsThisButton(hl_tools);
    fc_tb->setAutoRaise( true );
    fc_tb->setAccel( Key_F1 );
    fc_tb->setFocusPolicy( QWidget::NoFocus );
    QWhatsThis::add( fc_tb, "<b>What's This ?:</b>. "
                     " Describe objects on this window.");

        // Close
    fc_tb = new QToolButton( hl_tools, "formcontrol_close");
    fc_tb->setAutoRaise( true );
    fc_tb->setIconSet( stdicons->getIconSet( QmvIcons::QuitIcon ) );
    fc_tb->setTextLabel("Close the Viewer");
    fc_tb->setAccel( CTRL+Qt::Key_W );
    fc_tb->setFocusPolicy( QWidget::NoFocus );
    QWhatsThis::add( fc_tb, "<b>Close:</b>. "
                     " Close the form viewer.");
    connect( fc_tb, SIGNAL( clicked() ), this, SLOT( hide() ) );
    
        // add the form
    vl_top->addWidget( formViewer() );
    vl_top->setStretchFactor( formViewer(), 10 );
    setMaximumSize( QApplication::desktop()->width(), QApplication::desktop()->height() );
}  


// Destructor

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QmvForm::~QmvForm()
{
  
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvForm::slotFirstPage()
{
    formViewer()->slotFirstPage();
    if ( page_total > 0 )
        page_current = 1;
    else
        page_current = 0;
    slotSetBannerText( QString("Page %1 of %2 ").arg( page_current ).arg( page_total ) );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvForm::slotPrevPage()
{
    formViewer()->slotPrevPage();
    if ( page_current > 1 )
        page_current--;
    slotSetBannerText( QString("Page %1 of %2 ").arg( page_current ).arg( page_total ) );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvForm::slotNextPage()
{
    formViewer()->slotNextPage();
    if ( page_current < page_total )
        page_current++;
    slotSetBannerText( QString("Page %1 of %2 ").arg( page_current ).arg( page_total ) );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvForm::slotLastPage()
{
    formViewer()->slotLastPage();
    page_current = page_total;
    slotSetBannerText( QString("Page %1 of %2 ").arg( page_current ).arg( page_total ) );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvForm::slotPrintReport()
{
        // can use default printer, so no need to check.
    
        // Set the Print target
    selectTarget( formPrint, "" );
        //execute
    formViewer()->slotPrintReport();
    print_status = true;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvForm::slotFaxReport()
{

    if (ft_fax.program.isEmpty() || ft_fax.options.isEmpty() || ft_fax.name.isEmpty() ) {
        QMessageBox::information( 0, QString( "Configuration Error" ),
                                  QString( tr("The FAX function has not been configured") )
                                  );
        return;
    }

        // Verify the destination number
    bool ok = false;
    QString sendto = QInputDialog::getItem( QString("Fax Destination Number"),
                                            QString("Fax to"),
                                            ft_fax.destination,
                                            0,true,
                                            &ok, this );
    if (!ok) {
        QMessageBox::information( 0, QString( "Fax Cancelled" ),
                                  QString( tr("The FAX function has been cancelled") )
                                  );
        return;
    }
        // Remove from the first right parenthesis
    sendto.replace(QRegExp("(.*$"), "");
        // Remove whitespace
    sendto.replace(QRegExp("[^0-9]"), "");
    
    if ( sendto.isEmpty() ) {
        QMessageBox::information( 0, QString( "Missing Fax number" ),
                                  QString( tr("The FAX destination number is missing") )
                                  );
        return;
    }

        // Print form to a file
    printToFile( ft_fax.name );

        // Send the output file to the fax-handler
    QString fax_program = QString(ft_fax.options)
        .arg(ft_fax.program)
        .arg(sendto)
        .arg(ft_fax.name);
    if (system(fax_program)) {
        QMessageBox::information( 0, QString( "Failed to send FAX" ),
                                  QString( "The output could not be sent to the FAX device (# %1)" )
                                  .arg(sendto)
                                  );
        print_status = false;
    } else {
        QMessageBox::information( 0, QString( "Sent to FAX device" ),
                                  QString( "The output has been sent to the FAX device (# %1)" )
                                  .arg(sendto)
                                  );
        print_status = true;
    }

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvForm::slotEmailReport()
{
    if (ft_email.program.isEmpty() || ft_email.options.isEmpty() || ft_email.name.isEmpty() ) {
        QMessageBox::information( 0, QString( "Configuration Error" ),
                                  QString( tr("The EMAIL function has not been configured") )
                                  );
        return;
    }

        // Verify the destination address
    bool ok = false;
    QString sendto = QInputDialog::getItem( QString("Email Destination Address"),
                                            QString("Email to"),
                                            ft_email.destination,
                                            0,true,
                                            &ok, this );
    if (!ok) {
        QMessageBox::information( 0, QString( "Email Cancelled" ),
                                  QString( tr("The EMAIL function has been cancelled") )
                                  );
        return;
    }
        // Remove leading and trailing whitespace
    sendto = sendto.stripWhiteSpace();
        // Remove after first nonvalid character
    sendto.replace(QRegExp("[^0-9a-z@._-]+.*$", false), "");
    
    if ( sendto.isEmpty() ) {
        QMessageBox::information( 0, QString( "Missing Email address" ),
                                  QString( tr("The EMAIL destination address is missing") )
                                  );
        return;
    }

        // Print form to a file
    printToFile( ft_email.name );

        // Send the output file to the email-handler
    QString email_program = QString(ft_email.options)
        .arg(ft_email.program)
        .arg(sendto)
        .arg(ft_email.name);
    if (system(email_program)) {
        QMessageBox::information( 0, QString( "Failed to send EMAIL" ),
                                  QString( "The output could not be sent to the EMAIL device (# %1)" )
                                  .arg(sendto)
                                  );
        print_status = false;
    } else {
        QMessageBox::information( 0, QString( "Sent to EMAIL device" ),
                                  QString( "The output has been sent to the EMAIL device (# %1)" )
                                  .arg(sendto)
                                  );
        print_status = true;
    }

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvForm::printToFile( const QString & filename )
{
    selectTarget( formFile, filename );
    formViewer()->slotPrintReport();
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvForm::slotSetBannerText( const QString & text )
{
    banner->setText( QString( "%1" ).arg( text, -60 ) );
}


//! Set the parent relation
/*!
  This allows a QmvForm object to be reused for multiple relation objects.

  \param relation       
*/
//--------------------------------------------------------------------------------
void QmvForm::setRelation( QmvSqlClass * relation )
{

    form_data = relation;
}

//--------------------------------------------------------------------------------
bool QmvForm::printedStatus() const
{
    return print_status;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void QmvForm::setPrint( const QString & program, const QString & options, const QString & name )
{
    ft_print.name = name;
    ft_print.options = options;
    ft_print.program = program;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void QmvForm::setFax( const QString & program, const QString & options, const QString & name,
                      const QStringList & destination )
{
    ft_fax.program = program;
    ft_fax.options = options;
    ft_fax.name = name;
    ft_fax.destination = destination;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void QmvForm::setEmail( const QString & program, const QString & options, const QString & name,
                        const QStringList & destination )
{
    ft_email.name = name;
    ft_email.options = options;
    ft_email.program = program;
    ft_email.destination = destination;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void QmvForm::selectTarget( targetType target, const QString & name )
{

    formTarget * ft = 0;
    QString program;
    switch (target)
    {
        case formPrint:
            formViewer()->setPrintProgram( ft_print.program );
            formViewer()->setPrinterSelectionOption( ft_print.options );
            formViewer()->setPrinterName( (name.isEmpty()) ? ft_print.name : name );
            formViewer()->setOutputFileName( QString::null );
            break;
        case formFile:
            formViewer()->setPrintProgram( QString::null );
            formViewer()->setPrinterSelectionOption( QString::null );
            formViewer()->setPrinterName( QString::null );
            formViewer()->setOutputFileName( name );
            break;
        default:
            return;
            break;
    };
}


//--------------------------------------------------------------------------------
//! Display a view form.
/*!
  Load and display a form

*/
// --------------------------------------------------------------------------------
bool QmvForm::viewForm( bool init)
{

    if ( init )
    {
        if ( form_name.length() )
        {
            if ( !initForm( form_name ) )
                return(FALSE);
        } else
        {
            if ( !initForm() )
                return(FALSE);
        }    
    }
    
    if (!setTemplate())
    {
        QMessageBox::information( 0, QString( "Fatal Error" ),
                               QString( tr("Form Specifications could not be understood") )
                               );
        return(false);
    }
    
    if (!setData())
    {
        QMessageBox::information( 0, QString( "Fatal Error" ),
                               QString( tr("Form Data could not be understood") )
                               );
        return(false);
    }
    
    renderForm();
    formViewer()->show();
    formViewer()->raise();
    formViewer()->adjustSize();
    formViewer()->updateGeometry();
    qApp->processEvents();  // needed to get some first display processed correctly.
    adjustSize();
    updateGeometry();
    setFocus();
    page_total = formViewer()->pageCount();
    slotFirstPage();
    print_status = false;

        // this status is not used - the real status is whether printed or not.
    int status = exec();
}


//--------------------------------------------------------------------------------
//! Print a form
/*!
  Load and print a form.

  \param init   true=initialise object
  \return       status
*/
//--------------------------------------------------------------------------------
bool QmvForm::printForm( bool init )
{
    if (init)
    {        
        if ( form_name.length() )
        {
            if ( !initForm( form_name ) )
                return(false);
        } else
        {
            if ( !initForm() )
                return(false);
        }
    }
    
    if (!setTemplate())
        return(false);
    if (!setData())
        return(false);
    
    renderForm();
    formViewer()->printReport();
    print_status = true;
    
    return(true);
}


//--------------------------------------------------------------------------------
//! Edit text fields in a named form.
/*!
  Edit values of fixed text fields in a form - prior to building the form

  \param form   
  \return TRUE/FALSE
*/
// --------------------------------------------------------------------------------
bool QmvForm::editFixedFields( const QString form_detail)
{
    int n_fields = reln_fmdt->open( QString( "select * from fmdt where %1 and fmdt_desc ~ '^=' order by fmdt_desc" )
                                    .arg( form_detail ) );

    if (n_fields < 1)
        return true;

    QString x_title = reln_form->attributeValue(0,"form_desc");

    QmvFormEditFields * edit_static = new QmvFormEditFields( this->parentWidget(), x_title);
    

    for (int y = 0; y < n_fields; y++) {
            // labels
        QString label = reln_fmdt->attributeValue(y,"fmdt_desc");
            // yes, it should begin with "="
        if (label.startsWith("="))
            label.remove(0,1);
        QLabel * ptr_lab = new QLabel(label, edit_static);
        ptr_lab->setAlignment( AlignCenter );
        ptr_lab->setFrameStyle( QFrame::Box | QFrame::Raised );
            //grid_editrow->( ptr_lab, y, 0 );

            // fields
        QmvMultiLineEdit * editor =  new QmvMultiLineEdit( reln_fmdt->tupleAttribute(y,"fmdt_text"),
                                                           edit_static, QString("fmdt_text :%1").arg(y));
        editor->setFixedHeight(30);
        editor->deselect();
        edit_static->addEditorRow( ptr_lab, editor, y);
        if (y == 0)
            editor->setFocus();
    }

    edit_static->setFixedSize( edit_static->sizeHint() );
        // Run it
    int status = edit_static->exec();
        // remove
    
    delete edit_static;

        // probably not needed - save occures after each field
    if ( status ) {
        reln_fmdt->save();
        return true;
    }

    return false;
}



//--------------------------------------------------------------------------------
//! Initialise a form based on parent relation.
/*!
  Popup a list of form options if more than one form is found.

  \return TRUE/FALSE
*/
// --------------------------------------------------------------------------------
bool QmvForm::initForm()
{
    reln_form->open( QString( "select * from form where form_classname = '%1'" )
                     .arg( form_data->relationName() ) );


        // if more than 1 row, popup selector
    int row;
    int formcount = reln_form->count();
    if ( formcount == 1 )
        row = 0;
    else if ( formcount > 1 )
    {
        QmvSelect form_select( reln_form, this, "Form selector" );
        row = form_select.exec();
    }
    else
        return(FALSE); // no forms
    
    if ( row < 0 )
        return(false);

        // Get the form_code
    QString form;
    if ( row >= 0 && row < reln_form->count() )
        form = reln_form->attributeValue( row, "form_code" );
    else
    {
        QMessageBox::information( 0, QString( "Fatal Error - Form Specifications not found" ),
                               QString( tr("Cannot locate a form record  <%1>") )
                               .arg( form ) );
        return(false);
    }

    return initForm( form );
}


//--------------------------------------------------------------------------------
//! Initialise a named form.
/*!
  Initialise a named form and load with the data from the parent relation.

  \param form   
  \return TRUE/FALSE
*/
// --------------------------------------------------------------------------------
QString QmvForm::formDetailList()
{
    if ( reln_form->count() < 1 || !form_name.length() )
        return QString("false");
    
    QString form_detail = QString ( "%1 %2" )
        .arg( form_name ).arg( reln_form->attributeValue( 0, "form_includes" ) );
    form_detail.simplifyWhiteSpace();
    form_detail.replace( QRegExp("[, ]"), "','" );
    form_detail = QString( "fmdt_form_code in ( '%1')" )
        .arg( form_detail );
    return form_detail;
}


//--------------------------------------------------------------------------------
//! Initialise a named form.
/*!
  Initialise a named form and load with the data from the parent relation.

  \param form   
  \return TRUE/FALSE
*/
// --------------------------------------------------------------------------------
bool QmvForm::initForm( const QString & form )
{
    form_name = form;
    form_template_name = QString( "%1.kut" ).arg( form );
    form_data_name = QString( "%1.kud" ).arg( form );
    form_template_str = "";
    form_data_str = "";

        // -----------------------------------------------------
        // Prepare the header set
        // -----------------------------------------------------
    reln_form->open( QString( "select * from form where form_code = '%1'::text" ).arg( form ) );
    if ( reln_form->count() != 1)
    {
        QMessageBox::information( 0, QString( "Fatal Error - Form Specifications not found" ),
                                  QString( tr("Cannot locate a form record  <%1>") )
                                  .arg( form ) );
        return(FALSE);
    }
        // Look for includes
    QString form_detail = formDetailList();

        // -----------------------------------------------------
        // Are there any editable fixed strings?
        // -----------------------------------------------------
    if (!editFixedFields(form_detail))
    {
        return(false);
    }
    
        // -----------------------------------------------------
        // Open the template string for writing
        // -----------------------------------------------------

    QTextStream tt( & form_template_str, IO_WriteOnly );

        // -----------------------------------------------------
        // Open the data string for writing
        // -----------------------------------------------------

    QTextStream td( & form_data_str, IO_WriteOnly );
    
        // -----------------------------------------------------
        // Prepare the Template - preamble
        // -----------------------------------------------------
    QString format;
        // Header
    tt << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl << endl;
    tt << "<!DOCTYPE KugarTemplate SYSTEM \"kugartemplate.dtd\">" << endl;
    format = QString("<KugarTemplate PageSize=\"%1\" PageOrientation=\"%2\" TopMargin=\"%3\"" )
        .arg( reln_form->attributeValue( 0, "form_pagesize" ) )
        .arg( reln_form->attributeValue( 0, "form_orientation" ) )
        .arg( reln_form->attributeValue( 0, "form_mg_top" ) );
    tt << format  << endl;
    format = QString( "  BottomMargin=\"%1\" LeftMargin=\"%2\" RightMargin=\"%3\">" )
        .arg( reln_form->attributeValue( 0, "form_mg_bottom" ) )
        .arg( reln_form->attributeValue( 0, "form_mg_left" ) )
        .arg( reln_form->attributeValue( 0, "form_mg_right" ) );
    tt << format << endl;
    tt << endl;

    FieldType field_type;
    
        // -----------------------------------------------------
        // Prepare the Template - report header section
        // -----------------------------------------------------
    format = QString( "  <ReportHeader Height=\"%1\" PrintFrequency=\"%2\">" )
        .arg( reln_form->attributeValue( 0, "form_rh_height" ) )
        .arg( reln_form->attributeValue( 0, "form_rh_frequency" ) );
    tt << format  << endl;

    reln_fmdt->open( QString( "select * from fmdt where %1 and fmdt_section = '0'" ).arg( form_detail ) );
    for ( int i = 0; i < reln_fmdt->count(); i++ )
        tt << formatSelect( reln_fmdt, i );

    tt << "  </ReportHeader>" << endl;
    
        // -----------------------------------------------------
        // Prepare the Template - page header section
        // -----------------------------------------------------
    format = QString( "  <PageHeader Height=\"%1\" PrintFrequency=\"%2\">" )
        .arg( reln_form->attributeValue( 0, "form_ph_height" ) )
        .arg( reln_form->attributeValue( 0, "form_ph_frequency" ) );
    tt << format  << endl;
        
    reln_fmdt->open( QString( "select * from fmdt where %1 and fmdt_section = '1'" ).arg( form_detail ) );
    for ( int i = 0; i < reln_fmdt->count(); i++ )
        tt << formatSelect( reln_fmdt, i );
    tt << "  </PageHeader>" << endl;
    
        // -----------------------------------------------------
        // Prepare the Template - detail section
        // -----------------------------------------------------
    format = QString( "  <Detail Height=\"%1\">" )
        .arg( reln_form->attributeValue( 0, "form_dt_height" ) );
    tt << format  << endl;
        
    reln_fmdt->open( QString( "select * from fmdt where %1 and fmdt_section = '2'" ).arg( form_detail ) );
    for ( int i = 0; i < reln_fmdt->count(); i++ )
        tt << formatSelect( reln_fmdt, i ) << endl;
    tt << "  </Detail>" << endl;
    
        // -----------------------------------------------------
        // Prepare the Template - page footer section
        // -----------------------------------------------------
    format = QString( "  <PageFooter Height=\"%1\" PrintFrequency=\"%2\">" )
        .arg( reln_form->attributeValue( 0, "form_pf_height" ) )
        .arg( reln_form->attributeValue( 0, "form_pf_frequency" ) );
    tt << format  << endl;
        
    reln_fmdt->open( QString( "select * from fmdt where %1 and fmdt_section = '3'" ).arg( form_detail ) );
    for ( int i = 0; i < reln_fmdt->count(); i++ )
        tt << formatSelect( reln_fmdt, i ) << endl;
    tt << "  </PageFooter>" << endl;

       
    
        // -----------------------------------------------------
        // Prepare the Template - Report Footer section
        // -----------------------------------------------------
    reln_fmdt->open( QString( "select * from fmdt where %1 and fmdt_section = '4'" ).arg( form_detail ) );
    format = QString( "  <ReportFooter Height=\"%1\" PrintFrequency=\"%2\">" )
        .arg( reln_form->attributeValue( 0, "form_rf_height" ) )
        .arg( reln_form->attributeValue( 0, "form_rf_frequency" ) );
    tt << format  << endl;
        
    for ( int i = 0; i < reln_fmdt->count(); i++ )
        tt << formatSelect( reln_fmdt, i ) << endl;
    tt << "  </ReportFooter>" << endl;

    tt << "</KugarTemplate>" << endl;
    
    
        // -----------------------------------------------------
        // Prepare the Data - field specifications
        // -----------------------------------------------------
    
    reln_fmdt->open( QString( "select * from fmdt where %1"
                              " and fmdt_section = '%2' and fmdt_field_type = '%3'" )
                     .arg( form_detail )
                     .arg( Detail )
                     .arg( Field ) );
        // Header
    td << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl << endl;
    td << "<!DOCTYPE KugarData [" << endl;
    
    td << "  <!ELEMENT KugarData (Row* )>" << endl;
    td << "  <!ATTLIST KugarData" << endl;
    td << "    Template CDATA #REQUIRED>" << endl;
    td << endl;
    td << "  <!ELEMENT Row EMPTY>" << endl;

        
    int max_repeat = 0;         // The largest repeat value
    QString page_token = "_PAGE";
    QString item_token = "_ITEM";
    int items_per_page = 1;     // Subset page capacity
    int page_counter = 0;       // Sub-set page counter
    int item_counter = 0;       // Sub-set item counter
    int page_total = 0;         // Sub-set page total
    for ( int i = 0; i < reln_fmdt->count(); i++ )
    {
        if ( i == 0)
            td << "  <!ATTLIST Row " << endl;
        QString fmdt_text = reln_fmdt->attributeValue( i, "fmdt_text" );
        
        int repeat = parseRepeat( &fmdt_text );
        if ( repeat > max_repeat)
            max_repeat = repeat;
        
        for ( int k = 0; k <= repeat; k++ )
        {
            format = QString( "    %1_%2 CDATA #REQUIRED" )
                .arg( fmdt_text )
                .arg( k );
            td << format;
            if ( k < repeat )
                td << endl;
        }
        
        if ( i+1 == reln_fmdt->count() )
            td << ">";
        td << endl;
    }
    td << "]>" << endl << endl;

        // determine page capacity
    items_per_page = max_repeat + 1;
    
    format = QString("<KugarData Template=\"%1\">" ).arg( form_template_name );
        
    td << format << endl;

    if ( reln_fmdt->count() > 0 )
    {
            // Check for break variables
        int brkflag = 0; // 0 = no, 1 = yes
            // cleanup var list
            // - it is of the form   var1,var2,...:varn,varn+1,...
            // - where the ":" separates "paging" from "non-paging" sort-variables
        QString str_varlist = reln_form->attributeValue( 0, "form_page_attribute" );
        str_varlist = str_varlist.simplifyWhiteSpace();
        str_varlist.replace( QRegExp(":.*$"), "");
        str_varlist.replace( QRegExp(", *"), "," );
        QStringList pb_att_varlist = QStringList::split( QRegExp(","), str_varlist );
        QStringList pb_att_vallist;
        
            // check the attributes in list
        for ( int bl = 0; bl < pb_att_varlist.count(); bl++ )
            if ( form_data->attribute( pb_att_varlist[bl] ) )
                brkflag = 1;        // we have a page-break attribute.
            else
                brkflag = 0;

        page_counter = 0;       // built page count
        item_counter = 0;       // item count on the current page
            // Scan the dataset
        for ( int j = 0; j < form_data->count(); j++ )
        {
                // configured to break
            if ( brkflag )
            {
                    // build the new value list
                QStringList newvallist;
                newvallist.clear();
                for ( int bl = 0; bl < pb_att_varlist.count(); bl++ )
                    newvallist += form_data->attributeValue( j, pb_att_varlist[bl] );

                    // the break attribute has changed
                if (pb_att_vallist != newvallist )
                {
                        // Page breaks
                    if ( j > 0)
                        td << "  <row ACTION=\"FF\"/>" << endl;
                    
                        // Reset counters
                    pb_att_vallist = newvallist;
                    page_counter = 0;
                    item_counter = 0;
                    int item_total = 1;
                
                        // calculate page count
                    for ( int tot = j+1; tot < form_data->count(); tot++ )
                    {
                        QStringList newvallist;
                        newvallist.clear();
                        for ( int bl = 0; bl < pb_att_varlist.count(); bl++ )
                            newvallist += form_data->attributeValue( tot, pb_att_varlist[bl] );
                        if ( pb_att_vallist != newvallist )
                            break;
                        else
                            item_total++;
                    }
                    
                    page_total = ( (item_total - 1) / items_per_page ) + 1;
                }
            }
        
            
            page_counter++;
            item_counter++;
            td << "  <row";
                // store the non-repeating fields first.
            for ( int i = 0; i < reln_fmdt->count(); i++ )
            {
                QString var = reln_fmdt->attributeValue( i, "fmdt_text" );
                int repeat = parseRepeat( &var );
                if ( repeat )
                    continue;

                td << endl << "    ";
                QString val;
                
                if ( var == page_token )
                    val = QString( "%1 of %2" ).arg( page_counter ).arg( page_total );
                else if ( var == item_token )
                    val = QString( "%1" ).arg( item_counter );
                else
                    val = form_data->attributeValue( j, var );
                
                escapeXML( &var );
                escapeXML( &val);
                
                td << QString( " %1_0=\"%2\"" ).arg( var ).arg( val );
            }
                // now the repeating fields

            int j_plus;         // how far into set?
            j_plus = 0;         // how far into set?
            for ( int i = 0; i < reln_fmdt->count(); i++ )
            {
                QString var = reln_fmdt->attributeValue( i, "fmdt_text" );
                int repeat = parseRepeat( &var );
                if ( !repeat )
                    continue;

                int incn;               // incursion counter
                incn = 0;               // incursion counter
                for ( int k = 0; k <= repeat; k++ )
                {
                    td << endl << "    ";
                    QString val;

                        // Repeating page counters ???
                    if ( var == page_token )
                        val = QString( "%1 of %2" ).arg( page_counter ).arg( page_total );
                    else if ( var == item_token )
                        val = QString( "%1" ).arg( item_counter+k );
                    else
                        val = form_data->attributeValue( j+k, var );
                    
                    escapeXML( &var );
                    escapeXML( &val);
                
                    td << QString( " %1_%2=\"%3\"" ).arg( var ).arg( k ).arg( val );
                    incn = k;
                    
                        // Skip repeats if no break attribute defined.
                    if ( !brkflag )
                        break;

                        // End of set
                    if ( j+k+1 >= form_data->count() )
                        break;

                        // End of break set.
                    QStringList newvallist;
                    newvallist.clear();
                    for ( int bl = 0; bl < pb_att_varlist.count(); bl++ )
                        newvallist += form_data->attributeValue( j+k+1, pb_att_varlist[bl] );
                    if ( pb_att_vallist != newvallist )
                        break;
                    
                }
                    // remember the longest incursion into set
                if ( incn > j_plus )
                    j_plus = incn;
                
            }
                // push the set-pointer to furthest incursion in repeat section.
            j += j_plus;
            item_counter += j_plus;
            td << "/>" << endl;
            
        }
    }
        
        
    td << "</KugarData>" << endl;
    return(TRUE);
}

// --------------------------------------------------------------------------------
//! Parse a string for a "repeat" suffix
/*!
  Parse a string of the form ATTNAME{:REPEAT}. Return the in value of
  REPEAT if it exists, else return 0. Trim the suffix from the string.

  \param str    Pointer to string
  \return       Repeat value.
*/
//--------------------------------------------------------------------------------
int QmvForm::parseRepeat( QString * str )
{
    int repeat = 0;
    int pos_suffix = str->find( ":" );
    if ( pos_suffix > 0 )
    {
        int len_suffix = str->length() - pos_suffix - 1;
        repeat = str->right( len_suffix ).toInt();
        str->remove( pos_suffix, len_suffix + 1 );
    }
    return repeat;
}

// --------------------------------------------------------------------------------
//! Escape XML characters
/*!

  \param str    
*/
//--------------------------------------------------------------------------------
void QmvForm::escapeXML( QString * str )
{
    str->replace( QRegExp( "&" ), "&amp;" );
    str->replace( QRegExp( "<" ), "&lt;" );
    str->replace( QRegExp( "'" ), "&apos;" );
    str->replace( QRegExp( "\"" ), "&quot;" );
}


// --------------------------------------------------------------------------------
//! Select format method base on field type (Internal)
/*!

  An internal method containing a switch block.

  \param reln_fmdt      
  \param row    
  \return <ReturnValue>
*/
//--------------------------------------------------------------------------------
QString QmvForm::formatSelect( QmvSqlClass * reln_fmdt, int row )
{
    QString format;             // the formated XML string
    QString varname;            // a variable name
    QString value;              // the value of a variable
    QString multisep = "~";     // separator in multi-variable fields.
    QStringList multilist;      // the list of tokens in multi-variable fields.
    
    format = "";                // initialise
    int section = reln_fmdt->attributeValue( row, "fmdt_section" ).toInt();
    
    switch ( reln_fmdt->attributeValue( row, "fmdt_field_type" ).toInt() )
    {
        case Field:
            varname = reln_fmdt->attributeValue( row, "fmdt_text" );
            if ( section == Detail )
            {
                int repeat = parseRepeat( &varname );
                for ( int k = 0; k <= repeat; k++ )
                    format.append( formatField( reln_fmdt, row, varname, k ) );
                
            } else
            {
                    // If not in the detail section, then variables can be evaluated and
                    // handled as labels.
                multilist = QStringList::split( multisep, varname );
                value = "";
                for ( QStringList::Iterator it = multilist.begin(); it != multilist.end(); ++it )
                {
                        // if attribute use value, else literal
                    if ( form_data->attribute( (*it) ) )
                        value += form_data->attributeValue( 0, (*it) );
                    else
                        value += (*it);
                }
                
                value.replace( QRegExp( "&" ), "&amp;" );
                value.replace( QRegExp( "<" ), "&lt;" );
                value.replace( QRegExp( "'" ), "&apos;" );
                value.replace( QRegExp( "\"" ), "&quot;" );
                format = formatLabel( reln_fmdt, row, value );   
            }

            break;
        case Label:
            value = reln_fmdt->attributeValue( row, "fmdt_text" );
            value.replace( QRegExp( "&" ), "&amp;" );
            value.replace( QRegExp( "<" ), "&lt;" );
            value.replace( QRegExp( "'" ), "&apos;" );
            value.replace( QRegExp( "\"" ), "&quot;" );
            format = formatLabel( reln_fmdt, row, value );
            break;
        case Line:
            format = formatLine( reln_fmdt, row );
            break;
        case Count:
        case Sum:
        case Average:
        case Variance:
        case StdDeviation:
            format = formatCalculationType( reln_fmdt, row,
                                            (FieldType) reln_fmdt->attributeValue( row, "fmdt_field_type" )
                                            .toInt() );
            break;
        case Date:
        case Page:
            format = formatSpecial( reln_fmdt, row,
                                    (FieldType) reln_fmdt->attributeValue( row, "fmdt_field_type" )
                                    .toInt() );
            break;
        default:
            break;
    }
    return format;
}

// --------------------------------------------------------------------------------
//! Prepare format template for \em LABEL type.
/*!

  \param reln_fmdt      FMDT object
  \param row            Row number - 0-based, refereds to datarow in fmdt object (not formrow)
  \param value          Literal value ( from fmdt object)
  \return               The XML format string.
*/
//--------------------------------------------------------------------------------
QString QmvForm::formatLabel( QmvSqlClass * reln_fmdt, int row, const QString & value )
{
    QString format;
    
    format = QString( "    <Label Text=\"%1\" X=\"%2\" Y=\"%3\" Width=\"%4\" Height=\"%5\"\n" )
        .arg( value )
        .arg( reln_fmdt->attributeValue( row, "fmdt_x_coord" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_y_coord" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_width" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_height" ) );

    format.append( QString( "           BackgroundColor=\"%1\" ForegroundColor=\"%2\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bg_colour" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_fg_colour" ) ) );

    format.append( QString( "           BorderColor=\"%1\" BorderWidth=\"%2\" BorderStyle=\"%3\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bd_colour" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bd_width" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bd_style" ) ) );

    format.append( QString( "           FontFamily=\"%1\" FontSize=\"%2\" FontWeight=\"%3\""
                      " FontItalic=\"%4\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_family" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_size" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_weight" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_italic" ) ) );

    format.append( QString( "           HAlignment=\"%1\" VAlignment=\"%2\" WordWrap=\"%3\"/>\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_hz_align" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_vt_align" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_word_wrap" ) ) );

    return format;
}

//--------------------------------------------------------------------------------
//! Prepare format template for \em FIELD type.
/*!

  \param reln_fmdt      FMDT object
  \param row            Row number - 0-based, refereds to datarow in fmdt object (not formrow)
  \param value          Data value
  \param repeat         Repeat value
  \return The XML format string.
*/
//--------------------------------------------------------------------------------
QString QmvForm::formatField( QmvSqlClass * reln_fmdt, int row, const QString & value, int repeat )
{
    QString format;
        
    format = QString( "    <Field Field=\"%1_%2\" Text=\"\" X=\"%3\" Y=\"%4\""
                      " Width=\"%4\" Height=\"%5\"\n" )
        .arg( value )
        .arg( repeat )
        .arg( reln_fmdt->attributeValue( row, "fmdt_x_coord" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_y_coord" ).toInt()
              + ( repeat * reln_fmdt->attributeValue( row, "fmdt_height" ).toInt() ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_width" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_height" ) );

    format.append( QString( "           BackgroundColor=\"%1\" ForegroundColor=\"%2\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bg_colour" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_fg_colour" ) ) );

    format.append( QString( "           BorderColor=\"%1\" BorderWidth=\"%2\" BorderStyle=\"%3\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bd_colour" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bd_width" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bd_style" ) ) );

    format.append( QString( "           FontFamily=\"%1\" FontSize=\"%2\" FontWeight=\"%3\""
                      " FontItalic=\"%4\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_family" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_size" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_weight" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_italic" ) ) );

    format.append( QString( "           HAlignment=\"%1\" VAlignment=\"%2\" WordWrap=\"%3\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_hz_align" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_vt_align" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_word_wrap" ) ) );

    format.append( QString( "           DataType=\"%1\" DateFormat=\"%2\" Precision=\"%3\""
                      " Currency=\"%4\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_data_type" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_date_format" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_precision" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_currency" ) ) );

    format.append( QString( "           NegValueColor=\"%1\" CommaSeparator=\"%2\"/>\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_neg_colour" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_comma_sep" ) ) );

    return format;

}

//--------------------------------------------------------------------------------
//! Prepare format template for \em LINE type.
/*!

  \param reln_fmdt      
  \param row    
  \return The XML format string.
*/
//--------------------------------------------------------------------------------
QString QmvForm::formatLine( QmvSqlClass * reln_fmdt, int row )
{
    QString format;
    
    int x = reln_fmdt->attributeValue( row, "fmdt_x_coord" ).toInt();
    int y = reln_fmdt->attributeValue( row, "fmdt_y_coord" ).toInt();
    int w = reln_fmdt->attributeValue( row, "fmdt_width" ).toInt();
    int h = reln_fmdt->attributeValue( row, "fmdt_height" ).toInt();
    int t = reln_fmdt->attributeValue( row, "fmdt_bd_width" ).toInt();
    int s = reln_fmdt->attributeValue( row, "fmdt_bd_style" ).toInt();
        
    format = QString( "    <Line X1=\"%1\" Y1=\"%2\" X2=\"%3\" Y2=\"%4\"\n" )
        .arg( x )
        .arg( y )
        .arg( x+w )
        .arg( y+h );

    format.append( QString( "           Width=\"%1\" Color=\"%2\" Style=\"%3\"/>\n" )
                   .arg( t )
                   .arg(reln_fmdt->attributeValue( row, "fmdt_bd_colour" ) )
                   .arg( s ) );


    return format;
    
}


//--------------------------------------------------------------------------------
//! Prepare format template for \em CALCULATION data type.
/*!

  \param reln_fmdt      
  \param row    
  \param type   
  \return The XML format string.
*/
//--------------------------------------------------------------------------------
QString QmvForm::formatCalculationType( QmvSqlClass * reln_fmdt, int row , FieldType type )
{
    QString format;
    QString value = reln_fmdt->attributeValue( row, "fmdt_text" );
    value.replace( QRegExp( "&" ), "&amp;" );
    value.replace( QRegExp( "<" ), "&lt;" );
    value.replace( QRegExp( "'" ), "&apos;" );
    value.replace( QRegExp( "\"" ), "&quot;" );
    
    format = QString( "    <CalculatedField CalculationType=\"%1\" Field=\"%2\" Text=\"\" X=\"%3\" Y=\"%4\""
                      " Width=\"%5\" Height=\"%6\"\n" )
        .arg( type % CalculatedField )
        .arg( value )
        .arg( reln_fmdt->attributeValue( row, "fmdt_x_coord" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_y_coord" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_width" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_height" ) );

    format.append( QString( "           BackgroundColor=\"%1\" ForegroundColor=\"%2\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bg_colour" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_fg_colour" ) ) );

    format.append( QString( "           BorderColor=\"%1\" BorderWidth=\"%2\" BorderStyle=\"%3\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bd_colour" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bd_width" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bd_style" ) ) );

    format.append( QString( "           FontFamily=\"%1\" FontSize=\"%2\" FontWeight=\"%3\""
                      " FontItalic=\"%4\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_family" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_size" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_weight" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_italic" ) ) );

    format.append( QString( "           HAlignment=\"%1\" VAlignment=\"%2\" WordWrap=\"%3\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_hz_align" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_vt_align" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_word_wrap" ) ) );

    format.append( QString( "           DataType=\"%1\" DateFormat=\"%2\" Precision=\"%3\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_data_type" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_date_format" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_precision" ) ) );

    format.append( QString( "           Currency=\"%1\" NegValueColor=\"%2\" CommaSeparator=\"%3\"/>\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_currency" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_neg_colour" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_comma_sep" ) ) );

    return format;
    
}


//--------------------------------------------------------------------------------
//! Prepare format template for \em SPECIAL data type.
/*!

  \param reln_fmdt      
  \param row    
  \param type   
  \return The XML format string.
*/
//--------------------------------------------------------------------------------
QString QmvForm::formatSpecial( QmvSqlClass * reln_fmdt, int row , FieldType type )
{
    QString format;
    
    format = QString( "    <Special Type=\"%1\" Text=\"\" X=\"%2\" Y=\"%3\""
                      " Width=\"%4\" Height=\"%5\"\n" )
        .arg( type % Special )
        .arg( reln_fmdt->attributeValue( row, "fmdt_x_coord" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_y_coord" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_width" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_height" ) );

    format.append( QString( "           BackgroundColor=\"%1\" ForegroundColor=\"%2\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bg_colour" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_fg_colour" ) ) );

    format.append( QString( "           BorderColor=\"%1\" BorderWidth=\"%2\" BorderStyle=\"%3\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bd_colour" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bd_width" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_bd_style" ) ) );

    format.append( QString( "           FontFamily=\"%1\" FontSize=\"%2\" FontWeight=\"%3\""
                      " FontItalic=\"%4\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_family" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_size" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_weight" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_ft_italic" ) ) );

    format.append( QString( "           HAlignment=\"%1\" VAlignment=\"%2\" WordWrap=\"%3\"\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_hz_align" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_vt_align" ) )
        .arg( reln_fmdt->attributeValue( row, "fmdt_word_wrap" ) ) );

    format.append( QString( "           DateFormat=\"%1\"/>\n" )
        .arg( reln_fmdt->attributeValue( row, "fmdt_date_format" ) ) );

    return format;

}

//--------------------------------------------------------------------------------
//! Transfer the current template to the form viewer.
/*!
  This does not render the form. \ref renderForm().

*/
//--------------------------------------------------------------------------------
bool QmvForm::setTemplate()
{
    return formViewer()->setReportTemplate( form_template_str );
}

//--------------------------------------------------------------------------------
//! Transfer the current data to the form viewer.
/*!
  This does not render the form. \ref renderForm().

*/
//--------------------------------------------------------------------------------
bool QmvForm::setData()
{
    return formViewer()->setReportData( form_data_str );
}

//--------------------------------------------------------------------------------
//! Write the current form template to a file.
/*!
  Write the template to a file in XML format.

  \return TRUE/FALSE
*/
//--------------------------------------------------------------------------------
bool QmvForm::writeTemplate()
{
    
    QFile ft( form_template_name );
    if ( !ft.open( IO_WriteOnly ) )
    {
        QMessageBox::information( 0, QString( "Fatal Error - File open" ),
                               QString( tr("Cannot open the template file <%1> for writing") )
                               .arg( form_template_name ) );
        return(FALSE);
    }
    QTextStream tt( &ft );
    tt << form_template_str;
    ft.close();
    return(TRUE);
}

//--------------------------------------------------------------------------------
//! Write the current form data to a file.
/*!
  Write the data to a file in XML format.

  \return TRUE/FALSE
*/
//--------------------------------------------------------------------------------
bool QmvForm::writeData()
{
    
    QFile fd( form_data_name );
    if ( !fd.open( IO_WriteOnly ) )
    {
        QMessageBox::information( 0, QString( "Fatal Error - File open" ),
                                  QString( tr("Cannot open the data file <%1> for writing") )
                                  .arg( form_data_name ) );
        return(FALSE);
    }
    QTextStream td( & fd );
    td << form_data_str;
    fd.close();
    return(TRUE);
}

//--------------------------------------------------------------------------------
//! Render the current form
/*!
  Render the current form onto the viewer object

*/
//--------------------------------------------------------------------------------
void QmvForm::renderForm()
{
    formViewer()->renderReport();;
    formViewer()->adjustSize();
    formViewer()->updateGeometry();
    adjustSize();
    updateGeometry();
}


//--------------------------------------------------------------------------------
//! Return the form viewer object
/*!
  Create the object if it does not exist.

*/
//--------------------------------------------------------------------------------
MReportViewer * QmvForm::formViewer()
{
    if ( !form_viewer )
        form_viewer = new MReportViewer(this);
    return form_viewer;
}

