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
-- Integrity functions and utility functions for invoices and statements
--

--
--
-- PL function to update invoice totals.
-- Although this script contains triggers for the cred and svpf files,
--  these are directed to maintaining invc totals
--

-- ------------------------------------------------------------
-- Function to calculate invoice totals
-- ------------------------------------------------------------
create or replace function invc_totals(integer)
returns numeric as $$
    DECLARE
    invc_number alias for $1;
    sum_svpf_amount svpf.svpf_amount%TYPE;
    sum_svpf_gst_amount svpf.svpf_gst_amount%TYPE;
    sum_cred_amount cred.cred_amount%TYPE;
    sum_cred_gst_amount cred.cred_gst_amount%TYPE;
    sum_svpf_balance svpf.svpf_amount%TYPE;

    BEGIN
        -- get service totals
        select sum(svpf.svpf_amount),
               sum(svpf.svpf_gst_amount)
        into   sum_svpf_amount,
               sum_svpf_gst_amount
        from	svpf
        where svpf.svpf_invc__sequence = invc_number;

        if (sum_svpf_amount is null) then
          sum_svpf_amount = 0.00;
        end if;
        if (sum_svpf_gst_amount is null) then
          sum_svpf_gst_amount = 0.00;
        end if;

        -- get credit totals
        select 	sum(cred.cred_amount),
               	sum(cred.cred_gst_amount)
        into   	sum_cred_amount,
               	sum_cred_gst_amount
        from	cred
        where cred.cred_invc__sequence = invc_number;

        if (sum_cred_amount is null) then
          sum_cred_amount = 0.00;
        end if;
        if (sum_cred_gst_amount is null) then
          sum_cred_gst_amount = 0.00;
        end if;

        update invc
          set invc_amount = sum_svpf_amount,
              invc_gst_amount = sum_svpf_gst_amount,
              invc_paid_amount = sum_cred_amount,
              invc_paid_gst_amount = sum_cred_gst_amount
          where invc.invc__sequence = invc_number;

        sum_svpf_balance := sum_svpf_amount + sum_svpf_gst_amount - sum_cred_amount - sum_cred_gst_amount;

    return sum_svpf_balance;
    END;
$$  LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- PL function to update invoice debit totals.
-- ------------------------------------------------------------

create or replace function invc_debit_totals()
returns trigger as $$
   DECLARE
    sum_svpf_amount svpf.svpf_amount%TYPE;
    sum_svpf_gst_amount svpf.svpf_gst_amount%TYPE;
    x_invc__sequence svpf.svpf_invc__sequence%TYPE;

    BEGIN

        if ( TG_OP = 'DELETE' ) then
          x_invc__sequence := old.svpf_invc__sequence;
        else
          x_invc__sequence := new.svpf_invc__sequence;
        end if;

        -- get service totals
        select sum(svpf_amount),
               sum(svpf_gst_amount)
        into   sum_svpf_amount,
               sum_svpf_gst_amount
        from   svpf
        where svpf_invc__sequence = x_invc__sequence;

        if (sum_svpf_amount is null) then
          sum_svpf_amount = 0.00;
        end if;
        if (sum_svpf_gst_amount is null) then
          sum_svpf_gst_amount = 0.00;
        end if;

        update invc
          set invc_amount = sum_svpf_amount,
              invc_gst_amount = sum_svpf_gst_amount
          where invc__sequence = x_invc__sequence;

    return new;
    END;
$$  LANGUAGE 'plpgsql';


drop trigger invc_debit_totals on svpf;
create trigger invc_debit_totals after insert or update or delete
    on svpf for each row
    execute procedure invc_debit_totals();

-- ------------------------------------------------------------
-- PL function to prevent deletion or update of printed invoices
-- ------------------------------------------------------------

create or replace function fn_invc_closedinvoice()
returns trigger as $$
    DECLARE

    x_sep char(1) := '~';
    BEGIN

    if (TG_OP != 'UPDATE' and TG_OP != 'DELETE') then
      RAISE EXCEPTION 'tr_invc_closedinvoice: this trigger can only be used for UPDATE and DELETE';
    end if;

    -- ------------------------------
    -- if invoice is open, then allow
    -- updates or deletes unconditionally
    -- ------------------------------
    
    if ( old.invc_date_printed is null ) then
      if (TG_OP = 'DELETE') then
        return old;
      else
        return new;
      end if;
    end if;

    -- only printed invoices from now on.

    -- ------------------------------
    -- CONSTRAINTS ON DELETE
    -- ------------------------------

    -- block deletes except office admin(32)
    if (TG_OP = 'DELETE') then
      if ( (select perms::integer & 32 from mvac_user where username = current_user) = 32 ) then
        -- audit
        insert into mtau(mtau_table_name, mtau_row_sequence, mtau_operation, mtau_attributes, mtau_before, mtau_after)
              select    'invc', old.invc__sequence, TG_OP,
                        x_sep || 
                        'invc_dbtr_code' || x_sep ||
                        'invc_bank_code' || x_sep ||
                        'invc_prov_code' || x_sep ||
                        'invc_patn__sequence' || x_sep ||
                        'invc_empl_code' || x_sep ||
                        'invc_feet_code' || x_sep ||
                        'invc_rfdr_code' || x_sep ||
                        'invc_rfdr_date' || x_sep ||
                        'invc_rfdr_period' || x_sep ||
                        'invc_date_created' || x_sep ||
                        'invc_date_printed' || x_sep ||
                        'invc_date_reprint' || x_sep ||
                        'invc_amount' || x_sep ||
                        'invc_paid_amount' || x_sep ||
                        'invc_gst_amount' || x_sep ||
                        'invc_paid_gst_amount',
                        x_sep || 
                        coalesce(old.invc_dbtr_code, '') || x_sep ||
                        coalesce(old.invc_bank_code, '') || x_sep ||
                        coalesce(old.invc_prov_code, '') || x_sep ||
                        coalesce(old.invc_patn__sequence, -1) || x_sep ||
                        coalesce(old.invc_empl_code, '') || x_sep ||
                        coalesce(old.invc_feet_code, '') || x_sep ||
                        coalesce(old.invc_rfdr_code, '') || x_sep ||
                        coalesce(old.invc_rfdr_date::text, '') || x_sep ||
                        coalesce(old.invc_rfdr_period, '') || x_sep ||
                        coalesce(old.invc_date_created::text, '') || x_sep ||
                        coalesce(old.invc_date_printed::text, '') || x_sep ||
                        coalesce(old.invc_date_reprint::text, '') || x_sep ||
                        coalesce(to_char(old.invc_amount,'9999999999.99'), '') || x_sep ||
                        coalesce(to_char(old.invc_paid_amount,'9999999999.99'), '') || x_sep ||
                        coalesce(to_char(old.invc_gst_amount,'9999999999.99'), '') || x_sep ||
                        coalesce(to_char(old.invc_paid_gst_amount,'9999999999.99'), ''),
                        NULL;
        return old;
      else
        RAISE EXCEPTION 'This record cannot be deleted - the invoice number % has been printed. Please contact your Systems Administrator for assistance.', old.invc__sequence;
        return null;
      end if;
    end if;

    -- ------------------------------
    -- CONSTRAINTS ON UPDATE
    -- ------------------------------
    if (TG_OP = 'UPDATE') then

      if ( new.invc_date_printed is null or new.invc_date_printed <> old.invc_date_printed ) then
        if ( (select perms::integer & 32 from mvac_user where username = current_user) = 32 ) then
          -- audit
          insert into mtau(mtau_table_name, mtau_row_sequence, mtau_operation, mtau_attributes, mtau_before, mtau_after)
              select    'invc', old.invc__sequence, TG_OP,
                        x_sep || 
                        'invc_date_printed',
                        x_sep ||
                        coalesce(old.invc_date_printed::text, ''),
                        x_sep ||
                        coalesce(new.invc_date_printed::text, '');
          return new;
        else
        -- dont allow updates to change date printed.
            RAISE EXCEPTION 'The date-printed on this record cannot be updated - the invoice number % has been printed',
                            old.invc__sequence;
          return null;
        end if;
      end if;
    end if;

    -- ------------------------------
    -- Tidy up.
    -- ------------------------------
    if (TG_OP = 'DELETE') then
      return old;
    else
      return new;
    end if;

    END;
$$  LANGUAGE 'plpgsql';

drop trigger tr_invc_closedinvoice on invc;
create trigger tr_invc_closedinvoice before delete or update
    on invc for each row
    execute procedure fn_invc_closedinvoice();

-- ------------------------------------------------------------
-- set invoice defaults if required.
-- ------------------------------------------------------------
create or replace function fn_invc_setdefaults()
returns trigger as $$
    DECLARE
      tmp_patn patn%ROWTYPE;
       
    BEGIN
      if (new.invc_patn__sequence is not null
         and new.invc_patn__sequence > 0
         and new.invc_dbtr_code = '-') then
        select   *
        into     tmp_patn
        from     patn
        where    patn.patn__sequence = new.invc_patn__sequence;

        if ( found ) then
          raise notice 'pat__sequence % to invoice %', new.invc_patn__sequence, new.invc__sequence; 
           new.invc_dbtr_code         = tmp_patn.patn_dbtr_code ;
           new.invc_prov_code         = tmp_patn.patn_prov_code ;
           new.invc_empl_code         = tmp_patn.patn_empl_code ;
           new.invc_feet_code         = tmp_patn.patn_feet_code ;
           new.invc_rfdr_code         = tmp_patn.patn_rfdr_code ;
           new.invc_rfdr_date         = tmp_patn.patn_ref_date ;
           new.invc_rfdr_period       = tmp_patn.patn_ref_period ;
           new.invc_hlfd_code         = tmp_patn.patn_hlfd_code ;
           new.invc_ins_level         = tmp_patn.patn_ins_level ;
           new.invc_healthnumb        = tmp_patn.patn_healthnumb ;
           new.invc_healthcard        = tmp_patn.patn_healthcard ;
           new.invc_claim_number      = tmp_patn.patn_claim_number ;
           new.invc_accident_date     = tmp_patn.patn_accident_date;
        end if;
      end if;

      return new;
    END;
$$  LANGUAGE 'plpgsql';

drop trigger tr_invc_setdefaults on invc;
create trigger tr_invc_setdefaults before insert or update
    on invc for each row
    execute procedure fn_invc_setdefaults();

-- update history;
\set mttb_name '\'invc\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
