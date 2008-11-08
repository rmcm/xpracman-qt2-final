/* -*- mode: C++ -*-
 *
 * Id: qmcdatenav.h,v 1.7 1999/11/19 19:41:25 michael Exp 
 *
 * Copyright (c) 1998 - 2000, Michael van der Westhuizen.
 * All rights reserved.
 *
 * See LICENSE.BSD for details. - enclosed below
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
 *
 * If this file is being built with or linked against any part of the
 * K Desktop Environment, KOffice Office Suite, KDE Libraries or KDE core
 * applications, the licensing model that may, at the developers
 * discretion, apply is LGPL.
 */
#ifndef QmcDATENAV_H
#define QmcDATENAV_H

#include <qwidget.h>
#include <qdatetime.h>
#include <qlist.h>

class QPopupMenu;


class QmcDateNav : public QWidget
{
    Q_OBJECT

public:
    QmcDateNav( class QWidget * parent = 0, const char * name = 0 );
    QmcDateNav( const class QDate &, class QWidget * parent = 0, const char * name = 0 );
    ~QmcDateNav();

    virtual QSizePolicy sizePolicy() const;
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

    bool frame() const;
    void setFrame( bool );

    QList<QDate> selectedDates() const;
    bool setDate( const QDate d );

    bool outlook() const;
    void setOutlook( bool );

    void installEventDayList( QList<QDate> * );
    void removeEventDayList();

    void installNonWorkingDayList( QList<QDate> * );
    void removeNonWorkingDayList();

public slots:
    void forceUpdate();

signals:
    void dateChanged();
    void yearChanged( int );
    void clicked();

private slots:
    void monthMenuClicked( int );
    void rbMenuClicked( int );

private:
    QDate currentMonth;
    QPopupMenu *sevenMonthPopup;
    QPopupMenu *rbPopup;
    bool pmDirty;
    bool doFrame;
    QPixmap *pm;
    void init();
    void makePixmap();
    void drawArrows( QPainter & );
    QRect * drawCaptionText( QPainter & );
    void drawCaption( QPainter & );
    void drawFrame( QPainter & );
    void drawDays( QPainter & );
    void drawDates( QPainter & );
    void drawSelectedDate( int, bool pe = false );
    void undrawSelectedDate( int );
    int numSelected();
    void drawToday( int );
    void drawSelections();
    void clear();
    void changeMonth( bool forward, int steps );
    QString monthName( int );
    int fixDays( int, int, int );
    void prepPopup();
    void prepRBPopup();
    bool hasEvent( const QDate & );
    bool isNonWorkingDay( const QDate & );
    int findIndex( QRect & );
    int findIndex( QDate & );

protected:
    virtual void paintEvent( QPaintEvent * );
    virtual void mousePressEvent( QMouseEvent * );
    virtual void mouseMoveEvent( QMouseEvent * );

};

#endif //QmcDATENAV_H
