/***************************************************************************
              mreportviewer.h  -  Kugar report viewer widget
              -------------------
    begin     : Fri Aug 13 1999                                           
    copyright : (C) 1999 by Mutiny Bay Software                         
    email     : info@mutinybaysoftware.com                                     
 ***************************************************************************/

#ifndef MREPORTVIEWER_H
#define MREPORTVIEWER_H

#include <qapplication.h>
#include <qwidget.h>
#include <qscrollview.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qprinter.h>
#include <qprogressdialog.h>

#include "mreportengine.h"
#include "mpagecollection.h"
#include "mpagedisplay.h"

#define M_PROGRESS_DELAY 500			// Number of ms to delay progress dialog display

/** Application reference, required for event processing */
extern QApplication *mAppRef;

/**Kugar report viewer widget
  *@author Mutiny Bay Software
  */

class MReportViewer : public QWidget
{
	Q_OBJECT

public:
	MReportViewer(QWidget *parent=0, const char *name=0);
	virtual ~MReportViewer();

	bool setReportData(const QString &);
	bool setReportData(QIODevice *);
	bool setReportTemplate(const QString &);
	bool setReportTemplate(QIODevice *);
	bool renderReport();
        int  pageCount();
	void clearReport();
        void setPrinterName( const QString & name );
        void setPrintProgram( const QString & program );
        void setPrinterSelectionOption( const QString & option );
        void setOutputFileName( const QString & filename );
	void printReport();

	QSize sizeHint() const;

public slots:
	void slotFirstPage();
	void slotNextPage();
	void slotPrevPage();
	void slotLastPage();
	void slotPrintReport();

signals:
	void preferedTemplate(const QString &);

private slots:
	void slotCancelPrinting();
	void slotRenderProgress(int);

private:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
        QString default_printer;
        QString default_printprogram;
        QString default_printeroption;
        QString default_filename;
        
	QScrollView *scroller;
	MPageDisplay *display;
	MReportEngine *rptEngine;
	MPageCollection *report;
	QPrinter *printer;
};

#endif
