--     $Id$
--
--   Copyright 2009 X=X Computer Software Trust
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
-- check referror (patn) - don't use retired referrors
-- ------------------------------------------------------------

create or replace function fn_rfdr_retired()
returns trigger
as $$
    DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    x_rfdr_name rfdr.rfdr_index%TYPE;

    BEGIN
        -- RAISE NOTICE 'fn_rfdr_retired: patn__sequence = %', new.patn__sequence;

        -- ----------------------------------------
        -- if referror retired then raise
        -- raise an exception
        -- ----------------------------------------
        if ( new.patn_rfdr_code is not null and 
              ( (new.patn_rfdr_code != old.patn_rfdr_code)
               or (old.patn_rfdr_code is null ) ) ) then
          select rfdr_name
          into   x_rfdr_name
          from   rfdr
          where rfdr_code = new.patn_rfdr_code
          and   rfdr_index = 'RETIRED';

          if ( found ) then
             RAISE EXCEPTION '% (%) is not an active referror, please select another', 
                   x_rfdr_name, new.patn_rfdr_code;
              return null;
          end if;
        end if;

    return new;

    END;
  $$ LANGUAGE 'plpgsql';

drop trigger tr_rfdr_retired on patn;
create trigger tr_rfdr_retired before update
    on patn for each row
    execute procedure fn_rfdr_retired();

-- ------------------------------------------------------------
-- check referror (evnt) - don't use retired referrors
-- ------------------------------------------------------------

create or replace function fn_evnt_rfdr_retired()
returns trigger
as $$
    DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    x_rfdr_name rfdr.rfdr_index%TYPE;

    BEGIN
        -- RAISE NOTICE 'fn_evnt_rfdr_retired: evnt__sequence = %', new.evnt__sequence;

        -- ----------------------------------------
        -- if referror retired then raise
        -- raise an exception
        -- ----------------------------------------
        if ( new.evnt_rfdr_code is not null and 
              ( (new.evnt_rfdr_code != old.evnt_rfdr_code)
               or (old.evnt_rfdr_code is null ) ) ) then
          select rfdr_name
          into   x_rfdr_name
          from   rfdr
          where rfdr_code = new.evnt_rfdr_code
          and   rfdr_index = 'RETIRED';

          if ( found ) then
             RAISE EXCEPTION '% (%) is not an active referror, please select another', 
                   x_rfdr_name, new.evnt_rfdr_code;
              return null;
          end if;
        end if;

    return new;

    END;
  $$ LANGUAGE 'plpgsql';

drop trigger tr_evnt_rfdr_retired on evnt;
create trigger tr_evnt_rfdr_retired before update
    on evnt for each row
    execute procedure fn_evnt_rfdr_retired();

-- update history;
\set mttb_name '\'patn\''
\set mttb_rcs_header '\'patn-triggers_rfdr_retired\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
