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

create or replace function quiv_resolved_fee(serv.serv_code%TYPE,
                                             feet.feet_code%TYPE,
                                             quit.quit_percentage%TYPE)
  returns numeric as $$
    select  fn_round_amount((fept_amount::numeric + fept_gst_amount::numeric) * $3/100)
    from    fept
    where   fept_serv_code = $1
    and     fept_feet_code = COALESCE((select conf_value from conf where conf_code = $2),$2);
$$
LANGUAGE SQL;
    

-- ------------------------------------------------------------
-- Create VIew "quiv" based on "quit"
-- ------------------------------------------------------------
--

-- \i schema/DBNAME/quiv-functions.sql   -- Functions need to be defined before the VIEW

create or replace view quiv                         -- Quotation Items
  as select

 --
 -- quit
 -- 
 quit_date_service                               as quiv_date_service,          -- Date Service
 quit_serv_code                                  as quiv_serv_code,             -- Serv Code
 quit_multireduction                             as quiv_multireduction,        -- Multi-reduction (M)
 quit_percentage                                 as quiv_percentage,            -- Percentage
 quit_desc                                       as quiv_desc,                  -- Desc
 quit_amount                                     as quiv_amount,                -- amount
 quiv_resolved_fee(quit_serv_code,
                   qute_feet_code,
                   100)::numeric(12,2)
                                                 as quiv_amount_normal,         -- amount_normal
 quiv_resolved_fee(quit_serv_code,
                   'MBS100',
                   quit_percentage)::numeric(12,2)
                                                 as quiv_amount_mbs100,         -- amount_mbs100
 quiv_resolved_fee(quit_serv_code,
                   'MBS85',
                   quit_percentage)::numeric(12,2)
                                                 as quiv_amount_mbs85,          -- amount_mbs85

 -- aggregates
 coalesce( (select sum(quit_amount)
            from   quit
            where  quit_qute__sequence = qute.qute__sequence), 0.0)::numeric
                                                 as quiv_total_amount,          -- grand total amount of quote
 coalesce( (select sum(quit_amount 
                        - quiv_resolved_fee(quit_serv_code,
                                                'MBS100',
                                                quit_percentage)::numeric(12,2))
            from   quit
            where  quit_qute__sequence = qute.qute__sequence), 0.0)::numeric
                                                 as quiv_total_amount_payable,  -- amount payable after rebate

 --
 -- qute
 --

 qute_desc                                       as quiv_qute_desc,             -- Desc
 date(qute_date_created)                         as quiv_qute_date_created,     -- Date Created
 qute_patn__sequence                             as quiv_patn__sequence,        -- Patn  Sequence
 qute_feet_code                                  as quiv_feet_code,             -- fee level
 qute__sequence                                  as quiv_qute__sequence,        -- Quote  Sequence
 
 --
 -- patn
 --
 coalesce(patn_fsnam, '')
 ||' '
 ||coalesce(patn_psnam, '')                      as quiv_patn_name,
 patn_address                                    as quiv_patn_address,
 coalesce(patn_suburb, '')                       
 ||' '                                           
 ||coalesce(patn_postcode , '')                  as quiv_patn_suburb,
 patn_state                                      as quiv_patn_state,
 patn_postcode                                   as quiv_patn_postcode,
 short_date(patn_dob)                            as quiv_patn_dob,
 patn_hlfd_code                                  as quiv_patn_hlfd_code,
 patn_ins_level                                  as quiv_patn_ins_level,
 patn_healthnumb                                 as quiv_patn_healthnumb,
 patn_medicare                                   as quiv_patn_medicare,
 patn_healthcard                                 as quiv_patn_healthcard,
 patn_patf_code                                  as quiv_patn_patf_code,
 patn_claim_number                               as quiv_patn_claim_number,
 short_date(patn_accident_date)                  as quiv_patn_accident_date,
 --                                              
 -- dbtr                                         
 --                                              
 dbtr_address(dbtr_code)                         as quiv_dbtr_full_address,
 dbtr_name                                       as quiv_dbtr_name,
 dbtr_address                                    as quiv_dbtr_address,
 coalesce(dbtr_suburb,'')                        
 ||' '                                           
 ||coalesce(dbtr_postcode,'')                    as quiv_dbtr_suburb,
 dbtr_state                                      as quiv_dbtr_state,

 --                                              
 -- rfdr                                         
 --                                              
 rfdr_code                                       as quiv_rfdr_code,
 rfdr_name                                       as quiv_rfdr_name,
 rfdr_street                                     as quiv_rfdr_street,
 rfdr_suburb                                     as quiv_rfdr_suburb,
 rfdr_postcode                                   as quiv_rfdr_postcode,
 rfdr_state                                      as quiv_rfdr_state,
 (case when rfdr_provider is null or length(rfdr_provider) < 1 then
    coalesce(rfdr_street,'') || ',' 
    || coalesce(rfdr_suburb, '') || ' '
    || coalesce(rfdr_postcode, '') || ' '
    || coalesce(rfdr_state, '')
  else
    rfdr_provider
  end)                                           as quiv_rfdr_provider,
 rfdr_phone                                      as quiv_rfdr_phone,
 rfdr_salutation                                 as quiv_rfdr_salutation,
 rfdr_index                                      as quiv_rfdr_index,
                                                 
 --                                              
 -- feet                                         
 --                                              
 feet_desc                                       as quiv_feet_desc,
 --                                              
 -- prov                                         
 --                                              
 prov_code                                       as quiv_prov_code,
 prov_provider_num                               as quiv_prov_provider_num,
 prov_name                                       as quiv_prov_name,
 prov_address                                    as quiv_prov_address,
 prov_suburb                                     as quiv_prov_suburb,
 prov_state                                      as quiv_prov_state,
 prov_postcode                                   as quiv_prov_postcode,
 prov_salutation                                 as quiv_prov_salutation,
 prov_phone                                      as quiv_prov_phone,
 prov_bank_code                                  as quiv_prov_bank_code,  

