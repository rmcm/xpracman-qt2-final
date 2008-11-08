--  -*- sql -*-
--
 
--    $Id: mtat-functions.sql 523 2001-06-05 23:25:55Z source $

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
 
--     Rex McMaster, rmcm@ariel.ucs.unimelb.edu.au
 

 
-- Revision Log
  
-- ------------------------------------------------------------
-- Function to insert a new record with dummy mtat_code
-- ------------------------------------------------------------

drop function mtat_insert_row();
create function mtat_insert_row()
returns integer
as 'DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    tmp_mtat__sequence mtat.mtat__sequence%TYPE;

    BEGIN

         -- get a mtat__sequence
         select nextval(''mtat_mtat__sequence_seq'')
         into tmp_mtat__sequence;

         -- add the new row
         insert into mtat( mtat_name, mtat__sequence )
                   values( tmp_mtat__sequence::text, tmp_mtat__sequence);

    return tmp_mtat__sequence;
    END;'
  LANGUAGE 'plpgsql';
