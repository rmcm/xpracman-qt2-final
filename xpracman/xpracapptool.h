/*
 *     $Id: xpracapptool.h 3216 2006-12-06 11:49:36Z source $
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

#ifndef XPRACAPPTOOL_H
#define XPRACAPPTOOL_H

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

#include <math.h>
#include <qdialog.h>
#include <qintdict.h>
#include <qlabel.h>
#include <qdatetime.h>
#include "qmcdatenav.h"
#include "qmvclass.h"
#include "qmvtoolwindow.h"
#include "qmvroweditor.h"

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */
//class QmvApplication;

class XpracAppTool : public QmvToolWindow
{
    Q_OBJECT

      public:
    XpracAppTool( QmvSqlClass * appt, QmvApplication * parent = 0, const char * name = 0 );
    ~XpracAppTool();  // Destructor

        // Amalgamate the editable event attributes
        // - bits 1-8:  attribute value
        // - bits 9-:   attribute id
    enum PropertyClass
    {
        PC_Provider = 256,
        PC_Referrer = 512,
        PC_Location = 1024,
        PC_Appttype = 2048,
        PC_Progress = 4096,
        PC_Mask = PC_Provider|PC_Referrer|PC_Location|PC_Progress
    };

    enum DisplayColourMode
    {
        ProviderColour,
        LocationColour,
        AppttypeColour,
        ProgressColour
    };

        // Define the units for recurring events.
    enum IntervalUnit
    {
        IntervalDay,
        IntervalMonth
    };

        // Special event tags = used in patient ID
    enum SpecialEvents
    {
        BookingInProgress = 0,
        VacantEvent = -1,
        SpareEvent = -2
    };


        // --------------------------------------------
        // a single existing appointment
        // --------------------------------------------
    struct eventItem
    {
        int patient_id;
        QString patient_str;    //!< Descriptive label
        QString provider;       //!< Provider code
        QString referrer;       //!< Referrer code
        QString location;       //!< Location code
        QString appttype;       //!< Type code
        QDateTime start;        //!< Start timestamp
        int duration;           //!< Duration in minutes
        int disable;             //!< -1 = block (used to also handle what is now "parallel")
        int parallel;           //!< mulitple booking?
        int skip;               //!< Skip slots
        bool active;            //!< Status
        QString progress;       //!< Progress code
        int rowid;              //!< evnv__sequence in DB.
        QRgb rgb;               //!< Display
            // the following describe display set which this item
            // is a part of. Display widths can be calculated from
            // these values.
        int n_templates;        //!< Number of templates in display set
        int n_events;           //!< Number of events in display set
        int n_blocks;           //!< Number of blocks in display set

            // constructor
        eventItem()
            {
                disable = 0;
                parallel = 0;
                skip = 0;
                active = false;
                n_templates = 0;
                n_events = 0;
                n_blocks = 0;
            }

    };


        // --------------------------------------------
        // Appointment Search Attributes,
        // - a template used for creation of virtual appointments,
        //   and loaded from the APPT/APTD table pair
        // --------------------------------------------

    struct searchForEvent
    {
        QString aptd_code;
        int disable;                     // -1 == deny, 0=allow
        int duration;                   // length in minutes
        int interval;                   // interval between events
            // descriptive
        QString aptd_desc;
        QString aptd_colour;
            // Demographic and date attributes are loaded into bit-arrays
            // - mapped to menulists for prov and locn, or numeric for date
            //   attributes.
            //
            // demographic attributes
        QBitArray ba_prov_code;         // can be a list -eg SMA,JAT
        QBitArray ba_locn_code;         //   as above
            // date attributes
        QBitArray ba_dayofweek;         // can be a list or range - as in crontab
        QBitArray ba_weekofyear;        //   as above
        QBitArray ba_dayofmonth;        //   as above
        QBitArray ba_monthofyear;       //   as above
        int aptd_year;
            //
            // Appointment geometry
            //
        int aptd_parallel;              // 0=no, 1=double ... (mulitple booking)
        int aptd_skip;                  // 0=no, 1=alternate .... (skip slots)
            //
            // recurrence attributes
            //
        QDate aptd_start_date;
        QDate aptd_end_date;
        IntervalUnit rec_unit;          // interval eg "5 days" or "1 month"
        int rec_value;                  // number of rec_unit
            // times - minutes from midnight.
        int aptd_starting;
        int aptd_ending;
        QString aptd__sequence;
    };

