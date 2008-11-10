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

create table empl                           -- Employers
(
 empl_code text not null,                   -- Code
 empl_name text,                            -- Name
 empl_address text,                         -- Street
 empl_suburb text,                          -- Suburb
 empl_postcode text,                        -- Postcode
 empl_state text,                           -- State
 empl_phone text,                           -- phone
 empl_notes text,                           -- notes
 empl_fax_number text                       -- Fax Number
 empl_email_address text,                   -- Email Address
 --
 -- standard headers
 --
 empl__sequence serial not null,                              -- Row-ID
 empl__timestamp timestamp default now() not null,        -- Timestamp
 empl__user_entry text default CURRENT_USER not null,         -- Entered by
 empl__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key (empl_code)
 );

grant select,insert,update on empl to public;
grant select,insert,update on empl_empl__sequence_seq to public;

insert into empl(empl_code) values('-');


select create_metadata('empl');

-- update history;
\set mttb_name '\'empl\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
