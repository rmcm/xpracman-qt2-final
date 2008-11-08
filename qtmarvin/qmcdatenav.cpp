/* -*- mode: C++ -*-
 *
 *  * Id: qmcdatenav.cpp,v 1.9 1999/11/19 19:41:25 michael Exp
 *
 * Copyright (c) 1998 - 2000, Michael van der Westhuizen.
 * All rights reserved.
 *
 * See LICENSE.BSD for details - enclosed below
 *
 * -------------------------------------------------------------------------------
 *  Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     o Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 * 
 *     o Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 * 
 *     o Neither name of the Qmc::Widgets Project nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * -------------------------------------------------------------------------------
 *
 * If this file is being built with or linked against any part of the
 * K Desktop Environment, KOffice Office Suite, KDE Libraries or KDE core
 * applications, the licensing model that may, at the developers
 * discretion, apply is LGPL.
 */
#include <qpixmap.h>
#include <qpainter.h>
#include <qrect.h>
#include <qpopupmenu.h>
#include <qcursor.h>

#include "qmcdatenav.h"


struct QmcDate
{
    QDate d;	// Date
    QRect r;	// Rect
    bool s;	// Selected
    bool t;	// Today
};

QList<QmcDate> dateList;	// All visible days

QList<QDate> *eventDayList;	// Only ever a pointer to a list
QList<QDate> *nonWorkingDayList;// Ditto

bool doOutlook; // Are we to paint in outlook compatible mode?

// All QRects below are for finding where a click really was.
QRect captionRect;
QRect leftArrowRect;
QRect rightArrowRect;


QRect sunBR;
QRect monBR;
QRect tueBR;
QRect wedBR;
QRect thuBR;
QRect friBR;
QRect satBR;

QRect daysRect;


QmcDateNav::QmcDateNav( class QWidget * parent, const char * name )
    : QWidget( parent, name )
{
    currentMonth = QDate( QDate::currentDate() );
    init();
}


QmcDateNav::QmcDateNav( const class QDate & d, class QWidget * parent, const char * name )
    : QWidget( parent, name )
{
    currentMonth = QDate( d );
    if( !currentMonth.isValid() )
    {
	currentMonth = QDate( QDate::currentDate() );
    }
    init();
}


QmcDateNav::~QmcDateNav()
{
    if( pm )
	delete pm;
    if( sevenMonthPopup )
	delete sevenMonthPopup;
    if( rbPopup )
	delete rbPopup;
    dateList.setAutoDelete( true );	// Ensures the next line works
    dateList.clear();			// will delete all QmcDate objects

    // Do NOT delete these - that's the job of the app programmer
    eventDayList = 0;
    nonWorkingDayList = 0;
}


void QmcDateNav::init()
{
    sevenMonthPopup = new QPopupMenu( this, "sevenMonthPopup" );
    connect( sevenMonthPopup, SIGNAL( activated( int ) ), this, SLOT( monthMenuClicked( int ) ) );

    rbPopup = new QPopupMenu( this, "rbPopup" );
    connect( rbPopup, SIGNAL( activated( int ) ), this, SLOT( rbMenuClicked( int ) ) );

    dateList.setAutoDelete( true );
    dateList.clear();

    // Initialise to 0 to prevent crashes
    eventDayList = 0;
    nonWorkingDayList = 0;

    pmDirty = true;
    doFrame = true;
    doOutlook = false;
    pm = 0;

    setMouseTracking( true ); // So we can do the cursor changes properly
    setFocusPolicy( QWidget::NoFocus ); // Trust me...
}


QSizePolicy QmcDateNav::sizePolicy() const
{
    return QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed, false );
}


QSize QmcDateNav::sizeHint() const
{
    return minimumSizeHint();
}


QSize QmcDateNav::minimumSizeHint() const
{
    int w, w1, w2;
    w1 = w2 = w = 0;
    int h = 0;
    int fw = frame() ? 1 : 0;

    w1 = ( ( fontMetrics().width( "00" ) + 6 ) * 7 ) + 12 + ( fw * 2 );
    w2 = fontMetrics().width( tr( "SEPTEMBER" ) ) + fontMetrics().width( " 0000" ) + 12 + 6 + ( fw * 2 );

    w = w1 > w2 ? w1 : w2;

    h = fw + 4 + fontMetrics().height() + 4 + fontMetrics().boundingRect( QChar( 'S' ) ).height() + 3 + 1;
    h += ( ( fontMetrics().boundingRect( QChar( '0' ) ).height() ) * 6 ) + 30 + 8 + fw + 1;

    return QSize( w, h );
}


void QmcDateNav::paintEvent( QPaintEvent * )
{
    if( pmDirty || pm == 0 )
	makePixmap();
    QPainter p;

    p.begin( pm, this );
    if( pmDirty && pm != 0 )
    {
	p.fillRect( 0, 0, width(), height(), colorGroup().base() );
	if( frame() )
	    drawFrame( p );
	drawCaption( p );
	drawDays( p );
	drawDates( p );
    }
    p.end();

    p.begin( this );
    p.drawPixmap( 0, 0, *pm );
    p.end();
    pmDirty = false;

    drawSelections();

    // The 'today' indicator - a red box around todays date.
    if( currentMonth.month() == QDate::currentDate().month() )
    {
	for( int i = 0; i <= 41; i++ )
	{
	    if( dateList.at( i )->t )
	    {
		drawToday( i );
		break;
	    }
	}
    }

    // If we set this in init(), the widget is black before the first paint event, which is not cool.
    // Once we've done the initial paint, this will reduce flicker.
    setBackgroundMode( NoBackground );
}