        // Appointment candidate - used in search.
        // -- applies to a single appointment in one day
    struct apptCandidate
    {
        QString prov;
        QString locn;
        QString type;
        QString desc;
        QRgb rgb;
        int start;
        int duration;
        int disable;
        int parallel;
        int skip;
//        bool locn_ignore = false;
    };
        // Appointment Type - header.
    struct apptType
    {
        QString aptp_code;
        QString aptp_desc;
        QString aptp_colour;
        int aptp_duration;
        int aptp_disable;
        int aptp_interval;
        QString aptp__sequence;
    };

        // Days per display page
    enum DisplayPeriod
    {
        DisplayOneDay = 1,
        DisplayFiveDay = 5,
        DisplaySevenDay = 7,
        DisplayMonth = 30
    };
        // Resolution on page
    enum PeriodInterval
    {
        PeriodMinute=1,
        PeriodHour = 60,
        PeriodOneDay = 60*24,
        PeriodFiveDay = 60*24*5,
        PeriodSevenDay = 60*24*7,
        PeriodMonth = 60*24*31,
        PeriodYear = 60*24*366
    };
        // A display period = may be many pages
    struct eventPeriod
    {
        QDateTime start;
        QDateTime end;
        PeriodInterval interval;        // resolution
    };
        // Display layout for a day
    struct periodDayDisplay
    {
        bool am_pm;             //!< true = AM/PM display, else 24hr.
        int hr_start;           //!< First hour displayed
        int hr_end;             //!< Last hour dislayed
        int granularity;        //!< appointment granularity
        int duration;           //!< default appt length in minutes
        int top_margin;         //!< Top margin - excluding bounding margin
        int bottom_margin;      //!< Bottom margin - no bounding margin at bottom
        int vmargin;            //!< Vertical bounding margin
        int hmargin;            //!< Horizontal bounding margin
        QPen line_pen;          //!< Pen for line drawing??
        QBrush qtr_pattern;     //!< Shading for 15 minute lines
        int width;              //!< Width of time display box
        int height;             //!< Height of time display box
        int top;                //!< Position of top hour line
        int bottom;             //!< Position of bottom hour line
        QRect display;          //!< ??used??
        int hr_vstep;           //!< Vertical size of hour
        int daypad;             //!< Horizontal space between display days
        int eventpad;           //!< space between events
        int labmenuwid;         //!< Width of menu area inside label.
        QRect rect_hold;        //!< The holding pen for unset events
        int w_template;         //!< Width of template object
        int w_blocked;          //!< Width of blocking object
        int w_event;            //!< Width of standard appointment
        int w_template_pref;    //!< Preferred template width
        int w_blocked_pref;     //!< Preferred blocked width
        int w_template_pctg;    //!< Preferred percentage of display width for templates.
    };

    enum findStatus { cancelSearch = QDialog::Rejected, newPatient, existingPatient };


        // --------------------------------------------
        // Class to contain and manipulate eventItem objects
        // --------------------------------------------

    typedef QList<eventItem>            XpracEventListBase;
    typedef QListIterator<eventItem>    XpracEventListIterator;

    class Q_EXPORT XpracEventList : public XpracEventListBase
    {
          public:
        XpracEventList( bool deepCopies=true )
            {
                dc = deepCopies;
                del_item = deepCopies;
            }
        XpracEventList( const XpracEventList &);
        ~XpracEventList()
            {
                clear();
            }
        XpracEventList& operator=( const XpracEventList & );

