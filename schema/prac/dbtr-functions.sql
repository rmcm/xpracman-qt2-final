--  -*- sql -*-
--
 
--    $Id$

--  Copyright 2001 X=X Computer Software Trust
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
-- Function to insert a new record with dummy dbtr_code
-- ------------------------------------------------------------

create or replace function dbtr_insert_row()
returns integer as $$
    DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    tmp_dbtr__sequence dbtr.dbtr__sequence%TYPE;

    BEGIN

         -- get a dbtr__sequence
         select nextval('dbtr_dbtr__sequence_seq')
         into tmp_dbtr__sequence;

         -- add the new row
         insert into dbtr( dbtr_code, dbtr__sequence )
                   values( tmp_dbtr__sequence::text, tmp_dbtr__sequence);

    return tmp_dbtr__sequence;
    END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- The total amount outstanding from this debtor
-- ------------------------------------------------------------

create or replace function dbtr_total_balance(text)
returns numeric as $$
    DECLARE

    tmp_rec record;

    BEGIN

    for tmp_rec in EXECUTE '
      select    sum(invc_balance(invc__sequence)) as total_balance
      from      invc
      where     invc_dbtr_code = ' || quote_literal($1) || ';'

    LOOP
        return (((ceil(tmp_rec.total_balance * 20) * 5)::numeric)/100)::numeric(12,2);
    END LOOP;

    return null;

    END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- The total amount outstanding from this debtor at a particular time
-- ------------------------------------------------------------

create or replace function dbtr_total_balance(text,integer)
returns numeric as $$
    DECLARE
    tmp_dbtr_code ALIAS for $1;
    tmp_daysago ALIAS for $2;
    tmp_rec record;
    tmp_daysago_str text;

    BEGIN

    tmp_daysago_str := tmp_daysago || ' days';
    -- RAISE NOTICE '%', tmp_daysago;
    for tmp_rec in EXECUTE '
      select    sum(invc_balance_then(invc__sequence,' || tmp_daysago || ')) as total_balance
      from      invc
      where     invc_dbtr_code = ' || quote_literal(tmp_dbtr_code) || '
      and       invc_date_printed is not null
      and       date(invc_date_printed) <= now() - ' || quote_literal(tmp_daysago_str) || '::interval;'

    LOOP
        if ( tmp_rec.total_balance is null ) then
          return 0.00::numeric;
        else
          return (((ceil(tmp_rec.total_balance * 20) * 5)::numeric)/100)::numeric(12,2);
        end if;
    END LOOP;

    return 0.00::numeric;

    END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- The period balance for this debtor at a particular time
-- ------------------------------------------------------------

create or replace function dbtr_period_balance(text, integer, integer)
returns numeric as $$
    DECLARE
    tmp_dbtr_code ALIAS for $1;
    tmp_period ALIAS for $2;
    tmp_daysago ALIAS for $3;
    tmp_rec record;
    sqlexec text;
    str_daysago text;

    BEGIN
    str_daysago := tmp_daysago || ' days';
    sqlexec = '
      select    sum(invc_balance_then(invc__sequence,' || tmp_daysago || ')) as period_total
      from      invc
      where     invc_dbtr_code = ' || quote_literal(tmp_dbtr_code) || '
      and       invc_date_printed is not null
      and       date(invc_date_printed) <= now() - ' || quote_literal(str_daysago) || '::interval 
      and       invc_age_period(invc__sequence,'||tmp_daysago||') = ' || tmp_period || ';';

    -- raise notice 'SQlEXEC: %', sqlexec;

    for tmp_rec in EXECUTE sqlexec
    LOOP
    -- RAISE NOTICE '% % %',tmp_period,tmp_rec.period_total,sqlexec;
        if ( tmp_rec.period_total is null ) then
          return 0.00::numeric;
        else
          return (((ceil(tmp_rec.period_total * 20) * 5)::numeric)/100)::numeric(12,2);
        end if;
    END LOOP;

    return 0.00::numeric;

    END;
$$
LANGUAGE 'plpgsql';
--
-- ------------------------------------------------------------
-- The last time an invoice was printed/reprinted for this debtor.
-- ------------------------------------------------------------

create or replace function dbtr_last_invc_printed(text)
returns date as $$
    DECLARE

    tmp_dbtr_code ALIAS for $1;  
    tmp_date_printed date; 
    tmp_date_reprint date;

    BEGIN

    select max(invc_date_printed), max(invc_date_reprint)
    into   tmp_date_printed,tmp_date_reprint
    from   invc
    where  invc_dbtr_code = tmp_dbtr_code;

    if ( tmp_date_printed is null ) then
      tmp_date_printed = 'epoch'::date;
    end if;

    if ( tmp_date_reprint is null ) then
      tmp_date_reprint = 'epoch'::date;
    end if;

    if ( tmp_date_reprint > tmp_date_printed ) then
      return tmp_date_reprint;
    else
      return tmp_date_printed;
    end if;

    END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Create an indexable alhpa field based on a patient list
-- ------------------------------------------------------------

create or replace function dbtr_patient_key(text)
returns text as $$
    DECLARE

    tmp_dbtr_code ALIAS for $1;
    tmp_rec record;
    query text;
    label text;
    count integer;

    BEGIN


--    query := 'select patn_psnam,patn_fsnam,invc_dbtr_code'
--           + 'from   invc,patn'
--           + 'where  invc_dbtr_code = ' + tmp_dbtr_code
--           + 'and    patn__sequence = invc_patn__sequence;';
--
--
--    for tmp_rec in EXECUTE query

    count := 0;

    for tmp_rec in
        select  distinct upper(patn_psnam) as patn_psnam,upper(patn_fsnam) as patn_fsnam,invc_dbtr_code
        from    invc,patn
        where   invc_dbtr_code = tmp_dbtr_code
        and     patn__sequence = invc_patn__sequence
      
    LOOP
        -- if more than one, then skip out
        count := count + 1;
        if (count > 1) then
          exit;
        end if;
        label := tmp_rec.patn_psnam || ',' || tmp_rec.patn_fsnam || ',' || tmp_rec.invc_dbtr_code;
    END LOOP;

    if (count = 1) then
        return label;
    end if;

    select      dbtr_name
    into        label
    from        dbtr
    where       dbtr_code = tmp_dbtr_code;

    if (not found) then
        return 'Unknown: ' || tmp_dbtr_code;
    end if;
    
    return label || ' *(Multiple Patients)';

    END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Create a combined debtor address
-- ------------------------------------------------------------

create or replace function dbtr_address(text)
returns text as $$
    DECLARE

    tmp_dbtr_code ALIAS for $1;
    tmp_rec record;
    address text;
    BEGIN


    select      dbtr.*
    into        tmp_rec
    from        dbtr
    where       dbtr_code = tmp_dbtr_code;

    if ( not found ) then
      --RAISE EXCEPTION 'Debtor code not found - %', tmp_dbtr_code;
      return '';
    end if;

    address := coalesce(tmp_rec.dbtr_name, '') || '<br>' 
                || coalesce(tmp_rec.dbtr_address, '') || '<br>'
                || coalesce(tmp_rec.dbtr_suburb, '') ||' '
                || coalesce(tmp_rec.dbtr_state, '') ||' '
                || coalesce(tmp_rec.dbtr_postcode, '');

    return address;

    END;
$$
LANGUAGE 'plpgsql';


-- update history;
\set mttb_name '\'dbtr\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