void QmcDateNav::makePixmap()
{
    if( !pmDirty )
	return;

    if( pm != 0 )
	delete pm;

    pm = new QPixmap( width(), height() );
}


void QmcDateNav::setFrame( bool f )
{
    if( f == doFrame )
	return;
    doFrame = f;
    updateGeometry();
}


bool QmcDateNav::frame() const
{
    return doFrame;
}


void QmcDateNav::drawArrows( QPainter & p )
{
    QRect * tbr = drawCaptionText( p );
    int topBase = ( ( tbr->bottom() + 2 ) - ( frame() ? 1 : 0 ) ) / 2;
    delete tbr;
    tbr = 0;
    int leftBase = 4 + ( frame() ? 1 : 0 );
    captionRect.setLeft( leftBase + 1 + 4 + 2 );
    leftArrowRect.setLeft( leftBase );
    leftArrowRect.setRight( leftBase + 1 + 4 + 1 );
    leftArrowRect.setTop( topBase + 1 - 4 - 1 );
    leftArrowRect.setBottom( topBase + 1 + 4 + 1 );

    p.setPen( colorGroup().text() );

    // left triangle - outline
    p.drawLine( leftBase + 1, topBase + 1, leftBase + 1 + 4, topBase + 1 - 4 );
    p.drawLine( leftBase + 1, topBase + 1, leftBase + 1 + 4, topBase + 1 + 4 );
    p.drawLine( leftBase + 1 + 4, topBase + 1 - 4, leftBase + 1 + 4, topBase + 1 + 4 );

    // left triangle - fill
    p.drawLine( leftBase + 1 + 3, topBase + 1 - 3, leftBase + 1 + 3, topBase + 1 + 3 );
    p.drawLine( leftBase + 1 + 2, topBase + 1 - 2, leftBase + 1 + 2, topBase + 1 + 2 );
    p.drawLine( leftBase + 1 + 1, topBase + 1 - 1, leftBase + 1 + 1, topBase + 1 + 1 );

    // adjust for right hand triangle
    leftBase = width() - ( frame() ? 1 : 0 ) - 2 - 4;
    captionRect.setRight( leftBase - 1 - 4 - 1 );
    rightArrowRect.setLeft( leftBase - 1 - 4 );
    rightArrowRect.setRight( leftBase + 1 );
    rightArrowRect.setTop( topBase + 1 - 4 - 1 );
    rightArrowRect.setBottom( topBase + 1 + 4 + 1 );

    // right triangle - outline
    p.drawLine( leftBase - 1, topBase + 1, leftBase - 1 - 4, topBase + 1 - 4 );
    p.drawLine( leftBase - 1, topBase + 1, leftBase - 1 - 4, topBase + 1 + 4 );
    p.drawLine( leftBase - 1 - 4, topBase + 1 - 4, leftBase - 1 - 4, topBase + 1 + 4 );

    // right triangle - fill
    p.drawLine( leftBase - 1 - 3, topBase + 1 - 3, leftBase - 1 - 3, topBase + 1 + 3 );
    p.drawLine( leftBase - 1 - 2, topBase + 1 - 2, leftBase - 1 - 2, topBase + 1 + 2 );
    p.drawLine( leftBase - 1 - 1, topBase + 1 - 1, leftBase - 1 - 1, topBase + 1 + 1 );
}


QRect * QmcDateNav::drawCaptionText( QPainter & p )
{
    QRect br( 0, 3, width(), height() );
    p.setPen( colorGroup().text() );
    QString cText = monthName( currentMonth.month() );
    cText += " ";
    cText += QString::number( currentMonth.year(), 10 );
    p.drawText( br, AlignTop | AlignHCenter, cText, -1, &br );
    captionRect.setTop( br.top() );
    captionRect.setBottom( br.bottom() );
    return new QRect( br );
}


void QmcDateNav::drawCaption( QPainter & p )
{
    QRect * tbr = drawCaptionText( p );
    int fw = frame() ? 1 : 0;
    p.fillRect( fw, fw, width() - ( fw * 2 ), tbr->bottom() + 2 + ( frame() ? 0 : 1 ), colorGroup().background() );
    drawCaptionText( p );
    p.setPen( colorGroup().dark() );
    p.drawLine( fw, tbr->bottom() + 3, width() - fw - 1, tbr->bottom() + 3 );
    if( frame() )
	p.drawLine( width() - fw - 1, tbr->bottom() + 3, width() - fw - 1, fw );
    delete tbr;
    tbr = 0;
    drawArrows( p );
}


void QmcDateNav::drawFrame( QPainter & p )
{
    if( !doFrame )
	return;
    p.setPen( colorGroup().dark() );
    p.drawLine( 0, 0, width() - 2, 0 );
    p.drawLine( 0, 0, 0, height() - 2 );
}


void QmcDateNav::drawDays( QPainter & p )
{
    int dayTop = drawCaptionText( p )->bottom() + 4;
    int colWidth = p.fontMetrics().width( "00" ) + 6;
    int tableWidth = ( colWidth * 7 );
    int leftBase = ( width() - tableWidth ) / 2;

    sunBR = QRect( leftBase, dayTop, p.fontMetrics().width( "00" ) + 6, p.fontMetrics().height() );
    monBR = QRect( sunBR.right() + 1, dayTop, sunBR.width(), sunBR.height() );
    tueBR = QRect( monBR.right() + 1, dayTop, sunBR.width(), sunBR.height() );
    wedBR = QRect( tueBR.right() + 1, dayTop, sunBR.width(), sunBR.height() );
    thuBR = QRect( wedBR.right() + 1, dayTop, sunBR.width(), sunBR.height() );
    friBR = QRect( thuBR.right() + 1, dayTop, sunBR.width(), sunBR.height() );
    satBR = QRect( friBR.right() + 1, dayTop, sunBR.width(), sunBR.height() );
    QRect tSatBR;

    p.setPen( colorGroup().text() );

    p.drawText( sunBR, AlignTop | AlignHCenter, QString( tr( "Sunday"    )[0] ) );
    p.drawText( monBR, AlignTop | AlignHCenter, QString( tr( "Monday"    )[0] ) );
    p.drawText( tueBR, AlignTop | AlignHCenter, QString( tr( "Tuesday"   )[0] ) );
    p.drawText( wedBR, AlignTop | AlignHCenter, QString( tr( "Wednesday" )[0] ) );
    p.drawText( thuBR, AlignTop | AlignHCenter, QString( tr( "Thursday"  )[0] ) );
    p.drawText( friBR, AlignTop | AlignHCenter, QString( tr( "Friday"    )[0] ) );
    p.drawText( satBR, AlignTop | AlignHCenter, QString( tr( "Saturday"  )[0] ), -1, &tSatBR );

    p.setPen( colorGroup().mid() );

    p.drawLine( leftBase, tSatBR.bottom() + 1, leftBase + tableWidth, tSatBR.bottom() + 1 );
    daysRect = QRect( sunBR.left(), sunBR.top(), satBR.right() - sunBR.left(), sunBR.height() );
}


void QmcDateNav::drawDates( QPainter & p )
{
    QDate *previousMonth;
    QDate *nextMonth;
    dateList.clear();

    // We need to find out what day the first of this month was.
    QDate *fdom = new QDate( currentMonth.year(), currentMonth.month(), 1 );

    /* That value is assigned to the start position of our matrix.
       The problem here is that Qt returns monday as 1, not 0, and Sunday
       as 7! We count our matrix from 0, so we just turn a 7 into 0 */
    int matrixStartPos = fdom->dayOfWeek();

    if( matrixStartPos == 7 )
	matrixStartPos = 0;

    /* If we are starting the month before thursday, we give the previous month
       a little more space. */
    if( matrixStartPos < 4 )
	matrixStartPos = matrixStartPos + 7;

    if( !( currentMonth.month() == 1 ) )
	previousMonth = new QDate( currentMonth.year(), currentMonth.month() - 1, 1 );
    else
	previousMonth = new QDate( currentMonth.year() - 1, 12, 1 );

    if( !( currentMonth.month() == 12 ) )
        nextMonth = new QDate( currentMonth.year(), currentMonth.month() + 1, 1 );
    else
	nextMonth = new QDate( currentMonth.year() + 1, 1, 1 );

    int leftBase = 0;
    int topBase = 0;
    int dayCounter = 1;
    int nextMonthDayCounter = 1;
    QDate *insertedDate;
    int col = 0;
    int row = 0;

    for( int i = 0; i <= 41; i++ )
    {
	if( !matrixStartPos == 0 )
	{
	    //insertedDate = new QDate( QDate::QDate( previousMonth->year(), previousMonth->month(), previousMonth->daysInMonth() - matrixStartPos + 1 ) );
	    insertedDate = new QDate( previousMonth->year(), previousMonth->month(), previousMonth->daysInMonth() - matrixStartPos + 1 );
	    matrixStartPos--;
	}
	else
	{
	    if( !( dayCounter == ( currentMonth.daysInMonth() + 1 ) ) )
	    {
	        //insertedDate = new QDate( QDate::QDate( currentMonth.year(), currentMonth.month(), dayCounter ) );
		insertedDate = new QDate( currentMonth.year(), currentMonth.month(), dayCounter );
		dayCounter++;
	    }
	    else
	    {
	        //insertedDate = new QDate( QDate::QDate( nextMonth->year(), nextMonth->month(), nextMonthDayCounter ) );
		insertedDate = new QDate( nextMonth->year(), nextMonth->month(), nextMonthDayCounter );
		nextMonthDayCounter++;
	    }
	}

	if( col == 0 )
	    leftBase = ( frame() ? 1 : 0 ) + 6;
	else
	    leftBase = dateList.last()->r.right() + 1;

	if( row == 0 )
	    topBase = ( frame() ? 1 : 0 ) + 4 + fontMetrics().height() + 4 + fontMetrics().boundingRect( QChar( 'S' ) ).height() + 3 + 1 + 3;
	else
	{
	    topBase = ( frame() ? 1 : 0 ) + 4 + fontMetrics().height() + 4 + fontMetrics().boundingRect( QChar( 'S' ) ).height() + 3 + 1 + 3;
	    topBase += ( ( ( fontMetrics().boundingRect( QChar( '0' ) ).height() ) + 6 ) * row );
	}
	
	if( insertedDate->month() != currentMonth.month() )
	    p.setPen( colorGroup().mid() );
	else
	    p.setPen( colorGroup().text() );
	
	bool he = hasEvent( *insertedDate );
	bool nwd = isNonWorkingDay( *insertedDate );
	QFont nFont = p.font();
	QFont oFont = p.font();
	if( he )
	    nFont.setBold( true );
	if( nwd )
	    nFont.setItalic( true );
	p.setFont( nFont );
	p.drawText( leftBase, topBase, fontMetrics().width( "00" ) + 6, fontMetrics().boundingRect( QChar( '0' ) ).height() + 6, AlignCenter, QString::number( insertedDate->day(), 10 ) );

	if( he || nwd )
	    p.setFont( oFont );
	
	QmcDate * d = new QmcDate;
	d->d = QDate( insertedDate->year(), insertedDate->month(), insertedDate->day() );
	d->r = QRect( leftBase, topBase, fontMetrics().width( "00" ) + 6, fontMetrics().boundingRect( QChar( '0' ) ).height() + 6 );
	d->s = false;
	d->t = false;
	if( d->d == QDate::currentDate() )
	    d->t = true;
	if( d->d == currentMonth )
	    d->s = true;

	dateList.append( d );

	if( col != 6 )
	    col++;
	else if( col == 6 )
	{
	    col = 0;
	    row++;
	}
    }

    if( previousMonth )
	delete previousMonth;
    if( nextMonth )
	delete nextMonth;
    if( fdom )
	delete fdom;
    if( insertedDate )
	delete insertedDate;
    previousMonth = nextMonth = fdom = insertedDate =  0;
}