          private:
        int compareItems(QCollection::Item s1, QCollection::Item s2 )
            {
                    // sort by provider then event type
                int i = ((const eventItem*)s1)->provider.compare( ((const eventItem*)s2)->provider );
                if (i != 0 )
                {
                    return i;
                } else {
                    i = ((const eventItem*)s1)->appttype.compare( ((const eventItem*)s2)->appttype );
                    if (i != 0 )
                    {
                        return i;
                    } else {
                        return ((const eventItem*)s1)->patient_str.compare( ((const eventItem*)s2)->patient_str );
                    }
                }
            }

        bool dc;
    };


        // --------------------------------------------
        // Class to hold dynamic display attributes
        // - operates on a perodDayDisplay and one or
        //   more XpracEventList lists to prepare 
        //   display attributes
        // --------------------------------------------

    class Q_EXPORT XpracEventCounter
    {
          public:
        XpracEventCounter() { clear(); };
        XpracEventCounter( const XpracEventCounter &);
        ~XpracEventCounter()
            {
            }
        XpracEventCounter& operator=( const XpracEventCounter & );
        
        bool addDataSet(XpracEventList&) {
        
        };
        void clear()
            {
                max_events = 0;
                max_templates = 0;
                max_blocks= 0;
                event_list.clear();
                template_list.clear();
                block_list.clear();
            };

            // ----------------------------------------
            //! Add list to collection
            /*!
              Add an XpracEventList list to the current
              collection.
              
              \param current_day    the day we are focussed on
              \param day_display    the overall display attributes
              \param list           the XpracEventList list
            */
            // ----------------------------------------
        void load( const QDate &current_day, const periodDayDisplay &day_display,
                   const XpracEventList & list)
            {
                    // walk over the list, and 
                XpracEventListIterator it(list);
                for ( ; it.current(); ++it )
                {
                    eventItem * ei = it.current();

                        // get the day (as days from current date)
                        // .. to use as an index into template and events arrays
                    int ev_day = current_day.daysTo(ei->start.date());

                        // time dimensions - start time in minutes from midnight
                        //  ... duration is already in minutes
                    int ev_start = static_cast<long> (floor(abs(ei->start.time().secsTo( QTime(0, 0) ) )/60));

                        // qDebug("start=%d,duration=%d,dayspan=%d-%d", ev_start, ei->duration,
                        // day_display.hr_start*60, day_display.hr_end*60 );
                

                        // Skip if not on display
                    if ( ei->start.date().isNull()
                         || ev_start < (day_display.hr_start * 60)
                         || (ev_start + day_display.duration) > (day_display.hr_end * 60)
                         || ei->patient_id == SpareEvent )
                        continue;

                    addItemToCounters( ei, ev_day, ev_start );
                
                }

                    // We should now have counters of events and templates at each time slice (granularity)

                setStatistics();
            }
        
        int maxTemplates() {return max_templates; };
        int maxEvents() { return max_events; };
        
        void setItemCounters( const QDate &current_day, const periodDayDisplay &day_display,
                              XpracEventList * list)
            {
                    // walk over the list, and 
                XpracEventListIterator it(*list);
                for ( ; it.current(); ++it )
                {
                    eventItem * ei = it.current();

                        // get the day (as days from current date)
                        // .. to use as an index into template and events arrays
                    int ev_day = current_day.daysTo(ei->start.date());

                        // time dimensions - start time in minutes from midnight
                        //  ... duration is already in minutes
                    int ev_start = static_cast<long> (floor(abs(ei->start.time().secsTo( QTime(0, 0) ) )/60));

                        // qDebug("start=%d,duration=%d,dayspan=%d-%d", ev_start, ei->duration,
                        // day_display.hr_start*60, day_display.hr_end*60 );
                

                        // Skip if not on display
                    if ( ei->start.date().isNull()
                         || ev_start < (day_display.hr_start * 60)
                         || (ev_start + day_display.duration) > (day_display.hr_end * 60)
                         || ei->patient_id == SpareEvent )
                        continue;

                        // now set the counters in the item
                    setItemFromCounters( ei, ev_day, ev_start );
                
                }
            }
        
        
          private:

