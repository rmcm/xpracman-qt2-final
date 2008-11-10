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

create table locn                       -- patient flags
(
 locn_code text not null,               -- Code
 locn_desc text,                        -- Description
 locn_id text,                          -- HIC location ID
 locn_address text,                     -- address
 locn_address_2 text,                   -- address 2nd line
 locn_suburb text,                      -- suburb
 locn_postcode text,                    -- postcode
 locn_state text,                       -- state
 locn_colour text default '#0000FF',    -- colour attribute
 locn_fax_number text                   -- Fax Number
 locn_email_address text,               -- Email Address
 --
 -- standard headers
 --
 locn__sequence serial not null,                              -- Row-ID
 locn__timestamp timestamp default now() not null,        -- Timestamp
 locn__user_entry text default CURRENT_USER not null,         -- Entered by
 locn__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(locn_code)
 );

create unique index ix_locn_id on locn(locn_id);
grant select,insert,update on locn to public;
grant select,insert,update on locn_locn__sequence_seq to public;
insert into locn(locn_code, locn_desc,locn_colour) values('-', 'Default','#FF0000');

select create_metadata('locn');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('locn','$Id$');

