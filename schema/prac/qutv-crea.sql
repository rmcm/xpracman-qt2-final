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

-- ------------------------------------------------------------
-- Create the view
-- ------------------------------------------------------------
create or replace view qutv                -- Services Performed (View)
 as select 

 --
 -- qute
 --

 qute_desc                                       as qutv_desc,               -- Desc
 qute_date_created                               as qutv_date_created,       -- Date Created
 qute_patn__sequence                             as qutv_patn__sequence,     -- Patn  Sequence
 qute_feet_code                                  as qutv_feet_code,          -- Fee level 
 -- aggregates
 coalesce( (select sum(quit_amount)
            from   quit
            where  quit_qute__sequence = qute.qute__sequence), 0.0)::numeric
                                                 as qutv_total_amount,   -- total amount of quote
 --
 -- patn
 --
 coalesce(patn_fsnam, '')
 ||' '
 ||coalesce(patn_psnam, '')                      as qutv_patn_name,
 patn_address                                    as qutv_patn_address,
 coalesce(patn_suburb, '')                       
 ||' '                                           
 ||coalesce(patn_postcode , '')                  as qutv_patn_suburb,
 patn_state                                      as qutv_patn_state,
 patn_postcode                                   as qutv_patn_postcode,
 patn_phone                                      as qutv_patn_phone,
 patn_phone_1                                    as qutv_patn_phone_1,
 patn_phone_2                                    as qutv_patn_phone_2,
 short_date(patn_dob)                            as qutv_patn_dob,
 patn_hlfd_code                                  as qutv_patn_hlfd_code,
 patn_ins_level                                  as qutv_patn_ins_level,
 patn_healthnumb                                 as qutv_patn_healthnumb,
 patn_medicare                                   as qutv_patn_medicare,
 patn_healthcard                                 as qutv_patn_healthcard,
 patn_patf_code                                  as qutv_patn_patf_code,
 patn_claim_number                               as qutv_patn_claim_number,
 short_date(patn_accident_date)                  as qutv_patn_accident_date,
 --                                              
 -- dbtr                                         
 --                                              
 dbtr_address(dbtr_code)                         as qutv_dbtr_full_address,
 dbtr_name                                       as qutv_dbtr_name,
 dbtr_address                                    as qutv_dbtr_address,
 coalesce(dbtr_suburb,'')                        
 ||' '                                           
 ||coalesce(dbtr_postcode,'')                    as qutv_dbtr_suburb,
 dbtr_state                                      as qutv_dbtr_state,

 --                                              
 -- rfdr                                         
 --                                              
 rfdr_code                                       as qutv_rfdr_code,
 rfdr_name                                       as qutv_rfdr_name,
 rfdr_street                                     as qutv_rfdr_street,
 rfdr_suburb                                     as qutv_rfdr_suburb,
 rfdr_postcode                                   as qutv_rfdr_postcode,
 rfdr_state                                      as qutv_rfdr_state,
 (case when rfdr_provider is null or length(rfdr_provider) < 1 then
    coalesce(rfdr_street,'') || ',' 
    || coalesce(rfdr_suburb, '') || ' '
    || coalesce(rfdr_postcode, '') || ' '
    || coalesce(rfdr_state, '')
  else
    rfdr_provider
  end)                                           as qutv_rfdr_provider,
 rfdr_phone                                      as qutv_rfdr_phone,
 rfdr_salutation                                 as qutv_rfdr_salutation,
 rfdr_index                                      as qutv_rfdr_index,
                                                 
 --                                              
 -- feet                                         
 --                                              
 feet_desc                                       as qutv_feet_desc,
 --                                              
 -- prov                                         
 --                                              
 prov_code                                       as qutv_prov_code,
 prov_provider_num                               as qutv_prov_provider_num,
 prov_name                                       as qutv_prov_name,
 prov_address                                    as qutv_prov_address,
 prov_suburb                                     as qutv_prov_suburb,
 prov_state                                      as qutv_prov_state,
 prov_postcode                                   as qutv_prov_postcode,
 prov_salutation                                 as qutv_prov_salutation,
 prov_phone                                      as qutv_prov_phone,
 prov_bank_code                                  as qutv_prov_bank_code,

 --
 -- standard headers
 --
 qute__sequence                                  as qutv__sequence,          -- Sequence
 qute__timestamp                                 as qutv__timestamp,         -- Timestamp
 qute__user_entry                                as qutv__user_entry,        -- User Entry
 qute__status                                    as qutv__status            -- Status
 --

 from (((((qute LEFT JOIN patn ON (qute.qute_patn__sequence = patn.patn__sequence))
                  LEFT JOIN feet ON (qute.qute_feet_code = feet.feet_code))
                  LEFT JOIN dbtr ON (patn.patn_dbtr_code = dbtr.dbtr_code))
                  LEFT JOIN rfdr ON (patn.patn_rfdr_code = rfdr.rfdr_code))
                  LEFT JOIN prov ON (patn.patn_prov_code = prov.prov_code))
                  LEFT JOIN hlfd ON (patn.patn_hlfd_code = hlfd.hlfd_code)

