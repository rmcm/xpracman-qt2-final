--  -*- sql -*-
-- 
--    $Id$

--  Copyright 1996 X=X Computer Software Trust
--   		  Kangaroo Ground Australia 3097


-- This is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public Licensepublished by
-- the Free Software Foundation; either version 2, or (at your option)
-- any later version.
 
-- This software is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
 
-- You should have received a copy of the GNU General Public License
-- along with this software; see the file COPYING.  If not, write to
-- the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
-- Report problems and direct all questions to:
 
--     Rex McMaster, rmcm@compsoft.com.au
 

 
-- Revision Log

create table aptd                       -- appointment types - details
(
 aptd_aptp_code text not null,                  -- Code
 aptd_desc text DEFAULT 'New Appointment Detail', -- Description
 aptd_colour text DEFAULT '#00ffffff',          -- colour attribute
 aptd_prov_code text DEFAULT '-',               -- provider
 aptd_locn_code text DEFAULT '-',               -- location
 aptd_locn_wildcard text,                       -- location expansion(null=*)
 aptd_dayofweek integer DEFAULT 0
        CHECK(aptd_dayofweek between 0 and 7 or aptd_dayofweek is null),  -- day of week
 aptd_weekofyear text DEFAULT null,               -- week of year 
 aptd_dayofmonth integer DEFAULT null
        CHECK(aptd_dayofmonth between 0 and 31 or aptd_dayofmonth is null), -- day of month
 aptd_monthofyear integer DEFAULT null
        CHECK(aptd_monthofyear between 0 and 12 or aptd_monthofyear is null),  -- month of year
 aptd_year integer DEFAULT null
        CHECK(aptd_year between 2002 and 2020 or aptd_year is null), -- year
 aptd_parallel integer DEFAULT 0
        CHECK(aptd_parallel between 0 and 5),    -- parallel booking 0=no, 1=double 2=triple ...
 aptd_skip integer DEFAULT 0
        CHECK(aptd_skip between 0 and 20),       -- skip booking 0=no, 1=alternate ...
 aptd_start_date date DEFAULT null,              -- recurrence : starting
 aptd_end_date date DEFAULT null,                --            : ending
 aptd_recurrence text DEFAULT null,             --            : interval
 aptd_starting time DEFAULT null,               -- starting time
 aptd_ending time DEFAULT null,                 -- ending time
 --
 -- standard headers
 --
 aptd__sequence serial not null,                              -- Row-ID
 aptd__timestamp timestamp default now() not null,        -- Timestamp
 aptd__user_entry text default CURRENT_USER not null,         -- Entered by
 aptd__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(aptd__sequence)
 );

grant select,insert,update on aptd to public;
grant select,insert,update on aptd_aptd__sequence_seq to public;

select create_metadata('aptd');
update mtcl set mtcl_title = 'Type Details', mtcl_order_by = 'order by aptd__timestamp desc' where mtcl_name = 'aptd';
--update mtat set mtat_title = '' where mtat_name = 'aptd_';
update mtat set mtat_displen = 5 where mtat_name ~ '^aptd_[a-z]*';
update mtat set mtat_displen = 15 where mtat_name = 'aptd_desc';
update mtat set mtat_displen = 10 where mtat_name in ('aptd_start_date','aptd_end_date');
update mtat set mtat_displen = 3 where mtat_name in ('aptd_dayofweek','aptd_dayofmonth','aptd_monthofyear','');
update mtat set mtat_input_method = 'SYSTEM' where mtat_name ~ '^aptd__';
update mtat set mtat_input_method = 'RO' where mtat_name = 'aptd_aptp_code';
update mtat set mtat_input_method = 'COLOUR' where mtat_name = 'aptd_colour';
update mtat set mtat_input_method = '10' where mtat_name = 'aptd_dayofmonth';
update mtat set mtat_input_method = 'SELECT= 0,Sunday;1,Monday;2,Tuesday;3,Wednesday;4,Thursday;5,Friday;6,Saturday;' where mtat_name = 'aptd_dayofweek';
update mtat set mtat_input_method = '10' where mtat_name = 'aptd_desc';
update mtat set mtat_input_method = 'SELECT=0,No;1,Yes' where mtat_name = 'aptd_disable';
update mtat set mtat_input_method = '10' where mtat_name = 'aptd_ending';
update mtat set mtat_input_method = 'FSL=30' where mtat_name = 'aptd_locn_code';
update mtat set mtat_input_method = 'SELECT=1,January;2,February;3,March;4,April;5,May;6,June;7,July;8,August;9,September;10,October;11,November;12,December;' where mtat_name = 'aptd_monthofyear';
update mtat set mtat_input_method = '10', mtat_title = 'Double'  where mtat_name = 'aptd_parallel';
update mtat set mtat_input_method = 'FSL=30' where mtat_name = 'aptd_prov_code';
update mtat set mtat_input_method = '10' where mtat_name = 'aptd_skip';
update mtat set mtat_input_method = '10' where mtat_name = 'aptd_start_date';
update mtat set mtat_input_method = '10' where mtat_name = 'aptd_end_date';
update mtat set mtat_input_method = '10' where mtat_name = 'aptd_recurrence';
update mtat set mtat_input_method = '10' where mtat_name = 'aptd_starting';
update mtat set mtat_input_method = 'SELECT=1,Odd Weeks;0,Even Weeks;' where mtat_name = 'aptd_weekofyear';
update mtat set mtat_input_method = 'SELECT=2002,2002;2003,2003;2004,2004;2005,2005;2006,2006;2007,2007;2008,2008;2009,2009;2010,2010;' where mtat_name = 'aptd_year';

update mtat set mtat_input_method = 'SYSTEM' where mtat_name = 'aptd_colour';

-- update history;
\set mttb_name '\'aptd\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

