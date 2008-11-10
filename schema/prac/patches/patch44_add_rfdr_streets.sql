--     $Id$
-- 
--   Copyright 2007 X=X Computer Software Trust
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


-- add street extentions

alter table rfdr add column rfdr_street_1 text;
alter table rfdr add column rfdr_street_2 text;

select create_metadata('rfdr');

-- place new attributes after existing street
update mtat set mtat_view_order = 
        (select mtat_view_order from mtat
                where mtat_name = 'rfdr_street')
        + to_number(substring(mtat_name from '[12]'),'9')
        where mtat_name ~ '^rfdr_street_[12]';

-- register the patch
\set mtpt_name '\'/patch44_add_rfdr_streets.sql\''
\set mtpt_rcs_header '\'$Id:$\''

insert into mtpt(mtpt_name, mtpt_rcs_header) select :mtpt_name, :mtpt_rcs_header
        except select mtpt_name, mtpt_rcs_header from mtpt;