void QmcDateNav::mousePressEvent( QMouseEvent * e )
{
    if( e->button() == RightButton )
    {
	prepRBPopup();
	rbPopup->exec( QCursor::pos() );
    }
    if( e->button() != LeftButton )
	return;
    
    // Mouse Rect
    QRect mr( e->x(), e->y(), 1, 1 );

    if( captionRect.contains( mr, true ) )
    {
	prepPopup();
	QPoint pa = mapToGlobal( QPoint( captionRect.left(), captionRect.bottom() + 4 ) );
	sevenMonthPopup->exec( pa, 4 );
	return;
    }

    if( leftArrowRect.contains( mr, true ) )
    {
	changeMonth( false, 1 );
    }

    if( rightArrowRect.contains( mr, true ) )
    {
	changeMonth( true, 1 );
    }


    if( daysRect.contains( mr, true ) )
    {
	int dayToSelect = 0;
	if( sunBR.contains( mr, true ) )
	    dayToSelect = 7;
	if( monBR.contains( mr, true ) )
	    dayToSelect = 1;
	if( tueBR.contains( mr, true ) )
	    dayToSelect = 2;
	if( wedBR.contains( mr, true ) )
	    dayToSelect = 3;
	if( thuBR.contains( mr, true ) )
	    dayToSelect = 4;
	if( friBR.contains( mr, true ) )
	    dayToSelect = 5;
	if( satBR.contains( mr, true ) )
	    dayToSelect = 6;
	if( dayToSelect == 0 )
	    return;
	if( e->state() != ControlButton )
	    clear();
	for( int i = 0; i <= 41; i++ )
	    if( dateList.at( i )->d.dayOfWeek() == dayToSelect )
		dateList.at( i )->s = true;
	drawSelections();
        emit clicked();
	emit dateChanged();
	return;
    }


    QRect weeksRect( frame() ? 1 : 0,
		     dateList.at( 0 )->r.top(),
		     dateList.at( 0 )->r.left() - ( frame() ? 1 : 0 ),
		     dateList.at( 41 )->r.bottom() - dateList.at( 0 )->r.top() );

    if( weeksRect.contains( mr, true ) )
    {
	if( e->state() != ControlButton )
	    clear();
	int i = -1;
	if( e->y() > dateList.at( 35 )->r.top() )
	    for( i = 35; i <= 41; i++ )
		dateList.at( i )->s = true;
	else if( e->y() > dateList.at( 28 )->r.top() )
	    for( i = 28; i <= 34; i++ )
		dateList.at( i )->s = true;
	else if( e->y() > dateList.at( 21 )->r.top() )
	    for( i = 21; i <= 27; i++ )
		dateList.at( i )->s = true;
	else if( e->y() > dateList.at( 14 )->r.top() )
	    for( i = 14; i <= 20; i++ )
		dateList.at( i )->s = true;
	else if( e->y() > dateList.at( 7 )->r.top() )
	    for( i = 7; i <= 13; i++ )
		dateList.at( i )->s = true;
	else if( e->y() > dateList.at( 0 )->r.top() )
	    for( i = 0; i <= 6; i++ )
		dateList.at( i )->s = true;
	drawSelections();
        emit clicked();
	emit dateChanged();
	return;
    }


    for( int r = 0; r <= 41; r++ )
    {
	if( dateList.at( r )->r.contains( mr, true ) )
	{
	    // Click outside the current month
	    if(  dateList.at( r )->d.month() != currentMonth.month() && e->state() != ControlButton && e->state() != ShiftButton )
	    {
		QDate sd( dateList.at( r )->d.year(), dateList.at( r )->d.month(), dateList.at( r )->d.day() );
		setDate( sd );
		return;
	    }

	    // MultiSelection : Control Button
	    if( e->state() & ControlButton )
	    {
		drawSelectedDate( r );
                emit clicked();
		emit dateChanged();
		return;
	    }
	    
	    // MultiSelection : Shift Button
	    if( e->state() & ShiftButton )
	    {
		// Always draw from the start of the selection
		int markStart = findIndex( *selectedDates().at( 0 ) );
		clear();
		int markEnd = r;
		int b = markStart < markEnd ? markStart : markEnd;
		int e = markStart > markEnd ? markStart : markEnd;
		for( ; b <= e; b++ )
		{
		    dateList.at( b )->s = true;
		}
		drawSelections();
		return;
	    }
	    else
	    {
		clear();
		drawSelectedDate( r );
                emit clicked();
		emit dateChanged();
		return;
	    }
	}
    }
}

