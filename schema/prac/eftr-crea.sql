-- -*- sql -*-

 
--    $Id$

--  Copyright 2001 X=X Computer Software Trust
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
  
--  Initial version

create table eftr                       -- Bank Deposit Reports
(
 eftr_date_created timestamp default now(),              -- Date created
 eftr_first__sequence integer,                                -- first paym sequence number
 eftr_last__sequence integer,                                 -- last paym sequence number
 --
 -- standard headers
 --
 eftr__sequence serial not null,                              -- Row-ID
 eftr__timestamp timestamp default now() not null,        -- Timestamp
 eftr__user_entry text default CURRENT_USER not null,         -- Entered by
 eftr__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key (eftr__sequence)
 );

grant select,insert,update on eftr to public;
grant select,insert,update on eftr_eftr__sequence_seq to public;

select create_metadata('eftr');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('eftr','$Id:');
