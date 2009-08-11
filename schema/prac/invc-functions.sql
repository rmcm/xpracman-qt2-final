-- -*-sql-*-
--      $Id$ 
--
--    Copyright 2000 X=X Computer Software Trust 
--                  Kangaroo Ground Australia 3097 
--
--
--   This is free software; you can redistribute it and/or modify 
--   it under the terms of the GNU General Public License published by 
--   the Free Software Foundation; either version 2, or (at your option) 
--   any later version. 
--
--   This software is distributed in the hope that it will be useful, 
--   but WITHOUT ANY WARRANTY; without even the implied warranty of 
--   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
--   GNU General Public License for more details. 
--
--   You should have received a copy of the GNU General Public License 
--   along with this software; see the file COPYING.  If not, write to 
--   the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
--
--   Report problems and direct all questions to: 
--
--       Rex McMaster, rmcm@compsoft.com.au 
--
-- General functions associated with the invc table
--

--
-- ------------------------------------------------------------
-- PL function which returns the invoice balance from the fields
-- in the invc table - no update of recalculation from svpf/cred tables.
-- is performed.
-- ------------------------------------------------------------
--

create or replace function invc_balance(integer)
returns numeric
as 'DECLARE
        invc_number alias for $1;
        rec_invc invc%ROWTYPE;
        balance invc.invc_amount%TYPE;

    BEGIN
        -- get record
        select  *
        into    rec_invc
        from    invc
        where   invc__sequence = invc_number;

        if ( not found ) then
          return null;
        end if;

        if (rec_invc.invc_amount is null) then
          rec_invc.invc_amount = 0;
        end if;

        if (rec_invc.invc_gst_amount is null) then
          rec_invc.invc_gst_amount = 0;
        end if;

        if (rec_invc.invc_paid_amount is null) then
          rec_invc.invc_paid_amount = 0;
        end if;

        if (rec_invc.invc_paid_gst_amount is null) then
         rec_invc.invc_paid_gst_amount  = 0;
        end if;

        balance = rec_invc.invc_amount
                + rec_invc.invc_gst_amount
                - rec_invc.invc_paid_amount
                - rec_invc.invc_paid_gst_amount;

      return balance;

    END;'
    LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Balance on invoice ## days ago
-- ------------------------------------------------------------

create or replace function invc_balance_then(integer, integer)
returns numeric
as 'DECLARE
        invc_number alias for $1;
        days_ago alias for $2;
        rec_invc invc%ROWTYPE;
        rec_cred cred%ROWTYPE;
        debits svpf.svpf_amount%TYPE;
        credits cred.cred_amount%TYPE;
        balance invc.invc_amount%TYPE;

    BEGIN
        -- get invc record
        select  *
        into    rec_invc
        from    invc
        where   invc__sequence = invc_number
        and     invc_date_printed <= now() - (to_char(days_ago,''9999'') || '' days'')::interval;

        if ( not found ) then
          -- RAISE NOTICE ''invc_balance_then - % not found'', invc_number;
          return null;
        end if;

        if (rec_invc.invc_amount is null) then
          rec_invc.invc_amount = 0;
        end if;

        if (rec_invc.invc_gst_amount is null) then
          rec_invc.invc_gst_amount = 0;
        end if;


        -- get cred record
        select  sum(cred_amount), sum(cred_gst_amount)
        into    rec_cred.cred_amount, rec_cred.cred_gst_amount
        from    cred,paym
        where   cred_invc__sequence = invc_number
        and     cred_paym__sequence = paym.paym__sequence
        and     paym_date_entry <= now() - (to_char(days_ago,''9999'') || '' days'')::interval;

        if ( not found ) then
          rec_cred.cred_amount = 0;
          rec_cred.cred_gst_amount = 0;
        end if;

        if (rec_cred.cred_amount is null) then
          rec_cred.cred_amount = 0;
        end if;

        if (rec_cred.cred_gst_amount is null) then
         rec_cred.cred_gst_amount  = 0;
        end if;

        balance = rec_invc.invc_amount
                + rec_invc.invc_gst_amount
                - rec_cred.cred_amount
                - rec_cred.cred_gst_amount;

      return balance;

    END;'
    LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- function to update the printed/reprinted dates
-- ------------------------------------------------------------

create or replace function invc_setprintdates(integer)
returns bool
as 'DECLARE
        invc_number alias for $1;
        printed invc.invc_date_printed%TYPE;

    BEGIN
        -- get record
        select  invc_date_printed
        into    printed
        from    invc
        where   invc__sequence = invc_number;

        if ( not found ) then
          return false;
        end if;

        if (printed is null) then
          update invc set invc_date_printed = ''now''::timestamp
          where  invc__sequence = invc_number;
        else
          update invc set invc_date_reprint = ''now''::timestamp
          where  invc__sequence = invc_number;
        end if;

      return true;

    END;'
    LANGUAGE 'plpgsql';

--

-- ------------------------------------------------------------
-- Return the age interval of an invoice -
--        - 0,30,90,120+
-- ------------------------------------------------------------