;

-- ------------------------------------------------------------
-- FUNCTIONS
-- ------------------------------------------------------------
-- ------------------------------------------------------------
-- Insert a row with supplied values
-- ------------------------------------------------------------
create or replace function qutv_insert_row(a_qutv qutv)
returns integer as $$
    DECLARE
      rec_qutv qutv%ROWTYPE;
      x_update text := '';
      sep char(1) := '';

    BEGIN
      -- add the qutv record 
      select qutv_insert_row()
        into rec_qutv.qutv__sequence;

      -- change to supplied values
      -- (only QUTE vars)

      if (a_qutv.qutv_desc is not null) then
        x_update := sep || 'qute_desc = \'' || a_qutv.qutv_desc || '\'::text';
        sep = ',';
      end if;
      
      if (a_qutv.qutv_date_created is not null) then
        x_update := sep || 'qute_date_created = \'' || a_qutv.qutv_date_created || '\'::timestamp';
        sep = ',';
      end if;
      
      if (a_qutv.qutv_patn__sequence is not null) then
        x_update := sep || 'qute_patn__sequence = \'' || a_qutv.qutv_patn__sequence || '\'::integer';
        sep = ',';
      end if;
      
      if (a_qutv.qutv_feet_code is not null) then
        x_update := sep || 'qute_feet_code = \'' || a_qutv.qutv_feet_code || '\'::integer';
        sep = ',';
      end if;
      

      if (x_update != '') then    
        x_update = 'update qute set ' || x_update || ' where qute__sequence = ' || rec_qutv.qutv__sequence;
        EXECUTE x_update;
      end if;

      return rec_qutv.qutv__sequence;
    END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Insert a row and return the PK sequence id (used by Marvin)
-- ------------------------------------------------------------
create or replace function qutv_insert_row()
returns integer as $$
    DECLARE
      rec_qute qute%ROWTYPE;

    BEGIN
      
      -- add the qute record 
      select qute_insert_row()
        into rec_qute.qute__sequence;

      return rec_qute.qute__sequence;
    END;
$$
LANGUAGE 'plpgsql';


-- ------------------------------------------------------------
-- Insert a row and return the PK sequence id (used by Marvin)
-- ------------------------------------------------------------
create or replace function qutv_update_row(a_new qutv, a_old qutv)
returns integer as $$
    DECLARE
      rec_qute qute%ROWTYPE;

    BEGIN
      -- update the qute record 

         update qute
         set    qute_desc            = a_new.qutv_desc,
                qute_date_created    = a_new.qutv_date_created,
                qute_patn__sequence  = a_new.qutv_patn__sequence,
                qute_feet_code       = a_new.qutv_feet_code
         where  qute__sequence  = a_new.qutv__sequence;

      return rec_qute.qute__sequence;
    END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- PERMISSIONS
-- ------------------------------------------------------------
grant select,insert,update on qutv to public;

-- ------------------------------------------------------------
-- RULES
-- ------------------------------------------------------------

-- ------------------------------------------------------------
-- Insert Rule
-- ------------------------------------------------------------
-- The insert is more flexibly handled by a function;
--    eg qutv_insert_row() in qutv-functions.sql
 create or replace rule rl_qutv_insert as on insert to qutv
   do instead
   (
     select qutv_insert_row(new.*);
   );

-- ------------------------------------------------------------
-- Update rule
-- ------------------------------------------------------------
 create or replace rule rl_qutv_update as on update to qutv
   do instead
   (
     select qutv_update_row(new.*, old.*);
   );

-- ------------------------------------------------------------
-- Delete rule
-- ------------------------------------------------------------
 create or replace rule rl_qutv_delete as on delete
        to qutv
        do instead
        (
         delete from qute
         where  qute__sequence  = old.qutv__sequence
        );

