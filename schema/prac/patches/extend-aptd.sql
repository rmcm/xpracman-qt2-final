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

alter table aptd add column aptd_locn_wildcard text;

select create_metadata('aptd');

update mtat set mtat_input_method = 10, mtat_title = 'Location Wildcard', 
                mtat_displen = 10, mtat_view_order = (select 1+mtat_view_order from mtat where mtat_name = 'aptd_locn_code')
                where mtat_name = 'aptd_locn_wildcard';


-- register the patch
\set mtpt_name '\'extend-aptd.sql\''
\set mtpt_rcs_header '\'$Id$\''

insert into mtpt(mtpt_name, mtpt_rcs_header) select :mtpt_name, :mtpt_rcs_header
        except select mtpt_name, mtpt_rcs_header from mtpt;
