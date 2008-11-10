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
  
create table prov                       -- Service Providers
(
 prov_code text not null,                       -- Provider Code
 prov_provider_num text,                        -- Provider number
 prov_name text,                                -- Name
 prov_address text,                             -- Street
 prov_suburb text,                              -- Suburb 
 prov_state text,                               -- State
 prov_postcode text,                            -- Postcode
 prov_salutation text,                          -- Salutation
 prov_phone text,                               -- Phone
 prov_bank_code text default text '-' not null, -- Banking info
 prov_colour text default '#0000FF',            -- colour attribute
 prov_fax_number text                            -- Fax Number
 prov_email_address text,                        -- Email Address
 --
 -- standard headers
 --
 prov__sequence serial not null,                              -- Row-ID
 prov__timestamp timestamp default now() not null,        -- Timestamp
 prov__user_entry text default CURRENT_USER not null,         -- Entered by
 prov__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(prov_code)
 );

grant select,insert,update on prov to public;
grant select,insert,update on prov_prov__sequence_seq to public;

insert into prov(prov_code) values('-');

select create_metadata('prov');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('prov','$Id$');



