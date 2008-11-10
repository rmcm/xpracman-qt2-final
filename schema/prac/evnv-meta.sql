--     $Id$
-- 
--   Copyright 2006 X=X Computer Software Trust
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


update mtcl set mtcl_title = 'Appointments View', mtcl_order_by = 'order by evnv_patn__sequence,evnv_starttime', mtcl_userlabel = '<b>%1 %2 (%3)</b><br><i>%4</i>, evnv_patn_fsnam,evnv_patn_psnam, evnv_patn_dob,,evnv_desc' where mtcl_name = 'evnv';
-- update mtat set mtat_title = '_@ATT_TITLE@_', mtat_displen = _@ATT_DISPLEN@_, mtat_view_order = _@ATT_VIEW_ORDER@_, mtat_input_method = '_@ATT_INPUT_METHOD@_'  where mtat_name = '_@ATT_NAME@_';

update mtat set mtat_title = 'Start', mtat_displen = 16, mtat_view_order = 10, mtat_input_method = '10' where mtat_name = 'evnv_starttime';
update mtat set mtat_title = 'Duration', mtat_displen = 47, mtat_view_order = 20, mtat_input_method = '10' where mtat_name = 'evnv_duration';
update mtat set mtat_title = 'Note', mtat_displen = 30, mtat_view_order = 30, mtat_input_method = '10' where mtat_name = 'evnv_desc';
update mtat set mtat_title = 'Note 1', mtat_displen = 30, mtat_view_order = 40, mtat_input_method = '10' where mtat_name = 'evnv_note_1';
update mtat set mtat_title = 'Note 2', mtat_displen = 30, mtat_view_order = 50, mtat_input_method = '10' where mtat_name = 'evnv_note_2';
update mtat set mtat_title = 'Provider', mtat_displen = 30, mtat_view_order = 52, mtat_input_method = 'FSL=20' where mtat_name = 'evnv_prov_code';
update mtat set mtat_title = 'Location', mtat_displen = 30, mtat_view_order = 60, mtat_input_method = 'FSL=30' where mtat_name = 'evnv_locn_code';
update mtat set mtat_title = 'Type', mtat_displen = 30, mtat_view_order = 70, mtat_input_method = 'FSL=20' where mtat_name = 'evnv_aptp_code';
update mtat set mtat_title = 'Status', mtat_displen = 30, mtat_view_order = 80, mtat_input_method = 'FSL=20' where mtat_name = 'evnv_apst_code';
update mtat set mtat_title = 'Patient ID', mtat_displen = 12, mtat_view_order = 110, mtat_input_method = 'RO' where mtat_name = 'evnv_patn__sequence';
update mtat set mtat_title = 'Surname', mtat_displen = 30, mtat_view_order = 120, mtat_input_method = '10' where mtat_name = 'evnv_patn_psnam';
update mtat set mtat_title = 'Firstname', mtat_displen = 30, mtat_view_order = 130, mtat_input_method = '10' where mtat_name = 'evnv_patn_fsnam';
update mtat set mtat_title = 'Title', mtat_displen = 30, mtat_view_order = 140, mtat_input_method = '10' where mtat_name = 'evnv_patn_title';
update mtat set mtat_title = 'DOB', mtat_displen = 30, mtat_view_order = 150, mtat_input_method = '10' where mtat_name = 'evnv_patn_dob';
update mtat set mtat_title = 'Address', mtat_displen = 30, mtat_view_order = 160, mtat_input_method = '10' where mtat_name = 'evnv_patn_address';
update mtat set mtat_title = 'Suburb', mtat_displen = 30, mtat_view_order = 170, mtat_input_method = 'COPYTEXT=pcde;pcde_locality;pcde_postcode;evnv_patn_postcode' where mtat_name = 'evnv_patn_suburb';
update mtat set mtat_title = 'Postcode', mtat_displen = 30, mtat_view_order = 180, mtat_input_method = 'COPYTEXT=pcde;pcde_postcode;pcde_locality;evnv_patn_suburb' where mtat_name = 'evnv_patn_postcode';
update mtat set mtat_title = 'Phone(H)', mtat_displen = 30, mtat_view_order = 190, mtat_input_method = '10' where mtat_name = 'evnv_patn_phone';
update mtat set mtat_title = 'Phone(W)', mtat_displen = 30, mtat_view_order = 192, mtat_input_method = '10' where mtat_name = 'evnv_patn_phone_1';
update mtat set mtat_title = 'Phone(M)', mtat_displen = 30, mtat_view_order = 194, mtat_input_method = '10' where mtat_name = 'evnv_patn_phone_2';
update mtat set mtat_title = 'Patient UR', mtat_displen = 30, mtat_view_order = 200, mtat_input_method = '10' where mtat_name = 'evnv_patn_flno';
update mtat set mtat_title = 'Referrer Code', mtat_displen = 30, mtat_view_order = 208, mtat_input_method = '10' where mtat_name = 'evnv_rfdr_code';
update mtat set mtat_title = 'Referral Date', mtat_displen = 30, mtat_view_order = 210, mtat_input_method = '10' where mtat_name = 'evnv_patn_ref_date';
update mtat set mtat_title = 'Referral Period', mtat_displen = 30, mtat_view_order = 220, mtat_input_method = '10' where mtat_name = 'evnv_patn_ref_period';
update mtat set mtat_title = 'Referral Expires', mtat_displen = 30, mtat_view_order = 222, mtat_input_method = 'RO' where mtat_name = 'evnv_patn_referral_expires';
update mtat set mtat_title = 'Last Visit', mtat_displen = 30, mtat_view_order = 230, mtat_input_method = 'RO' where mtat_name = 'evnv_patn_last_visit';
update mtat set mtat_title = 'Last Service', mtat_displen = 30, mtat_view_order = 232, mtat_input_method = 'RO' where mtat_name = 'evnv_patn_last_service';
update mtat set mtat_title = 'Amount Owing', mtat_displen = 786434, mtat_view_order = 240, mtat_input_method = 'RO' where mtat_name = 'evnv_patn_amount_outstanding';

update mtat set mtat_title = 'Ev Time', mtat_displen = 30, mtat_view_order = 1000, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_ev_time';
update mtat set mtat_title = 'Ev Date', mtat_displen = 30, mtat_view_order = 1010, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_ev_date';
update mtat set mtat_title = 'Ev Minutes', mtat_displen = 24, mtat_view_order = 1020, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_ev_minutes';
update mtat set mtat_title = 'Patn Desc', mtat_displen = 30, mtat_view_order = 1030, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_desc';
update mtat set mtat_title = 'Patn State', mtat_displen = 30, mtat_view_order = 1040, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_state';
update mtat set mtat_title = 'Patn Desc Comp', mtat_displen = 30, mtat_view_order = 1050, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_desc_comp';
update mtat set mtat_title = 'Location Name', mtat_displen = 30, mtat_view_order = 1080, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_locn_desc';
update mtat set mtat_title = 'Provider Number', mtat_displen = 30, mtat_view_order = 1090, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_prov_provider_num';
update mtat set mtat_title = 'Provider Name', mtat_displen = 30, mtat_view_order = 1100, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_prov_name';
update mtat set mtat_title = 'Provider Address', mtat_displen = 30, mtat_view_order = 1120, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_prov_address';
update mtat set mtat_title = 'Provider Suburb', mtat_displen = 30, mtat_view_order = 1130, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_prov_suburb';
update mtat set mtat_title = 'Provider State', mtat_displen = 30, mtat_view_order = 1140, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_prov_state';
update mtat set mtat_title = 'Aptp Desc', mtat_displen = 30, mtat_view_order =1150, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_aptp_desc';
update mtat set mtat_title = 'Provider PC', mtat_displen = 30, mtat_view_order = 1160, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_prov_postcode';
update mtat set mtat_title = 'Apst Desc', mtat_displen = 30, mtat_view_order = 1170, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_apst_desc';
update mtat set mtat_title = 'Provider Salutation', mtat_displen = 30, mtat_view_order = 1180, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_prov_salutation';
update mtat set mtat_title = 'Prov Phone', mtat_displen = 30, mtat_view_order = 1190, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_prov_phone';
update mtat set mtat_title = 'Prov Bank Code', mtat_displen = 30, mtat_view_order = 1200, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_prov_bank_code';
update mtat set mtat_title = 'Referrer Name', mtat_displen = 30, mtat_view_order = 1210, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_rfdr_name';
update mtat set mtat_title = 'Referrer Street', mtat_displen = 30, mtat_view_order = 1220, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_rfdr_street';
update mtat set mtat_title = 'Referrer Suburb', mtat_displen = 30, mtat_view_order = 1230, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_rfdr_suburb';
update mtat set mtat_title = 'Referrer Postcode', mtat_displen = 30, mtat_view_order = 1240, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_rfdr_postcode';
update mtat set mtat_title = 'Referrer State', mtat_displen = 30, mtat_view_order = 1250, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_rfdr_state';
update mtat set mtat_title = 'Referrer Provider Number', mtat_displen = 30, mtat_view_order = 1260, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_rfdr_provider';
update mtat set mtat_title = 'Referrer Phone', mtat_displen = 30, mtat_view_order = 1270, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_rfdr_phone';
update mtat set mtat_title = 'Referrer Salutation', mtat_displen = 30, mtat_view_order = 1280, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_rfdr_salutation';
update mtat set mtat_title = 'Referrer Index', mtat_displen = 30, mtat_view_order = 1290, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_rfdr_index';
update mtat set mtat_title = 'Country', mtat_displen = 30, mtat_view_order = 1300, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_country';
update mtat set mtat_title = 'Aboriginality', mtat_displen = 1, mtat_view_order = 1310, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_aboriginality';
update mtat set mtat_title = 'Sex', mtat_displen = 1, mtat_view_order = 1320, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_sex';
update mtat set mtat_title = 'Marital', mtat_displen = 1, mtat_view_order = 1330, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_marital';
update mtat set mtat_title = 'Account Class', mtat_displen = 30, mtat_view_order = 1340, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_accl_code';
update mtat set mtat_title = 'Accommodation', mtat_displen = 1, mtat_view_order = 1350, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_accommodation';
update mtat set mtat_title = 'Care', mtat_displen = 1, mtat_view_order = 1360, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_care';
update mtat set mtat_title = 'Sequence', mtat_displen = 10, mtat_view_order = 1370, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv__sequence';
update mtat set mtat_title = 'Patn Hlfd Code', mtat_displen = 30, mtat_view_order = 1380, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_hlfd_code';
update mtat set mtat_title = 'Timestamp', mtat_displen = 47, mtat_view_order = 1390, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv__timestamp';
update mtat set mtat_title = 'Patn Ins Level', mtat_displen = 1, mtat_view_order = 1400, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_ins_level';
update mtat set mtat_title = 'User Entry', mtat_displen = 30, mtat_view_order = 1410, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv__user_entry';
update mtat set mtat_title = 'Patn Healthnumb', mtat_displen = 30, mtat_view_order = 1420, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_healthnumb';
update mtat set mtat_title = 'Status', mtat_displen = 1, mtat_view_order = 1430, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv__status';
update mtat set mtat_title = 'Patn Medicare', mtat_displen = 30, mtat_view_order = 1440, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_medicare';
update mtat set mtat_title = 'Patn Feet Code', mtat_displen = 30, mtat_view_order = 1450, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_feet_code';
update mtat set mtat_title = 'Patn Healthcard', mtat_displen = 30, mtat_view_order = 1460, mtat_input_method = 'SYSTEM' where mtat_name = 'evnv_patn_healthcard';

-- Foreign keys

delete from mtfn where mtfn_master_class = 'evnv';
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Provider','evnv','evnv_prov_code','prov','prov_code','prov_name' );
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Patient','evnv','evnv_patn__sequence','patn','patn__sequence','patn_psnam' );
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Referror','evnv','evnv_rfdr_code','rfdr','rfdr_code','rfdr_name' );
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Location','evnv','evnv_locn_code','locn','locn_code','locn_desc' );
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Type','evnv','evnv_aptp_code','aptp','aptp_code','aptp_desc' );
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Status','evnv','evnv_apst_code','apst','apst_code','apst_desc' );
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Bank','evnv','evnv_prov_bank_code','bank','bank_code','bank_name' );
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'ACCL','evnv','evnv_patn_accl_code','accl','accl_code','accl_desc' );
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Fund','evnv','evnv_patn_hlfd_code','hlfd','hlfd_code','hlfd_desc' );
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Fee Level','evnv','evnv_patn_feet_code','feet','feet_code','feet_desc' );

