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
drop function notv_insert_row();
create function notv_insert_row()
returns integer
as '
    DECLARE
         rec_notv notv%ROWTYPE;
         tmp_note__sequence note.note__sequence%TYPE; 
         rec_patn patn%ROWTYPE;
                
    BEGIN

         -- Do not know patient yet, so use 0 
         if ( rec_notv.notv_patn__sequence is null ) then
           -- new invoice = 0 to trigger
           rec_notv.notv_patn__sequence := 0;
            select  *
            into    rec_patn
            from    patn
            where   patn__sequence = rec_notv.notv_patn__sequence;
            -- if not found create a dummy to temporarily satisfy constraints
            if ( not found ) then
              insert into patn(patn__sequence) values(0);
            end if;
         end if;

         -- get a note__sequence
         select nextval(''note_note__sequence_seq'')
         into tmp_note__sequence;
         -- add the note record 
         insert into note(
                     note_patn__sequence,
                     note__sequence)
               values(
                     rec_notv.notv_patn__sequence,
                     tmp_note__sequence);

    return tmp_note__sequence;
    END;'
    LANGUAGE 'plpgsql';


-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('notv','$Id$');

