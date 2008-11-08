/*
 *     $Id: qmvtswidget.cpp 2889 2006-06-11 11:47:27Z source $
 * 
 *   Copyright 2003 X=X Computer Software Trust
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
#include <qlayout.h>
#include <qpushbutton.h>
#include <qwhatsthis.h>
#include <qtooltip.h>
#include <qaccel.h>
#include <qlist.h>
#include <qregexp.h>
#include "qmvtswidget.h"
#include "qmvpopup.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmvtswidget_version[] = "$Id: qmvtswidget.cpp 2889 2006-06-11 11:47:27Z source $";
// #define QMVTSWIDGET_DEBUG

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

QmvTSWidget::QmvTSWidget( QWidget * parent, const char * name, uint mode )
        : QWidget( parent, name)
{
    current_dt = QDateTime();

    use_mode = mode;
    
        // Top Level layout for this widget
    QHBoxLayout * layout_top =  new QHBoxLayout( this, 0, -1, "layout_top" );

        // Calendar popup
    dl_datenav = new QDialog( this, "date nav dialog", FALSE);
    dl_datenav->setCaption(tr("Calendar"));
    QVBoxLayout * vl_datenav = new QVBoxLayout( dl_datenav, 1 );
    date_nav = new QmcDateNav( dl_datenav, "Date Navigator" );
    vl_datenav->addWidget(date_nav);
    dl_datenav->setFixedWidth( date_nav->width() );
    dl_datenav->setFixedHeight( date_nav->height() );
    connect( date_nav, SIGNAL( dateChanged() ), this, SLOT( setDateByCalendar() ) );
    connect( date_nav, SIGNAL( clicked() ), dl_datenav, SLOT( hide() ) );
    QAccel * ac_datenav = new QAccel( dl_datenav );
    ac_datenav->connectItem( ac_datenav->insertItem( CTRL+Key_W ), dl_datenav, SLOT(close()) );
    ac_datenav->connectItem( ac_datenav->insertItem( Key_Escape ), dl_datenav, SLOT(close()) );    

        // calendar button
    QPushButton * button = new QPushButton( "", this );
    layout_top->addWidget( button );
    button->setFixedWidth( 10 );
    QWhatsThis::add( button, "Popup a date navigator for selecting a valid date");
    QToolTip::add( button, "Date Navigator" );
    connect( button, SIGNAL( clicked() ), this, SLOT( showCalendar() ) );

        // date widget and mode-set.
    date_edit = new QLineEdit( this, "date editor" );
    layout_top->addWidget( date_edit );
    connect( date_edit, SIGNAL( returnPressed() ), this, SLOT( setDate() ) );
    if ( ! (use_mode & DateMode) )
    {
        date_edit->hide();
        button->hide();
    }
        // No direct text entry - only via datenav
    if ( use_mode & DateNoText )
    {
        date_edit->setReadOnly( TRUE );
        date_edit->setFrame( FALSE );
    }

        // time widget and mode-set
    time_edit = new QLineEdit( this, "time editor" );
    layout_top->addWidget( time_edit );
    connect( time_edit, SIGNAL( returnPressed() ), this, SLOT( setTime() ) );
    if ( ! (use_mode & TimeMode) )
        time_edit->hide();
    
    adjustSize();
    updateGeometry();
    show();
    
}

// Destructor

QmvTSWidget::~QmvTSWidget()
{
  
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
QDateTime QmvTSWidget::getDateTime() const
{
    return current_dt;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTSWidget::setDate( const QDate & date )
{
    if ( date.isValid() )
        date_edit->setText( QString( "%1/%2/%3" )
                            .arg( date.day() )
                            .arg( date.month() )
                            .arg( date.year() ) );
    else
        date_edit->setText( "" );
    
    current_dt.setDate( date );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTSWidget::setDate( const QString & date_str )
{
    QDate date_part = QDate();

    date_str.stripWhiteSpace();
    date_str.simplifyWhiteSpace();
    if ( date_str.length() < 1 )
    {
        setDate(date_part);
        return;
    }

        // some common aliases
    if ( date_str.contains( QRegExp( "^today$", FALSE, FALSE ) ) )
    {
        setDate( QDate::currentDate() );
        return;
    }
    if ( date_str.contains( QRegExp( "^now$", FALSE, FALSE ) ) )
    {
        setDate( QDate::currentDate() );
        setTime( QTime::currentTime() );
        return;
    }
    if ( date_str.contains( QRegExp( "^yesterday$", FALSE, FALSE ) ) )
    {
        setDate( QDate::currentDate().addDays(-1) );
        return;
    }
    if ( date_str.contains( QRegExp( "^tomorrow$", FALSE, FALSE ) ) )
    {
        setDate( QDate::currentDate().addDays(1) );
        return;
    }
    
        // expecting 27/02/2003
    QStringList d_parts = QStringList::split( QRegExp( "[/ -,]" ), date_str );
    if ( d_parts.count() == 3 )
        date_part = QDate( d_parts[2].toInt(), d_parts[1].toInt(), d_parts[0].toInt() );

    if ( date_part.isValid() )
        setDate(date_part);
    else {
        new QmvPopup( QString("<b>%1 :</b><br>Invalid date format").arg(date_str),
                      QCursor::pos(),  this, "error in date" );
        setDate( QDate() );
    }

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTSWidget::setDate()
{
    setDate( date_edit->text() );
    if ( getDateTime().date().isValid() )
        focusNextPrevChild(TRUE);
    
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTSWidget::setTime( const QTime & time )
{
    if ( time.isValid() )
        time_edit->setText( time.toString() );
    else
        time_edit->setText( "" );
    
    current_dt.setTime( time );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTSWidget::setTime( const QString & time_str )
{
    QTime time_part = QTime();
    
    time_str.stripWhiteSpace();
    time_str.simplifyWhiteSpace();
    if ( time_str.length() < 1 )
    {
        setTime(time_part);
        return;
    }

        // some common aliases
    if ( time_str.contains( QRegExp( "^now$", FALSE, FALSE ) ) )
    {
        setTime( QTime::currentTime() );
        return;
    }
    
        // expecting 08:55[:57.00 EST]
    QStringList ts_parts = QStringList::split( QRegExp( "\\s+" ), time_str );

    if ( ts_parts.count() >= 1 )
    {
        QStringList t_parts = QStringList::split( QRegExp( ":" ), ts_parts[0] );
            // hh:mm:ss.ms
        if ( t_parts.count() == 3 )
            time_part = QTime( t_parts[0].toInt(), t_parts[1].toInt(),
                               t_parts[2].left(2).toInt(), t_parts[2].right(2).toInt() );
        else if ( t_parts.count() == 2 )
            time_part = QTime( t_parts[0].toInt(), t_parts[1].toInt(),
                               t_parts[2].left(2).toInt() );
    }
    
    if ( time_part.isValid() )
        setTime(time_part);
    else {
        new QmvPopup( QString("<b>%1: </b><br>Invalid time format").arg(time_str),
                      QCursor::pos(),  this, "error in time" );
        setTime( QTime() );
    }

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTSWidget::setTime()
{
    setTime( time_edit->text() );
    if ( getDateTime().time().isValid() )
        focusNextPrevChild(TRUE);
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTSWidget::setDateTime( const QDateTime & datetime )
{
    if ( !datetime.isValid() )
        return;

    setDate( datetime.date() );
    setTime( datetime.time() );
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTSWidget::showCalendar()
{
    dl_datenav->move( QCursor::pos() );
    dl_datenav->show();
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvTSWidget::setDateByCalendar()
{
    QList<QDate> dlist = date_nav->selectedDates();
    QDate * dt = dlist.first();
    if ( dt )
        setDate( *dt );

}