--
-- standard headers
--
 quit__sequence                                  as quiv__sequence,              -- Row-ID
 quit__timestamp                                 as quiv__timestamp,             -- Timestamp
 quit__user_entry                                as quiv__user_entry,            -- Entered by
 quit__status                                    as quiv__status                 -- Record status
 --

 from ((((((quit LEFT JOIN qute ON (quit.quit_qute__sequence = qute.qute__sequence))
                  LEFT JOIN feet ON (qute.qute_feet_code = feet.feet_code))
                  LEFT JOIN patn ON (qute.qute_patn__sequence = patn.patn__sequence))
                  LEFT JOIN dbtr ON (patn.patn_dbtr_code = dbtr.dbtr_code))
                  LEFT JOIN rfdr ON (patn.patn_rfdr_code = rfdr.rfdr_code))
                  LEFT JOIN prov ON (patn.patn_prov_code = prov.prov_code))
                  LEFT JOIN hlfd ON (patn.patn_hlfd_code = hlfd.hlfd_code)

;

grant select,insert,update on quiv to public;

-- ------------------------------------------------------------
-- Functions
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
create or replace function quiv_insert_row(a_quiv quiv)
returns integer as $$
    DECLARE
      rec_quiv quiv%ROWTYPE;
      x_update text := '';
      sep char(1) := '';

    BEGIN
      -- add the quiv record 
      select quiv_insert_row()
        into rec_quiv.quiv__sequence;

      -- change to supplied values
      -- (only QUIT vars)

      if (a_quiv.quiv_date_service is not null) then
        x_update := sep || 'quit_date_service = \'' || a_quiv.quiv_date_service || '\'::timestamp';
        sep = ',';
      end if;
      
      if (a_quiv.quiv_serv_code is not null) then
        x_update := sep || 'quit_serv_code = \'' || a_quiv.quiv_serv_code || '\'::text';
        sep = ',';
      end if;
      
      if (a_quiv.quiv_multireduction is not null) then
        x_update := sep || 'quit_multireduction = \'' || a_quiv.quiv_multireduction || '\'::text';
        sep = ',';
      end if;
      
      if (a_quiv.quiv_percentage is not null) then
        x_update := sep || 'quit_percentage = \'' || a_quiv.quiv_percentage || '\'::integer';
        sep = ',';
      end if;
      
      if (a_quiv.quiv_desc is not null) then
        x_update := sep || 'quit_desc = \'' || a_quiv.quiv_desc || '\'::text';
        sep = ',';
      end if;
      
      if (a_quiv.quiv_amount is not null) then
        x_update := sep || 'quit_amount = \'' || a_quiv.quiv_amount || '\'::numeric';
        sep = ',';
      end if;

      if (a_quiv.quiv_amount_mbs100 is not null) then
        x_update := sep || 'quit_amount_mbs100 = \'' || a_quiv.quiv_amount_mbs100 || '\'::numeric';
        sep = ',';
      end if;

      if (a_quiv.quiv_amount_mbs85 is not null) then
        x_update := sep || 'quit_amount_mbs85 = \'' || a_quiv.quiv_amount_mbs85 || '\'::numeric';
        sep = ',';
      end if;

      if (x_update != '') then    
        x_update = 'update quit set ' || x_update || ' where quit__sequence = ' || rec_quiv.quiv__sequence;
        EXECUTE x_update;
      end if;

      return rec_quiv.quiv__sequence;
    END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Insert a row and return the PK sequence id (used by Marvin)
