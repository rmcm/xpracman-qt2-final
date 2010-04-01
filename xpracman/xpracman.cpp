/*
 *     $Id: xpracman.cpp 3383 2007-03-18 11:13:29Z source $
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

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char xpracman_cpp_version[] = "$Id: xpracman.cpp 3383 2007-03-18 11:13:29Z source $";


/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */


extern "C"
{
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#ifdef __NetBSD__
#include <fcntl.h>
#endif
#ifdef linux    
#include <sys/file.h>
#endif    
}
    
// Local includes

#include <qfiledialog.h>
#include "version.h"
#include "xpracman.h"
#include "images/xpracman_logo.xpm"
#include "images/logo_xpracman_light.xpm"
#include "images/logo_xpracman_dark.xpm"

namespace XpracManIcons
{
#include "agenda.xpm"
#include "spreadsheet.xpm"
#include "flag.xpm"
#include "right.xpm"
}

/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */

/*
 * ======================================================================
 * LOCAL FUNCTION PROTOTYPES:
 * ======================================================================
 */

/*
 * ======================================================================
 * LOCAL FUNCTIONS:
 * ======================================================================
 */

/*
 * ======================================================================
 * GLOBAL FUNCTIONS:
 * ======================================================================
 */

/*
 * ======================================================================
 * Constructor
 *
 * Create all child widgets of the XpracMan Widget
 * ======================================================================
 */

XpracMan::XpracMan( const QString & dbtitle, const QmvAccess * dbaccess,
                    QWidget* parent,  const char* initname)
        : QmvApplication( dbtitle, dbaccess, parent, initname ),
          actiongp_appointments(0), popup_appointments(0), toolbar_appointments(0),
          timer_exportmd2(0), doit_exportmd2(false)
{
    if ( !initname )
        setName( "XpracMan" );
    
    if ( caption().isNull() )
        if ( initname )
            setCaption( initname );
        else
            setCaption( "XpracMan" );
   
        // ---- Icon Sets - central

        // ---- Icon Sets - local

        // ---- Icon Sets - file
    
        // ---- Icon Sets - view
    
        // ---- Icon Sets - print
    
        // ------------------------------------------------------------
        // Create an appointment toolbar and menu
        // ------------------------------------------------------------
    
    actiongp_appointments = new QActionGroup( this, "actiongp_appointments" );
    setActionGroupColour( actiongp_appointments, new QColor("red") );
    popup_appointments = new QPopupMenu( this, "popup_appointment:Appointments Menu" );
        //menuBar()->insertItem( "&Appointments", popup_appointments );
    toolbar_appointments = new QToolBar( this, "toolbar_appointments" );
    toolbar_appointments->setHorizontalStretchable(FALSE);
    (void) new QLabel("Appointments ", toolbar_appointments, "Appointments_toolbar_label");
    


        // ------------------------------------------------------------
        // colours
        // ------------------------------------------------------------

    QColor c_file = getActionGroupColour( actiongp_file );
    QColor c_print = getActionGroupColour( actiongp_print );
    QColor c_appointments = getActionGroupColour( actiongp_appointments );
    
        // ------------------------------------------------------------
        // View icons
        // ------------------------------------------------------------
    QIconSet patients_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("P"), QFont("Helvetica"), c_file, QRect() ) );
    QIconSet debtors_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("D"), QFont("Helvetica"), c_file, QRect() ) );
    QIconSet invoice_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("I"), QFont("Helvetica"), c_file, QRect() ) );
    QIconSet unpaid_invoices_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("U"), QFont("Helvetica"), c_file, QRect() ) );
    QIconSet payments_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("$"), QFont("Helvetica"), c_file, QRect() ) );
    QIconSet banking_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("B"), QFont("Helvetica"), c_file, QRect() ) );
    QIconSet waiting_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("W"), QFont("Helvetica"), c_file, QRect() ) );
    QIconSet referrer_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("R"), QFont("Helvetica"), c_file, QRect() ) );
    QIconSet feebook_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("F"), QFont("Helvetica"), c_file, QRect() ) );
    QIconSet patientflag_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("+"), QFont("Helvetica"), c_file, QRect() ) );
    QIconSet employer_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("E"), QFont("Helvetica"), c_file, QRect() ) );
    QIconSet location_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("L"), QFont("Helvetica"), c_file, QRect() ) );
    QIconSet recent_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("R"), QFont("Helvetica"), c_file, QRect() ) );
        // Print icons
    QIconSet p_invoice_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("I"), QFont("Helvetica"), c_print, QRect() ) );
    QIconSet p_statement_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("S"), QFont("Helvetica"), c_print, QRect() ) );
    QIconSet p_label_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("L"), QFont("Helvetica"), c_print, QRect() ) );
    QIconSet p_bankdeposit_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("B"), QFont("Helvetica"), c_print, QRect() ) );
    QIconSet p_contract_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("C"), QFont("Helvetica"), c_print, QRect() ) );
    QIconSet p_claim_pci_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("@"), QFont("Helvetica"), c_print, QRect() ) );

        // Appointment Icons
    QIconSet appointments_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("A"), QFont("Helvetica"), c_appointments, QRect() ) );
    QIconSet providers_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("P"), QFont("Helvetica"), c_appointments, QRect() ) );
    QIconSet locations_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("L"), QFont("Helvetica"), c_appointments, QRect() ) );
    QIconSet types_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("T"), QFont("Helvetica"), c_appointments, QRect() ) );
    QIconSet status_icon(
        *stdicons->makeTextPixmap( new QPixmap( stdicons->getPixmap( QmvIcons::BlankPageIcon ) ),
                                   tr("S"), QFont("Helvetica"), c_appointments, QRect() ) );

    setViewList(FALSE);
    
        // ------------------------
        // View Patients
        // ------------------------

    QAction * ac;
    ac = new QAction( "Display Patients",
                      patients_icon,
                      "Display &Patients",
                      CTRL+ALT+Key_P,
                      actiongp_view, "action_patients", FALSE );
    ac->setStatusTip("Display Patients");
    ac->setWhatsThis("Display the Patient Demographics Panel");
    connect( ac, SIGNAL( activated() ), this, SLOT( displayPatients() ) );
    

    
        // ------------------------
        // View Debtors
        // ------------------------

    ac = new QAction( "Display Debtors",
                      debtors_icon,
                      "Display &Debtors",
                      CTRL+ALT+Key_D,
                      actiongp_view, "action_debtors", FALSE );
    ac->setStatusTip("Display Debtors");
    ac->setWhatsThis("Display the Debtors Panel");
    connect( ac, SIGNAL( activated() ), this, SLOT( displayDebtors() ) );
    

    
        // ------------------------
        // View Invoices
        // ------------------------

    ac = new QAction( "Display Invoices",
                      invoice_icon,
                      "Display &Invoices",
                      CTRL+ALT+Key_I,
                      actiongp_view, "action_invoices", FALSE );
    ac->setStatusTip("Display Invoices");
    ac->setWhatsThis("Display the Invoices Panel.");
    connect( ac, SIGNAL( activated() ), this, SLOT( displayInvoices() ) );
    

    
        // ------------------------
        // View Unpaid Invoices
        // ------------------------

    ac = new QAction( "Display Unpaid Invoices",
                      unpaid_invoices_icon,
                      "Display &Unpaid Invoices",
                      CTRL+ALT+Key_U,
                      actiongp_view, "action_unpaid_invoices", FALSE );
    ac->setStatusTip("Display Unpaid Invoices");
    ac->setWhatsThis("Display Unpaid Invoices Panel.");
    connect( ac, SIGNAL( activated() ), this, SLOT( displayUnpaidInvoices() ) );
    

    
        // ------------------------
        // View Payments
        // ------------------------

    ac = new QAction( "Display Payments",
                      payments_icon,
                      "Display Pa&yments",
                      CTRL+ALT+Key_Y,
                      actiongp_view, "ac_payments");
    ac->setStatusTip("Display Payments");
    ac->setWhatsThis(" Display the Payments Panel");
    connect( ac, SIGNAL( activated() ), this, SLOT( displayPayments() ));


        // ------------------------
        // View Banking
        // ------------------------

    ac = new QAction( "Display Banking",
                      banking_icon,
                      "Display &Banking",
                      CTRL+ALT+Key_B,
                      actiongp_view, "ac_banking" );
    ac->setStatusTip("Display Banking");
    ac->setWhatsThis("Display Banking Panel.");
    connect( ac, SIGNAL( activated() ), this, SLOT( displayBanking() ) );
    

//         // ------------------------
//         // View Waiting List Patients
//         // ------------------------

//     ac = new QAction( "Display Waiting Patients",
//                       waiting_icon,
//                       "Display &Waiting Patients",
//                       CTRL+ALT+Key_W,
//                       actiongp_view, "action_waiting", FALSE );
//     ac->setStatusTip("Display Waiting Patients");
//     ac->setWhatsThis("Display any patients on today's Waiting List.");
//     connect( ac, SIGNAL( activated() ), this, SLOT( todaysWaitingPatients() ) );
    
        // ------------------------
        // View Referrers
        // ------------------------

    ac = new QAction( "Display Referrers",
                      referrer_icon,
                      "Display &Referrers",
                      CTRL+ALT+Key_R,
                      actiongp_view, "action_referrer", FALSE );
    ac->setStatusTip("Display Referrers");
    ac->setWhatsThis("Display the Referrers Panel");
    connect( ac, SIGNAL( activated() ), this, SLOT( displayReferrers() ) );
    

        // ------------------------
        // View Feebook
        // ------------------------

    ac = new QAction( "Display Feebook",
                      feebook_icon,
                      "Display &Feebook",
                      CTRL+ALT+Key_F,
                      actiongp_view, "action_feebook", FALSE );
    ac->setStatusTip("Display Feebook");
    ac->setWhatsThis("Display the feebook Panel");
    connect( ac, SIGNAL( activated() ), this, SLOT( displayFeebook() ) );
    

        // ------------------------
        // View Patient Flags
        // ------------------------

    ac = new QAction( "Display Patient Flags",
                      patientflag_icon,
                      "Display Patient Fla&gs",
                      CTRL+ALT+Key_G,
                      actiongp_view, "action_patientflag", FALSE );
    ac->setStatusTip("Display Patient Flags");
    ac->setWhatsThis("Display the Patient Flags Panel.");
    connect( ac, SIGNAL( activated() ), this, SLOT( displayPatientFlags() ) );

    
        // ------------------------
        // View Employers
        // ------------------------

    ac = new QAction( "Display Employers",
                      employer_icon,
                      "Display &Employers",
                      CTRL+ALT+Key_E,
                      actiongp_view, "action_employer", FALSE );
    ac->setStatusTip("Display Employers");
    ac->setWhatsThis("Display the Employers Panel.");
    connect( ac, SIGNAL( activated() ), this, SLOT( displayEmployers() ) );

    
        // ------------------------------------------------------------
        // ---- Actions - Print options
        // ------------------------------------------------------------

        // ------------------------
        // Print Invoice
        // ------------------------

    ac = new QAction( "Print Invoice",
                      p_invoice_icon,
                      "Print &Invoice",
                      SHIFT+ALT+Key_I,
                      actiongp_print, "action_invoice", FALSE );
    ac->setStatusTip("Print Invoice");
    ac->setWhatsThis("Print Invoice for the currently selected service.");
    connect( ac, SIGNAL( activated() ), this, SLOT( printInvoice() ) );
    

    
        // ------------------------
        // Print Statement
        // ------------------------

    ac = new QAction( "Print Statement",
                      p_statement_icon,
                      "Print &Statement",
                      SHIFT+ALT+Key_S,
                      actiongp_print, "action_statement", FALSE );
    ac->setStatusTip("Print Statement");
    ac->setWhatsThis("Print Statement for the currently selected patient.");
    connect( ac, SIGNAL( activated() ), this, SLOT( printStatement() ) );
    

    
        // ------------------------
        // Print Label
        // ------------------------

    ac = new QAction( "Print Label",
                      p_label_icon,
                      "Print &Label",
                      SHIFT+ALT+Key_L,
                      actiongp_print, "action_label", FALSE );
    ac->setStatusTip("Print Label");
    ac->setWhatsThis("Print Label for the currently selected patient.");
    connect( ac, SIGNAL( activated() ), this, SLOT( printLabel() ) );
    
        // ------------------------
        // Print Bankdeposit
        // ------------------------

    ac = new QAction( "Print Bank Deposit",
                      p_bankdeposit_icon,
                      "Print &Bank Deposit",
                      SHIFT+ALT+Key_B,
                      actiongp_print, "action_bankdeposit", FALSE );
    ac->setStatusTip("Print Bank Deposit");
    ac->setWhatsThis("Print the currently selected Bank Deposit Report.");
    connect( ac, SIGNAL( activated() ), this, SLOT( printBankDeposit() ) );
    
        // ------------------------
        // Post Contract
        // ------------------------

    ac = new QAction( "Post &Contract Installment",
                      p_contract_icon,
                      "Post &Contract Installment",
                      SHIFT+ALT+Key_C,
                      actiongp_print, "action_contract", FALSE );
    ac->setStatusTip("Post Contract Installment");
    ac->setWhatsThis("Post contract installments for the currently selected patient.");
    connect( ac, SIGNAL( activated() ), this, SLOT( postContract() ) );
    
        // ------------------------
        // HIC Claim
        // ------------------------

    ac = new QAction( "HICOnline Claim",
                      p_claim_pci_icon,
                      "Claim Online",
                      SHIFT+ALT+Key_X,
                      actiongp_print, "action_hicpci", FALSE );
    ac->setStatusTip("Claim Online");
    ac->setWhatsThis("Claim Oline for the currently selected service.");
    connect( ac, SIGNAL( activated() ), this, SLOT( claimPCI() ) );
    

    

    

        // ------------------------------------------------------------
        // ---- Actions - Appointments options
        // ------------------------------------------------------------

   
        // ------------------------
        // View Appointments
        // ------------------------

    ac = new QAction( "Display Appointments",
                      QIconSet(XpracManIcons::agenda_xpm),
                      "&Appointments",
                      CTRL+ALT+Key_A,
                      actiongp_appointments, "action_appointments", FALSE );
    ac->setStatusTip("Display Appointments");
    ac->setWhatsThis("Display the appointments Panel");
    connect( ac, SIGNAL( activated() ), this, SLOT( displayAppointments() ) );
    
        // ------------------------
        // View Providers
        // ------------------------

    ac = new QAction( "Display Providers",
                      providers_icon,
                      "&Providers",
                      CTRL+ALT+Key_P,
                      actiongp_appointments, "action_providers", FALSE );
    ac->setStatusTip("Display Providers");
    ac->setWhatsThis("Display the providers Panel");
    connect( ac, SIGNAL( activated() ), this, SLOT( displayProviders() ) );
    
        // ------------------------
        // View Locations
        // ------------------------

    ac = new QAction( "Display Locations",
                      locations_icon,
                      "&Locations",
                      CTRL+ALT+Key_P,
                      actiongp_appointments, "action_locations", FALSE );
    ac->setStatusTip("Display Locations");
    ac->setWhatsThis("Display the locations Panel");
    connect( ac, SIGNAL( activated() ), this, SLOT( displayLocations() ) );
    
        // ------------------------
        // View Types
        // ------------------------

    ac = new QAction( "Display Appointment Types",
                      types_icon,
                      "Appointment &Types",
                      CTRL+ALT+Key_T,
                      actiongp_appointments, "action_types", FALSE );
    ac->setStatusTip("Display Appointment Types");
    ac->setWhatsThis("Display the appointment types Panel");
    connect( ac, SIGNAL( activated() ), this, SLOT( displayAppointmentTypes() ) );
    
        // ------------------------
        // View Status
        // ------------------------

    ac = new QAction( "Display Appointment Status",
                      status_icon,
                      "Appointment &Status",
                      CTRL+ALT+Key_S,
                      actiongp_appointments, "action_status", FALSE );
    ac->setStatusTip("Display Appointment Status");
    ac->setWhatsThis("Display the appointment status Panel");
    connect( ac, SIGNAL( activated() ), this, SLOT( displayAppointmentStatus() ) );

    
        // ------------------------
        // Start timer for MD2 update
        // ------------------------

    timer_exportmd2 = new QTimer( this );
    connect( timer_exportmd2, SIGNAL(timeout()), this, SLOT( exportMD2() ) );
 



    
        // ------------------------
        // Install on tool+menu bars
        // - clear existing action groups first.
        // ------------------------
    
    actiongp_file->removeFrom( toolbar_file );
    actiongp_file->addTo( toolbar_file );
    actiongp_view->removeFrom( toolbar_view );
    actiongp_view->addTo( toolbar_view );
    actiongp_print->removeFrom( toolbar_print );
    actiongp_print->addTo( toolbar_print );
    actiongp_appointments->removeFrom( toolbar_appointments );
    actiongp_appointments->addTo( toolbar_appointments );
    actiongp_file->removeFrom( popup_file );
    actiongp_file->addTo( popup_file );
    actiongp_view->removeFrom( popup_view );
    actiongp_view->addTo( popup_view );
    actiongp_print->removeFrom( popup_print );
    actiongp_print->addTo( popup_print );
    actiongp_appointments->removeFrom( popup_appointments );
    actiongp_appointments->addTo( popup_appointments );

        // Disable utilities while testing geometry control
    if (dbsel->count() <= 1 )
        toolbar_file->hide();
    toolbar_spacer->hide();
    toolbar_utilities->hide();
    
    popup_print->insertSeparator();
    popup_print->insertItem( "&Reports", this, SLOT(printReport() ) );

    setAppPath( XpracManDir );
    setAppName( initname );
    setAppLabel( "XPracman" );
    setUserLabel();
    QPixmap pm( xpracman_logo_xpm );
        // limit height during testing of geometry control
    pm.resize( pm.width(), 16);
    setAppLogo( pm );
        
    loadPreferences();    

    setToolBarsMovable(FALSE);

    setIcon(QPixmap(logo_xpracman_dark_xpm));
    