// i == the index of the date
// pe == in paint event (shouldn't toggle selection)
void QmcDateNav::drawSelectedDate( int i, bool pe )
{
    if( dateList.at( i )->s && !pe )
    {
	if( numSelected() <= 1 )
	    return;
	undrawSelectedDate( i );
	dateList.at( i )->s = false;
	drawToday( i );
	return;
    }
    dateList.at( i )->s = true;
    QPainter p;
    p.begin( this );
    if( doOutlook )
	p.fillRect( dateList.at( i )->r, colorGroup().background() );
    else
	p.fillRect( dateList.at( i )->r, colorGroup().highlight() );
    p.setPen( colorGroup().highlightedText() );
    QFont pFont = p.font();
    if( hasEvent( dateList.at( i )->d ) )
	pFont.setBold( true );
    if( isNonWorkingDay( dateList.at( i )->d ) )
	pFont.setItalic( true );
    p.setFont( pFont );
    p.drawText( dateList.at( i )->r, AlignCenter, QString::number( dateList.at( i )->d.day(), 10 ) );
    p.end();
    drawToday( i );
}


void QmcDateNav::undrawSelectedDate( int i )
{
    QPainter p;
    p.begin( this );
    p.fillRect( dateList.at( i )->r, colorGroup().base() );
    p.setPen( colorGroup().mid() );
    if( dateList.at( i )->d.month() == currentMonth.month() )
	p.setPen( colorGroup().text() );
    QFont pFont = p.font();
    if( hasEvent( dateList.at( i )->d ) )
	pFont.setBold( true );
    if( isNonWorkingDay( dateList.at( i )->d ) )
	pFont.setItalic( true );
    p.setFont( pFont );
    p.drawText( dateList.at( i )->r, AlignCenter, QString::number( dateList.at( i )->d.day(), 10 ) );
    p.end();
}


int QmcDateNav::numSelected()
{
    int rv = 0;
    for( int i = 0; i <= 41; i++ )
    {
	if( dateList.at( i )->s == true )
	    rv++;
    }
    return rv;
}


void QmcDateNav::drawToday( int i )
{
    if( !dateList.at( i )->t )
	return;
    QPainter p;
    p.begin( this );
    p.setPen( QColor( 255, 0, 0 ) );
    p.drawRect( dateList.at( i )->r );
    p.end();
}


void QmcDateNav::drawSelections()
{
    for( int i = 0; i <= 41; i++ )
    {
	if( dateList.at( i )->s )
	    drawSelectedDate( i, true );
    }
}


void QmcDateNav::clear()
{
    for( int i = 0; i <= 41; i++ )
    {
	if( dateList.at( i )->s )
	{
	    undrawSelectedDate( i );
	    dateList.at( i )->s = false;
	    drawToday( i );
	}
    }
}


void QmcDateNav::changeMonth( bool forward, int steps )
{
    int cyn, cmn, cdn;
    cyn = cmn = cdn = 0;
    cmn = currentMonth.month();
    cyn = currentMonth.year();
    for( int i = 0; i <= 41; i++ )
    {
	if( dateList.at( i )->s )
	    cdn = dateList.at( i )->d.day();
    }

    if( numSelected() > 1 || numSelected() < 1 )
	cdn = 1;

    if( forward )
    {
	cmn += steps;
	if( cmn > 12 )
	{
	    cmn -= 12;
	    cyn++;
	}
    }
    else
    {
	cmn -= steps;
	if( cmn < 1 )
	{
	    cmn += 12;
	    cyn--;
	}
    }

    QDate nd( cyn, cmn, fixDays( cyn, cmn, cdn ) );

    int py = currentMonth.year();
    currentMonth = QDate( nd );
    emit dateChanged();
    if( currentMonth.year() != py )
    {
	emit yearChanged( currentMonth.year() );
    }
    pmDirty = true;
    repaint();
}


