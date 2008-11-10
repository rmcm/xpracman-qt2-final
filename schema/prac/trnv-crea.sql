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



-- Create VIew "trnv" based on "svpf"
--

-- \i schema/DBNAME/trnv-functions.sql   -- Functions need to be defined before the VIEW

create or replace view trnv                         -- All Transactions
  as select
 svpf_patn__sequence                            as trnv_patn__sequence,
 patn_patient_title_name(svpf_patn__sequence)   as trnv_patn_name,
 patn_patient_address(svpf_patn__sequence)      as trnv_patn_address,
 'D'::text                                      as trnv_type,
 date(svpf_date_service)                        as trnv_date,
 svpf_serv_code                                 as trnv_item_code,
 svpf_desc                                      as trnv_desc,
 svpf_amount                                    as trnv_debit_amount,
 svpf_gst_amount                                as trnv_debit_gst_amount,
 0.0::numeric                                   as trnv_credit_amount,
 0.0::numeric                                   as trnv_credit_gst_amount,
 0.0::numeric                                   as trnv_payment_amount,
 ''::text                                       as trnv_drawer,
 
--
-- standard headers
--
 svpf__sequence                                 as trnv__sequence,              -- Row-ID
 svpf__timestamp                                as trnv__timestamp,             -- Timestamp
 svpf__user_entry                               as trnv__user_entry,            -- Entered by
 svpf__status                                   as trnv__status                 -- Record status
 --
 from svpf

 UNION

  select
 invc_patn__sequence                            as trnv_patn__sequence,
 patn_patient_title_name(invc_patn__sequence)   as trnv_patn_name,
 patn_patient_address(invc_patn__sequence)      as trnv_patn_address,
 'C'::text                                      as trnv_type,
 date(cred__timestamp)                          as trnv_date,
 paym_tdtp_code                                 as trnv_item_code,
 cred_notes                                     as trnv_desc,
 0.0::numeric                                   as trnv_debit_amount,
 0.0::numeric                                   as trnv_debit_gst_amount,
 cred_amount                                    as trnv_credit_amount,
 cred_gst_amount                                as trnv_credit_gst_amount,
 paym_amount                                    as trnv_payment_amount,
 paym_drawer                                    as trnv_drawer,
 
--
-- standard headers
--
 cred__sequence + 1000000                       as trnv__sequence,              -- Row-ID
 cred__timestamp                                as trnv__timestamp,             -- Timestamp
 cred__user_entry                               as trnv__user_entry,            -- Entered by
 cred__status                                   as trnv__status                 -- Record status
 --
 from (cred JOIN paym ON (cred.cred_paym__sequence = paym.paym__sequence))
        JOIN invc ON (cred.cred_invc__sequence = invc.invc__sequence)

;

-- ------------------------------------------------------------
-- PERMISSIONS
-- ------------------------------------------------------------
grant select,insert,update on trnv to public;

-- ------------------------------------------------------------
-- FUNCTIONS
-- ------------------------------------------------------------
-- 
-- Create a function to insert via view
-- This function returns the serial_number of the new "virtual" row.
-- This is used instead of an "insert" rule, as the latter cannot use "lastoid"
-- to revover the inserted row.
-- 

-- ------------------------------------------------------------
-- Insert a row with supplied values
-- ------------------------------------------------------------
create or replace function trnv_insert_row(a_trnv trnv)
returns integer as $$
    DECLARE
      rec_trnv trnv%ROWTYPE;
      x_update text := '';
      sep char(1) := '';

    BEGIN
      -- add the trnv record 
      select trnv_insert_row()
        into rec_trnv.trnv__sequence;

      -- change to supplied values
      -- (only svpf vars)

      if (a_trnv.trnv_XX is not null) then
        x_update := sep || 'svpf_XX = \'' || a_trnv.trnv_XX || '\'::timestamp';
        sep = ',';
      end if;
      
      if (x_update != '') then    
        x_update = 'update svpf set ' || x_update || ' where svpf__sequence = ' || rec_trnv.trnv__sequence;
        EXECUTE x_update;
      end if;

      return rec_trnv.trnv__sequence;
    END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Insert a row and return the PK sequence id (used by Marvin)
-- ------------------------------------------------------------
create or replace function trnv_insert_row()
returns integer as $$
    DECLARE
      rec_svpf svpf%ROWTYPE;

    BEGIN
      
      -- add the svpf record 
      select svpf_insert_row()
        into rec_svpf.svpf__sequence;

      return rec_svpf.svpf__sequence;
    END;
$$
LANGUAGE 'plpgsql';


