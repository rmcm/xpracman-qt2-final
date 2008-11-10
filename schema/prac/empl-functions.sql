--  -*- sql -*-
--
 
--    $Id$

--  Copyright 2001 X=X Computer Software Trust
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
  
-- ------------------------------------------------------------
-- Function to insert a new record with dummy empl_code
-- ------------------------------------------------------------

drop function empl_insert_row();
create function empl_insert_row()
returns integer
as 'DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    tmp_empl__sequence empl.empl__sequence%TYPE;

    BEGIN

         -- get a empl__sequence
         select nextval(''empl_empl__sequence_seq'')
         into tmp_empl__sequence;

         -- add the new row
         insert into empl( empl_code, empl__sequence )
                   values( tmp_empl__sequence::text, tmp_empl__sequence);

    return tmp_empl__sequence;
    END;'
  LANGUAGE 'plpgsql';
