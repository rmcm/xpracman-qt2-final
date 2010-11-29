/*
 *     $Id: xpracapptool.cpp 3336 2007-01-28 04:54:30Z source $
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
 * DESCRIPTION: User interface for Appointment Book
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */

#include <qtoolbar.h>
#include <qstatusbar.h>
#include <qftp.h>
#include <qstringlist.h>
#include <qcombobox.h>
#include <qcolordialog.h>
#include <qspinbox.h>
#include <qslider.h>
#include <qsimplerichtext.h>
#include <qlineedit.h>
#include "qmvapplication.h"
#include "xpracapptool.h"
#include "qmvpopup.h"

namespace XpracAppToolIcons
{
#include "left1arrow.xpm"
#include "right1arrow.xpm"
#include "left2arrow.xpm"
#include "right2arrow.xpm"
#include "print.xpm"
#include "days_1.xpm"
#include "days_5.xpm"
#include "days_7.xpm"
#include "find.xpm"
#include "month.xpm"
#include "today.xpm"
#include "agenda.xpm"
#include "zoomin.xpm"
#include "zoomout.xpm"
#include "quit.xpm"
#include "images/xpracman_float.xpm"
} // end XpracAppToolIcons

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char xpracapptool_version[] = "$Id: xpracapptool.cpp 3336 2007-01-28 04:54:30Z source $";
// #define XPRACAPPTOOL_DEBUG
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

//! Constructor for Appointment tool.
/*!
  Create an appointment tool object

  \param appt           The \ref QmvSqlClass object
  \param parent         parent widget
  \param name
  \param wf
*/
// Constructor

XpracAppTool::XpracAppTool( QmvSqlClass * appt, QmvApplication * parent, const char * name )
        :QmvToolWindow( appt, parent, name )
{
    reln_evnv = appt;
    
        // ------------------------------------------------------------
        // Query Objects
        // ------------------------------------------------------------


        // Provider object

    QmvSqlClass * prov_existing = (QmvSqlClass *) reln_evnv->dataBase()->dbRelation( "prov" );
    if ( prov_existing )
        reln_prov = new QmvSqlClass( *prov_existing );
    else
        throw( QmvException( QString( "<h3>XpracAppTool::XpracAppTool</h2>"
                                      "Failed to build PROV object" ),
                             QmvException::Critical) );

        // Location object

    QmvSqlClass * locn_existing = (QmvSqlClass *) reln_evnv->dataBase()->dbRelation( "locn" );
    if ( locn_existing )
        reln_locn = new QmvSqlClass( *locn_existing );
    else
        throw( QmvException( QString( "<h3>XpracAppTool::XpracAppTool</h2>"
                                      "Failed to build LOCN object" ),
                             QmvException::Critical) );

        // Appointment Type

    QmvSqlClass * aptp_existing = (QmvSqlClass *) reln_evnv->dataBase()->dbRelation( "aptp" );
    if ( aptp_existing )
        reln_aptp = new QmvSqlClass( *aptp_existing );
    else
        throw( QmvException( QString( "<h3>XpracAppTool::XpracAppTool</h2>"
                                      "Failed to build APPT object" ),
                             QmvException::Critical) );

        // Appointment Search details.

    QmvSqlClass * aptd_existing = (QmvSqlClass *) reln_evnv->dataBase()->dbRelation( "aptd" );
    if ( aptd_existing )
        reln_aptd = new QmvSqlClass( *aptd_existing );
    else
        throw( QmvException( QString( "<h3>XpracAppTool::XpracAppTool</h2>"
                                      "Failed to build APTP object" ),
                             QmvException::Critical) );

        // Appointment Status Flag.

    QmvSqlClass * apst_existing = (QmvSqlClass *) reln_evnv->dataBase()->dbRelation( "apst" );
    if ( apst_existing )
        reln_apst = new QmvSqlClass( *apst_existing );
    else
        throw( QmvException( QString( "<h3>XpracAppTool::XpracAppTool</h2>"
                                      "Failed to build APST object" ),
                             QmvException::Critical) );

        // Patient Object

    QmvSqlClass * patn_existing = (QmvSqlClass *) reln_evnv->dataBase()->dbRelation( "patn" );
    if ( patn_existing )
        reln_patn = new QmvSqlClass( *patn_existing );
    else
        throw( QmvException( QString( "<h3>XpracAppTool::XpracAppTool</h2>"
                                      "Failed to build PATN object" ),
                             QmvException::Critical) );

        // ------------------------------------------------------------
        // Current filter attribute values
        // <0 = all,  otherwise order on list
        // popup_XXXX_codes is the map from current_X_id to XXXX_code
        // (the db table index)
        // ------------------------------------------------------------
    current_provider_id = -1;
    current_location_id = -1;
    current_appttype_id = -1;
    current_progress_id = -1;

        // Attribute labels
    current_provider_str = "";
    current_location_str = "";
    current_appttype_str = "";
    current_progress_str = "";

    current_patient_str = "";
    current_day = QDate::currentDate();
    dl_search = 0;

        // colour button
    cc_size = QSize( 8, 8 );

        // Popup menus - filter versions
    popup_provider = new QPopupMenu( this, "Provider Menu" );
    popup_provider->setCheckable( TRUE );
    popup_location = new QPopupMenu( this, "Location Menu" );
    popup_location->setCheckable( TRUE );
    popup_appttype = new QPopupMenu( this, "Type Menu" );
    popup_appttype->setCheckable( TRUE );
    popup_progress = new QPopupMenu( this, "Status Menu" );
    popup_progress->setCheckable( TRUE );
        // Popup menus - event versions
    popup_ev_provider = new QPopupMenu( this, "Provider Event Menu" );
    popup_ev_provider->setCheckable( TRUE );
    popup_ev_location = new QPopupMenu( this, "Location Event Menu" );
    popup_ev_location->setCheckable( TRUE );
    popup_ev_appttype = new QPopupMenu( this, "Type Menu" );
    popup_ev_appttype->setCheckable( TRUE );
    popup_ev_progress = new QPopupMenu( this, "Status Menu" );
    popup_ev_progress->setCheckable( TRUE );

        // the mast event menu - add other menus to this
    popup_ev_properties = new QPopupMenu( this, "Event Properties Menu" );

    popup_ev_properties->insertItem( "Provider", popup_ev_provider );
    popup_ev_properties->insertItem( "Location", popup_ev_location );
    popup_ev_properties->insertItem( "Type", popup_ev_appttype );
    popup_ev_properties->insertItem( "Status", popup_ev_progress );
    popup_ev_properties->insertItem( "Properties", -1, -1);


        // Modify navigation toolbuttons ----------------------------
    setNavigationAction( AGNavBack4, "Previous Year", QIconSet(), "Move to the previous year", 0 );
    setNavigationAction( AGNavBack3, "Previous Month", QIconSet(), "Move to the previous month", 0 );
    setNavigationAction( AGNavBack2, "Previous Week", QIconSet(), "Move to the previous week", 0 );
    setNavigationAction( AGNavBack1, "Previous Day", QIconSet(), "Move to the previous day", 0 );
    setNavigationAction( AGNavForward1, "Next Day", QIconSet(), "Move to the next day", 0 );
    setNavigationAction( AGNavForward2, "Next Week", QIconSet(), "Move to the next week", 0 );
    setNavigationAction( AGNavForward3, "Next Month", QIconSet(), "Move to the next month", 0 );
    setNavigationAction( AGNavForward4, "Next Year", QIconSet(), "Move to the next year", 0 );
    setNavigationAction( AGNavDisplay1, "Single Day", QIconSet(XpracAppToolIcons::days_1_xpm),
                         "Display Single day per page", 0 );
    setNavigationAction( AGNavDisplay2, "Five Days", QIconSet(XpracAppToolIcons::days_5_xpm),
                         "Display Five days per page", 0 );
    setNavigationAction( AGNavDisplay3, "Seven Days", QIconSet(XpracAppToolIcons::days_7_xpm),
                         "Display Seven days per page", 0 );

        // setup a calendar for navigation
    dl_datenav = new QDialog( this, "date nav dialog", FALSE);
    dl_datenav->setCaption(tr("Calendar"));
    QVBoxLayout * vl_datenav = new QVBoxLayout( dl_datenav, 1 );
    date_nav = new QmcDateNav( dl_datenav, "Date Navigator" );
    vl_datenav->addWidget(date_nav);
    dl_datenav->setFixedWidth( date_nav->width() );
    dl_datenav->setFixedHeight( date_nav->height() );
    connect( date_nav, SIGNAL( clicked() ), this, SLOT( setDateByCalendar() ) );
    QAccel * ac_datenav = new QAccel( dl_datenav );
    ac_datenav->connectItem( ac_datenav->insertItem( CTRL+Key_W ), dl_datenav, SLOT(close()) );
    ac_datenav->connectItem( ac_datenav->insertItem( Key_Escape ), dl_datenav, SLOT(close()) );
    ac_datenav->connectItem( ac_datenav->insertItem( Key_Home ), this, SLOT(slotToday()) );
    ac_datenav->connectItem( ac_datenav->insertItem( ALT+Key_Left ), this, SLOT(slotBack4()) );
    ac_datenav->connectItem( ac_datenav->insertItem( CTRL+Key_Left ), this, SLOT(slotBack3()) );
    ac_datenav->connectItem( ac_datenav->insertItem( SHIFT+Key_Left ), this, SLOT(slotBack2()) );
    ac_datenav->connectItem( ac_datenav->insertItem( Key_Left ), this, SLOT(slotBack1()) );
    ac_datenav->connectItem( ac_datenav->insertItem( ALT+Key_Right ), this, SLOT(slotForward4()) );
    ac_datenav->connectItem( ac_datenav->insertItem( CTRL+Key_Right ), this, SLOT(slotForward3()) );
    ac_datenav->connectItem( ac_datenav->insertItem( SHIFT+Key_Right ), this, SLOT(slotForward2()) );
    ac_datenav->connectItem( ac_datenav->insertItem( Key_Right ), this, SLOT(slotForward1()) );
    ac_datenav->connectItem( ac_datenav->insertItem( Key_F ), this, SLOT(slotFind()) );
    ac_datenav->connectItem( ac_datenav->insertItem( SHIFT+Key_F ), this, SLOT(slotFind()) );
    setNavigationAction( AGNavHome, "Calendar", QIconSet(XpracAppToolIcons::agenda_xpm),
                         "Navigation Calendar", Key_C );

        // setup a calendar for searching
    dl_search = new QDialog( this, "date search dialog", FALSE);
    dl_search->setCaption(tr("Search"));
    vl_datenav = new QVBoxLayout( dl_search, 1 );
    search_date_nav = new QmcDateNav( dl_search, "Date Search" );
    vl_datenav->addWidget(search_date_nav);
    dl_search->setFixedWidth( search_date_nav->width() );
    dl_search->setFixedHeight( search_date_nav->height() );
    connect( search_date_nav, SIGNAL( clicked() ),
             this, SLOT( nextAvailableAppointment() ) );

    ac_datenav = new QAccel( dl_search );
    ac_datenav->connectItem( ac_datenav->insertItem( CTRL+Key_W ),
                             dl_search, SLOT(close()) );
    ac_datenav->connectItem( ac_datenav->insertItem( Key_Return ),
                             this, SLOT(nextAvailableAppointment()) );
    ac_datenav->connectItem( ac_datenav->insertItem( Key_Backspace ),
                             this, SLOT(previousAvailableAppointment()) );
    ac_datenav->connectItem( ac_datenav->insertItem( Key_Home ),
                             this, SLOT(slotSearchToday()) );
    ac_datenav->connectItem( ac_datenav->insertItem( ALT+Key_Left ), this, SLOT(slotBack4()) );
    ac_datenav->connectItem( ac_datenav->insertItem( CTRL+Key_Left ), this, SLOT(slotBack3()) );
    ac_datenav->connectItem( ac_datenav->insertItem( SHIFT+Key_Left ), this, SLOT(slotBack2()) );
    ac_datenav->connectItem( ac_datenav->insertItem( Key_Left ), this, SLOT(slotBack1()) );
    ac_datenav->connectItem( ac_datenav->insertItem( ALT+Key_Right ), this, SLOT(slotForward4()) );
    ac_datenav->connectItem( ac_datenav->insertItem( CTRL+Key_Right ), this, SLOT(slotForward3()) );
    ac_datenav->connectItem( ac_datenav->insertItem( SHIFT+Key_Right ), this, SLOT(slotForward2()) );
    ac_datenav->connectItem( ac_datenav->insertItem( Key_Right ), this, SLOT(slotForward1()) );
    ac_datenav->connectItem( ac_datenav->insertItem( Key_C ), this, SLOT(slotHome()) );
    ac_datenav->connectItem( ac_datenav->insertItem( SHIFT+Key_C ), this, SLOT(slotHome()) );


        // Change the accel for Find
    setNavigationAction( AGNavFind, "Next Appointment", QIconSet(XpracAppToolIcons::find_xpm),
                         "Search for next matching appointment", Key_F );

        // ------------------------------------------------------------
        // Other toolbar
        // ------------------------------------------------------------
    otherToolBar()->clear();

        // ------------------------------------------------------------
        // Spare appointment
        // ------------------------------------------------------------
    QActionGroup * actiongp_spare = new QActionGroup( this, "actiongp_spare" );
    QAction * ac;

    ac = new QAction( "Spare",
                      stdicons->getIconSet( QmvIcons::BlankPageIcon ),
                      "Spare Appointment",
                      ALT+Key_N,
                      actiongp_spare, "spare_spare", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotAddSpareAppointment() ) );
        actiongp_spare->addTo( otherToolBar() );


        // ------------------------------------------------------------
        // Selectors for provider, location, type and status filters
        // ------------------------------------------------------------

    QActionGroup * actiongp_filter = new QActionGroup( this, "actiongp_filter" );

        // ------------------------------------------------------------
    ac = new QAction( "Provider",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                                 tr("P"), QFont("Times"), QColor( red ), QRect() ),
                      "Provider",
                      ALT+Key_P,
                      actiongp_filter, "provider_filter", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( showProviderMenu() ) );
    connect( popup_provider, SIGNAL( activated( int ) ), this, SLOT( setProvider( int ) ) );
    connect( popup_provider, SIGNAL( aboutToShow() ), this, SLOT( loadProviderMenu() ) );
    loadProviderMenu();


        // ------------------------------------------------------------
    ac = new QAction( "Location",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                                 tr("L"), QFont("Times"), QColor( red ), QRect() ),
                      "Location",
                      ALT+Key_L,
                      actiongp_filter, "location_filter", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( showLocationMenu() ) );
    connect( popup_location, SIGNAL( activated( int ) ), this, SLOT( setLocation( int ) ) );
    connect( popup_location, SIGNAL( aboutToShow() ), this, SLOT( loadLocationMenu() ) );
    loadLocationMenu();


        // ------------------------------------------------------------
    ac = new QAction( "Appointment Type",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                                 tr("T"), QFont("Times"), QColor( red ), QRect() ),
                      "Appointment Type",
                      ALT+Key_T,
                      actiongp_filter, "appttype_filter", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( showAppttypeMenu() ) );
    connect( popup_appttype, SIGNAL( activated( int ) ), this, SLOT( setAppttype( int ) ) );
    connect( popup_appttype, SIGNAL( aboutToShow() ), this, SLOT( loadAppttypeMenu() ) );
    loadAppttypeMenu();


        // ------------------------------------------------------------
    ac = new QAction( "Status",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                                 tr("S"), QFont("Times"), QColor( red ), QRect() ),
                      "Status of Appointment",
                      ALT+Key_S,
                      actiongp_filter, "progress_filter", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( showProgressMenu() ) );
    connect( popup_progress, SIGNAL( activated( int ) ), this, SLOT( setProgress( int ) ) );
    connect( popup_progress, SIGNAL( aboutToShow() ), this, SLOT( loadProgressMenu() ) );
    display_booked = true;
    display_vacant = true;
    loadProgressMenu();

    new QLabel( "Filter by:", otherToolBar(), "filter_label" );
    actiongp_filter->addTo( otherToolBar() );

        // ------------------------------------------------------------
        // Selectors for provider, location, type and status display
        // ------------------------------------------------------------

    QActionGroup * actiongp_display = new QActionGroup( this, "actiongp_display" );
    actiongp_display->setExclusive(TRUE);

        // ------------------------------------------------------------
    ac = new QAction( "Provider",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                                 tr("P"), QFont("Times"), QColor( blue ), QRect() ),
                      "Provider",
                      ALT+SHIFT+Key_P,
                      actiongp_display, "provider_filter", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( showProviderColour() ) );

        // ------------------------------------------------------------
    ac = new QAction( "Location",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                                 tr("L"), QFont("Times"), QColor( blue ), QRect() ),
                      "Location",
                      ALT+SHIFT+Key_L,
                      actiongp_display, "location_filter", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( showLocationColour() ) );
    ac->setOn(TRUE);
    disp_colour_mode = LocationColour;

        // ------------------------------------------------------------
    ac = new QAction( "Appointment Type",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                                 tr("T"), QFont("Times"), QColor( blue ), QRect() ),
                      "Appointment Type",
                      ALT+SHIFT+Key_T,
                      actiongp_display, "appttype_filter", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( showAppttypeColour() ) );

        // ------------------------------------------------------------
    ac = new QAction( "Status",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                                 tr("S"), QFont("Times"), QColor( blue ), QRect() ),
                      "Status of Appointment",
                      ALT+SHIFT+Key_S,
                      actiongp_display, "progress_filter", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( showProgressColour() ) );

    new QLabel( "Display by: ", otherToolBar(), "display_label" );
    actiongp_display->addTo( otherToolBar() );

        // ----------------------------------------
        // Rebuild the general toolbar - remove quit
        // ----------------------------------------
    generalToolBar()->clear();

    QActionGroup * actiongp_gen = new QActionGroup( this, "actiongp_general" );

    ac = new QAction( "Main Window",
                      QPixmap( XpracAppToolIcons::xpracman_float_xpm ),
                      "Main Window",
                      CTRL+Key_O,
                      actiongp_gen, "gen_quit", FALSE );
    connect( ac, SIGNAL( activated() ), parentApp(), SLOT( raise() ) );
    actiongp_gen->addTo( generalToolBar() );



        // ----------------------------------------
        // default - 1 day per page
        // ----------------------------------------
    display_period = DisplayOneDay;

        // -------------------------------------------
        // Mouse Clicks ------------------------------
        // -------------------------------------------

        // notice button clicks on grid
    connect( canvasView(), SIGNAL( clicked( QCanvasItem *, const QPoint &, ButtonState, ButtonState ) ),
             this, SLOT( handleMouseClicks( QCanvasItem *, const QPoint &, ButtonState, ButtonState ) ) );

        //  doubleclick - edit appointment
    connect( canvasView(), SIGNAL( doubleClicked( QCanvasItem *, const QPoint &, ButtonState, ButtonState ) ),
             this, SLOT( dataChangeRequested( QCanvasItem *, const QPoint &, ButtonState, ButtonState ) ) );

        // notice grid movements
    connect( canvasView(), SIGNAL( moved( QCanvasItem *, const QPoint &, const QPoint & ) ),
             this, SLOT( eventLabelMoved( QCanvasItem *, const QPoint &, const QPoint & ) ) );

        // notice resizes
    connect( canvasView(), SIGNAL( resized( QCanvasItem *, const QRect &, const QRect & ) ),
             this, SLOT( eventLabelResized( QCanvasItem *, const QRect &, const QRect & ) ) );

        // --- ACCELERATORS  ---------------------------------------
        // ------------------------------------------------------------

    QAccel * ap_accel = new QAccel( this, "appointments" );
    int a_id;

        // line down
    a_id = ap_accel->insertItem( Key_Down );
    ap_accel->connectItem( a_id, this, SLOT( slotLineDown() ) );
    ap_accel->setWhatsThis( a_id, "Scroll down one line" );
        // line up
    a_id = ap_accel->insertItem( Key_Up );
    ap_accel->connectItem( a_id, this, SLOT( slotLineUp() ) );
    ap_accel->setWhatsThis( a_id, "Scroll up one line" );

        // page down
    a_id = ap_accel->insertItem( Key_Next );
    ap_accel->connectItem( a_id, this, SLOT( slotPageDown() ) );
    ap_accel->setWhatsThis( a_id, "Scroll down one page" );
        // page up
    a_id = ap_accel->insertItem( Key_Prior );
    ap_accel->connectItem( a_id, this, SLOT( slotPageUp() ) );
    ap_accel->setWhatsThis( a_id, "Scroll up one page" );

        // Jump to today
    a_id = ap_accel->insertItem( Key_Home );
    ap_accel->connectItem( a_id, this, SLOT( slotToday() ) );
    ap_accel->setWhatsThis( a_id, "Go to Today" );

        // Add shifted C for calendar
    a_id = ap_accel->insertItem( SHIFT+Key_C );
    ap_accel->connectItem( a_id, this, SLOT( slotHome() ) );
    ap_accel->setWhatsThis( a_id, "Calendar" );

        // Add shifted F for Find
    a_id = ap_accel->insertItem( SHIFT+Key_F );
    ap_accel->connectItem( a_id, this, SLOT( slotFind() ) );
    ap_accel->setWhatsThis( a_id, "Find" );

        // Get default Magnification Parameters
    double mag_min = 1.0;
    double mag_max = 3.0;
    int mag_step = 4;
    QString pref = parentApp()->userPreference("XpracAppTool.magmin");
    if ( !pref.isNull() )
    {
        bool ok;
        double d = pref.toDouble( &ok );
        if ( ok )
            mag_min = d;
    }

    pref = parentApp()->userPreference("XpracAppTool.magmax");
    if ( !pref.isNull() )
    {
        bool ok;
        double d = pref.toDouble( &ok );
        if ( ok )
            mag_max = d;
    }

    pref = parentApp()->userPreference("XpracAppTool.magstep");
    if ( !pref.isNull() )
    {
        bool ok;
        int i = pref.toInt( &ok );
        if ( ok )
            mag_step = i;
    }
    setMagFactor(mag_min, mag_max, mag_step);

        // Get default Magnification
    pref = parentApp()->userPreference("XpracAppTool.magstart");
    if ( !pref.isNull() )
    {
        bool ok;
        int i = pref.toInt( &ok );
        if ( ok )
            setMagCurrent(i);
    }

        // Set colour display mode

    pref = parentApp()->userPreference("XpracAppTool.displaycolours");
    if ( !pref.isNull() )
    {
        bool ok;
        int i = pref.toInt( &ok );
        if ( ok && i >= (int) ProviderColour && i <= (int) ProgressColour)
            disp_colour_mode = (DisplayColourMode) i;
    }

        // prepare for display
    setToolBarsMovable(FALSE);
    init();

    adjustSize();
    updateGeometry();
    setFocus();
        // adjust canvas to the base magnification.
    setCanvasSize(parentApp()->size());
}

// --------------------------------------------------------------------------------
// Destructor
// --------------------------------------------------------------------------------

XpracAppTool::~XpracAppTool()
{
    event_list.setAutoDelete( true );
    event_list.clear();
    search_list.setAutoDelete( true );
    search_list.clear();
    deny_list.setAutoDelete( true );
    deny_list.clear();
    vacant_events.setAutoDelete( true );
    vacant_events.clear();
    base_events.setAutoDelete( true );
    base_events.clear();
    new_events.setAutoDelete( true );
    new_events.clear();

    if ( reln_prov )
        delete reln_prov;
    if ( reln_locn )
        delete reln_locn;
    if ( reln_aptp )
        delete reln_aptp;
    if ( reln_aptd )
        delete reln_aptd;
    if ( reln_apst )
        delete reln_apst;
    if ( reln_patn )
        delete reln_patn;

}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::showProviderMenu()
{
   popup_provider->exec(QCursor::pos());
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::showLocationMenu()
{
   popup_location->exec(QCursor::pos());
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::showAppttypeMenu()
{
   popup_appttype->exec(QCursor::pos());
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::showProgressMenu()
{
   popup_progress->exec(QCursor::pos());
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::showProviderColour()
{
    disp_colour_mode = ProviderColour;
    init();
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::showLocationColour()
{
    disp_colour_mode = LocationColour;
    init();
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::showAppttypeColour()
{
    disp_colour_mode = AppttypeColour;
    init();
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::showProgressColour()
{
    disp_colour_mode = ProgressColour;
    init();
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

QColor XpracAppTool::fixColour( const QString colour_str ) const
{
    QString str = colour_str;
    bool ok;
    QRgb rgb = str.replace( QRegExp("#"), "").toUInt( &ok, 16);
        // convert black to grey
    if ( ok && rgb % 0x1000000 )
        return QColor( rgb );
    else
        return QColor( 190, 190, 190 );
}


// --------------------------------------------------------------------------------
//! Load the provider menu
/*!
  Build the provider list to be used for display filtering.

*/
// --------------------------------------------------------------------------------
void XpracAppTool::loadProviderMenu()
{
    if (  ! popup_provider )
        return;
    reln_prov->open("select * from prov where prov__status !~* 'R' order by prov_code");
    // revert to all providers if none match
    if (reln_prov->count() < 1)
        reln_prov->open("select * from prov order by prov_code");

    popup_provider->clear();
    popup_ev_provider->clear();
    popup_prov_codes.setAutoDelete(TRUE);
    popup_prov_codes.clear();
    popup_prov_index.setAutoDelete(TRUE);
    popup_prov_index.clear();
    prov_code_colours.setAutoDelete(TRUE);
    prov_code_colours.clear();

    QPixmap pm( cc_size );
    for ( int i = 0; i < reln_prov->count(); i++ )
    {
        QString prov_code = reln_prov->attributeValue( i, "prov_code" );
        QString prov_name = reln_prov->attributeValue( i, "prov_name" );
        int prov_sequence = reln_prov->attributeValue( i, "prov__sequence" ).toInt();
        QColor prov_colour = fixColour(reln_prov->attributeValue( i, "prov_colour" ));

            //  fill a pixmap
        pm.fill( prov_colour );

            // insert the menu item (set attribute bit)
        int id = popup_provider->insertItem( pm, prov_name, PC_Provider|i, -1 );
        popup_provider->setWhatsThis( id, reln_prov->attributeValue( i, "prov_code" ) );

            // and now for event popup
        popup_ev_provider->insertItem( pm, prov_name, id, -1 );

        popup_prov_codes.insert( i, new QString( prov_code ) );
        popup_prov_index.insert( prov_code, new int(i) );
        prov_code_colours.insert( prov_code, new QString( prov_colour.name() ) );

        popup_provider->setItemChecked( id, FALSE);
    }

        // default to all providers ( menu id will be negative)
    pm.fill( color0 );
    pm.setMask( QBitmap( pm.size(), TRUE ) );
    int new_id = popup_provider->insertItem( pm, "All Providers", -1, -1);
    if ( current_provider_id < 0 )
        current_provider_id = new_id;

    popup_provider->setItemChecked(PC_Provider|current_provider_id, TRUE);
    QIconSet *is = popup_provider->iconSet(PC_Provider|current_provider_id);
}

// --------------------------------------------------------------------------------
//! Load the location menu
/*!
  Build the location menu to be used in display filtering.

*/
// --------------------------------------------------------------------------------
void XpracAppTool::loadLocationMenu()
{

    if (  ! popup_location )
        return;
    reln_locn->open("select * from locn order by locn_code");
    popup_location->clear();
    popup_ev_location->clear();
    popup_locn_codes.setAutoDelete(TRUE);
    popup_locn_codes.clear();
    popup_locn_index.setAutoDelete(TRUE);
    popup_locn_index.clear();
    locn_code_colours.setAutoDelete(TRUE);
    locn_code_colours.clear();

    QPixmap pm( cc_size );
    for ( int i = 0; i < reln_locn->count(); i++ )
    {
        QString locn_code = reln_locn->attributeValue( i, "locn_code" );
        QString locn_desc = reln_locn->attributeValue( i, "locn_desc" );
        int locn_sequence = reln_locn->attributeValue( i, "locn__sequence" ).toInt();
        QColor locn_colour = fixColour(reln_locn->attributeValue( i, "locn_colour" ));

            //  fill a pixmap
        pm.fill( locn_colour );

            // insert the menu item
        int id = popup_location->insertItem( pm, locn_desc, PC_Location|i, -1 );
        popup_location->setWhatsThis( id, reln_locn->attributeValue( i, "locn_code" ) );

            // and now for event popup
        popup_ev_location->insertItem( pm, locn_desc, id, -1 );

        popup_locn_codes.insert( i, new QString( locn_code ) );
        popup_locn_index.insert( locn_code, new int(i) );
        locn_code_colours.insert( locn_code, new QString( locn_colour.name() ) );

        popup_location->setItemChecked( id, FALSE);
    }

        // default to all locations ( menu id will be negative)
    pm.fill( color0 );
    pm.setMask( QBitmap( pm.size(), TRUE ) );
    int new_id = popup_location->insertItem( pm, "All Locations", -1, -1);
    if ( current_location_id < 0 )
        current_location_id = new_id;

    popup_location->setItemChecked( PC_Location|current_location_id, TRUE);
    QIconSet *is = popup_location->iconSet(PC_Location|current_location_id);
}

// --------------------------------------------------------------------------------
//! Load the appttype menu
/*!
  Build the appttype menu to be used in display filtering.

*/
// --------------------------------------------------------------------------------
void XpracAppTool::loadAppttypeMenu()
{

    if (  ! popup_appttype )
        return;
    reln_aptp->open("select * from aptp order by aptp_code");
    popup_appttype->clear();
    popup_ev_appttype->clear();
    popup_aptp_codes.setAutoDelete(TRUE);
    popup_aptp_codes.clear();
    popup_aptp_index.setAutoDelete(TRUE);
    popup_aptp_index.clear();
    aptp_code_colours.setAutoDelete(TRUE);
    aptp_code_colours.clear();


    QPixmap pm( cc_size );
    for ( int i = 0; i < reln_aptp->count(); i++ )
    {
        QString aptp_code = reln_aptp->attributeValue( i, "aptp_code" );
        QString aptp_desc = reln_aptp->attributeValue( i, "aptp_desc" );
        int aptp_sequence = reln_aptp->attributeValue( i, "aptp__sequence" ).toInt();
        QColor aptp_colour = fixColour(reln_aptp->attributeValue( i, "aptp_colour" ));

            // invent a color and fill a pixmap
        pm.fill( aptp_colour );

            // insert the menu item
        int id = popup_appttype->insertItem( pm, aptp_desc, PC_Appttype|i, -1 );
        popup_appttype->setWhatsThis( id, reln_aptp->attributeValue( i, "aptp_code" ) );

            // and now for event popup
        popup_ev_appttype->insertItem( pm, aptp_desc, id, -1 );

        popup_aptp_codes.insert( i, new QString( aptp_code ) );
        popup_aptp_index.insert( aptp_code, new int(i) );
        aptp_code_colours.insert( aptp_code, new QString( aptp_colour.name() ) );

        popup_appttype->setItemChecked( id, FALSE);
    }

        // default to all appttypes ( menu id will be negative)
    pm.fill( color0 );
    pm.setMask( QBitmap( pm.size(), TRUE ) );
    int new_id = popup_appttype->insertItem( pm, "All Types", -1, -1);
    if ( current_appttype_id < 0 )
        current_appttype_id = new_id;

    popup_appttype->setItemChecked( PC_Appttype|current_appttype_id, TRUE);
    QIconSet *is = popup_appttype->iconSet(PC_Appttype|current_appttype_id);
}

// --------------------------------------------------------------------------------
//! Load the progress menu
/*!
  Build the progress menu to be used in display filtering.

*/
// --------------------------------------------------------------------------------
void XpracAppTool::loadProgressMenu()
{

    if (  ! popup_progress )
        return;
    reln_apst->open( QString("select * from apst %1").arg(reln_apst->orderBy()) );
    popup_progress->clear();
    popup_ev_progress->clear();
    popup_apst_codes.setAutoDelete(TRUE);
    popup_apst_codes.clear();
    popup_apst_index.setAutoDelete(TRUE);
    popup_apst_index.clear();
    apst_code_colours.setAutoDelete(TRUE);
    apst_code_colours.clear();
    popup_progress->insertItem( new QLabel(" Appointments Status", popup_progress), -1, -1 );
    popup_progress->insertSeparator();

    QPixmap pm( cc_size );
    for ( int i = 0; i < reln_apst->count(); i++ )
    {
        QString apst_code = reln_apst->attributeValue( i, "apst_code" );
        QString apst_desc = reln_apst->attributeValue( i, "apst_desc" );
        int apst_sequence = reln_apst->attributeValue( i, "apst__sequence" ).toInt();
        QColor apst_colour = fixColour(reln_apst->attributeValue( i, "apst_colour" ));

            // fill a pixmap with the configured colour
        pm.fill( apst_colour );

            // insert the menu item
        int id = popup_progress->insertItem( pm, apst_desc, PC_Progress|i, -1 );
        popup_progress->setWhatsThis( id, reln_apst->attributeValue( i, "apst_code" ) );

            // and now for event popup
        popup_ev_progress->insertItem( pm, apst_desc, id, -1 );

        popup_apst_codes.insert( i, new QString( apst_code ) );
        popup_apst_index.insert( apst_code, new int(i) );
        apst_code_colours.insert( apst_code, new QString( apst_colour.name() ) );

        popup_progress->setItemChecked( id, FALSE);
    }


        // default to all progresss ( menu id will be negative)
    pm.fill( color0 );
    pm.setMask( QBitmap( pm.size(), TRUE ) );
    all_progress_id = popup_progress->insertItem( pm, "All Status", -1, -1);
    if ( current_progress_id < 0 )
        current_progress_id = all_progress_id;

    popup_progress->setItemChecked( PC_Progress|current_progress_id, TRUE);

    popup_progress->insertSeparator();

        // Then insert options for toggling display of booked and vacant events.
    popup_progress->insertItem( new QLabel(" Include Appointments which are ...", popup_progress), -1, -1 );
    popup_progress->insertSeparator();
    QString str_booked( "Booked" );
    booked_progress_id = popup_progress->insertItem( str_booked, -1, -1 );
    popup_progress->setItemChecked( booked_progress_id, display_booked );
    popup_apst_codes.insert( booked_progress_id, new QString( str_booked ) );
    QString str_vacant( "Vacant" );
    vacant_progress_id = popup_progress->insertItem( str_vacant, -1, -1 );
    popup_progress->setItemChecked( vacant_progress_id, display_vacant );
    popup_apst_codes.insert( vacant_progress_id, new QString( str_vacant ) );

}

// --------------------------------------------------------------------------------
//! Find menu ID by provider code
/*!

Return the \ref QPopupMenu menu ID of the provider code \e prov. Return -1 if not found.

  \param prov   provider code.
  \return       ID, else -1 if not found
*/
// --------------------------------------------------------------------------------
int XpracAppTool::getMenuIDbyProviderCode( const QString & prov ) const
{

    QIntDictIterator<QString> it(popup_prov_codes);
    while( it.current() )
    {
        QString * ptr = it.current();
        if ( ptr && *ptr == prov )
            return it.currentKey();
        ++it;
    }
    return -1;
}

// --------------------------------------------------------------------------------
//! Find menu ID by location code
/*!

Return the \ref QPopupMenu menu ID of the location code \e locn. Return -1 if not found.

  \param locn   location code.
  \return       ID, else -1 if not found
*/
// --------------------------------------------------------------------------------
int XpracAppTool::getMenuIDbyLocationCode( const QString & locn ) const
{

    QIntDictIterator<QString> it(popup_locn_codes);
    while( it.current() )
    {
        QString * ptr = it.current();
        if ( ptr && *ptr == locn )
            return it.currentKey();
        ++it;
    }
    return -1;
}

// --------------------------------------------------------------------------------
//! Find menu ID by appttype code
/*!

Return the \ref QPopupMenu menu ID of the appttype code \e aptp. Return -1 if not found.

  \param aptp   appttype code.
  \return       ID, else -1 if not found
*/
// --------------------------------------------------------------------------------
int XpracAppTool::getMenuIDbyAppttypeCode( const QString & aptp ) const
{

    QIntDictIterator<QString> it(popup_aptp_codes);
    while( it.current() )
    {
        QString * ptr = it.current();
        if ( ptr && *ptr == aptp )
            return it.currentKey();
        ++it;
    }
    return -1;
}

// --------------------------------------------------------------------------------
//! Find menu ID by progress code
/*!

Return the \ref QPopupMenu menu ID of the progress code \e apst. Return -1 if not found.

  \param apst   progress code.
  \return       ID, else -1 if not found
*/
// --------------------------------------------------------------------------------
int XpracAppTool::getMenuIDbyProgressCode( const QString & apst ) const
{

    QIntDictIterator<QString> it(popup_apst_codes);
    while( it.current() )
    {
        QString * ptr = it.current();
        if ( ptr && *ptr == apst )
            return it.currentKey();
        ++it;
    }
    return -1;
}


// --------------------------------------------------------------------------------
//! Create a spare appointment
/*!

*/
// --------------------------------------------------------------------------------
void XpracAppTool::slotAddSpareAppointment()
{
    eventItem * ei = createSpareEvent(current_day);
    dataChangeRequested(ei);
    delete ei;
}

// --------------------------------------------------------------------------------
//! Set the provider in persistent storage
/*!

  \param id     the id of the menu item selected.
*/
// --------------------------------------------------------------------------------
void XpracAppTool::setProvider( int id )
{
    if ( (id^PC_Provider) == current_provider_id )
        return;

        // reset item checks
    for (int i = 0; i < popup_provider->count(); i++ )
        popup_provider->setItemChecked( popup_provider->idAt(i), FALSE);

    QString label = popup_provider->text( id );
    if ( label.length() )
    {
            // strip the attribute bit
        current_provider_id = id^PC_Provider;

        if ( current_provider_id >= 0 )
        {
            reln_prov->open( QString( "select * from prov where prov_code = '%1'" )
                             .arg( *popup_prov_codes[current_provider_id] ) );
            if ( reln_prov->count() != 1 )
            {
                    // back to defaults.
                loadProviderMenu();
                return;
            }
            current_provider_str = reln_prov->attributeValue( 0, "prov_code" );
        } else
            current_provider_str = "";
    }
    popup_provider->setItemChecked( id, TRUE);
    init();
}


// --------------------------------------------------------------------------------
//! Set the location in persistent storage.
/*!

  \param id     the id of the menu item selected.
*/
// --------------------------------------------------------------------------------
void XpracAppTool::setLocation( int id )
{
    if ( (id^PC_Location) == current_location_id )
        return;

        // reset item checks
    for (int i = 0; i < popup_location->count(); i++ )
        popup_location->setItemChecked( popup_location->idAt(i), FALSE);

    QString label = popup_location->text( id );
    if ( label.length() )
    {
        current_location_id = id^PC_Location;
        QIconSet *is = popup_location->iconSet(id);

        if ( current_location_id >= 0 )
        {
            reln_locn->open( QString( "select * from locn where locn_code = '%1'" )
                             .arg( *popup_locn_codes[current_location_id] ) );
            if ( reln_locn->count() != 1 )
            {
                    // back to defaults.
                loadLocationMenu();
                return;
            }
            current_location_str = reln_locn->attributeValue( 0, "locn_code" );
        } else
            current_location_str = "";
    }
    popup_location->setItemChecked( id, TRUE);
    init();
}


// --------------------------------------------------------------------------------
//! Set the appttype in persistent storage.
/*!

  \param id     the id of the menu item selected.
*/
// --------------------------------------------------------------------------------
void XpracAppTool::setAppttype( int id )
{
    if ( (id^PC_Appttype) == current_appttype_id )
        return;

        // reset item checks
    for (int i = 0; i < popup_appttype->count(); i++ )
        popup_appttype->setItemChecked( popup_appttype->idAt(i), FALSE);

    QString label = popup_appttype->text( id );
    if ( label.length() )
    {
        current_appttype_id = id^PC_Appttype;
        QIconSet *is = popup_appttype->iconSet(id);

        if ( current_appttype_id >= 0 )
        {
            reln_aptp->open( QString( "select * from aptp where aptp_code = '%1'" )
                             .arg( *popup_aptp_codes[current_appttype_id] ) );
            if ( reln_aptp->count() != 1 )
            {
                    // back to defaults.
                loadAppttypeMenu();
                return;
            }
            current_appttype_str = reln_aptp->attributeValue( 0, "aptp_code" );
        } else
            current_appttype_str = "";
    }
    popup_appttype->setItemChecked( id, TRUE);
    init();
}


// --------------------------------------------------------------------------------
//! Set the progress in persistent storage.
/*!

  \param id     the id of the menu item selected.
*/
// --------------------------------------------------------------------------------
void XpracAppTool::setProgress( int id )
{
    if ( (id^PC_Progress) == current_progress_id )
        return;

        // reset item checks
    for (int i = 0; i < popup_progress->count(); i++ )
        popup_progress->setItemChecked( popup_progress->idAt(i), FALSE);

        // if no label then ignore
    QString label = popup_progress->text( id );
    if ( !label.length() )
        return;

    if ( id >= 0 )
    {
        current_progress_id = id^PC_Progress;
        reln_apst->open( QString( "select * from apst where apst_code = '%1'" )
                         .arg( *popup_apst_codes[current_progress_id] ) );
        if ( reln_apst->count() != 1 )
        {
                // back to defaults.
            loadProgressMenu();
            return;
        }
        current_progress_str = reln_apst->attributeValue( 0, "apst_code" );
        popup_progress->setItemChecked( id, TRUE);
    } else
    {
        if ( id == all_progress_id )
        {
            current_progress_str = "";
            current_progress_id = id^PC_Progress;
            popup_progress->setItemChecked( id, TRUE);
        } else
            if ( id == booked_progress_id )
                if ( display_booked )
                    display_booked = false;
                else
                    display_booked = true;
            else if ( id == vacant_progress_id )
                if ( display_vacant )
                    display_vacant = false;
                else
                    display_vacant = true;
    }


    init();
}

// --------------------------------------------------------------------------------
//! Create a spare event
/*!

  Create a spare event item

*/
// --------------------------------------------------------------------------------
XpracAppTool::eventItem *  XpracAppTool::createSpareEvent(const QDate day) const
{
        eventItem * ni = new eventItem;
        ni->patient_id = SpareEvent;
        ni->patient_str = "Spare Appointment";
        ni->provider = "-";
        ni->location = "-";
        ni->appttype = "-";
        ni->start = QDateTime( day, QTime(0, 0) );
        ni->duration = day_display.duration;
        ni->disable = 0;
        ni->parallel = 0;
        ni->skip = 0;
        ni->rgb = QColor( Qt::yellow ).rgb();
        ni->active = false;
        ni->progress = "";
        ni->rowid = -1;

        return ni;

}

// --------------------------------------------------------------------------------
//! Load the vacant event set.
/*!

  The vacant event set is maniplated by this widgets controls( eg
  location, date, display, provider, type.

  \param from   start date
  \param to     end date

*/
// --------------------------------------------------------------------------------
XpracAppTool::eventPeriod  XpracAppTool::loadVacantSet( const QDate & from, const QDate & to )
{

        // Load events from the QmvRelationWidget
    vacant_events.setAutoDelete( true );
    vacant_events.clear();
    static eventPeriod ep;
        // inialise to null
    ep.start = QDateTime();
    ep.end = QDateTime();

        // load events from event candidates
    for ( QDate day = from; day <= to; day = day.addDays(1) )
    {
            // load ALL vacant events for the day
        nextEventList( current_appttype_str, day );

            // Transfer vacant events into vacant_events list
        QIntDictIterator<apptCandidate> it( app_list );
        while ( it.current() )
        {
            apptCandidate * ptr = it.current();

            eventItem * ni = new eventItem;
            ni->patient_id = VacantEvent;
            ni->patient_str = ptr->desc;
            ni->provider = ptr->prov;
            ni->location = ptr->locn;
            ni->appttype = ptr->type;
            ni->start = QDateTime( day, QTime( ptr->start/60, ptr->start % 60 ) );
            ni->duration = ptr->duration;
            ni->disable = ptr->disable;
            ni->parallel = ptr->parallel;
            ni->skip = ptr->skip;
            ni->rgb = ptr->rgb;
            ni->active = false;
            ni->progress = "";  // unbooked == vacant
                // rowid is used as index in lists, so create a dummy
                // negative index
            ni->rowid = -1;

            vacant_events.append( ni );

            ++it;
        }
            // add a extra default item, which always is present

        eventItem * ni = createSpareEvent(day);

        vacant_events.append( ni );
    }
    qDebug("vacant_events:: count = %d", vacant_events.count());
    vacant_events.sort();
    return ep;
}

// --------------------------------------------------------------------------------
//! Load the base query set.
/*!

  Load the base query set, which is set from the parent widget -eg it
  may represent a particular patient set.

  \param query  the query which defines the base set.

*/
// --------------------------------------------------------------------------------
XpracAppTool::eventPeriod  XpracAppTool::loadBaseSet( const QString & query )
{
    if ( query.length() )
        setBaseQuery( query );

    reln_evnv->open( getBaseQuery() );

        // Load events from the QmvRelationWidget
    base_events.setAutoDelete( true );
    base_events.clear();
    static eventPeriod ep;
        // inialise to null
    ep.start = QDateTime();
    ep.end = QDateTime();

        // load events from current object
    for ( int i = 0; i < reln_evnv->count(); i++ )
    {
            // skip where time is not allocated
        if ( !reln_evnv->attributeValue( i, "evnv_starttime" ).length() )
            continue;

        eventItem * ni = new eventItem;
        ni->patient_id = reln_evnv->attributeValue( i, "evnv_patn__sequence" ).toInt();
        ni->patient_str = reln_evnv->userLabelValue( i );
        ni->provider = reln_evnv->attributeValue( i, "evnv_prov_code" );
        ni->referrer = reln_evnv->attributeValue( i, "evnv_rfdr_code" );
        ni->location = reln_evnv->attributeValue( i, "evnv_locn_code" );
        ni->appttype = reln_evnv->attributeValue( i, "evnv_aptp_code" );
        ni->progress = reln_evnv->attributeValue( i, "evnv_apst_code" );
        ni->start = reln_evnv->strToDateTime( reln_evnv->attributeValue( i, "evnv_starttime" ) );
        ni->duration = reln_evnv->strToMinutes( reln_evnv->attributeValue( i, "evnv_duration" ) );

            // calculate min and max
        if ( ep.start.isNull() || ni->start < ep.start )
            ep.start = ni->start;
        QDateTime tmp_end = ni->start.addSecs( ni->duration * 60);
        if ( ep.end.isNull() || tmp_end > ep.end )
            ep.end = tmp_end;

        ni->active = true;
        ni->rowid = reln_evnv->attributeValue( i, "evnv__sequence" ).toInt();
        base_events.append( ni );
    }
    qDebug("base_events:: count = %d", base_events.count());
    base_events.sort();
    return ep;
}

// --------------------------------------------------------------------------------
//! Load the general query set.
/*!

  The general query set is maniplated by this widgets controls( eg
  location, date, display, provider.

  \param query  defines the general set.

*/
// --------------------------------------------------------------------------------
XpracAppTool::eventPeriod  XpracAppTool::loadCurrentSet( const QString & query )
{

        // new query ?
    if ( query.length() )
        reln_evnv->open( query );
    else
        reln_evnv->open( "select * from evnv where 1=0" );

        // Load events from the QmvRelationWidget
    event_list.setAutoDelete( true );
    event_list.clear();
    static eventPeriod ep;
        // inialise to null
    ep.start = QDateTime();
    ep.end = QDateTime();

        // load events from current object
    for ( int i = 0; i < reln_evnv->count(); i++ )
    {
            // check for existance in base set - do not repeat
        int id =  reln_evnv->attributeValue( i, "evnv__sequence" ).toInt();
        if ( eventInList( id, &base_events ) )
            continue;
            // skip where time is not allocated
        if ( !reln_evnv->attributeValue( i, "evnv_starttime" ).length() )
            continue;

        eventItem * ni = new eventItem;
        ni->patient_id = reln_evnv->attributeValue( i, "evnv_patn__sequence" ).toInt();
        ni->patient_str = reln_evnv->userLabelValue( i );
        ni->provider = reln_evnv->attributeValue( i, "evnv_prov_code" );
        ni->referrer = reln_evnv->attributeValue( i, "evnv_rfdr_code" );
        ni->location = reln_evnv->attributeValue( i, "evnv_locn_code" );
        ni->appttype = reln_evnv->attributeValue( i, "evnv_aptp_code" );
        ni->progress = reln_evnv->attributeValue( i, "evnv_apst_code" );
        ni->start = reln_evnv->strToDateTime( reln_evnv->attributeValue( i, "evnv_starttime" ) );
        ni->duration = reln_evnv->strToMinutes( reln_evnv->attributeValue( i, "evnv_duration" ) );

            // calculate min and max
        if ( ep.start.isNull() || ni->start < ep.start )
            ep.start = ni->start;
        QDateTime tmp_end = ni->start.addSecs( ni->duration * 60);
        if ( ep.end.isNull() || tmp_end > ep.end )
            ep.end = tmp_end;

        ni->active = true;
        ni->rowid = reln_evnv->attributeValue( i, "evnv__sequence" ).toInt();
        event_list.append( ni );
    }
    qDebug("event_list:: count = %d", event_list.count());
    event_list.sort();
    return ep;
}

// --------------------------------------------------------------------------------
//! Load the new event set.
/*!

  Load the new, unallocated event set, where no starttime has been


*/
// --------------------------------------------------------------------------------
void  XpracAppTool::loadNewSet( )
{

    reln_evnv->open( "select * from evnv where evnv_starttime is null" );

        // Load events from the QmvRelationWidget
    new_events.setAutoDelete( true );
    new_events.clear();
    static eventPeriod ep;
        // inialise to null
    ep.start = QDateTime();
    ep.end = QDateTime();

        // load events from current object
    for ( int i = 0; i < reln_evnv->count(); i++ )
    {
        eventItem * ni = new eventItem;
        ni->patient_id = reln_evnv->attributeValue( i, "evnv_patn__sequence" ).toInt();
        ni->patient_str = reln_evnv->userLabelValue( i );
        ni->provider = reln_evnv->attributeValue( i, "evnv_prov_code" );
        ni->referrer = reln_evnv->attributeValue( i, "evnv_rfdr_code" );
        ni->location = reln_evnv->attributeValue( i, "evnv_locn_code" );
        ni->appttype = reln_evnv->attributeValue( i, "evnv_aptp_code" );
        ni->progress = reln_evnv->attributeValue( i, "evnv_apst_code" );
        ni->start = QDateTime();
        ni->duration = reln_evnv->strToMinutes( reln_evnv->attributeValue( i, "evnv_duration" ) );

        ni->active = true;
        ni->rowid = reln_evnv->attributeValue( i, "evnv__sequence" ).toInt();
        new_events.append( ni );
    }
    qDebug("new_events:: count = %d", new_events.count());
    new_events.sort();
}


// --------------------------------------------------------------------------------
//! Check for existence of row in set
/*!
  Check in \e elist,  a \ref QList of \ref eventItems, for the existence of an item
  with rowid = \e id

  \param id             row id
  \param elist          list of eventItems
  \return               TRUE if found, else FALSE
*/
// --------------------------------------------------------------------------------
const bool XpracAppTool::eventInList( const int id, const XpracEventList * elist ) const
{
    for ( int i = 0; i < elist->count(); i++ )
    {
        eventItem * ei = const_cast<XpracEventList *>(elist)->at(i);
        if ( id == ( ei->rowid ) )
            return true;
    }

    return false;
}


// --------------------------------------------------------------------------------
//! Initialize the display
/*!

  Initialize the display, after loading the general set.

  \param query  defines the general set.

*/
// --------------------------------------------------------------------------------
void XpracAppTool::init( const QString & query )
{
    initDisplayParameters();

    QString real_query = query;
    if ( !real_query.length() )
    {
        real_query = "select * from evnv where "
            + loadPeriodQuery( QDateTime(current_day),
                               QDateTime(current_day.addDays(display_period)).addSecs(-1),
                               "", TRUE);
    }

        // --------------------------------------------
        // Load the event datasets
        // --------------------------------------------
    loadBaseSet();
    loadCurrentSet( real_query );
    loadNewSet();
    loadVacantSet( current_day, current_day.addDays(display_period-1) );

        // --------------------------------------------
        // Calculate display characteristics of datasets
        // --------------------------------------------

    setObjectDisplayWidths();
    
        // --------------------------------------------
        // Clear the background and set default labels
        // --------------------------------------------
    clearCanvasView();
    buildBackgroundPixmap();
    getCanvas()->setBackgroundPixmap ( background );
    setCaption( QString( "Appointments Management - %1, %2" )
                .arg(reln_evnv->dataBase()->dbTitle())
                .arg(current_day.toString() ) );
}

// --------------------------------------------------------------------------------
//! Set the preferred widths of
/*!

*/
// --------------------------------------------------------------------------------
void XpracAppTool::setObjectDisplayWidths()
{
    event_counter.clear();
    event_counter.load( current_day, day_display, event_list );
    event_counter.load( current_day, day_display, vacant_events );
    event_counter.load( current_day, day_display, base_events );
    event_counter.load( current_day, day_display, new_events );

        // set counters in items
    event_counter.setItemCounters( current_day, day_display, &event_list );
 
    int n_templates = event_counter.maxTemplates();
    int n_events    = event_counter.maxEvents();
    

        // adjust template widths first.
    if (n_events == 0)
        day_display.w_template_pctg = 100;
    if (n_templates > 0 ) {
        day_display.w_template = (day_display.width - (day_display.eventpad * display_period))
                                  / ( n_templates * display_period);
        day_display.w_template *= day_display.w_template_pctg;
        day_display.w_template /= 100;
        if (day_display.w_template > day_display.w_template_pref) 
            day_display.w_template = day_display.w_template_pref;
    } else {
        day_display.w_template = day_display.w_template_pref;
    }
    
        // Set blocked to preference (TODO: check if sensible)
    day_display.w_blocked = day_display.w_blocked_pref / display_period;
    
        // Adjust the appointment widths 
    day_display.w_event = 0;
    if ( n_events > 0) {
        int w_template_area = day_display.w_template * n_templates * display_period;
        int w_pad_period = day_display.eventpad * (display_period - 1);
        
        day_display.w_event = (day_display.width - w_template_area) / (n_events * display_period);
            // padding is included in width, but remove it for a safety margin
        day_display.w_event -= (day_display.eventpad / display_period);
    }
    
    qDebug("init:: ==> period=%d templates=%d(%d) events=%d(%d)",
           display_period, event_counter.maxTemplates(), day_display.w_template,
           event_counter.maxEvents(), day_display.w_event );

}
// --------------------------------------------------------------------------------
//!
/*!

*/
// --------------------------------------------------------------------------------
void XpracAppTool::initDisplayParameters()
{
        //configured - get from resource file?
    QString pref;
    bool ok;

        // ----------------------------------------
        // settable in preferences.
        // ----------------------------------------
    day_display.hr_start = 8;
    day_display.hr_end = 20;
    day_display.granularity = 15; //minutes
    day_display.am_pm = false;
    day_display.duration = 15;   //minutes
        // standard object widths
    day_display.w_template_pref = 16;
    day_display.w_template_pctg = 30;
    day_display.w_blocked_pref = 100;

        // default appt length
    pref = parentApp()->userPreference("XpracAppTool.duration");
    if ( !pref.isNull() )
    {
        int i = pref.toInt( &ok );
        if ( ok && i >= 0 )
            day_display.duration = i;
    }
        // am/pm display
    pref = parentApp()->userPreference("XpracAppTool.am_pm");
    if ( !pref.isNull() )
    {
        int i = pref.toInt( &ok );
        if ( ok && i >= 0 )
            day_display.am_pm = true;
    }

        // start of appt day
    pref =  parentApp()->userPreference("XpracAppTool.hr_start");
    if ( !pref.isNull() )
    {
        int i = pref.toInt( &ok );
        if ( ok && i >= 0 && i <= 24 )
            day_display.hr_start = i;
    }

        // end of appt day
    pref =  parentApp()->userPreference("XpracAppTool.hr_end");
    if ( !pref.isNull() )
    {
        int i = pref.toInt( &ok );
        if ( ok && i >= 0 && i <= 24 )
            day_display.hr_end = i;
    }

        // granularity
    pref = parentApp()->userPreference("XpracAppTool.granularity");
    if ( !pref.isNull() )
    {
        int i = pref.toInt( &ok );
        if ( ok && i >= 10 && i <= 60 )
            day_display.granularity = (i/5)*5;  // 5 minute bites
    }

    granularity = day_display.granularity;


        // template prefered width in pixels
    pref = parentApp()->userPreference("XpracAppTool.w_template_pref");
    if ( !pref.isNull() )
    {
        int i = pref.toInt( &ok );
        if ( ok && i >= day_display.w_template_pref && i <= day_display.w_template_pref * 10 )
            day_display.w_template_pref = i;
    }

        // template share of display (percentage - 10-60)
    pref = parentApp()->userPreference("XpracAppTool.w_template_pctg");
    if ( !pref.isNull() )
    {
        int i = pref.toInt( &ok );
        if ( ok && i >= 10 && i <= 60 )
            day_display.w_template_pctg = i;
    }
    
        // Blocked objects - width
    pref = parentApp()->userPreference("XpracAppTool.w_blocked_pref");
    if ( !pref.isNull() )
    {
        int i = pref.toInt( &ok );
        if ( ok && i >= 30 && i <= 200 )
            day_display.w_blocked_pref = i;
    }
    
        // Font for display objects
    QString family = "helvetica";
    int pointsize = 8;
    pref = parentApp()->userPreference("XpracAppTool.label_font_family");
    if ( !pref.isNull() )
    {
        family = pref;
    }
    label_font.setFamily(family);
    
    pref = parentApp()->userPreference("XpracAppTool.label_font_pointsize");
    if ( !pref.isNull() )
    {
        int i = pref.toInt( &ok );
        if ( ok && i >= 5 && i <= 20 )
            pointsize = i;
    }
    label_font.setPointSize(pointsize);
    
        // Include default in provider selections

    def_prov_in_select = QStringList();

    pref = parentApp()->userPreference("XpracAppTool.def_prov_in_select");
    if ( !pref.isNull() )
    {
        def_prov_in_select = QStringList::split( QRegExp( "\\s*,\\s*" ), pref );
    }
    
    day_display.top_margin = 20;
    day_display.bottom_margin = 20;
    day_display.vmargin = 20;
    day_display.hmargin = 40;
    day_display.daypad = 2;
    day_display.eventpad = 4;
    day_display.labmenuwid = 3;

//    day_display.line_pen = QPen( Qt::blue, 2, SolidLine );
    day_display.line_pen = QPen( colorGroup().foreground() );
    if (parentApp()->userPreference("XpracAppTool.shaded").lower() == "yes") {
        day_display.qtr_pattern = QBrush( QColor(lightGray), Dense5Pattern );
    } else {
        day_display.qtr_pattern = QBrush(Qt::NoBrush);
    }

        // calculated
    day_display.width = canvasSize().width() - (day_display.hmargin*2);
    day_display.height = canvasSize().height()
        - day_display.top_margin - day_display.bottom_margin - (day_display.vmargin*2);
    day_display.display.setLeft( day_display.hmargin );
    day_display.display.setTop( day_display.top_margin + day_display.vmargin );
    day_display.display.setHeight( day_display.height );
    day_display.display.setWidth( day_display.width );
    day_display.hr_vstep = day_display.height/(day_display.hr_end - day_display.hr_start);
}

// --------------------------------------------------------------------------------
//!
/*!

*/
// --------------------------------------------------------------------------------
void XpracAppTool::buildBackgroundPixmap( )
{
    background.resize( canvasSize() );
    QSize sz = background.size();

    background.fill( colorGroup().background() );
    QPainter p( &background );
    switch ( display_period )
    {
        case DisplayMonth:
            buildMonthPixmap( p );
            break;
        case DisplaySevenDay:
        case DisplayFiveDay:
        case DisplayOneDay:
        default:
            buildDayGridPixmap( p );
            break;
    }
        // clear the canvas event index
    canvas_items.clear();
    canvas_events.setAutoDelete(FALSE);
    canvas_events.clear();

    if ( display_vacant )
        displayEventsByDay( current_day, p, &vacant_events, QBrush( QColor(gray), SolidPattern ) );

    if ( display_booked )
    {
        displayEventsByDay( current_day, p, &event_list, QBrush( QColor(gray), SolidPattern ) );
        displayEventsByDay( current_day, p, &base_events, QBrush( QColor(red), SolidPattern ) );
        displayEventsByDay( current_day, p, &new_events, QBrush( QColor(yellow), SolidPattern ) );
    }

#ifdef XPRACAPPTOOL_DEBUG
    background.save("bg.xpm","XPM" );
#endif
}


// --------------------------------------------------------------------------------
void XpracAppTool::slotSetBannerText( const QString & text )
{
    qDebug("XpracAppTool::slotSetBannerText: %s", text.latin1() );

    QmvToolWindow::slotSetBannerText( text );

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotBack4()
{
    setCurrentDate( current_day.addDays( -current_day.daysInYear() ) );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotBack3()
{
        // subtract days in previous month
    QDate pm = current_day.addDays( -28 );
    setCurrentDate( current_day.addDays( -pm.daysInMonth() ) );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotBack2()
{
    setCurrentDate( current_day.addDays( -7 ) );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotBack1()
{
    setCurrentDate( current_day.addDays( -1 ) );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotToday()
{
    setCurrentDate( QDate::currentDate() );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::setCurrentDate( const QDate & cdate )
{
    current_day =  cdate;
    date_nav->setDate(current_day);
    search_date_nav->setDate(current_day);
    init();
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::setDateByCalendar()
{
    QList<QDate> dlist = date_nav->selectedDates();
    setCurrentDate( *dlist.first() );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotHome()
{
    dl_search->hide();
    dl_datenav->show();
    dl_datenav->raise();
    dl_datenav->setFocus();
    QCursor::setPos( dl_datenav->pos() + dl_datenav->rect().center() );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotForward1()

{
    setCurrentDate( current_day.addDays( 1 ) );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotForward2()
{
    setCurrentDate( current_day.addDays( 7 ) );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotForward3()
{
        // add days in current month
    setCurrentDate( current_day.addDays( current_day.daysInMonth() ) );
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotForward4()
{
    setCurrentDate( current_day.addDays( current_day.daysInYear() ) );
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotPrint()
{
    slotPrintReport( QString("") );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotSinglePrint(eventItem * ei)
{
    if (!ei) return;

    QString query = QString( "evnv__sequence = %1").arg(ei->rowid);
    slotPrintReport( query );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotPrintReport( const QString & query )
{
        // Select from report options.
        // - look for candidates.
        // - suffices:
        //    _alpha   : alpha-sorted on patient name
    QmvSqlClass * reln_form =  (QmvSqlClass *) reln_evnv->dataBase()->dbRelation("form");
    int form_count = reln_form->open("select * from form "
                                     " where form_classname ~ '^evnv'");
    int form_row = -1;
    if ( form_count < 1 ) {
            // error message
        QMessageBox::warning( 0, "warning",
                              tr("<b>%1:</b><br>"
                                 "There are no configured forms available ...<br>"
                                 " contact your administrator.")
                              .arg( reln_evnv->relationTitle() ),
                              "OK", 0 );
    } else if ( form_count > 1 ) {
        QmvSelect * form_select = new QmvSelect( reln_form, this );
        form_row = form_select->exec();
        delete form_select;
    } else {
        form_row = 0;
    }
    if ( form_row < 0 || form_row >= form_count )
        return;


        // now we should have a row from the forms object

    QString sort = reln_form->attributeValue( form_row, "form_page_attribute" ).simplifyWhiteSpace();
    sort.replace( QRegExp(" *[,:] *"), "," );
    QString realquery = QString( "select * from %1 where %2 %3" )
        .arg(reln_form->attributeValue( form_row, "form_classname" ))
        .arg( (query.length() > 0) ? (QString(" %1 and ").arg(query)) : QString("") )
        .arg(loadPeriodQuery( QDateTime(current_day),
                              QDateTime(current_day.addDays(display_period)).addSecs(-1),
                              sort,
                              TRUE ));

        // load all

    int open_count = reln_evnv->open( realquery );
    if (open_count < 0)
    {
        QString error = reln_evnv->lastError();
        if (error && error.isNull())
        {
            error = "An error occurred while printing";
        }
        QMessageBox::warning( 0, "warning",
                              tr("<b>%1:</b><br>" + error +
                                 "<br> contact your administrator.")
                              .arg( reln_evnv->relationTitle() ),
                              "OK", 0 );
        return;
    }

        // Setup a form viewer, and prepare the report.
    static QGuardedPtr<QmvForm> form_viewer;
    if ( !form_viewer )
        form_viewer = new QmvForm( reln_evnv, this, "events" );

    if ( !form_viewer->initForm(reln_form->attributeValue( form_row, "form_code") ) )
        return;
    form_viewer->setCaption( reln_form->attributeValue( form_row, "form_desc" ) );


    form_viewer->setPrint( parentApp()->userPreference("PrintProgramEvents"),
                           parentApp()->userPreference("PrinterSelectionOptionEvents"),
                           parentApp()->userPreference("PrinterNameEvents") );

        // Set FAX Attributes - create a faxlist
    QStringList faxlist;
    QString fax_sendto;
    fax_sendto = reln_evnv->attributeValue(0, "evnv_locn_fax_number");
    if ( !fax_sendto.isEmpty() )faxlist << QString("%1 (Location: %2)")
                                    .arg(fax_sendto)
                                    .arg(reln_evnv->attributeValue(0, "evnv_locn_desc"));

    form_viewer->setFax( parentApp()->userPreference("FaxProgramName"),
                         parentApp()->userPreference("FaxProgramFormat"),
                         QString("%1/%2")
                         .arg(parentApp()->sessionFolder())
                         .arg(parentApp()->userPreference("FaxFileName")),
                         faxlist );
    
        // Set EMAIL Attributes - create a emaillist
    QStringList emaillist;
    QString email_sendto;
    email_sendto = reln_evnv->attributeValue(0, "evnv_locn_email_address");
    if ( !email_sendto.isEmpty() )emaillist << QString("%1 (Location: %2)")
                                      .arg(email_sendto)
                                      .arg(reln_evnv->attributeValue(0, "evnv_locn_desc"));

    form_viewer->setEmail( parentApp()->userPreference("EmailProgramName"),
                           parentApp()->userPreference("EmailProgramFormat"),
                           QString("%1/%2")
                           .arg(parentApp()->sessionFolder())
                           .arg(parentApp()->userPreference("EmailFileName")),
                           emaillist );

    if ( parentApp()->userPreference("ViewFormFirst").lower() == "yes" )
        form_viewer->viewForm(false);
    else
        form_viewer->printForm(false);

    bool status = form_viewer->printedStatus();
    if ( !status )
    {
        ;
    }

}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotDisplay1()
{
    display_period = DisplayOneDay;
    init( "" );
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotDisplay2()
{
    display_period = DisplayFiveDay;
    init( "" );
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotDisplay3()
{
    display_period = DisplaySevenDay;
    init( "" );
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotShowMonth()
{
    display_period = DisplayMonth;
    init( "" );
}

// --------------------------------------------------------------------------------
QString XpracAppTool::loadPeriodQuery( const QDateTime & from, const QDateTime & to,
                                       const QString & orderby, bool filter )
{
    QString query;
    query = QString( "evnv_starttime >= '%1'::timestamp"
                     " and evnv_starttime <= '%2'::timestamp" )
        .arg( qtDateTimeToPgTimestamp(from) )
        .arg( qtDateTimeToPgTimestamp(to) ) ;

        // add filters
    if ( filter )
    {
        if ( current_provider_str.length() ) {
            QString prov_select = "";
            prov_select.append( QString( " evnv_prov_code = '%1'" ).arg( current_provider_str ) );
                // see if we need to add the default provider
            if ( current_provider_str != "-" && def_prov_in_select.grep(current_provider_str).count() > 0 )
            {
                prov_select.append( QString(" or evnv_prov_code = '-'") );
            }
            query.append( QString(" and (%1)").arg(prov_select));
        }
        if ( current_location_str.length() )
            query.append( QString( " and evnv_locn_code = '%1'" ).arg( current_location_str ) );
        if ( current_appttype_str.length() )
            query.append( QString( " and evnv_aptp_code = '%1'" ).arg( current_appttype_str ) );
        if ( current_progress_str.length() )
            query.append( QString( " and evnv_apst_code = '%1'" ).arg( current_progress_str ) );
    }

        // add order by
    if ( orderby.length() )
        query.append( QString( " order by %1" ).arg( orderby ) );
    else
        query.append( " order by evnv_starttime,evnv_prov_code,evnv_locn_code,evnv_aptp_code,evnv_apst_code");

    return query;

}


// --------------------------------------------------------------------------------
//!
/*!

*/
// --------------------------------------------------------------------------------
void XpracAppTool::buildMonthPixmap( QPainter & p )
{
    p.drawText( 0, 0,
                p.viewport().width(), p.viewport().height(),
                AlignCenter, QString( "Month not implemented" ) );

}

// --------------------------------------------------------------------------------
//!
/*!

*/
// --------------------------------------------------------------------------------
const QColor XpracAppTool::currentProviderColour( const QString & code ) const
{
    QString col;
    QString * x = prov_code_colours.find(code);
    if (x)
        col = QString(*x);

    return fixColour(col);
}

// --------------------------------------------------------------------------------
//!
/*!

*/
// --------------------------------------------------------------------------------
const QColor XpracAppTool::currentLocationColour( const QString & code ) const
{
    QString col;
    QString * x = locn_code_colours.find(code);
    if (x)
        col = QString(*x);

    return fixColour(col);
}

// --------------------------------------------------------------------------------
//!
/*!

*/
// --------------------------------------------------------------------------------
const QColor XpracAppTool::currentAppttypeColour( const QString & code ) const
{
    QString col;
    QString * x = aptp_code_colours.find(code);
    if (x)
        col = QString(*x);

    return fixColour(col);
}

// --------------------------------------------------------------------------------
//!
/*!

*/
// --------------------------------------------------------------------------------
const QColor XpracAppTool::currentProgressColour( const QString & code ) const
{
    QString col;
    QString * x = apst_code_colours.find(code);
    if (x)
        col = QString(*x);

    return fixColour(col);
}

// --------------------------------------------------------------------------------
//! Build the background grid for the current display format
/*!
  Handle the number of days to display.

  \param p      a \ref \QPainter object
*/
// --------------------------------------------------------------------------------
void XpracAppTool::buildDayGridPixmap( QPainter & p )
{

        // calculated
    day_display.width = (p.viewport().width() -
                         (day_display.daypad*(display_period-1)) - day_display.hmargin*2 )/display_period;
    day_display.height = p.viewport().height()
        - day_display.top_margin - day_display.bottom_margin - (day_display.vmargin*2);
    day_display.top  = day_display.top_margin + day_display.vmargin;
    day_display.bottom = day_display.height + day_display.top;
    day_display.hr_vstep = day_display.height/(day_display.hr_end - day_display.hr_start);
    p.setPen( day_display.line_pen );
    day_display.rect_hold = QRect(day_display.hmargin,day_display.bottom,
                                  day_display.width ,day_display.bottom_margin);

        // hour text on left
    QString tdisplay;
    for ( int yhr = 0; yhr < ( day_display.hr_end - day_display.hr_start); yhr++ )
    {
        int x_hrs = yhr+day_display.hr_start;
        int x_lab = day_display.top + yhr*day_display.hr_vstep - 5;

        if ( day_display.am_pm )
            if ( x_hrs < 12 )
                tdisplay = QString( "%1am").arg(x_hrs);
            else if ( x_hrs == 12 )
                tdisplay = QString( "%1pm").arg(x_hrs);
            else
                tdisplay = QString( "%1pm").arg(x_hrs-12);
        else
            tdisplay = QString( "%1:00" ).arg( x_hrs );
        p.drawText( 3,
                    x_lab,
                    day_display.hmargin-5,
                    p.fontMetrics().lineSpacing(),
                    AlignRight|AlignTop,
                    tdisplay );
            // draw 15 minute
        int h_qtr = day_display.hr_vstep / 4;
        for (int m = 1; m < 4; m++)
            p.drawText( 3,
                        x_lab + (m * h_qtr),
                        day_display.hmargin-5,
                        p.fontMetrics().lineSpacing(),
                        AlignRight|AlignTop,
                        QString("%1").arg(m*15) );
    }


        // Build a legend - 6 sections
    QRect rect_legend(day_display.hmargin, 0,
                      p.viewport().width() - (day_display.hmargin*2),
                      day_display.vmargin);

        // Create a pixmap for the legend - the pixmap will later be
        // drawn onto the main painter, p.
    QPixmap pm( rect_legend.size() );
    pm.fill(getTransparentColor());
    QPainter ptmp( &pm );
    ptmp.setFont( QFont("Times", 10) );
    int width_label = rect_legend.width()/6;
    QRect rect_label = pm.rect();
    rect_label.setWidth( width_label );

        // Provider Legend
    ptmp.setPen( QPen( currentProviderColour(current_provider_str), 2, SolidLine ) );
    rect_label.moveBy(0,0);
    ptmp.drawText( rect_label, Qt::AlignCenter,
                   QString("Provider: %1").arg(popup_provider->text( PC_Provider|current_provider_id) ));
        // Location Legend
    ptmp.setPen( QPen( currentLocationColour(current_location_str), 2, SolidLine ) );
    rect_label.moveBy(width_label, 0);
    ptmp.drawText( rect_label, Qt::AlignCenter,
                   QString("Location: %1").arg(popup_location->text( PC_Location|current_location_id) ));
        // Appttype Legend
    ptmp.setPen( QPen( currentAppttypeColour(current_appttype_str), 2, SolidLine ) );
    rect_label.moveBy(width_label, 0);
    ptmp.drawText( rect_label, Qt::AlignCenter,
                   QString("Type: %1").arg(popup_appttype->text( PC_Appttype|current_appttype_id) ));
        // Progress Legend
    ptmp.setPen( QPen( currentProgressColour(current_progress_str), 2, SolidLine ) );
    rect_label.moveBy(width_label, 0);
    ptmp.drawText( rect_label, Qt::AlignCenter,
                   QString("Status: %1").arg(popup_progress->text( PC_Progress|current_progress_id) ));
        // Booked/Vacant legend
    ptmp.setPen( QPen( Qt::black, 2, SolidLine ) );
    rect_label.moveBy(width_label, 0);
    ptmp.drawText( rect_label, Qt::AlignCenter,
                   QString("Show: %1%2%3")
                   .arg( display_booked ? "Booked" : "" )
                   .arg( (display_booked && display_vacant) ? " and " : "" )
                   .arg( display_vacant ? "Vacant" : "" ) );
        // Display-by legend
    ptmp.setPen( QPen( Qt::black, 2, SolidLine ) );
    rect_label.moveBy(width_label, 0);
    QString cmode;
    if ( disp_colour_mode == ProviderColour )
        cmode = "Provider";
    else if ( disp_colour_mode == LocationColour )
        cmode = "Location";
    else if ( disp_colour_mode == AppttypeColour )
        cmode = "Type";
    else if ( disp_colour_mode == ProgressColour )
        cmode = "Status";

    ptmp.drawText( rect_label, Qt::AlignCenter,
                   QString("By: %1").arg( cmode ) );

    p.drawPixmap( rect_legend.topLeft(), pm );

        // Background colour for date labels

    date_heading.init( day_display, display_period, current_day );


    p.fillRect( date_heading,
                QBrush( currentProviderColour(current_provider_str), SolidPattern) );

        // loop over the days to be displayed.
    for ( int grid_day = 0; grid_day < display_period; grid_day++ )
    {
            // ----------------------------------------
            // Date heading
            // ----------------------------------------
        int left = day_display.hmargin + ((day_display.width + day_display.daypad)*grid_day);
        int right = left + day_display.width;
        p.drawText( left, day_display.top_margin,
                    day_display.width,
                    day_display.top_margin,
                    WordBreak|AlignCenter,
                    current_day.addDays(grid_day).toString() );

        QPointArray pa;
        pa.truncate(0);

            // ----------------------------------------
            // hour lines
            // ----------------------------------------

        for ( int yhr = 0; yhr < ( day_display.hr_end - day_display.hr_start); yhr++ )
        {
            int ypos =  day_display.top + yhr*day_display.hr_vstep;
                // horizontal hour lines
            pa.putPoints( pa.size(), 2, left, ypos, right, ypos );

                // qtr_pattern
            if (day_display.qtr_pattern.style() != Qt::NoBrush)
            {
                int qstep = day_display.hr_vstep/4;
                for (int q = 0; q < 4; q+=2) {
                    int qh = ypos + qstep * q;
                        //pa.putPoints( pa.size(), 2, left, qh, right, qh);
                    p.fillRect( QRect(left, qh, right-left, qstep),
                                day_display.qtr_pattern);
                }
            }

        }

            // ----------------------------------------
            // Add borders
            // ----------------------------------------

        pa.putPoints( pa.size(), 2, left, day_display.bottom, right, day_display.bottom );
        pa.putPoints( pa.size(), 2, left, day_display.top,    left,  day_display.bottom );
        pa.putPoints( pa.size(), 2, right,day_display.top,    right, day_display.bottom );

        QPen curr_pen = p.pen();
        p.setPen( QPen( currentProviderColour(current_provider_str), 3, SolidLine ) );
        p.drawLineSegments( pa );
        p.setPen( curr_pen );
    }

        // ----------------------------------------
        // hour text on right
        // ----------------------------------------

    for ( int yhr = 0; yhr < ( day_display.hr_end - day_display.hr_start); yhr++ )
    {
        QString tdisplay;
        int x_hrs = yhr+day_display.hr_start;
        int x_lab = day_display.top + yhr*day_display.hr_vstep - 5;

        if ( day_display.am_pm )
            if ( x_hrs < 12 )
                tdisplay = QString( "%1am").arg(x_hrs);
            else if ( x_hrs == 12 )
                tdisplay = QString( "%1pm").arg(x_hrs);
            else
                tdisplay = QString( "%1pm").arg(x_hrs-12);
        else
            tdisplay = QString( "%1:00" ).arg( x_hrs );
        p.drawText( p.viewport().width() - day_display.hmargin - 3,
                    x_lab,
                    day_display.hmargin-5,
                    p.fontMetrics().lineSpacing(),
                    AlignRight|AlignTop,
                    tdisplay );
            // draw 15 minute
        int h_qtr = day_display.hr_vstep / 4;
        for (int m = 1; m < 4; m++)
            p.drawText( p.viewport().width() - day_display.hmargin - 3,
                        x_lab + (m * h_qtr),
                        day_display.hmargin-5,
                        p.fontMetrics().lineSpacing(),
                        AlignRight|AlignTop,
                        QString("%1").arg(m*15) );
    }

}



// --------------------------------------------------------------------------------
//! Display the current day events
/*!
   in event \ref QList \e el using \ref QBrush \e brush

  \param day            \ref QDate to be displayed.
  \param p              painter
  \param el             Event List
  \param brush          \re QBrush to use
*/
// --------------------------------------------------------------------------------
void XpracAppTool::displayEventsByDay( QDate day, QPainter & p, XpracEventList * el, const QBrush & brush )
{
    if ( el->count() < 1 )
        return;

    slotSetBannerText( QString("%1").arg( day.toString() ) );

    eventItem * ei;

        // prepare the events in the list.
    XpracEventListIterator it(*el);
    for ( ; it.current(); ++it )
    {
        ei = it.current();

            // ignore if not in current display period - except nulls
        if ( !ei->start.date().isNull()
             && ei->start.date() < current_day || ei->start.date() >= current_day.addDays(display_period) )
            continue;

            // ignore where filtered, except if
            //   - a spare appointment
        if ( ei->patient_id != SpareEvent
             && ( ( ei->provider != "-" && current_provider_str.length()
                    && ei->provider != current_provider_str )
                  || ( ei->location != "-" && current_location_str.length()
                       && ei->location != current_location_str )
                  || ( current_appttype_str.length()
                       && ei->appttype != current_appttype_str )
                  || ( current_progress_str.length()
                       && ei->progress != current_progress_str )) )
            continue;

            // build a QRect for this event
        QRect rect = eventRectInDay( ei );
        
            // Calculate a position for the event
            // - the topleft of the returned rect is the first possible (ie leftmost)
            //   candidate position for the event item.
            // - offset from similar time events
        int left_margin = rect.left();
        int right_margin  = left_margin + day_display.width;
        bool overlaid = false;
        int move_by = (10 + day_display.eventpad)/display_period;
            // qDebug("displayEventsByDay:: provider=%s appttype=%s left=%d, right=%d",
            // ei->provider.latin1(), ei->appttype.latin1(), left_margin, right_margin);

        int x_minimum_display_width = 20;       // TODO: make this a preference
        if ( ei->active )
            while ( getCanvas()->collisions( rect ).count() > 0 )
            {
                
                    // qDebug("displayEventsByDay:: pos=%d, label=%s",
                    // rect.left(), ei->patient_str.latin1());
                
                    // over the right border and still colliding
                if ( rect.right() > ( right_margin - move_by ) )
                {
                        // make sure the object is abutting the right margin
                    rect.setRight(right_margin - day_display.eventpad);
                        // if the object is too narrow to shrink,
                        // then give up and mark it as overlaid
                    if (rect.width() < x_minimum_display_width + move_by) {
                            // recheck collisions
                        if (getCanvas()->collisions( rect ).count() > 0 )
                            overlaid = true;
                        break;
                    }
                    rect.setLeft(rect.left() + move_by);
                } else {
                    rect.moveBy( move_by, 0 );
                }
            }

        else
                // Holding tray
            while ( getCanvas()->collisions( rect ).count() > 0
                    && rect.right() <= right_margin )       // TODO: is this the correct "right-margin"??
                rect.moveBy( move_by, 0 );

        rect = rect.normalize();

        QPoint origin = rect.topLeft();
            // reset origin, ready for drawing
        rect.moveTopLeft( QPoint(0,0) );

            // build an event label using a pixmap
        QPixmap pm( rect.size() );
        QPainter pt(&pm);

            // If overlaid then mark the rect
        BrushStyle style = QBrush::SolidPattern;
        QFont font = label_font;
        if ( overlaid )
        {
            style = QBrush::Dense6Pattern;
            font.setWeight( QFont::Bold );
        }

            // fill the demog background
        pt.setPen( QPen( Qt::black, 2, SolidLine ) );
        QBrush brush;
        brush.setStyle(style);

        switch ( disp_colour_mode )
        {
            case ProviderColour:
                brush.setColor(currentProviderColour(ei->provider));
                break;
            case LocationColour:
                brush.setColor(currentLocationColour(ei->location));
                break;
            case AppttypeColour:
                brush.setColor(currentAppttypeColour(ei->appttype));
                break;
            case ProgressColour:
                brush.setColor(currentProgressColour(ei->progress));
                break;
        }
        pt.fillRect(rect, brush);

            // draw the demog label
        pt.setFont( font );
        QString display_str; // = (ei->rowid != VacantEvent || ei->disable != 0) ? ei->patient_str : QString("");
        if ( ei->rowid == VacantEvent )
        {
            if (ei->disable != 0)
            {
                display_str = QString("X<BR>%1").arg(ei->patient_str);
            } else {
                if (ei->parallel > 0 )
                    display_str = QString("<center>%1</center>").arg(ei->parallel+1);
            }
        } else {
            display_str = ei->patient_str;
        }


        QSimpleRichText* doc = new QSimpleRichText( display_str, font );
        doc->adjustSize();
        doc->setWidth( &pt, rect.width() );
        QRect rdoc( 0, 0, doc->widthUsed(), doc->height() );
        doc->draw( &pt, 2, 2, rdoc, colorGroup(), &brush );

            // draw a border around the lot
        int w_border = 2;
            // only draw border if the object is big enough
        if ( rect.width() > (w_border * 3) && rect.height() > (w_border * 3) )
        {
            pt.setBrush( Qt::NoBrush );
            if ( ei->disable )
                    // blocked
                pt.setPen(QPen( Qt::white, 1, SolidLine ) );
            else
                    // Standard
                pt.setPen(QPen( Qt::black, 2, SolidLine ) );
            pt.drawRect( rect );
        }

            // put the prepared pixmap on a canvas object
        QmvCanvasGrid * disp_obj = new QmvCanvasGrid( getCanvas(), rect.size(), getTransparentColor() );
            // use the canvas object as a key in list of displayed events
        canvas_events.insert( disp_obj, ei );
        canvas_items.insert( ei->rowid, disp_obj );

            //disp_obj->setOpaqueFactor( 30 );
        disp_obj->drawPixmap( QPoint(0,0), pm, pm.rect() );

            // Move the object into place
        disp_obj->move( origin.x(), origin.y() );

            // Put blocked times below.
        if ( ei->disable != 0 )
            disp_obj->setZ(100);
        else
            disp_obj->setZ(200 + pm.rect().left() + (overlaid?1:0));

            // Add tooltip
//        QToolTip::add( disp_obj, display_str );
        disp_obj->show();
    }

}

// --------------------------------------------------------------------------------
//! Return a display \ref QRect for the \ref eventItem \e ei
/*!

*/
// --------------------------------------------------------------------------------
QRect XpracAppTool::eventRectInDay( eventItem * ei )
{

        // Determine the final object width
    int app_width = 0;
    if ( ei->patient_id < BookingInProgress )
    {
            // Vacant template events - no assigned patient
        if (ei->disable == 0 )
        {
                // standard template
            app_width = day_display.w_template;
        } else {
                // blocking template
            app_width = day_display.w_blocked;
        }
    } else {
            // Width - standard appointments
            // app_width = day_display.w_event;
            // Width - calculated for this item
        if (ei->n_events > 0) {
            if (ei->n_blocks > 0) {
                app_width = (day_display.width - (day_display.w_blocked * ei->n_blocks))
                    / ei->n_events;
            } else {
                app_width = (day_display.width - (day_display.w_template * ei->n_templates))
                    / ei->n_events;
            }
            qDebug("eventRectInDay::app_width=%d n_templates=%d n_events=%d",
                   app_width, ei->n_templates, ei->n_events);
        } else {
            app_width = day_display.w_event;
            qDebug("eventRectInDay::(std)app_width=%d n_templates=%d n_events=%d",
                   app_width, ei->n_templates, ei->n_events);
        }
    }

        // Build the QRect    
    QRect rect;
    
        // time dimensions - hours into day, and duration in minutes
    double ev_start = static_cast<double> ( abs(ei->start.time().secsTo( QTime(0, 0) ) )/60) / 60;
    double duration = static_cast<double> (ei->duration) / 60;

        // if active, and in time-display range, place it, else put on bottom margin
    if ( ev_start >= day_display.hr_start && ev_start <= day_display.hr_end - duration )
    {
            // on-calendar events
        ei->active = true;
        rect.setTop( static_cast<int> (day_display.top_margin + day_display.vmargin
                                       + (ev_start - day_display.hr_start)*day_display.hr_vstep) );
        int grid_day = current_day.daysTo(ei->start.date());
        rect.setLeft( day_display.hmargin
                      + ((day_display.width + day_display.daypad) * grid_day) );
            // trim a pixel for packing space
        rect.setHeight( static_cast<int> ((day_display.hr_vstep * duration) - day_display.eventpad) );

    } else {
            // in holding pen on bottom margin
        ei->active = false;
        rect.setTop( day_display.rect_hold.top() );
        rect.setLeft( day_display.rect_hold.left() );
            // make larger than usual - uniform size in holding pen
        rect.setHeight( day_display.rect_hold.height() );
    }

    rect.setWidth( app_width - day_display.eventpad );

    return rect.normalize();
}


// --------------------------------------------------------------------------------
//! Handle clicks
/*!
  Decide what to do with mouse clicks

  \param pressed        pointer to canvas object
  \param pt             coords where clicked
  \param button         which button
  \param state          the button state
*/

// --------------------------------------------------------------------------------
void XpracAppTool::handleMouseClicks( QCanvasItem * pressed, const QPoint & pt,
                                      ButtonState button, ButtonState state )
{
        // a canvas object was clicked
    
    if (pressed) {
        popupEventDetails( pressed, pt, button, state );
        return;
    }

        // click on canvas background - check for heading
    QDate date_clicked = date_heading.dateSelected(pt);
        // not clicked on interesting day (ie in multi-day view)
    if (date_clicked.isNull()
        || (date_clicked == current_day && display_period == DisplayOneDay) ) {
        return;
    }

        // expand to single day view on clicked day
    setCurrentDate(date_clicked);
    slotDisplay1();
    
    return;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::popupEventDetails( QCanvasItem * pressed, const QPoint & pt,
                                      ButtonState button, ButtonState state )
{
    if ( !pressed )
        return;
    eventItem * ei = canvas_events.find( pressed );
    if ( !ei )
        return;

    if ( button == Qt::RightButton )
    {
            // if there are stacked events, then first
            // select a target event for the menu
        QCanvasItemList ev_list = pressed->canvas()->collisions(pt);
            // disable this for testing.
        if ( 0 && ev_list.count() > 0 )
        {
            QPopupMenu select_menu;
            int x = select_menu.insertItem( QString("Stacked Appointments"), -1, 0 );
            select_menu.insertSeparator();
            QCanvasItemList::Iterator it;
            QString label;
            double z_max = -1;
            for ( it = ev_list.begin(); it != ev_list.end(); it++ )
            {
                eventItem * ei = canvas_events.find( *it );
                if (!ei)
                    continue;
                double z = (*it)->z() + 1;
                int pos = ev_list.findIndex( *it );
                label = ei->patient_str;
                if ( z > z_max )
                    z_max = z;
                select_menu.insertItem( label, pos, pos  );
            }

                // WIP.
//             for (int i = 0; i < select_menu.count(); i++)
//             {
//                 int j = select_menu.idAt(i);
//                 int k;
//             }


                // run the menu
            int id = select_menu.exec( mapToGlobal( pt ));
                // return if no selection
            if ( id < 0 )
                return;
            if ( pressed != *(ev_list.at(id)) )
            {
                pressed = *(ev_list.at(id));
                pressed->setZ(z_max + 1);
            }
        }

        eventItem * ei = canvas_events.find( pressed );
        if ( ei )
        {
                // A popup menu for multiple, overlaid event objects
            QPopupMenu item_menu;

                // insert the current event
            QString display_str = QString( "%1 (%2, %3)" )
                .arg( ei->patient_str)
                .arg(ei->provider)
                .arg(ei->location);

            item_menu.insertItem( new QLabel(display_str, &item_menu), -1, -1 );
            item_menu.insertSeparator();
            int ie_use = item_menu.insertItem( QString( "Use this appointment" ), -1, -1 );
            int ie_edit = item_menu.insertItem( QString( "Edit this appointment" ), -1, -1 );
            int ie_delete = item_menu.insertItem( QString( "Delete this appointment" ), -1, -1 );
            int ie_set_status = item_menu.insertItem( QString( "Set the Status" ), -1, -1 );
            int ie_print = item_menu.insertItem( QString( "Print this appointment" ), -1, -1 );
            
            if ( ei->rowid < 0 )
            {
                item_menu.setItemEnabled( ie_edit, FALSE );
                item_menu.setItemEnabled( ie_delete, FALSE );
                item_menu.setItemEnabled( ie_set_status, FALSE );
                item_menu.setItemEnabled( ie_print, FALSE );
            } else
                item_menu.setItemEnabled( ie_use, FALSE );

            int ie_properties = item_menu.insertItem( QString( "Properties" ), -1, -1 );
            int id = item_menu.exec( mapToGlobal( pt ));
            if ( id == ie_delete )
            {
                if ( QMessageBox::information( this, "Delete Appointment",
                                               tr("Are you sure that you want to delete this appointment \n\n %1 %2: %3")
                                               .arg(ei->start.isNull() ? QString("TBA") : ei->start.date().toString())
                                               .arg(ei->start.isNull() ? QString("TBA") : ei->start.time().toString())
                                               .arg( ei->patient_str ),
                                               QMessageBox::Yes | QMessageBox::Default,
                                               QMessageBox::Cancel | QMessageBox::Escape) == QMessageBox::Yes )
                {
                    int status = sql_exec( QString( "delete from evnv where evnv__sequence = %1" ).arg(ei->rowid ) );
                    init();

                }
            } else if ( id == ie_properties )
                popupEventDetails( ei, pt );
            else if ( id == ie_edit || id == ie_use )
                dataChangeRequested( pressed, mapToGlobal( pt ), Qt::RightButton, Qt::RightButton );
            else if ( id == ie_set_status )
                setProgressFlag( ei );
            else if ( id == ie_print )
                slotSinglePrint( ei );
        }
    } else if ( button == Qt::LeftButton )
        popupEventDetails( ei, pt );


}
// --------------------------------------------------------------------------------
//! Display appointment details
/*!
  Short lived popup display of appointment details.

  \param ei
  \param pt
*/
// --------------------------------------------------------------------------------
void XpracAppTool::popupEventDetails( const eventItem *ei, const QPoint pt  )
{
    new QmvPopup( QString("<b>%1</b><br>"
                          "Provider: %2<br>"
                          "Referror: %3<br>"
                          "Location: %4<br>"
                          "Type: %5<br>"
                          "Date: %6<br>"
                          "Time: %7<br>"
                          "Duration: %8 minutes<br>"
                          "Status: %9")
                  .arg(ei->patient_str)
                  .arg(ei->provider)
                  .arg(ei->referrer)
                  .arg(ei->location)
                  .arg(ei->appttype)
                  .arg(ei->start.isNull() ? QString("TBA") : ei->start.date().toString())
                  .arg(ei->start.isNull() ? QString("TBA") : ei->start.time().toString())
                  .arg(ei->duration)
                  .arg(ei->progress), mapToGlobal(pt),  this, "event_detail" );
}


// --------------------------------------------------------------------------------
//! Set the Event Progress
/*!
  Set the progress (status) of an event.

*/
// --------------------------------------------------------------------------------
void XpracAppTool::setProgressFlag(eventItem * ei)
{

        // Create a menu with Status options.
    reln_apst->open( QString("select * from apst %1").arg(reln_apst->orderBy()) );

    QPopupMenu popup_setprogress;
    popup_setprogress.insertItem( new QLabel(" Set Appointment Status", &popup_setprogress), -1, -1 );
    popup_setprogress.insertSeparator();
    QIntDict<QString> id_code;
    id_code.setAutoDelete( TRUE );

    QPixmap pm( cc_size );
    for ( int i = 0; i < reln_apst->count(); i++ )
    {
        QString apst_code = reln_apst->attributeValue( i, "apst_code" );
        QString apst_desc = reln_apst->attributeValue( i, "apst_desc" );
        QColor apst_colour = fixColour(reln_apst->attributeValue( i, "apst_colour" ));

            // fill a pixmap with the configured colour
        pm.fill( apst_colour );

            // insert the menu item
        int id = popup_setprogress.insertItem( pm, apst_desc, -1, -1 );
        id_code.insert( id, new QString( apst_code ) );

        if ( ei->progress == apst_code )
            popup_setprogress.setItemChecked( id, TRUE);
        else
            popup_setprogress.setItemChecked( id, FALSE);
    }

    popup_setprogress.setItemChecked( PC_Progress|current_progress_id, TRUE);

        // run the menu
    int id = popup_setprogress.exec( QCursor::pos() );
    QString * code = id_code.find(id);
    if ( code )
    {
        int status = sql_exec( QString("update evnt set evnt_apst_code = '%1'"
                                       " where evnt__sequence = %2" )
                               .arg( *code )
                               .arg( ei->rowid ) );
        emit eventUpdated( ei->rowid );
            // repaint
        init();
    }


}

// --------------------------------------------------------------------------------
//! Slot to receive movement signals from \ref QmvCanvasItem objects
/*!
  Process movement signals from \ref QmvCanvasItem objects.

  - update timestamps of events.
  - recover from illegal movement.

  \param pressed        the QmvCanvasItem object
  \param from           the original mouse position
  \param to             the final mouse position
*/
// --------------------------------------------------------------------------------
void XpracAppTool::eventLabelMoved( QCanvasItem * pressed, const QPoint & from, const QPoint & to )
{
    eventItem * ei = canvas_events.find( pressed );
    QPoint ev_origin = QPoint( static_cast<int> (pressed->x()), static_cast<int> (pressed->y()) );

    QDateTime oldtime = ei->start;

        // if simple click (or slight move)  then show details.
    int moved = abs((from - to).manhattanLength());
    if ( moved < 10 )
    {
            // cleanup if slight move.
        if ( moved > 0 )
        {
            init();
            canvasView()->update();
        }
        return;
    }


    if ( pressed->y() >= day_display.rect_hold.top() )
        ei->start = QDateTime();
    else {
            // evaluate the new position
        QDateTime newts = coordToDateTime( ev_origin );
        if ( !newts.isNull() )
            ei->start = newts;
    }

        // if changed, update the starttime of the event.
    if ( ei->start != oldtime )
    {
        QString newtss;
        if ( ei->start.isNull() )
            newtss = "null";
        else
            newtss = QString("'%1'").arg(qtDateTimeToPgTimestamp(ei->start) );

        sql_exec( QString("update evnt set evnt_starttime = %1::timestamp"
                          " where evnt__sequence = %2" )
                  .arg( newtss )
                  .arg( ei->rowid ) );
        emit eventUpdated( ei->rowid );
    }

    init();
    canvasView()->update();

    return;


}

// --------------------------------------------------------------------------------
//! Handle resize events to change appointment duration.
/*!
  Calculate, validate and store in persistent storage, the appointment duration change signalled.


  \param pressed        The \ref QmvCanvasGrid canvas item which is resized.
  \param from           \re QRect of the original size
  \param to             \ref QRect of the final size.
*/
// --------------------------------------------------------------------------------
void XpracAppTool::eventLabelResized( QCanvasItem * pressed, const QRect & from, const QRect & to )
{
    eventItem * ei = canvas_events.find( pressed );
    int new_duration = (((to.height() * 60 ) / day_display.hr_vstep)/granularity)*granularity;
    int available = (day_display.hr_end * 60 ) - ((ei->start.time().hour()*60) + ei->start.time().minute());

        // keep in display range
    if ( new_duration <= 0 )
        new_duration = granularity;
    if ( new_duration > available )
        new_duration = available;
    sql_exec( QString("update evnt set evnt_duration = '%1 minutes'::interval"
                      " where evnt__sequence = %2" )
              .arg( new_duration )
              .arg( ei->rowid ) );
    emit eventUpdated( ei->rowid );
    init();
}



// --------------------------------------------------------------------------------
//! Convert \ref QPoint to a \ref QDateTime
/*!
  Convert a position on the display calendar to a \ref QDateTime object.

  \param mp
  \return \ref QDateTime object
*/
// --------------------------------------------------------------------------------
const QDateTime XpracAppTool::coordToDateTime( QPoint mp ) const
{
    for ( int grid_day = 0; grid_day < display_period; grid_day++ )
    {
        int left = day_display.hmargin + ((day_display.width + day_display.daypad)*grid_day);
        if ( QRect( left, day_display.top, day_display.width, day_display.height )
             .contains( mp ) )
        {
                // valid calander space
                // time
            double hour = (static_cast<double>(mp.y() - day_display.top) / day_display.hr_vstep )
                + day_display.hr_start;
            int minutes = static_cast<int>(hour * 60);
                // round up minutes to nearest qtr hour
            minutes = (minutes/granularity) * granularity;
            QTime time( minutes / 60, minutes % 60);
            QDate day = current_day.addDays((mp.x() - day_display.hmargin)
                                            / (day_display.width + day_display.daypad) );

            return QDateTime( day, time );
        }
    }

    return QDateTime();
}



// --------------------------------------------------------------------------------
//! Convert \ref QDateTime to PostgreSQL timestamp
/*!

  \param        dt
  \return       PostgreSQL timestamp
*/
// --------------------------------------------------------------------------------
const QString XpracAppTool::qtDateTimeToPgTimestamp( const QDateTime & dt ) const
{
    if ( dt.isNull() )
        return QString("");

    return( QString( "%1-%2-%3 %4:%5" )
            .arg( dt.date().day() )
            .arg( dt.date().month() )
            .arg( dt.date().year())
            .arg( dt.time().hour() )
            .arg( dt.time().minute() ) );
}

 // --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotFind()
{
    dl_datenav->hide();
    dl_search->show();
    dl_search->raise();
    dl_search->setFocus();
    QCursor::setPos( dl_search->pos() + dl_search->rect().center() );

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::slotSearchToday()
{
    if ( !search_date_nav )
        return;
    current_day = QDate::currentDate();
    search_date_nav->setDate(current_day);
    init();
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::nextAvailableAppointment()
{
    scanForAppointment(1);
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::previousAvailableAppointment()
{
    scanForAppointment(-1);
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::scanForAppointment( int step )
{
    int limit = 100;  // arbitary for now
        // make sure there is NO status filtering.
    setProgress(all_progress_id);

    QDate sd = current_day;
    QList<QDate> dlist = search_date_nav->selectedDates();
    if (dlist.count() == 1)
        sd = *(dlist.first());

    search_date_nav->setDate(sd);
    while ( limit > 0 )
    {
            // start on next/previous day
        sd = sd.addDays(step);

            // Set the display on search panel
        search_date_nav->setDate(sd);

            // load event list for set day
        nextEventList( current_appttype_str, sd );

            // scan for enabled appointments
        QIntDictIterator<apptCandidate> it( app_list );
        int enabled_events = 0;
        while (it.current() )
        {
            apptCandidate * ptr = it.current();
            if (ptr->disable == 0)
            {
                enabled_events++;
                break;
            }
            ++it;
        }
            // found enabled appointment(s)
        if ( enabled_events > 0 )
            break;
        limit--;
    }

    if ( limit > 0 )
        setCurrentDate(sd);
    else
        QMessageBox::information( 0, "No Appointments Found",
                                  tr("No free appointments were located before:<b>"
                                     "%1").arg(sd.toString() ),
                                  "OK", 0 );
}



// --------------------------------------------------------------------------------
//! Load the appttype template
/*!
  Load the appointment-type template specifications into two lists;
  - the search list containing defined appointments (search_list)
  - the deny list containing banned appointments (deny_list)

  \param type   the (single) appointment type for the template lists
  \param force  TRUE=force reload of lists FALSE=use cached lists (default)

*/
// --------------------------------------------------------------------------------
void XpracAppTool::loadAppttypeTemplate( const QString & type, bool force )
{
        // cached?
    apptType * cap = appt_type_list.find(type);
    if ( type.length() && cap && !force )
        return;

        // reset
    search_list.setAutoDelete( true );
    search_list.clear();
    deny_list.setAutoDelete( true );
    deny_list.clear();

    appt_type_list.setAutoDelete(true );
    if ( cap )
        appt_type_list.remove(type);
    else
        appt_type_list.clear();


        // TODO - these should come from somewhere standard
    QStringList full_provlist;
    QStringList full_locnlist;
    for ( int i = 0; i < popup_prov_codes.count(); i++ )
        full_provlist += *(popup_prov_codes[i]);
    for ( int i = 0; i < popup_locn_codes.count(); i++ )
        full_locnlist += *(popup_locn_codes[i]);

        // ----------------------------------------
        // read in the header first
        // ----------------------------------------

    QString select = "select * from aptp";
    if ( type.length() )
        select += QString(" where aptp_code = '%1'").arg(type);

    reln_aptp->open( select );
    for ( int i = 0; i < reln_aptp->count(); i++ )
    {
        bool ok;

        apptType * appt_type_search = new apptType;
        appt_type_search->aptp_code = reln_aptp->attributeValue( i, "aptp_code");
        appt_type_search->aptp_desc = reln_aptp->attributeValue( i, "aptp_desc");
        appt_type_search->aptp_colour = reln_aptp->attributeValue( i, "aptp_colour");
        appt_type_search->aptp_duration = reln_aptp->attributeValue( i, "aptp_duration").toInt(&ok);
        if ( !ok )
            appt_type_search->aptp_duration = day_display.granularity;

        appt_type_search->aptp_disable = reln_aptp->attributeValue( i, "aptp_disable").toInt(&ok);
        if ( !ok )
            appt_type_search->aptp_disable = 0;

        appt_type_search->aptp_interval = reln_aptp->attributeValue( i, "aptp_interval").toInt(&ok);
        if ( !ok )
            appt_type_search->aptp_interval = 0;

        appt_type_search->aptp__sequence = reln_aptp->attributeValue( i, "aptp__sequence");

            // add the search item to the list
        appt_type_list.insert(appt_type_search->aptp_code, appt_type_search);

            // ----------------------------------------
            // Load the search conditions
            // ----------------------------------------

        reln_aptd->open( QString("select * from aptd where aptd_aptp_code = '%1'")
                         .arg(appt_type_search->aptp_code));

        for ( int j = 0; j < reln_aptd->count(); j++ )
        {

            searchForEvent * ns = new searchForEvent;
                // load
            bool ok;
                // the aptp attributes
            ns->aptd_code = reln_aptd->attributeValue( j, "aptd_aptp_code" );
            ns->disable = appt_type_search->aptp_disable;
            ns->duration = appt_type_search->aptp_duration;
            ns->interval = appt_type_search->aptp_interval;
                // the descriptive attributes
            ns->aptd_desc = reln_aptd->attributeValue( j, "aptd_desc" );
            ns->aptd_colour = reln_aptd->attributeValue( j, "aptd_colour" );

                // Load the provider bitmap
            ns->ba_prov_code.fill( FALSE, full_provlist.count() );
            ok = loadStringBA( ns->ba_prov_code, reln_aptd->attributeValue( j, "aptd_prov_code" ), full_provlist );
                // Load the location bitmap
            ns->ba_locn_code.fill( FALSE, full_locnlist.count() );
            ok = loadStringBA( ns->ba_locn_code, reln_aptd->attributeValue( j, "aptd_locn_code" ), full_locnlist );

                // not for the int specs
            QString spec;

                // Load the dow bitmap
            spec = reln_aptd->attributeValue( j, "aptd_dayofweek" ).stripWhiteSpace();
            if ( spec.length() )
            {
                ns->ba_dayofweek.fill( FALSE, 7 );
                ok = loadIntBA( ns->ba_dayofweek, spec, 0, 6 );
            } else
                 ns->ba_dayofweek.fill( TRUE, 7 );

                // Load the woy bitmap
            spec = reln_aptd->attributeValue( j, "aptd_weekofyear" ).stripWhiteSpace();
            if ( spec.length() )
            {
                ns->ba_weekofyear.fill( FALSE, 53 );
                ok = loadIntBA( ns->ba_weekofyear, spec, 1, 53 );
            } else
                 ns->ba_weekofyear.fill( TRUE, 53 );

                // Load the dom bitmap
            spec = reln_aptd->attributeValue( j, "aptd_dayofmonth" ).stripWhiteSpace();
            if ( spec.length() )
            {
                ns->ba_dayofmonth.fill( FALSE, 31 );
                ok = loadIntBA( ns->ba_dayofmonth, spec, 1, 31 );
            } else
                 ns->ba_dayofmonth.fill( TRUE, 31 );

                // Load the moy bitmap
            spec = reln_aptd->attributeValue( j, "aptd_monthofyear" ).stripWhiteSpace();
            if ( spec.length() )
            {
                ns->ba_monthofyear.fill( FALSE, 12 );
                ok = loadIntBA( ns->ba_monthofyear, spec, 1, 12 );
            } else
                 ns->ba_monthofyear.fill( TRUE, 12 );

            ns->aptd_year = reln_aptd->attributeValue( j, "aptd_year" ).toInt(&ok);
            if (!ok)
                ns->aptd_year = -1;

                // Parallel
            ns->aptd_parallel = reln_aptd->attributeValue( j, "aptd_parallel" ).toInt(&ok);
            if (!ok)
                ns->aptd_parallel = 0;

                // Skip timeslots
            ns->aptd_skip = reln_aptd->attributeValue( j, "aptd_skip" ).toInt(&ok);
            if (!ok)
                ns->aptd_skip = 0;

                // recurrence
            ns->aptd_start_date = reln_aptd->strToDateTime(reln_aptd->attributeValue( j, "aptd_start_date" )).date();
            ns->aptd_end_date = reln_aptd->strToDateTime(reln_aptd->attributeValue( j, "aptd_end_date" )).date();
            ns->rec_unit = IntervalDay; // default
            ns->rec_value = 1;          // default
            QString rec = reln_aptd->attributeValue( j, "aptd_recurrence" ).stripWhiteSpace();
            if ( rec.length() > 0 )
            {
                rec = rec.simplifyWhiteSpace();
                QStringList rec_list = QStringList::split(" ", rec );
                if ( rec_list.count() == 2 )
                {
                        // parse the value
                    bool ok;
                    int val = rec_list[0].toInt(&ok);
                    if (ok)
                        ns->rec_value = val;
                        // parse the unit
                    QString unit = rec_list[1].lower();
                    if ( unit.find(QRegExp("^mon")) == 0 )
                        ns->rec_unit = IntervalMonth;
                    else if ( unit.find(QRegExp("^week")) == 0 )
                        ns->rec_value *= 7;
                    else if ( unit.find(QRegExp("^fortn")) == 0 )
                        ns->rec_value *= 14;
                }
            }


            ns->aptd_starting = reln_aptd->timeToMinutes(reln_aptd->attributeValue( j, "aptd_starting" ));
            if (ns->aptd_starting < 0 || ns->aptd_starting < day_display.hr_start * 60 )
                ns->aptd_starting = day_display.hr_start * 60;
            ns->aptd_ending = reln_aptd->timeToMinutes(reln_aptd->attributeValue( j, "aptd_ending" ));
            if (ns->aptd_ending < 0 || ns->aptd_ending > day_display.hr_end * 60 )
                ns->aptd_ending = day_display.hr_end * 60;
            ns->aptd__sequence = reln_aptd->attributeValue( j, "aptd__sequence" );

                // insert
            if ( ns->disable == 0 )
                search_list.append( ns );
            else
                deny_list.append(ns);

        }
    }
    qDebug("loadAppttypeTemplate: search_list=%d, deny_list=%d",search_list.count(), deny_list.count());
}


// --------------------------------------------------------------------------------
//! Parse a range string
/*!
  Parse QString \e ref to determine \e from, \e to and \e step ... these variables
  are set and the status of the operation is returned as TRUE/FALSE

  \param rs             A range string (eg 1-9 sets{1,9,1}, 1-6/2 sets {1,6,2})
  \param from           destination for minumum of range
  \param to             destination for maximum of range
  \param step           destination for the range step.
  \return <ReturnValue> TRUE/FALSE
*/
// --------------------------------------------------------------------------------
bool XpracAppTool::parseRangeString( QString & rs, int * from, int * to, int * step )
{
    bool ok;
    int x_from = -1, x_to = -1, x_step = -1;
    int slash = rs.find( "/", 0 );

        // determine the step and remove the step data.
    QString x_rs = rs;
    if ( slash < 0 )
        x_step = 1;
    else {
        x_step = rs.mid( slash+1 ).toInt(&ok);
        if (!ok)
            x_step = 1;
        x_rs = rs.left(slash);
    }

        // analyse the range
    int dash = x_rs.find( "-", 0 );
        // simple value
    if ( dash < 0 )
    {
        x_from = x_rs.toInt(&ok);
        if (!ok)
            return ok;
        x_to = x_from;
    } else {
            // range specified.
        x_from = x_rs.left(dash).toInt(&ok);
        if (!ok)
            return ok;
        x_to = x_rs.mid(dash+1).toInt(&ok);
        if (!ok)
            return ok;
    }

    *from = x_from;
    *to = x_to;
    *step = x_step;

    return ok;


}


// --------------------------------------------------------------------------------
//! Load a bit array with map for an integer list
/*!
  Load a bit array \e ba with the integer list specified in \e spec, limited by \e min
  and \e max. EG. 4-12/3, 1-3,20,21 specifies the list 4,7,10,12,1,2,3,20,21. The list
  is then limited further by \e min and \e max.

  \param ba             destination bitarray
  \param spec           specification list
  \param min            overall minimum
  \param max            overall maximum
  \return <ReturnValue> TRUE/FALSE
*/
// --------------------------------------------------------------------------------
bool XpracAppTool::loadIntBA( QBitArray & ba, const QString & spec, int min, int max )
{
        // form of specifiation ... X[-[Y[/Z]]]
        // where X,Y,Z = [0-6]

    if ( min > max )
        return false;

        // Walk along the token list.
    QStringList speclist = QStringList::split( ",", spec );
    for ( int tok = 0; tok < speclist.count(); tok++ )
    {
            // parse the range
        int from, to, step;
        bool ok = parseRangeString( speclist[tok], &from, &to, &step );
        if (!ok)
            return ok;

        if ( from < min )
            from = min;
        if ( to > max )
            to = max;

            // Set the bits
        for ( int i = from; i <= to; i+=step )
            ba.setBit( i );
    }

    return TRUE;
}

// --------------------------------------------------------------------------------
//!  Load a bit array with map for a string list
/*!
  Load a bitarray \e ba with a string list specified in \e spec (a comma-separated
  list of strings which may be regular expressions (sans commas), and delimited by
  the master string list \e list. The specification may contain a list of regular
  expressions. EG The master list is {John,Betty,Spot}, and the specification is
  Jo.*,.*t.* - so the bitarray is 111

  \param ba     the bitarray
  \param spec   the specifications for the bitarray settings
  \param list   the master list of strings.
  \param cs     case sensitive (default = TRUE)
  \return <ReturnValue>
*/
// --------------------------------------------------------------------------------
bool XpracAppTool::loadStringBA( QBitArray & ba, const QString & spec, const QStringList & list, bool cs )
{
    if ( list.count() < 1 )
        return false;

        // Walk along the token list.
    QStringList speclist = QStringList::split( ",", spec );
        // if nothing specified, assume the whole list will match
    if ( speclist.count() == 0 )
        speclist = list;

        // walk along the token list
    for ( int tok = 0; tok < speclist.count(); tok++ )
    {
        QRegExp re;
        re.setPattern(speclist[tok]);
            // Set the bits if there is a match
        for ( int i = 0; i <= list.count(); i++ )
            if ( re.match( list[i], 0 ) >= 0 )
                ba.setBit( i );

    }

    return TRUE;
}


// --------------------------------------------------------------------------------
//! Test the existance of a \ref QDate in a \ref searchForEvent
/*!
  Test for the existance of \e search_day in \se which is a template for specifying a
  series of events.

  \param se             the event template
  \param search_day     the QDate to be tested.
  \return <ReturnValue> TRUE/FALSE
*/
// --------------------------------------------------------------------------------
bool XpracAppTool::dayMatches( const searchForEvent * se, const QDate & search_day ) const
{
        // DOY is 0-6 - but QDate is 1-7, week
    int sd = search_day.dayOfWeek();
    if ( sd > 6 || sd < 1 ) sd = 0;
    if ( !se->ba_dayofweek.at( sd ) )
        return false;
        // WOY is 0-52 but QDate is
    if ( !se->ba_weekofyear.at(weekOfYear(search_day) ) )
        return false;
        // DOM is 1-31
    if ( !se->ba_dayofmonth.at(search_day.day() - 1 ) )
        return false;
        // MOY is 1-12
    if ( !se->ba_monthofyear.at(search_day.month() - 1 ) )
        return false;
    if ( se->aptd_year > 0 && se->aptd_year != search_day.year() )
        return false;

        // recurrents
        // - start date specified
    if ( !se->aptd_start_date.isNull() )
        {
                // before startdate
            if ( search_day < se->aptd_start_date )
                return false;
                // not on recurrent date
            switch (se->rec_unit)
            {
                case IntervalDay:
                    if ( se->aptd_start_date.daysTo(search_day) % se->rec_value )
                        return false;
                    break;
                case IntervalMonth:
                    if ( (search_day.month() - se->aptd_start_date.month()) % se->rec_value )
                        return false;
                    break;
            }
        }
        // after end date
    if ( !se->aptd_end_date.isNull() && search_day > se->aptd_end_date )
        return false;

    return true;
}

// --------------------------------------------------------------------------------
//! Test if an appointment has been blocked
/*!
  <long-description>

  \param ei
  \return <ReturnValue>
*/
// --------------------------------------------------------------------------------
bool XpracAppTool::eventDenied( const QDate & day, const apptCandidate * ac )
{
    for ( int i = 0; i < deny_list.count(); i++ )
    {
        searchForEvent * ns = deny_list.at(i);

            // matching day
        if ( !dayMatches( ns, day ) )
            continue;

            // matching wildcard provider
        int * w_prov = popup_prov_index.find("-");
        if ( w_prov && !ns->ba_prov_code.testBit(*w_prov) )
        {
                // try matching explicit provider
            int * i_prov = popup_prov_index.find(ac->prov);
            if ( i_prov && !ns->ba_prov_code.testBit(*i_prov) )
                continue;
        }

            // matching wildcard location
        int * w_locn = popup_locn_index.find("-");
        if ( w_locn && !ns->ba_locn_code.testBit(*w_locn) )
        {
                // try matching explicit location
            int * i_locn = popup_locn_index.find(ac->locn);
            if ( i_locn && !ns->ba_locn_code.testBit(*i_locn) )
                continue;
        }

            // matching time
         if ( ((ac->start + ac->duration) <= ns->aptd_starting)
             || (ac->start >= ns->aptd_ending) )
            continue;

            // matching deny found
        return true;
    }
        // get to here, so nothing denied
    return false;
}

// --------------------------------------------------------------------------------
//! Load the appointment list \e "app_list"
/*!
  Using the appointment templates \ref search_list and \ref deny_list, prepare a
  list of appointment candidates (\ref app_list) for \e type and \e day. Note: the
  list includes denied appointments as immutable items

  \param type   type of appointment
  \param day    day of appointment
  \return       number of matching appointments loaded.
*/
// --------------------------------------------------------------------------------
const int XpracAppTool::nextEventList( const QString & type, const QDate & day )
{
    loadAppttypeTemplate( type, TRUE );         // loads search_list
    app_list.setAutoDelete(TRUE);
    app_list.clear();
    QDate search_day = day;
    QString str_date = search_day.toString();

        // ----------------------------------------
        // Initialise
        // ----------------------------------------

    QStringList full_provlist;
    QStringList full_locnlist;
    for ( int i = 0; i < popup_prov_codes.count(); i++ )
        full_provlist += *(popup_prov_codes[i]);
    for ( int i = 0; i < popup_locn_codes.count(); i++ )
        full_locnlist += *(popup_locn_codes[i]);

    apptCandidate newapp;       // shuttle structure for new appointments
    int counter = 0;            // count of possible appointments

        // ----------------------------------------
        // Load the possible appointments that
        // match the templates - this forms the list
        // of all specified appointments (search_list)
        // ----------------------------------------
    for ( int i = 0; i < search_list.count(); i++ )
    {
        searchForEvent * ns = search_list.at(i);
        if (!ns)
            continue;

            // skip if day does not match current template
        if ( !dayMatches( ns, search_day ) )
            continue;

            // Find the time limits and duration
        int start = ns->aptd_starting;
        if ( start < 0 )
            start = day_display.hr_start * 60;
        int end = ns->aptd_ending;
        if ( end < 0 )
            end = day_display.hr_end * 60;
        int duration = ns->duration;
        if ( duration <= 0 )
            duration = granularity;
        QRgb rgb = QColor(ns->aptd_colour).rgb();

        if ( (start < day_display.hr_start * 60)
             || ( (start+duration) > day_display.hr_end * 60 ) )
            continue;


            // build the list of possibles
            // (applying the provider and location which are set)
        for (int j = start; j < end; j+= duration )
        {
                // skip "skipped slots"
            if ( (j-start) % ( (ns->aptd_skip + 1)*duration) != 0 )
                continue;

            for ( int x_p = 0; x_p < ns->ba_prov_code.size(); x_p++ )
            {
                    // Template does not include this provider
                if ( !ns->ba_prov_code.testBit(x_p) )
                    continue;

                    // Provider is not in filter list
                if ( current_provider_str.length()
                     && (*(popup_prov_codes[x_p]) != current_provider_str )
                     && (*(popup_prov_codes[x_p]) != "-")
                     )
                    continue;

                    // expand locations
                for ( int x_l = 0; x_l < ns->ba_locn_code.size(); x_l++ )
                {
                        // template does not include this location
                    if ( !ns->ba_locn_code.testBit(x_l) )
                        continue;

                        // Location is not in the filter list
                    if ( current_location_str.length()
                         && ( *(popup_locn_codes[x_l]) != current_location_str )
                         && ( *(popup_locn_codes[x_l]) != "-" )
                         )
                        continue;

                    newapp.start = j;
                    newapp.duration = ((j + duration) > end) ? (end - j) : duration;
                    newapp.disable = ns->disable;
                    newapp.parallel = ns->aptd_parallel;
                    newapp.skip = ns->aptd_skip;
                    newapp.prov = *(popup_prov_codes[x_p]);
                    newapp.locn = *(popup_locn_codes[x_l]);
                    newapp.type = ns->aptd_code;
                    newapp.desc = ns->aptd_desc;
                    newapp.rgb = rgb;

                        // check for block on this appointment
                    if ( eventDenied( day, &newapp ) )
                        continue;

                        // Handle parallel bookings
//                    for ( int k = 0; k <= newapp.parallel; k++ ) {

                        // insert the appointment into app_list
                    apptCandidate * ptr = new apptCandidate;
                    ptr->start = newapp.start;
                    ptr->duration = newapp.duration;
                    ptr->disable = newapp.disable;
                    ptr->parallel = newapp.parallel;
                    ptr->skip = newapp.skip;
                    ptr->prov = newapp.prov;
                    ptr->locn = newapp.locn;
                    ptr->type = newapp.type;
                    ptr->desc = newapp.desc;
                    ptr->rgb = newapp.rgb;

                    app_list.insert( counter++, ptr );
//                  }

                }

            }

        }
    }

        // ----------------------------------------------------------------------
        // Load the denied appointments
        // ----------------------------------------------------------------------
    for ( int i = 0; i < deny_list.count(); i++ )
    {
        searchForEvent * ns = deny_list.at(i);
        if (!ns)
            continue;

            // skip if day does not match current template
        if ( !dayMatches( ns, search_day ) )
            continue;

            // Find the time limits and duration
        int start = ns->aptd_starting;
        if ( start < 0 )
            start = day_display.hr_start * 60;
        int end = ns->aptd_ending;
        if ( end < 0 )
            end = day_display.hr_end * 60;
        int duration = ns->duration;
        if ( duration <= 0 )
            duration = granularity;
        QRgb rgb = QColor(ns->aptd_colour).rgb();

            // build the list of possibles
            // (applying the provider and location which are set)
            // Build one single object - ignore duration
            for ( int x_p = 0; x_p < ns->ba_prov_code.size(); x_p++ )
            {
                if ( !ns->ba_prov_code.testBit(x_p) )
                    continue;

                    // Provider is not in filter list
                if ( current_provider_str.length()
                     && (*(popup_prov_codes[x_p]) != current_provider_str )
                     && (*(popup_prov_codes[x_p]) != "-")
                     )
                    continue;

                for ( int x_l = 0; x_l < ns->ba_locn_code.size(); x_l++ )
                {
                    if ( !ns->ba_locn_code.testBit(x_l) )
                        continue;

                        // Location is not in the filter list
                    if ( current_location_str.length()
                         && ( *(popup_locn_codes[x_l]) != current_location_str )
                         && ( *(popup_locn_codes[x_l]) != "-" )
                         )
                        continue;

                    apptCandidate * ptr = new apptCandidate;
                    ptr->start = start;
                    ptr->duration = end - start;
                    ptr->disable = ns->disable;
                    ptr->prov = *(popup_prov_codes[x_p]);
                    ptr->locn = *(popup_locn_codes[x_l]);
                    ptr->type = ns->aptd_code;
                    ptr->desc = ns->aptd_desc;
                    ptr->rgb = rgb;

                    app_list.insert( counter++, ptr );
                }
            }
    }



        // Load existing appointments on this day
    reln_evnv->open( QString("select * from evnv where date(evnv_starttime) = '%1-%2-%3'::timestamp"
                             " order by evnv_starttime").
                     arg( search_day.day() ).arg( search_day.month() ).arg( search_day.year() ));

    QString evnv_prov_code;
    QString evnv_locn_code;
    QDateTime x_starttime;
    int x_duration;

        // ----------------------------------------------------------------------
        // Scan over the booked appointments and remove booked appointments from
        // the list of all possible appointments (search_list)
        // ----------------------------------------------------------------------
    for ( int i = 0; i < reln_evnv->count(); i++ )
    {
        QString x_prov = reln_evnv->attributeValue( i, "evnv_prov_code" );
        QString x_locn = reln_evnv->attributeValue( i, "evnv_locn_code" );
        QString x_aptd = reln_evnv->attributeValue( i, "evnv_aptp_code" );

        QTime x_time = reln_evnv->strToDateTime( reln_evnv->attributeValue( i, "evnv_starttime")).time();
        int x_start = (x_time.hour() * 60 ) + x_time.minute();
        x_duration = reln_evnv->strToMinutes( reln_evnv->attributeValue( i, "evnv_duration" ) );

        // ------------------------------------------------------------------------
            // Remove matching templates::
            //  - non-blocking
            //  - overlap time
            //  - first match provider and location and then general wildcard ("-")
            //  - match type
        // ------------------------------------------------------------------------
        
            // First pass -  match provider and location
            // Second pass - match wildcard provider and location"-"
        QIntDictIterator<apptCandidate> it( app_list );
        QDict<char> removedTemplateItems;
        removedTemplateItems.setAutoDelete( false );
        removedTemplateItems.clear();
        QStringList p1, p2, p3, p4;
        p1.clear();
        p2.clear();
        p3.clear();
        p3.clear();
        p1 << x_prov << x_locn << x_aptd;
        p2 << x_prov << "-"    << x_aptd;
        p3 << "-"    << x_locn << x_aptd;
        p4 << "-"    << "-"    << x_aptd;
        QIntDict<QStringList> x_matchorder;
        x_matchorder.clear();
        x_matchorder.insert(1, &p1);
        x_matchorder.insert(2, &p2);
        x_matchorder.insert(3, &p3);
        x_matchorder.insert(4, &p4);
        
        for ( int j = 1; j <= x_matchorder.count(); j++)
        {    
            // Load the match profile
          QString j_prov = QString( *(x_matchorder[j]->at(0)));
          QString j_locn = QString( *(x_matchorder[j]->at(1)));
          QString j_aptd = QString( *(x_matchorder[j]->at(2)));
          it.toFirst();
          while (it.current() )
          {
            apptCandidate * ptr = it.current();
            //qDebug("DEBUG::nextEventList:: i=%d :: j=%d :: x_prov=%s j_prov=%s ptr_prov=%s, x_locn=%s j_locn=%s ptr_locn=%s x_type=%s, ptr_type=%s",
            //     i, j, x_prov.latin1(), j_prov.latin1(), ptr->prov.latin1(), x_locn.latin1(), j_locn.latin1(), ptr->locn.latin1(), x_aptd.latin1(), ptr->type.latin1());
                // Check for a match
                // TODO: make RegEx comparisons
                
            if ( ptr // is this candidate still in place - it may have been removed.
                 && ptr->disable == 0                                   // not blocked
                 && ( x_start < ( ptr->start + ptr->duration) )         // appt starts before end of template
                 && ((x_start + x_duration) > ptr->start )              // AND ends after start of template
                 && ( ptr->prov == j_prov )                             // provider
                 && ( ptr->locn == j_locn )                             // location
                 && ( ptr->type == j_aptd )                             // type match
                 )
            {
                //qDebug("==> MATCH");
                    // this may be a current multiple template (parallel > 0)
                if (ptr->parallel > 0)
                {
                    //qDebug("==> PARALLEL");
                    (ptr->parallel)--;
                } else {
                    //qDebug("==> DELETE-CANDIDATE");
                        // Time to delete this object. First
                        // prepare a key
                    QString key_remove = QString("%1~%2~%3~%4~%5")
                        .arg(ptr->start)
                        .arg(ptr->duration)
                        .arg(ptr->prov)
                        .arg(ptr->locn)
                        .arg(ptr->type);
                        // if not already removed, then remove it.
                    if ( !removedTemplateItems.find(key_remove) )
                    {
                        //qDebug("==> REMOVE");
                            // a new match, so remove it
                        app_list.remove(it.currentKey());
                        removedTemplateItems.insert(key_remove, "");
                            // iterator is adjusted after removal, don't increment
                        continue;
                    }
                }

            }
                // Not deleted, so try next template item
            ++ it;
          }
        }


    }

    qDebug("nextEventList: type=%s day=%s count=%d", type.latin1(), day.toString().latin1(),app_list.count());

    return app_list.count();

}


// --------------------------------------------------------------------------------
//! Week number in year
/*!
  Return the number of the week in \ref QDate dt

  \param dt             Date
  \return               Day of week
*/
// --------------------------------------------------------------------------------
int XpracAppTool::weekOfYear( const QDate & dt ) const
{
        // days in week 1 of year
    int wkone = 7 - QDate(dt.year(), 1, 1).dayOfWeek();
    int week = ( (dt.dayOfYear() - wkone) / 7 ) + 1;
    return week;
}



// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::dataChangeRequested( QCanvasItem * pressed, const QPoint & pt, ButtonState button, ButtonState state )
{
    eventItem * ei = canvas_events.find( pressed );
    if (!ei || ei->disable != 0)
        return;

        // edit the event
    dataChangeRequested(ei);

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void XpracAppTool::dataChangeRequested( eventItem * ei )
{

    static QmvSqlClass * new_evnv = 0;
    QmvRowEditor * edit_evnv = 0;

        // Event object

    if ( !new_evnv )
        new_evnv = new QmvSqlClass( *reln_evnv );

        // Load the event - this may be a real or a virtual event.
    int evnv_count = new_evnv->open( QString( "select * from evnv where evnv__sequence = %1;" ).arg(ei->rowid ) );

        // Editor object
    edit_evnv = new QmvRowEditor( new_evnv, TRUE, this, "row editor" );

        // Set the editor for editing
    edit_evnv->setAttList( new_evnv->sortedAttlist( "mtat_view_order", QmvClass::NoBlank|QmvClass::NoSystem ) );
    edit_evnv->setReturnButton( "Close", 0, "Close the appointment editor");

        // Find the patient ID from the event item pressed. Extend the patient list
        // if necessary
    int x_patn__sequence = findPatient( ei->patient_id, ei->provider );
    if ( x_patn__sequence < 0 )
    {
        QMessageBox::information( 0, "No Patient Set",
                                  tr( "No patient has been set for this appointment" ),
                                  "OK", 0 );
        return;
    }

        // If the event is virtual, then create a row for it.
    if ( evnv_count == 0 )
    {
        if ( new_evnv->newTuple( TRUE ) )
        {

            QMessageBox::information( 0, "The new appointment cannot be initialized",
                                      tr("Searching for the new appointment is failing"),
                                      "OK", 0 );
            return;
        }
            // load the event from the virtual event item
        new_evnv->update( 0, "evnv_patn__sequence", QString("%1").arg(x_patn__sequence) );
        if ( ei->provider.length() )
            new_evnv->update( 0, "evnv_prov_code", ei->provider);
        if ( ei->referrer.length() )
            new_evnv->update( 0, "evnv_rfdr_code", ei->referrer);
        if ( ei->location.length() )
            new_evnv->update( 0, "evnv_locn_code", ei->location);
        if ( ei->appttype.length() )
            new_evnv->update( 0, "evnv_aptp_code", ei->appttype);
        if ( ei->progress.length() )
            new_evnv->update( 0, "evnv_apst_code", ei->progress);
        QString newtss;
        if ( ei->start.isNull() )
            newtss = "null";
        else
            newtss = QString("%1").arg(qtDateTimeToPgTimestamp(ei->start) );
        new_evnv->update( 0, "evnv_starttime", QString( "%1").arg(newtss) );
        new_evnv->update( 0, "evnv_duration", QString( "%1 minutes").arg(ei->duration));
        int rows_saved = new_evnv->save();
        // if save  failed, display the reason and remove the
        if (rows_saved != 1) {
            QMessageBox::information( 0, "The new appointment cannot be saved",
                                      QString(tr("This appointment cannot be saved:<BR><BR>%1"))
                                      .arg(new_evnv->lastError()),
                                      "OK", 0 );
            new_evnv->remove( 0, true );
            init(); // redisplay
            return;
        }
            // Redisplay the day.
        init();
    }


    int current_row = 0;

    edit_evnv->load(current_row );
    if (new_evnv->attributeValue( current_row, "evnv_patn_patf_code") == "BLOCK") {
        QMessageBox::information( 0, "The new appointment cannot be changed",
                                  QString(tr("This appointment cannot be changed:<BR><BR>%1"))
                                  .arg("the patient is blocked"),
                                  "OK", 0 );
        return;
    }
    edit_evnv->exec();
    init();

}

// --------------------------------------------------------------------------------
// Look for the patient
// --------------------------------------------------------------------------------
int XpracAppTool::findPatient( int sequence, const QString & provider )
{

        // patn object and editor
    static QmvSqlClass * new_patn = 0;
    if ( !new_patn )
        new_patn = new QmvSqlClass( *reln_patn );
    static QmvRowEditor * find_patn = 0;
    if ( !find_patn )
        find_patn = new QmvRowEditor( new_patn, FALSE, this, "row editor" );

        // - Row editor for assigning event to patient, possibly new patient
    static QStringList att_patn_search;
    if ( att_patn_search.count() == 0 )
        att_patn_search << "patn__sequence"
                        << "patn_psnam"
                        << "patn_fsnam"
                        << "patn_title"
                        << "patn_dob"
                        << "patn_address"
                        << "patn_suburb"
                        << "patn_postcode"
                        << "patn_phone"
                        << "patn_phone_1"
                        << "patn_phone_2"
                        << "patn_ref_date"
                        << "patn_ref_period"
                        << "patn_feet_code";

    static QDict<QmvAttribute::access_method> acc_patn_search;
    if ( acc_patn_search.count() == 0 )
    {
        QmvAttribute::access_method rw = QmvAttribute::ReadWrite;
        QmvAttribute::access_method ro = QmvAttribute::ReadOnly;
        acc_patn_search.insert( "patn__sequence", &rw );
        acc_patn_search.insert( "patn_psnam", &rw );
        acc_patn_search.insert( "patn_fsnam", &rw );
        acc_patn_search.insert( "patn_title", &rw );
        acc_patn_search.insert( "patn_dob", &rw );
        acc_patn_search.insert( "patn_address", &rw );
        acc_patn_search.insert( "patn_suburb", &rw );
        acc_patn_search.insert( "patn_postcode", &rw );
        acc_patn_search.insert( "patn_phone", &rw );
        acc_patn_search.insert( "patn_phone_1", &rw );
        acc_patn_search.insert( "patn_phone_2", &rw );
        acc_patn_search.insert( "patn_ref_date", &rw );
        acc_patn_search.insert( "patn_ref_period", &rw );
        acc_patn_search.insert( "patn_feet_code", &rw );

            // get the "search" attribute list and set the access permissions.
        QmvAttList att_patn = new_patn->sortedAttlist( "", att_patn_search );
        for (QmvAttribute * at_ptr = att_patn.first(); at_ptr != 0; at_ptr = att_patn.next())
        {
            QmvAttribute::access_method *ac = acc_patn_search.find(at_ptr->attributeName());
            if ( ac )
                at_ptr->setAccessMethod( *ac );
        }
            // Set the editor for searching
        find_patn->setAttList( att_patn );

        find_patn->setReturnButton( "Existing Patient", existingPatient, "Search for an existing patient who matches");
        find_patn->setReturnButton( "New Patient", newPatient, "Add a new patient to the database");
        find_patn->setReturnButton( "Cancel", cancelSearch, "Exit without proceeding further");
    }

        // Start the search
    int patn_count = 0;
    int patn_row = -1;
    QString query = QString("select * from patn where patn__sequence = %1").arg(sequence);
        // use patn_reln for searches
    patn_count = reln_patn->open( query );

        // Single patient located - return.
    if ( patn_count == 1 )
        return sequence;

        // No patient found - get some search details

        // Create a null tuple for search details
    int find_patn_row = new_patn->newTuple( FALSE );
    if ( find_patn_row < 0 )
    {

        QMessageBox::information( 0, "The patient search cannot be initialized",
                                  tr("Searching for a patient is failing"),
                                  "OK", 0 );
        return -2;
    }
    find_patn->load(find_patn_row);
    findStatus status = (findStatus) find_patn->exec();

        // search is cancelled.
    if ( status == cancelSearch )
        return -1;

        // Collect the search criteria

    QString patn__sequence =    QmvClass::cleanForSQL( new_patn->attributeValue(find_patn_row, "patn__sequence") );
    QString patn_psnam =        QmvClass::cleanForSQL( new_patn->attributeValue(find_patn_row, "patn_psnam") );
    QString patn_fsnam =        QmvClass::cleanForSQL( new_patn->attributeValue(find_patn_row, "patn_fsnam") );
    QString patn_title =        QmvClass::cleanForSQL( new_patn->attributeValue(find_patn_row, "patn_title") );
    QString patn_dob =          QmvClass::cleanForSQL( new_patn->attributeValue(find_patn_row, "patn_dob") );
    QString patn_address =      QmvClass::cleanForSQL( new_patn->attributeValue(find_patn_row, "patn_address") );
    QString patn_suburb =       QmvClass::cleanForSQL( new_patn->attributeValue(find_patn_row, "patn_suburb") );
    QString patn_postcode =     QmvClass::cleanForSQL( new_patn->attributeValue(find_patn_row, "patn_postcode") );
    QString patn_phone =        QmvClass::cleanForSQL( new_patn->attributeValue(find_patn_row, "patn_phone") );
    QString patn_phone_1 =      QmvClass::cleanForSQL( new_patn->attributeValue(find_patn_row, "patn_phone_1") );
    QString patn_phone_2 =      QmvClass::cleanForSQL( new_patn->attributeValue(find_patn_row, "patn_phone_2") );
    QString patn_ref_date =     QmvClass::cleanForSQL( new_patn->attributeValue(find_patn_row, "patn_ref_date") );
    QString patn_ref_period =   QmvClass::cleanForSQL( new_patn->attributeValue(find_patn_row, "patn_ref_period") );
    QString patn_feet_code =    QmvClass::cleanForSQL( new_patn->attributeValue(find_patn_row, "patn_feet_code") );

        // clear the tuple, no longer required
    new_patn->remove( find_patn_row, FALSE );

        // An existing patient
    if ( status == existingPatient )
    {
        query = "select * from patn";
            // Sequence number is unique
        if ( patn__sequence.toInt() > 0 )
            query += QString( " where patn__sequence = %1" ).arg(patn__sequence);
        else {
            QString conj = "where";
            if ( patn_psnam.length() )
            {
                query += QString( " %1 patn_psnam ~* '^%2'::text" ).arg(conj).arg(patn_psnam);
                conj = "and";
            }
            if ( patn_fsnam.length() )
            {
                query += QString( " %1 patn_fsnam ~* '^%2'::text" ).arg(conj).arg(patn_fsnam);
                conj = "and";
            }
            if ( patn_title.length() )
            {
                query += QString( " %1 patn_title ~* '^%2'::text" ).arg(conj).arg(patn_title);
                conj = "and";
            }
            if ( patn_dob.length() )
            {
                query += QString( " %1 patn_dob = '%2'::timestamp" ).arg(conj).arg(patn_dob);
                conj = "and";
            }
            if ( patn_address.length() )
            {
                query += QString( " %1 patn_address ~* '^%2'::text" ).arg(conj).arg(patn_address);
                conj = "and";
            }
            if ( patn_suburb.length() )
            {
                query += QString( " %1 patn_suburb ~* '^%2'::text" ).arg(conj).arg(patn_suburb);
                conj = "and";
            }
            if ( patn_postcode.length() )
            {
                query += QString( " %1 patn_postcode ~* '^%2'::text" ).arg(conj).arg(patn_postcode);
                conj = "and";
            }
            if ( patn_phone.length() )
            {
                query += QString( " %1 patn_phone ~* '^%2'::text" ).arg(conj).arg(patn_phone);
                conj = "and";
            }
            if ( patn_phone_1.length() )
            {
                query += QString( " %1 patn_phone_1 ~* '^%2'::text" ).arg(conj).arg(patn_phone_1);
                conj = "and";
            }
            if ( patn_phone_2.length() )
            {
                query += QString( " %1 patn_phone_2 ~* '^%2'::text" ).arg(conj).arg(patn_phone_2);
                conj = "and";
            }
            if ( patn_ref_date.length() )
            {
                query += QString( " %1 patn_ref_date ~* '^%2'::timestamp" ).arg(conj).arg(patn_ref_date);
                conj = "and";
            }
            if ( patn_ref_period.length() )
            {
                query += QString( " %1 patn_ref_period ~* '^%2'::text" ).arg(conj).arg(patn_ref_period);
                conj = "and";
            }
            if ( patn_feet_code.length() )
            {
                query += QString( " %1 patn_feet_code ~* '^%2'::text" ).arg(conj).arg(patn_feet_code);
                conj = "and";
            }

            query += " order by patn_psnam, patn_fsnam limit 200";


        }
            // execute the search
        patn_count = monitoredQuery( query );

            // Single patient located - return.
        if ( patn_count == 1 )
        {
            patn__sequence = reln_patn->attributeValue(0, "patn__sequence");
            return patn__sequence.toInt();
        }


            // popup a select list, if more than one
        if ( patn_count > 1 )
        {
            QStringList select_labels;
            select_labels << "patn_psnam"
                          << "patn_fsnam"
                          << "patn_feet_code"
                          << "patn_dob"
                          << "patn_suburb"
                          << "patn_ref_date"
                          << "patn_ref_period"
                          << "patn_feet_code"
                          << "patn_last_visit"
                          << "patn_amount_outstanding";
            QmvSelect patn_select( reln_patn, select_labels, this );
            patn_select.setCaption( tr( "Select a patient for this appointment") );
            patn_row = patn_select.exec();
            if ( patn_row >= 0 )
            {
                patn__sequence = reln_patn->attributeValue( patn_row, "patn__sequence" );
                return patn__sequence.toInt();
            } else
                    // escape from select list
                return -2;
        } else {
                // no existing patient
            QMessageBox::information( this, "Patient not found",
 				      tr("<h2>This patient was not found </h2>"
                                         "<ul><li>Surname: <b>%1</b>"
                                         "<li>Firstname: <b>%2</b>"
                                         "<li>DOB: <b>%3</b>"
                                         "<li>Street: <b>%4</b>"
                                         "<li>Suburb: <b>%5 %6</b>"
                                         "<li>Phone(H): <b>%7</b></ul>"
                                         "<li>Phone(W): <b>%8</b></ul>"
                                         "<li>Phone(M): <b>%9</b></ul>")
                                      .arg( patn_psnam )
                                      .arg( patn_fsnam )
                                      .arg( patn_title )
                                      .arg( patn_dob )
                                      .arg( patn_address )
                                      .arg( patn_suburb )
                                      .arg( patn_postcode )
                                      .arg( patn_phone )
                                      .arg( patn_phone_1 )
                                      .arg( patn_phone_2 ),
 				      "OK", 0 );
            return -2;
        }


    } // end of block for existing patient

        // --------------------------------------------------
        // Still no patn -  insert a new patient
        // --------------------------------------------------

        // New tuple
    patn_row = reln_patn->newTuple( TRUE );
    if ( patn_row < 0 )
    {

        QMessageBox::information( this, "The patient record not saved",
                                  tr("The new patient record could not be saved"),
                                  "OK", 0 );
        return -2;
    }
        // Transfer the data
    reln_patn->update( patn_row, "patn_psnam", patn_psnam);
    reln_patn->update( patn_row, "patn_fsnam", patn_fsnam);
    reln_patn->update( patn_row, "patn_title", patn_title);
    reln_patn->update( patn_row, "patn_dob", patn_dob );
    reln_patn->update( patn_row, "patn_address", patn_address );
    reln_patn->update( patn_row, "patn_suburb", patn_suburb );
    reln_patn->update( patn_row, "patn_postcode", patn_postcode );
    reln_patn->update( patn_row, "patn_phone", patn_phone );
    reln_patn->update( patn_row, "patn_phone_1", patn_phone_1 );
    reln_patn->update( patn_row, "patn_phone_2", patn_phone_2 );
    reln_patn->update( patn_row, "patn_ref_date", patn_ref_date );
    reln_patn->update( patn_row, "patn_ref_period", patn_ref_period );
    reln_patn->update( patn_row, "patn_feet_code", patn_feet_code );

        // Use current filter values if notnull
    if ( current_provider_str.length() )
        reln_patn->update( patn_row, "patn_prov_code", current_provider_str );
    else if ( provider.length() )
        reln_patn->update( patn_row, "patn_prov_code", provider );

    if ( reln_patn->save() )
        patn__sequence = reln_patn->attributeValue( patn_row, "patn__sequence" );
    else
    {
        QMessageBox::information( this, "The patient record not saved",
                                  tr("The new patient record could not be saved"),
                                  "OK", 0 );
        reln_patn->remove( patn_row, TRUE );
        return -2;
    }

        // return the new patient
    return patn__sequence.toInt();
}

//--------------------------------------------------------------------------------
//! Run a monitored query, with checks for wildness
/*!

  TODO: this is a copy of  QmvQueryWidget::updateRelationSet(), since this query doesn't use
  the qmvquery classes ... it should
  
  \return int count (returns -1 if error)
*/
//--------------------------------------------------------------------------------

int XpracAppTool::monitoredQuery( const QString query )
{
    bool ok = false;
    int query_limit = reln_patn->queryLimit().toInt(&ok);
    if (!ok) {
        query_limit = 0;
    }
    
    int estimate = countEstimate(query);
    if (query_limit > 0 && estimate > query_limit ) {
        QString msg = QString("<h2>Are you sure that you want to proceed with this query ?</h2> \n"
                              "This query may result in a lengthy search (<b>an estimated %1 rows</b>)\n\n"
                              "If you decide to proceed, you can cancel the query at any time.")
            .arg(estimate);

        int status =
            QMessageBox::information( this,
                                      QString("Warning of possibly long search: %1")
                                      .arg(reln_patn->relationTitle()),
                                      msg,
                                      "&Yes", "&Cancel", QString::null,
                                      0, 1 );
        switch(status)                                   
        {
            case 0:
                break;
            case 1:
                return -1;
                break;
            default: // just for sanity
                break;
        }
    }

    QProgressDialog * controller = new QProgressDialog( QString("<h2>%1:</h2> Loading Records ...")
                                                        .arg(reln_patn->relationTitle()),
                                                        "Cancel", 100,
                                                        this, "progress", TRUE );
    
    int status = reln_patn->open( query, controller );
    delete controller;
    return status;
}


//--------------------------------------------------------------------------------
//! Return the estimated count associated with current query
/*!
  Runs a select count(*) with the current query to estimate the number of rows
  to be returned.

  TODO: this is a copy of QmvSqlQuery::countEstimate(), since this query doesn't use
  the qmvquery classes ... it should
  
  \return int count (returns -1 if error)
*/
//--------------------------------------------------------------------------------

int XpracAppTool::countEstimate( const QString query )
{
    QString str_query = query;
    str_query.replace( QRegExp( "^.* from ", false ), "SELECT count(*) as count_estimate FROM " );
    str_query.replace( QRegExp( " order by +.*$", false ), "" );
    int status = sql_exec(str_query);
    if (status < 1) {
        return -1;
    }

    bool ok = false;
    int count =  QString(sql_value(0, "count_estimate")).toInt(&ok);
    if (ok) {
        return count;
    } else {
        return -1;
    }
}

