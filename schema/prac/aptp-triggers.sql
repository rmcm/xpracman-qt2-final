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
create or replace function aptp_tr_before()
returns trigger
as 'DECLARE

    BEGIN
    -- set a dummy code
    if ( TG_OP = ''INSERT'' or TG_OP = ''UPDATE'') then
      if ( new.aptp_code is null ) then
        new.aptp_code = new.aptp__sequence::text;
      end if;
    end if;

    -- prevent changing ''-''
    if ( TG_OP = ''UPDATE'' ) then
      if ( old.aptp_code = ''-'' ) then
        RAISE EXCEPTION ''You cannot change this code'';
        return old;
      end if;
    end if;

    -- prevent deletion of ''-''
    if ( TG_OP = ''DELETE'' ) then
      if ( old.aptp_code = ''-'' ) then
        RAISE EXCEPTION ''You cannot remove this code'';
        return old;
      end if;
    end if;

    return new;

    END;'
    LANGUAGE 'plpgsql';

drop trigger aptp_tr_before on aptp;
create trigger aptp_tr_before before insert or update or delete
    on aptp for each row
    execute procedure aptp_tr_before();


-- update history;
\set mttb_name '\'aptp\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
