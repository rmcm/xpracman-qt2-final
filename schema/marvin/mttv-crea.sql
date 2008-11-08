--     $Id: mttv-crea.sql 1658 2003-04-16 02:39:46Z source $
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

-- system view of tables and attributes - can be used for comparing table schemas

drop view mttv;
create view mttv
 as select
 pg_class.relname                       as mttv_class_name,
 pg_attribute.attnum                    as mttv_attribute_order,
 pg_attribute.attname                   as mttv_attribute_name,
 --
 -- standard headers
 --
 (pg_class.oid*100)+pg_attribute.attnum as mttv__sequence,              -- Row-ID
 'now'::timestamp                       as mttv__timestamp,             -- Timestamp
 CURRENT_USER                           as mttv__user_entry,            -- Entered by
 'V'::char(1)                           as mttv__status                 -- Record status
 --

      from pg_class, pg_attribute
     where pg_class.relname !~ '^pg_'
       and (pg_class.relkind = 'r' or pg_class.relkind = 'v')
       and pg_class.oid = pg_attribute.attrelid and pg_attribute.attnum > 0
  order by pg_class.relname, pg_attribute.attnum
;

grant select on mttv to public;

select create_metadata('mttv');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('mttv','$Id: mttv-crea.sql 1658 2003-04-16 02:39:46Z source $');
