 --  -*- sql -*-
--
--    $Id$

--  Copyright 2000 X=X Computer Software Trust
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
-- 

create table conf                                   -- Configuration
(
 conf_code text default text '-' not null,          -- Attribute Name
 conf_desc text,                                    -- Description
 conf_value text,                                   -- Value
 conf_type integer default 0 not null,              -- Type
 conf_scope integer,                                -- Scope
 conf_access integer,                               -- Access
 --
 -- standard headers
 --
 conf__sequence serial not null,                             -- Row-ID
 conf__timestamp timestamp default now() not null,       -- Timestamp
 conf__user_entry text default CURRENT_USER not null,        -- Entered by
 conf__status char default text 'N' not null,                -- Record status
 --
 -- constraints
 --
 primary key (conf_code)
 );



grant select,insert,update on conf to public;
grant select,insert,update on conf_conf__sequence_seq to public;

insert into conf(conf_code) values('-');


select create_metadata('conf');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('conf','$Id:');