#ifdef XPRACMAN_DEBUG
    QDir cwd;
    qDebug("XpracMan::XpracMan: cwd=%s", cwd.currentDirPath().latin1() );
#endif    
        // Open the patient panel at startup
    displayPatients();
    statusBar()->message( "Ready", 2000 );
    
}


XpracMan::~XpracMan()
{
    delete printer;
}


// --------------------------------------------------------------------------------
void XpracMan::loadPreferences() {
        
    setCurrentPreferences( getenv( "PRAC_LOCATION" ) );
    QmvApplication::loadPreferences();  // reload to use XpracManDir
    
        // Add MD2 to Preferences Menu
    popup_preferences->insertSeparator();
    menu_exportmd2 = popup_preferences->insertItem("MD2 - export Patient Demographics");
    popup_preferences->connectItem(menu_exportmd2, this, SLOT(toggleExportMD2()));
    
    // Examine preferences for MD2
    // - need a filename, and a time interval
    QString patients_in = userPreference("XpracMD2PATIENTS_IN");
    bool ok = false;
    int x_timeout = userPreference("XpracMD2TimerSecs").toInt(&ok);
    if (!ok)
        x_timeout = -1;
    else if ( x_timeout < 60 ) // don't swamp the system
        x_timeout = 60;
    ok = false;
    int wait_exportmd2 = userPreference("XpracMD2WaitSecs").toInt(&ok);
    if (!ok)
        wait_exportmd2 = x_timeout/2;
    if ( patients_in.length() > 0 && x_timeout > 0 )
        setExportMD2(true);
    else {
         // turn the export service off.
        setExportMD2(false);
        popup_preferences->setItemEnabled( menu_exportmd2, false );
    }
        
}
// --------------------------------------------------------------------------------
const QString XpracMan::findPatient() const
{
    QString patn__sequence = "";
    
        // look for the dbwidget by the title of the current database
    QmvDBWidget * dbw = dict_dbtitle.find( database->dbTitle() );
    if ( dbw )
    {
        QmvTuple * curtup = dbw->getSelectedTuple( "patn" );
        if ( curtup )
            patn__sequence = curtup->attributeValue("patn__sequence");
        else
        {
            curtup = dbw->getSelectedTuple( "invc" );
            if ( curtup )
                patn__sequence = curtup->attributeValue("invc_patn__sequence");
            else
            {
                curtup = dbw->getSelectedTuple( "svpf" );
                if ( curtup )
                    patn__sequence = curtup->attributeValue("svpf_patn__sequence");
            }
        }
    }
    
    return patn__sequence;
}

  
// --------------------------------------------------------------------------------
const QString XpracMan::findDebtor() const
{
    QString dbtr_code = "";
    
        // look for the dbwidget by the title of the current database
    QmvDBWidget * dbw = dict_dbtitle.find( database->dbTitle() );
    if ( dbw )
    {
        QmvTuple * curtup = dbw->getSelectedTuple( "dbtr" );
        if ( curtup )
            dbtr_code = curtup->attributeValue("dbtr_code");
        else
        {
            curtup = dbw->getSelectedTuple( "invc" );
            if ( curtup )
                dbtr_code = curtup->attributeValue("invc_dbtr_code");
            else
            {
                curtup = dbw->getSelectedTuple( "patn" );
                if ( curtup )
                    dbtr_code = curtup->attributeValue("patn_dbtr_code");
            }
        }
    }
    
    return dbtr_code;
}

  
// --------------------------------------------------------------------------------
const QString XpracMan::findInvoice() const
{
    QString invc__sequence = "";
    
        // look for the dbwidget by the title of the current database
    QmvDBWidget * dbw = dict_dbtitle.find( database->dbTitle() );
    if ( dbw )
    {
        QmvTuple * curtup = dbw->getSelectedTuple( "svpf" );
        if ( curtup )
            invc__sequence = curtup->attributeValue("svpf_invc__sequence");
        else
        {
            curtup = dbw->getSelectedTuple( "invc" );
            if ( curtup )
                invc__sequence = curtup->attributeValue("invc__sequence");
            else
            {
                curtup = dbw->getSelectedTuple( "crep" );
                if ( curtup )
                    invc__sequence = curtup->attributeValue("crep_invc__sequence");
                else
                {
                    curtup = dbw->getSelectedTuple( "agdi" );
                    if ( curtup )
                        invc__sequence = curtup->attributeValue("agdi__sequence");
                }
                
                    
            }
        }
    }
    
    return invc__sequence;
}

// --------------------------------------------------------------------------------
void XpracMan::postContract()
{
    QString patn__sequence = findPatient();
    
    if ( !patn__sequence.length() )
    {
        QMessageBox::warning( 0, "postContract:warning",
                              tr("Select a row to indicate the patient."),
                              "OK", 0 );
        return;  
    }
    postContract( patn__sequence );
}

// --------------------------------------------------------------------------------
bool XpracMan::postContract( const QString & patn_number )
{
        // first check contracts
    int status = sql_exec( QString( "select cnrt_post_debit(%1) as count_posted" )
                            .arg( patn_number ) );
    if ( status == 1 )
    {
        QString posted = sql_value( 0, "count_posted" );
        status = posted.toInt();
    } else
        status = -1;
    
    if ( status > 0 )
    {
            // posted
        QMessageBox::warning( 0, "postContract:warning",
                              tr("Contract installments have been posted"),
                              "OK", 0 );
        return true;
    }
    else  if ( status == 0 )
    {
                // nothing to post
        QMessageBox::warning( 0, "postContract:warning",
                              tr("No contract installments are due to be posted"),
                              "OK", 0 );
        return true;
    }
    else
    {
            // error
        QMessageBox::warning( 0, "postContract:warning",
                              tr("An error occurred"),
                              "OK", 0 );
        return false;
    }
    
    
}


// --------------------------------------------------------------------------------
void XpracMan::claimPCI()
{
    QString invc__sequence = findInvoice();
    
    if ( !invc__sequence.length() )
    {
        QMessageBox::warning( 0, "Online Claim:warning",
                              tr("Select a row to indicate the invoice."),
                              "OK", 0 );
        return;  
    }

        // create a claim (PCI) from the invoice
    int status = sql_exec( QString( "select fn_create_hpcl_batch(%1)" )
                           .arg( invc__sequence ) );
        
    if ( status < 0 )
    {
        QString err_msg = sql_error();
        if (!err_msg || err_msg.length() < 1) {
            err_msg = "The Claim could not be created from this invoice";
        }
        
        QMessageBox::warning( 0, "Online Claim:warning",
                              err_msg,
                              "OK", 0 );
        return;
    }
    QMessageBox::information( 0, "Online Claim:",
                              QString("A claim has been prepared from Invoice %1::<br>"
                              "Refer to the claiming system for more information").arg(invc__sequence),
                              "OK", 0 );
    
}

// --------------------------------------------------------------------------------
void XpracMan::printInvoice()
{
    QString invc__sequence = findInvoice();
    
    if ( !invc__sequence.length() )
    {
        QMessageBox::warning( 0, "printInvoice:warning",
                              tr("Select a row to indicate the invoice."),
                              "OK", 0 );
        return;  
    }

    bool status = false;
    if ( userPreference("InvoiceUseShellVersion").lower() == "yes" )
    {
            // Shell Version
        QString pi_command = QString("%1/print_invoice %2")
            .arg(::XpracManDir)
            .arg( invc__sequence );
        int shell_status = system( pi_command );
        if ( !shell_status )
            status = true;
    } else {
            // QmvForm Version
        status = printInvoice( invc__sequence );
    }
    
    
    if ( !status )
    {
        QMessageBox::warning( 0, "printInvoice:warning",
                              tr("An invoice was not printed."),
                              "OK", 0 );
        return;  
    }
}

// --------------------------------------------------------------------------------
bool XpracMan::printInvoice( const QString & invc_number )
{
    QDict<int> form_codes;   // a list of forms required
    form_codes.setAutoDelete(true);
    QString query_string;
    
        // Maintain a local copy of the svpv object
    static QmvSqlClass * reln_svpv;
    if ( !reln_svpv )
    {
        QmvSqlClass * svpv_existing = (QmvSqlClass *) database->dbRelation( "svpv" );
        if ( svpv_existing )
            reln_svpv = new QmvSqlClass( *svpv_existing );
        else
            return(FALSE);
    }
    
        // Maintain a local copy of the form object
    static QmvSqlClass * reln_form;
    if ( !reln_form )
    {
        QmvSqlClass * form_existing = ( QmvSqlClass *) database->dbRelation( "form" );
        if ( form_existing )
            reln_form = new QmvSqlClass( *form_existing );
        else
            return(FALSE);
    }
    
        // Load the svpv data
    if ( invc_number.toInt() > 0 )
    {
            // first check contracts
        sql_exec( QString( "select cnrt_post_debit(invc_patn__sequence) from invc"
                           " where invc__sequence = %1"
                           " and invc_date_printed is null" )
                  .arg( invc_number ) );
        query_string = QString("select * from svpv where svpv_invc__sequence = %1")
            .arg(invc_number);
        reln_svpv->open( QString( "%1 %2" )
                         .arg( query_string )
                         .arg( reln_svpv->orderBy()) );
        if (reln_svpv->count() > 0)
            form_codes.insert(reln_svpv->attributeValue(0, "svpv_form_code"), new int(1) );
        
    } else {
            // first check contracts
        sql_exec( QString( "select cnrt_post_debit(invc_patn__sequence) from invc"
                           " where invc_date_printed is null" ) );

            // Get a list of form codes
        query_string = QString("select * from svpv where svpv_invc_date_printed is null"
                               " and svpv_patn_patf_code != 'HOLD'"
                               " and svpv_invc_balance > 0");
        
        reln_svpv->open( QString( "%1 %2" )
                         .arg(query_string)
                         .arg( reln_svpv->orderBy()) );
            // count the form code groups
        for (int i = 0; i < reln_svpv->count(); i++) {
            QString x_form = reln_svpv->attributeValue(i, "svpv_form_code");
            if (form_codes[x_form]) {
                (*form_codes[x_form])++;
            } else {
                form_codes.insert(x_form, new int(1));
            }
        }
        
    }
    

        // No svpv rows;
    if ( reln_svpv->count() < 1 )
    {
        QMessageBox::warning( 0, "warning",
                              tr("<b>Print Invoice(s):</b><br>"
                                 "No invoices found" ),
                              "OK", 0 );
        
        return(FALSE);
    }
    
    static QGuardedPtr<QmvForm> form_viewer;
    if ( !form_viewer )
        form_viewer = new QmvForm( reln_svpv, this, "invoice" );

    int form_total = form_codes.count();
    int form_counter = 0;
    QDictIterator<int> it(form_codes);
    while (it.current()) {
        form_counter++;
        QString form_code = it.currentKey();
        QString form_desc;
        
        reln_svpv->open( QString( "%1 and svpv_form_code = '%2' %3" )
                         .arg(query_string)
                         .arg(form_code)
                         .arg(reln_svpv->orderBy()));
        reln_form->open(QString("select * from form where form_code = '%1'")
                        .arg(form_code));
        if (reln_form->count() == 1) {
            form_desc = QString(reln_form->attributeValue(0, "form_desc"));
        } else {
            form_desc = form_code;
        }

            // Setup a form viewer, and prepare the invoice.
        if ( !form_viewer->initForm( form_code ) )
            return(FALSE);
        if (invc_number.toInt() > 0) {
            form_viewer->setCaption( QString("Invoice-%1").arg(invc_number));
        } else {
            form_viewer->setCaption( QString("%1 (Group %2 of %3)")
                                     .arg(form_desc)
                                     .arg(form_counter)
                                     .arg(form_total));
        }
            // Print Attributes
        form_viewer->setPrint( userPreference("PrintProgramInvoice"),
                               userPreference("PrinterSelectionOptionInvoice"),
                               userPreference("PrinterNameInvoice") );
            // Set FAX Attributes - create a faxlist
        QStringList faxlist;
        QString fax_sendto;
        fax_sendto = reln_svpv->attributeValue(0, "svpv_patn_fax_number");
        if ( !fax_sendto.isEmpty() )faxlist << QString("%1 (Patient)").arg(fax_sendto);

        fax_sendto = reln_svpv->attributeValue(0, "svpv_dbtr_fax_number");
        if ( !fax_sendto.isEmpty() )faxlist << QString("%1 (Debtor)").arg(fax_sendto);

        fax_sendto = reln_svpv->attributeValue(0, "svpv_rfdr_fax_number");
        if ( !fax_sendto.isEmpty() )faxlist << QString("%1 (Referror)").arg(fax_sendto);

        fax_sendto = reln_svpv->attributeValue(0, "svpv_prov_fax_number");
        if ( !fax_sendto.isEmpty() )faxlist << QString("%1 (Provider)").arg(fax_sendto);

        fax_sendto = reln_svpv->attributeValue(0, "svpv_hlfd_fax_number");
        if ( !fax_sendto.isEmpty() )faxlist << QString("%1 (Healthfund)").arg(fax_sendto);

        fax_sendto = reln_svpv->attributeValue(0, "svpv_empl_fax_number");
        if ( !fax_sendto.isEmpty() )faxlist << QString("%1 (Employer)").arg(fax_sendto);

        form_viewer->setFax( userPreference("FaxProgramName"),
                             userPreference("FaxProgramFormat"),
                             QString("%1/%2").arg(sessionFolder()).arg(userPreference("FaxFileName")),
                             faxlist );
    
            // Set EMAIL Attributes - create a emaillist
        QStringList emaillist;
        QString email_sendto;
        email_sendto = reln_svpv->attributeValue(0, "svpv_patn_email_address");
        if ( !email_sendto.isEmpty() )emaillist << QString("%1 (Patient)").arg(email_sendto);

        email_sendto = reln_svpv->attributeValue(0, "svpv_dbtr_email_address");
        if ( !email_sendto.isEmpty() )emaillist << QString("%1 (Debtor)").arg(email_sendto);

        email_sendto = reln_svpv->attributeValue(0, "svpv_rfdr_email_address");
        if ( !email_sendto.isEmpty() )emaillist << QString("%1 (Referror)").arg(email_sendto);

        email_sendto = reln_svpv->attributeValue(0, "svpv_prov_email_address");
        if ( !email_sendto.isEmpty() )emaillist << QString("%1 (Provider)").arg(email_sendto);

        email_sendto = reln_svpv->attributeValue(0, "svpv_hlfd_email_address");
        if ( !email_sendto.isEmpty() )emaillist << QString("%1 (Healthfund)").arg(email_sendto);

        email_sendto = reln_svpv->attributeValue(0, "svpv_empl_email_address");
        if ( !email_sendto.isEmpty() )emaillist << QString("%1 (Employer)").arg(email_sendto);

        form_viewer->setEmail( userPreference("EmailProgramName"),
                             userPreference("EmailProgramFormat"),
                             QString("%1/%2").arg(sessionFolder()).arg(userPreference("EmailFileName")),
                             emaillist );
    
            // Start the formviewer
        if ( userPreference("ViewFormFirst").lower() == "yes" )
            form_viewer->viewForm();
        else
            form_viewer->printForm();

        bool status = form_viewer->printedStatus();
        if ( status )
        {
            QDict<char> invc_str;
            char * ok = "OK";
            for ( int i = 0; i < reln_svpv->count(); i++ )
                    // Update printed/reprinted dates
                    //  - checking that update has not already occured 
            {
                    // the update has already happened.
                if ( invc_str.find( reln_svpv->attributeValue( i, "svpv_invc__sequence" ) ) )
                    continue;

                invc_str.insert( reln_svpv->attributeValue( i, "svpv_invc__sequence" ), ok );

                    // execute the SQL
                sql_exec( QString( "select invc_setprintdates(%1)" )
                          .arg( reln_svpv->attributeValue( i, "svpv_invc__sequence" ) ) );
            }
        }
        ++it;
    }
    
    
    return(true);

}

