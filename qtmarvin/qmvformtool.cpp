/*
 *     $Id: qmvformtool.cpp 3232 2006-12-15 01:18:27Z source $
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
 * DESCRIPTION: User interface for editing forms specifications.
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
#include "qmvformtool.h"
#include "qmvpopup.h"

namespace QmvFormToolIcons
{
#include "left1arrow.xpm"
#include "right1arrow.xpm"
#include "left2arrow.xpm"
#include "right2arrow.xpm"
#include "print.xpm"
#include "fax.xpm"
#include "email.xpm"
#include "days_1.xpm"
#include "days_5.xpm"
#include "days_7.xpm"
#include "month.xpm"
#include "today.xpm"
#include "agenda.xpm"
#include "zoomin.xpm"
#include "zoomout.xpm"
#include "quit.xpm"
} // end QmvFormToolIcons

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmvformtool_version[] = "$Id: qmvformtool.cpp 3232 2006-12-15 01:18:27Z source $";
// #define QMVFORMTOOL_DEBUG
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

QmvFormTool::QmvFormTool( QmvRelationWidget * rlw, QmvApplication * parent, const char * name )
        : QmvToolWindow( rlw->getRelation(), parent, name )
{
    
        // ------------------------------------------------------------
        // Query Objects
        // ------------------------------------------------------------
    
    
        // Form header
    fh = 0;
    pagesize = background.size();
    
    if ( rlw )
        rlw_form = rlw;
    else
        throw( QmvException( QString( "<h3>QmvFormTool::QmvFormTool</h2>"
                                      "Failed to build FORM object" ),
                             QmvException::Critical) );
    reln_form = rlw->getRelation();
    if ( !reln_form )
        throw( QmvException( QString( "<h3>QmvFormTool::QmvFormTool</h2>"
                                      "Failed to build FORM object" ),
                             QmvException::Critical) );
        // Form details.
    
    QmvSqlClass * fmdt_existing = (QmvSqlClass *) reln_form->dataBase()->dbRelation( "fmdt" );
    if ( fmdt_existing )
        reln_fmdt = new QmvSqlClass( *fmdt_existing );
    else
        throw( QmvException( QString( "<h3>QmvFormTool::QmvFormTool</h2>"
                                      "Failed to build FMDT object" ),
                             QmvException::Critical) );
    
    
 
        // -------------------------------------------
        // Mouse Clicks ------------------------------
        // -------------------------------------------
    
        // notice button clicks on grid
    connect( canvasView(), SIGNAL( clicked( QCanvasItem *, const QPoint &, ButtonState, ButtonState ) ),
             this, SLOT( popupEventDetails( QCanvasItem *, const QPoint &, ButtonState, ButtonState ) ) );

        //  doubleclick - edit
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

    QAccel * ap_accel = new QAccel( this, "forms" );
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

    QAction * ac;
    QActionGroup * ag_insert = new QActionGroup( this, "ag_insert" );
    
        // ------------------------------------------------------------
        // Navigation Toolbar
        // ------------------------------------------------------------
        // clear the navigation toolbar - not needed yet
    navigationToolBar()->clear();
    
        // ------------------------------------------------------------
        // Tool buttons for adding detail objects
        // ------------------------------------------------------------
    
    ag_insert->setExclusive(TRUE);
    QSignalMapper *ndt_sigmap = new QSignalMapper( this );
    connect( ndt_sigmap, SIGNAL( mapped(int) ),
             this, SLOT( slotAddNewDetail(int) ) );
        
        // Label -----------------------------------------------------
    ac = new QAction( "Text",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                                 tr("T"), QFont("Times"), QColor( blue ), QRect() ),
                      "Text",
                      ALT+SHIFT+Key_T,
                      ag_insert, "text", FALSE );
    ndt_sigmap->setMapping( ac, formDetail::Label );
    connect( ac, SIGNAL( activated() ), ndt_sigmap, SLOT( map() ) );
        
        // Field -----------------------------------------------------
    ac = new QAction( "Field",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                                 tr("F"), QFont("Times"), QColor( blue ), QRect() ),
                      "Field",
                      ALT+SHIFT+Key_F,
                      ag_insert, "Field", FALSE );
    ndt_sigmap->setMapping( ac, formDetail::Field );
    connect( ac, SIGNAL( activated() ), ndt_sigmap, SLOT( map() ) );
        
        // Line -----------------------------------------------------
    ac = new QAction( "Line",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                                 tr("L"), QFont("Times"), QColor( blue ), QRect() ),
                      "Line",
                      ALT+SHIFT+Key_L,
                      ag_insert, "Line", FALSE );
    ndt_sigmap->setMapping( ac, formDetail::Line );
    connect( ac, SIGNAL( activated() ), ndt_sigmap, SLOT( map() ) );
        
        // Calc -----------------------------------------------------
    ac = new QAction( "Calculation",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                                 tr("C"), QFont("Times"), QColor( blue ), QRect() ),
                      "Calculation",
                      ALT+SHIFT+Key_C,
                      ag_insert, "Calculation", FALSE );
    ndt_sigmap->setMapping( ac, formDetail::Calc );
    connect( ac, SIGNAL( activated() ), ndt_sigmap, SLOT( map() ) );
        
        // Special -----------------------------------------------------
    ac = new QAction( "Special",
                      *stdicons->makeTextPixmap( new QPixmap(stdicons->getPixmap(QmvIcons::BlankPageIcon)),
                                                 tr("S"), QFont("Times"), QColor( blue ), QRect() ),
                      "Special",
                      ALT+SHIFT+Key_S,
                      ag_insert, "Special", FALSE );
    ndt_sigmap->setMapping( ac, formDetail::Special );
    connect( ac, SIGNAL( activated() ), ndt_sigmap, SLOT( map() ) );
    
        // Print ------------------------------------------------------
    ac = new QAction( "Print",
                      QPixmap( QmvFormToolIcons::print_xpm ),
                      "&Print",
                      Key_P,
                      ag_insert, "form_print", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotPrint() ) );

        // Fax ------------------------------------------------------
    ac = new QAction( "Fax",
                      QPixmap( QmvFormToolIcons::fax_xpm ),
                      "Fa&x",
                      Key_X,
                      ag_insert, "form_fax", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotFax() ) );

        // Email ------------------------------------------------------
    ac = new QAction( "Email",
                      QPixmap( QmvFormToolIcons::email_xpm ),
                      "&Email",
                      Key_E,
                      ag_insert, "form_email", FALSE );
    connect( ac, SIGNAL( activated() ), this, SLOT( slotEmail() ) );

        // Add actiongroup to toolbar
    otherToolBar()->clear();
    new QLabel( "Add new detail: ", otherToolBar(), "display_label" );
    ag_insert->addTo( otherToolBar() );

   
        // prepare for display
    setToolBarsMovable(FALSE);
    adjustSize();
    updateGeometry();
    setFocus();
        // adjust canvas to the base magnification.
    setCanvasSize();
}

// --------------------------------------------------------------------------------
// Destructor
// --------------------------------------------------------------------------------

QmvFormTool::~QmvFormTool()
{
    canvas_details.setAutoDelete( true );
    canvas_details.clear();

    if ( reln_form )
        delete reln_form;
    if ( reln_fmdt )
        delete reln_fmdt;
    if ( fh )
        delete fh;
}



// --------------------------------------------------------------------------------
//! Initialize the display
/*!

Initialize the display, after loading (or reloading) the form. If \em name is null,
then use the exising form.

\param name   The code for requested form

*/
// --------------------------------------------------------------------------------
void QmvFormTool::init( const QString & name )
{
    

        // If null name, then there must already be a form.
    if ( !name.length() && !fh )
        return;

        // Construct a formHeader strutcure if not exists.
    if (!fh)
        fh = new formHeader;

        // Reload with a new form
    if ( name.length() )
        for ( int i = 0; i < reln_form->count(); i++ )
        {
            if ( reln_form->attributeValue( i, "form_code" ) == name )
            {
                if (!fh->load( i, reln_form ))
                {
                    QMessageBox::information( 0, "init::Not Found",
                                              tr("No form could be loaded."),
                                              "OK", 0 );
                    return;
                }
                break;
            }
        }

        // Look for includes
    QString form_detail = QString ( "%1 %2" )
        .arg( fh->form_code ).arg( fh->form_includes );
    form_detail.simplifyWhiteSpace();
    form_detail.replace( QRegExp("[, ]"), "','" );
    form_detail = QString( "fmdt_form_code in ( '%1')" )
        .arg( form_detail );
    
    int count = reln_fmdt->open( QString("select * from fmdt where %1" ).arg(form_detail) );

        // Clear existing objects
    canvas_details.setAutoDelete( true );
    canvas_details.clear();

    initDisplayParameters();

        // --------------------------------------------
        // Setup the page.
        // Clear the background and set default labels
        // --------------------------------------------

        // Resize canvas to pagesize
    
    clearCanvasView();
    buildBackgroundPixmap();
    QSize sz = background.size();
    getCanvas()->resize( background.width(), background.height() );
    getCanvas()->setBackgroundPixmap ( background );
    canvasView()->adjustSize();
    adjustSize();
    updateGeometry();
    sz = getCanvas()->size();
    setCaption( QString( "Forms Management - %1" ).arg(reln_form->attributeValue( 0, "form_desc" ) ) );
    displayDetails();
}

