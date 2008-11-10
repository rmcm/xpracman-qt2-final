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
drop function apst_tr_before();
create function apst_tr_before()
returns opaque
as 'DECLARE

    BEGIN
    -- set a dummy code
    if ( new.apst_code is null ) then
      new.apst_code = new.apst__sequence::text;
    end if;

    return new;

    END;'
    LANGUAGE 'plpgsql';

drop trigger apst_tr_before on apst;
create trigger apst_tr_before before insert or update
    on apst for each row
    execute procedure apst_tr_before();
