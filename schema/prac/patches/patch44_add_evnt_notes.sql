--     $Id$
-- 
--   Copyright 2006 X=X Computer Software Trust
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


alter table evnt add column evnt_note_3 text;
alter table evnt add column evnt_note_4 text;
alter table evnt add column evnt_note_5 text;
alter table evnt add column evnt_note_6 text;
alter table evnt add column evnt_note_7 text;
alter table evnt add column evnt_note_8 text;
alter table evnt add column evnt_note_9 text;

select create_metadata('evnt');

-- reorder SYSTEM attributes to high space
update mtat set mtat_view_order = mtat_view_order + 1000
        where mtat_name ~ '^evnt__' and mtat_view_order < 1000;
update mtat set mtat_input_method = 'SYSTEM'
        where mtat_name ~ '^evnt__';
-- place new attributes at end of list, but before SYSTEM attributes.
update mtat set mtat_input_method = 'SYSTEM', mtat_view_order = 
        (select max(mtat_view_order) from mtat
                where mtat_class_name = 'evnt'
                and   mtat_name !~ '^evnt_note_[3-9]'
                and   mtat_name !~ '^evnt__')
        + to_number(substring(mtat_name from '[3-9]'),'9') * 10
        where mtat_name ~ '^evnt_note_[3-9]';

-- register the patch
\set mtpt_name '\'/patch44_add_evnt_notes.sql\''
\set mtpt_rcs_header '\'$Id:$\''

insert into mtpt(mtpt_name, mtpt_rcs_header) select :mtpt_name, :mtpt_rcs_header
        except select mtpt_name, mtpt_rcs_header from mtpt;