// --------------------------------------------------------------------------------
//!
/*!

*/
// --------------------------------------------------------------------------------
void QmvFormTool::initDisplayParameters()
{
        //configured - get from resource file?
    QString pref;
    bool ok;
    current_page = FirstPage;

        // ----------------------------------------
        // settable in preferences.
        // ----------------------------------------

//         // am/pm display
//     pref = parentApp()->userPreference("QmvFormTool.am_pm");
//     if ( !pref.isNull() )
//     {
//         int i = pref.toInt( &ok );
//         if ( ok && i >= 0 )
//             day_display.am_pm = true;
//     }
    
}

// --------------------------------------------------------------------------------
//! 
/*!

*/
// --------------------------------------------------------------------------------
void QmvFormTool::buildBackgroundPixmap( )
{
        // Determine page size using a QPinter object

    QPrinter* prt;
    QSize ps;

        // Set the page size
    prt = new QPrinter();
    prt->setFullPage(true);
    prt->setPageSize( (QPrinter::PageSize) fh->form_pagesize.toInt() );
    prt->setOrientation( (QPrinter::Orientation) fh->form_orientation.toInt() );

        // Get the page metrics
    QPaintDeviceMetrics pdm(prt);
    ps.setWidth(pdm.width());
    ps.setHeight(pdm.height());

    delete prt;

    background.resize( ps );
    pagesize = ps;
    
    background.fill( getTransparentColor() );

        // clear the canvas event index
    canvas_details.setAutoDelete(FALSE);
    canvas_details.clear();

}


