-- -*-sql-*-
--      $Id: constraints.sql 2385 2005-01-16 01:39:21Z source $ 
--
--    Copyright 2000 X=X Computer Software Trust 
--                  Kangaroo Ground Australia 3097 
--
--
--   This is free software; you can redistribute it and/or modify 
--   it under the terms of the GNU General Public License published by 
--   the Free Software Foundation; either version 2, or (at your option) 
--   any later version. 
--
--   This software is distributed in the hope that it will be useful, 
--   but WITHOUT ANY WARRANTY; without even the implied warranty of 
--   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
--   GNU General Public License for more details. 
--
--   You should have received a copy of the GNU General Public License 
--   along with this software; see the file COPYING.  If not, write to 
--   the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
--
--   Report problems and direct all questions to: 
--
--       Rex McMaster, rmcm@compsoft.com.au 
--
-- General functions associated with any database
--
-- This script provides basic functionality for maintaining RI constraint triggers.
-- See also the shell scripts;
--
--  ri_check.sh:        checks the RI of constrained tables.
--  drop_constraint.sh: drops specified constraints
--
--  see also: dev_ri and mtsv views;
--
-- ------------------------------------------------------------
-- look for bad triggers -output "drop trigger" command
-- ------------------------------------------------------------

drop view bad_triggers;
create view bad_triggers as
select  tgname,
        tgconstrname,
        'drop trigger "'|| tgname ||'" on '||
        (case when tgtype = 21 then substr(tgconstrname,1,4)
        else substr(tgconstrname,length(tgconstrname)-3,4) end)||';' as drop
from    pg_trigger
where   tgname ~ '^RI_'
and     tgisconstraint is true
and     tgtype in (9,17,21)
and     tgconstrrelid = 0;  

-- ------------------------------------------------------------
-- output "drop trigger" command
-- ------------------------------------------------------------

drop view drop_trigger;
create view drop_trigger as
select  tgconstrname,
        'drop trigger "'|| tgname ||'" on '||
        ctrig.relname ||';' as drop
from    pg_trigger,
        pg_class ctrig,
        pg_class ccons
where   tgname ~ '^RI_'
and     tgisconstraint is true
and     tgtype in (9,17,21)
and     tgrelid = ctrig.oid 
and     tgconstrrelid = ccons.oid;

-- ------------------------------------------------------------
-- output count of RI triggers for each constraint.
-- ------------------------------------------------------------


drop view ri_count;
create view ri_count as
select  tgconstrname,
        count(*)
from    pg_trigger,
        pg_class ctrig
where   tgname ~ '^RI_'
and     tgisconstraint is true
and     tgtype in (9,17,21)
and     tgrelid = ctrig.oid 
group by tgconstrname;

-- ------------------------------------------------------------
-- output RI data-check commands.
-- ------------------------------------------------------------


drop view ri_check;
create view ri_check as
select  distinct mtsv_name as ri_name,
        'select distinct '|| quote_literal(mtsv_key) || '::text, ' || mtsv_key
        || ' from ' || mtsv_master_class
        || ' except select ' || quote_literal(mtsv_key) || '::text, ' || mtsv_join
        || ' from ' || mtsv_other_class || ';' as ri_sql
from    mtsv;