-- ------------------------------------------------------------
create or replace function quiv_insert_row()
returns integer as $$
    DECLARE
      rec_quit quit%ROWTYPE;

    BEGIN
      
      -- add the quit record 
      select quit_insert_row()
        into rec_quit.quit__sequence;

      return rec_quit.quit__sequence;
    END;
$$
LANGUAGE 'plpgsql';


-- ------------------------------------------------------------
-- update quiv row
-- ------------------------------------------------------------
create or replace function quiv_update_row(a_new quiv, a_old quiv)
returns integer as $$
    DECLARE
      rec_quit quit%ROWTYPE;

    BEGIN

      -- ----------------------------------------
      -- get serv data
      -- ----------------------------------------
      if ( COALESCE(a_new.quiv_serv_code,'') != COALESCE(a_old.quiv_serv_code,'') ) then
        select  fept_desc,
                fept_amount + fept_gst_amount
        into    a_new.quiv_desc,
                a_new.quiv_amount
        from    fept
        where   fept_serv_code = a_new.quiv_serv_code
        and     fept_feet_code = a_new.quiv_feet_code;

      end if;

      -- ----------------------------------------
      -- sanity checks
      -- ----------------------------------------
      if (a_new.quiv_percentage > 100) then
        a_new.quiv_percentage := 100;
      elseif (a_new.quiv_percentage < 0) then
        a_new.quiv_percentage := 0;
      elseif (a_new.quiv_percentage is null) then
        a_new.quiv_percentage := 100;
      end if;

      -- ----------------------------------------
      -- normalise MSR flag
      -- ----------------------------------------
      -- default to "M"
      a_new.quiv_multireduction := COALESCE(a_new.quiv_multireduction, 'M');
      -- exclude consults
      if (a_new.quiv_serv_code in ('104','105')) then
        a_new.quiv_multireduction := '-';
      end if;
      -- convert non-"-" to "M"
      if (a_new.quiv_multireduction != '-') then
        a_new.quiv_multireduction := 'M';
      end if;
      -- reverting from "M" to "-"
      if (a_new.quiv_multireduction = '-' and a_old.quiv_multireduction = 'M') then
        a_new.quiv_amount := a_new.quiv_amount_normal;
      end if;

      a_new.quiv_percentage = 100;
      -- ----------------------------------------
      -- update the quit record 
      -- ----------------------------------------
      update quit
      set    quit_date_service    = a_new.quiv_date_service,
             quit_serv_code       = a_new.quiv_serv_code,
             quit_multireduction  = a_new.quiv_multireduction,
             quit_percentage      = coalesce(a_new.quiv_percentage, 100),
             quit_desc            = a_new.quiv_desc,
             quit_amount          = coalesce(a_new.quiv_amount, 0.0),
             quit_amount_mbs100   = coalesce(a_new.quiv_amount_mbs100, 0,0),
             quit_amount_mbs85    = coalesce(a_new.quiv_amount_mbs85, 0.0),
             quit_qute__sequence  = a_new.quiv_qute__sequence,
             quit_patn__sequence  = a_new.quiv_patn__sequence
      where  quit__sequence  = a_new.quiv__sequence;

      -- ----------------------------------------
      -- update multireduction
      -- ... this has to be done after the save
      -- ----------------------------------------
