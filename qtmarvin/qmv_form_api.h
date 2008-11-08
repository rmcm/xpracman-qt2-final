/*
 *     $Id: qmv_form_api.h 1701 2003-04-27 05:00:23Z source $
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

#ifndef QMV_FORM_API_H
#define QMV_FORM_API_H

/*
 * ======================================================================
 * DESCRIPTION: An API to the form/fmdt relation set
 *
 * ======================================================================
 */

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <qstring.h>
#include "qmvclass.h"

/*
 * ======================================================================
 * STRUCT DEFINITION
 * ======================================================================
 */

struct formHeader 
{
    formHeader();
    ~formHeader();

    bool load( int row, QmvSqlClass * reln_form );
    bool update( int row, QmvSqlClass * reln_form );
    QString form_code;
    QString form_desc;
    QString form_classname;
    QString form_page_attribute;
    QString form_includes;
    QString form_pagesize;
    QString form_orientation;
    int form_mg_top;
    int form_mg_bottom;
    int form_mg_left;
    int form_mg_right;
    int form_rh_height;
    int form_rh_frequency;
    int form_rf_height;
    int form_rf_frequency;
    int form_ph_height;
    int form_ph_frequency;
    int form_pf_height;
    int form_pf_frequency;
    int form_dt_height;
    int form__sequence;
    QString form__timestamp;
    QString form__user_entry;
    QString form__status;        
};

struct formDetail
{
    formDetail();
    ~formDetail();

    enum fieldType {
        Label = 0,
        Field = 1,
        Line = 2,
        Calc = 3,
        Special = 4
    };
    enum formSection {
        ReportHeader = 0,
        PageHeader = 1,
        Detail = 2,
        PageFooter = 3,
        ReportFooter = 4
    };
    
    bool load( int row, QmvSqlClass * reln_form);
    bool update( int row, QmvSqlClass * reln_form);
    QString fmdt_form_code;                                 // Form Code 
    QString fmdt_desc;                                      // description
    int fmdt_field_type;                                    // 0=Label; 1=Field; 2=line; 3=Calc; 4=Special
    int fmdt_section;                                       // 0=RH;1=PH;2=D;3=PF;4=RF
    QString fmdt_text;                                      // literal/attribute_name
    int fmdt_x_coord;                                       // X Co-ordinate
    int fmdt_y_coord;                                       // Y Co-ordinate
    int fmdt_width;                                         // also used for Line
    int fmdt_height;                                        // also used for Line
    QString fmdt_bg_colour;                                 // background "NNN;NNN;NNN" (R;G;B)
    QString fmdt_fg_colour;                                 // foreground "NNN;NNN;NNN" (R;G;B)
    QString fmdt_bd_colour;                                 // border "NNN;NNN;NNN" (R;G;B) 
    int fmdt_bd_width;                                      // also used for Line
    int fmdt_bd_style;                                      // also used for Line
    QString fmdt_ft_family;
    int fmdt_ft_size;
    int fmdt_ft_weight;
    int fmdt_ft_Italic;
    int fmdt_hz_align;
    int fmdt_vt_align;
    int fmdt_word_wrap;
    int fmdt_data_type;
    int fmdt_date_format;
    int fmdt_precision;
    int fmdt_currency;
    QString fmdt_neg_colour;                                // "NNN;NNN;NNN" (R;G;B)
    int fmdt_comma_sep;
    long fmdt__sequence;                                    // Row-ID
    QString fmdt__timestamp;                                // Timestamp
    QString fmdt__user_entry;                               // Entered by
    QString fmdt__status;                                   // Record status
};


          
/* STRUCT Qmv_Form_Api */

#endif /* QMV_FORM_API_H */
// Local Variables:
// mode:C++
// End:
