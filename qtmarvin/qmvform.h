/*
 *     $Id: qmvform.h 3232 2006-12-15 01:18:27Z source $
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

#ifndef QMVFORM_H
#define QMVFORM_H

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

#include <qdialog.h>
#include <qstring.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qwhatsthis.h>
#include <mreportviewer.h>
#include "qmvclass.h"
#include "qmvselect.h"

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvSqlClass;

class QmvForm : public QDialog
{
    Q_OBJECT

      public:
    enum FieldType { Label=0, Field, Line,
                     CalculatedField=100, Count=100, Sum, Average, Variance, StdDeviation,
                     Special=200, Date=200, Page };
    
    enum SectionType { ReportHeader=0, PageHeader,
                       Detail,
                       PageFooter, ReportFooter };
    
    QmvForm( QmvSqlClass * relation, QWidget * parent = 0, const char * name = 0 );    // Constructor
    ~QmvForm();  // Destructor

    void setRelation( QmvSqlClass * relation );
    bool initForm();
    bool initForm( const QString & form );

    bool setTemplate();
    bool setData();
    bool writeTemplate();
    bool writeData();
    void renderForm();
    bool printedStatus() const;
    MReportViewer * formViewer();

        // Class for form targets (print,email,fax etc)
    struct formTarget
    {
        QString program;        // program to exec       (def = "lp
        QString options;        // option to set printer (def="-P")
        QString name;           // printer/outputfile name
        QStringList destination;// list of eg fax numbers or email addresses
    };

    enum targetType
    {
        formPrint,
        formFile
    };  

  public slots:
    bool viewForm( bool init = true );
    void setPrint( const QString & program, const QString & options, const QString & name );
    void setFax( const QString & program, const QString & options, const QString & name,
                 const QStringList & destination );
    void setEmail( const QString & program, const QString & options, const QString & name,
                   const QStringList & destination );
    void selectTarget( targetType, const QString & name  );
    
    bool printForm( bool init = true );
    void slotFirstPage();
    void slotPrevPage();
    void slotNextPage();
    void slotLastPage();
    void slotPrintReport();
    void slotFaxReport();
    void slotEmailReport();
    void printToFile( const QString & filename );
    void slotSetBannerText( const QString & text );
    
      private:
    QLabel * banner;
    formTarget ft_print;
    formTarget ft_fax;
    formTarget ft_email;
    int page_total;
    int page_current;
    bool print_status;
    QString formDetailList();
    bool editFixedFields( const QString form_detail );
    int parseRepeat( QString * str );
    void escapeXML( QString * str );
    QString formatSelect( QmvSqlClass * reln_fmdt, int row );
    QString formatLabel( QmvSqlClass * reln_fmdt, int row, const QString & value );
    QString formatField( QmvSqlClass * reln_fmdt, int row, const QString & value, int repeat = 0 );
    QString formatLine( QmvSqlClass * reln_fmdt, int row );
    QString formatCalculationType( QmvSqlClass * reln_fmdt, int row , FieldType type );
    QString formatSpecial( QmvSqlClass * reln_fmdt, int row , FieldType type );

    QmvSqlClass * form_data;                    //~< The relation for data selection.
    QmvSqlClass * reln_form;                    //!< Specifications from the form relation
    QmvSqlClass * reln_fmdt;                    //!< Specifications from the fmdt relation
    QString form_name;                          //!< Form Name - primary key to form table.
    QString form_template_str;                  //!< Template string
    QString form_data_str;                      //!< Data string
    QString form_template_name;                 //!< Name for template file
    QString form_data_name;                     //!< Name for data file
    
    QGuardedPtr<MReportViewer> form_viewer;
    
}; /* CLASS QmvForm */

#endif /* QMVFORM_H */
// Local Variables:
// mode:C++
// End:
