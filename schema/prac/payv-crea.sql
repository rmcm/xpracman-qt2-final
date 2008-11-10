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



-- Create VIew "payv" based on "paym"
--

-- \i schema/DBNAME/payv-functions.sql   -- Functions need to be defined before the VIEW

create or replace view payv             -- Payment view for banking batches
  as select

 paym_bkdp__sequence                            as payv_bkdp__sequence, --
 date(paym_date_entry)                          as payv_date_entry,     --
 paym_user_entry                                as payv_user_entry,     --
 paym_site_entry                                as payv_site_entry,     --
 paym_amount                                    as payv_amount,         --
 paym_tdtp_code                                 as payv_tdtp_code,      --
 paym_drawer                                    as payv_drawer,         --
 paym_bank                                      as payv_bank,           --
 paym_branch                                    as payv_branch,         --

--
-- standard headers
--
 paym__sequence                         as payv__sequence,              -- Row-ID
 paym__timestamp                        as payv__timestamp,             -- Timestamp
 paym__user_entry                       as payv__user_entry,            -- Entered by
 paym__status                           as payv__status                 -- Record status
 --
 from paym
;

grant select,insert,update on payv to public;

-- Rules

-- Insert Rule
-- The insert is more flexibly handled by a function;
--    eg payv_insert_row() in payv-functions.sql

-- Update rule
drop rule rl_payv_update;
create rule rl_payv_update as on update
       to payv
       do instead
       (
        update paym
        set    paym_bkdp__sequence  = new.payv_bkdp__sequence
        where  paym__sequence  = new.payv__sequence
       );

-- add the default metadata
select create_metadata('payv');
update mtcl set mtcl_title = 'Payments', mtcl_order_by = 'order by payv__timestamp' where mtcl_name = 'payv';
-- update mtat set mtat_title = '_@ATT_TITLE@_', mtat_displen = _@ATT_DISPLEN@_, mtat_input_method = '_@ATT_INPUT_METHOD@_', mtat_view_order = _@ATT_VIEW_ORDER@_ where mtat_name = '_@ATT_NAME@_';
update mtat set mtat_title = 'Batch', mtat_displen = 10, mtat_input_method = '10', mtat_view_order = 5 where mtat_name = 'payv_bkdp__sequence';
update mtat set mtat_title = 'Received', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 10 where mtat_name = 'payv_date_entry';
update mtat set mtat_title = 'User', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 20 where mtat_name = 'payv_user_entry';
update mtat set mtat_title = 'Amount', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 40 where mtat_name = 'payv_amount';
update mtat set mtat_title = 'Tender', mtat_displen = 5, mtat_input_method = 'RO', mtat_view_order = 50 where mtat_name = 'payv_tdtp_code';
update mtat set mtat_title = 'Drawer', mtat_displen = 30, mtat_input_method = 'RO', mtat_view_order = 60 where mtat_name = 'payv_drawer';
update mtat set mtat_title = 'Bank', mtat_displen = 5, mtat_input_method = 'RO', mtat_view_order = 70 where mtat_name = 'payv_bank';
update mtat set mtat_title = 'Branch', mtat_displen = 30, mtat_input_method = 'RO', mtat_view_order = 80 where mtat_name = 'payv_branch';
update mtat set mtat_title = 'Site', mtat_displen = 5, mtat_input_method = 'RO', mtat_view_order = 95 where mtat_name = 'payv_site_entry';
update mtat set mtat_title = 'Entered by', mtat_displen = 10, mtat_input_method = 'SYSTEM', mtat_view_order = 100 where mtat_name = 'payv__user_entry';
update mtat set mtat_title = 'Payment ID', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 200 where mtat_name = 'payv__sequence';
update mtat set mtat_title = 'Timestamp', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 210 where mtat_name = 'payv__timestamp';
update mtat set mtat_title = 'Record status', mtat_displen = 1, mtat_input_method = 'SYSTEM', mtat_view_order = 230 where mtat_name = 'payv__status';


-- add foreign
delete from mtfn where mtfn_master_class = 'payv';
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Payment Batch','payv','payv_bkdp__sequence','bkdp','bkdp__sequence','bkdp__sequence' );

-- make the child of bkdp
update mtrl set mtrl_other_class = 'payv', mtrl_join = 'payv_bkdp__sequence' where mtrl_master_class = 'bkdp' and mtrl_other_class = 'paym';

-- add child - invoices
insert into mtrl(mtrl_master_class, mtrl_key, mtrl_other_class, mtrl_join)
        select 'payv', 'payv__sequence', 'crep', 'crep_paym__sequence' from mtrl except select mtrl_master_class, mtrl_key, mtrl_other_class, mtrl_join from mtrl;
update mtrl set mtrl_title = 'Payment Distribution'
        where mtrl_master_class = 'payv' and mtrl_key = 'payv__sequence' and mtrl_other_class = 'crep' and mtrl_join = 'crep_paym__sequence';

-- cleanup bkdp parent
update mtat set mtat_title = 'Batch', mtat_type = 'integer', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 5 where mtat_name = 'bkdp__sequence';
update mtat set mtat_type = 'timestamp', mtat_displen = 20, mtat_input_method = 'RO' where mtat_name = 'bkdp_date_created';

-- update history;
\set mttb_name '\'payv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

