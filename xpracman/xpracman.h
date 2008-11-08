/*
 *     $Id: xpracman.h 3118 2006-10-02 11:48:29Z source $
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

#ifndef XPRACMAN_H
#define XPRACMAN_H

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

#include <qsignalmapper.h>
#include <qmvapplication.h>
#include <qmvform.h>
#include "xpracapptool.h"
#include "qmvformtool.h"

/*
 * ======================================================================
 * DEFINITIONS - typedefs, enums, structures etc
 * ======================================================================
 */

extern QString XpracManDir;

class XpracMan : public QmvApplication
{
    Q_OBJECT

      public:

    enum reportType { SingleInvoice=0,
                      SingleStatement,
                      ReportPatientLabel,
                      ReportBankDeposit,
                      ReportUnprintedInvoices,
                      ReportStatement,
                      ReportAgedDebtors,
                      ReportGSTInvoices,
                      ReportGSTReceipts,
                      ReportDirectedCredits,
                      ReportWorkList,
                      ReportWorkSummary,
                      ReportCreditList,
                      ReportCreditSummary,
                      ReportWorkCreditList
    };

    enum documentMode { DocumentCreate = 0,
                        DocumentAccess,
                        DocumentDelete
    };
    
    XpracMan( const QString & dbtitle, const QmvAccess * dbaccess,
              QWidget *parent = 0, const char *name = 0 );    // Constructor
    ~XpracMan(); // Destructor

    const QString findPatient() const;
    const QString findDebtor() const;
    const QString findInvoice() const;
    bool postContract( const QString & patn_number);
    bool printInvoice( const QString & invc_number );
    
    bool printStatement( const int days_ago, const int zerodays = 0, const char * debtor_regex = 0 );
    bool printStatement( const QString & query, int days_ago = 0 );

    public slots:
    void loadPreferences();
  protected slots:

    void postContract();
    void claimPCI();
    void printInvoice();
    void printStatement();
    void printLabel();
    bool printLabel( const QString & label_class, const QString & patn__sequence );
    void printBankDeposit();
    void displayPatients();
    void displayDebtors();
    void displayInvoices();
    void displayUnpaidInvoices();
    void displayPayments();
    void displayBanking();
    void todaysWaitingPatients();
    void displayReferrers();
    void displayFeebook();
    void displayPatientFlags();
    void displayEmployers();
    void displayAppointments();
    void displayProviders();
    void displayLocations();
    void displayAppointmentTypes();
    void displayAppointmentStatus();
    void slotTupleSelected( int id, QmvTuple * tup );
    void slotTupleChanged( QmvTuple * tup );
    void todaysRecentPatients();
    void addRowToSetTrigger( QString & reln, int sequence ); 
    void deleteRowFromSetTrigger( QString & reln, int sequence );
    virtual void newListPanel( QmvRelationWidget * nlp );
    void accessRowTrigger( QString & reln, int sequence ); 
    void documentInterface( QString & reln, int sequence, documentMode mode );
    bool documentCreate( const QString & docname, QmvSqlClass * reln_patv,
                         const QString & doctemplate );
    void documentAccess( const QString & docname );
    void documentDelete( const QString & docname );
    void displayAppTool( int index = 0 );
    void loadWaitingPatients( const QDate & xdate = QDate() );
    void loadRecentPatients( const QDate & xdate = QDate() );

    void printReport();
    bool setDateRange( const QString & title, const QString & from_title, const QString & to_title,
                       QDateTime *from, QDateTime *to );
    bool printAged( int days = 0 );
    bool printGSTInvoices(  const QDateTime &from, const QDateTime &to );
    bool printGSTReceipts(  const QDateTime &from, const QDateTime &to );
    bool printDirectedCredits();
    void exportMD2();
    void setExportMD2( bool on = true );
    void toggleExportMD2();
    bool printWorkList( const QDateTime & from, const QDateTime & to );
    bool printWorkSummary( const QDateTime & from, const QDateTime & to );
    bool printCreditList( const QDateTime & from, const QDateTime & to );
    bool printCreditSummary( const QDateTime & from, const QDateTime & to );
    bool printWorkCreditList( const QDateTime & from, const QDateTime & to );
    
    void about_qmvapplication();
    void help_qmvapplication();
    
      signals:
    
      private:

    QGuardedPtr<XpracAppTool> apptool;
    QActionGroup * actiongp_appointments;
    QPopupMenu * popup_appointments;
    QToolBar * toolbar_appointments;

        // QmvRelationWidget extensions
        //
    QGuardedPtr<QSignalMapper> app_signals;
    QIntDict<QmvRelationWidget> app_lists;
    
    QGuardedPtr<QSignalMapper> svp_signals;     //!< A signal mapper for SVP* extensions
    QIntDict<QmvRelationWidget> rwe_lists;      //!< Map index to QmvRelationWidget.

        // MD2 attributes
    int menu_exportmd2;                         //!< ID of item in preferences menu.
    QTimer * timer_exportmd2;                   //!< Timer to export to MD2
    bool doit_exportmd2;                        //!< flag to control timer_exportmd2
    
 private slots:
    void multiServiceReduction( int index );    //!< Handler for multiservice reductions
    void formEditor( int index );               //!< Handler for entering forms editor
    void displayFlaggedPatients( int index);    //!< Handler for viewing flagged patients

};


#endif /* XPRACMAN_H */
// Local Variables:
// mode:C++
// End:



