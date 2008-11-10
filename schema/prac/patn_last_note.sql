--     $Id$
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

-- Create a virtual attribute for last-note date
create or replace function last_patn_note( integer )
returns text as $$
    DECLARE
        a_patn__sequence alias for $1;
        x_last_note_date timestamp;
        x_note_count integer;

    BEGIN
        select  max(note__timestamp),count(*)
        into    x_last_note_date, x_note_count
        from    note
        where   note_patn__sequence = a_patn__sequence;

        if (x_last_note_date is null) then
          return '';
        else
          return to_char(x_last_note_date, 'DD-MM-YYYY HH:MI AM') || ' (' || x_note_count || ')';
        end if;
    END;
$$
LANGUAGE 'plpgsql';

-- add patn_last_note
delete from mtal where mtal_mtat_name = 'patn_last_note';
delete from mtat where mtat_name = 'patn_last_note';
insert into mtat(mtat_name, mtat_type, mtat_title, mtat_class_name, mtat_displen, mtat_browse_order, mtat_view_order, mtat_input_method)
        values('patn_last_note', 'text', 'Last Note', 'patn', 20, 370, 370, 'VIRTUAL=last_patn_note(patn__sequence)');
-- alert
insert into mtal(mtal_mtat_name,mtal_mtop_code,mtal_value,mtal_colour, mtal_priority)
        values('patn_last_note',8,'','#FF0000', 1);


-- update history;
\set mttb_name '\'patn\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
