-- -*- sql -*-
 
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
  
--
-- PL function to set defaults if null
--

create or replace function fn_patn_null2default()
returns trigger as $$
        DECLARE
          rec_dbtr dbtr%ROWTYPE;
          dbtr_changed bool;
          dbtr_request bool;
          x_dbtr_empl_code text := '';
          x_patn_as_debtor text;

        BEGIN

        x_patn_as_debtor := '/'||trim( both from to_char(new.patn__sequence,'0000000'));

        if (new.patn_feet_code is null) then
          select  coalesce(fn_column_default('patn', 'patn_feet_code'), '-')
          into    new.patn_feet_code;
        end if;

        -- Set self-debtor by default
        if (new.patn_dbtr_code is null) then
          new.patn_dbtr_code := '-';
        end if;

        -- if the patient is the debtor and patient details have changed
        -- then set debtor details 
        -- RAISE NOTICE '>>> % : % : % : %', x_patn_as_debtor, new.patn_dbtr_code, new.patn_title, old.patn_title;
        if (TG_OP = 'UPDATE' 
                and new.patn_sync_debtor = 1
                and new.patn_dbtr_code = x_patn_as_debtor) then
          if (   new.patn_title   != COALESCE(old.patn_title, '')
             or new.patn_psnam    != COALESCE(old.patn_psnam, '')
             or new.patn_fsnam    != COALESCE(old.patn_fsnam, '')
             or new.patn_address  != COALESCE(old.patn_address, '')
             or new.patn_suburb   != COALESCE(old.patn_suburb, '')
             or new.patn_postcode != COALESCE(old.patn_postcode, '')
             or new.patn_state    != COALESCE(old.patn_state, '')
             or new.patn_phone    != COALESCE(old.patn_phone, '') ) then
            new.patn_dbtr_code := '-';
          end if;
        end if;
        -- RAISE NOTICE '>>> % : %', x_patn_as_debtor, new.patn_dbtr_code;

        -- the "-E" code triggers using employer as debtor, creating/update a debtor
        --     record.
        if (upper(new.patn_dbtr_code) = '-E') then
          select  fn_make_empl_dbtr(new.patn_empl_code)
          into    x_dbtr_empl_code;
          -- RAISE NOTICE 'checking -E ... %', x_dbtr_empl_code;
          if (x_dbtr_empl_code is not null) then
            new.patn_dbtr_code = x_dbtr_empl_code;
          end if;         
        end if;


        -- the "-" character trigger insert/update of patient dbtr record
        -- also refersh if already a patient debtor
        if (( new.patn_dbtr_code = '-' or new.patn_dbtr_code = x_patn_as_debtor )
            and new.patn__sequence is not null
            and new.patn__sequence > 0 
            ) then
          -- RAISE NOTICE 'Self debtor - %', new.patn_dbtr_code;
          dbtr_request := false;
          if ( new.patn_dbtr_code = '-' and new.patn_sync_debtor != -1 ) then
            dbtr_request := true;
          end if;

          new.patn_dbtr_code := x_patn_as_debtor;
          
          select  *
          into    rec_dbtr
          from    dbtr
          where   dbtr_code = new.patn_dbtr_code;
          if ( not found ) then
            rec_dbtr.dbtr_code = new.patn_dbtr_code;
            insert into dbtr( dbtr_code )
                      values(rec_dbtr.dbtr_code );
          end if;
          dbtr_changed := false;

          if ( rec_dbtr.dbtr_name is null or dbtr_request ) then
            rec_dbtr.dbtr_name := trim(both from 
                                        coalesce(new.patn_title,'') ||' '
                                        ||coalesce(new.patn_fsnam,'')||' '
                                        ||coalesce(new.patn_psnam,'XXX-NO-NAME'));
            
            -- RAISE NOTICE 'Changing dbtr_name';
            dbtr_changed := true;
          end if;

          if ( rec_dbtr.dbtr_address is null or dbtr_request ) then
            rec_dbtr.dbtr_address := new.patn_address;
            dbtr_changed := true;
          end if;

          if ( rec_dbtr.dbtr_suburb is null or dbtr_request ) then
            rec_dbtr.dbtr_suburb := new.patn_suburb;
            dbtr_changed := true;
          end if;

          if ( rec_dbtr.dbtr_state is null  or dbtr_request) then
            rec_dbtr.dbtr_state := new.patn_state;
            dbtr_changed := true;
          end if;

          if ( rec_dbtr.dbtr_postcode is null or dbtr_request ) then
            rec_dbtr.dbtr_postcode := new.patn_postcode;
            dbtr_changed := true;
          end if;

          if ( rec_dbtr.dbtr_phone is null or dbtr_request ) then
            rec_dbtr.dbtr_phone := new.patn_phone;
            dbtr_changed := true;
          end if;

          if ( dbtr_changed = true) then
            update dbtr
            set    dbtr_name = rec_dbtr.dbtr_name,
                   dbtr_address = rec_dbtr.dbtr_address,
                   dbtr_suburb = rec_dbtr.dbtr_suburb,
                   dbtr_state = rec_dbtr.dbtr_state,
                   dbtr_postcode = rec_dbtr.dbtr_postcode,
                   dbtr_phone = rec_dbtr.dbtr_phone
            where  dbtr_code = rec_dbtr.dbtr_code;
          end if;
                              
        end if;

        if (new.patn_empl_code is null) then
          new.patn_empl_code := '-';
        end if;

        if (new.patn_rfdr_code is null) then
          new.patn_rfdr_code := '-';
        end if;

        if (new.patn_prov_code is null) then
          select  coalesce(fn_column_default('patn', 'patn_prov_code'), '-')
          into    new.patn_prov_code;
        end if;

        if (new.patn_flno is null or new.patn_flno = '-') then
          new.patn_flno = new.patn__sequence;
        end if;

    return new;
    END;
