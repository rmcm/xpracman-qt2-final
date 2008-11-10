--     $Id$
-- 
--   Copyright 2003 X=X Computer Software Trust
--    		  Kangaroo Ground Australia 3097
-- 
-- 
--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License published by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
--  
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--  
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
--  
--  Report problems and direct all questions to:
--  
--      Rex McMaster, rmcm@compsoft.com.au
--  
-- 


-- Patch file
-- 
-- Adjust column widths

-- make sure timestamps are specified
update mtat set mtat_type = 'timestamp' where mtat_type = 'datetime';

-- patn
update mtat set mtat_displen =  1 where mtat_name = 'patn_aboriginality';
update mtat set mtat_displen =  3 where mtat_name = 'patn_accl_code';
update mtat set mtat_displen =  1 where mtat_name = 'patn_accommodation';
update mtat set mtat_displen =  1 where mtat_name = 'patn_care';
update mtat set mtat_displen =  4 where mtat_name = 'patn_country';
update mtat set mtat_displen =  1 where mtat_name = 'patn_marital';
update mtat set mtat_displen =  1 where mtat_name = 'patn_sex';
update mtat set mtat_displen = 15 where mtat_name = 'patn_psnam';
update mtat set mtat_displen = 10 where mtat_name = 'patn_fsnam';
update mtat set mtat_displen =  5 where mtat_name = 'patn_feet_code';
update mtat set mtat_displen =  5 where mtat_name = 'patn_patf_code';
update mtat set mtat_displen =  6 where mtat_name = 'patn_title';
update mtat set mtat_displen = 10 where mtat_name = 'patn_dob';
update mtat set mtat_displen = 15 where mtat_name = 'patn_address';
update mtat set mtat_displen = 15 where mtat_name = 'patn_suburb';
update mtat set mtat_displen =  3 where mtat_name = 'patn_state';
update mtat set mtat_displen =  4 where mtat_name = 'patn_postcode';
update mtat set mtat_displen = 15 where mtat_name = 'patn_phone';
update mtat set mtat_displen = 10 where mtat_name = 'patn_flno';
update mtat set mtat_displen = 15 where mtat_name = 'patn_medicare';
update mtat set mtat_displen = 15 where mtat_name = 'patn_healthcard';
update mtat set mtat_displen = 10 where mtat_name = 'patn_hlfd_code';
update mtat set mtat_displen =  3 where mtat_name = 'patn_ins_level';
update mtat set mtat_displen = 15 where mtat_name = 'patn_healthnumb';
update mtat set mtat_displen = 12 where mtat_name = 'patn_dbtr_code';
update mtat set mtat_displen = 12 where mtat_name = 'patn_rfdr_code';
update mtat set mtat_displen = 10 where mtat_name = 'patn_ref_date';
update mtat set mtat_displen =  5 where mtat_name = 'patn_ref_period';
update mtat set mtat_displen =  5 where mtat_name = 'patn_prov_code';
update mtat set mtat_displen = 12 where mtat_name = 'patn_empl_code';
update mtat set mtat_displen = 12 where mtat_name = 'patn__sequence';
update mtat set mtat_displen = 15 where mtat_name = 'patn__timestamp';
update mtat set mtat_displen =  8 where mtat_name = 'patn__user_entry';
update mtat set mtat_displen =  1 where mtat_name = 'patn__status';

-- svpf
update mtat set mtat_displen = 12 where mtat_name = 'svpf_mdaf__sequence';
update mtat set mtat_displen = 10 where mtat_name = 'svpf_date_service';
update mtat set mtat_displen =  8 where mtat_name = 'svpf_serv_code';
update mtat set mtat_displen =  3 where mtat_name = 'svpf_percentage';
update mtat set mtat_displen = 30 where mtat_name = 'svpf_desc';
update mtat set mtat_displen =  8 where mtat_name = 'svpf_amount';
update mtat set mtat_displen =  8 where mtat_name = 'svpf_gst_amount';
update mtat set mtat_displen = 12 where mtat_name = 'svpf_invc__sequence';
update mtat set mtat_displen = 12 where mtat_name = 'svpf_patn__sequence';
update mtat set mtat_displen = 12 where mtat_name = 'svpf__sequence';
update mtat set mtat_displen = 12 where mtat_name = 'svpf__timestamp';
update mtat set mtat_displen = 12 where mtat_name = 'svpf__user_entry';
update mtat set mtat_displen =  1 where mtat_name = 'svpf__status';

-- invc

update mtat set mtat_displen = 12 where mtat_name = 'invc__sequence';
update mtat set mtat_displen = 10 where mtat_name = 'invc_amount';
update mtat set mtat_displen =  8 where mtat_name = 'invc_gst_amount';
update mtat set mtat_displen = 10 where mtat_name = 'invc_paid_amount';
update mtat set mtat_displen =  8 where mtat_name = 'invc_paid_gst_amount';
update mtat set mtat_displen = 12 where mtat_name = 'invc_dbtr_code';
update mtat set mtat_displen =  4 where mtat_name = 'invc_bank_code';
update mtat set mtat_displen =  8 where mtat_name = 'invc_prov_code';
update mtat set mtat_displen =  8 where mtat_name = 'invc_patn__sequence';
update mtat set mtat_displen =  8 where mtat_name = 'invc_empl_code';
update mtat set mtat_displen =  8 where mtat_name = 'invc_feet_code';
update mtat set mtat_displen =  8 where mtat_name = 'invc_rfdr_code';
update mtat set mtat_displen = 10 where mtat_name = 'invc_rfdr_date';
update mtat set mtat_displen =  4 where mtat_name = 'invc_rfdr_period';
update mtat set mtat_displen = 10 where mtat_name = 'invc_date_created';
update mtat set mtat_displen = 10 where mtat_name = 'invc_date_printed';
update mtat set mtat_displen = 10 where mtat_name = 'invc_date_reprint';
update mtat set mtat_displen = 15 where mtat_name = 'invc__timestamp';
update mtat set mtat_displen =  8 where mtat_name = 'invc__user_entry';
update mtat set mtat_displen =  1 where mtat_name = 'invc__status';