// --------------------------------------------------------------------------------
void QmvFormTool::slotAddNewDetail( int id )
{
    if ( id < formDetail::Label || id >  formDetail::Special )
        return;
    
        // do not auto-save, since data needs to be massaged with FK first.
    int row = reln_fmdt->newTuple(FALSE);
    if ( row < 0 )
    {
        QMessageBox::warning(0, "slotAddNewDetail:Failed",
                             tr("<b>%1:</b><br><br>Failed to add a new row<br><b>%2</b>")
                             .arg(reln_fmdt->relationTitle())
                             .arg(reln_fmdt->lastError()),
                             "OK", 0 );
        return;
    }
        // could come from current cursor
    QPoint pos = QPoint(20,20);
    QSize size;
    QString tmp_text;
    
    reln_fmdt->update( row, "fmdt_form_code", fh->form_code );
    reln_fmdt->update( row, "fmdt_field_type", QString("%1").arg(id) );

        // special actions
    switch (id)
    {
        case formDetail::Label:
            size = QSize(100,12);
            tmp_text = "LABEL";
            break;
        case formDetail::Field:
            size = QSize(50,12);
            tmp_text = "FIELD";
            break;
        case formDetail::Line:
            size = QSize(100,2);
            tmp_text = "";
            break;
        case formDetail::Calc:
            break;
        case formDetail::Special:
            break;
    }
    reln_fmdt->update( row, "fmdt_text", tmp_text );
    reln_fmdt->update( row, "fmdt_width", QString("%1").arg(size.width()) );
    reln_fmdt->update( row, "fmdt_height", QString("%1").arg(size.height()) );
    reln_fmdt->update( row, "fmdt_x_coord", QString("%1").arg(pos.x()) );
    reln_fmdt->update( row, "fmdt_y_coord", QString("%1").arg(pos.y()) );
                
    if ( !reln_fmdt->save( row ) )
    {
        QMessageBox::warning(0, "slotAddNewDetail:Failed",
                             tr("<b>%1:</b><br><br>Failed to add a new row<br><b>%2</b>")
                             .arg(reln_fmdt->relationTitle())
                             .arg(reln_fmdt->lastError()),
                             "OK", 0 );
        return;
    }

    init();
    
}

