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



-- Create MetaData for Object "hpcv"
--

-- modify the default metadata

update mtcl set mtcl_title = 'HIC Claims', mtcl_order_by = 'order by hpcv__sequence' where mtcl_name = 'hpcv';

-- pass the following to makeDBSubstitute.sh MODULE DATABASE OBJECT
-- (MODULE is not used here - pass a dummy argument)
-- update mtat set mtat_title = '_@ATT_TITLE@_', mtat_displen = _@ATT_DISPLEN@_, mtat_view_order = _@ATT_VIEW_ORDER@_ where mtat_name = '_@ATT_NAME@_';

update mtat set mtat_title = 'Sequence', mtat_displen = 30, mtat_view_order = 10 where mtat_name = 'hpcv__sequence';
update mtat set mtat_title = 'Aftercareoverrideind', mtat_displen = 30, mtat_view_order = 10 where mtat_name = 'hpcv_aftercareoverrideind';
update mtat set mtat_title = 'Dateofservice', mtat_displen = 30, mtat_view_order = 10 where mtat_name = 'hpcv_dateofservice';
update mtat set mtat_title = 'Accountpaidind', mtat_displen = 30, mtat_view_order = 10 where mtat_name = 'hpcv_accountpaidind';
update mtat set mtat_title = 'Timestamp', mtat_displen = 30, mtat_view_order = 20 where mtat_name = 'hpcv__timestamp';
update mtat set mtat_title = 'Chargeamount', mtat_displen = 30, mtat_view_order = 20 where mtat_name = 'hpcv_chargeamount';
update mtat set mtat_title = 'Timeofservice', mtat_displen = 30, mtat_view_order = 20 where mtat_name = 'hpcv_timeofservice';
update mtat set mtat_title = 'Claimsubmissionauthorised', mtat_displen = 30, mtat_view_order = 20 where mtat_name = 'hpcv_claimsubmissionauthorised';
update mtat set mtat_title = 'User Entry', mtat_displen = 30, mtat_view_order = 30 where mtat_name = 'hpcv__user_entry';
update mtat set mtat_title = 'Duplicateserviceoverrideind', mtat_displen = 30, mtat_view_order = 30 where mtat_name = 'hpcv_duplicateserviceoverrideind';
update mtat set mtat_title = 'Referraloverridetypecde', mtat_displen = 30, mtat_view_order = 30 where mtat_name = 'hpcv_referraloverridetypecde';
update mtat set mtat_title = 'Dateoflodgement', mtat_displen = 30, mtat_view_order = 30 where mtat_name = 'hpcv_dateoflodgement';
update mtat set mtat_title = 'Status', mtat_displen = 1, mtat_view_order = 40 where mtat_name = 'hpcv__status';
update mtat set mtat_title = 'Equipmentid', mtat_displen = 30, mtat_view_order = 40 where mtat_name = 'hpcv_equipmentid';
update mtat set mtat_title = 'Referringprovidernum', mtat_displen = 30, mtat_view_order = 40 where mtat_name = 'hpcv_referringprovidernum';
update mtat set mtat_title = 'Patientdateofbirth', mtat_displen = 30, mtat_view_order = 40 where mtat_name = 'hpcv_patientdateofbirth';
update mtat set mtat_title = 'Fieldquantity', mtat_displen = 30, mtat_view_order = 50 where mtat_name = 'hpcv_fieldquantity';
update mtat set mtat_title = 'Requestingprovidernum', mtat_displen = 30, mtat_view_order = 50 where mtat_name = 'hpcv_requestingprovidernum';
update mtat set mtat_title = 'Transactionid', mtat_displen = 30, mtat_view_order = 50 where mtat_name = 'hpcv_transactionid';
update mtat set mtat_title = 'Patientfamilyname', mtat_displen = 30, mtat_view_order = 50 where mtat_name = 'hpcv_patientfamilyname';
update mtat set mtat_title = 'Hospitalind', mtat_displen = 30, mtat_view_order = 60 where mtat_name = 'hpcv_hospitalind';
update mtat set mtat_title = 'Referralissuedate', mtat_displen = 30, mtat_view_order = 60 where mtat_name = 'hpcv_referralissuedate';
update mtat set mtat_title = 'Patientfirstname', mtat_displen = 30, mtat_view_order = 60 where mtat_name = 'hpcv_patientfirstname';
update mtat set mtat_title = 'Invc  Sequence', mtat_displen = 30, mtat_view_order = 70 where mtat_name = 'hpcv_invc__sequence';
update mtat set mtat_title = 'Itemnum', mtat_displen = 30, mtat_view_order = 70 where mtat_name = 'hpcv_itemnum';
update mtat set mtat_title = 'Requestissuedate', mtat_displen = 30, mtat_view_order = 70 where mtat_name = 'hpcv_requestissuedate';
update mtat set mtat_title = 'Patientmedicarecardnum', mtat_displen = 30, mtat_view_order = 70 where mtat_name = 'hpcv_patientmedicarecardnum';
update mtat set mtat_title = 'Hpcl  Sequence', mtat_displen = 30, mtat_view_order = 80 where mtat_name = 'hpcv_hpcl__sequence';
update mtat set mtat_title = 'Lccid', mtat_displen = 30, mtat_view_order = 80 where mtat_name = 'hpcv_lccid';
update mtat set mtat_title = 'Referralperiodtypecde', mtat_displen = 30, mtat_view_order = 80 where mtat_name = 'hpcv_referralperiodtypecde';
update mtat set mtat_title = 'Claimantaddressline1', mtat_displen = 30, mtat_view_order = 80 where mtat_name = 'hpcv_claimantaddressline1';
update mtat set mtat_title = 'Patientreferencenum', mtat_displen = 30, mtat_view_order = 80 where mtat_name = 'hpcv_patientreferencenum';
update mtat set mtat_title = 'Lspnum', mtat_displen = 30, mtat_view_order = 90 where mtat_name = 'hpcv_lspnum';
update mtat set mtat_title = 'Requesttypecde', mtat_displen = 30, mtat_view_order = 90 where mtat_name = 'hpcv_requesttypecde';
update mtat set mtat_title = 'Claimantaddressline2', mtat_displen = 30, mtat_view_order = 90 where mtat_name = 'hpcv_claimantaddressline2';
update mtat set mtat_title = 'Servicingprovidernum', mtat_displen = 30, mtat_view_order = 90 where mtat_name = 'hpcv_servicingprovidernum';
update mtat set mtat_title = 'Multipleprocedureoverrideind', mtat_displen = 30, mtat_view_order = 100 where mtat_name = 'hpcv_multipleprocedureoverrideind';
update mtat set mtat_title = 'Claimantaddresslocality', mtat_displen = 30, mtat_view_order = 100 where mtat_name = 'hpcv_claimantaddresslocality';
update mtat set mtat_title = 'Timeoflodgement', mtat_displen = 30, mtat_view_order = 100 where mtat_name = 'hpcv_timeoflodgement';
update mtat set mtat_title = 'Servicing Provider Name', mtat_displen = 30, mtat_view_order = 100 where mtat_name = 'hpcv_servicing_prov_name';
update mtat set mtat_title = 'Noofpatientsseen', mtat_displen = 30, mtat_view_order = 110 where mtat_name = 'hpcv_noofpatientsseen';
update mtat set mtat_title = 'Claimantaddresspostcode', mtat_displen = 30, mtat_view_order = 110 where mtat_name = 'hpcv_claimantaddresspostcode';
update mtat set mtat_title = 'Accountreferenceid', mtat_displen = 30, mtat_view_order = 110 where mtat_name = 'hpcv_accountreferenceid';
update mtat set mtat_title = 'Patientcontribamt', mtat_displen = 30, mtat_view_order = 120 where mtat_name = 'hpcv_patientcontribamt';
update mtat set mtat_title = 'Claimantdateofbirth', mtat_displen = 30, mtat_view_order = 120 where mtat_name = 'hpcv_claimantdateofbirth';
update mtat set mtat_title = 'Bankaccountname', mtat_displen = 30, mtat_view_order = 120 where mtat_name = 'hpcv_bankaccountname';
update mtat set mtat_title = 'Rule3exemptind', mtat_displen = 30, mtat_view_order = 130 where mtat_name = 'hpcv_rule3exemptind';
update mtat set mtat_title = 'Claimantfamilyname', mtat_displen = 30, mtat_view_order = 130 where mtat_name = 'hpcv_claimantfamilyname';
update mtat set mtat_title = 'Bankaccountnum', mtat_displen = 30, mtat_view_order = 130 where mtat_name = 'hpcv_bankaccountnum';
update mtat set mtat_title = 'S4b3exemptind', mtat_displen = 30, mtat_view_order = 140 where mtat_name = 'hpcv_s4b3exemptind';
update mtat set mtat_title = 'Claimantfirstname', mtat_displen = 30, mtat_view_order = 140 where mtat_name = 'hpcv_claimantfirstname';
update mtat set mtat_title = 'Bsbcode', mtat_displen = 30, mtat_view_order = 140 where mtat_name = 'hpcv_bsbcode';
update mtat set mtat_title = 'Selfdeemedcde', mtat_displen = 30, mtat_view_order = 150 where mtat_name = 'hpcv_selfdeemedcde';
update mtat set mtat_title = 'Claimantmedicarecardnum', mtat_displen = 30, mtat_view_order = 150 where mtat_name = 'hpcv_claimantmedicarecardnum';
update mtat set mtat_title = 'Servicetext', mtat_displen = 30, mtat_view_order = 160 where mtat_name = 'hpcv_servicetext';
update mtat set mtat_title = 'Claimantreferencenum', mtat_displen = 30, mtat_view_order = 160 where mtat_name = 'hpcv_claimantreferencenum';
update mtat set mtat_title = 'Timeduration', mtat_displen = 30, mtat_view_order = 170 where mtat_name = 'hpcv_timeduration';
update mtat set mtat_title = 'Contactphonenum', mtat_displen = 30, mtat_view_order = 170 where mtat_name = 'hpcv_contactphonenum';
update mtat set mtat_title = 'Payeeprovidernum', mtat_displen = 30, mtat_view_order = 180 where mtat_name = 'hpcv_payeeprovidernum';
update mtat set mtat_title = 'Storeandforward', mtat_displen = 30, mtat_view_order = 270 where mtat_name = 'hpcv_storeandforward';


-- update history;
\set mttb_name '\'hpcv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