// Print patient statement
// --------------------------------------------------------------------------------
void XpracMan::printStatement()
{
        // there are two forms of statment - Debtor and Patient
        // The Debtor statement refers to the outstanding debtor invoices
        // The Patient statement refers to the outstanding patient invoices.
        // Selection is configured by a userpreference - AlwaysUseDebtorStatement

        // Debtor statement
    if ( userPreference("AlwaysUseDebtorStatement").lower() == "yes" )
    {
        QString x_dbtr_code = findDebtor();
        if (!x_dbtr_code.length() )

        {
            QMessageBox::warning( 0, "printStatement:warning",
                                  tr("Select a row to indicate the debtor."),
                                  "OK", 0 );
            return;  
        }
        QString query = QString("select * from dbst where dbst_dbtr_code =  '%1'" )
            .arg( x_dbtr_code );
        printStatement(query, 0);
        return;
    }

        // Patient Statement
    QString patn__sequence = findPatient();
    
    if ( !patn__sequence.length() )
    {
        QMessageBox::warning( 0, "printStatement:warning",
                              tr("Select a row to indicate the patient."),
                              "OK", 0 );
        return;  
    }
    QString query = QString( "select * from ptst where ptst_patn__sequence = '%1'" )
        .arg( patn__sequence );

        // order by
    query.append( QString( " order by ptst_date_printed" ) );

        // Maintain a local copy of the ptst object
    static QmvSqlClass * reln_ptst;
    if ( !reln_ptst )
    {
        QmvSqlClass * ptst_existing = (QmvSqlClass *) database->dbRelation( "ptst" );
        if ( ptst_existing )
            reln_ptst = new QmvSqlClass( *ptst_existing );
        else
            return;
    }
    
        // Maintain a local copy of the form object
    static QmvSqlClass * reln_form;
    if ( !reln_form )
    {
        QmvSqlClass * form_existing = ( QmvSqlClass *) database->dbRelation( "form" );
        if ( form_existing )
            reln_form = new QmvSqlClass( *form_existing );
        else
            return;
    }
    
    reln_ptst->open( QString( query ));

        // No ptst rows;
    if ( reln_ptst->count() < 1 )
    {
        QMessageBox::warning( 0, "warning",
                              tr("<b>%1:</b><br>"
                                 "No data found" )
                              .arg( reln_ptst->relationTitle() ),
                              "OK", 0 );
        return;
    }
    
        // load the form specifications
    QString form_code = "ptstatement";
    reln_form->open( QString( "select * from form where form_code = '%1'::text" )
                     .arg( form_code ));
    
        // no form spec.
    if ( reln_form->count() != 1 )
        return;

        // Setup a form viewer, and prepare the statements
    static QGuardedPtr<QmvForm> form_viewer;
    if ( !form_viewer )
        form_viewer = new QmvForm( reln_ptst, this, "form_ptst" );

    if ( !form_viewer->initForm( form_code ) )
    {
        QMessageBox::information(0, "printStatement:information",
                                 tr("<b>%1:</b><br><br>This form <b>%2</b> cannot be initialised.")
                                 .arg( reln_ptst->relationTitle() )
                                 .arg( form_code ),
                                 "OK", 0 );
        return;
    }
    
    form_viewer->setPrint( userPreference("PrintProgramStatement"),
                           userPreference("PrinterSelectionOptionStatement"),
                           userPreference("PrinterNameStatement") );
    
    if ( userPreference("ViewFormFirst").lower() == "yes" )
        form_viewer->viewForm();
    else
        form_viewer->printForm();
   
    return;

}

// --------------------------------------------------------------------------------
//! 
/*!
  <long-description>

  \param delay  
  \param zerodays       
  \param debtor_regex   
*/
// --------------------------------------------------------------------------------
bool XpracMan::printStatement( const int days_ago, const int zerodays, const char *  debtor_regex )
{
    QString query = "select * from dbst where dbst_total_balance != 0.00::numeric";
    
        // Select previous statements older than delay
        //              and new statements
        // where balance is not current.
        // Ignore invoices for patients on HOLD
    
    query.append( QString( " and (dbst_last_statement is null "
                           "      or  dbst_last_statement < dbst_date - coalesce(dbst_delay_statement,'30 days'::interval) )"
                           " and ( (dbst_total_balance > dbst_balance_0 + dbst_balance_30) or "
                           "       (dbst_last_invoice < dbst_date - coalesce(dbst_first_statement,'40 days'::interval)))" ) );

    
    if ( zerodays >= 0 )
    {
        if ( zerodays == 0 )
                // ignore zero-balance invoices
            query.append( " and dbst_balance != 0" );
        else
                // ignore zero-balance invoices before current statement period
            query.append( QString( " and (dbst_balance != 0"
                                   "      and coalesce(dbst_date_reprint,dbst_date_printed) "
                                   "      < dbst_date - %1)")
                          .arg( zerodays) );
    }

    if ( debtor_regex )
        query.append( QString( " and dbst_name ~* '%1'" ).arg( *debtor_regex ) );
                      
    return printStatement( QString( query.append( " order by dbst_patient_key, dbst_date_printed;" ) ), days_ago);
   
}
// --------------------------------------------------------------------------------
bool XpracMan::printStatement( const QString & query, int days_ago )
{
        // Maintain a local copy of the dbst object
    static QmvSqlClass * reln_dbst;
    if ( !reln_dbst )
    {
        QmvSqlClass * dbst_existing = (QmvSqlClass *) database->dbRelation( "dbst" );
        if ( dbst_existing )
            reln_dbst = new QmvSqlClass( *dbst_existing );
        else
            return(FALSE);
    }
    
        // Maintain a local copy of the form object
    static QmvSqlClass * reln_form;
    if ( !reln_form )
    {
        QmvSqlClass * form_existing = ( QmvSqlClass *) database->dbRelation( "form" );
        if ( form_existing )
            reln_form = new QmvSqlClass( *form_existing );
        else
            return(FALSE);
    }
    

        // disable the MD timer (hack) to prevent transaction conflict
    bool hold_timer_exportmd2 = timer_exportmd2;
    setExportMD2( false );
        // Begin the transaction which protects the current setting for dbst_statement_age
    sql_exec("begin work");
        // load the conf row for dbst_statement_age
    sql_exec ( QString( "delete from conf where conf_code = 'dbst_statement_age';"
                        " insert into conf(conf_code, conf_desc, conf_value)"
                        " values('dbst_statement_age', 'Statement date is %1 days ago', '%2');" )
               .arg(days_ago).arg(days_ago) );

    QProgressDialog controller( "Loading records...", "Cancel", 100,
                                this, "progress", TRUE );
    int status = reln_dbst->open( QString( query ), &controller );
    controller.cancel();
    sql_exec("abort work");
    setExportMD2( hold_timer_exportmd2 ); // reinstate
    
        // Load the dbst data
    switch ( status )
    { 
        case -1:
            QMessageBox::warning( 0, "warning",
                                  tr("<b>%1:</b><br>"
                                     "The data query failed. The following message"
                                     " may help to explain the failure.<br><br>"
                                     "SQL: %2")
                                  .arg( reln_dbst->relationTitle() )
                                  .arg( reln_dbst->lastError() ),
                                  "OK", 0 );
            return(FALSE);
            break;
        case -2:
            QMessageBox::warning( 0, "warning",
                                  tr("<b>%1:</b><br>"
                                     "The data query has been cancelled as requested.")
                                  .arg( reln_dbst->relationTitle() ),
                                  "OK", 0 );
            return(FALSE);
            break;
        default:
            break;
    }
    
        // No dbst rows;
    if ( reln_dbst->count() < 1 )
    {
        QMessageBox::warning( 0, "warning",
                              tr("<b>%1:</b><br>"
                                 "No data found" )
                              .arg( reln_dbst->relationTitle() ),
                              "OK", 0 );
        return(FALSE);
    }
    
        // load the form specifications
    controller.reset();
    QString form_code = "dbstatement";
    reln_form->open( QString( "select * from form where form_code = '%1'::text" )
                     .arg( form_code ),
                     &controller  );
    controller.cancel();
    
        // no form spec.
    if ( reln_form->count() != 1 )
        return(FALSE);

        // Setup a form viewer, and prepare the statements
    controller.reset();
    controller.setLabelText(" Preparing statement(s) for viewing ..." );
    static QGuardedPtr<QmvForm> form_viewer;
    if ( !form_viewer )
        form_viewer = new QmvForm( reln_dbst, this, "form_dbst" );

    controller.setProgress( 10 );
    if ( !form_viewer->initForm( form_code ) )
    {
        QMessageBox::information(0, "printStatement:information",
                                 tr("<b>%1:</b><br><br>This form <b>%2</b> cannot be initialised.")
                                 .arg( reln_dbst->relationTitle() )
                                 .arg( form_code ),
                                 "OK", 0 );
        controller.cancel();
        return(FALSE);
    }
    
    form_viewer->setPrint( userPreference("PrintProgramStatement"),
                           userPreference("PrinterSelectionOptionStatement"),
                           userPreference("PrinterNameStatement") );
    if ( userPreference("ViewFormFirst").lower() == "yes" )
        form_viewer->viewForm();
    else
        form_viewer->printForm();

        // update statement date
    bool print_status = form_viewer->printedStatus();
    if ( print_status )
    {    
        QString curr_dbtr_code;
        for ( int i = 0; i < reln_dbst->count(); i++ )
        {
            QString this_dbtr_code;
            this_dbtr_code = reln_dbst->attributeValue( i, "dbst_dbtr_code" );
            if ( this_dbtr_code == curr_dbtr_code )
                continue;
            curr_dbtr_code = this_dbtr_code;
            sql_exec( QString("update dbtr set dbtr_last_statement = now() - '%1 days'::interval"
                              " where dbtr_code = '%2'::text" )
                      .arg(days_ago)
                      .arg(curr_dbtr_code) );
        }
    }
    
    
    return(print_status);

}


// --------------------------------------------------------------------------------
void XpracMan::printLabel()
{
    
        // Maintain a local copy of the form object
    static QmvSqlClass * reln_form;
    if ( !reln_form )
    {
        QmvSqlClass * form_existing = (QmvSqlClass *) database->dbRelation( "form" );
        if ( form_existing )
            reln_form = new QmvSqlClass( *form_existing );
        else
        {
            QMessageBox::warning( 0, "printLabel:warning",
                                  tr("Form specification object cannot be located."),
                                  "OK", 0 );
            return;
        }
    }

        // look for the dbwidget by the title of the current database
    QmvDBWidget * dbw = dict_dbtitle.find( database->dbTitle() );
    if ( !dbw )
        return;

    QmvTuple * curtup = dbw->getSelectedTuple( "patn" );
    if ( !curtup )
    {
        QMessageBox::warning( 0, "printLabel:warning",
                              tr("Select a patient."),
                              "OK", 0 );
        return;  
    }

    QString patn__sequence = curtup->attributeValue("patn__sequence");
    
    if ( !patn__sequence.length() )
    {
        QMessageBox::warning( 0, "printLabel:warning",
                              tr("An Patient Number could not be locate in the selected row."),
                              "OK", 0 );
        return;  
    }

        // Look for a view based on feetype
    QString label_class = "patv";
    QString patn_feet_code = curtup->attributeValue("patn_feet_code");
    if ( !patn_feet_code.isNull() )
    {
        label_class = QString("patv_%1").arg(patn_feet_code);
        if ( !database->dbRelation( label_class ) )
            label_class = "patv"; //revert
    }

        // Use the internal form table for labels?
    if ( userPreference( "UseFormsForLabels" ).lower() == "yes" )
    {
        printLabel( label_class, patn__sequence );
        return;
    }
    
        // ------------------------------------------------------------
        // Try filesystem method - TODO: obsolete this method.
        // ------------------------------------------------------------

    
    QString external_labels = userPreference( "ExternalLabels" );
    
    QString layout_name;
    if ( external_labels.length() )
    {
        QStringList lablist = QStringList::split( QRegExp( "\\s*;\\s*" ), external_labels );
        
        QPopupMenu popup_label( this, "label selector" );
        popup_label.setCheckable(TRUE);
        popup_label.clear();
        for ( int i = 0; i < lablist.count(); i++ )
        {
            QPixmap pm( QSize(8,8) );
            pm.fill( Qt::blue );
            popup_label.insertItem( pm, lablist[i], i, -1 );
            popup_label.setItemChecked( i, FALSE );
        }
        popup_label.setItemChecked( 0, TRUE );
        int row = popup_label.exec( QCursor::pos() );
        if ( row >= 0 && row < lablist.count() )
            layout_name = lablist[row];
        else
            return;
    } else {
        layout_name = "Standard";
        if ( !patn_feet_code.isNull() )
            layout_name = QString( "Standard_%1" ).arg(patn_feet_code);
    }

        // Prepare a system command
    QString pi_command = QString("env PRINTER=%1 %2/print_form %3 '%4'")
        .arg( userPreference("PrinterNameForm") )
        .arg( ::XpracManDir )
        .arg( patn__sequence )
        .arg( layout_name );
    int status = system( pi_command );
    if ( status )
    {
        
        QMessageBox::warning( 0, "printLabel:warning",
                              tr("A label could not be printed for this patient."),
                              "OK", 0 );
        return;  
    }
}