            // ----------------------------------------
            //! Add event item to width counters
            /*!
              Add the eventItem object to the coverage
              counters

              \param ei         eventItem object        
              \param ev_start   start of event (minutes from midnight)
            */
        
            // ----------------------------------------
        void addItemToCounters( eventItem * ei, int ev_day, int ev_start )
            {
                
                    // get the array for templates on this day,
                    // ... create it if not exists
                QIntDict<int> * tpdl = template_list[ev_day];
                if (!tpdl) {
                    tpdl = new QIntDict<int>;
                    template_list.insert(ev_day, tpdl);
                }
                
                    // Get the array for events on this day
                    // ... create it if not exists
                QIntDict<int> * evdl = event_list[ev_day];
                if (!evdl) {
                    evdl = new QIntDict<int>;
                    event_list.insert(ev_day, evdl);
                }

                    // Get the array for blocks on this day
                    // ... create it if not exists
                QIntDict<int> * bldl = block_list[ev_day];
                if (!bldl) {
                    bldl = new QIntDict<int>;
                    block_list.insert(ev_day, bldl);
                }

                    // Scan domain of event, and increment counters at each granularity
                for (long i = ev_start; i < (ev_start + ei->duration); i++ ) {
                    if (ei->disable) {
                        int * bll = (*bldl)[i];
                        if (!bll) {
                            bll = new int(1);
                            bldl->insert(i, bll);
                        } else {
                            (*bll)++;
                        }
                    } else if (ei->patient_id == VacantEvent) {
                        int * tpl = (*tpdl)[i];
                        if (!tpl) {
                            tpl = new int(1);
                            tpdl->insert(i, tpl);
                        } else {
                            (*tpl)++;
                        }
                    } else {
                        int * evl = (*evdl)[i];
                        if (!evl) {
                            evl = new int(1);
                            evdl->insert(i, evl);
                        } else {
                            (*evl)++;
                        }
                            // qDebug("time=%ld Event=%d", i, ei->patient_id);
                    }
                }
            }
        
        
            // ----------------------------------------
            //! Set composite display statistics
            /*!
              Set the composite display statistics for
              the loaded lists
            */
            // ----------------------------------------
        void setStatistics()
            {
                
                    // Determine max (template count per timeslice)
                max_templates = 0;
                QIntDictIterator<QIntDict<int> > itday_templates(template_list);
                while( itday_templates.current() ) {
                    QIntDictIterator<int> it_templates(*(itday_templates.current()));
                    while ( it_templates.current() ) {
                        int * i = it_templates.current();
                        if (*i > max_templates) max_templates = *i;
                        ++it_templates;
                    }
                    ++itday_templates;
                }
            
                    // Determine max (event count per timeslice)
                max_events = 0;
                QIntDictIterator<QIntDict<int> > itday_events(event_list);
                while( itday_events.current() ) {
                    QIntDictIterator<int> it_events(*(itday_events.current()));
                    while ( it_events.current() ) {
                        int * i = it_events.current();
                            // qDebug(">>> time=%ld count=%d",it_events.currentKey() , *i);
                        if (*i > max_events) max_events = *i;
                        ++it_events;
                    }
                    ++itday_events;
                }
                    // Determine max (block count per timeslice)
                max_blocks = 0;
                QIntDictIterator<QIntDict<int> > itday_blocks(block_list);
                while( itday_blocks.current() ) {
                    QIntDictIterator<int> it_blocks(*(itday_blocks.current()));
                    while ( it_blocks.current() ) {
                        int * i = it_blocks.current();
                            // qDebug(">>> time=%ld count=%d",it_blocks.currentKey() , *i);
                        if (*i > max_blocks) max_blocks = *i;
                        ++it_blocks;
                    }
                    ++itday_blocks;
                }

                qDebug("XpracEventCounter::==> max_templates=%d, max_events=%d, max_blocks=%d",
                       max_templates, max_events, max_blocks);
            };

