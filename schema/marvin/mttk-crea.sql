--     $Id: mttk-crea.sql 2427 2005-02-05 22:59:06Z source $
-- 
--   Copyright 2004 X=X Computer Software Trust
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



drop table mttk;
drop sequence mttk_mttk__sequence_seq;
create table mttk                       -- Ticket Management
(
 mttk_code text not null,                                       -- ticket name
 mttk_expires timestamp not null default now() + '60 seconds'::interval, -- ticket expires
 --
 -- standard headers
 --
 mttk__sequence serial not null,                                -- Row-ID
 mttk__timestamp timestamp default now() not null,              -- Timestamp
 mttk__user_entry text default CURRENT_USER not null,           -- Entered by
 mttk__status char default text 'N' not null,                   -- Record status
 --
 -- constraints
 --
 primary key(mttk_code)
 );

grant select,insert,update on mttk to public;
grant select,insert,update on mttk_mttk__sequence_seq to public;

select create_metadata('mttk');
update mtcl set mtcl_title = 'Ticket Management' where mtcl_name = 'mttk';

-- cleaning trigger
-- ------------------------------------------------------------
-- Function: Clean the ticket manager - remove expired tickets
-- ------------------------------------------------------------

drop trigger tr_vacuumtickets on mttk;
drop function fn_vacuumtickets();
create function fn_vacuumtickets()
returns opaque
as 'DECLARE

    BEGIN

    if ( TG_OP = ''INSERT'' ) then
      delete from mttk where mttk_expires < now();
    end if;

    return new;
    END;'
  LANGUAGE 'plpgsql';


create trigger tr_vacuumtickets before insert
    on mttk for each row
    execute procedure fn_vacuumtickets();


-- update history;
\set mttb_name '\'mttk\''
\set mttb_rcs_header '\'$Id: mttk-crea.sql 2427 2005-02-05 22:59:06Z source $\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