-- ------------------------------------------------------------
-- Insert a row and return the PK sequence id (used by Marvin)
-- ------------------------------------------------------------
create or replace function trnv_update_row(a_new trnv, a_old trnv)
returns integer as $$
    DECLARE
      rec_svpf svpf%ROWTYPE;

    BEGIN

      -- update the svpf record 

         update svpf
         set    svpf_XX    = a_new.trnv_XX
         where  svpf__sequence  = a_new.trnv__sequence;

      return rec_svpf.svpf__sequence;
    END;
$$
LANGUAGE 'plpgsql';


-- ------------------------------------------------------------
-- RULES - NONE, this is a read-only view
-- ------------------------------------------------------------

 ---- Insert Rule
 ---- The insert is more flexibly handled by a function;
 ----    eg trnv_insert_row() in trnv-functions.sql
 --   create or replace rule rl_trnv_insert as on insert to trnv
 --        do instead
 --        (
 --          select trnv_insert_row(new.*);
 --        );
 --
 ---- Update rule
 -- create or replace rule rl_trnv_update as on update
 --        to trnv
 --        do instead
 --        (
 --          select trnv_update_row(new.*, old.*);
 --        );
 -- 
 -- -- Delete rule
 -- create or replace rule rl_trnv_delete as on delete
 --        to trnv
 --        do instead
 --        (
 --         delete from svpf
 --         where  svpf__sequence  = old.trnv__sequence
 --        );

-- ------------------------------------------------------------
-- METADATA
-- ------------------------------------------------------------
-- add the default metadata
select create_metadata('trnv');
update mtcl set mtcl_title = 'All Transactions', mtcl_userlabel = 'trnv_desc', mtcl_order_by = 'order by trnv__timestamp desc' where mtcl_name = 'trnv';
--
-- /home/source/bin/makeDBSubstitute.sh module database table
-- update mtat set mtat_title = '_@ATT_TITLE@_', mtat_displen = _@ATT_DISPLEN@_, mtat_input_method = '_@ATT_INPUT_METHOD@_', mtat_view_order = _@ATT_VIEW_ORDER@_ where mtat_name = '_@ATT_NAME@_';
update mtat set mtat_title = 'Type', mtat_displen = 3, mtat_input_method = 'RO', mtat_view_order = 10 where mtat_name = 'trnv_type';
update mtat set mtat_title = 'Date', mtat_type = 'text', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 20 where mtat_name = 'trnv_date';
update mtat set mtat_title = 'Item Code', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 30 where mtat_name = 'trnv_item_code';
update mtat set mtat_title = 'Description', mtat_displen = 20, mtat_input_method = 'RO', mtat_view_order = 40 where mtat_name = 'trnv_desc';
update mtat set mtat_title = 'Debit', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 50 where mtat_name = 'trnv_debit_amount';
update mtat set mtat_title = 'Debit GST', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 60 where mtat_name = 'trnv_debit_gst_amount';
update mtat set mtat_title = 'Credit', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 70 where mtat_name = 'trnv_credit_amount';
update mtat set mtat_title = 'Credit GST', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 80 where mtat_name = 'trnv_credit_gst_amount';
update mtat set mtat_title = 'Payment', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 90 where mtat_name = 'trnv_payment_amount';
update mtat set mtat_title = 'Drawer', mtat_displen = 30, mtat_input_method = 'RO', mtat_view_order = 100 where mtat_name = 'trnv_drawer';
update mtat set mtat_title = 'Patient ID', mtat_displen = 10, mtat_input_method = 'RO', mtat_view_order = 102 where mtat_name = 'trnv_patn__sequence';
update mtat set mtat_title = 'Patient Name', mtat_displen = 20, mtat_input_method = 'SYSTEM', mtat_view_order = 104 where mtat_name = 'trnv_patn_name';
update mtat set mtat_title = 'Patient Address', mtat_displen = 20, mtat_input_method = 'SYSTEM', mtat_view_order = 106 where mtat_name = 'trnv_patn_address';
update mtat set mtat_title = 'Sequence', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 110 where mtat_name = 'trnv__sequence';
update mtat set mtat_title = 'Timestamp', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 120 where mtat_name = 'trnv__timestamp';
update mtat set mtat_title = 'User Entry', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 130 where mtat_name = 'trnv__user_entry';
update mtat set mtat_title = 'Status', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 140 where mtat_name = 'trnv__status';

-- add foreign
delete from mtfn where mtfn_master_class = 'trnv';
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Patient','trnv','trnv_patn__sequence','patn','patn__sequence','patn__sequence' );

-- make the child of patn
delete from mtrl where mtrl_other_class = 'trnv';
insert into mtrl(mtrl_title, mtrl_master_class, mtrl_key, mtrl_other_class, mtrl_join) values('Ledger', 'patn', 'patn__sequence', 'trnv', 'trnv_patn__sequence');

-- update history;
\set mttb_name '\'trnv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