            // ----------------------------------------
            //! Add event item to width counters
            /*!
              Add the eventItem object to the coverage
              counters

              \param ei         eventItem object        
              \param ev_start   start of event (minutes from midnight)
            */
        
            // ----------------------------------------
        void setItemFromCounters( eventItem * ei, int ev_day, int ev_start )
            {
                int template_count = 0;
                int event_count = 0;
                int block_count = 0;
                
                    // get the array for templates on this day,
                    // ... create it if not exists
                QIntDict<int> * tpdl = template_list[ev_day];
                
                    // Get the array for events on this day
                    // ... create it if not exists
                QIntDict<int> * evdl = event_list[ev_day];

                    // Get the array for blocks on this day
                    // ... create it if not exists
                QIntDict<int> * bldl = block_list[ev_day];

                    // Scan domain of event, and increment counters at each granularity
                for (long i = ev_start; i < (ev_start + ei->duration); i++ ) {
                    if (tpdl) {
                        int * tpl = (*tpdl)[i];
                        if (tpl && (*tpl) > template_count)
                            template_count = *tpl;
                    }
                    
                    if (evdl) {
                        int * evl = (*evdl)[i];
                        if (evl && (*evl) > event_count)
                            event_count = *evl;
                    }

                    if (bldl) {
                        int * bll = (*bldl)[i];
                        if (bll && (*bll) > block_count)
                            block_count = *bll;
                    }
                }

                    // now we know how many template and events to share the width with.
                ei->n_templates = template_count;
                ei->n_events = event_count;
                ei->n_blocks = block_count;
            }
        
        
        QIntDict<QIntDict<int> > event_list;           //!< list of event counts at each time slice
        QIntDict<QIntDict<int> > template_list;        //!< List of template counts at each time slice
        QIntDict<QIntDict<int> > block_list;           //!< List of block counts at each time slice
        int time_slice;                     //!< Current timeslice
        int max_events;                     //!< Maximum event count in a time slice
        int max_templates;                  //!< Maximum template count in a time slice
        int max_blocks;                     //!< Maximum block count in a time slice
    };

        // --------------------------------------------
        // Class to hold date-headings on canvas
        // --------------------------------------------

    class Q_EXPORT XpracDateHeading : public QRect
    {
          public:
        XpracDateHeading( const periodDayDisplay & day_display,
                          const DisplayPeriod & display_period,
                          const QDate &current_day )
                : QRect( day_display.hmargin, day_display.top_margin,
                         (day_display.width * display_period) - (day_display.hmargin*2),
                         day_display.vmargin)
            {
                init( day_display,
                      display_period,
                      current_day);
            }
        XpracDateHeading() {}
        ~XpracDateHeading() {}
        void init( const periodDayDisplay & day_display,
                   const DisplayPeriod & display_period,
                   const QDate &current_day ) 
            {
                
                dd = day_display;
                dp = display_period;
                cd = current_day;
                setRect(day_display.hmargin, day_display.top_margin,
                        (day_display.width * display_period),
                        day_display.vmargin);
            }
        
        QDate dateSelected( const QPoint & pt ) 
            {
                if (contains(pt)) {
                    int daynum = int( (dp * (pt.x() - left())) / width() );
                    return cd.addDays(daynum);
                } else {
                    return QDate();
                }
            }
          private:
        periodDayDisplay dd;
        DisplayPeriod dp;
        QDate cd;
    };
    


public slots:

eventPeriod loadVacantSet( const QDate & from, const QDate & to );
    eventPeriod loadBaseSet( const QString & query = QString::null );
    eventPeriod loadCurrentSet( const QString & query );
    void loadNewSet();
    void setCurrentDate( const QDate & cdate );
    void slotToday();

        // Reimplemented

    virtual void init( const QString & query = QString::null );

protected slots:

