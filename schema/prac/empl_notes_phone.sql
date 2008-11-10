--     $Id$
-- 
--   Copyright 2005 X=X Computer Software Trust
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


-- add an attribute - service annotation

alter table empl add column empl_phone text;
alter table empl add column empl_notes text;

delete from mtat where mtat_name = 'empl_phone';
delete from mtat where mtat_name = 'empl_notes';
insert into mtat(mtat_name, mtat_type, mtat_title, mtat_class_name, mtat_displen, mtat_browse_order, mtat_view_order, mtat_input_method)
        values('empl_phone', 'text', 'Phone', 'empl', 30, 70, 70, '10');
insert into mtat(mtat_name, mtat_type, mtat_title, mtat_class_name, mtat_displen, mtat_browse_order, mtat_view_order, mtat_input_method)
        values('empl_notes', 'text', 'Annotation', 'empl', 30, 80, 80, '10');

-- update history;
\set mttb_name '\'empl\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;


