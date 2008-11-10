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
  
create table rfdr                       -- Referrers
(
 rfdr_code text not null,        -- Code
 rfdr_name text,                 -- Name
 rfdr_street text,               -- Street
 rfdr_street_1 text,             -- Street +1
 rfdr_street_2 text,             -- Street +2
 rfdr_suburb text,               -- Suburb
 rfdr_postcode text,             -- Postcode
 rfdr_state text,                -- State
 rfdr_provider text,             -- Provider number
 rfdr_phone text,                -- Phone
 rfdr_salutation text,           -- Salutation
 rfdr_index text,                -- Sort order
 rfdr_fax_number,                -- Fax Number
 rfdr_email_address,             -- Email Address

 --
 -- standard headers
 --
 rfdr__sequence serial not null,                              -- Row-ID
 rfdr__timestamp timestamp default now() not null,        -- Timestamp
 rfdr__user_entry text default CURRENT_USER not null,         -- Entered by
 rfdr__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(rfdr_code)
 );

create unique index ix_rfdr_code on rfdr(rfdr_code);
grant select,insert,update on rfdr to public;
grant select,insert,update on rfdr_rfdr__sequence_seq to public;

insert into rfdr(rfdr_code, rfdr_name) values('-', 'Default');

select create_metadata('rfdr');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('rfdr','$Id$');

