--     $Id$
-- 
--   Copyright 2005 X=X Computer Software Trust
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
-- PL function to set defaults if null
--

create or replace function empl_tr_before()
returns trigger as $$
  DECLARE

    BEGIN
    -- set a dummy code
    if ( TG_OP = 'UPDATE' or TG_OP = 'INSERT' ) then
      if ( new.empl_code is null ) then
         new.empl_code := new.empl__sequence::text;
      end if;
    end if;

    if ( TG_OP = 'UPDATE' ) then
      if ( old.empl_code = '-' and new.empl_code != '-') then
        new.empl_code := old.empl_code;
        RAISE EXCEPTION 'This code (-) cannot be altered';
        return old;
      end if;
    end if;
    if ( TG_OP = 'DELETE') then
       if ( old.empl_code = '-') then
         RAISE EXCEPTION 'This code (-) cannot be deleted';
         return old;
      end if;
    end if;

    if (TG_OP = 'DELETE') then
      return old;
    else
      return new;
    end if;

    END;
$$
LANGUAGE 'plpgsql';

drop trigger empl_tr_before on empl;
create trigger empl_tr_before before insert or update or delete
    on empl for each row
    execute procedure empl_tr_before();

