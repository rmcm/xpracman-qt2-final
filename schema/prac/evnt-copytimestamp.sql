--     $Id$
--
--   Copyright 2005 X=X Computer Software Trust
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
-- Copy timestamp to evnt row
-- ------------------------------------------------------------

create or replace function fn_copytimestamp()
returns trigger
as $$
    DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    rec_patn patn%ROWTYPE;

    BEGIN
        -- RAISE NOTICE 'fn_copytimestamp: patn__sequence = %', new.evnt_patn__sequence;

        -- ----------------------------------------
        -- if status changes then write note
        -- ----------------------------------------
        if ( new.evnt_apst_code != old.evnt_apst_code and new.evnt_apst_code = 'AT') then
          new.evnt_note_2 := to_char(now(), ' HH24:MI');
          --RAISE NOTICE 'new status is %', new.evnt_note_2;
        end if;

    return new;

    END;
  $$ LANGUAGE 'plpgsql';

drop trigger tr_copytimestamp on evnt;
create trigger tr_copytimestamp before update
    on evnt for each row
    execute procedure fn_copytimestamp();

-- update history;
\set mttb_name '\'evnt\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
