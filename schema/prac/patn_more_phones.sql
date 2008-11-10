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

alter table patn add column patn_phone_1 text;
alter table patn add column patn_phone_2 text;

delete from mtat where mtat_name = 'patn_phone_1';
delete from mtat where mtat_name = 'patn_phone_2';
insert into mtat(mtat_name, mtat_type, mtat_title, mtat_class_name, mtat_displen, mtat_browse_order, mtat_view_order, mtat_input_method)
        values('patn_phone_1', 'text', 'Phone-work', 'patn', 30, 112, 112, '10');
insert into mtat(mtat_name, mtat_type, mtat_title, mtat_class_name, mtat_displen, mtat_browse_order, mtat_view_order, mtat_input_method)
        values('patn_phone_2', 'text', 'Phone-mobile', 'patn', 30, 114, 114, '10');

-- update evnv
\echo Updating EVNV View
drop view evnv_nosurgical;
drop view evnv_surgical;
drop view evnv;
drop view patv;
\i schema/prac/evnv-functions.sql
\i schema/prac/evnv-crea.sql
\i schema/prac/evnv_surgical_views.sql
\i schema/prac/patv-crea.sql
\i schema/prac/patv-meta.sql

-- update history;
\set mttb_name '\'patn\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;


