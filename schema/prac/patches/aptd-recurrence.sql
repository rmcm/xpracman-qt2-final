--     $Id$
-- 
--   Copyright 2003 X=X Computer Software Trust
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


-- Patch file

-- add recurrence attributes
alter table aptd add column aptd_start_date date DEFAULT null;
alter table aptd add column aptd_end_date date DEFAULT null;
alter table aptd add column aptd_recurrence text DEFAULT null;

select create_metadata('aptd');

alter table aptd alter column aptd_dayofweek set default null;
alter table aptd alter column aptd_weekofyear set default null;
alter table aptd alter column aptd_dayofmonth set default null;
alter table aptd alter column aptd_monthofyear set default null;
alter table aptd alter column aptd_year set default null;

update aptd set aptd_dayofmonth = null where aptd_dayofmonth = 0;
update aptd set aptd_monthofyear = null where aptd_monthofyear = 0;
update aptd set aptd_year = null where aptd_year = 0;

-- allow editing of aptd_aptp_code to enable copy and move.

update mtat set mtat_input_method = '' where mtat_name = 'aptd_aptp_code';

-- register the patch
insert into mtpt(mtpt_name, mtpt_rcs_header) values('aptd-recurrence.sql', '$Id$');
