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
-- PL functions related to cred table
--
--
-- PL function to update invoice credit totals.
--

create or replace function cred_tr_before()
returns trigger as $$

  DECLARE
    sum_cred_amount cred.cred_amount%TYPE;
    sum_cred_gst_amount cred.cred_gst_amount%TYPE;
    sum_unpaid cred.cred_amount%TYPE;
    sum_gst_unpaid cred.cred_amount%TYPE;
    rec_invc invc%ROWTYPE;
    rec_paym paym%ROWTYPE;
    sum_other_paym paym.paym_amount%TYPE;
    avail_paym paym.paym_amount%TYPE;

    BEGIN
        -- ----------------------------------------
        -- get credit/debit totals from invoice
        -- ----------------------------------------
        select *
        into   rec_invc
        from   invc
        where  invc__sequence = new.cred_invc__sequence;

        -- ----------------------------------------
        -- THERE MUST BE AN INVOICE!!
        -- ----------------------------------------
        if ( not found ) then
          raise EXCEPTION 'cred_tr_before:invoice # % does not exist', new.cred_invc__sequence;
          return NULL;
        end if;

        -- ----------------------------------------
        -- get the current balances
        -- ----------------------------------------

        sum_unpaid := rec_invc.invc_amount - rec_invc.invc_paid_amount;
        sum_gst_unpaid := rec_invc.invc_gst_amount - rec_invc.invc_paid_gst_amount;
        if ( TG_OP = 'UPDATE' ) then
          sum_unpaid := sum_unpaid + old.cred_amount;
          sum_gst_unpaid := sum_gst_unpaid + old.cred_gst_amount;
        end if;

        -- ----------------------------------------
        -- If no payment record, then create dummy
        -- ----------------------------------------

        if ( new.cred_paym__sequence = 0 or new.cred_paym__sequence is null ) then
          new.cred_paym__sequence := 0;
          select  *
          into    rec_paym
          from    paym
          where   paym__sequence = new.cred_paym__sequence;

          -- if not found create a dummy to temporarily satisfy constraints
          if ( not found ) then
            insert into paym(paym__sequence) values(0);
            -- raise notice 'cred_tr_before:inserting new payment';
          end if;
        end if;

        -- get the paym record

        select  *
        into    rec_paym
        from    paym
        where   paym__sequence = new.cred_paym__sequence;

        -- get the payment already-allocated amount

        select coalesce(sum(cred_amount + cred_gst_amount), '0.00'::numeric)
        into   sum_other_paym
        from   cred
        where  cred_paym__sequence = new.cred_paym__sequence
        and    cred__sequence != new.cred__sequence;

        -- if the payment is zero, then default the payment amount to the outstanding
        -- ( this will be adjusted after credit amounts are finalised)

        if ( rec_paym.paym_amount = 0.00::numeric ) then
          avail_paym := sum_unpaid + sum_gst_unpaid;
        else
          avail_paym := rec_paym.paym_amount - sum_other_paym;
        end if;

        if ( avail_paym < 0 ) then
          avail_paym = 0;
        end if;

        sum_cred_amount := new.cred_amount + new.cred_gst_amount;

        -- if new credit amounts are 0, then look for outstanding amounts
        -- to use as default

        if ( new.cred_amount = 0 and new.cred_gst_amount = 0 ) then
          -- raise notice 'cred_tr_before: adjusting zero amount to %/%', sum_unpaid, sum_gst_unpaid;
          new.cred_amount     := sum_unpaid;
          new.cred_gst_amount := sum_gst_unpaid;

         end if;

        -- ----------------------------------------
        -- sanity checks
        -- ----------------------------------------

        -- No overpayments of GST ----------
        -- ---------------------------------
        if ( new.cred_gst_amount > sum_gst_unpaid ) then
          -- raise notice 'cred_tr_before: adjusting GST overpayment to % from %', new.cred_gst_amount, sum_gst_unpaid;
          new.cred_gst_amount := sum_gst_unpaid;
        end if;

        -- No negatives amounts ----------
        -- -------------------------------
        if ( new.cred_amount  < 0 ) then
          -- raise notice 'cred_tr_before: adjusting negative payment to 0.00 from %', new.cred_amount;
          new.cred_amount := 0;
        end if;
        if ( new.cred_gst_amount < 0) then
          -- raise notice 'cred_tr_before: adjusting negative GST payment to 0.00 from %', new.cred_gst_amount;
          new.cred_gst_amount := 0;
        end if;

        -- No overpayments if GST is unpaid
        -- --------------------------------
        if ( (sum_unpaid < new.cred_amount) and (sum_gst_unpaid > new.cred_gst_amount) ) then
          raise exception 'You cannot make an overpayment which ignores outstanding GST';
          return null;
        end if;

    return new;
    END;