-- ------------------------------------------------------------
-- METADATA
-- ------------------------------------------------------------
-- update metadata
select create_metadata('qutv');
update mtcl set mtcl_title = 'Quotations', mtcl_userlabel = '<b>Quotation #%1 (%2)</b> $ %3<br><i>%4 (%5)</i><br><i>%6</i><br><i>%7</i><br>Ph: %8(H) %9(W) %+1(M)<br>, qutv__sequence, qutv_date_created, qutv_total_amount, qutv_patn_name, ,qutv_patn_dob, qutv_patn_address, qutv_patn_suburb, qutv_patn_phone, qutv_patn_phone_1, qutv_patn_phone_2', mtcl_order_by = 'order by qutv__sequence' where mtcl_name = 'qutv';
--
-- ~/bin/makeDBSubstitute.sh module database table
-- update mtat set mtat_title = '_@ATT_TITLE@_', mtat_displen = _@ATT_DISPLEN@_, mtat_view_order = _@ATT_VIEW_ORDER@_, mtat_input_method = '_@ATT_INPUT_METHOD@_'  where mtat_name = '_@ATT_NAME@_';
update mtat set mtat_input_method = 'SYSTEM' where mtat_class_name = 'qutv';

update mtat set mtat_title = 'Quotation ID', mtat_displen = 10, mtat_view_order = 10, mtat_input_method = '10'  where mtat_name = 'qutv__sequence';
update mtat set mtat_title = 'Description', mtat_displen = 20, mtat_view_order = 20, mtat_input_method = '10'  where mtat_name = 'qutv_desc';
update mtat set mtat_title = 'Date Created', mtat_displen = 10, mtat_view_order = 30, mtat_input_method = '10'  where mtat_name = 'qutv_date_created';
update mtat set mtat_title = 'Fee Level', mtat_displen = 10, mtat_view_order = 40, mtat_input_method = '10'  where mtat_name = 'qutv_feet_code';
update mtat set mtat_title = 'Total Amount', mtat_displen = 10, mtat_view_order = 60, mtat_input_method = '10'  where mtat_name = 'qutv_total_amount';
update mtat set mtat_title = 'Patient ID', mtat_displen = 30, mtat_view_order = 230, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn__sequence';
update mtat set mtat_title = 'Patient Name', mtat_displen = 30, mtat_view_order = 240, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_name';
update mtat set mtat_title = 'Patient Address', mtat_displen = 30, mtat_view_order = 250, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_address';
update mtat set mtat_title = 'Patient Suburb', mtat_displen = 30, mtat_view_order = 260, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_suburb';
update mtat set mtat_title = 'Patient State', mtat_displen = 30, mtat_view_order = 270, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_state';
update mtat set mtat_title = 'Patient Postcode', mtat_displen = 30, mtat_view_order = 280, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_postcode';
update mtat set mtat_title = 'Patient Phone(H)', mtat_displen = 30, mtat_view_order = 282, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_phone';
update mtat set mtat_title = 'Patient Phone(W)', mtat_displen = 30, mtat_view_order = 284, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_phone_1';
update mtat set mtat_title = 'Patient Phone(M)', mtat_displen = 30, mtat_view_order = 286, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_phone_2';
update mtat set mtat_title = 'Patient DOB', mtat_displen = 30, mtat_view_order = 290, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_dob';
update mtat set mtat_title = 'Patient Hlfd Code', mtat_displen = 30, mtat_view_order = 300, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_hlfd_code';
update mtat set mtat_title = 'Patient Ins Level', mtat_displen = 1, mtat_view_order = 310, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_ins_level';
update mtat set mtat_title = 'Patient Healthnumb', mtat_displen = 30, mtat_view_order = 320, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_healthnumb';
update mtat set mtat_title = 'Patient Medicare', mtat_displen = 30, mtat_view_order = 330, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_medicare';
update mtat set mtat_title = 'Patient Healthcard', mtat_displen = 30, mtat_view_order = 340, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_healthcard';
update mtat set mtat_title = 'Patient Patf Code', mtat_displen = 30, mtat_view_order = 350, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_patf_code';
update mtat set mtat_title = 'Patient Claim Number', mtat_displen = 30, mtat_view_order = 360, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_claim_number';
update mtat set mtat_title = 'Patient Accident Date', mtat_displen = 30, mtat_view_order = 370, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_patn_accident_date';
update mtat set mtat_title = 'Debtor Full Address', mtat_displen = 30, mtat_view_order = 380, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_dbtr_full_address';
update mtat set mtat_title = 'Debtor Name', mtat_displen = 30, mtat_view_order = 390, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_dbtr_name';
update mtat set mtat_title = 'Debtor Address', mtat_displen = 30, mtat_view_order = 400, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_dbtr_address';
update mtat set mtat_title = 'Debtor Suburb', mtat_displen = 30, mtat_view_order = 410, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_dbtr_suburb';
update mtat set mtat_title = 'Debtor State', mtat_displen = 30, mtat_view_order = 420, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_dbtr_state';
update mtat set mtat_title = 'Referrer Code', mtat_displen = 30, mtat_view_order = 430, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_rfdr_code';
update mtat set mtat_title = 'Referrer Name', mtat_displen = 30, mtat_view_order = 440, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_rfdr_name';
update mtat set mtat_title = 'Referrer Street', mtat_displen = 30, mtat_view_order = 450, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_rfdr_street';
update mtat set mtat_title = 'Referrer Suburb', mtat_displen = 30, mtat_view_order = 460, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_rfdr_suburb';
update mtat set mtat_title = 'Referrer Postcode', mtat_displen = 30, mtat_view_order = 470, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_rfdr_postcode';
update mtat set mtat_title = 'Referrer State', mtat_displen = 30, mtat_view_order = 480, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_rfdr_state';
update mtat set mtat_title = 'Referrer Provider', mtat_displen = 30, mtat_view_order = 490, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_rfdr_provider';
update mtat set mtat_title = 'Referrer Phone', mtat_displen = 30, mtat_view_order = 500, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_rfdr_phone';
update mtat set mtat_title = 'Referrer Salutation', mtat_displen = 30, mtat_view_order = 510, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_rfdr_salutation';
update mtat set mtat_title = 'Referrer Index', mtat_displen = 30, mtat_view_order = 520, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_rfdr_index';
update mtat set mtat_title = 'Feet Desc', mtat_displen = 30, mtat_view_order = 600, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_feet_desc';
update mtat set mtat_title = 'Provider Code', mtat_displen = 30, mtat_view_order = 610, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_prov_code';
update mtat set mtat_title = 'Provider Provider Num', mtat_displen = 30, mtat_view_order = 620, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_prov_provider_num';
update mtat set mtat_title = 'Provider Name', mtat_displen = 30, mtat_view_order = 630, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_prov_name';
update mtat set mtat_title = 'Provider Address', mtat_displen = 30, mtat_view_order = 640, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_prov_address';
update mtat set mtat_title = 'Provider Suburb', mtat_displen = 30, mtat_view_order = 650, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_prov_suburb';
update mtat set mtat_title = 'Provider State', mtat_displen = 30, mtat_view_order = 660, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_prov_state';
update mtat set mtat_title = 'Provider Postcode', mtat_displen = 30, mtat_view_order = 670, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_prov_postcode';
update mtat set mtat_title = 'Provider Salutation', mtat_displen = 30, mtat_view_order = 680, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_prov_salutation';
update mtat set mtat_title = 'Provider Phone', mtat_displen = 30, mtat_view_order = 690, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_prov_phone';
update mtat set mtat_title = 'Provider Bank Code', mtat_displen = 30, mtat_view_order = 700, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv_prov_bank_code';
update mtat set mtat_title = 'Timestamp', mtat_displen = 30, mtat_view_order = 720, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv__timestamp';
update mtat set mtat_title = 'User Entry', mtat_displen = 30, mtat_view_order = 730, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv__user_entry';
update mtat set mtat_title = 'Status', mtat_displen = 1, mtat_view_order = 740, mtat_input_method = 'SYSTEM'  where mtat_name = 'qutv__status';



-- add foreigns
delete from mtfn where mtfn_master_class = 'qutv';
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Patient','qutv','qutv_patn__sequence','patn','patn__sequence','patn__sequence' );
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Fee Level','qutv','qutv_feet_code','feet','feet_code','feet_code' );

-- make the child of parents
delete from mtrl where mtrl_other_class = 'qutv';
insert into mtrl(mtrl_title, mtrl_master_class, mtrl_key, mtrl_other_class, mtrl_join) values('Quotes', 'patn', 'patn__sequence', 'qutv', 'qutv_patn__sequence');
insert into mtrl(mtrl_title, mtrl_master_class, mtrl_key, mtrl_other_class, mtrl_join) values('Quotes', 'feet', 'feet_code', 'qutv', 'qutv_feet_code');



-- update history;
\set mttb_name '\'qutv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;


