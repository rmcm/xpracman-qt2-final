/*
 *     $Id: qtkugar.cpp 605 2001-07-03 22:17:41Z source $
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
 * DESCRIPTION: 
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */

#include "qtkugar.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

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

QtKugar::QtKugar()
{
  	rptviewer = new MReportViewer(this);
	connect(rptviewer,SIGNAL(preferedTemplate(const QString &)),
			  SLOT(slotPreferedTemplate(const QString &)));

	setCentralWidget(rptviewer);

}

// Destructor

QtKugar::~QtKugar()
{
  
}


void QtKugar::renderReport()
{
    rptviewer->renderReport();
}

void QtKugar::setReportData(const QString & report_data)
{
    QFile f_data(report_data);
    if ( ! f_data.open( IO_ReadOnly ) )
        QMessageBox::critical( 0, QString( "Fatal Error - No Template available" ),
                          "Cannot open data file");
    if ( !rptviewer->setReportData( &f_data ) )
        QMessageBox::critical( 0, QString( "Fatal Error - No Template available" ),
                          "Cannot parse data file");
}
void QtKugar::setReportTemplate(const QString & report_template )
{

    QFile f_template( report_template );


    if ( !f_template.open( IO_ReadOnly ) )
        QMessageBox::critical( 0, QString( "Fatal Error - No Template available" ),
                          "Cannot open template file");
    if ( !rptviewer->setReportTemplate( &f_template ) )
        QMessageBox::critical( 0, QString( "Fatal Error - No Template available" ),
                          "Cannot parse template file");
    
}

// Return the preferred size.

QSize QtKugar::sizeHint() const
{
	// This should also allow for the bits KMainWindow adds.

	return rptviewer -> sizeHint();
}