// --------------------------------------------------------------------------------
void QmvFormTool::slotSetBannerText( const QString & text )
{
    qDebug("QmvFormTool::slotSetBannerText: %s", text.latin1() );
 
    QmvToolWindow::slotSetBannerText( text );
 
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvFormTool::slotBack1()
{
    if ( current_page <= FirstPage )
        return;
    
    current_page--;
    init();
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvFormTool::slotForward1()

{
    if ( current_page >= LastPage )
        return;
    
    current_page++;
    init();
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvFormTool::slotPrint()
{

        // find an existing data object
    QString obj = reln_form->attributeValue( 0, "form_classname" );
    QmvSqlClass * obj_reln = (QmvSqlClass *) reln_form->dataBase()->dbRelation( obj );
    if ( !obj_reln || obj_reln->count() < 1 )
    {
        
        QMessageBox::information( 0, "slotPrint::Not Found",
                                  tr("No matching data was located, to be used for printing."
                                     " You need to load some data for <b>%1</b>")
                                  .arg( obj_reln ? obj_reln->relationTitle() : obj),
                                  "OK", 0 );
        return;
    }
    
        // Setup a form viewer, and prepare the report.
    QmvForm form_viewer( obj_reln, this, "events" );

    if ( !form_viewer.initForm( reln_form->attributeValue( 0, "form_code" ) ) )
        return;
    form_viewer.setCaption( "Forms - Test Display" );

    form_viewer.setPrint( parentApp()->userPreference("PrintProgram"),
                          parentApp()->userPreference("PrinterSelectionOption"),
                          parentApp()->userPreference("PrinterName") );
    
    form_viewer.viewForm();

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvFormTool::slotFax()
{}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvFormTool::slotEmail()
{}

// --------------------------------------------------------------------------------
//! Display the form details
/*!

*/
// --------------------------------------------------------------------------------
void QmvFormTool::displayDetails()
{
    
    slotSetBannerText( QString("%1").arg( fh->form_desc ) );

        // determine page data
    

        // prepare the details in the list.
    formDetail fd;
    QRect rect;
    QPixmap pm;
    QPainter pt;
    int x,y,w,h;
    QPoint pos;
    QSize size;
    QColor fill;
    
    for ( int row = 0; row < reln_fmdt->count(); row++ )
    {
        if (!fd.load( row, reln_fmdt) )
            continue;
        
        if ( fd.fmdt_field_type < formDetail::Label || fd.fmdt_field_type > formDetail::Special )
            continue;
        
            // the raw position from fmdt
        pos = QPoint( fd.fmdt_x_coord, fd.fmdt_y_coord );
        size = QSize( fd.fmdt_width, fd.fmdt_height );
        fill = QColor( Qt::gray );
        
            // special actions
        switch (fd.fmdt_field_type)
        {
            case formDetail::Label:
                break;
            case formDetail::Field:
                break;
            case formDetail::Line:
                fill = QColor( Qt::red );
                size.setHeight(2);
                break;
            case formDetail::Calc:
                break;
            case formDetail::Special:
                break;
            default:
                continue;
                break;
        }
        
        
            // adjust for section and margins
        switch (fd.fmdt_section)
        {
            case formDetail::ReportHeader:
            case formDetail::PageHeader:
                pos += QPoint( fh->form_mg_left, fh->form_mg_top );
                break;
            case formDetail::Detail:
                pos += QPoint( fh->form_mg_left, (fh->form_mg_top + fh->form_ph_height));
                break;
            case formDetail::PageFooter:
            case formDetail::ReportFooter:
                    // measure from bottom
                pos += QPoint( fh->form_mg_left, (pagesize.height() - fh->form_mg_bottom - fh->form_pf_height) );
                break;
        }

        if ( pos.x() < 0 || pos.y() < 0 || size.width() < 1 || size.height() < 1 )
            continue;
        
            // build a QRect for this detail
        rect.setSize( size );
            // reset origin, ready for drawing
        rect.moveTopLeft( QPoint(0,0) );
        
            // build a detail label using a pixmap
        pm.resize( rect.size() );
        pm.fill(getTransparentColor());
        pt.begin(&pm);

            // fill the background and draw the label
        pt.setPen( QPen( Qt::black, 2, SolidLine ) );
        pt.fillRect( rect, QBrush( fill, QBrush::SolidPattern ) );
        pt.setFont( QFont( fd.fmdt_ft_family, fd.fmdt_ft_size) );
        pt.drawText( rect, WordBreak|AlignCenter, fd.fmdt_text );

            // draw a border around the object
        pt.setBrush( Qt::NoBrush );
        pt.setPen(QPen( Qt::black, 2, SolidLine ) );
        pt.drawRect( rect );
        pt.end();

            // put the prepared pixmap on a canvas object
        QmvCanvasGrid * disp_obj = new QmvCanvasGrid( getCanvas(), rect.size(), getTransparentColor() );
            // use the canvas object as a key in list of displayed details
        canvas_details.insert( disp_obj, new int(row) );
        
            //disp_obj->setOpaqueFactor( 30 );
        disp_obj->drawPixmap( QPoint(0,0), pm, pm.rect() );

            // Move the object into place
        disp_obj->move( pos.x(), pos.y() );
        
        disp_obj->show();
            //pm.save( QString("tmp/%1.xpm").arg(row),"XPM" );
    }
    canvasView()->update();
}



// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvFormTool::popupEventDetails( QCanvasItem * pressed, const QPoint & pt,
                                      ButtonState button, ButtonState state )
{

    if ( button == Qt::RightButton && pressed )
    {
        int *i = canvas_details.find( pressed );
        if (!i)
            return;
        int row = *i;
        if ( row >= 0 && row < reln_fmdt->count() )
        {
            QString detail = reln_fmdt->attributeValue( row, "fmdt_text" );
            bool ok;
            int rowid = reln_fmdt->attributeValue( row, "fmdt__sequence" ).toInt(&ok);
            if (!ok)
            {
                QMessageBox::information( 0, "popupEventDetails::Not Found",
                                          tr("The current detail, <b>%1</b>, appears to be corrupted"
                                             " and the storage index cannot be determined.")
                                          .arg( detail ),
                                          "OK", 0 );
                return;
            }

                // A popup menu for multiple, overlaid event objects
            QPopupMenu item_menu;
                // insert the current event
            item_menu.insertItem( QString( "%1" )
                                  .arg( reln_fmdt->attributeValue(row,"fmdt_text") ),
                                  -1, -1 );
            item_menu.insertSeparator();
            int row_edit = item_menu.insertItem( QString( "Edit this detail" ), -1, -1 );
            int row_delete = item_menu.insertItem( QString( "Delete this detail" ), -1, -1 );
            int row_properties = item_menu.insertItem( QString( "Properties" ), -1, -1 );
            int id = item_menu.exec( mapToGlobal( pt ));
            if ( id == row_delete )
            {
                if ( QMessageBox::information( this, "popupEventDetails::Delete",
                                               tr("Are you sure that you want to delete this detail?<br>"
                                                  " <b>%1</b>")
                                               .arg( detail ),
                                               QMessageBox::Yes | QMessageBox::Default,
                                               QMessageBox::Cancel | QMessageBox::Escape) == QMessageBox::Yes )
                {
                    if ( !reln_fmdt->remove( row, TRUE ) )
                    {
                        QMessageBox::warning(0, "Delete Detail:Warning",
                                             tr("<b>%1:</b><br><br>The delete failed<br><b>%2</b>")
                                             .arg(reln_fmdt->relationTitle())
                                             .arg(reln_fmdt->lastError()),
                                             "OK", 0 );
                        return;
                    }

                    init();
                }
            } else if ( id == row_properties )
                new QmvPopup( QString("<b>%1</b><br>"
                                      "X: %2<br>"
                                      "Y: %3<br>"
                                      "Width: %4<br>"
                                      "Height: %5<br>"
                                      "Font: %6<br>"
                                      "Point: %7<br>"
                                      "H-align: %8<br>"
                                      "V-align: %9")
                              .arg( detail )
                              .arg( reln_fmdt->attributeValue( row, "fmdt_x_coord" ) )
                              .arg( reln_fmdt->attributeValue( row, "fmdt_y_coord" ) )
                              .arg( reln_fmdt->attributeValue( row, "fmdt_width" ) )
                              .arg( reln_fmdt->attributeValue( row, "fmdt_height" ) )
                              .arg( reln_fmdt->attributeValue( row, "fmdt_ft_family" ) )
                              .arg( reln_fmdt->attributeValue( row, "fmdt_ft_size" ) )
                              .arg( reln_fmdt->attributeValue( row, "fmdt_hz_align" ) )
                              .arg( reln_fmdt->attributeValue( row, "fmdt_vt_align" ) ),
                              mapToGlobal(pt),  this, "event_detail" );
            else
                dataChangeRequested( pressed, mapToGlobal( pt ), Qt::RightButton, Qt::RightButton );
            
        }
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
void QmvFormTool::eventLabelMoved( QCanvasItem * pressed, const QPoint & from, const QPoint & to )
{
    int *i = canvas_details.find( pressed );
    if ( !i || *i < 0 || *i >= reln_fmdt->count() )
        return;
    int row = *i;
    
    formDetail fd;
    if ( !fd.load( row, reln_fmdt ) )
        return;

        // the new position
    QPoint new_pos = QPoint( static_cast<int> (pressed->x()), static_cast<int> (pressed->y()) );
    
        // adjust for section and margins
    switch (fd.fmdt_section)
    {
        case formDetail::ReportHeader:
        case formDetail::PageHeader:
            new_pos -= QPoint( fh->form_mg_left, fh->form_mg_top );
            break;
        case formDetail::Detail:
            new_pos -= QPoint( fh->form_mg_left, (fh->form_mg_top + fh->form_ph_height));
            break;
        case formDetail::PageFooter:
        case formDetail::ReportFooter:
                // measure from bottom
            new_pos -= QPoint( fh->form_mg_left, (pagesize.height() - fh->form_mg_bottom - fh->form_pf_height) );
            break;
    }
             

        // the old position
    QPoint old_pos = QPoint( fd.fmdt_x_coord, fd.fmdt_y_coord );
    
    
        // if changed, so update update 
    if ( new_pos != old_pos )
    {
            // special actions
        switch (fd.fmdt_field_type)
        {
            case formDetail::Label:
                break;
            case formDetail::Field:
                break;
            case formDetail::Line:
                break;
            case formDetail::Calc:
                break;
            case formDetail::Special:
                break;
            default:
                break;
        }
        fd.fmdt_x_coord = new_pos.x();
        fd.fmdt_y_coord = new_pos.y();
        fd.update( row, reln_fmdt );
    
        if (! reln_fmdt->save(row) )
        {
            QMessageBox::warning(0, "eventLabelMoved::Failed",
                                 tr("<b>%1:</b><br><br>Failed to update with new position<br><b>%2</b>")
                                 .arg(reln_fmdt->relationTitle())
                                 .arg(reln_fmdt->lastError()),
                                 "OK", 0 );
            return;
        }
    }
    
    init();

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
void QmvFormTool::eventLabelResized( QCanvasItem * pressed, const QRect & from, const QRect & to )
{
    int *i = canvas_details.find( pressed );
    if ( !i || *i < 0 || *i >= reln_fmdt->count() )
        return;
    int row = *i;
    
    formDetail fd;
    if ( !fd.load( row, reln_fmdt ) )
        return;

    
    QString detail = reln_fmdt->attributeValue( row, "fmdt_text" );
    bool ok;
    int rowid = reln_fmdt->attributeValue( row, "fmdt__sequence" ).toInt(&ok);
    if (!ok)
    {
        QMessageBox::information( 0, "popupEventDetails::Not Found",
                                  tr("The current detail, <b>%1</b>, appears to be corrupted"
                                     " and the storage index cannot be determined.")
                                  .arg( detail ),
                                  "OK", 0 );
        return;
    }

        // if changed, so update update 
    if ( from != to )
    {
        QRect new_rect = QRect(to);
        
            // special actions
        switch (fd.fmdt_field_type)
        {
            case formDetail::Label:
                break;
            case formDetail::Field:
                break;
            case formDetail::Line:
                new_rect.setHeight(0);
                break;
            case formDetail::Calc:
                break;
            case formDetail::Special:
                break;
            default:
                break;
        }
        fd.fmdt_width = new_rect.width();
        fd.fmdt_height = new_rect.height();
        fd.update( row, reln_fmdt );
    
        if (! reln_fmdt->save(row) )
        {
            QMessageBox::warning(0, "eventLabelResized::Failed",
                                 tr("<b>%1:</b><br><br>Failed to update with new size<br><b>%2</b>")
                                 .arg(reln_fmdt->relationTitle())
                                 .arg(reln_fmdt->lastError()),
                                 "OK", 0 );
            return;
        }
    }

    init();

    return;
}
    
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvFormTool::dataChangeRequested( QCanvasItem * pressed, const QPoint & pt, ButtonState button, ButtonState state )
{
//     static QmvSqlClass * new_fmdt = 0;
//     static QmvRowEditor * edit_evnv = 0;
//     static QDict<QmvAttribute::access_method> acc_evnv_edit;
        
//     eventItem * ei = canvas_details.find( pressed );
    
//     QmvAttribute::access_method rw = QmvAttribute::ReadWrite;
//     QmvAttribute::access_method ro = QmvAttribute::ReadOnly;

//         // Event object

//     if ( !new_evnv )
//         new_evnv = new QmvSqlClass( *reln_evnv );

//         // Load the event - this may be a real or a virtual event.
//     int evnv_count = new_evnv->open( QString( "select * from evnv where evnv__sequence = %1;" ).arg(ei->rowid ) );

//         // Editor object
//     if ( !edit_evnv )
//     {
//         edit_evnv = new QmvRowEditor( new_evnv, TRUE, this, "row editor" );
//             // Build lists of QmvAttribute items to use in editor window.
//             // For editing an existing patient.
//         QStringList att_evnv_edit;
//         att_evnv_edit << "evnv_patn__sequence"
//                       << "evnv_patn_psnam"
//                       << "evnv_patn_fsnam"
//                       << "evnv_patn_dob"
//                       << "evnv_patn_address"
//                       << "evnv_patn_suburb"
//                       << "evnv_patn_postcode"
//                       << "evnv_patn_phone"
//                       << "evnv_prov_code"
//                       << "evnv_rfdr_code"
//                       << "evnv_locn_code"
//                       << "evnv_aptp_code"
//                       << "evnv_apst_code"
//                       << "evnv_starttime"
//                       << "evnv_duration"
//                       << "evnv_desc";

//         if ( acc_evnv_edit.count() == 0 )
//         {
//             acc_evnv_edit.insert( "evnv_patn__sequence", &ro );
//             acc_evnv_edit.insert( "evnv_patn_psnam", &rw );
//             acc_evnv_edit.insert( "evnv_patn_fsnam", &rw );
//             acc_evnv_edit.insert( "evnv_patn_dob", &rw );
//             acc_evnv_edit.insert( "evnv_patn_address", &rw );
//             acc_evnv_edit.insert( "evnv_patn_suburb", &rw );
//             acc_evnv_edit.insert( "evnv_patn_postcode", &rw );
//             acc_evnv_edit.insert( "evnv_patn_phone", &rw );
//         }
    

//             // get the "search" attribute list and set the access permissions.
//         QmvAttList att_evnv = new_evnv->sortedAttlist( "", att_evnv_edit );
//         for (QmvAttribute * at_ptr = att_evnv.first(); at_ptr != 0; at_ptr = att_evnv.next())
//         {
//             QmvAttribute::access_method *ac = acc_evnv_edit.find(at_ptr->attributeName());
//             if ( ac )
//                 at_ptr->setAccessMethod( *ac );
//         }

//             // Set the editor for editing
//         edit_evnv->setAttList( att_evnv );
//         edit_evnv->setReturnButton( "Close", 0, "Close the appointment editor");
//     }
    
//         // Find the patient ID from the event item pressed. Extend the patient list
//         // if necessary
//     int x_patn__sequence = findPatient( ei->patient_id );
//     if ( x_patn__sequence < 0 )
//     {
//         QMessageBox::information( 0, "No Patient Set",
//                                   tr( "No patient has been set for this appointment" ),
//                                   "OK", 0 );
//         return;
//     }

//         // If the event is virtual, then create a row for it.
//     if ( evnv_count == 0 )
//     {
//         if ( new_evnv->newTuple( TRUE ) )
//         {
            
//             QMessageBox::information( 0, "The new appointment cannot be initialized",
//                                       tr("Searching for the new appointment is failing"),
//                                       "OK", 0 );
//             return;
//         }
//             // load the event from the virtual event item
//         new_evnv->update( 0, "evnv_patn__sequence", QString("%1").arg(x_patn__sequence) );
//         if ( ei->provider.length() )
//             new_evnv->update( 0, "evnv_prov_code", ei->provider);
//         if ( ei->referrer.length() )
//             new_evnv->update( 0, "evnv_rfdr_code", ei->referrer);
//         if ( ei->location.length() )
//             new_evnv->update( 0, "evnv_locn_code", ei->location);
//         if ( ei->appttype.length() )
//             new_evnv->update( 0, "evnv_aptp_code", ei->appttype);
//         if ( ei->progress.length() )
//             new_evnv->update( 0, "evnv_apst_code", ei->progress);
//         QString newtss;
//         if ( ei->start.isNull() )
//             newtss = "null";
//         else
//             newtss = QString("%1").arg(qtDateTimeToPgTimestamp(ei->start) );
//         new_evnv->update( 0, "evnv_starttime", QString( "%1").arg(newtss) );
//         new_evnv->update( 0, "evnv_duration", QString( "%1 minutes").arg(ei->minutes));
//         new_evnv->save();

//             // Redisplay the day.
//         init();
//     }
    
        
//     int current_row = 0;

//     edit_evnv->load(current_row );
//     edit_evnv->exec();
//     init();

}

// --------------------------------------------------------------------------------
// Look for the patient
// --------------------------------------------------------------------------------
int QmvFormTool::findPatient( int sequence )
{

//         // patn object and editor
//     static QmvSqlClass * new_patn = 0;
//     if ( !new_patn )
//         new_patn = new QmvSqlClass( *reln_patn );
//     static QmvRowEditor * find_patn = 0;
//     if ( !find_patn )
//         find_patn = new QmvRowEditor( new_patn, FALSE, this, "row editor" );

//         // - Row editor for assigning event to patient, possibly new patient
//     static QStringList att_patn_search;
//     if ( att_patn_search.count() == 0 )
//         att_patn_search << "patn__sequence"
//                         << "patn_psnam"
//                         << "patn_fsnam"
//                         << "patn_dob"
//                         << "patn_address"
//                         << "patn_suburb"
//                         << "patn_postcode"
//                         << "patn_phone";
    
//     static QDict<QmvAttribute::access_method> acc_patn_search;
//     if ( acc_patn_search.count() == 0 )
//     {
//         QmvAttribute::access_method rw = QmvAttribute::ReadWrite;
//         QmvAttribute::access_method ro = QmvAttribute::ReadOnly;
//         acc_patn_search.insert( "patn__sequence", &rw );
//         acc_patn_search.insert( "patn_psnam", &rw );    
//         acc_patn_search.insert( "patn_fsnam", &rw );
//         acc_patn_search.insert( "patn_dob", &rw );
//         acc_patn_search.insert( "patn_address", &rw );
//         acc_patn_search.insert( "patn_suburb", &rw );
//         acc_patn_search.insert( "patn_postcode", &rw );
//         acc_patn_search.insert( "patn_phone", &rw );

//             // get the "search" attribute list and set the access permissions.
//         QmvAttList att_patn = new_patn->sortedAttlist( "", att_patn_search );
//         for (QmvAttribute * at_ptr = att_patn.first(); at_ptr != 0; at_ptr = att_patn.next())
//         {
//             QmvAttribute::access_method *ac = acc_patn_search.find(at_ptr->attributeName());
//             if ( ac )
//                 at_ptr->setAccessMethod( *ac );
//         }
//             // Set the editor for searching
//         find_patn->setAttList( att_patn );
//         find_patn->setReturnButton( "Existing Patient", 0, "Search for an existing patient who matches");
//         find_patn->setReturnButton( "New Patient", 1, "Add a new patient to the database");
//         find_patn->setReturnButton( "Cancel", -1, "Exit without proceeding further");
//     }
    
//         // Start the search
//     int patn_count = 0;
//     int patn_row = -1;
//     QString query = QString("select * from patn where patn__sequence = %1").arg(sequence);
//         // use patn_reln for searches
//     patn_count = reln_patn->open( query );

//         // Single patient located - return.
//     if ( patn_count == 1 )
//         return sequence;

//         // No patient found - get some search details

//         // Create a null tuple for search details
//     int find_patn_row = new_patn->newTuple( FALSE );
//     if ( find_patn_row < 0 )
//     {
            
//         QMessageBox::information( 0, "The patient search cannot be initialized",
//                                   tr("Searching for a patient is failing"),
//                                   "OK", 0 );
//         return -2;
//     }
//     find_patn->load(find_patn_row);
//     int status = find_patn->exec();
    
//         // search is cancelled.
//     if ( status < 0 )
//         return status;
    
//         // Collect the search criteria
    
//     QString patn__sequence = new_patn->attributeValue(find_patn_row, "patn__sequence");
//     QString patn_psnam = new_patn->attributeValue(find_patn_row, "patn_psnam");
//     QString patn_fsnam = new_patn->attributeValue(find_patn_row, "patn_fsnam");
//     QString patn_dob = new_patn->attributeValue(find_patn_row, "patn_dob");
//     QString patn_address = new_patn->attributeValue(find_patn_row, "patn_address");
//     QString patn_suburb = new_patn->attributeValue(find_patn_row, "patn_suburb");
//     QString patn_postcode = new_patn->attributeValue(find_patn_row, "patn_postcode");
//     QString patn_phone = new_patn->attributeValue(find_patn_row, "patn_phone");

//         // clear the tuple, no longer required
//     new_patn->remove( find_patn_row, FALSE );

//         // An existing patient
//     if ( status == 0 )
//     {
//         query = "select * from patn";
//             // Sequence number is unique
//         if ( patn__sequence.toInt() > 0 )
//             query += QString( " where patn__sequence = %1" ).arg(patn__sequence);
//         else {
//             QString conj = "where";
//             if ( patn_psnam.length() )
//             {
//                 query += QString( " %1 patn_psnam ~~* '%2'::text" ).arg(conj).arg(patn_psnam);
//                 conj = "and";
//             }
//             if ( patn_fsnam.length() )
//             {
//                 query += QString( " %1 patn_fsnam ~~* '%2'::text" ).arg(conj).arg(patn_fsnam);
//                 conj = "and";
//             }
//             if ( patn_dob.length() )
//             {
//                 query += QString( " %1 patn_dob = '%2'::timestamp" ).arg(conj).arg(patn_dob);
//                 conj = "and";
//             }
//             if ( patn_address.length() )
//             {
//                 query += QString( " %1 patn_address ~~* '%2'::text" ).arg(conj).arg(patn_address);
//                 conj = "and";
//             }
//             if ( patn_suburb.length() )
//             {
//                 query += QString( " %1 patn_suburb ~~* '%2'::text" ).arg(conj).arg(patn_suburb);
//                 conj = "and";
//             }
//             if ( patn_postcode.length() )
//             {
//                 query += QString( " %1 patn_postcode ~~* '%2'::text" ).arg(conj).arg(patn_postcode);
//                 conj = "and";
//             }
//             if ( patn_phone.length() )
//             {
//                 query += QString( " %1 patn_phone ~~* '%2'::text" ).arg(conj).arg(patn_phone);
//                 conj = "and";
//             }

//             query += " order by patn_psnam, patn_fsnam limit 200";
            
        
//         }
//             // execute the search 
//         patn_count = reln_patn->open( query );

//             // Single patient located - return.
//         if ( patn_count == 1 )
//         {
//             patn__sequence = reln_patn->attributeValue(0, "patn__sequence");
//             return patn__sequence.toInt();
//         }
        

//             // popup a select list, if more than one
//         if ( patn_count > 1 )
//         {
//             QStringList select_labels;
//             select_labels << "patn_psnam"
//                           << "patn_fsnam"
//                           << "patn_feet_code"
//                           << "patn_dob"
//                           << "patn_suburb";
//             QmvSelect patn_select( reln_patn, select_labels, this );
//             patn_select.setCaption( tr( "Select a patient for this appointment") );
//             patn_row = patn_select.exec();
//             if ( patn_row >= 0 )
//             {
//                 patn__sequence = reln_patn->attributeValue( patn_row, "patn__sequence" );
//                 return patn__sequence.toInt();
//             } else
//                     // escape from select list
//                 return -2;
//         } else {
//                 // no existing patient
//             QMessageBox::information( this, "Patient not found",
//  				      tr("<h2>This patient was not found </h2>"
//                                          "<ul><li>Surname: <b>%1</b>"
//                                          "<li>Firstname: <b>%2</b>"
//                                          "<li>DOB: <b>%3</b>"
//                                          "<li>Street: <b>%4</b>"
//                                          "<li>Suburb: <b>%5 %6</b>"
//                                          "<li>Phone: <b>%7></b></ul>")
//                                       .arg( patn_psnam )
//                                       .arg( patn_fsnam )
//                                       .arg( patn_dob )
//                                       .arg( patn_address )
//                                       .arg( patn_suburb )
//                                       .arg( patn_postcode )
//                                       .arg( patn_phone ),
//  				      "OK", 0 );
//                 return -2;
//         }
        
        
//     } // end of block for existing patient
    
//         // --------------------------------------------------
//         // Still no patn -  insert a new patient
//         // --------------------------------------------------
    
//         // New tuple
//     patn_row = reln_patn->newTuple( TRUE );
//     if ( patn_row < 0 )
//     {
            
//         QMessageBox::information( this, "The patient record not saved",
//                                   tr("The new patient record could not be saved"),
//                                   "OK", 0 );
//         return -2;
//     }
//         // Transfer the data
//     reln_patn->update( patn_row, "patn_psnam", patn_psnam);
//     reln_patn->update( patn_row, "patn_fsnam", patn_fsnam);
//     reln_patn->update( patn_row, "patn_dob", patn_dob );
//     reln_patn->update( patn_row, "patn_address", patn_address );
//     reln_patn->update( patn_row, "patn_suburb", patn_suburb );
//     reln_patn->update( patn_row, "patn_postcode", patn_postcode );
//     reln_patn->update( patn_row, "patn_phone", patn_phone );
    
//     if ( reln_patn->save() )
//         patn__sequence = reln_patn->attributeValue( patn_row, "patn__sequence" );
//     else
//     {
//         QMessageBox::information( this, "The patient record not saved",
//                                   tr("The new patient record could not be saved"),
//                                   "OK", 0 );
//         reln_patn->remove( patn_row, TRUE );
//         return -2;
//     }

//         // return the new patient
//     return patn__sequence.toInt();
    return 0;
}