create or replace function invc_age_period(integer, integer)
returns integer
as 'DECLARE
    tmp_invc__sequence ALIAS for $1;
    tmp_daysago ALIAS for $2;
    period integer;

    BEGIN

    -- 3600*24 = 86400
--    select  trunc(date_part(''epoch'',age(invc_date_printed))/(2592000))
    select  trunc(date_part(''epoch'',age(invc_date_printed))/86400)
    into    period
    from    invc
    where   invc_date_printed is not null
    and     invc__sequence = tmp_invc__sequence;

    if ( not found ) then
      return 0;
    end if;
    period := (period-tmp_daysago)/30;
    if ( period < 0 ) then
      period := 0;
    end if;

    if ( period > 4 ) then
      period := 4;
    end if;

    period := period * 30;

    return period;
      
    END;'
  LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Return formatted patient name
-- ------------------------------------------------------------

create or replace function invc_patient_name(integer)
returns text
as 'DECLARE
    invc_number alias for $1;
    rec_patn patn%ROWTYPE;
    patient_name text;

    BEGIN
    select coalesce(patn_psnam, '') || '', '' || coalesce(patn_fsnam, '')
    into   patient_name
    from   patn,invc
    where  invc__sequence = invc_number
    and    patn__sequence = invc_patn__sequence;

    if (not found) then
      RAISE EXCEPTION ''Patient not found for invoice number <%>'', invc_number;
      return null;
    end if;
      
    
    return patient_name;
    END;'
  LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Update all the invoice totals, return the balance
-- ------------------------------------------------------------

create or replace function invc_fix_totals(integer)
returns numeric
as 'DECLARE
    invc_number alias for $1;

    BEGIN

        update  invc
            set invc_amount = (
                select  coalesce(sum(svpf_amount),0.00)
                from    svpf
                where   svpf_invc__sequence = invc.invc__sequence)
        where   invc__sequence = invc_number;
        update invc
            set invc_gst_amount = (
                select coalesce(sum(svpf_gst_amount),0.00)
                from svpf
                where svpf_invc__sequence = invc.invc__sequence)
        where   invc__sequence = invc_number;
        update invc
            set invc_paid_amount = (
                select coalesce(sum(cred_amount),0.00)
                from cred
                where cred_invc__sequence = invc.invc__sequence)
        where   invc__sequence = invc_number;
        update invc
            set invc_paid_gst_amount = (
                select coalesce(sum(cred_gst_amount),0.00)
                from cred
                    where cred_invc__sequence = invc.invc__sequence)
        where   invc__sequence = invc_number;
    
    return invc_balance(invc_number);

    END;'
  LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Locate/Create an invoice
-- ------------------------------------------------------------

create or replace function invc_locate(integer,text,text,text,text,text,text,boolean,boolean)
returns integer as $$

  DECLARE
    a_patn__sequence    ALIAS for $1;
    a_dbtr_code         ALIAS for $2;
    a_prov_code         ALIAS for $3;
    a_feet_code         ALIAS for $4;
    a_rfdr_code         ALIAS for $5;
    a_empl_code         ALIAS for $6;
    a_hlfd_code         ALIAS for $7;
    a_printed           ALIAS for $8;   -- true = ignore printed state
    a_create            ALIAS for $9;   -- true = create if doesn't exist

    x_invc__sequence integer := null;
    x_oid oid;

  BEGIN

    -- look for the newest unprinted invoice for the debtor/patient/prov/feet
    select max(invc__sequence)
    into   x_invc__sequence
    from   invc
    where  invc_patn__sequence  = a_patn__sequence
    and    invc_dbtr_code       = a_dbtr_code
    and    invc_prov_code       = a_prov_code
    and    invc_feet_code       = a_feet_code
    and    invc_empl_code       = a_empl_code
    and    invc_hlfd_code       = a_hlfd_code
    and    (invc_date_printed is null or a_printed);

    if (x_invc__sequence is null) then
      if (a_create) then
        -- Insert a new invc record, from patn defaults
        insert into invc(
            invc_dbtr_code,
            invc_prov_code,
            invc_patn__sequence,
            invc_empl_code,
            invc_feet_code,
            invc_rfdr_code,
            invc_rfdr_date,
            invc_rfdr_period,
            invc_hlfd_code,
            invc_ins_level,
            invc_healthnumb,
            invc_healthcard,
            invc_claim_number,
            invc_accident_date)
        select
            a_dbtr_code,
            a_prov_code,
            a_patn__sequence,
            a_empl_code,
            a_feet_code,
            a_rfdr_code,
            patn.patn_ref_date,
            patn.patn_ref_period,
            a_hlfd_code,
            patn.patn_ins_level,
            patn.patn_healthnumb,
            patn.patn_healthcard,
            patn.patn_claim_number,
            patn.patn_accident_date
        from   patn
        where  patn__sequence = a_patn__sequence;

        if (FOUND) then
          GET DIAGNOSTICS x_oid = RESULT_OID;
          select  invc__sequence
          into    x_invc__sequence
          from    invc
          where   oid = x_oid;
          if (FOUND) then
             return x_invc__sequence;
          end if;
        end if;
      end if;
      return null;
    end if;

    return x_invc__sequence;

  END;
$$
LANGUAGE PLPGSQL;
