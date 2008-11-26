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
  
-- ------------------------------------------------------------
-- PL function to set defaults if null
-- ------------------------------------------------------------

create or replace function fn_svpf_null2default()
returns trigger as $$

        DECLARE
                tmp_invc__sequence svpf.svpf_invc__sequence%TYPE; 
                tmp_serv_code serv.serv_code%TYPE;
                tmp_fept fept%ROWTYPE;
                tmp_patn patn%ROWTYPE;
                tmp_invc invc%ROWTYPE;
                sequence_id integer;
                x_pos integer;
        BEGIN

        -- Patient is an absolute requirement
        if (new.svpf_patn__sequence is null) then
           RAISE EXCEPTION 'Patient ID number (svpf_patn__sequence) is mandatory';
        end if;

        -- Set svpf_patn__sequence from invoice if necessary and possible
        RAISE NOTICE 'new.svpf_patn__sequence = %', new.svpf_patn__sequence;
        if (new.svpf_patn__sequence = 0
            and new.svpf_invc__sequence is not null
            and new.svpf_invc__sequence > 0) then
            select *
            from   invc
            into   tmp_invc
            where  invc__sequence = new.svpf_invc__sequence;
            if (found) then
              new.svpf_patn__sequence = tmp_invc.invc_patn__sequence;
              RAISE NOTICE 'setting new.svpf_patn__sequence to %', new.svpf_patn__sequence;
            end if;
        end if;

        select *
        into   tmp_patn
        from   patn
        where  patn__sequence = new.svpf_patn__sequence;

        -- add a patn record if missing
        if not found then
          insert into patn(patn__sequence)
          values(new.svpf_patn__sequence);
          select  *
          into    tmp_patn
          from    patn
          where   patn__sequence = new.svpf_patn__sequence;

          if ( not found ) then
            RAISE EXCEPTION 'Unable to create a Patient Record (%)', new.svpf_patn__sequence;
          end if;
        end if;

        -- if missing service code, then use a default
        if (new.svpf_serv_code is null) then
          new.svpf_serv_code := '-';
        end if;

        -- invc can be found or created
        if (new.svpf_invc__sequence is null or new.svpf_invc__sequence = 0) then

          -- look for the newest unprinted invoice for the patient/debtor/prov/feet/rfdr

          select invc_locate(
                tmp_patn.patn__sequence,
                tmp_patn.patn_dbtr_code,
                tmp_patn.patn_prov_code,
                tmp_patn.patn_feet_code,
                tmp_patn.patn_rfdr_code,
                tmp_patn.patn_empl_code,
                tmp_patn.patn_hlfd_code,
                false,          -- unprinted
                true)           -- create if necessary
          into   tmp_invc__sequence;

          if tmp_invc__sequence is null then
              RAISE EXCEPTION 'Unable to locate a suitable invoice';
          else
            -- recover it
            select *
            into   tmp_invc
            from   invc
            where  invc__sequence = tmp_invc__sequence;
            
            new.svpf_invc__sequence := tmp_invc.invc__sequence;
          end if;
        end if;

        -- Now for service code
        select fept.*
        into   tmp_fept
        from   fept,patn
        where  patn__sequence = new.svpf_patn__sequence
        and    fept_serv_code = new.svpf_serv_code
        and    fept_feet_code = patn_feet_code;

        if not found then
          RAISE EXCEPTION 'item % not found for patient %', new.svpf_serv_code,new.svpf_patn__sequence;
          tmp_fept.fept_desc := 'UNKNOWN';
          tmp_fept.fept_amount := 0.00;
          tmp_fept.fept_gst_amount := 0.00;
        end if;
        if ( TG_OP = 'INSERT' ) then
              -- RAISE EXCEPTION 'item = %, desc=%', tmp_fept.fept_serv_code, tmp_fept.fept_desc;
          if (new.svpf_desc is null) then
            new.svpf_desc := tmp_fept.fept_desc;
          end if;
          if (new.svpf_amount is null) then
            new.svpf_amount := fn_round_amount(tmp_fept.fept_amount * (new.svpf_percentage/100::numeric));
          end if;
          if (new.svpf_gst_amount is null and new.svpf_date_service < 'jul 1 2000' ) then
            new.svpf_gst_amount := fn_round_amount(tmp_fept.fept_gst_amount * (new.svpf_percentage/100::numeric));
          end if;
        end if;

        if ( TG_OP = 'UPDATE' ) then
           --   RAISE NOTICE 'item = %, desc=%, perc=%', tmp_fept.fept_serv_code,
           --                                              tmp_fept.fept_desc,
           --                                              new.svpf_percentage;
           --   RAISE NOTICE 'amount = %, perc=%, gst=%', tmp_fept.fept_amount,
           --                                               tmp_fept.fept_gst_percentage,
           --                                               tmp_fept.fept_gst_amount;
           -- item code change - refresh description and amount
           if ( new.svpf_serv_code <> old.svpf_serv_code) then
              new.svpf_desc := tmp_fept.fept_desc;
              new.svpf_amount := fn_round_amount(tmp_fept.fept_amount * (new.svpf_percentage/100::numeric));
           end if;
           -- percentage change - refresh amount from fept, but leave description
           if ( new.svpf_serv_code <> old.svpf_serv_code or new.svpf_percentage <> old.svpf_percentage) then
              new.svpf_amount := fn_round_amount(tmp_fept.fept_amount * (new.svpf_percentage/100::numeric));
           end if;
           -- always recalculate the GST amount
           if ( new.svpf_date_service > 'jul 1 2000' ) then
             new.svpf_gst_amount := fn_round_amount(new.svpf_amount * (tmp_fept.fept_gst_percentage/100::numeric));
           end if;
           -- final sanity checks
           if (new.svpf_amount is null) then
             new.svpf_amount := 0.00;
           end if;
           if (new.svpf_gst_amount is null) then
             new.svpf_gst_amount := 0.00;
           end if;
           if (new.svpf_percentage is null
               or new.svpf_percentage < 0) then
             new.svpf_percentage := 100;
           end if;

        end if;

        -- And check for %P in desc - replace with provider
        -- 
        x_pos := position( '~P' in new.svpf_desc);
        if ( x_pos > 0 ) then
          if ( length(new.svpf_desc) > x_pos + 1 ) then
            new.svpf_desc := substring(new.svpf_desc from 1 for (x_pos-1)) || tmp_patn.patn_prov_code || substring(new.svpf_desc from (x_pos+2));
          else
            new.svpf_desc := substring(new.svpf_desc from 1 for (x_pos-1)) || tmp_patn.patn_prov_code;
          end if;
        end if;
    return new;
    END;
$$  LANGUAGE 'plpgsql';

drop trigger tr_svpf_null2default on svpf;
create trigger tr_svpf_null2default before insert or update
    on svpf for each row
    execute procedure fn_svpf_null2default();

-- ------------------------------------------------------------
-- PL function to prevent change or deleteion
-- ------------------------------------------------------------

create or replace function fn_svpf_closedinvoice()
returns trigger as $$
    DECLARE
    tmp_invc_date_printed invc.invc_date_printed%TYPE;
    old_invc__sequence invc.invc__sequence%TYPE;
    new_invc__sequence invc.invc__sequence%TYPE;
    x_mtau_attributes mtau.mtau_attributes%TYPE := '';
    x_mtau_before mtau.mtau_before%TYPE := '';
    x_mtau_after mtau.mtau_after%TYPE := '';
    x_sep char(1) := '~';
    BEGIN

    old_invc__sequence := -1;
    new_invc__sequence := -1;

    -- ------------------------------  
    -- INSERTS should not be here
    -- ------------------------------  

    if (TG_OP != 'UPDATE' and TG_OP != 'DELETE') then
      RAISE EXCEPTION 'tr_svpf_closedinvoice: this trigger can only be used for UPDATE and DELETE';
    end if;

    -- ------------------------------  
    -- uninteresting DELETES
    -- ------------------------------  
    if ( TG_OP = 'DELETE' ) then
      if (old.svpf_invc__sequence is null or old.svpf_invc__sequence = 0) then
        return old;
      end if;
      old_invc__sequence := old.svpf_invc__sequence;
      new_invc__sequence := old.svpf_invc__sequence;
    end if;


    -- ------------------------------  
    -- UPDATES may be of interest
    -- ------------------------------  
    if (TG_OP = 'UPDATE') then
      old_invc__sequence := old.svpf_invc__sequence;
      new_invc__sequence := new.svpf_invc__sequence;
    end if;
    
    -- ------------------------------  
    -- look for invc_date_printed
    -- ------------------------------ 
    select invc_date_printed
    into   tmp_invc_date_printed
    from   invc
    where  (invc__sequence = old_invc__sequence
           or invc__sequence = new_invc__sequence);

    -- ------------------------------
    -- This trigger might be part 
    -- of an invc delete, so no
    -- parent invc.
    -- ------------------------------
    if ( not found ) then
      tmp_invc_date_printed = now();
    end if;

    -- RAISE NOTICE 'tr_svpf_closedinvoice: tmp_invc_date_printed = %, old_invc__sequence = %, new_invc__sequence = %', tmp_invc_date_printed, old_invc__sequence, new_invc__sequence;

    -- ------------------------------  
    -- ALREADY PRINTED - only
    -- admin can proceed.
    -- ------------------------------

    if ( tmp_invc_date_printed is not null ) then
      -- 32 is office-admin
      if ( (select perms::integer & 32 from mvac_user where username = current_user) != 32 ) then
        RAISE EXCEPTION 'This record cannot be copied, changed or deleted - the invoice number % has been printed. Please contact your Systems Administrator for assistance.',
         old_invc__sequence;
        return null;
      end if;
    end if;

    -- ------------------------------
    -- Audit deletes after printing.
    -- ------------------------------
    if (TG_OP = 'DELETE' and tmp_invc_date_printed is not null ) then
  
      insert into mtau(mtau_table_name, mtau_row_sequence, mtau_operation, mtau_attributes, mtau_before, mtau_after)
              select    'svpf', old.svpf__sequence, TG_OP,
                        x_sep ||
                        'svpf_date_service' || x_sep ||
                        'svpf_serv_code' || x_sep ||
                        'svpf_percentage' || x_sep ||
                        'svpf_desc' || x_sep ||
                        'svpf_amount' || x_sep ||
                        'svpf_gst_amount' || x_sep ||
                        'svpf_invc__sequence' || x_sep ||
                        'svpf_mdaf__sequence' || x_sep ||
                        'svpf_patn__sequence',
                        x_sep ||
                        coalesce( old.svpf_date_service::text, '' ) || x_sep ||
                        coalesce( old.svpf_serv_code, '' ) || x_sep ||
                        coalesce( old.svpf_percentage, -1 ) || x_sep ||
                        coalesce( old.svpf_desc, '' ) || x_sep ||
                        coalesce( to_char(old.svpf_amount,'9999999999.99'), '' ) || x_sep ||
                        coalesce( to_char(old.svpf_gst_amount,'9999999999.99'), '' ) || x_sep ||
                        coalesce( old.svpf_invc__sequence, -1 ) || x_sep ||
                        coalesce( old.svpf_mdaf__sequence, -1 ) || x_sep ||
                        coalesce( old.svpf_patn__sequence, -1 ),
                        NULL;
      return old;

    end if;

    -- ------------------------------
    -- Audit updates after printing
    -- ------------------------------
  
    if ( TG_OP = 'UPDATE' and tmp_invc_date_printed is not null ) then

      insert into mtau(mtau_table_name, mtau_row_sequence, mtau_operation, mtau_attributes, mtau_before, mtau_after)
              select    'svpf', old.svpf__sequence, TG_OP,
                        x_sep ||
                        'svpf_date_service' || x_sep ||
                        'svpf_serv_code' || x_sep ||
                        'svpf_percentage' || x_sep ||
                        'svpf_desc' || x_sep ||
                        'svpf_amount' || x_sep ||
                        'svpf_gst_amount' || x_sep ||
                        'svpf_invc__sequence' || x_sep ||
                        'svpf_mdaf__sequence' || x_sep ||
                        'svpf_patn__sequence',
                        x_sep ||
                        coalesce( old.svpf_date_service::text, '' ) || x_sep ||
                        coalesce( old.svpf_serv_code, '' ) || x_sep ||
                        coalesce( old.svpf_percentage, -1 ) || x_sep ||
                        coalesce( old.svpf_desc, '' ) || x_sep ||
                        coalesce( to_char(old.svpf_amount,'9999999999.99'), '' ) || x_sep ||
                        coalesce( to_char(old.svpf_gst_amount,'9999999999.99'), '' ) || x_sep ||
                        coalesce( old.svpf_invc__sequence, -1 ) || x_sep ||
                        coalesce( old.svpf_mdaf__sequence, -1 ) || x_sep ||
                        coalesce( old.svpf_patn__sequence, -1 ),
                        x_sep ||
                        coalesce( new.svpf_date_service::text, '' ) || x_sep ||
                        coalesce( new.svpf_serv_code, '' ) || x_sep ||
                        coalesce( new.svpf_percentage, -1 ) || x_sep ||
                        coalesce( new.svpf_desc, '' ) || x_sep ||
                        coalesce( to_char(new.svpf_amount,'9999999999.99'), '' ) || x_sep ||
                        coalesce( to_char(new.svpf_gst_amount,'9999999999.99'), '' ) || x_sep ||
                        coalesce( new.svpf_invc__sequence, -1 ) || x_sep ||
                        coalesce( new.svpf_mdaf__sequence, -1 ) || x_sep ||
                        coalesce( new.svpf_patn__sequence, -1 );
      return new;
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

drop trigger tr_svpf_closedinvoice on svpf;
create trigger tr_svpf_closedinvoice before update or delete
    on svpf for each row
    execute procedure fn_svpf_closedinvoice();

-- update history;
\set mttb_name '\'svpf\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