    // Reimplemented

virtual void slotSetBannerText( const QString & text );
    virtual void slotBack4();
    virtual void slotBack3();
    virtual void slotBack2();
    virtual void slotBack1();
    virtual void slotHome();
    virtual void slotForward1();
    virtual void slotForward2();
    virtual void slotForward3();
    virtual void slotForward4();
    virtual void slotPrint();
    virtual void slotDisplay1();
    virtual void slotDisplay2();
    virtual void slotDisplay3();
    virtual void slotFind();

        // new
    void showProviderMenu();
    void showLocationMenu();
    void showAppttypeMenu();
    void showProgressMenu();
    void showProviderColour();
    void showLocationColour();
    void showAppttypeColour();
    void showProgressColour();

    void setDateByCalendar();
    void slotShowMonth();

    QColor fixColour( const QString colour_str ) const;

    void loadProviderMenu();
    void loadLocationMenu();
    void loadAppttypeMenu();
    void loadProgressMenu();
    void slotAddSpareAppointment();
    void setProvider( int id );
    void setLocation( int id );
    void setAppttype( int id );
    void setProgress( int id );

    QString loadPeriodQuery( const QDateTime & from, const QDateTime & to,
                             const QString & orderby, bool filter = FALSE );
    void handleMouseClicks( QCanvasItem * pressed, const QPoint & pt, ButtonState button, ButtonState state );
    void popupEventDetails( QCanvasItem * pressed, const QPoint & pt, ButtonState button, ButtonState state );
    void popupEventDetails( const eventItem *ei, const QPoint pt  );
    void setProgressFlag(eventItem * ei);
    void eventLabelMoved( QCanvasItem * pressed, const QPoint & from, const QPoint & to );
    void eventLabelResized( QCanvasItem * pressed, const QRect & from, const QRect & to );
    void dataChangeRequested( QCanvasItem * pressed, const QPoint & pt, ButtonState button, ButtonState state );
    const QDateTime coordToDateTime( QPoint mp ) const;
    const QString qtDateTimeToPgTimestamp( const QDateTime & dt ) const;
    void nextAvailableAppointment();
    void previousAvailableAppointment();
    void scanForAppointment( int step = 1 );
    void slotSearchToday();

    void slotSinglePrint(eventItem * ei);
    void slotPrintReport( const QString & query );

      signals:
    void eventUpdated( int rowid );

      protected:

        // reimplemented
    virtual void buildBackgroundPixmap();

      private:

    int granularity;                    //!< The appointment granularity in minutes.
    XpracEventList event_list;          //!< List of all events in period
    QDict<apptType> appt_type_list;     //!< List of appt type headers indexed by type
    QList<searchForEvent> search_list;  //!< List of search attributes
    QList<searchForEvent> deny_list;    //!< List of denied areas - eg holidays.
    QIntDict<apptCandidate> app_list;   //!< List of available appointments on search_day
    XpracEventList base_events;         //!< List of basic queried events(eg single ptient)
    XpracEventList new_events;          //!< List of events with null starttime
    XpracEventList vacant_events;       //!< List of events with invalid rowid
    XpracEventCounter event_counter;    //!< List of per-time event counts.

    const bool eventInList( const int id, const XpracEventList * elist ) const;
    int findPatient( int patn_sequence = -1, const QString & provider = QString::null );

    QDate current_day;
    periodDayDisplay day_display;
    DisplayColourMode disp_colour_mode; //!< Which colour code to display
    QmvSqlClass * reln_evnv;
    QmvSqlClass * reln_prov;
    QmvSqlClass * reln_locn;
    QmvSqlClass * reln_aptp;
    QmvSqlClass * reln_aptd;
    QmvSqlClass * reln_apst;
    QmvSqlClass * reln_patn;
    QDialog * dl_datenav;
    QmcDateNav * date_nav;
    QPopupMenu * popup_datenav;
    QPopupMenu * popup_provider;
    QPopupMenu * popup_location;
    QPopupMenu * popup_appttype;
    QPopupMenu * popup_progress;
    QPopupMenu * popup_ev_provider;
    QPopupMenu * popup_ev_location;
    QPopupMenu * popup_ev_appttype;
    QPopupMenu * popup_ev_progress;
    QPopupMenu * popup_ev_properties;
    QString current_provider_str;
    QString current_location_str;
    QString current_appttype_str;
    QString current_progress_str;
    QString current_patient_str;
    int getMenuIDbyProviderCode( const QString & prov ) const;
    int getMenuIDbyLocationCode( const QString & locn ) const;
    int getMenuIDbyAppttypeCode( const QString & locn ) const;
    int getMenuIDbyProgressCode( const QString & locn ) const;

