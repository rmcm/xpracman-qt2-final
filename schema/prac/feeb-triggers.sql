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
-- Trigger to add feeb rows to match new feet rows
--

drop trigger tr_before_feeb on feeb;
drop trigger feeb_tr_after_feet on feet;
drop trigger feeb_tr_after_serv on serv;
-- 
-- drop function feeb_tr_after() CASCADE;
-- drop function tr_before_feeb() CASCADE;

create or replace function feeb_tr_after()
returns trigger as $$
    DECLARE

    BEGIN
    -- add new feeb rows

    insert into feeb(feeb_serv_code, feeb_feet_code)
    select distinct serv_code, feet_code
    from serv, feet
    except select feeb_serv_code, feeb_feet_code from feeb;

    return new;

    END;$$
    LANGUAGE 'plpgsql';

create or replace function tr_before_feeb()
returns trigger as $$
    DECLARE

    BEGIN

    return new;

    END; $$
    LANGUAGE 'plpgsql';

create trigger feeb_tr_after_feet after insert
    on feet for each row
    execute procedure feeb_tr_after();
create trigger feeb_tr_after_serv after insert
    on serv for each row
    execute procedure feeb_tr_after();
create trigger tr_before_feeb before insert or update
    on feeb for each row
    execute procedure tr_before_feeb();

-- update history;
\set mttb_name '\'feeb\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

