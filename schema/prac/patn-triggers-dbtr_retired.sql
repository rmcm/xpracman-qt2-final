--     $Id$
--
--   Copyright 2011 X=X Computer Software Trust
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
-- check debtor (patn) - don't use retired debtors
-- ------------------------------------------------------------

create or replace function fn_dbtr_retired()
returns trigger
as $$
    DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    x_dbtr_name dbtr.dbtr_group%TYPE;

    BEGIN
        -- RAISE NOTICE 'fn_dbtr_retired: patn__sequence = %', new.patn__sequence;

        -- ----------------------------------------
        -- if detbor retired then raise
        -- raise an exception
        -- ----------------------------------------
        if ( new.patn_dbtr_code is not null and
              ( (new.patn_dbtr_code != old.patn_dbtr_code)
               or (old.patn_dbtr_code is null ) ) ) then
          select dbtr_name
          into   x_dbtr_name
          from   dbtr
          where dbtr_code = new.patn_dbtr_code
          and   dbtr_group = 'RETIRED';

          if ( found ) then
             RAISE EXCEPTION '% (%) is not an active debtor, please select another',
                   x_dbtr_name, new.patn_dbtr_code;
              return null;
          end if;
        end if;

    return new;

    END;
  $$ LANGUAGE 'plpgsql';

drop trigger tr_dbtr_retired on patn;
create trigger tr_dbtr_retired before update
    on patn for each row
    execute procedure fn_dbtr_retired();


-- update history;
\set mttb_name '\'patn\''
\set mttb_rcs_header '\'patn-triggers_dbtr_retired\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
