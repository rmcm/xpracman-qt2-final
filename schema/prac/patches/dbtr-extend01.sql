--    $Id$
--
--  Copyright 2001 X=X Computer Software Trust
--                  Kangaroo Ground Australia 3097
--
--
-- This is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License published by
-- the Free Software Foundation; either version 2, or (at your option)
-- any later version.
-- 
-- This software is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
-- 
-- You should have received a copy of the GNU General Public License
-- along with this software; see the file COPYING.  If not, write to
-- the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
-- 
-- Report problems and direct all questions to:
-- 
--     Rex McMaster, rmcm@compsoft.com.au
-- 
--

-- add 3 extra fields - see dbtr-crea.sql v1.6->1.7

-- add columns
alter table dbtr add column dbtr_group text;
alter table dbtr add column dbtr_last_statement timestamp;
alter table dbtr add column dbtr_delay_statement interval;

-- set defaults
alter table dbtr alter column dbtr_delay_statement set default '1 month';

-- add metadata
select create_metadata('dbtr');
-- update new values
update dbtr set dbtr_delay_statement = '1 month'::interval;
update dbtr set dbtr_last_statement = coalesce(maxinvc.lastdate, maxinvc.firstdate)
  from (select invc_dbtr_code,
               max(invc.invc_date_reprint) as lastdate,
               max(invc.invc_date_printed) as firstdate
          from invc
          group by invc_dbtr_code) as maxinvc
 where maxinvc.invc_dbtr_code = dbtr.dbtr_code;
