--     $Id: a4toa5.sql 2848 2006-05-22 01:06:39Z source $
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


-- convert a form from a4 to a5

\set form_x_name '\'eventlist-evnv\''

update form set form_pagesize = 9 where form_code = :form_x_name;
update fmdt set
        fmdt_x_coord = (fmdt_x_coord/sqrt(2))::integer,
        fmdt_y_coord = (fmdt_y_coord/sqrt(2))::integer,
        fmdt_width   = (fmdt_width/sqrt(2))::integer,
        fmdt_height  = (fmdt_height/sqrt(2))::integer
where   fmdt_form_code = :form_x_name;

        
