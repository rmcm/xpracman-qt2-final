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



-- Patch file

-- Modify appointment types to handle parallel and skip methods
-- 1. Modify aptd to add new method attributes
-- NOTE: backup aptd first!!!!
select * into old_aptd from aptd;
select * into old_aptp from aptp;
drop table aptd;
\i schema/prac/aptd-crea.sql
\i schema/prac/aptd-addconstraints.sql
insert into aptd(
 aptd__status,
 aptd__timestamp,
 aptd__user_entry,
 aptd_aptp_code,
 aptd_colour,
 aptd_ending,
 aptd_locn_code,
 aptd_prov_code,
 aptd_recurrence,
 aptd_starting,
 aptd_weekofyear,
 aptd_year,
 aptd_desc,
 aptd_start_date,
 aptd_end_date,
 aptd_dayofweek,
 aptd_dayofmonth,
 aptd_monthofyear,
 aptd__sequence
) 
select 
 aptd__status,
 aptd__timestamp,
 aptd__user_entry,
 aptd_aptp_code,
 aptd_colour,
 aptd_ending,
 aptd_locn_code,
 aptd_prov_code,
 aptd_recurrence,
 aptd_starting,
 aptd_weekofyear,
 aptd_year,
 aptd_desc,
 aptd_start_date,
 aptd_end_date,
 aptd_dayofweek,
 aptd_dayofmonth,
 aptd_monthofyear,
 aptd__sequence
 
from old_aptd;

-- 2. rename aptp_repeat to aptp_disable  (blocking)
alter table aptp rename aptp_repeat to aptp_disable;
alter table aptp alter column aptp_duration set default 15;
alter table aptp alter column aptp_disable set default 0;
alter table aptp alter column aptp_colour set default '#00d3d3d3';

--update aptd set aptd_parallel = aptp.aptp_disable from aptp where aptp_code = aptd_aptp_code and aptp_disable >= 0;
update aptp set aptp_disable = 0 where aptp_disable > 0;
update aptp set aptp_disable = 1 where aptp_disable < 0;

update mtat set mtat_name = 'aptp_disable'  where mtat_name = 'aptp_repeat';
update mtat set mtat_title = 'Block', mtat_input_method = 'SELECT=0,No;1,Yes' where mtat_name = 'aptp_disable';
update mtat set mtat_input_method = 'SYSTEM' where mtat_name ~ '^aptp__';
update mtat set mtat_displen = '15' where mtat_name = 'aptp_desc';
update mtat set mtat_displen = '8' where mtat_name in ('aptp_code', 'aptp_colour', 'aptp_duration', 'aptp_disable');
update mtcl set mtcl_userlabel = '<b>%1 %2 (%3)</b><br><i>Address: %4 %5 %6</i><br><i>Ref: %7</i>, evnv_patn_fsnam,evnv_patn_psnam,evnv_patn_phone, evnv_patn_address,evnv_patn_suburb,evnv_patn_postcode, evnv_rfdr_name' where mtcl_name = 'evnv';
-- register the patch
\set mtpt_name '\'patch_aptp_01.sql\''
\set mtpt_rcs_header '\'$Id$\''

insert into mtpt(mtpt_name, mtpt_rcs_header) select :mtpt_name, :mtpt_rcs_header
        except select mtpt_name, mtpt_rcs_header from mtpt;