--      if (a_new.quiv_multireduction != a_old.quiv_multireduction) then
        PERFORM quiv_ms_adjust(a_new.quiv_qute__sequence);
--      end if;


      return rec_quit.quit__sequence;
    END;
$$
LANGUAGE 'plpgsql';


-- ------------------------------------------------------------
-- Perform a multiservice reduction on the items of an quotation.
--     The reductions are fixed at 100/50/25 ...
-- ------------------------------------------------------------

 create or replace function quiv_ms_adjust(a_qute__sequence qute.qute__sequence%TYPE)
 returns void as $$
    DECLARE
         
    tmp_rec record;
    count integer;
    new_pc integer;
 
    BEGIN
 
    count := 0;
 
    -- change the percentage for MSR items
    --  and adjust the amounts

    FOR tmp_rec in EXECUTE '
      select    quiv_amount_normal,
                quiv_amount_mbs100,
                quiv_amount_mbs85,
                quiv__sequence
      from      quiv
      where     quiv_qute__sequence = ' || a_qute__sequence || '
        and     quiv_multireduction != ''-''
        and     quiv_serv_code not in (''104'',''105'')
      order by  quiv_amount_normal desc;'
 
    LOOP
        count := count + 1;
        new_pc := 25;
        if ( count = 1) then
          new_pc := 100;
        end if;
        if ( count = 2) then
          new_pc := 50;
        end if;
 
        update  quit
        set     quit_percentage         = new_pc,
                quit_amount             = fn_round_amount(tmp_rec.quiv_amount_normal * (new_pc/100::numeric)),
                quit_amount_mbs100      = fn_round_amount(tmp_rec.quiv_amount_mbs100 * (new_pc/100::numeric)),
                quit_amount_mbs85       = fn_round_amount(tmp_rec.quiv_amount_mbs85 * (new_pc/100::numeric))
        where   quit__sequence = tmp_rec.quiv__sequence;
 
    END LOOP;
 
    return;
    END;
$$
LANGUAGE 'plpgsql';
 

-- ------------------------------------------------------------
-- Rules
-- ------------------------------------------------------------

-- Insert Rule
-- The insert is more flexibly handled by a function;
--    eg quiv_insert_row() in quiv-functions.sql
   create or replace rule rl_quiv_insert as on insert to quiv
        do instead
        (
          select quiv_insert_row(new.*);
        );

-- Update rule
 create or replace rule rl_quiv_update as on update
        to quiv
        do instead
        (
          select quiv_update_row(new.*, old.*);
        );
 
 -- Delete rule
 create or replace rule rl_quiv_delete as on delete
        to quiv
        do instead
        (
         delete from quit
         where  quit__sequence  = old.quiv__sequence
        );

-- add the default metadata
select create_metadata('quiv');
update mtcl set mtcl_title = 'Quotation Items', mtcl_userlabel = '<B>Item:</B> %1<BR><I>%2</I><BR><B>Fee:</B> %3<BR><B>Standard Fee:</B> %4, quiv_serv_code, quiv_desc, quiv_amount, quiv_amount_normal', mtcl_order_by = 'order by quiv_amount desc' where mtcl_name = 'quiv';
--
-- /home/source/bin/makeDBSubstitute.sh module database table
-- update mtat set mtat_title = '_@ATT_TITLE@_', mtat_displen = _@ATT_DISPLEN@_, mtat_input_method = '_@ATT_INPUT_METHOD@_', mtat_view_order = _@ATT_VIEW_ORDER@_ where mtat_name = '_@ATT_NAME@_';

update mtat set mtat_input_method = 'SYSTEM' where mtat_class_name = 'quiv';

