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



-- Create VIew "rcpv" based on "svpf"
--

-- \i schema/DBNAME/rcpv-functions.sql   -- Functions need to be defined before the VIEW

create or replace view rcpv                         -- All Transactions
  as select
 invc_patn__sequence                            as rcpv_patn__sequence,
 invc__sequence                                 as rcpv_invc__sequence,
 patn_patient_title_name(patn__sequence)        as rcpv_patn_name,
 patn_address                                   as rcpv_patn_address,
 patn_suburb || ' ' || patn_postcode            as rcpv_patn_suburb,
 patn_total_balance(invc_patn__sequence,0)      as rcpv_patn_balance,
 date(cred__timestamp)                          as rcpv_date,
 paym_tdtp_code                                 as rcpv_tender,
 cred_notes                                     as rcpv_desc,
 cred_amount                                    as rcpv_credit_amount,
 cred_gst_amount                                as rcpv_credit_gst_amount,
 paym_amount                                    as rcpv_payment_amount,
 paym_drawer                                    as rcpv_drawer,
 
--
-- standard headers
--
 cred__sequence + 1000000                       as rcpv__sequence,              -- Row-ID
 cred__timestamp                                as rcpv__timestamp,             -- Timestamp
 cred__user_entry                               as rcpv__user_entry,            -- Entered by
 cred__status                                   as rcpv__status                 -- Record status
 --
 from ((cred JOIN paym ON (cred.cred_paym__sequence = paym.paym__sequence))
        JOIN invc ON (cred.cred_invc__sequence = invc.invc__sequence))
        JOIN patn ON (patn.patn__sequence = invc.invc_patn__sequence)

;

-- ------------------------------------------------------------
-- PERMISSIONS
-- ------------------------------------------------------------
grant select,insert,update on rcpv to public;

-- ------------------------------------------------------------
-- FUNCTIONS
-- ------------------------------------------------------------
-- 

-- ------------------------------------------------------------
-- RULES - NONE, this is a read-only view
-- ------------------------------------------------------------

-- ------------------------------------------------------------
-- METADATA
-- ------------------------------------------------------------
-- add the default metadata
select create_metadata('rcpv');


update mtcl set mtcl_title = 'Patient Receipts', mtcl_order_by = 'order by rcpv__sequence' where mtcl_name = 'rcpv';
--
-- /home/source/bin/makeDBSubstitute.sh module database table
-- update mtat set mtat_title = '_@ATT_TITLE@_', mtat_displen = _@ATT_DISPLEN@_, mtat_input_method = '_@ATT_INPUT_METHOD@_', mtat_view_order = _@ATT_VIEW_ORDER@_ where mtat_name = '_@ATT_NAME@_';
update mtat set mtat_input_method = 'SYSTEM' where mtat_class_name = 'rcpv';

update mtat set mtat_title = 'Name', mtat_displen = 10, mtat_input_method = 'SYSTEM', mtat_view_order = 10 where mtat_name = 'rcpv_patn_name';
update mtat set mtat_title = 'Address', mtat_displen = 15, mtat_input_method = 'SYSTEM', mtat_view_order = 20 where mtat_name = 'rcpv_patn_address';
update mtat set mtat_title = 'Suburb', mtat_displen = 10, mtat_input_method = 'SYSTEM', mtat_view_order = 25 where mtat_name = 'rcpv_patn_suburb';
update mtat set mtat_title = 'Date', mtat_type = 'text', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 30 where mtat_name = 'rcpv_date';
update mtat set mtat_title = 'Invoice', mtat_type = 'text', mtat_displen = 8, mtat_input_method = 'RO', mtat_view_order = 40 where mtat_name = 'rcpv_invc__sequence';
update mtat set mtat_title = 'Credit', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 50 where mtat_name = 'rcpv_credit_amount';
update mtat set mtat_title = 'Type', mtat_displen = 4, mtat_input_method = 'RO', mtat_view_order = 60 where mtat_name = 'rcpv_tender';
update mtat set mtat_title = 'Credit GST', mtat_displen = 8, mtat_input_method = 'RO', mtat_view_order = 60 where mtat_name = 'rcpv_credit_gst_amount';
update mtat set mtat_title = 'Notes', mtat_displen = 15, mtat_input_method = 'RO', mtat_view_order = 70 where mtat_name = 'rcpv_desc';
update mtat set mtat_title = 'Payment', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 70 where mtat_name = 'rcpv_payment_amount';
update mtat set mtat_title = 'Drawer', mtat_displen = 15, mtat_input_method = 'RO', mtat_view_order = 80 where mtat_name = 'rcpv_drawer';
update mtat set mtat_title = 'Patient ID', mtat_displen = 6, mtat_input_method = 'RO', mtat_view_order = 90 where mtat_name = 'rcpv_patn__sequence';
update mtat set mtat_title = 'Sequence', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 100 where mtat_name = 'rcpv__sequence';
update mtat set mtat_title = 'Timestamp', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 110 where mtat_name = 'rcpv__timestamp';
update mtat set mtat_title = 'User Entry', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 120 where mtat_name = 'rcpv__user_entry';
update mtat set mtat_title = 'Status', mtat_displen = 1, mtat_input_method = 'SYSTEM', mtat_view_order = 130 where mtat_name = 'rcpv__status';




-- add foreign
delete from mtfn where mtfn_master_class = 'rcpv';
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Receipts','rcpv','rcpv_patn__sequence','patn','patn__sequence','patn__sequence' );

-- make the child of patn
delete from mtrl where mtrl_other_class = 'rcpv';
insert into mtrl(mtrl_title, mtrl_master_class, mtrl_key, mtrl_other_class, mtrl_join) values('Receipts', 'patn', 'patn__sequence', 'rcpv', 'rcpv_patn__sequence');


-- update history;
\set mttb_name '\'rcpv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

