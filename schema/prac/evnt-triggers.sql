--     $Id$
--
--   Copyright 2003 X=X Computer Software Trust
--                Kangaroo Ground Australia 3097
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

-- ------------------------------------------------------------
-- Copy defaults from patient
-- ------------------------------------------------------------

create or replace function fn_copypatndata()
returns trigger as $$
  DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    rec_patn patn%ROWTYPE;

    BEGIN
        -- RAISE NOTICE 'fn_copypatndata: patn__sequence = %', new.evnt_patn__sequence;

        -- ----------------------------------------
        -- need patn_sequence for the search
        -- ----------------------------------------
        if (new.evnt_patn__sequence is null) then
           return new;
        end if;

        -- ----------------------------------------
        -- if update and same patient then do nothing
        -- ----------------------------------------
        if ( TG_OP = 'UPDATE' ) then
          if (new.evnt_patn__sequence = old.evnt_patn__sequence) then
            return new;
          end if;
        end if;

        -- ----------------------------------------
        -- search for patient data
        -- ----------------------------------------
        select  *
        into    rec_patn
        from    patn
        where   patn__sequence = new.evnt_patn__sequence;

        -- ----------------------------------------
        -- if not found return
        -- ----------------------------------------
        if ( not found ) then
          return new;
        end if;
        -- RAISE NOTICE 'fn_copypatndata: found ....  patn__sequence = %', new.evnt_patn__sequence;
        -- ----------------------------------------
        -- if not null and new values not null then copy to evnt
        -- ----------------------------------------
        if ( rec_patn.patn_prov_code is not null
                and (new.evnt_prov_code is null
                     or new.evnt_prov_code = '-') ) then
          -- RAISE NOTICE 'fn_copypatndata: setting prov_code from patn';
          new.evnt_prov_code = rec_patn.patn_prov_code;
        end if;
        if ( rec_patn.patn_rfdr_code is not null
                and (new.evnt_rfdr_code is null
                     or new.evnt_rfdr_code = '-') ) then
          -- RAISE NOTICE 'fn_copypatndata: setting rfdr_code from patn';
          new.evnt_rfdr_code = rec_patn.patn_rfdr_code;
        end if;

    return new;

    END;
$$  LANGUAGE 'plpgsql';

drop trigger tr_copypatndata on evnt;
create trigger tr_copypatndata before insert or update
    on evnt for each row
    execute procedure fn_copypatndata();

create or replace function fn_blockevent()
returns trigger as $$
  DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    rec_patn patn%ROWTYPE;

    BEGIN
        -- ----------------------------------------
        -- search for patient block
        -- ----------------------------------------
        select  *
        into    rec_patn
        from    patn
        where   patn__sequence = new.evnt_patn__sequence
        and     patn_patf_code = 'BLOCK';

        -- ----------------------------------------
        -- if found block
        -- ----------------------------------------
        if ( found ) then
          RAISE EXCEPTION 'This patient is blocked from new appointments';
          return null;
        end if;

        -- ok, no block
        return new;
    END;
$$ LANGUAGE 'plpgsql';

drop trigger tr_blockevent on evnt;
create trigger tr_blockevent before insert or update
    on evnt for each row
    execute procedure fn_blockevent();

-- update history;
\set mttb_name '\'evnt\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
