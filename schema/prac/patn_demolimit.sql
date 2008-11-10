--     $Id$
-- 
--   Copyright 2006 X=X Computer Software Trust
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


create or replace function patn_demo_limit()
returns trigger
as $$
  DECLARE
    x_count integer;

    BEGIN

        -- get count
        select count(*)
        into    x_count
        from    patn;

        if (x_count > 100) then
          RAISE EXCEPTION 'Only 100 patients are allowed in a DEMONSTRATION DB';
        end if;

    return new;
    END;
$$
LANGUAGE 'plpgsql';


drop trigger patn_demo_limit on patn;
create trigger patn_demo_limit after insert
    on patn for each row
    execute procedure patn_demo_limit();