QString QmcDateNav::monthName( int mn )
{
    QString rv = QString::null;
    switch( mn )
    {
	case 1:
	{
	    rv += tr( "January" );
	    break;
	}
	case 2:
	{
	    rv += tr( "February" );
	    break;
	}
	case 3:
	{
	    rv += tr( "March" );
	    break;
	}
	case 4:
	{
	    rv += tr( "April" );
	    break;
	}
	case 5:
	{
	    rv += tr( "May" );
	    break;
	}
	case 6:
	{
	    rv += tr( "June" );
	    break;
	}
	case 7:
	{
	    rv += tr( "July" );
	    break;
	}
	case 8:
	{
	    rv += tr( "August" );
	    break;
	}
	case 9:
	{
	    rv += tr( "September" );
	    break;
	}
	case 10:
	{
	    rv += tr( "October" );
	    break;
	}
	case 11:
	{
	    rv += tr( "November" );
	    break;
	}
	case 12:
	{
	    rv += tr( "December" );
	    break;
	}
    }
    return rv;
}


int QmcDateNav::fixDays( int tmpYear, int tmpMonth, int d )
{
    int tmpDay = d;

    switch( tmpMonth )
    {
	case 2:
	{
	    if( tmpDay >= 29 )
	    {
		if( QDate::leapYear( tmpYear ) )
		    tmpDay = 29;
		else
		    tmpDay = 28;
	    }
	    break;
	}
	case 4:
	{
	    if( tmpDay == 31 )
		tmpDay = 30;
	    break;
	}
	case 6:
	{
	    if( tmpDay == 31 )
		tmpDay = 30;
	    break;
	}
	case 9:
	{
	    if( tmpDay == 31 )
		tmpDay = 30;
	    break;
	}
	case 11:
	{
	    if( tmpDay == 31 )
		tmpDay = 30;
	    break;
	}
    }
    return tmpDay;
}


void QmcDateNav::prepRBPopup()
{
    rbPopup->clear();
    rbPopup->insertItem( "Go to Today", 1 );
}


void QmcDateNav::prepPopup()
{
    QDate backThree, backTwo, backOne, forwardOne, forwardTwo, forwardThree;

    sevenMonthPopup->clear();

    int newY = currentMonth.year();
    int newM = currentMonth.month();
    int newD = 15;

    newM -= 3;
    if( newM < 1 )
    {
	newM += 12;
	newY--;
    }
    backThree = QDate( newY, newM, newD );

    newM++;
    if( newM > 12 )
    {
	newM -= 12;
	newY++;
    }
    backTwo = QDate( newY, newM, newD );

    newM++;
    if( newM > 12 )
    {
	newM -= 12;
	newY++;
    }
    backOne = QDate( newY, newM, newD );

    newM += 2;
    if( newM > 12 )
    {
	newM -= 12;
	newY++;
    }
    forwardOne = QDate( newY, newM, newD );

    newM++;
    if( newM > 12 )
    {
	newM -= 12;
	newY++;
    }
    forwardTwo = QDate( newY, newM, newD );

    newM++;
    if( newM > 12 )
    {
	newM -= 12;
	newY++;
    }
    forwardThree = QDate( newY, newM, newD );

    sevenMonthPopup->insertItem( QString( monthName( backThree.month() ) + " " + QString::number( backThree.year(), 10 ) ), 1 );
    sevenMonthPopup->insertItem( QString( monthName( backTwo.month() ) + " " + QString::number( backTwo.year(), 10 ) ), 2 );
    sevenMonthPopup->insertItem( QString( monthName( backOne.month() ) + " " + QString::number( backOne.year(), 10 ) ), 3 );
    sevenMonthPopup->insertItem( QString( monthName( currentMonth.month() ) + " " + QString::number( currentMonth.year(), 10 ) ), 4 );
    sevenMonthPopup->insertItem( QString( monthName( forwardOne.month() ) + " " + QString::number( forwardOne.year(), 10 ) ), 5 );
    sevenMonthPopup->insertItem( QString( monthName( forwardTwo.month() ) + " " + QString::number( forwardTwo.year(), 10 ) ), 6 );
    sevenMonthPopup->insertItem( QString( monthName( forwardThree.month() ) + " " + QString::number( forwardThree.year(), 10 ) ), 7 );
    sevenMonthPopup->setFixedWidth( captionRect.width() );
}


void QmcDateNav::monthMenuClicked( int id )
{
    if( id < 4 && id > 0 )
    {
	if( id == 1 ) id = 3;
	else if( id == 3 ) id = 1;
	changeMonth( false, id );
    }
    else if( id > 4 && id < 8 )
    {
	changeMonth( true, id - 4 );
    }
}


QList<QDate> QmcDateNav::selectedDates() const
{
    QList<QDate> rl;
    rl.clear();
    for( int i = 0; i <= 41; i++ )
	if( dateList.at( i )->s ) rl.append( new QDate( dateList.at( i )->d ) );
    return rl;
}


bool QmcDateNav::setDate( const QDate d )
{
    if( !d.isValid() )
    {
	return false;
    }
    int py = currentMonth.year();
    currentMonth = QDate( d );
    pmDirty = true;
    repaint();
    emit dateChanged();
    if( currentMonth.year() != py )
    {
	emit yearChanged( currentMonth.year() );
    }
    return true;
}


bool QmcDateNav::hasEvent( const QDate & d )
{
    if( eventDayList == 0 )
	return false;
    QDate * ld;
    for( ld = eventDayList->first(); ld != 0; ld = eventDayList->next() )
    {
	if( d.year() == ld->year() && d.month() == ld->month() && d.day() == ld->day() )
	    return true;
    }
    return false;
}