$$  LANGUAGE 'plpgsql';

drop trigger cred_tr_before on cred;
create trigger cred_tr_before before insert or update
    on cred for each row
    execute procedure cred_tr_before();


--  ---------------------------------------------------------------------
-- PL function to update invoice and payment credit totals.  If INSERT
-- or DELETE, then there is only one invoice/payment to adjust - the
-- new and the old respectively. In the case of UPDATE, both the new
-- and old invoices/payments need to be adjusted.
--  ---------------------------------------------------------------------
create or replace function invc_credit_totals()
returns trigger as $$
   DECLARE

    x_cred_invc invc.invc__sequence%TYPE;
    x_cred_paym paym.paym__sequence%TYPE;
    x_cred_invc_old invc.invc__sequence%TYPE;
    x_cred_paym_old paym.paym__sequence%TYPE;
    update_count integer;

    BEGIN
    -- ---------------------------------------------
    -- Set the OLD/NEW Invoice/Payment sequence-ID's
    -- ---------------------------------------------
      if ( TG_OP = 'DELETE' ) then
        x_cred_invc = old.cred_invc__sequence;
        x_cred_paym = old.cred_paym__sequence;
      else
        x_cred_invc = new.cred_invc__sequence;
        x_cred_paym = new.cred_paym__sequence;
      end if;
      if ( TG_OP = 'INSERT' ) then
        x_cred_invc_old = new.cred_invc__sequence;
        x_cred_paym_old = new.cred_paym__sequence;
      else
        x_cred_invc_old = old.cred_invc__sequence;
        x_cred_paym_old = old.cred_paym__sequence;
      end if;

    -- ---------------------------------------------
    -- Update the invoice totals (paid and GST)
    -- ---------------------------------------------
      update invc set
        invc_paid_amount = (
           select  coalesce(sum(cred_amount),0.00)
           from    cred
           where   cred_invc__sequence = invc.invc__sequence),
        invc_paid_gst_amount = (
           select  coalesce(sum(cred_gst_amount),0.00)
           from    cred
           where   cred_invc__sequence = invc.invc__sequence)
      where invc__sequence in (x_cred_invc, x_cred_invc_old);
      GET DIAGNOSTICS update_count = ROW_COUNT;
      RAISE NOTICE 'invc_cred_totals::invoices = %', update_count;

    -- ---------------------------------------------
    -- Update the payment total
    -- ---------------------------------------------
      update paym set
          paym_amount = (
               select  coalesce(sum(cred.cred_amount),0.00) + coalesce(sum(cred_gst_amount),0.00)
               from    cred
               where   cred_paym__sequence = paym.paym__sequence)
      where paym__sequence in (x_cred_paym, x_cred_paym_old);
      GET DIAGNOSTICS update_count = ROW_COUNT;
      RAISE NOTICE 'invc_cred_totals::payments = %', update_count;

    -- ---------------------------------------------
    -- Return the correct record
    -- ---------------------------------------------
      if ( TG_OP = 'DELETE' ) then
        return old;
      else
        return new;
      end if;
    END;
$$ LANGUAGE 'plpgsql';

drop trigger invc_credit_totals on cred;
create trigger invc_credit_totals after insert or update or delete
    on cred for each row
    execute procedure invc_credit_totals();



-- update history;
\set mttb_name '\'cred\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
