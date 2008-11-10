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

create table feet                       -- Fee Levels
(
 feet_code text not null,                 -- Code
 feet_desc text,                          -- Description
 --
 -- standard headers
 --
 feet__sequence serial not null,                              -- Row-ID
 feet__timestamp timestamp default now() not null,        -- Timestamp
 feet__user_entry text default CURRENT_USER not null,         -- Entered by
 feet__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(feet_code)
 );

grant select,insert,update on feet to public;
grant select,insert,update on feet_feet__sequence_seq to public;

insert into feet(feet_code) values('-');


select create_metadata('feet');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('feet','$Id:');