bool QmcDateNav::isNonWorkingDay( const QDate & d )
{
    if( nonWorkingDayList == 0 )
	return false;
    QDate * ld;
    for( ld = nonWorkingDayList->first(); ld != 0; ld = nonWorkingDayList->next() )
    {
	if( d.year() == ld->year() && d.month() == ld->month() && d.day() == ld->day() )
	    return true;
    }
    return false;
}


bool QmcDateNav::outlook() const
{
    return doOutlook;
}


void QmcDateNav::setOutlook( bool on )
{
    if( on == doOutlook )
	return;
    doOutlook = on;
}


void QmcDateNav::installEventDayList( QList<QDate> *n )
{
    eventDayList = n;
}


void QmcDateNav::removeEventDayList()
{
    eventDayList = 0;
}


void QmcDateNav::installNonWorkingDayList( QList<QDate> *n )
{
    nonWorkingDayList = n;
}


void QmcDateNav::removeNonWorkingDayList()
{
    nonWorkingDayList = 0;
}


void QmcDateNav::forceUpdate()
{
    pmDirty = true;
    QWidget::update();
}


// Returns the index of the passed rect, or -1 on failure.
int QmcDateNav::findIndex( QRect & ir )
{
    for( int i = 0; i <= 41; i++ )
    {
	if( dateList.at( i )->r == ir )
	    return i;
    }
    return -1;
}



// Returns the index of the passed rect, or -1 on failure.
int QmcDateNav::findIndex( QDate & id )
{
    for( int i = 0; i <= 41; i++ )
    {
	if( dateList.at( i )->d == id )
	    return i;
    }
    return -1;
}


// Handles cursor changing for mouse move events.
// TODO: Correct pointer cursors for this stuff.
// Notes: Cursors are system specific see $QTDIR/src/kernel/qcursor_x11.cpp
// 	  I need to figure out how to get the system specific cursor, and
//	  modify it for my use.
void QmcDateNav::mouseMoveEvent( QMouseEvent * e )
{
    QRect mr( e->x(), e->y(), 1, 1 );

    if( daysRect.contains( mr, true ) )
    {
	setCursor( pointingHandCursor );
	return;
    }

    QRect weeksRect( frame() ? 1 : 0,
		     dateList.at( 0 )->r.top(),
		     dateList.at( 0 )->r.left() - ( frame() ? 1 : 0 ),
		     dateList.at( 41 )->r.bottom() - dateList.at( 0 )->r.top() );

    if( weeksRect.contains( mr, true ) )
    {
	setCursor( pointingHandCursor );
	return;
    }

    unsetCursor();
}


// Callback for an item on the right-mouse buttton menu being clicked.
void QmcDateNav::rbMenuClicked( int id )
{
    if( id == 1 )
	setDate( QDate::currentDate() );
}

//////////////////////////////////////////////////////////////////////////
// Class Docs
/////////////////////////////////////////////////////////////////////////

/*!
 * \file qmcdatenav.h
 * \brief Date Navigator (Declarations)
 *
 * \author Michael van der Westhuizen
 */
/*!
 * \file qmcdatenav.cpp
 * \brief Date Navigator (Definitions)
 *
 * \author Michael van der Westhuizen
 * \sa QmcDateNav
 */
/*!
 * \class QmcDateNav qmcdatenav.h
 * \brief Date Navigation Widget.
 *
 * The QmcDateNav widget is visually similar to the calendar navigation
 * widgets found in many popular PIMs.
 *
 * Currently the widget is quite simple, with constructors to create a widget
 * with either the current date selected or a specific date selected, and
 * methods to retrieve a list of all dates selected or set the selected date.
 *
 * A signal is emitted whenever the selection changes so that you can retrieve
 * the selection list and your application can react to that data.
 *
 * <b>Non-Working Days and Event Days</b>
 *
 * A PIM application generally displays days that have events or are non-working days (such as weekends) in either
 * a different font or colour.
 *
 * %QmcDateNav paints non-working days with an italic font face, and days containing event with a bold font face.
 *
 * Event days and non-working days are two areas that will almost definitely cause headaches and grey hairs for an application developer.
 *
 * The grey area that tends to cause confusion is the ownership and maintenance of the lists of non-working and event days. %QmcDateNav
 * assigns this responsibility solely to the application writer, and provides hooks for passing the address of the list to the widget
 * for iteration.
 *
 * The widget will, under no circumstances, attempt to modify the lists in any way whatsoever.
 *
 * The lists may be any size, and contain any number of dates, although iteration performance will (clearly) suffer on large lists.<br>
 * To avoid unusually large lists, it is recommended that you use the yearChanged() and, to a lesser extent, the dateChanged() signals
 * to alert your application to the fact that the list needs updating.
 *
 * It's a good idea to force a widget repaint using the forceUpdate() method if you add or remove a date in your list that is
 * in the currently visible month.
 *
 * \warning Unexpected behaviour will result if the lists are deleted behind the widgets back.<br>
 * The correct way to go about removing a list is to use the supplied remove method before using delete on the list.
 *
 * See the test application source for the recommended usage of the list manipulation methods.
 *
 * \sa installEventDayList()
 * \sa removeEventDayList()
 * \sa installNonWorkingDayList()
 * \sa removeNonWorkingDayList()
 *
 * \author Michael van der Westhuizen (michael@mudpuppies.co.za).
 * \date $Date: 2006-06-11 21:47:27 +1000 (Sun, 11 Jun 2006) $
 */
