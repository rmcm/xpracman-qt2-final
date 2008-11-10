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

create table aptp                       -- appointment types
(
 aptp_code text not null,               -- Code
 aptp_desc text DEFAULT 'New Appointment Type', -- Description
 aptp_colour text DEFAULT '#00d3d3d3',    -- colour attribute
 aptp_duration integer DEFAULT 15 
        CHECK(aptp_duration between 10 and 60), -- length (minutes)
 aptp_disable integer DEFAULT 0,         -- disable bookings.
 --
 -- standard headers
 --
 aptp__sequence serial not null,                              -- Row-ID
 aptp__timestamp timestamp default now() not null,        -- Timestamp
 aptp__user_entry text default CURRENT_USER not null,         -- Entered by
 aptp__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(aptp_code)
 );

grant select,insert,update on aptp to public;
grant select,insert,update on aptp_aptp__sequence_seq to public;
insert into aptp(aptp_code, aptp_desc, aptp_colour) values('-', 'Default', '#00d3d3d3');

select create_metadata('aptp');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('aptp','$Id$');