-- crev

update mtat set mtat_view_order = 122 where mtat_name = 'crev_invc__sequence';
update mtat set mtat_view_order = 124 where mtat_name = 'crev_site_entry';
update mtat set mtat_view_order = 126 where mtat_name = 'crev_paym__sequence';
update mtat set mtat_title = 'Payment ID', mtat_displen = 10 where mtat_name = 'crev_paym__sequence';

-- rfdr

update mtat set mtat_displen = 10 where mtat_name = 'rfdr_code';
update mtat set mtat_displen = 10 where mtat_name = 'rfdr_postcode';
update mtat set mtat_displen = 10 where mtat_name = 'rfdr_state';
update mtat set mtat_displen = 10 where mtat_name = 'rfdr_provider';
update mtat set mtat_displen = 10 where mtat_name = 'rfdr_salutation';
update mtat set mtat_displen = 5 where mtat_name = 'rfdr_index';

-- paym

update mtat set mtat_view_order = 100 where mtat_name = 'paym__user_entry';
update mtat set mtat_displen = 10 where mtat_name in ('paym_user_entry','paym__user_entry');
update mtat set mtat_displen = 10 where mtat_name = 'paym_tdtp_code';
update mtat set mtat_displen = 10 where mtat_name = 'paym_bank';
update mtat set mtat_displen = 10 where mtat_name = 'paym_bkdp__sequence';
update mtat set mtat_displen = 10 where mtat_name = 'paym_amount';
update mtat set mtat_displen = 1 where mtat_name = 'paym__status';
update mtat set mtat_title = 'Payment ID', mtat_displen = 10 where mtat_name = 'paym__sequence';
update mtat set mtat_input_method = '' where mtat_name = 'paym_date_entry';
update mtat set mtat_displen = 5, mtat_input_method = 'RO', mtat_view_order = 95  where mtat_name = 'paym_site_entry';

-- bkdp

update mtat set mtat_displen = 10 where mtat_name in ('bkdp__sequence', 'bkdp_date_printed');

-- form

update mtat set mtat_displen = 5 where mtat_name ~ '^form_';
update mtat set mtat_displen = 10 where mtat_name in ('form_code', 'form_desc', 'form_page_attribute', 'form_includes');

-- fmdt

update mtat set mtat_displen = 15 where mtat_name in ('fmdt_text');
update mtat set mtat_displen = 10 where mtat_name in ('fmdt_form_code', 'fmdt_desc', 'fmdt_ft_family');
update mtat set mtat_displen = 5 where mtat_name in ('fmdt_field_type', 'fmdt_section', 'fmdt_x_coord', 'fmdt_y_coord');
update mtat set mtat_displen = 5 where mtat_name in ('fmdt_width', 'fmdt_height', 'fmdt_hz_align', 'fmdt_vt_align');
update mtat set mtat_displen = 5 where mtat_name in ('fmdt_ft_size', 'fmdt_ft_weight', 'fmdt_ft_italic');
update mtat set mtat_displen = 5 where mtat_name in ('fmdt_date_format', 'fmdt_precision', 'fmdt_word_wrap');
update mtat set mtat_displen = 5 where mtat_name in ('fmdt_data_type', 'fmdt_currency', 'fmdt_comma_sep');
update mtat set mtat_displen = 5 where mtat_name in ('fmdt_word_wrap', 'fmdt_bd_width', 'fmdt_bd_style');
update mtat set mtat_displen = 10, mtat_input_method = 'COLOUR' where mtat_name ~ '^fmdt_.*_colour';
update mtat set mtat_view_order = (select mtat_view_order from mtat where mtat_name = 'fmdt_height') + 1 
        where mtat_name = 'fmdt_ft_size';
update mtat set mtat_view_order = (select mtat_view_order from mtat where mtat_name = 'fmdt_height') + 2
        where mtat_name = 'fmdt_ft_weight';
update mtat set mtat_view_order = (select mtat_view_order from mtat where mtat_name = 'fmdt_height') + 3 
        where mtat_name = 'fmdt_hz_align';
update mtat set mtat_view_order = (select mtat_view_order from mtat where mtat_name = 'fmdt_height') + 4 
        where mtat_name = 'fmdt_vt_align';

-- aptd
update mtat set mtat_displen = 5 where mtat_name ~ '^aptd_[a-z]*';
update mtat set mtat_displen = 15 where mtat_name = 'aptd_desc';
update mtat set mtat_displen = 7 where mtat_name in ('aptd_start_date','aptd_end_date');
update mtat set mtat_displen = 3 where mtat_name in ('aptd_dayofweek','aptd_dayofmonth','aptd_monthofyear','');

-- tdtp
update mtat set mtat_displen = 5 where mtat_name in ('tdtp_code', 'tdtp_entity', 'tdtp_location');


-- register the patch
insert into mtpt(mtpt_name, mtpt_rcs_header) values('mtat-widths.sql', '$Id$');