/*!
 * \fn QmcDateNav::QmcDateNav( class QWidget * parent = 0, const char * name = 0 )
 * \brief Default Constructor.
 *
 * This constructor creates a date navigation widget with todays date selected.
 *
 * The \c parent and \c name arguments are passed to the %QWidget constructor.
 */
/*!
 * \fn QmcDateNav::QmcDateNav( const class QDate & d, class QWidget * parent = 0, const char * name = 0 )
 * \brief Alternate Constructor.
 *
 * This constructor creates a date navigation widget with the date specified by \c d selected.
 *
 * If \c d is an invalid date, todays date is selected.
 *
 * The \c parent and \c name arguments are passed to the %QWidget constructor.
 *
 * \param d The date on which to base this widget.
 * \sa setDate()
 */
/*!
 * \fn QmcDateNav::~QmcDateNav()
 * \brief Destructor
 *
 * Destroys the widget, freeing allocated memory and emptying lists.
 *
 */
/*!
 * \fn QSizePolicy QmcDateNav::sizePolicy() const
 *
 * \return A %QSizePolicy of QSizePolicy::Fixed by QSizePolicy::Fixed with no height-for-width capabilities.
 */
/*!
 * \fn QSize QmcDateNav::sizeHint() const
 *
 * \return minimumSizeHint().
 */
/*!
 * \fn QSize QmcDateNav::minimumSizeHint() const
 *
 * \return The amount of space this widget requires to be useful.
 */
/*!
 * \fn bool QmcDateNav::frame() const
 *
 * \return True if this widget is drawn with a frame, false otherwise.
 * \sa setFrame()
 */
/*!
 * \fn void QmcDateNav::setFrame( bool f )
 *
 * Toggles the drawing of a frame.
 *
 * \param f Set to true to enable the frame, false to disable.
 *
 * The default is to draw a frame.
 *
 * \sa frame()
 */
/*!
 * \fn QList<QDate> QmcDateNav::selectedDates() const
 *
 * \return A QList containing all of the selected dates in the navigator.
 *
 */
/*!
 * \fn void QmcDateNav::dateChanged()
 *
 * This signal is emitted whenever the selection changes.
 */
/*!
 * \fn bool QmcDateNav::outlook() const
 *
 * \return True if this widget is in Outlook mode, false otherwise.
 * \sa setOutlook()
 */
/*!
 * \fn void QmcDateNav::setOutlook( bool on )
 *
 * The QmcDateNav widget can paint itself in a way similar to Microsoft Outlook or
 * using highlight colours from the current %Qt style.
 *
 * The default is to use standard %Qt colours.
 *
 * \param on Pass true if you wish the widget to be painted in Outlook mode, false if you want standard %Qt colours.
 * \sa outlook()
 */
/*!
 * \fn bool QmcDateNav::setDate( const QDate d )
 *
 * If the new date is valid the date is changed to that date.
 *
 * \param d The date to change to.
 * \return True if the date is changed, false otherwise.
 */
/*!
 * \fn QmcDateNav::yearChanged( int )
 *
 * Emitted whenever the current year changes.
 */
/*!
 * \fn void QmcDateNav::paintEvent( QPaintEvent * )
 *
 * \reimp
 */
/*!
 * \fn void QmcDateNav::mousePressEvent( QMouseEvent * )
 *
 * \reimp
 */
/*!
 * \fn void QmcDateNav::installEventDayList( QList<QDate> *n )
 *
 * Assigns the address of the list \c n to be iterated over when searching for dates with events.
 *
 * \param n The new list address to assign to the internal event day list pointer.
 * \warning This is the one area you can really break your application. Always call removeEventDayList() before using delete on your list.<br>
 * If you don't, you may find the internal pointer pointing to deallocated memory and crashing your application.
 *
 * \sa removeEventDayList()
 */
/*!
 * \fn void QmcDateNav::removeEventDayList()
 *
 * Sets the internal event day list pointer to 0 and prevents any attempts at list iteration.
 *
 * Always call this before using the delete operator on your list.
 *
 * \sa installEventDayList()
 */
/*!
 * \fn void QmcDateNav::installNonWorkingDayList( QList<QDate> *n )
 *
 * Assigns the address of the list \c n to be iterated over when searching for dates that are non-working days.
 *
 * \param n The new list address to assign to the internal non-working day list pointer.
 * \warning This is the one area you can really break your application. Always call removeNonWorkingDayList() before using delete on your list.<br>
 * If you don't, you may find the internal pointer pointing to deallocated memory and crashing your application.
 *
 * \sa removeNonWorkingDayList()
 */
/*!
 * \fn void QmcDateNav::removeNonWorkingDayList()
 *
 * Sets the internal non-working day list pointer to 0 and prevents any attempts at list iteration.
 *
 * Always call this before using the delete operator on your list.
 *
 * \sa installNonWorkingDayList()
 */
/*!
 * \fn void QmcDateNav::forceUpdate()
 *
 * Ensures that the entire widget is repainted by setting the offscreen buffer state flag to dirty and calling QWidget::update().
 */
/*!
 * \fn void QmcDateNav::mouseMoveEvent( QMouseEvent * )
 *
 * \reimp
 */





