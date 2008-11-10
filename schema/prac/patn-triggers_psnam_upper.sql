--     $Id$
-- 
--   Copyright 2007 X=X Computer Software Trust
--    		  Kangaroo Ground Australia 3097
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

--

create or replace function fn_patn_psnam_upper()
returns trigger as $$
    DECLARE
    BEGIN
      -- dont upcase existing names
      if ( TG_OP = 'UPDATE') then
        if (length(COALESCE(old.patn_psnam, '')) > 0) then
          return new;
        end if;
      end if;
      if ( length(COALESCE(new.patn_psnam, '')) > 0 ) then
        new.patn_psnam := upper(new.patn_psnam);
      end if;
      return new;
    END;
$$
LANGUAGE 'plpgsql';

drop trigger tr_patn_psnam_upper on patn;
create trigger tr_patn_psnam_upper before insert or update
    on patn for each row
    execute procedure fn_patn_psnam_upper();


-- update history;
\set mttb_name '\'patn\''
\set mttb_rcs_header '\'$Id:$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
