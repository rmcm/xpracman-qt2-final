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
 

-- 
-- Create a function to insert via view
-- This function returns the serial_number of the new "virtual" row.
-- This is used instead of an "insert" rule, as the latter cannot use "lastoid"
-- to revover the inserted row.
-- 
drop function cnrv_insert_row();
create function cnrv_insert_row()
returns integer
as '
    DECLARE
                rec_cnrv cnrv%ROWTYPE;
                rec_patn patn%ROWTYPE;
                tmp_cnrt__sequence cnrt.cnrt__sequence%TYPE; 
                
    BEGIN

         -- Do not know invoice yet, so use 0 
         if ( rec_cnrv.cnrv_patn__sequence is null ) then
           -- new invoice = 0 to trigger
           rec_cnrv.cnrv_patn__sequence := 0;
            select  *
            into    rec_patn
            from    patn
            where   patn__sequence = rec_cnrv.cnrv_patn__sequence;
            -- if not found create a dummy to temporarily satisfy constraints
            if ( not found ) then
              insert into patn(patn__sequence) values(0);
            end if;
         end if;

         -- get a cnrt__sequence
         select nextval(''cnrt_cnrt__sequence_seq'')
         into tmp_cnrt__sequence;
         -- add the cnrt record 
         insert into cnrt(cnrt__sequence)
          values(tmp_cnrt__sequence);

    return tmp_cnrt__sequence;
    END;'
    LANGUAGE 'plpgsql';