    int current_provider_id;            //!< prov__sequence
    int current_location_id;            //!< locn__sequence
    int current_appttype_id;            //!< locn__sequence
    int current_progress_id;            //!< locn__sequence
    int all_progress_id;                //!< pointer tp "all" slot in popup_progress
    int booked_progress_id;             //!< pointer to booked slot in popup_progress
    int vacant_progress_id;             //!< pointer to vacant slot in popup_progress
    QIntDict<QString> popup_prov_codes; //!< menu index -> provider code
    QIntDict<QString> popup_locn_codes; //!< menu index -> location code
    QIntDict<QString> popup_aptp_codes; //!< menu index -> appttype code
    QIntDict<QString> popup_apst_codes; //!< menu index -> progress code
    QDict<int> popup_prov_index;        //!< provider code -> menu index
    QDict<int> popup_locn_index;        //!< location code -> menu index
    QDict<int> popup_aptp_index;        //!< appttype code -> menu index
    QDict<int> popup_apst_index;        //!< progress code -> menu index

    QDict<QString> prov_code_colours;   //!< provider colour -> code
    QDict<QString> locn_code_colours;   //!< location colour -> code
    QDict<QString> aptp_code_colours;   //!< appttype colour -> code
    QDict<QString> apst_code_colours;   //!< progress colour -> code

    eventItem * createSpareEvent(const QDate day) const;

    const QColor currentProviderColour( const QString & code ) const;
    const QColor currentLocationColour( const QString & code ) const;
    const QColor currentAppttypeColour( const QString & code ) const;
    const QColor currentProgressColour( const QString & code ) const;

    int monitoredQuery( const QString query );
    int countEstimate( const QString query );

    DisplayPeriod display_period;
    bool display_booked;
    bool display_vacant;
    QPixmap background;
    void setObjectDisplayWidths();
    void initDisplayParameters();
    void buildMonthPixmap( QPainter & p );
    void buildDayGridPixmap( QPainter & p );
    void displayEventsByDay( QDate day, QPainter & p, XpracEventList * el, const QBrush & brush );
    QRect eventRectInDay( eventItem * ei );
    int weekOfYear( const QDate & dt ) const;

    QDialog * dl_search;
    QmcDateNav * search_date_nav;

    void loadAppttypeTemplate( const QString & type, bool force = FALSE  );
    const int nextEventList( const QString & type, const QDate & day );

    bool parseRangeString( QString & rs, int * from, int * to, int * step );
    bool loadIntBA( QBitArray & ba, const QString & spec, int min, int max );
    bool loadStringBA( QBitArray & ba, const QString & spec, const QStringList & list, bool cs = TRUE );
    bool dayMatches( const searchForEvent * ev, const QDate & search_day ) const;
    bool eventDenied(  const QDate & day, const apptCandidate * ac );

    void dataChangeRequested( eventItem * ei );

    QPopupMenu * popup_file;
    QPopupMenu * popup_print;
    QPopupMenu * popup_utilities;
    QPopupMenu * popup_help;

    QIntDict<QCanvasItem> canvas_items;
    QPtrDict<eventItem> canvas_events;

    QSize cc_size;                      //!< size of color code pixmap
    QFont label_font;                   //!< font for drawing on appointment objects
    QStringList def_prov_in_select;     //!< include default provider in select

    XpracDateHeading date_heading;

}; /* CLASS XpracAppTool */

#endif /* XPRACAPPTOOL_H */
// Local Variables:
// mode:C++
// End:
