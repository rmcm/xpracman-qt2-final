--  -*- sql -*-
--  
--    $Id$

--  Copyright 1996 X=X Computer Software Trust
--   		  Kangaroo Ground Australia 3097


-- This is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public Licensepublished by
-- the Free Software Foundation; either version 2, or (at your option)
-- any later version.
 
-- This software is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
 
-- You should have received a copy of the GNU General Public License
-- along with this software; see the file COPYING.  If not, write to
-- the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
-- Report problems and direct all questions to:
 
--     Rex McMaster, rmcm@compsoft.com.au
 

 
-- Revision Log
  
create table mdbt                       -- Medicare Batches
	(
  mdbt_prov_code text,                                  -- Service provider code
  mdbt_first_voucher text,                              -- First form number in batch
  mdbt_last_voucher text,                               -- Last form number in batch
  mdbt_batch_code text,                                 -- Batch code - external
  mdbt_location_id text,                                -- Location identifier
  mdbt_product text,                                    -- PMS Product name
  mdbt_version text,                                    -- PMS Version
  mdbt_accpaid_ind text,                                -- Account paid indicator
  mdbt_claimsubauth text,                               -- Claim Submission Authorised
  mdbt_accunpaid text,                                  -- Account Unpaid.
  mdbt_voucher_count integer,                           -- Form count in batch
  mdbt_total_amount numeric(12,2),                      -- Total amount claimed
  mdbt_claim_date timestamp,                            -- Date claim transmitted
  mdbt_recipient text,                                  -- HIC recipient address
  mdbt_server text,                                     -- HIC claims server
  mdbt_claim_status text,                               -- Status of claim.
  mdbt_payee_prov_code text,                            -- Payee provider code (if different to service provider)
  mdbt_transactionid text,                              -- HIC transaction id
  --
  -- standard headers
  --
  mdbt__sequence serial,                                -- Row-ID
  mdbt__timestamp timestamp default now(),              -- Timestamp
  mdbt__user_entry text default CURRENT_USER,           -- Entered by
  mdbt__status char default text 'N',                   -- Record status
  --
  -- constraints
  --
  primary key(mdbt__sequence)
  );


grant select,insert,update on mdbt to public;
grant select,insert,update on mdbt_mdbt__sequence_seq to public;

select create_metadata('mdbt');
update mtcl set mtcl_title = 'HIC Claims - Log' where mtcl_name = 'mdbt';

update mtat set mtat_title = 'Provider', mtat_displen = 5  where mtat_name = 'mdbt_prov_code';
update mtat set mtat_title = 'First Voucher', mtat_displen = 5 where mtat_name = 'mdbt_first_voucher';
update mtat set mtat_title = 'Last Voucher', mtat_displen = 5  where mtat_name = 'mdbt_last_voucher';
update mtat set mtat_title = 'External Code', mtat_displen = 10 where mtat_name = 'mdbt_batch_code';
update mtat set mtat_title = 'Voucher Count', mtat_displen = 4 where mtat_name = 'mdbt_voucher_count';
update mtat set mtat_title = 'Total Amount', mtat_displen = 10  where mtat_name = 'mdbt_total_amount';
update mtat set mtat_title = 'Claim Date', mtat_displen = 17  where mtat_name = 'mdbt_claim_date';
update mtat set mtat_title = 'Claim Status', mtat_displen = 5  where mtat_name = 'mdbt_claim_status';
update mtat set mtat_title = 'Payee Provider', mtat_displen = 10 where mtat_name = 'mdbt_payee_prov_code';
update mtat set mtat_title = 'Transaction ID', mtat_displen = 10 where mtat_name = 'mdbt_transactionid';
update mtat set mtat_title = 'Internal ID', mtat_displen = 5  where mtat_name = 'mdbt__sequence';
update mtat set mtat_title = 'Created', mtat_displen = 5  where mtat_name = 'mdbt__timestamp';
update mtat set mtat_title = 'User', mtat_displen = 10 where mtat_name = 'mdbt__user_entry';
update mtat set mtat_title = 'Status', mtat_displen = 5 where mtat_name = 'mdbt__status';

update mtat set mtat_input_method = 'RO' where mtat_name ~ '^mdbt__.*';

delete from mtrl where mtrl_master_class = 'mdbt';
insert into mtrl(mtrl_title, mtrl_master_class, mtrl_key, mtrl_other_class, mtrl_join)
        values('Vouchers', 'mdbt', 'mdbt__sequence',
                 'mdaf', 'mdaf_mdbt__sequence');
delete from mtfn where mtfn_master_class = 'mdbt';
insert into mtfn(mtfn_title, mtfn_master_class, mtfn_key, mtfn_other_class, mtfn_join, mtfn_view)
        values('Provider', 'mdbt', 'mdbt_prov_code',
                 'prov', 'prov_code', 'prov_name');
insert into mtfn(mtfn_title, mtfn_master_class, mtfn_key, mtfn_other_class, mtfn_join, mtfn_view)
        values('Payee', 'mdbt', 'mdbt_payee_prov_code',
                 'prov', 'prov_code', 'prov_name');

-- update history;
\set mttb_name '\'mdbt\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