// --------------------------------------------------------------------------------
//! Print labels for the current patient
/*!
  Print label(s) for the currently sleected patient record.

  \param        patn__sequence
  \return       true/false for success
*/
bool XpracMan::printLabel( const QString & label_class, const QString & patv__sequence )
{

        // TODO: fix this method - it is not completed!!
    
        // Maintain a local copy of the label  object
    static QmvSqlClass * reln_label;
    if ( !reln_label )
    {
        QmvSqlClass * patv_existing = (QmvSqlClass *) database->dbRelation( label_class );
        if ( patv_existing )
            reln_label = new QmvSqlClass( *patv_existing );
        else
            return(FALSE);
    }
    
        // Maintain a local copy of the form object
    static QmvSqlClass * reln_form;
    if ( !reln_form )
    {
        QmvSqlClass * form_existing = ( QmvSqlClass *) database->dbRelation( "form" );
        if ( form_existing )
            reln_form = new QmvSqlClass( *form_existing );
        else
            return(FALSE);
    }
        // If there is more than one member of the label class
    
    reln_form->open( QString( "select * from form where form_classname = '%1'" )
                     .arg( label_class ) );


        // if more than 1 row, popup selector
    int row;
    int formcount = reln_form->count();
    if ( formcount == 1 )
        row = 0;
    else if ( formcount > 1 )
    {
        QPopupMenu popup_label( this, "label selector" );
        popup_label.setCheckable(TRUE);
        popup_label.clear();
        for ( int i = 0; i < formcount; i++ )
        {
            QPixmap pm( QSize(8,8) );
            pm.fill( Qt::blue );
            popup_label.insertItem( pm, reln_form->attributeValue( i, "form_desc" ), i, -1 );
            popup_label.setItemChecked( i, FALSE );
        }
        popup_label.setItemChecked( 0, TRUE );
        row = popup_label.exec( QCursor::pos() );
    }
    else
        return(FALSE); // no forms
    
    if ( row < 0 )
        return(FALSE);

        // Get the form_code
    QString label_name;
    if ( row >= 0 && row < reln_form->count() )
        label_name = reln_form->attributeValue( row, "form_code" );
    else
    {
        QMessageBox::critical( 0, QString( "Fatal Error - Form Specifications not found" ),
                               QString( tr("Cannot locate a form record  <%1>") )
                               .arg( label_name ) );
        return(FALSE);
    }
        // Load the patv data
    reln_label->open( QString( "select * from %1 where patv__sequence = %2" )
                      .arg( label_class )
                      .arg( patv__sequence ) );

        // No patv rows;
    if ( reln_label->count() < 1 )
        return(FALSE);

        // Setup a form viewer, and prepare the label.
    static QGuardedPtr<QmvForm> form_viewer;
    if ( !form_viewer )
        form_viewer = new QmvForm( reln_label, this, "form_label" );

    if ( !form_viewer->initForm( label_name ) )
        return(FALSE);
    
    form_viewer->setPrint( userPreference("PrintProgramForm"),
                           userPreference("PrinterSelectionOptionForm"),
                           userPreference("PrinterNameForm") );
    
    if ( userPreference("ViewFormFirst").lower() == "yes" )
        form_viewer->viewForm();
    else
        form_viewer->printForm();

    return (TRUE);
}

 
// --------------------------------------------------------------------------------
void XpracMan::printBankDeposit()
{
        // look for the dbwidget by the title of the current database
    QmvDBWidget * dbw = dict_dbtitle.find( database->dbTitle() );
    if ( !dbw )
        return;

    QmvTuple * curtup = dbw->getSelectedTuple( "bkdp" );
    
    QString bkdp__sequence;
    
    if ( curtup )
    {
        bkdp__sequence = curtup->attributeValue("bkdp__sequence");
        if ( !bkdp__sequence.length() )
        {
            QMessageBox::warning( 0, "printBankDeposit:warning",
                                  tr("A bank deposit batch could not be located in the selected row."),
                                  "OK", 0 );
            return;
        }
        
    } else
        bkdp__sequence = "0";

    QString form_name;
    QString bank_select;
    
    switch( QMessageBox::information( this,
                                      "printBankDeposit:Select",
				      QString("Banking/Reconciliation Report\nBatch-ID %1").arg(bkdp__sequence),
				      "Bank", "Reconciliation", "Today's Payments",
				      2, -1) )
    {
        case 0:
            if (bkdp__sequence.toInt() == 0) {
              int inserted = sql_exec("SELECT bkdp_insert_row() as new_sequence");
              if (inserted == 1) {
                 bkdp__sequence = QString(sql_value(0, "new_sequence" ));
              } else {
                 QMessageBox::warning( 0, "printBankDeposit:warning",
                              tr("A new batch could not be created - perhaps there is nothing to batch."),
                              "OK", 0 );
                 return;
              }
            }
            form_name = "bank-deposit";
            bank_select = QString("select * from bank_deposit_report(%1)")
                .arg( bkdp__sequence );
            break;
        case 1:
            form_name = "bank-reconciliation";
            bank_select = QString("select * from bank_deposit_report(%1)")
                .arg( bkdp__sequence );
            break;
        case 2:
            form_name = "bank-today";
            bank_select = QString("select * from bank_deposit_report('%1'::date)")
                .arg(QDate::currentDate().toString());
            break;
        default:
            return;
            break;
    }
        // Use the form method for reporting

            // Maintain a local copy of the bkdr object
    static QmvSqlClass * reln_bkdr;
    if ( !reln_bkdr )
    {
        QmvSqlClass * bkdr_existing = (QmvSqlClass *) database->dbRelation( "bkdr" );
        if ( bkdr_existing )
            reln_bkdr = new QmvSqlClass( *bkdr_existing );
        else
            return;
    }
 
    int i = reln_bkdr->open( bank_select );
 
    if ( i < 1 )
    {
        QMessageBox::information(0, "printBankDeposit:information",
                                 tr("There is nothing to print."),
                                 "OK", 0 );
        return;
    }
    
    
        // Setup a form viewer, and prepare the invoice.
    static QGuardedPtr<QmvForm> form_viewer;
    if ( !form_viewer )
        form_viewer = new QmvForm( reln_bkdr, this, "form_bkdr" );

    if ( !form_viewer->initForm( form_name ) )
        return;

    form_viewer->setPrint( userPreference("PrintProgram"),
                           userPreference("PrinterSelectionOption"),
                           userPreference("PrinterName") );
    
    if ( userPreference("ViewFormFirst").lower() == "yes" )
        form_viewer->viewForm( false ); // form_viewer is already initialised above
    else
        form_viewer->printForm();

    
}




// --------------------------------------------------------------------------------
void XpracMan::displayPatients()
{
    displayQuerySet( "patn", "" );
}

// --------------------------------------------------------------------------------
void XpracMan::displayDebtors()
{
    QString query;
    QmvDBWidget * dbw = dict_dbtitle.find( database->dbTitle() );
    if ( dbw )
    {
        QmvTuple * curtup = dbw->getSelectedTuple( "patn" );
        if ( curtup && curtup->attributeValue("patn_dbtr_code").length()  )
        {
            QString value = QmvClass::cleanForSQL(curtup->attributeValue("patn_dbtr_code"));
            query = QString( "select * from dbtr where dbtr_code = '%1'::text"
                             " and dbtr_code != '-'" )
                .arg( value );
        }
    }
    displayQuerySet( "dbtr", query );
}

// --------------------------------------------------------------------------------
void XpracMan::displayInvoices()
{
    displayQuerySet( "invc", "" );
}

// --------------------------------------------------------------------------------
void XpracMan::displayUnpaidInvoices()
{
    displayQuerySet( "invc", "select * from invc where invc_balance(invc__sequence) != 0"
                     " order by invc_date_printed" );
}

// --------------------------------------------------------------------------------
void XpracMan::displayPayments()
{
    displayQuerySet( "paym", "" );
}

// --------------------------------------------------------------------------------
void XpracMan::displayBanking()
{
    displayQuerySet( "bkdp", "" );
}

// --------------------------------------------------------------------------------
void XpracMan::todaysWaitingPatients()
{
    loadWaitingPatients( QDate() );
}

// --------------------------------------------------------------------------------
void XpracMan::displayReferrers()
{
    QString query;
    QmvDBWidget * dbw = dict_dbtitle.find( database->dbTitle() );
    if ( dbw )
    {
        int panel_id = dbw->getPanelID();
        QString relname = "patn"; //default
        QmvRelationWidget * curr_relwid = dbw->getRelationWidget(panel_id);
        if (curr_relwid && curr_relwid->getRelation()) {
                relname = curr_relwid->getRelation()->relationName();
        }
            
        QmvTuple *curtup = dbw->getSelectedTuple (relname);
        QString fk = QString("%1_rfdr_code").arg(relname);
        if (curtup && curtup->attributeValue (fk).length ())
        {
            QString value =
                QmvClass::cleanForSQL (curtup->attributeValue (fk));
            query =
                QString ("select * from rfdr where rfdr_code = '%1'::text"
                         " and rfdr_code != '-'").arg (value);
        }
    }
    displayQuerySet( "rfdr", query );

}

// --------------------------------------------------------------------------------
void XpracMan::displayFeebook()
{
    displayQuerySet( "serv", "" );
}

// --------------------------------------------------------------------------------
void XpracMan::displayPatientFlags()
{
    displayQuerySet( "patf", "" );
}
// --------------------------------------------------------------------------------
void XpracMan::displayEmployers()
{
    displayQuerySet( "empl", "" );
}
// --------------------------------------------------------------------------------
void XpracMan::todaysRecentPatients()
{
    loadRecentPatients( QDate() );
}


// --------------------------------------------------------------------------------
//! Display the Appointments List
/*!

*/
// --------------------------------------------------------------------------------
void XpracMan::displayAppointments()
{
        // Find the initial evnv object
    QmvSqlClass * evnv_existing = (QmvSqlClass *) database->dbRelation( "evnv" );
    if (! evnv_existing )
        return;
    
    bool linked =  ( userPreference("XpracAppTool.linked").lower() == "yes" );

        // If exists then set the display's active set
    if (!apptool)
    {
            // create a new relation object
        QmvSqlClass * reln_evnv = new QmvSqlClass( *evnv_existing );
        apptool = new XpracAppTool( reln_evnv, this, "Appointments" );
        
            // refresh apptool when evnv changes
        if ( linked )
            connect( evnv_existing, SIGNAL( tupleChanged( QmvTuple * ) ),
                     this, SLOT( slotTupleChanged( QmvTuple * ) ) );
        
    } else if ( linked )
        apptool->setBaseQuery( evnv_existing->currentQuery() );

    displayAppTool( -1 );
}


// --------------------------------------------------------------------------------
//! Display the Providers List
/*!

*/
// --------------------------------------------------------------------------------
void XpracMan::displayProviders()
{
    displayQuerySet( "prov", "" );
}


//! Display the Locations List
/*!

*/
// --------------------------------------------------------------------------------
void XpracMan::displayLocations()
{
    displayQuerySet( "locn", "" );
}


// --------------------------------------------------------------------------------
//! Display the Appointment Types List
/*!

*/
// --------------------------------------------------------------------------------
void XpracMan::displayAppointmentTypes()
{
    displayQuerySet( "aptp", "" );
}


// --------------------------------------------------------------------------------
//! Display the Appointment Status List
/*!

*/
// --------------------------------------------------------------------------------
void XpracMan::displayAppointmentStatus()
{
    displayQuerySet( "apst", "" );
}


// --------------------------------------------------------------------------------
//! Display the appointment tool
/*!
  If it does not exist, then return.
  
*/
// --------------------------------------------------------------------------------
void XpracMan::displayAppTool( int index )
{
    if ( !apptool )
        return;

    QDate current_day;
    QmvDBWidget * dbw = dict_dbtitle.find( database->dbTitle() );
    if ( !dbw )
        return;

        // Is there a current tuple?
    QmvTuple * curtup = dbw->getSelectedTuple( "evnv" );

        // Is there a relation widget
    QmvRelationWidget * rlw = app_lists.find( index );
    QmvSqlClass * reln_evnv = 0;
    if ( rlw )
    {
        reln_evnv = rlw->getRelation();
        if ( reln_evnv )
            apptool->setBaseQuery( reln_evnv->currentQuery() );
    }
    

        // look for selected tuple, and set the date if found.
    if ( reln_evnv && curtup && curtup->attributeValue("evnv_starttime").length() )
        current_day = reln_evnv->strToDateTime( curtup->attributeValue("evnv_starttime" ) ).date();
    if ( current_day.isValid() )
        apptool->setCurrentDate( current_day );
    else
        apptool->init();

    apptool->move(frameGeometry().topLeft()); // hack

    apptool->show();
    apptool->raise();

}
    

