--  -*- sql -*-
--
 
--    $Id: mtcl-functions.sql 523 2001-06-05 23:25:55Z source $

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
-- Function to insert a new record with dummy mtcl_code
-- ------------------------------------------------------------

drop function mtcl_insert_row();
create function mtcl_insert_row()
returns integer
as 'DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    tmp_mtcl__sequence mtcl.mtcl__sequence%TYPE;

    BEGIN

         -- get a mtcl__sequence
         select nextval(''mtcl_mtcl__sequence_seq'')
         into tmp_mtcl__sequence;

         -- add the new row
         insert into mtcl( mtcl_name, mtcl__sequence )
                   values( tmp_mtcl__sequence::text, tmp_mtcl__sequence);

    return tmp_mtcl__sequence;
    END;'
  LANGUAGE 'plpgsql';