update mtat set mtat_title = 'Item', mtat_displen = 8, mtat_input_method = '10', mtat_view_order = 10 where mtat_name = 'quiv_serv_code';
update mtat set mtat_title = '%', mtat_displen = 3, mtat_input_method = 'RO', mtat_view_order = 12 where mtat_name = 'quiv_percentage';
update mtat set mtat_title = 'MSR', mtat_displen = 3, mtat_input_method = '10', mtat_view_order = 14 where mtat_name = 'quiv_multireduction';
update mtat set mtat_title = 'Description', mtat_displen = 20, mtat_input_method = '10', mtat_view_order = 30 where mtat_name = 'quiv_desc';
update mtat set mtat_title = 'Item Amount', mtat_displen = 10, mtat_input_method = '10', mtat_view_order = 40 where mtat_name = 'quiv_amount';
update mtat set mtat_title = 'MBS', mtat_displen = 10, mtat_input_method = '10', mtat_view_order = 42 where mtat_name = 'quiv_amount_mbs100';
update mtat set mtat_title = 'MEDICARE', mtat_displen = 10, mtat_input_method = '10', mtat_view_order = 44 where mtat_name = 'quiv_amount_mbs85';
update mtat set mtat_title = 'Total Amount', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 90 where mtat_name = 'quiv_total_amount';
update mtat set mtat_title = 'Date added', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 90 where mtat_name = 'quiv_date_service';
update mtat set mtat_title = 'Patient ID', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 92 where mtat_name = 'quiv_patn__sequence';
update mtat set mtat_title = 'Quotation ID', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 94 where mtat_name = 'quiv_qute__sequence';
update mtat set mtat_title = 'Patient Name', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 96 where mtat_name = 'quiv_patn_name';
update mtat set mtat_title = 'Patient Address', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 98 where mtat_name = 'quiv_patn_address';
update mtat set mtat_title = 'Total Amount Mbs100', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 100 where mtat_name = 'quiv_total_amount_mbs100';
update mtat set mtat_title = 'Patient Suburb', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 100 where mtat_name = 'quiv_patn_suburb';
update mtat set mtat_title = 'Quotation Description', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 102 where mtat_name = 'quiv_qute_desc';
update mtat set mtat_title = 'Patn State', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 104 where mtat_name = 'quiv_patn_state';
update mtat set mtat_title = 'Quotation Date Created', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 106 where mtat_name = 'quiv_qute_date_created';
update mtat set mtat_title = 'Patient Postcode', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 108 where mtat_name = 'quiv_patn_postcode';
update mtat set mtat_title = 'Total Amount Mbs85', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 110 where mtat_name = 'quiv_total_amount_mbs85';
update mtat set mtat_title = 'Patient Dob', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 110 where mtat_name = 'quiv_patn_dob';
update mtat set mtat_title = 'Patient Hlfd Code', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 112 where mtat_name = 'quiv_patn_hlfd_code';
update mtat set mtat_title = 'Patient Ins Level', mtat_displen = 1, mtat_input_method = 'SYSTEM', mtat_view_order = 120 where mtat_name = 'quiv_patn_ins_level';
update mtat set mtat_title = 'Patient Healthnumb', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 130 where mtat_name = 'quiv_patn_healthnumb';
update mtat set mtat_title = 'Patient Medicare', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 140 where mtat_name = 'quiv_patn_medicare';
update mtat set mtat_title = 'Patient Healthcard', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 150 where mtat_name = 'quiv_patn_healthcard';
update mtat set mtat_title = 'Patient Patf Code', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 160 where mtat_name = 'quiv_patn_patf_code';
update mtat set mtat_title = 'Patient Claim Number', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 170 where mtat_name = 'quiv_patn_claim_number';
update mtat set mtat_title = 'Patient Accident Date', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 180 where mtat_name = 'quiv_patn_accident_date';
update mtat set mtat_title = 'Debtor Full Address', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 190 where mtat_name = 'quiv_dbtr_full_address';
update mtat set mtat_title = 'Debtor Name', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 200 where mtat_name = 'quiv_dbtr_name';
update mtat set mtat_title = 'Debtor Address', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 210 where mtat_name = 'quiv_dbtr_address';
update mtat set mtat_title = 'Debtor Suburb', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 220 where mtat_name = 'quiv_dbtr_suburb';
update mtat set mtat_title = 'Debtor State', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 230 where mtat_name = 'quiv_dbtr_state';
update mtat set mtat_title = 'Referrer Code', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 240 where mtat_name = 'quiv_rfdr_code';
update mtat set mtat_title = 'Referrer Name', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 250 where mtat_name = 'quiv_rfdr_name';
update mtat set mtat_title = 'Referrer Street', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 260 where mtat_name = 'quiv_rfdr_street';
update mtat set mtat_title = 'Referrer Suburb', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 270 where mtat_name = 'quiv_rfdr_suburb';
update mtat set mtat_title = 'Referrer Postcode', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 280 where mtat_name = 'quiv_rfdr_postcode';
update mtat set mtat_title = 'Referrer State', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 290 where mtat_name = 'quiv_rfdr_state';
update mtat set mtat_title = 'Referrer Provider Number', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 300 where mtat_name = 'quiv_rfdr_provider';
update mtat set mtat_title = 'Referrer Phone', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 310 where mtat_name = 'quiv_rfdr_phone';
update mtat set mtat_title = 'Referrer Salutation', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 320 where mtat_name = 'quiv_rfdr_salutation';
update mtat set mtat_title = 'Referrer Index', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 330 where mtat_name = 'quiv_rfdr_index';
update mtat set mtat_title = 'Feetype Code', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 340 where mtat_name = 'quiv_feet_code';
update mtat set mtat_title = 'Feetype Description', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 350 where mtat_name = 'quiv_feet_desc';
update mtat set mtat_title = 'Provider Code', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 360 where mtat_name = 'quiv_prov_code';
update mtat set mtat_title = 'Provider Providerider Num', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 370 where mtat_name = 'quiv_prov_provider_num';
update mtat set mtat_title = 'Provider Name', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 380 where mtat_name = 'quiv_prov_name';
update mtat set mtat_title = 'Provider Address', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 390 where mtat_name = 'quiv_prov_address';
update mtat set mtat_title = 'Provider Suburb', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 400 where mtat_name = 'quiv_prov_suburb';
update mtat set mtat_title = 'Provider State', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 410 where mtat_name = 'quiv_prov_state';
update mtat set mtat_title = 'Provider Postcode', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 420 where mtat_name = 'quiv_prov_postcode';
update mtat set mtat_title = 'Provider Salutation', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 430 where mtat_name = 'quiv_prov_salutation';
update mtat set mtat_title = 'Provider Phone', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 440 where mtat_name = 'quiv_prov_phone';
update mtat set mtat_title = 'Provider Bank Code', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 450 where mtat_name = 'quiv_prov_bank_code';
update mtat set mtat_title = 'Sequence', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 460 where mtat_name = 'quiv__sequence';
update mtat set mtat_title = 'Timestamp', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 470 where mtat_name = 'quiv__timestamp';
update mtat set mtat_title = 'User Entry', mtat_displen = 30, mtat_input_method = 'SYSTEM', mtat_view_order = 480 where mtat_name = 'quiv__user_entry';
update mtat set mtat_title = 'Status', mtat_displen = 1, mtat_input_method = 'SYSTEM', mtat_view_order = 490 where mtat_name = 'quiv__status';