// --------------------------------------------------------------------------------
//! Slot which is called when a new panel is created.
/*!
  This virtual method is implemented here to enable extension of function
  at the time of creation of a new QmvRelationWidget.

  \param nlp    pointer to the  \ref QmvRelationWidget created.
*/
// --------------------------------------------------------------------------------
void XpracMan::newListPanel( QmvRelationWidget * nlp )
{
    if ( !nlp )
        return;
    
        // appointments
    if ( nlp->getRelation()->relationName() == "evnv" )
    {
            // initialise the basic appointment display
        displayAppointments();

            // make connections for this QmvRelationWidget
        app_signals = new QSignalMapper( this );
            // display window when button clicked
        connect( app_signals, SIGNAL( mapped(int) ),
                 this, SLOT( displayAppTool(int) ));

        if ( userPreference("XpracAppTool.linked").lower() == "yes" )
        {
                // refresh apptool when evnv row selected
            connect( nlp, SIGNAL( tupleSelected( int, QmvTuple * ) ),
                     this, SLOT( slotTupleSelected( int, QmvTuple * ) ) );
                // refresh evnv when apptool updates
            connect( apptool, SIGNAL( eventUpdated( int ) ),
                     nlp, SLOT( refreshRowBySequenceId( int ) ) );
        }        
        
        QToolButton * tb = nlp->addTupleButton("appointments");
        tb->setTextLabel( QString("Appointments (Ctrl+Shift+Alt+B)") );
        QIconSet appointments_icon( XpracManIcons::agenda_xpm );
        tb->setIconSet( appointments_icon );
        QWhatsThis::add(tb , QString( QString("A graphical appointment window") ) );
        tb->setAccel( CTRL+SHIFT+ALT+Key_B );

            // ----------------------------------------
            // Register this QmvRelationWidget and
            // connect the toolbutton
            // ----------------------------------------
            // Add QmvRelationWidget to a dictionary
        int idx = app_lists.count();
        app_lists.insert( idx, nlp );
        
            // map the button signal
        app_signals->setMapping( tb, idx );
        connect( tb, SIGNAL( clicked() ), app_signals, SLOT( map() ) );
        tb->show();
        return;
    }

        // services
    if ( nlp->getRelation()->relationName() == "svpf" )
    {
            // setup a single signal mapper for svpf objects
        if (!svp_signals)
        {
            svp_signals = new QSignalMapper( this );
            connect( svp_signals, SIGNAL( mapped(int) ),
                     this, SLOT( multiServiceReduction(int) ) );
        }

            // Add QmvRelationWidget to the generic dictionary,
            // to enable identification of the widget at signal time.
        int idx = rwe_lists.count();
        rwe_lists.insert( idx, nlp );

            // Now do the special setups
        QToolButton * tb = nlp->addTupleButton("multiservice");
        tb->setTextLabel( QString("Multi-Service Reduction (Ctrl+Shift+Alt+M)") );
        QIconSet msr_icon( XpracManIcons::spreadsheet_xpm );
        tb->setIconSet( msr_icon );
        QWhatsThis::add(tb , QString( QString("Adjust the set of services in the current invoice"
                                              " according to the preset 100/50/25 ...") ) );
        tb->setAccel( CTRL+SHIFT+ALT+Key_M );
        
            // map the button signal
        svp_signals->setMapping( tb, idx );
        connect( tb, SIGNAL( clicked() ), svp_signals, SLOT( map() ) );
        tb->show();
    }

        // forms
    if ( nlp->getRelation()->relationName() == "form" )
    {
            // setup a single signal mapper
        QSignalMapper *sigmap = new QSignalMapper( this );
        connect( sigmap, SIGNAL( mapped(int) ),
                 this, SLOT( formEditor(int) ) );

            // Add QmvRelationWidget to the generic dictionary,
            // to enable identification of the widget at signal time.
        int idx = rwe_lists.count();
        rwe_lists.insert( idx, nlp );

            // Now do the special setups
        QToolButton * tb = nlp->addTupleButton("formeditor");
        tb->setTextLabel( QString("Form Editor (Ctrl+Shift+Alt+F)") );
        QIconSet form_icon( XpracManIcons::spreadsheet_xpm );
        tb->setIconSet( form_icon );
        QWhatsThis::add(tb , QString( QString("Edit form layout") ) );
        tb->setAccel( CTRL+SHIFT+ALT+Key_F );
        
            // map the button signal
        sigmap->setMapping( tb, idx );
        connect( tb, SIGNAL( clicked() ), sigmap, SLOT( map() ) );
        tb->show();
    }
        // Patient
    if ( nlp->getRelation()->relationName() == "patn" )
    {
            // setup a single signal mapper
        QSignalMapper *sigmap = new QSignalMapper( this );
        connect( sigmap, SIGNAL( mapped(int) ),
                 this, SLOT( displayFlaggedPatients(int) ) );

            // Add QmvRelationWidget to the generic dictionary,
            // to enable identification of the widget at signal time.
        int idx = rwe_lists.count();
        rwe_lists.insert( idx, nlp );

            // Now do the special setups
        QToolButton * tb = nlp->addTupleButton("PatientFlags");
        tb->setTextLabel( QString("Display Flagged Patients (Ctrl+Shift+Alt+F)") );
        QIconSet patnflag_icon( XpracManIcons::flag_xpm );
        tb->setIconSet( patnflag_icon );
        QWhatsThis::add(tb , QString( QString("Display Flagged Patients") ) );
        tb->setAccel( CTRL+SHIFT+ALT+Key_F );
        
            // map the button signal
        sigmap->setMapping( tb, idx );
        connect( tb, SIGNAL( clicked() ), sigmap, SLOT( map() ) );
        tb->show();
    }
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracMan::formEditor( int index )
{
    const QString relnname = "form";
    static QmvFormTool * form_editor = 0;
    
        // Get the dbwidget in order to identify the selected tuple
    QmvDBWidget * dbw = dict_dbtitle.find( database->dbTitle() );
    if ( !dbw )
        return;

        // Is there a current tuple?
    QmvTuple * curtup = dbw->getSelectedTuple( relnname );
    if (!curtup)
    {    
        QMessageBox::warning( 0, "formEditor::warning",
                              tr("Select a form to edit."),
                              "OK", 0 );
        return;
    }
    
        // Is there a relation widget
    QmvRelationWidget * rlw = rwe_lists.find( index );
    if ( !rlw || rlw->getRelation()->relationName() != relnname )
        return;

        // create a form editor tool
    if ( !form_editor )
        form_editor = new QmvFormTool( rlw, this, "form editor" );

    form_editor->init(curtup->attributeValue("form_code") );
    form_editor->show();
    form_editor->raise();

        
        // Refresh the set.
//    rlw->displayList(rlw->getRelation()->queryDescription());
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracMan::displayFlaggedPatients( int index )
{
    

    const QString relnname = "patn";

        // Prepare the flag data --------------------------------
        // Maintain a local copy of the patf object
    static QmvSqlClass * reln_patf;
    if ( !reln_patf )
    {
        QmvSqlClass * patf_existing = (QmvSqlClass *) database->dbRelation( "patf" );
        if ( patf_existing )
            reln_patf = new QmvSqlClass( *patf_existing );
        else
        {
            QMessageBox::warning( 0, "displayFlaggedPatients:warning",
                                  tr("Patient flags object cannot be located."),
                                  "OK", 0 );
            return;
        }
    }
    int patf_count = reln_patf->open( QString("select * from patf %1 limit 200")
                                      .arg(reln_patf->orderBy() ) );
    if ( patf_count < 1 )
        return;
    
    QPopupMenu popup_patf( this, "flag selector" );
    popup_patf.setCheckable(TRUE);
    popup_patf.clear();
    
    popup_patf.insertItem( new QLabel("Select By Patient Flags", &popup_patf), -1, -1 );
    popup_patf.insertSeparator();
    for ( int i = 0; i < patf_count; i++ )
    {
        QPixmap pm( QSize(8,8) );
        pm.fill( Qt::blue );
        popup_patf.insertItem( pm, reln_patf->attributeValue( i, "patf_desc" ), i, -1 );
        popup_patf.setItemChecked( i, FALSE );
    }
    
    popup_patf.setItemChecked( 0, TRUE );
    int row = popup_patf.exec( QCursor::pos() );

    if ( row >= 0 && row < patf_count )
        displayQuerySet( "patn", QString("select * from patn where patn_patf_code = '%1'"
                                         " order by patn_psnam, patn_fsnam limit 200")
                         .arg( reln_patf->attributeValue( row, "patf_code" ) ) );

    return;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracMan::multiServiceReduction( int index )
{
    const QString relnname = "svpf";
    
        // Get the dbwidget in order to identify the selected tuple
    QmvDBWidget * dbw = dict_dbtitle.find( database->dbTitle() );
    if ( !dbw )
        return;

        // Is there a current tuple?
    QmvTuple * curtup = dbw->getSelectedTuple( relnname );
    if (!curtup)
    {    
        QMessageBox::warning( 0, "multiServiceReduction:warning",
                              tr("Select a service."),
                              "OK", 0 );
        return;
    }
    
        // Is there a relation widget
    QmvRelationWidget * rlw = rwe_lists.find( index );
    if ( !rlw || rlw->getRelation()->relationName() != relnname )
        return;

        // get the service details
    QString msr_invc__sequence = curtup->attributeValue("svpf_invc__sequence" );
    QDate tmp_date = QmvClass::strToDateTime(curtup->attributeValue("svpf_date_service" )).date();
    QString msr_date_service = QString("%1/%2/%3")
        .arg(tmp_date.day()).arg(tmp_date.month()).arg(tmp_date.year());
    
    if ( msr_invc__sequence.isNull() || msr_date_service.isNull() )
        return;

        // make sure
    if ( QMessageBox::information( this, "multiServiceReduction:OK?",
				      tr("Are you sure that you want to reduce fees for"
                                         " services on invoice# <b>%1</b>, dated <b>%2</b>")
                                      .arg( msr_invc__sequence )
                                      .arg( msr_date_service ),
                                   QMessageBox::Yes | QMessageBox::Default,
                                   QMessageBox::Cancel | QMessageBox::Escape) != QMessageBox::Yes )
        return;
    
    QApplication::setOverrideCursor( Qt::waitCursor );
    int status = sql_exec( QString("select svpf_ms_adjust(%1,'%2');")
                           .arg(msr_invc__sequence)
                           .arg(msr_date_service) );
    QApplication::restoreOverrideCursor();
    
    if ( status < 0 )
    {   
        QMessageBox::warning( 0, "multiServiceReduction:warning",
                              tr("The multi-service reduction appears to have failed:<br>"
                                 "SQL: %1")
                              .arg( sql_error() ),
                              "OK", 0 );
        return;
    }
        
        // Refresh the set.
    rlw->displayList(rlw->getRelation()->queryDescription());
}

// --------------------------------------------------------------------------------
//! Slot to respond to tuple selection
/*!
  A slot to accept notification of tuple selection and respond locally

  \param id     
  \param tup    
*/
// --------------------------------------------------------------------------------
void XpracMan::slotTupleSelected( int id, QmvTuple * tup )
{
    if ( !tup )
        return;

    QmvSqlClass * reln = (QmvSqlClass *) tup->parentClass();

        // Appointment changes 
    if ( reln->relationName() == "evnv" )
    {
            // Respond by displaying the current tuple starttime, or
            // today if there is not a valid starttime
            // TODO - this might display day of next vacant
            // appointment.
        
        QDate current_day;
        if ( tup->attributeValue("evnv_starttime").length() )
            current_day = reln->strToDateTime( tup->attributeValue("evnv_starttime" ) ).date();
        if ( current_day.isValid() )
            apptool->setCurrentDate( current_day );
        else
            apptool->init();
        apptool->update();
    }
    
}

// --------------------------------------------------------------------------------
//! Slot to respond to tuple updates
/*!
  Respond to a tuple update locally.

  \param tup    
*/
// --------------------------------------------------------------------------------
void XpracMan::slotTupleChanged( QmvTuple * tup )
{
    if ( !tup )
        return;
    
    QmvSqlClass * reln = (QmvSqlClass *) tup->parentClass();

        // Appointments
    if ( reln->relationName() == "evnv" )
        slotTupleSelected( -1, tup );    // refresh screen
        
}

// --------------------------------------------------------------------------------
void XpracMan::addRowToSetTrigger( QString & reln, int sequence )
{
        // Underdevelopment - so hardwired for now.
    if ( reln == "docs" )
        documentInterface( reln, sequence, DocumentCreate );
}

// --------------------------------------------------------------------------------
void XpracMan::deleteRowFromSetTrigger( QString & reln, int sequence )
{
        // Underdevelopment - so hardwired for now.
    if ( reln == "docs" )
        documentInterface( reln, sequence, DocumentDelete );
}

// --------------------------------------------------------------------------------
void XpracMan::accessRowTrigger( QString & reln, int sequence )
{
        // Underdevelopment - so hardwired for now.
    if ( reln == "docs" )
        documentInterface( reln, sequence, DocumentAccess );
}

// --------------------------------------------------------------------------------
void XpracMan::documentInterface( QString & reln, int docseq, documentMode mode )
{
        // get the patient tuple
        // look for the dbwidget by the title of the current database
    QmvDBWidget * dbw = dict_dbtitle.find( database->dbTitle() );
    if ( !dbw )
        return;

    QmvTuple * docs_tuple = dbw->getSelectedTuple( "docs" );
    if ( !docs_tuple )
    {
        QMessageBox::warning( 0, "documentInterface:warning",
                              tr("Select a document."),
                              "OK", 0 );
        return;  
    }

    QString patn__sequence = docs_tuple->attributeValue("docs_patn__sequence");
    QString docs_url = docs_tuple->attributeValue("docs_url");
    
        // Prepare the patient data --------------------------------
        // Maintain a local copy of the patv object
    static QmvSqlClass * reln_patv;
    if ( !reln_patv )
    {
        QmvSqlClass * patv_existing = (QmvSqlClass *) database->dbRelation( "patv" );
        if ( patv_existing )
            reln_patv = new QmvSqlClass( *patv_existing );
        else
        {
            QMessageBox::warning( 0, "documentInterface:warning",
                                  tr("Patient details object cannot be located."),
                                  "OK", 0 );
            return;
        }
    }

    reln_patv->open( QString( "select * from patv where patv__sequence = %1" )
                     .arg(patn__sequence) );
    if ( reln_patv->count() != 1 )
    {
        QMessageBox::warning( 0, "documentInterface:warning",
                              tr("No patient details can be located."),
                              "OK", 0 );
        return;  
    }
    QmvTuple * patv_tuple = reln_patv->tupleAt(0);

        // Get environmental variables
    static QString docroot = getenv( "PRAC_DOCROOT" );
    static QString docsuffix = getenv( "PRAC_DOCSUFFIX" );
    static QString doctemplatedir = getenv( "PRAC_DOCTEMPLATEDIR" );
    static QString doctemplate = getenv ( "PRAC_DOCTEMPLATE" );
    static bool doc_init = false;
    
        // Internal defaults
    if ( !doc_init )
    {
        if ( docroot.length() < 1 )
        {
            docroot = QDir::currentDirPath();
        }
        docroot = QString( "%1/%2" ).arg(docroot).arg(database->dbName());
        docroot.replace( QRegExp( "/$" ), "" );
        if ( docsuffix.length() < 1 )
            docsuffix = "txt";
        if ( doctemplatedir.length() < 1 )
            doctemplatedir = QString( "%1/TEMPLATES" ).arg(docroot);
        if ( doctemplate.length() < 1 )
            doctemplate = QString( "xpracman.%1" ).arg(docsuffix);

        doctemplate = QString("%1/%2").arg(doctemplatedir).arg(doctemplate);
        
        doc_init = true;
    }
    
    if ( mode == DocumentCreate )
    {
            // check for the templates dir
        QDir td;
        if ( !td.cd( doctemplatedir ) )
        {
            QMessageBox::warning( 0, "Select Template",
                                  tr("<b>Failed to locate TEMPLATE directory:</b><br>"
                                     "The Template Directory:<br><b>%1</b><br>"
                                     " could not be located.<br>")
                                  .arg(doctemplatedir),
                                  "OK", 0);
            td.mkdir( doctemplatedir);

            return;
        }

        QStringList template_list = td.entryList(QString("*.%1").arg(docsuffix));
        int template_count = template_list.count();
        QString tmp_template;
        if ( template_count < 1 )
        {
            QMessageBox::warning( 0, "documentInterface:warning",
                                  tr( QString( "No templates were found in :<BR>  %1").arg(doctemplatedir)),
                                  "OK", 0 );
            return;
        } else if ( template_count > 1 ) {
            tmp_template = QFileDialog::getOpenFileName( doctemplate, QString("*.%1").arg(docsuffix),
                                                             this, "xpracman_findtemplate", "Select a Template" );
        } else {
            tmp_template = QString("%1/%2").arg(doctemplatedir).arg(template_list[0]);
        }
        
        if ( tmp_template.isEmpty() )
        {
            QMessageBox::warning( 0, "documentInterface:warning",
                                  tr( QString( "Operator cancelled new document creation")),
                                  "OK", 0 );
            return;
        } else {
            doctemplate = tmp_template;
        }
    }
    

        // Check for docroot
    QDir dir_docroot( docroot ); 
    if ( !dir_docroot.exists() )
    {
        if ( !dir_docroot.mkdir( docroot ) )
        {
            QMessageBox::warning( 0, "documentInterface:warning",
                                  tr( QString( "Could not create the directory:<br>%1").arg(docroot) ),
                                  "OK", 0 );
            return;
        }
    }
    

    if ( docs_url.length() < 1 )
    {
            // Establish document subdirectory for patient
        QString docdir = QString( "%1_%2_%3" )
            .arg( patv_tuple->attributeValue("patv_psnam") )
            .arg( patv_tuple->attributeValue("patv_fsnam").left(1) )
            .arg( patv_tuple->attributeValue("patv__sequence") );
        docdir.replace( QRegExp( "[^A-z0-9]" ), "_" );
        QString fulldocdir = QString( "%1/%2").arg( docroot ).arg( docdir);
        
        QDir dir_fulldocdir( fulldocdir ); 
        if ( !dir_fulldocdir.exists() )
        {
            if ( !dir_fulldocdir.mkdir( fulldocdir ) )
            {
                QMessageBox::warning( 0, "documentInterface:warning",
                                      tr( QString( "Could not create the directory:<br>%1")
                                          .arg(fulldocdir) ),
                                      "OK", 0 );
                return;
            }
        }
        
            // construct document name
        QDate today = QDate::currentDate();
        docs_url = QString( "%1/%2-%3-%4-%5_%6.%7" )
            .arg(docdir)
            .arg(patv_tuple->attributeValue("patv_psnam"))
            .arg(today.year()).arg(today.month()).arg(today.day())
            .arg( docseq )
            .arg( docsuffix );
        docs_url.replace( QRegExp( "[^A-z0-9/.]" ), "_" );
    }
    
    QString curdir = QDir::currentDirPath();
    QDir::setCurrent( docroot );
    switch ( mode )
    {
        case DocumentCreate:
                // construct document
            
            if ( !documentCreate( docs_url, reln_patv,
                                  QString( "%1" )
                                  .arg(doctemplate) ) )
                break;
            else
            {
                docs_tuple->update("docs_url", docs_url );
                docs_tuple->save();
            }
            
                // then access
        case DocumentAccess:
            documentAccess( docs_url );
            break;
        case DocumentDelete:
            documentDelete( docs_url );
            break;
    }
    QDir::setCurrent( curdir );

}

// --------------------------------------------------------------------------------
bool XpracMan::documentCreate( const QString & docname, QmvSqlClass * reln_patv,
                               const QString & doctemplate )
{
   
        // Prepare the file interfaces -----------------------------
    QFile f_template( doctemplate );
    QFile f_doc( docname );

    if ( !f_template.open(IO_ReadOnly) )
    {    
        QMessageBox::warning( 0, "documentCreate:warning",
                              tr( QString( "The document <br> (%1) <br> cannot be opened." )
                                  .arg(doctemplate) ),
                              "OK", 0 );
        return(FALSE);
    }
    
    if ( !f_doc.open(IO_WriteOnly) )
    {
        QMessageBox::warning( 0, "documentCreate:warning",
                              tr( QString( "The document <br> (%1) <br> cannot be opened." )
                                  .arg(docname ) ),
                              "OK", 0 );
        return(FALSE);
    }

    QTextStream t_template( &f_template );    
    QTextStream t_doc( &f_doc );
    QString str_template;

        // Read the template and substitute values from patv object
    str_template = t_template.read();
        // Build a default list of attributes
    QmvAttList atlist = reln_patv->sortedAttlist( "mtat_view_order",
                                                  QmvClass::NoBlank|QmvClass::NoSystem|QmvClass::NoZero );
    for ( QmvAttribute * at_ptr = atlist.first(); at_ptr != 0; at_ptr = atlist.next() )
    {
        QString *att = at_ptr->find( "mtat_name" );
        str_template.replace( *att, reln_patv->attributeValue( 0, *att ) );
    }
    str_template.replace( QString( "<br>" ), QString( "\n\\par " ) );
    QDate today = QDate::currentDate();
    str_template.replace( QString( "_TODAY" ),
			  QString("%1-%2-%3").arg(today.day()).arg(today.month()).arg(today.year()) );
    
        // Write and close
    t_doc << str_template;
    f_template.close();
    f_doc.close();

    return(TRUE);
}

// --------------------------------------------------------------------------------
void XpracMan::documentAccess( const QString & docname )
{
    if ( !QFile::exists( docname ) )
    {    
        QMessageBox::warning( 0, "documentAccess:warning",
                              tr( QString( "The document (%1) does not exist." )
                                  .arg(docname) ),
                              "OK", 0 );
        
        return;  
    }
        
    QString doceditor = getenv( "PRAC_DOCEDITOR" );
    if ( doceditor.length() < 1 )
        doceditor = "xedit";
    else
        if ( !doceditor.contains( "/") )
            doceditor.prepend( ::XpracManDir + "/" );
    
    QString pi_command = QString("env DBNAME=%1 %2 %3&")
        .arg(database->dbName())
        .arg( doceditor )
        .arg( docname );
    int status = system( pi_command );

    if ( status )
    {    
        QMessageBox::warning( 0, "documentAccess:warning",
                              tr( QString( "An error occurred while accessing the document (%1).")
                                  .arg( docname ) ),
                              "OK", 0 );
        return;  
    }
}

// --------------------------------------------------------------------------------
void XpracMan::documentDelete( const QString & docname )
{   
    if ( QFile::exists( docname ) )
        QFile::remove( docname );
}        

/*
 * ======================================================================
 * SLOT XpracMan::loadWaitingPatients()
 *
 * Select waiting patients
 * 
 * ======================================================================
 */

void XpracMan::loadWaitingPatients( const QDate & xdate )
{

    QString appdate;
    if ( xdate.isNull() )
        appdate = QDate::currentDate().toString();
    else
        appdate = QString( xdate.toString() );

//      displayPatients( QString( "select * from patn where patn__sequence"
//                                " in ( select wait_patn__sequence from wait"
//                                " where date_trunc('day', wait_start)"
//                                " = date_trunc('day', '%1'::timestamp) )"
//                                " order by patn_psnam, patn_pfnam" )
//                       .arg(appdate) );

} /* XpracMan::load_waiting_patients */

/*
 * ======================================================================
 * SLOT XpracMan::loadRecentPatients()
 *
 * Select recent patients
 * 
 * ======================================================================
 */

void XpracMan::loadRecentPatients( const QDate & xdate )
{
    QString svcdate;
    if ( xdate.isNull() )
        svcdate = QDate::currentDate().toString();
    else
        svcdate = QString( xdate.toString() );
    
    displayQuerySet( "patn", QString( "select * from patn where patn__sequence in "
                                      " ( select svpf_patn__sequence from svpf"
                                      "   where svpf_date_service > '%1'::timestamp - '1 week'::interval"
                                      " )" )
                     .arg( svcdate ) );

} /* XpracMan::load_recent_patients */



// --------------------------------------------------------------------------------
// Reports
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
void XpracMan::printReport( )
{
    
    QPopupMenu popup_report( this, "popup_report" );
    QLabel * popup_label = new QLabel("Reports", this);
    popup_label->setAlignment( Qt::AlignCenter );
    popup_report.insertItem( popup_label, -1, -1 );
    popup_report.insertSeparator();
    popup_report.insertItem( "&Unprinted Invoices", ReportUnprintedInvoices );
    popup_report.insertItem( "&Statements", ReportStatement );
    popup_report.insertItem( "Work &List", ReportWorkList );
    popup_report.insertItem( "&Work Summary", ReportWorkSummary );
    popup_report.insertItem( "C&redit List", ReportCreditList );
    popup_report.insertItem( "&Credit Summary", ReportCreditSummary );
    popup_report.insertItem( "W&ork/Credit List", ReportWorkCreditList );
    popup_report.insertItem( "&Aged Debtors", ReportAgedDebtors );
    if ( (QmvSqlClass *) database->dbRelation( "gsti" ) )
        popup_report.insertItem( "GS&T Invoices", ReportGSTInvoices );
    if ( (QmvSqlClass *) database->dbRelation( "gstv" ) )
        popup_report.insertItem( "&GST Receipts", ReportGSTReceipts );
    if ( (QmvSqlClass *) database->dbRelation( "eftv" ) )
        popup_report.insertItem( "&Directed Credits", ReportDirectedCredits );

    int report_type = popup_report.exec( QCursor::pos() );
    
    static QDateTime from = QDateTime::currentDateTime();
    static QDateTime to = QDateTime::currentDateTime();
    int month, year;
    
    
    switch ( report_type )
    {
        case ReportUnprintedInvoices:
            printInvoice( QString("-1") );
            break;
            
        case ReportStatement:
            from = QDateTime::currentDateTime();
            to = QDateTime();
            if ( setDateRange( "Statements", "Statement Date", "", &from, &to ) )
                printStatement( from.date().daysTo( QDate::currentDate() ) );
            break;
            
        case ReportAgedDebtors:
            from = QDateTime::currentDateTime();
            to = QDateTime();
            if ( setDateRange( "Aged Debtors",  "Age to", "",  &from, &to ) )
                printAged( from.date().daysTo( QDate::currentDate() ) );
            break;
            
        case ReportGSTInvoices:
            month = (( (from.date().month() - 1) / 3 ) * 3 ) + 1;
            year  = from.date().year();
            from = QDateTime(QDate(year, month, 1));
            month += 3;
            if (month > 12) {
                month %= 3;
                year++;
            }
            to = QDateTime(QDate(year, month, 1).addDays(-1));
            
            if ( setDateRange( "GST Invoices",  "Start Date",  "End Date", &from, &to ) )
                printGSTInvoices( from, to );
            break;
            
        case ReportGSTReceipts:
            month = (( (from.date().month() -1 ) / 3 ) * 3 ) + 1;
            year  = from.date().year();
            from = QDateTime(QDate(year, month, 1));
            month += 3;
            if (month > 12) {
                month %= 3;
                year++;
            }
            to = QDateTime(QDate(year, month, 1).addDays(-1));
            
            if ( setDateRange( "GST Receipts",  "Start Date",  "End Date", &from, &to ) )
                printGSTReceipts( from, to );
            break;
            
        case ReportDirectedCredits:
            if ( sql_exec( "insert into eftr default values" ) == 1 )
                printDirectedCredits();
            else
                QMessageBox::information(0, "printDirectedCredits:information",
                                         tr("There is nothing to print."),
                                         "OK", 0 );
            break;
            
        case ReportWorkList:
            from = QDateTime::currentDateTime();
            to = QDateTime::currentDateTime();
            if ( setDateRange( "Work List",  "Start Date", "End Date", &from, &to ) )
                printWorkList( from, to );
            break;
            
        case ReportWorkSummary:
            from = QDateTime::currentDateTime();
            to = QDateTime::currentDateTime();
            if ( setDateRange( "Work Summary",  "Start Date", "End Date",  &from, &to ) )
                printWorkSummary( from, to );
            
            break;
            
        case ReportCreditList:
            from = QDateTime::currentDateTime();
            to = QDateTime::currentDateTime();
            if ( setDateRange( "Credit List",  "Start Date", "End Date", &from, &to ) )
                printCreditList( from, to );
            break;
            
        case ReportCreditSummary:
            from = QDateTime::currentDateTime();
            to = QDateTime::currentDateTime();
            if ( setDateRange( "Credit Summary",  "Start Date", "End Date", &from, &to ) )
                printCreditSummary( from, to );            
            break;
            
        case ReportWorkCreditList:
            from = QDateTime::currentDateTime();
            to = QDateTime::currentDateTime();
            if ( setDateRange( "Work/Credit List",  "Start Date", "End Date", &from, &to ) )
                printWorkCreditList( from, to );
            break;
            
        default:
            break;
            
    }
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
bool XpracMan::setDateRange( const QString & title, const QString & from_title, const QString & to_title,
                             QDateTime *from, QDateTime *to )
{

    QString from_str;
    QString to_str;
    bool from_enabled = false;
    bool to_enabled = false;
    
        // set dates, hide to-date if null
    if ( !from->isNull() ) {
        from_enabled = true;
        from_str = QString( "%1-%2-%3" )
            .arg( from->date().day() )
            .arg( from->date().month() )
            .arg( from->date().year() );
    }
    
    if ( !to->isNull() ) {
        to_enabled = true;
        to_str = QString( "%1-%2-%3" )
            .arg( to->date().day() )
            .arg( to->date().month() )
            .arg( to->date().year() );
    }
    
        // setup Date entry widget
    QDialog * dl_date = new QDialog( this, "date range dialog", TRUE );
    dl_date->setCaption(tr("Dates"));
        
    QVBox * vt_datenav = new QVBox( dl_date, "date range vbox");
    QHBox * hz_datenav = new QHBox( vt_datenav, "date range hbox" );
    QHBox * hz_controls = new QHBox( vt_datenav, "controls hbox" );
    QVBox * vt_fromto = new QVBox(hz_datenav, "From-to stack" );
    QGroupBox * gb_fromto = new QGroupBox( 4, Qt::Vertical, title, vt_fromto, "Date Range display" );
    QLabel * lab_from = new QLabel(from_title, gb_fromto, "");
    QLineEdit * le_from = new QLineEdit( from_str, gb_fromto, "display from" );
    QLabel * lab_to = new QLabel(to_title, gb_fromto, "");
    QLineEdit * le_to = new QLineEdit( to_str, gb_fromto, "display from" );
    
        // controls

    QPushButton * pb_accept = new QPushButton( "OK", hz_controls );
    QWhatsThis::add(pb_accept , "Use the dates ... continue" );
    connect( pb_accept, SIGNAL( clicked() ), dl_date, SLOT( accept() ) );

    QPushButton * pb_reject = new QPushButton( "CANCEL", hz_controls );
    QWhatsThis::add(pb_reject , "Use the dates ... continue" );
    connect( pb_reject, SIGNAL( clicked() ), dl_date, SLOT( reject() ) );

    QAccel * ac_datenav = new QAccel( dl_date );
    ac_datenav->connectItem( ac_datenav->insertItem( CTRL+Key_W ), dl_date, SLOT( reject()) );
    ac_datenav->connectItem( ac_datenav->insertItem( Key_Return ), dl_date, SLOT( accept()) );
    
    if ( !from_enabled ) {
        lab_from->hide();
        le_from->hide();
    }
    if ( !to_enabled ) {
        lab_to->hide();
        le_to->hide();
    }
    if (!from_enabled && !to_enabled )
        return false;
   
        // Set the sizes
    vt_datenav->adjustSize();
    dl_date->adjustSize();
    dl_date->setFixedWidth( dl_date->width() );
    dl_date->setFixedHeight( dl_date->height() );

    if ( from_enabled )
        dl_date->setFocusProxy(le_from);
    else
        dl_date->setFocusProxy(le_to);
    
    dl_date->setFocus();

    bool rv = false;    // overall success flag
    
        // Transfer the results.
    while ( true )
    {
        QDateTime new_from;
        QDateTime new_to;
        
            // Run the dialog
        int status = dl_date->exec();
        if (!status)
            break;
        
            // test from-date
        if ( from_enabled ) {
            new_from = QmvClass::strToDateTime( le_from->text() );
            if ( !new_from.isValid() ) {
                if ( QMessageBox::warning( this, "setDateRange",
                                           tr("The \"Start Date\" date you entered is invalid: %1 <br>"
                                              "Do you want to re-enter this date ?")
                                           .arg( le_from->text() ),
                                           QMessageBox::Ok|QMessageBox::Default,
                                           QMessageBox::Cancel|QMessageBox::Escape ) == QMessageBox::Ok )
                    continue;
                else
                    break;
            }    
        }
        
            // Test to-date
        if ( to_enabled ) {
            new_to = QmvClass::strToDateTime( le_to->text() );
            if ( !new_to.isValid() ) {
                if ( QMessageBox::warning( this, "setDateRange",
                                           tr("The \"End Date\" date you entered is invalid: %1 <br>"
                                              "Do you want to re-enter this date ?")
                                           .arg( le_to->text() ),
                                           QMessageBox::Ok|QMessageBox::Default,
                                           QMessageBox::Cancel|QMessageBox::Escape ) == QMessageBox::Ok )
                    continue;
                else
                    break;
            }    
        }

            // if we get to here, then both dates are successful.
        if ( from_enabled )
            from->setDate( new_from.date() );
        if ( to_enabled )
            to->setDate( new_to.date() );
        rv = true;
        break;
    }

    delete dl_date;
    return rv;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
bool XpracMan::printAged( int days_ago )
{
        // Maintain a local copy of the dbagobject
    static QmvSqlClass * reln_dbag;
    if ( !reln_dbag )
    {
        QmvSqlClass * dbag_existing = (QmvSqlClass *) database->dbRelation( "dbag" );
        if ( dbag_existing )
            reln_dbag = new QmvSqlClass( *dbag_existing );
        else
            return(FALSE);
    }
    
        // Maintain a local copy of the form object
    static QmvSqlClass * reln_form;
    if ( !reln_form )
    {
        QmvSqlClass * form_existing = ( QmvSqlClass *) database->dbRelation( "form" );
        if ( form_existing )
            reln_form = new QmvSqlClass( *form_existing );
        else
            return(FALSE);
    }
    
    QString query = "select * from dbag order by dbag_patient_key";

        // disable the MD timer (hack) to prevent transaction conflict
    bool hold_timer_exportmd2 = timer_exportmd2;
    setExportMD2( false );
        // Begin the transaction which protects the current setting for dbst_statement_age
    sql_exec("begin work");
        // load the conf row for dbst_statement_age
    sql_exec ( QString( "delete from conf where conf_code = 'dbst_statement_age';"
                        " insert into conf(conf_code, conf_desc, conf_value)"
                        " values('dbst_statement_age', 'Statement date is %1 days ago', '%2');" )
               .arg(days_ago).arg(days_ago) );
    
    QProgressDialog controller( "Loading records...", "Cancel", 100,
                                this, "progress", TRUE );
    int status = reln_dbag->open( QString( query ), &controller );
    controller.cancel();
    sql_exec("abort work");
    setExportMD2( hold_timer_exportmd2 ); // reinstate
    
        // Load the dbag data
    switch ( status )
    { 
        case -1:
            QMessageBox::warning( 0, "warning",
                                  tr("<b>%1:</b><br>"
                                     "The data query failed. The following message"
                                     " may help to explain the failure.<br><br>"
                                     "SQL: %2")
                                  .arg( reln_dbag->relationTitle() )
                                  .arg( reln_dbag->lastError() ),
                                  "OK", 0 );
            return(FALSE);
            break;
        case -2:
            QMessageBox::warning( 0, "warning",
                                  tr("<b>%1:</b><br>"
                                     "The data query has been cancelled as requested.")
                                  .arg( reln_dbag->relationTitle() ),
                                  "OK", 0 );
            return(FALSE);
            break;
        default:

            break;
    }
    
        // No dbag rows;
    if ( reln_dbag->count() < 1 )
    {
        QMessageBox::warning( 0, "warning",
                              tr("<b>%1:</b><br>"
                                 "No data found" )
                              .arg( reln_dbag->relationTitle() ),
                              "OK", 0 );
        return(FALSE);
    }
    
        // load the form specifications
    QString form_code = "dbaged";
    reln_form->open( QString( "select * from form where form_code = '%1'::text" )
                     .arg( form_code ) );
    
        // no form spec.
    if ( reln_form->count() != 1 )
    {
        QMessageBox::warning( 0, "warning",
                              tr("<b>%1:</b><br>"
                                 "There is no form named <%2>. This form is required for the "
                                 " aged debtor report." )
                              .arg( reln_dbag->relationTitle() )
                              .arg( form_code ),
                              "OK", 0 );
        return(FALSE);
    }
    

        // Setup a form viewer, and prepare the aged report
    static QGuardedPtr<QmvForm> form_viewer;
    if ( !form_viewer )
        form_viewer = new QmvForm( reln_dbag, this, "form_dbag" );

    if ( !form_viewer->initForm( form_code ) )
    {
        QMessageBox::information(0, "printAged:information",
                                 tr("<b>%1:</b><br><br>This form <b>%2</b> cannot be initialised.")
                                 .arg( reln_dbag->relationTitle() )
                                 .arg( form_code ),
                                 "OK", 0 );
        return(FALSE);
    }
    controller.setLabelText(" Preparing aged debtors report for viewing ..." );
    controller.setProgress( 10 );
    form_viewer->setTemplate();
    form_viewer->setData();
    
    controller.setProgress( 20 );
    form_viewer->writeTemplate();       // debugging
    form_viewer->writeData();           // debugging
    
    controller.setProgress( 30 );
    form_viewer->renderForm();
    
    controller.cancel();
    form_viewer->viewForm();

    
    return(TRUE);

}

// --------------------------------------------------------------------------------
bool XpracMan::printGSTInvoices(  const QDateTime &from, const QDateTime &to )
{

        // setup the query range
    QDate x_from = (from.isValid()) ? from.date() : QDate::currentDate();
    QDate x_to   = (to.isValid()) ? to.date() : QDate::currentDate();

    QString from_str = QString("%1-%2-%3")
        .arg(x_from.day())
        .arg(x_from.month())
        .arg(x_from.year());
    QString to_str = QString("%1-%2-%3")
        .arg(x_to.day())
        .arg(x_to.month())
        .arg(x_to.year());

    
        // Maintain a local copy of the gsti object
    static QmvSqlClass * reln_gsti;
    if ( !reln_gsti )
    {
        QmvSqlClass * gsti_existing = (QmvSqlClass *) database->dbRelation( "gsti" );
        if ( gsti_existing )
            reln_gsti = new QmvSqlClass( *gsti_existing );
        else
            return(FALSE);
    }
 
    int i = reln_gsti->open( QString ("select * from gsti"
                                      " where date(gsti_invc_date_printed) >= '%1'"
                                      " and date(gsti_invc_date_printed) <= '%2'"
                                      " order by gsti_invc_date_printed" )
                             .arg( from_str )
                             .arg( to_str ));
 
    if ( i < 1 )
    {
        QMessageBox::information(0, "printGSTInvoices:information",
                                 tr("There is nothing to print."),
                                 "OK", 0 );
        return(FALSE);
    }
    
    
        // Setup a form viewer, and prepare the invoice.
    static QGuardedPtr<QmvForm> form_viewer;
    if ( !form_viewer )
        form_viewer = new QmvForm( reln_gsti, this, "form_gsti" );

    if ( !form_viewer->initForm( "gst-invoices" ) )
        return(FALSE);

    form_viewer->setPrint( userPreference("PrintProgram"),
                           userPreference("PrinterSelectionOption"),
                           userPreference("PrinterName") );
    
    if ( userPreference("ViewFormFirst").lower() == "yes" )
        form_viewer->viewForm();
    else
        form_viewer->printForm();
}
// --------------------------------------------------------------------------------
bool XpracMan::printGSTReceipts( const QDateTime &from, const QDateTime &to )
{
        // setup the query range
    QDate x_from = (from.isValid()) ? from.date() : QDate::currentDate();
    QDate x_to   = (to.isValid()) ? to.date() : QDate::currentDate();

    QString from_str = QString("%1-%2-%3")
        .arg(x_from.day())
        .arg(x_from.month())
        .arg(x_from.year());
    QString to_str = QString("%1-%2-%3")
        .arg(x_to.day())
        .arg(x_to.month())
        .arg(x_to.year());

    
        // Maintain a local copy of the gstv object
    static QmvSqlClass * reln_gstv;
    if ( !reln_gstv )
    {
        QmvSqlClass * gstv_existing = (QmvSqlClass *) database->dbRelation( "gstv" );
        if ( gstv_existing )
            reln_gstv = new QmvSqlClass( *gstv_existing );
        else
            return(FALSE);
    }
 
    int i = reln_gstv->open( QString ("select * from gstv"
                                      " where date(gstv_bkdp_date_created) >= '%1'"
                                      " and date(gstv_bkdp_date_created) <= '%2'"
                                      " order by gstv_bkdp_date_created, gstv_paym_date_entry" )
                             .arg( from_str )
                             .arg( to_str ));
 
    if ( i < 1 )
    {
        QMessageBox::information(0, "printGSTReceipts:information",
                                 tr("There is nothing to print."),
                                 "OK", 0 );
        return(FALSE);
    }
    
    
        // Setup a form viewer, and prepare the invoice.
    static QGuardedPtr<QmvForm> form_viewer;
    if ( !form_viewer )
        form_viewer = new QmvForm( reln_gstv, this, "form_gstv" );

    if ( !form_viewer->initForm( "gst-receipts" ) )
        return(FALSE);

    form_viewer->setPrint( userPreference("PrintProgram"),
                           userPreference("PrinterSelectionOption"),
                           userPreference("PrinterName") );
    
    if ( userPreference("ViewFormFirst").lower() == "yes" )
        form_viewer->viewForm();
    else
        form_viewer->printForm();
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
bool XpracMan::printWorkList( const QDateTime &from, const QDateTime &to )
{
        // setup the query period
    QDateTime x_from;
    QDateTime x_to;
    x_from.setDate( QDate::currentDate() );
    static QGuardedPtr<QmvForm> form_viewer;
     
    if ( !from.isNull() )
        x_from = from;
    if ( !to.isNull() )
        x_to = to;
    if ( x_to.isNull() )
        x_to = x_from;
    
        // Maintain a local copy of the prov object
    static QmvSqlClass * reln_prov = 0;
    if ( !reln_prov )
    {
        QmvSqlClass * prov_existing = (QmvSqlClass *) database->dbRelation( "prov" );
        if ( prov_existing )
            reln_prov = new QmvSqlClass( *prov_existing );
        else
            return(false);
    }
        // Maintain a local copy of the svltobject
    static QmvSqlClass * reln_svlt = 0;
    if ( !reln_svlt )
    {
        QmvSqlClass * svlt_existing = (QmvSqlClass *) database->dbRelation( "svlt" );
        if ( svlt_existing )
            reln_svlt = new QmvSqlClass( *svlt_existing );
        else
            return(false);
    }
    
    QString str_from = QString("%1-%2-%3")
        .arg(x_from.date().day())
        .arg(x_from.date().month())
        .arg(x_from.date().year());
    QString str_to = QString("%1-%2-%3")
        .arg(x_to.date().day())
        .arg(x_to.date().month())
        .arg(x_to.date().year());

        // get a list of the represented providers, to enable
        // separate reports
    int n_prov = reln_prov->open("select * from prov order by prov_name");
  
    if ( n_prov < 1 )
    {
        QMessageBox::information(0, "printWorkSummary:Nothing",
                                 tr("There are no providers."),
                                 "OK", 0 );
        return(false);
    }

    int active_prov = 0;        // count the providers with activity

    QString sv_date;
    if (userPreference("ReportByRealDate").lower() == "yes" )
        sv_date = "date(svlt__timestamp) >= '%1' and date(svlt__timestamp) <= '%2'";
    else
        sv_date = "date(svlt_date_service) >= '%1' and date(svlt_date_service) <= '%2'";

    for ( int p = 0; p < n_prov; p++ )
    {
        QString query =  QString ("select * from svlt"
                                  " where " + sv_date +
                                  " and svlt_prov_code = '%3' %4" )
            .arg( str_from ).arg( str_to).arg(reln_prov->attributeValue(p, "prov_code") )
            .arg( reln_svlt->orderBy() );
    
        QProgressDialog controller( QString("Loading records for %1").arg(reln_prov->attributeValue(p, "prov_name")),
                                    "Cancel", 100, this, "progress", TRUE );
        int i = reln_svlt->open( query, &controller );
        controller.cancel();
        if (i < 1)
            continue;
    
  
        active_prov++;
    
            // Setup a form viewer, and prepare the invoice.
        if ( !form_viewer )
            form_viewer = new QmvForm( reln_svlt, this, "form_svlt" );

        if ( !form_viewer->initForm( "list-svlt" ) )
            return(false);

        form_viewer->writeTemplate();       // debugging
        form_viewer->writeData();           // debugging
    
        form_viewer->setPrint( userPreference("PrintProgram"),
                               userPreference("PrinterSelectionOption"),
                               userPreference("PrinterName") );
    
        if ( userPreference("ViewFormFirst").lower() == "yes" )
            form_viewer->viewForm();
        else
            form_viewer->printForm();
    
    }
    
    if ( active_prov < 1 )
    {
        QMessageBox::information(0, "printWorkSummary:Nothing",
                                 tr("There is nothing to print."),
                                 "OK", 0 );
        return(false);
    }
    
}



// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
bool XpracMan::printWorkSummary( const QDateTime &from, const QDateTime &to )
{
        // setup the query period
    QDateTime x_from;
    QDateTime x_to;
    x_from.setDate( QDate::currentDate() );
    int return_status = true;
    static QGuardedPtr<QmvForm> form_viewer;
     
    if ( !from.isNull() )
        x_from = from;
    if ( !to.isNull() )
        x_to = to;
    if ( x_to.isNull() )
        x_to = x_from;
    
        // Maintain a local copy of the prov object
    static QmvSqlClass * reln_prov = 0;
    if ( !reln_prov )
    {
        QmvSqlClass * prov_existing = (QmvSqlClass *) database->dbRelation( "prov" );
        if ( prov_existing )
            reln_prov = new QmvSqlClass( *prov_existing );
        else
            return(false);
    }
    
        // Maintain a local copy of the svsmobject
    static QmvSqlClass * reln_svsm = 0;
    if ( !reln_svsm )
    {
        QmvSqlClass * svsm_existing = (QmvSqlClass *) database->dbRelation( "svsm" );
        if ( svsm_existing )
            reln_svsm = new QmvSqlClass( *svsm_existing );
        else
            return(false);
    }
    
    QString str_from = QString("%1-%2-%3")
        .arg(x_from.date().day())
        .arg(x_from.date().month())
        .arg(x_from.date().year());
    QString str_to = QString("%1-%2-%3")
        .arg(x_to.date().day())
        .arg(x_to.date().month())
        .arg(x_to.date().year());

        // get a list of the represented providers, to enable
        // separate reports
    int n_prov = reln_prov->open("select * from prov order by prov_name");
  
    if ( n_prov < 1 )
    {
        QMessageBox::information(0, "printWorkSummary:Nothing",
                                 tr("There are no providers."),
                                 "OK", 0 );
        return(false);
    }

    int active_prov = 0;        // count the providers with activity

        // disable the MD timer (hack) to prevent transaction conflict
    bool hold_timer_exportmd2 = timer_exportmd2;
    setExportMD2( false );
        // Begin the transaction which protects the current worklist
    sql_exec("begin work");
        // load the work list with date range
    QString sv_date = "svpf_date_service";
    if (userPreference("ReportByRealDate").lower() == "yes" )
        sv_date = "svpf__timestamp";
    sql_exec( QString("select svsm_load('%1', '%2', '%3')").arg( str_from ).arg( str_to).arg(sv_date) );

        // work through the provider list
    for ( int p = 0; p < n_prov; p++ )
    {
            // build current provider list
        QString query =  QString ("select * from svsm"
                                  " where svsm_prov_code = '%1'"
                                  " order by svsm_type, svsm_serv_code;")
            .arg(reln_prov->attributeValue(p, "prov_code") );

        QProgressDialog controller( QString("Loading records for %1")
                                    .arg(reln_prov->attributeValue(p, "prov_name")),
                                    "Cancel", 100, this, "progress", TRUE );
        int count = reln_svsm->open( query, &controller );
        controller.cancel();
        if (count < 1)
            continue;
    
        active_prov++;
        
            // Setup a form viewer, and prepare the report.
        if ( !form_viewer )
            form_viewer = new QmvForm( reln_svsm, this, "form_svsm" );

        if ( !form_viewer->initForm( "list-svsm" ) )
        {
            return_status = false;
            break;
        }
        
        form_viewer->setPrint( userPreference("PrintProgram"),
                               userPreference("PrinterSelectionOption"),
                               userPreference("PrinterName") );
    
        if ( userPreference("ViewFormFirst").lower() == "yes" )
            form_viewer->viewForm();
        else
            form_viewer->printForm();
    }

    sql_exec("abort work");
    setExportMD2( hold_timer_exportmd2 ); // reinstate
  
    if ( active_prov < 1 )
    {
        QMessageBox::information(0, "printWorkSummary:Nothing",
                                 tr("There is nothing to print."),
                                 "OK", 0 );
        return(false);
    }
    return return_status;
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
bool XpracMan::printCreditList( const QDateTime &from, const QDateTime &to )
{
        // setup the query period
    QDateTime x_from;
    QDateTime x_to;
    x_from.setDate( QDate::currentDate() );
    static QGuardedPtr<QmvForm> form_viewer;
     
    if ( !from.isNull() )
        x_from = from;
    if ( !to.isNull() )
        x_to = to;
    if ( x_to.isNull() )
        x_to = x_from;
    
        // Maintain a local copy of the prov object
    static QmvSqlClass * reln_prov = 0;
    if ( !reln_prov )
    {
        QmvSqlClass * prov_existing = (QmvSqlClass *) database->dbRelation( "prov" );
        if ( prov_existing )
            reln_prov = new QmvSqlClass( *prov_existing );
        else
            return(false);
    }
        // Maintain a local copy of the crltobject
    static QmvSqlClass * reln_crlt = 0;
    if ( !reln_crlt )
    {
        QmvSqlClass * crlt_existing = (QmvSqlClass *) database->dbRelation( "crlt" );
        if ( crlt_existing )
            reln_crlt = new QmvSqlClass( *crlt_existing );
        else
            return(false);
    }
    
    QString str_from = QString("%1-%2-%3")
        .arg(x_from.date().day())
        .arg(x_from.date().month())
        .arg(x_from.date().year());
    QString str_to = QString("%1-%2-%3")
        .arg(x_to.date().day())
        .arg(x_to.date().month())
        .arg(x_to.date().year());

        // get a list of the represented providers, to enable
        // separate reports
    int n_prov = reln_prov->open("select * from prov order by prov_name");
  
    if ( n_prov < 1 )
    {
        QMessageBox::information(0, "printWorkSummary:Nothing",
                                 tr("There are no providers."),
                                 "OK", 0 );
        return(false);
    }

    int active_prov = 0;        // count the providers with activity

    QString cr_date;
    if (userPreference("ReportByRealDate").lower() == "yes" )
        cr_date = "date(crlt__timestamp) >= '%1' and date(crlt__timestamp) <= '%2'";
    else
        cr_date = "date(crlt_date_payment) >= '%1' and date(crlt_date_payment) <= '%2'";
    
    for ( int p = 0; p < n_prov; p++ )
    {
        QString query =  QString ("select * from crlt"
                                  " where " + cr_date +
                                  " and crlt_prov_code = '%3' %4" )
            .arg( str_from ).arg( str_to).arg(reln_prov->attributeValue(p, "prov_code") )
            .arg( reln_crlt->orderBy() );
    
        QProgressDialog controller( QString("Loading records for %1").arg(reln_prov->attributeValue(p, "prov_name")),
                                    "Cancel", 100, this, "progress", TRUE );
        int i = reln_crlt->open( query, &controller );
        controller.cancel();
        if (i < 1)
            continue;
    
  
        active_prov++;
    
            // Setup a form viewer, and prepare the invoice.
        if ( !form_viewer )
            form_viewer = new QmvForm( reln_crlt, this, "form_crlt" );

        if ( !form_viewer->initForm( "list-crlt" ) )
            return(false);

        form_viewer->writeTemplate();       // debugging
        form_viewer->writeData();           // debugging
    
        form_viewer->setPrint( userPreference("PrintProgram"),
                               userPreference("PrinterSelectionOption"),
                               userPreference("PrinterName") );
    
        if ( userPreference("ViewFormFirst").lower() == "yes" )
            form_viewer->viewForm();
        else
            form_viewer->printForm();
    
    }
    
    if ( active_prov < 1 )
    {
        QMessageBox::information(0, "printWorkSummary:Nothing",
                                 tr("There is nothing to print."),
                                 "OK", 0 );
        return(false);
    }
    
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
bool XpracMan::printCreditSummary( const QDateTime &from, const QDateTime &to )
{
        // setup the query period
    QDateTime x_from;
    QDateTime x_to;
    x_from.setDate( QDate::currentDate() );
    int return_status = true;
    static QGuardedPtr<QmvForm> form_viewer;
     
    if ( !from.isNull() )
        x_from = from;
    if ( !to.isNull() )
        x_to = to;
    if ( x_to.isNull() )
        x_to = x_from;
    
        // Maintain a local copy of the prov object
    static QmvSqlClass * reln_prov = 0;
    if ( !reln_prov )
    {
        QmvSqlClass * prov_existing = (QmvSqlClass *) database->dbRelation( "prov" );
        if ( prov_existing )
            reln_prov = new QmvSqlClass( *prov_existing );
        else
            return(false);
    }
    
        // Maintain a local copy of the crsmobject
    static QmvSqlClass * reln_crsm = 0;
    if ( !reln_crsm )
    {
        QmvSqlClass * crsm_existing = (QmvSqlClass *) database->dbRelation( "crsm" );
        if ( crsm_existing )
            reln_crsm = new QmvSqlClass( *crsm_existing );
        else
            return(false);
    }
    
    QString str_from = QString("%1-%2-%3")
        .arg(x_from.date().day())
        .arg(x_from.date().month())
        .arg(x_from.date().year());
    QString str_to = QString("%1-%2-%3")
        .arg(x_to.date().day())
        .arg(x_to.date().month())
        .arg(x_to.date().year());

        // get a list of the represented providers, to enable
        // separate reports
    int n_prov = reln_prov->open("select * from prov order by prov_name");
  
    if ( n_prov < 1 )
    {
        QMessageBox::information(0, "printCreditSummary:Nothing",
                                 tr("There are no providers."),
                                 "OK", 0 );
        return(false);
    }

    int active_prov = 0;        // count the providers with activity


        // disable the MD timer (hack) to prevent transaction conflict
    bool hold_timer_exportmd2 = timer_exportmd2;
    setExportMD2( false );

        // Begin the transaction which protects the current creditlist
    sql_exec("begin work");
        // load the credit list with date range
    QString cr_date = "paym_date_entry";
    if (userPreference("ReportByRealDate").lower() == "yes" )
        cr_date = "cred__timestamp";
    sql_exec( QString("select crsm_load('%1', '%2', '%3')").arg( str_from ).arg( str_to).arg(cr_date) );

        // work through the provider list
    for ( int p = 0; p < n_prov; p++ )
    {

        QString query =  QString ("select * from crsm"
                                  " where crsm_prov_code = '%1'"
                                  " order by crsm_prov_code, crsm_type, crsm_tdtp_code;")
            .arg(reln_prov->attributeValue(p, "prov_code") );

        QProgressDialog controller( QString("Loading records for %1")
                                    .arg(reln_prov->attributeValue(p, "prov_name")),
                                    "Cancel", 100, this, "progress", TRUE );
        int count = reln_crsm->open( query, &controller );
        controller.cancel();
        if (count < 1)
            continue;
    
        active_prov++;
        
            // Setup a form viewer, and prepare the report.
        if ( !form_viewer )
            form_viewer = new QmvForm( reln_crsm, this, "form_crsm" );

        if ( !form_viewer->initForm( "list-crsm" ) )
        {
            return_status = false;
            break;
        }

        form_viewer->setPrint( userPreference("PrintProgram"),
                               userPreference("PrinterSelectionOption"),
                               userPreference("PrinterName") );
    
        if ( userPreference("ViewFormFirst").lower() == "yes" )
            form_viewer->viewForm();
        else
            form_viewer->printForm();
    }

    sql_exec("abort work");
    setExportMD2( hold_timer_exportmd2 ); // reinstate
  
    if ( active_prov < 1 )
    {
        QMessageBox::information(0, "printCreditSummary:Nothing",
                                 tr("There is nothing to print."),
                                 "OK", 0 );
        return(false);
    }
    return return_status;
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
bool XpracMan::printWorkCreditList( const QDateTime &from, const QDateTime &to )
{
        // setup the query period
    QDateTime x_from;
    QDateTime x_to;
    x_from.setDate( QDate::currentDate() );
    static QGuardedPtr<QmvForm> form_viewer;
     
    if ( !from.isNull() )
        x_from = from;
    if ( !to.isNull() )
        x_to = to;
    if ( x_to.isNull() )
        x_to = x_from;
    
        // Maintain a local copy of the prov object
    static QmvSqlClass * reln_prov = 0;
    if ( !reln_prov )
    {
        QmvSqlClass * prov_existing = (QmvSqlClass *) database->dbRelation( "prov" );
        if ( prov_existing )
            reln_prov = new QmvSqlClass( *prov_existing );
        else
            return(false);
    }
        // Maintain a local copy of the scltobject
    static QmvSqlClass * reln_sclt = 0;
    if ( !reln_sclt )
    {
        QmvSqlClass * sclt_existing = (QmvSqlClass *) database->dbRelation( "sclt" );
        if ( sclt_existing )
            reln_sclt = new QmvSqlClass( *sclt_existing );
        else
            return(false);
    }
    
    QString str_from = QString("%1-%2-%3")
        .arg(x_from.date().day())
        .arg(x_from.date().month())
        .arg(x_from.date().year());
    QString str_to = QString("%1-%2-%3")
        .arg(x_to.date().day())
        .arg(x_to.date().month())
        .arg(x_to.date().year());

        // get a list of the represented providers, to enable
        // separate reports
    int n_prov = reln_prov->open("select * from prov order by prov_name");
  
    if ( n_prov < 1 )
    {
        QMessageBox::information(0, "printWorkSummary:Nothing",
                                 tr("There are no providers."),
                                 "OK", 0 );
        return(false);
    }
    
        // disable the MD timer (hack) to prevent transaction conflict
    bool hold_timer_exportmd2 = timer_exportmd2;
    setExportMD2( false );
    
        // Load SCLT with the target list of transactions.
    
        // Begin the transaction which protects the current creditlist
    sql_exec("begin work");
    sql_exec("delete from sclt");
    const char * debit_target = "sclt_date, sclt_patn_name, sclt_dc, sclt_prov_code, sclt_prov_name, sclt_type, "
        "sclt_desc, sclt_invc__sequence, sclt_debit, sclt_gst_debit, sclt_total_amount, sclt__timestamp";
        
    const char * credit_target = "sclt_date, sclt_patn_name, sclt_dc, sclt_prov_code, sclt_prov_name, sclt_type, "
        "sclt_desc, sclt_invc__sequence, sclt_credit, sclt_gst_credit, sclt_total_amount, sclt__timestamp";

    const char * debit_source = "svlt_date_service, svlt_patn_name, 'D', svlt_prov_code, svlt_prov_name, svlt_serv_code, "
        "svlt_desc, svlt_invc__sequence, svlt_amount, svlt_gst_amount, svlt_total_amount, svlt__timestamp";

    const char * credit_source = "crlt_date_credit, crlt_patn_name, 'C', crlt_prov_code, crlt_prov_name, crlt_tdtp_code, "
        "crlt_drawer, crlt_invc__sequence, crlt_cred_amount, crlt_cred_gst_amount, -crlt_total_amount, crlt__timestamp";

    QString sv_date, cr_date;
    if (userPreference("ReportByRealDate").lower() == "yes" ) {
        sv_date = "date(svlt__timestamp) >= '%3' and date(svlt__timestamp) <= '%4'";
        cr_date = "date(crlt__timestamp) >= '%3' and date(crlt__timestamp) <= '%4'";
    } else {
        sv_date = "date(svlt_date_service) >= '%3' and date(svlt_date_service) <= '%4'";
        cr_date = "date(crlt_date_payment) >= '%3' and date(crlt_date_payment) <= '%4'";
    }
    
    QString query;
    
        // Debits
    statusBar()->message( "Loading debits ..." );
    QApplication::setOverrideCursor( Qt::waitCursor );
    
    query =  QString ("insert into sclt(%1)"
                      " select %2"
                      " from svlt"
                      " where " + sv_date)
        .arg(debit_target).arg(debit_source)
        .arg( str_from ).arg( str_to);

    sql_exec( query);

        // Credits
    statusBar()->message( "Loading credits ..." );
    query =  QString ("insert into sclt(%1)"
                      " select %2"
                      " from crlt"
                      " where " + cr_date)
        .arg(credit_target).arg(credit_source)
        .arg( str_from ).arg( str_to);

    sql_exec( query);
    
    QApplication::restoreOverrideCursor();


    
    int active_prov = 0;        // count the providers with activity

    for ( int p = 0; p < n_prov; p++ )
    {
        query =  QString ("select * from sclt"
                          " where sclt_prov_code = '%1' %2" )
            .arg(reln_prov->attributeValue(p, "prov_code") )
            .arg( reln_sclt->orderBy() );
        QProgressDialog controller( QString("Combining debits and credits for %1")
                                    .arg(reln_prov->attributeValue(p, "prov_name")),
                                    "Cancel", 100, this, "progress", TRUE );
        int i = reln_sclt->open( query, &controller );
        controller.cancel();
        if (i < 1)
            continue;
    
  
        active_prov++;
    
            // Setup a form viewer, and prepare the report
        if ( !form_viewer )
            form_viewer = new QmvForm( reln_sclt, this, "form_sclt" );

        if ( !form_viewer->initForm( "list-sclt" ) ) {
            sql_exec("abort work");
            setExportMD2( hold_timer_exportmd2 ); // reinstate
            return(false);
        }
    
        form_viewer->setPrint( userPreference("PrintProgram"),
                               userPreference("PrinterSelectionOption"),
                               userPreference("PrinterName") );
    
        if ( userPreference("ViewFormFirst").lower() == "yes" )
            form_viewer->viewForm();
        else
            form_viewer->printForm();
    
    }
    sql_exec("abort work");
    setExportMD2( hold_timer_exportmd2 ); // reinstate
    
    if ( active_prov < 1 )
    {
        QMessageBox::information(0, "printWorkSummary:Nothing",
                                 tr("There is nothing to print."),
                                 "OK", 0 );
        return(false);
    }
    
}

// --------------------------------------------------------------------------------
bool XpracMan::printDirectedCredits()
{
        // Maintain a local copy of the eftvobject
    static QmvSqlClass * reln_eftv;
    if ( !reln_eftv )
    {
        QmvSqlClass * eftv_existing = (QmvSqlClass *) database->dbRelation( "eftv" );
        if ( eftv_existing )
            reln_eftv = new QmvSqlClass( *eftv_existing );
        else
            return(FALSE);
    }
 
    int i = reln_eftv->open( "select * from eftv"
                             " where eftv_eftr__sequence = (select max(eftr__sequence) from eftr);" );
 
    if ( i < 1 )
    {
        QMessageBox::information(0, "printDirectedCredits:information",
                                 tr("There is nothing to print."),
                                 "OK", 0 );
        return(FALSE);
    }
    
    
        // Setup a form viewer, and prepare the invoice.
    static QGuardedPtr<QmvForm> form_viewer;
    if ( !form_viewer )
        form_viewer = new QmvForm( reln_eftv, this, "form_eftv" );

    if ( !form_viewer->initForm( "directed_report" ) )
        return(FALSE);

    form_viewer->setPrint( userPreference("PrintProgram"),
                           userPreference("PrinterSelectionOption"),
                           userPreference("PrinterName") );
    
    if ( userPreference("ViewEFTFormFirst").lower() == "yes" )
        form_viewer->viewForm();
    else
        form_viewer->printForm();

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracMan::exportMD2()
{
        // this should be active
    if ( !doit_exportmd2 )
        return setExportMD2(doit_exportmd2);

        // Check the preferences.
    QString patients_in = userPreference("XpracMD2PATIENTS_IN");
    if ( patients_in.length() < 1 )
        return setExportMD2(false); // turn the export service off.
    QFile f_export( patients_in );
    bool ok = false;
    int x_timeout = userPreference("XpracMD2TimerSecs").toInt(&ok);
    if (!ok)
        return setExportMD2(false);
    if ( x_timeout < 60 ) // don't swamp the system
        x_timeout = 60;
    int wait_exportmd2 = userPreference("XpracMD2WaitSecs").toInt(&ok);
    if (!ok)
        wait_exportmd2 = x_timeout/2;

        // look for a ticket (use default expiry - 60 seconds)
    const QString ticket = "XpracMD2PATIENTS_IN";
    int status = sql_exec( QString( "insert into mttk(mttk_code, mttk_expires)"
                                    " values('%1', now() + '%2 sec'::interval)" )
                           .arg( ticket )
                           .arg( wait_exportmd2 ));
    if ( status != 1 )
        return;

    QString delete_ticket = QString( "delete from mttk where mttk_code = '%1'" ).arg( ticket );
    
        // is anything pending?
    if ( sql_exec("begin work") != 0 )
    {
        sql_exec( delete_ticket );              // delete tickets
        return;
    }
        
        // - then flag the records
    int num_rows = sql_exec( QString( "update pate set pate__status = 'D' where pate__status = 'N'") );
    
        // If records flagged, then open and lock the export file, or abort
    if ( num_rows < 1 ||
         !f_export.open(IO_Append | IO_WriteOnly) ||
         flock(f_export.handle(), LOCK_EX|LOCK_NB) )
    {
        sql_exec("abort work");                 // release the records 
        sql_exec( QString( delete_ticket ) );   // delete tickets
        return;
    }
    
        // Read then write the records
        // (trailing whitespace is trimmed, so include a terminator)
    num_rows = sql_exec( QString( "select pemd_export || chr(13) as pemd_export from pemd where pemd__status = 'D'") );
    QTextStream ts( &f_export );
    QString eol = "\n";
    for ( int i = 0; i < num_rows; i++ )
        ts << sql_value(i, "pemd_export") << eol;
    

        // Cleanup
       
    chmod( patients_in.latin1(),  S_IRWXU|S_IRWXG );                            // set the mode;
    f_export.close();                                                           // release the lock
    sql_exec( QString( "delete from pate where pate__status = 'D'" ) );         // purge the records
    sql_exec("commit work");
        // uncomment this to release the ticket - otherwise it provides
        // a refractory period.
        // sql_exec( QString( delete_ticket ) );                                       // release the ticket
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracMan::toggleExportMD2()
{
    doit_exportmd2 = !doit_exportmd2;
    setExportMD2( doit_exportmd2 );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracMan::setExportMD2( bool on )
{
    if (!on)
    {
            // turn the service off.
        timer_exportmd2->stop();
        doit_exportmd2 = false;
        popup_preferences->setItemChecked( menu_exportmd2, false );
        return;
    }
    
        // re-check preferences for MD2
        // - start the timer if an interval is specified
    bool ok = false;
    int x_timeout = userPreference("XpracMD2TimerSecs").toInt(&ok);
    if (!ok)
      return( setExportMD2(false) );
    
    if ( x_timeout < 60 ) // don't swamp the system
        x_timeout = 60;
    statusBar()->message( QString( "Exports timer has been set to %1 seconds" ).arg(x_timeout), 2000);
    
    exportMD2();                                            // first event immediately
    timer_exportmd2->start( x_timeout * 1000, false );      // remaining.
    doit_exportmd2 = true;
    popup_preferences->setItemChecked( menu_exportmd2, true );
    popup_preferences->setItemEnabled( menu_exportmd2, true );
}

// --------------------------------------------------------------------------------
//! About XpracMan
/*!
  Information - brief

*/
void XpracMan::about_qmvapplication()
{
    QMessageBox::about( this,"About XPRacMan",
                        QString( "<b>XPracMan</b> is an applicaton "
                                 "for managing patient records.<br>"
                                 "<a href=\"http://blogs.compsoft.com.au/infoblog/\">"
                                 "http://blogs.compsoft.com.au/infoblog/</a>"
                                 "<br><br><br><em>%1</em>"
                                 )
                        .arg(xpracman_version) );
    
} /* about_xpracman */

// --------------------------------------------------------------------------------
//! Help with XpracMan
/*!
  Help - website

*/
void XpracMan::help_qmvapplication()
{
    QString help_command = getenv( "PRAC_HELPCOMMAND" );
    if (help_command.length() < 1) {
        help_command = QString("konqueror http://blogs.compsoft.com.au/infoblog/tutorials/");
    }
    
    system(help_command + "&");
    
} /* about_xpracman */

