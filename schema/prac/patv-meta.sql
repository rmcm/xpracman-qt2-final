--     $Id$
-- 
--   Copyright 2005 X=X Computer Software Trust
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
-- meta data for PATV

update mtcl set mtcl_order_by = 'order by patv_psnam, patv_fsnam, patv__sequence' where mtcl_name = 'patv';
--update mtat set mtat_title = '_@ATT_TITLE@_', mtat_displen = _@ATT_DISPLEN@_, mtat_view_order = _@ATT_VIEW_ORDER@_ where mtat_name = '_@ATT_NAME@_';
update mtat set mtat_title = 'Marital Status', mtat_displen = 1, mtat_view_order = 0 where mtat_name = 'patv_marital';
update mtat set mtat_title = 'Gender', mtat_displen = 1, mtat_view_order = 0 where mtat_name = 'patv_sex';
update mtat set mtat_title = 'Aboriginality', mtat_displen = 1, mtat_view_order = 0 where mtat_name = 'patv_aboriginality';
update mtat set mtat_title = 'Account type', mtat_displen = 3, mtat_view_order = 0 where mtat_name = 'patv_accl_code';
update mtat set mtat_title = 'Accommodation type', mtat_displen = 1, mtat_view_order = 0 where mtat_name = 'patv_accommodation';
update mtat set mtat_title = 'Care type', mtat_displen = 1, mtat_view_order = 0 where mtat_name = 'patv_care';
update mtat set mtat_title = 'Country of birth', mtat_displen = 4, mtat_view_order = 0 where mtat_name = 'patv_country';
update mtat set mtat_title = 'Surname', mtat_displen = 25, mtat_view_order = 30 where mtat_name = 'patv_psnam';
update mtat set mtat_title = 'Firstname', mtat_displen = 20, mtat_view_order = 40 where mtat_name = 'patv_fsnam';
update mtat set mtat_title = 'File number', mtat_displen = 10, mtat_view_order = 45 where mtat_name = 'patv_flno';
update mtat set mtat_title = 'Title', mtat_displen = 6, mtat_view_order = 50 where mtat_name = 'patv_title';
update mtat set mtat_title = 'DOB', mtat_displen = 10, mtat_view_order = 60 where mtat_name = 'patv_dob';
update mtat set mtat_title = 'Street', mtat_displen = 30, mtat_view_order = 70 where mtat_name = 'patv_address';
update mtat set mtat_title = 'Suburb', mtat_displen = 30, mtat_view_order = 80 where mtat_name = 'patv_suburb';
update mtat set mtat_title = 'State', mtat_displen = 3, mtat_view_order = 90 where mtat_name = 'patv_state';
update mtat set mtat_title = 'Postcode', mtat_displen = 4, mtat_view_order = 100 where mtat_name = 'patv_postcode';
update mtat set mtat_title = 'Phone(H)', mtat_displen = 30, mtat_view_order = 110 where mtat_name = 'patv_phone';
update mtat set mtat_title = 'Phone(W)', mtat_displen = 30, mtat_view_order = 110 where mtat_name = 'patv_phone_1';
update mtat set mtat_title = 'Phone(M)', mtat_displen = 30, mtat_view_order = 120 where mtat_name = 'patv_phone_2';
update mtat set mtat_title = 'Amount Outstanding', mtat_displen = 786434, mtat_view_order = 130 where mtat_name = 'patv_amount_outstanding';
update mtat set mtat_title = 'Last Visit', mtat_displen = 30, mtat_view_order = 140 where mtat_name = 'patv_last_visit';
update mtat set mtat_title = 'Fee level (Code)', mtat_displen = 5, mtat_view_order = 150 where mtat_name = 'patv_feet_code';
update mtat set mtat_title = 'Fee Level', mtat_displen = 30, mtat_view_order = 152 where mtat_name = 'patv_feet_desc';
update mtat set mtat_title = 'Last Service', mtat_displen = 30, mtat_view_order = 154 where mtat_name = 'patv_last_service';
update mtat set mtat_title = 'Medicare Number', mtat_displen = 15, mtat_view_order = 160 where mtat_name = 'patv_medicare';
update mtat set mtat_title = 'Claim Number', mtat_displen = 30, mtat_view_order = 162 where mtat_name = 'patv_claim_number';
update mtat set mtat_title = 'Reference', mtat_displen = 15, mtat_view_order = 170 where mtat_name = 'patv_healthcard';
update mtat set mtat_title = 'Accident Date', mtat_displen = 30, mtat_view_order = 170 where mtat_name = 'patv_accident_date';
update mtat set mtat_title = 'Health Fund (Code)', mtat_displen = 10, mtat_view_order = 172 where mtat_name = 'patv_hlfd_code';
update mtat set mtat_title = 'Health Fund', mtat_displen = 30, mtat_view_order = 173 where mtat_name = 'patv_hlfd_desc';
update mtat set mtat_title = 'Fund Group', mtat_displen = 30, mtat_view_order = 174 where mtat_name = 'patv_hlfd_group';
update mtat set mtat_title = 'Health Fund number', mtat_displen = 30, mtat_view_order = 176 where mtat_name = 'patv_healthnumb';
update mtat set mtat_title = 'Insurance level', mtat_displen = 3, mtat_view_order = 178 where mtat_name = 'patv_ins_level';
update mtat set mtat_title = 'Usual Debtor (code)', mtat_displen = 12, mtat_view_order = 180 where mtat_name = 'patv_dbtr_code';
update mtat set mtat_title = 'Debtor Name', mtat_displen = 30, mtat_view_order = 182 where mtat_name = 'patv_dbtr_name';
update mtat set mtat_title = 'Debtor Address', mtat_displen = 30, mtat_view_order = 184 where mtat_name = 'patv_dbtr_address';
update mtat set mtat_title = 'Debtor Suburb', mtat_displen = 30, mtat_view_order = 186 where mtat_name = 'patv_dbtr_suburb';
update mtat set mtat_title = 'Debtor Postcode', mtat_displen = 30, mtat_view_order = 187 where mtat_name = 'patv_dbtr_postcode';
update mtat set mtat_title = 'Debtor State', mtat_displen = 30, mtat_view_order = 188 where mtat_name = 'patv_dbtr_state';
update mtat set mtat_title = 'Debtor Phone', mtat_displen = 30, mtat_view_order = 189 where mtat_name = 'patv_dbtr_phone';
update mtat set mtat_title = 'Usual Referrer (code)', mtat_displen = 12, mtat_view_order = 200 where mtat_name = 'patv_rfdr_code';
update mtat set mtat_title = 'Referrer Name', mtat_displen = 30, mtat_view_order = 201 where mtat_name = 'patv_rfdr_name';
update mtat set mtat_title = 'Referrer Street', mtat_displen = 30, mtat_view_order = 202 where mtat_name = 'patv_rfdr_street';
update mtat set mtat_title = 'Referrer Suburb', mtat_displen = 30, mtat_view_order = 203 where mtat_name = 'patv_rfdr_suburb';
update mtat set mtat_title = 'Referrer Postcode', mtat_displen = 30, mtat_view_order = 204 where mtat_name = 'patv_rfdr_postcode';
update mtat set mtat_title = 'Referrer State', mtat_displen = 30, mtat_view_order = 205 where mtat_name = 'patv_rfdr_state';
update mtat set mtat_title = 'Referrer Phone', mtat_displen = 30, mtat_view_order = 206 where mtat_name = 'patv_rfdr_phone';
update mtat set mtat_title = 'Referrer Salutation', mtat_displen = 30, mtat_view_order = 207 where mtat_name = 'patv_rfdr_salutation';
update mtat set mtat_title = 'Referrer Provider', mtat_displen = 30, mtat_view_order = 208 where mtat_name = 'patv_rfdr_provider';
update mtat set mtat_title = 'Referrer Index', mtat_displen = 30, mtat_view_order = 209 where mtat_name = 'patv_rfdr_index';
update mtat set mtat_title = 'Referral date', mtat_displen = 12, mtat_view_order = 210 where mtat_name = 'patv_ref_date';
update mtat set mtat_title = 'Referral period', mtat_displen = 5, mtat_view_order = 220 where mtat_name = 'patv_ref_period';
update mtat set mtat_title = 'Usual provider (code)', mtat_displen = 5, mtat_view_order = 230 where mtat_name = 'patv_prov_code';
update mtat set mtat_title = 'Provider Name', mtat_displen = 30, mtat_view_order = 231 where mtat_name = 'patv_prov_name';
update mtat set mtat_title = 'Provider Address', mtat_displen = 30, mtat_view_order = 232 where mtat_name = 'patv_prov_address';
update mtat set mtat_title = 'Provider Suburb', mtat_displen = 30, mtat_view_order = 233 where mtat_name = 'patv_prov_suburb';
update mtat set mtat_title = 'Provider Postcode', mtat_displen = 30, mtat_view_order = 234 where mtat_name = 'patv_prov_postcode';
update mtat set mtat_title = 'Provider State', mtat_displen = 30, mtat_view_order = 235 where mtat_name = 'patv_prov_state';
update mtat set mtat_title = 'Provider Phone', mtat_displen = 30, mtat_view_order = 236 where mtat_name = 'patv_prov_phone';
update mtat set mtat_title = 'Provider Salutation', mtat_displen = 30, mtat_view_order = 237 where mtat_name = 'patv_prov_salutation';
update mtat set mtat_title = 'Provider Provider Num', mtat_displen = 30, mtat_view_order = 238 where mtat_name = 'patv_prov_provider_num';
update mtat set mtat_title = 'Provider Bank Code', mtat_displen = 30, mtat_view_order = 239 where mtat_name = 'patv_prov_bank_code';
update mtat set mtat_title = 'Usual Employer (code)', mtat_displen = 12, mtat_view_order = 280 where mtat_name = 'patv_empl_code';
update mtat set mtat_title = 'Employer Name', mtat_displen = 30, mtat_view_order = 281 where mtat_name = 'patv_empl_name';
update mtat set mtat_title = 'Employer Address', mtat_displen = 30, mtat_view_order = 282 where mtat_name = 'patv_empl_address';
update mtat set mtat_title = 'Employer Suburb', mtat_displen = 30, mtat_view_order = 283 where mtat_name = 'patv_empl_suburb';
update mtat set mtat_title = 'Employer Postcode', mtat_displen = 30, mtat_view_order = 310 where mtat_name = 'patv_empl_postcode';
update mtat set mtat_title = 'Employer State', mtat_displen = 30, mtat_view_order = 320 where mtat_name = 'patv_empl_state';
update mtat set mtat_title = 'Employer Phone', mtat_displen = 30, mtat_view_order = 400 where mtat_name = 'patv_empl_phone';
update mtat set mtat_title = 'Employer Notes', mtat_displen = 30, mtat_view_order = 410 where mtat_name = 'patv_empl_notes';
update mtat set mtat_title = 'Row-ID', mtat_displen = 12, mtat_view_order = 510 where mtat_name = 'patv__sequence';
update mtat set mtat_title = 'Timestamp', mtat_displen = 12, mtat_view_order = 520 where mtat_name = 'patv__timestamp';
update mtat set mtat_title = 'Entered by', mtat_displen = 12, mtat_view_order = 530 where mtat_name = 'patv__user_entry';
update mtat set mtat_title = 'Record status', mtat_displen = 12, mtat_view_order = 540 where mtat_name = 'patv__status';

-- update history;
\set mttb_name '\'patv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

