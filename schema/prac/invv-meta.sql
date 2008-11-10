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
-- Meta Data for INVV

update mtcl set mtcl_order_by = 'order by invv_invc__sequence desc, invv_amount desc' where mtcl_name = 'invv';

--update mtat set mtat_title = '_@ATT_TITLE@_', mtat_displen = _@ATT_DISPLEN@_, mtat_view_order = _@ATT_VIEW_ORDER@_ where mtat_name = '_@ATT_NAME@_';

update mtat set mtat_title = 'ID', mtat_displen = 5  where mtat_name = 'invv__sequence';
update mtat set mtat_title = 'Status', mtat_displen = 5  where mtat_name = 'invv__status';
update mtat set mtat_title = 'Timestamp', mtat_displen = 5  where mtat_name = 'invv__timestamp';
update mtat set mtat_title = 'User Entry', mtat_displen = 5  where mtat_name = 'invv__user_entry';
update mtat set mtat_title = 'Credit Summary', mtat_displen = 5  where mtat_name = 'invv_cred_summary';
update mtat set mtat_title = 'Date Printed', mtat_displen = 5  where mtat_name = 'invv_date_printed';
update mtat set mtat_title = 'Date Reprinted', mtat_displen = 5  where mtat_name = 'invv_date_reprint';
update mtat set mtat_title = 'Debtor Address', mtat_displen = 5  where mtat_name = 'invv_dbtr_address';
update mtat set mtat_title = 'Debtor Full Address', mtat_displen = 5  where mtat_name = 'invv_dbtr_full_address';
update mtat set mtat_title = 'Debtor Name', mtat_displen = 5  where mtat_name = 'invv_dbtr_name';
update mtat set mtat_title = 'Debtor State', mtat_displen = 5  where mtat_name = 'invv_dbtr_state';
update mtat set mtat_title = 'Debtor Suburb', mtat_displen = 5  where mtat_name = 'invv_dbtr_suburb';
update mtat set mtat_title = 'Employer Address', mtat_displen = 5  where mtat_name = 'invv_empl_address';
update mtat set mtat_title = 'Employer Code', mtat_displen = 5  where mtat_name = 'invv_empl_code';
update mtat set mtat_title = 'Employer Name', mtat_displen = 5  where mtat_name = 'invv_empl_name';
update mtat set mtat_title = 'Employer Postcode', mtat_displen = 5  where mtat_name = 'invv_empl_postcode';
update mtat set mtat_title = 'Employer State', mtat_displen = 5  where mtat_name = 'invv_empl_state';
update mtat set mtat_title = 'Employer Suburb', mtat_displen = 5  where mtat_name = 'invv_empl_suburb';
update mtat set mtat_title = 'Fee Level Code', mtat_displen = 5  where mtat_name = 'invv_feet_code';
update mtat set mtat_title = 'Fee Level', mtat_displen = 5  where mtat_name = 'invv_feet_desc';
update mtat set mtat_title = 'Fund', mtat_displen = 5  where mtat_name = 'invv_hlfd_desc';
update mtat set mtat_title = 'Invoice ID', mtat_displen = 5  where mtat_name = 'invv_invc__sequence';
update mtat set mtat_title = 'Invoice Accident Date', mtat_displen = 5  where mtat_name = 'invv_invc_accident_date';
update mtat set mtat_title = 'Invoice Amount', mtat_displen = 5  where mtat_name = 'invv_invc_amount';
update mtat set mtat_title = 'Invoice Balance', mtat_displen = 5  where mtat_name = 'invv_invc_balance';
update mtat set mtat_title = 'Invoice Claim Number', mtat_displen = 5  where mtat_name = 'invv_invc_claim_number';
update mtat set mtat_title = 'Invoice Credits', mtat_displen = 5  where mtat_name = 'invv_invc_credits';
update mtat set mtat_title = 'Invoice Date Printed', mtat_displen = 5  where mtat_name = 'invv_invc_date_printed';
update mtat set mtat_title = 'Invoice Date Reprint', mtat_displen = 5  where mtat_name = 'invv_invc_date_reprint';
update mtat set mtat_title = 'Invoice Gst Amount', mtat_displen = 5  where mtat_name = 'invv_invc_gst_amount';
update mtat set mtat_title = 'Invoice Healthcard', mtat_displen = 5  where mtat_name = 'invv_invc_healthcard';
update mtat set mtat_title = 'Invoice Healthnumb', mtat_displen = 5  where mtat_name = 'invv_invc_healthnumb';
update mtat set mtat_title = 'Invoice Ins Level', mtat_displen = 5  where mtat_name = 'invv_invc_ins_level';
update mtat set mtat_title = 'Invoice Medicare', mtat_displen = 5  where mtat_name = 'invv_invc_medicare';
update mtat set mtat_title = 'Invoice Reference 1', mtat_displen = 5  where mtat_name = 'invv_invc_reference_1';
update mtat set mtat_title = 'Invoice Reference 2', mtat_displen = 5  where mtat_name = 'invv_invc_reference_2';
update mtat set mtat_title = 'Invoice Reference 3', mtat_displen = 5  where mtat_name = 'invv_invc_reference_3';
update mtat set mtat_title = 'Patient ID', mtat_displen = 5  where mtat_name = 'invv_patn__sequence';
update mtat set mtat_title = 'Patient Accident Date', mtat_displen = 5  where mtat_name = 'invv_patn_accident_date';
update mtat set mtat_title = 'Patient Address', mtat_displen = 5  where mtat_name = 'invv_patn_address';
update mtat set mtat_title = 'Patient Claim Number', mtat_displen = 5  where mtat_name = 'invv_patn_claim_number';
update mtat set mtat_title = 'Patient DOB', mtat_displen = 5  where mtat_name = 'invv_patn_dob';
update mtat set mtat_title = 'Patient Healthcard', mtat_displen = 5  where mtat_name = 'invv_patn_healthcard';
update mtat set mtat_title = 'Patient Healthnumb', mtat_displen = 5  where mtat_name = 'invv_patn_healthnumb';
update mtat set mtat_title = 'Patient Fund', mtat_displen = 5  where mtat_name = 'invv_patn_hlfd_code';
update mtat set mtat_title = 'Patient Fund Level', mtat_displen = 5  where mtat_name = 'invv_patn_ins_level';
update mtat set mtat_title = 'Patient Medicare', mtat_displen = 5  where mtat_name = 'invv_patn_medicare';
update mtat set mtat_title = 'Patient Name', mtat_displen = 5  where mtat_name = 'invv_patn_name';
update mtat set mtat_title = 'Patient Flag', mtat_displen = 5  where mtat_name = 'invv_patn_patf_code';
update mtat set mtat_title = 'Patient Postcode', mtat_displen = 5  where mtat_name = 'invv_patn_postcode';
update mtat set mtat_title = 'Patient State', mtat_displen = 5  where mtat_name = 'invv_patn_state';
update mtat set mtat_title = 'Patient Suburb', mtat_displen = 5  where mtat_name = 'invv_patn_suburb';
update mtat set mtat_title = 'Provider Address', mtat_displen = 5  where mtat_name = 'invv_prov_address';
update mtat set mtat_title = 'Provider Bank Code', mtat_displen = 5  where mtat_name = 'invv_prov_bank_code';
update mtat set mtat_title = 'Provider Code', mtat_displen = 5  where mtat_name = 'invv_prov_code';
update mtat set mtat_title = 'Provider Name', mtat_displen = 5  where mtat_name = 'invv_prov_name';
update mtat set mtat_title = 'Provider Phone', mtat_displen = 5  where mtat_name = 'invv_prov_phone';
update mtat set mtat_title = 'Provider Postcode', mtat_displen = 5  where mtat_name = 'invv_prov_postcode';
update mtat set mtat_title = 'Provider Provider Num', mtat_displen = 5  where mtat_name = 'invv_prov_provider_num';
update mtat set mtat_title = 'Provider Salutation', mtat_displen = 5  where mtat_name = 'invv_prov_salutation';
update mtat set mtat_title = 'Provider State', mtat_displen = 5  where mtat_name = 'invv_prov_state';
update mtat set mtat_title = 'Provider Suburb', mtat_displen = 5  where mtat_name = 'invv_prov_suburb';
update mtat set mtat_title = 'Referrer Code', mtat_displen = 5  where mtat_name = 'invv_rfdr_code';
update mtat set mtat_title = 'Referrer Date', mtat_displen = 5  where mtat_name = 'invv_rfdr_date';
update mtat set mtat_title = 'Referrer Index', mtat_displen = 5  where mtat_name = 'invv_rfdr_index';
update mtat set mtat_title = 'Referrer Name', mtat_displen = 5  where mtat_name = 'invv_rfdr_name';
update mtat set mtat_title = 'Referrer Period', mtat_displen = 5  where mtat_name = 'invv_rfdr_period';
update mtat set mtat_title = 'Referrer Phone', mtat_displen = 5  where mtat_name = 'invv_rfdr_phone';
update mtat set mtat_title = 'Referrer Postcode', mtat_displen = 5  where mtat_name = 'invv_rfdr_postcode';
update mtat set mtat_title = 'Referrer Provider', mtat_displen = 5  where mtat_name = 'invv_rfdr_provider';
update mtat set mtat_title = 'Referrer Salutation', mtat_displen = 5  where mtat_name = 'invv_rfdr_salutation';
update mtat set mtat_title = 'Referrer State', mtat_displen = 5  where mtat_name = 'invv_rfdr_state';
update mtat set mtat_title = 'Referrer Street', mtat_displen = 5  where mtat_name = 'invv_rfdr_street';
update mtat set mtat_title = 'Referrer Suburb', mtat_displen = 5  where mtat_name = 'invv_rfdr_suburb';

-- update history;
\set mttb_name '\'invv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