-- add foreign
delete from mtfn where mtfn_master_class = 'quiv';
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Items','quiv','quiv_qute__sequence','qutv','qutv__sequence','qutv__sequence' );
insert into mtfn(mtfn_title,mtfn_master_class,mtfn_key,mtfn_other_class,mtfn_join,mtfn_view) values( 'Services','quiv','quiv_serv_code','serv','serv_code','serv_desc' );

-- make the child of qutv
delete from mtrl where mtrl_other_class = 'quiv';
insert into mtrl(mtrl_title, mtrl_master_class, mtrl_key, mtrl_other_class, mtrl_join) values('Items', 'qutv', 'qutv__sequence', 'quiv', 'quiv_qute__sequence');

-- add conf entries for MBS85, MBS100 and REFRESH

insert  into conf(conf_code,conf_desc,conf_value)
select  'MBS100', 'MBS Fee Level Code', 'SCH'
where   'MBS100' not in (select conf_code from conf);

insert  into conf(conf_code, conf_desc, conf_value)
select  'MBS85', 'MEDICARE Rebate Level', 'REB'
where   'MBS85' not in (select conf_code from conf);

insert  into conf(conf_code, conf_desc, conf_value)
select  'QMVRELATIONWIDGET_ALWAYS_REFRESH_SET_QUIV', 'QUIV Refresh Flag', 'yes'
where   'QMVRELATIONWIDGET_ALWAYS_REFRESH_SET_QUIV' not in (select conf_code from conf);

-- update history;
\set mttb_name '\'quiv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

