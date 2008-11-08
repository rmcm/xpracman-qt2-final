/*
 *     $Id: qmv_form_api.cpp 1701 2003-04-27 05:00:23Z source $
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
 * DESCRIPTION: API to form/fmdt relation set.
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include "qmv_form_api.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmv_form_api_version[] = "$Id: qmv_form_api.cpp 1701 2003-04-27 05:00:23Z source $";
// #define QMV_FORM_API_DEBUG

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

formHeader::formHeader()
{}

formDetail::formDetail()
{}

// Destructor

formHeader::~formHeader()
{}

formDetail::~formDetail()
{}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
bool formHeader::load( const int row, QmvSqlClass * reln_form )
{
    bool ok = true;
    
    form_code = reln_form->attributeValue( row, "form_code" );
    form_desc = reln_form->attributeValue( row, "form_desc" );
    form_classname = reln_form->attributeValue( row, "form_classname" );
    form_page_attribute = reln_form->attributeValue( row, "form_page_attribute" );
    form_includes = reln_form->attributeValue( row, "form_includes" );
    form_pagesize = reln_form->attributeValue( row, "form_pagesize" );
    form_orientation = reln_form->attributeValue( row, "form_orientation" );
    form_mg_top = reln_form->attributeValue( row, "form_mg_top" ).toInt();
    form_mg_bottom = reln_form->attributeValue( row, "form_mg_bottom" ).toInt();
    form_mg_left = reln_form->attributeValue( row, "form_mg_left" ).toInt();
    form_mg_right = reln_form->attributeValue( row, "form_mg_right" ).toInt();
    form_rh_height = reln_form->attributeValue( row, "form_rh_height" ).toInt();
    form_rh_frequency = reln_form->attributeValue( row, "form_rh_frequency" ).toInt();
    form_rf_height = reln_form->attributeValue( row, "form_rf_height" ).toInt();
    form_rf_frequency = reln_form->attributeValue( row, "form_rf_frequency" ).toInt();
    form_ph_height = reln_form->attributeValue( row, "form_ph_height" ).toInt();
    form_ph_frequency = reln_form->attributeValue( row, "form_ph_frequency" ).toInt();
    form_pf_height = reln_form->attributeValue( row, "form_pf_height" ).toInt();
    form_pf_frequency = reln_form->attributeValue( row, "form_pf_frequency" ).toInt();
    form_dt_height = reln_form->attributeValue( row, "form_dt_height" ).toInt();
    form__sequence = reln_form->attributeValue( row, "form__sequence" ).toInt();
    form__timestamp = reln_form->attributeValue( row, "form__timestamp" );
    form__user_entry = reln_form->attributeValue( row, "form__user_entry" );
    form__status = reln_form->attributeValue( row, "form__status" );

    return ok;
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
bool formHeader::update( const int row, QmvSqlClass * reln_form )
{
    bool ok = true;
    
    reln_form->update( row, "form_code" , form_code );
    reln_form->update( row, "form_desc" , form_desc );
    reln_form->update( row, "form_classname" , form_classname );
    reln_form->update( row, "form_page_attribute" , form_page_attribute );
    reln_form->update( row, "form_includes" , form_includes );
    reln_form->update( row, "form_pagesize" , form_pagesize );
    reln_form->update( row, "form_orientation" , form_orientation );
    reln_form->update( row, "form_mg_top" , QString("%1").arg(form_mg_top) );
    reln_form->update( row, "form_mg_bottom" , QString("%1").arg(form_mg_bottom) );
    reln_form->update( row, "form_mg_left" , QString("%1").arg(form_mg_left) );
    reln_form->update( row, "form_mg_right" , QString("%1").arg(form_mg_right) );
    reln_form->update( row, "form_rh_height" , QString("%1").arg(form_rh_height) );
    reln_form->update( row, "form_rh_frequency" , QString("%1").arg(form_rh_frequency) );
    reln_form->update( row, "form_rf_height" , QString("%1").arg(form_rf_height) );
    reln_form->update( row, "form_rf_frequency" , QString("%1").arg(form_rf_frequency) );
    reln_form->update( row, "form_ph_height" , QString("%1").arg(form_ph_height) );
    reln_form->update( row, "form_ph_frequency" , QString("%1").arg(form_ph_frequency) );
    reln_form->update( row, "form_pf_height" , QString("%1").arg(form_pf_height) );
    reln_form->update( row, "form_pf_frequency" , QString("%1").arg(form_pf_frequency) );
    reln_form->update( row, "form_dt_height" , QString("%1").arg(form_dt_height) );
    reln_form->update( row, "form__sequence" , QString("%1").arg(form__sequence) );
    reln_form->update( row, "form__timestamp" , form__timestamp );
    reln_form->update( row, "form__user_entry" , form__user_entry );
    reln_form->update( row, "form__status" , form__status );

    return ok;
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
bool formDetail::load( const int row, QmvSqlClass * reln_fmdt )
{
    bool ok = true;
    
    fmdt_form_code = reln_fmdt->attributeValue( row, "fmdt_form_code" );
    fmdt_desc = reln_fmdt->attributeValue( row, "fmdt_desc" );
    fmdt_field_type = reln_fmdt->attributeValue( row, "fmdt_field_type" ).toInt();
    fmdt_section = reln_fmdt->attributeValue( row, "fmdt_section" ).toInt();
    fmdt_text = reln_fmdt->attributeValue( row, "fmdt_text" );
    fmdt_x_coord = reln_fmdt->attributeValue( row, "fmdt_x_coord" ).toInt();
    fmdt_y_coord = reln_fmdt->attributeValue( row, "fmdt_y_coord" ).toInt();
    fmdt_width = reln_fmdt->attributeValue( row, "fmdt_width" ).toInt();
    fmdt_height = reln_fmdt->attributeValue( row, "fmdt_height" ).toInt();
    fmdt_bg_colour = reln_fmdt->attributeValue( row, "fmdt_bg_colour" );
    fmdt_fg_colour = reln_fmdt->attributeValue( row, "fmdt_fg_colour" );
    fmdt_bd_colour = reln_fmdt->attributeValue( row, "fmdt_bd_colour" );
    fmdt_bd_width = reln_fmdt->attributeValue( row, "fmdt_bd_width" ).toInt();
    fmdt_bd_style = reln_fmdt->attributeValue( row, "fmdt_bd_style" ).toInt();
    fmdt_ft_family = reln_fmdt->attributeValue( row, "fmdt_ft_family" );
    fmdt_ft_size = reln_fmdt->attributeValue( row, "fmdt_ft_size" ).toInt();
    fmdt_ft_weight = reln_fmdt->attributeValue( row, "fmdt_ft_weight" ).toInt();
    fmdt_ft_Italic = reln_fmdt->attributeValue( row, "fmdt_ft_Italic" ).toInt();
    fmdt_hz_align = reln_fmdt->attributeValue( row, "fmdt_hz_align" ).toInt();
    fmdt_vt_align = reln_fmdt->attributeValue( row, "fmdt_vt_align" ).toInt();
    fmdt_word_wrap = reln_fmdt->attributeValue( row, "fmdt_word_wrap" ).toInt();
    fmdt_data_type = reln_fmdt->attributeValue( row, "fmdt_data_type" ).toInt();
    fmdt_date_format = reln_fmdt->attributeValue( row, "fmdt_date_format" ).toInt();
    fmdt_precision = reln_fmdt->attributeValue( row, "fmdt_precision" ).toInt();
    fmdt_currency = reln_fmdt->attributeValue( row, "fmdt_currency" ).toInt();
    fmdt_neg_colour = reln_fmdt->attributeValue( row, "fmdt_neg_colour" );
    fmdt_comma_sep = reln_fmdt->attributeValue( row, "fmdt_comma_sep" ).toInt();
    fmdt__sequence = reln_fmdt->attributeValue( row, "fmdt__sequence" ).toInt();
    fmdt__timestamp = reln_fmdt->attributeValue( row, "fmdt__timestamp" );
    fmdt__user_entry = reln_fmdt->attributeValue( row, "fmdt__user_entry" );
    fmdt__status = reln_fmdt->attributeValue( row, "fmdt__status" );

    return ok;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
bool formDetail::update( const int row, QmvSqlClass * reln_fmdt )
{
    bool ok = true;
    
  reln_fmdt->update( row, "fmdt_form_code" , fmdt_form_code );
  reln_fmdt->update( row, "fmdt_desc" , fmdt_desc );
  reln_fmdt->update( row, "fmdt_field_type" , QString("%1").arg(fmdt_field_type) );
  reln_fmdt->update( row, "fmdt_section" , QString("%1").arg(fmdt_section) );
  reln_fmdt->update( row, "fmdt_text" , fmdt_text );
  reln_fmdt->update( row, "fmdt_x_coord" , QString("%1").arg(fmdt_x_coord) );
  reln_fmdt->update( row, "fmdt_y_coord" , QString("%1").arg(fmdt_y_coord) );
  reln_fmdt->update( row, "fmdt_width" , QString("%1").arg(fmdt_width) );
  reln_fmdt->update( row, "fmdt_height" , QString("%1").arg(fmdt_height) );
  reln_fmdt->update( row, "fmdt_bg_colour" , fmdt_bg_colour );
  reln_fmdt->update( row, "fmdt_fg_colour" , fmdt_fg_colour );
  reln_fmdt->update( row, "fmdt_bd_colour" , fmdt_bd_colour );
  reln_fmdt->update( row, "fmdt_bd_width" , QString("%1").arg(fmdt_bd_width) );
  reln_fmdt->update( row, "fmdt_bd_style" , QString("%1").arg(fmdt_bd_style) );
  reln_fmdt->update( row, "fmdt_ft_family" , fmdt_ft_family );
  reln_fmdt->update( row, "fmdt_ft_size" , QString("%1").arg(fmdt_ft_size) );
  reln_fmdt->update( row, "fmdt_ft_weight" , QString("%1").arg(fmdt_ft_weight) );
  reln_fmdt->update( row, "fmdt_ft_Italic" , QString("%1").arg(fmdt_ft_Italic) );
  reln_fmdt->update( row, "fmdt_hz_align" , QString("%1").arg(fmdt_hz_align) );
  reln_fmdt->update( row, "fmdt_vt_align" , QString("%1").arg(fmdt_vt_align) );
  reln_fmdt->update( row, "fmdt_word_wrap" , QString("%1").arg(fmdt_word_wrap) );
  reln_fmdt->update( row, "fmdt_data_type" , QString("%1").arg(fmdt_data_type) );
  reln_fmdt->update( row, "fmdt_date_format" , QString("%1").arg(fmdt_date_format) );
  reln_fmdt->update( row, "fmdt_precision" , QString("%1").arg(fmdt_precision) );
  reln_fmdt->update( row, "fmdt_currency" , QString("%1").arg(fmdt_currency) );
  reln_fmdt->update( row, "fmdt_neg_colour" , fmdt_neg_colour );
  reln_fmdt->update( row, "fmdt_comma_sep" , QString("%1").arg(fmdt_comma_sep) );
  reln_fmdt->update( row, "fmdt__sequence" , QString("%1").arg(fmdt__sequence) );
  reln_fmdt->update( row, "fmdt__timestamp" , fmdt__timestamp );
  reln_fmdt->update( row, "fmdt__user_entry" , fmdt__user_entry );
  reln_fmdt->update( row, "fmdt__status" , fmdt__status );

    return ok;
}
