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

create or replace function fn_paym_null2default()
returns trigger
as 'DECLARE
       tmp_bkdp__sequence bkdp.bkdp__sequence%TYPE; 
    BEGIN
        if (new.paym_tdtp_code is null) then
          new.paym_tdtp_code := ''-'';
        end if;

        if (new.paym_bkdp__sequence is null) then
          new.paym_bkdp__sequence := 0;
        end if;

        -- make sure the holding record in bkdp is present
        if (TG_OP = ''INSERT'') then
          select bkdp__sequence
          into   tmp_bkdp__sequence
          from   bkdp
          where  bkdp__sequence = 0;

          if not found then
             insert into bkdp (bkdp_desc, bkdp__sequence)
                    values (''Unbanked Payments'',0);
          end if;
        end if;

    return new;
    END;'
    LANGUAGE 'plpgsql';

drop trigger tr_paym_null2default on paym;
create trigger tr_paym_null2default before insert or update
    on paym for each row
    execute procedure fn_paym_null2default();

--
-- PL function to prevent change or deletion
--

create or replace function fn_paym_closeddeposit()
returns trigger
as 'DECLARE
    x_paym_bkdp__sequence paym.paym_bkdp__sequence%TYPE;
    x_mtau_attributes mtau.mtau_attributes%TYPE := '''';
    x_mtau_before mtau.mtau_before%TYPE := '''';
    x_mtau_after mtau.mtau_after%TYPE := '''';
    x_sep char(1) := ''~'';

    BEGIN

    if (TG_OP = ''INSERT'') then
      x_paym_bkdp__sequence := new.paym_bkdp__sequence;
    else
      x_paym_bkdp__sequence := old.paym_bkdp__sequence;
    end if;

    --
    -- ok to proceed if not allocated to bank batch or the user has administrator access
    --

    if (x_paym_bkdp__sequence is null or x_paym_bkdp__sequence = 0) then
      if (TG_OP = ''DELETE'') then
        return old;
      else
        return new;
      end if;
    else
          -- 32 is office-admin
      if ( (select perms::integer & 32 from mvac_user where username = current_user) = 32 ) then

        if (TG_OP = ''DELETE'') then
  
          insert into mtau(mtau_table_name, mtau_row_sequence, mtau_operation, mtau_attributes, mtau_before, mtau_after)
                  select    ''paym'', old.paym__sequence, TG_OP,
                            x_sep || ''paym_date_entry'' || x_sep ||
                            ''paym_user_entry'' || x_sep ||
                            ''paym_site_entry'' || x_sep ||
                            ''paym_amount'' || x_sep ||
                            ''paym_tdtp_code'' || x_sep ||
                            ''paym_drawer'' || x_sep ||
                            ''paym_bank'' || x_sep ||
                            ''paym_branch'' || x_sep ||
                            ''paym_bkdp__sequence'',
                            x_sep || old.paym_date_entry || x_sep ||
                            old.paym_user_entry || x_sep ||
                            old.paym_site_entry || x_sep ||
                            to_char(old.paym_amount,''9999999999.99'') || x_sep ||
                            old.paym_tdtp_code || x_sep ||
                            old.paym_drawer || x_sep ||
                            old.paym_bank || x_sep ||
                            old.paym_branch || x_sep ||
                            old.paym_bkdp__sequence,
                            NULL;
          return old;

        else

          -- paym_date_entry
          if (new.paym_date_entry <> old.paym_date_entry ) then
            x_mtau_attributes := x_mtau_attributes || x_sep || ''paym_date_entry'';
            x_mtau_before     := x_mtau_before     || x_sep || old.paym_date_entry;
            x_mtau_after      := x_mtau_after      || x_sep || new.paym_date_entry;
          end if;
  
          -- paym_user_entry
          if (new.paym_user_entry <> old.paym_user_entry ) then
            x_mtau_attributes := x_mtau_attributes || x_sep || ''paym_user_entry'';
            x_mtau_before     := x_mtau_before     || x_sep || old.paym_user_entry;
            x_mtau_after      := x_mtau_after      || x_sep || new.paym_user_entry;
          end if;
  
          -- paym_site_entry
          if (new.paym_site_entry <> old.paym_site_entry ) then
            x_mtau_attributes := x_mtau_attributes || x_sep || ''paym_site_entry'';
            x_mtau_before     := x_mtau_before     || x_sep || old.paym_site_entry;
            x_mtau_after      := x_mtau_after      || x_sep || new.paym_site_entry;
          end if;
  
          -- paym_amount
          if (new.paym_amount <> old.paym_amount ) then
            x_mtau_attributes := x_mtau_attributes || x_sep || ''paym_amount'';
            x_mtau_before     := x_mtau_before     || x_sep || to_char(old.paym_amount, ''9999999999.99'');
            x_mtau_after      := x_mtau_after      || x_sep || to_char(new.paym_amount, ''9999999999.99'');
          end if;
  
          -- paym_tdtp_code
          if (new.paym_tdtp_code <> old.paym_tdtp_code ) then
            x_mtau_attributes := x_mtau_attributes || x_sep || ''paym_tdtp_code'';
            x_mtau_before     := x_mtau_before     || x_sep || old.paym_tdtp_code;
            x_mtau_after      := x_mtau_after      || x_sep || new.paym_tdtp_code;
          end if;
  
          -- paym_drawer
          if (new.paym_drawer <> old.paym_drawer ) then
            x_mtau_attributes := x_mtau_attributes || x_sep || ''paym_drawer'';
            x_mtau_before     := x_mtau_before     || x_sep || old.paym_drawer;
            x_mtau_after      := x_mtau_after      || x_sep || new.paym_drawer;
          end if;
  
          -- paym_bank
          if (new.paym_bank <> old.paym_bank ) then
            x_mtau_attributes := x_mtau_attributes || x_sep || ''paym_bank'';
            x_mtau_before     := x_mtau_before     || x_sep || old.paym_bank;
            x_mtau_after      := x_mtau_after      || x_sep || new.paym_bank;
          end if;
  
          -- paym_branch
          if (new.paym_branch <> old.paym_branch ) then
            x_mtau_attributes := x_mtau_attributes || x_sep || ''paym_branch'';
            x_mtau_before     := x_mtau_before     || x_sep || old.paym_branch;
            x_mtau_after      := x_mtau_after      || x_sep || new.paym_branch;
          end if;
  
          -- paym_bkdp__sequence
          if (new.paym_bkdp__sequence <> old.paym_bkdp__sequence ) then
            x_mtau_attributes := x_mtau_attributes || x_sep || ''paym_bkdp__sequence'';
            x_mtau_before     := x_mtau_before     || x_sep || old.paym_bkdp__sequence;
            x_mtau_after      := x_mtau_after      || x_sep || new.paym_bkdp__sequence;
          end if;
  
  
          insert into mtau(mtau_table_name, mtau_row_sequence, mtau_operation, mtau_attributes, mtau_before, mtau_after)
                  select ''paym'', new.paym__sequence, TG_OP, x_mtau_attributes, x_mtau_before, x_mtau_after;
          return new;
        end if;
      end if;
    end if;

    

    RAISE EXCEPTION ''This record cannot be changed or deleted - the deposit batch number % has been created. Please contact your Systems Administrator for assistance.'', x_paym_bkdp__sequence;
 
        END;'
    LANGUAGE 'plpgsql';

drop trigger tr_paym_closeddeposit on paym;
create trigger tr_paym_closeddeposit before update or delete
    on paym for each row
    execute procedure fn_paym_closeddeposit();

-- ------------------------------------------------------------
-- Function: Keep bkdp_amount upto date when admin modifies
-- ------------------------------------------------------------

create or replace function fn_setbkdpamount()
returns trigger
as 'DECLARE
    x_before bkdp.bkdp_amount%TYPE;
    x_after bkdp.bkdp_amount%TYPE;
    old_paym_bkdp__sequence paym.paym_bkdp__sequence%TYPE;
    new_paym_bkdp__sequence paym.paym_bkdp__sequence%TYPE;
    x_sep char(1) := ''~'';

    BEGIN

    -- more than one batch may be involved - eg in transfer.

    old_paym_bkdp__sequence := -1;
    new_paym_bkdp__sequence := -1;

    if (TG_OP = ''DELETE'') then
      old_paym_bkdp__sequence := coalesce(old.paym_bkdp__sequence,0);
    end if;

    if (TG_OP = ''INSERT'') then
      new_paym_bkdp__sequence := coalesce(new.paym_bkdp__sequence,0);
    end if;

    if (TG_OP = ''UPDATE'') then
      old_paym_bkdp__sequence := coalesce(old.paym_bkdp__sequence,0);
      new_paym_bkdp__sequence := coalesce(new.paym_bkdp__sequence,0);
    end if;

    -- the source of the transaction - adjust old balance
    if (old_paym_bkdp__sequence > 0) then

        -- current balance
      select    bkdp_amount
      into      x_before
      from      bkdp
      where     bkdp__sequence = old_paym_bkdp__sequence;

        -- new balance
      select    sum(coalesce(paym_amount,0.00::numeric))
      into      x_after
      from      paym
      where     paym_bkdp__sequence = old_paym_bkdp__sequence;

      update bkdp
      set    bkdp_amount = x_after
      where  bkdp__sequence = old_paym_bkdp__sequence;

      insert into mtau(mtau_table_name, mtau_row_sequence, mtau_operation, mtau_attributes, mtau_before, mtau_after)
        select ''bkdp'', old_paym_bkdp__sequence, ''UPDATE'', x_sep||''bkdp_amount'', 
                x_sep||to_char(x_before, ''9999999999.99''),
                x_sep||to_char(x_after, ''9999999999.99'');

    end if;

    -- the destination for the transaction - adjust new balance
    if (new_paym_bkdp__sequence > 0) then

        -- current balance
      select    bkdp_amount
      into      x_before
      from      bkdp
      where     bkdp__sequence = new_paym_bkdp__sequence;

        -- new balance
      select    sum(coalesce(paym_amount,0.00::numeric))
      into      x_after
      from      paym
      where     paym_bkdp__sequence = new_paym_bkdp__sequence;

      update bkdp
      set    bkdp_amount = x_after
      where  bkdp__sequence = new_paym_bkdp__sequence;

      insert into mtau(mtau_table_name, mtau_row_sequence, mtau_operation, mtau_attributes, mtau_before, mtau_after)
        select ''bkdp'', new_paym_bkdp__sequence, ''UPDATE'', x_sep||''bkdp_amount'', 
                x_sep||to_char(x_before, ''9999999999.99''),
                x_sep||to_char(x_after, ''9999999999.99'');

    end if;

    if (TG_OP = ''DELETE'') then
      return old;
    else
      return new;
    end if;

    END;'
  LANGUAGE 'plpgsql';

drop trigger tr_setbkdpamount on paym;
create trigger tr_setbkdpamount after insert or update or delete
    on paym for each row
    execute procedure fn_setbkdpamount();

-- update history;
\set mttb_name '\'paym\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