$$
LANGUAGE 'plpgsql';

drop trigger tr_patn_null2default on patn;
create trigger tr_patn_null2default before insert or update
    on patn for each row
    execute procedure fn_patn_null2default();

--
-- PL function to prevent deletion or update of printed invoices
--

create or replace function fn_patn_activepatient()
returns trigger
as '
        BEGIN

          RAISE EXCEPTION ''This record cannot be deleted :: #%'', old.patn__sequence;

        END;'
    LANGUAGE 'plpgsql';

drop trigger tr_patn_activepatient on patn;
create trigger tr_patn_activepatient before delete
    on patn for each row
    execute procedure fn_patn_activepatient();


-- ------------------------------------------------------------
-- Function: Make a debtor record from an employer record
-- .. DBTR_CODE = '/E-' || EMPL_CODE
-- ------------------------------------------------------------

create or replace function fn_make_empl_dbtr(text)
returns text as $$
  DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    a_empl_code ALIAS for $1;
    rec_empl empl%ROWTYPE;
    rec_dbtr dbtr%ROWTYPE;
    x_dbtr_empl_code text := '/E-' || a_empl_code;

  BEGIN

    -- Find the employer record
    select  *
    into    rec_empl
    from    empl
    where   empl_code = a_empl_code;

    if (NOT FOUND) then
      RAISE EXCEPTION 'No Employer Found: %', a_empl_code;
      return null;
    end if;

   -- look for an existing debtor record for this Employer 
    select  *
    into    rec_dbtr
    from    dbtr
    where   dbtr_code = x_dbtr_empl_code;

    -- Find the employer record
    if (NOT FOUND) then
      insert into  dbtr(dbtr_code,
                        dbtr_name,
                        dbtr_address,
                        dbtr_suburb,
                        dbtr_state,
                        dbtr_postcode)
                 values(x_dbtr_empl_code,
                        rec_empl.empl_name,
                        rec_empl.empl_address,
                        rec_empl.empl_suburb,
                        rec_empl.empl_state,
                        rec_empl.empl_postcode);

    else
      update dbtr
      set    dbtr_name = rec_empl.empl_name,
             dbtr_address = rec_empl.empl_address,
             dbtr_suburb = rec_empl.empl_suburb,
             dbtr_state = rec_empl.empl_state,
             dbtr_postcode = rec_empl.empl_postcode
      where  dbtr_code = x_dbtr_empl_code;

    end if;

    return x_dbtr_empl_code;
  END;
$$
LANGUAGE 'plpgsql';

-- update history;
\set mttb_name '\'patn\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
