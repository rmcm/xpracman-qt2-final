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
  
create table tdtp
(
 tdtp_code text not null,                                       -- Tender-type code
 tdtp_list text default text 'Cheques' not null,                -- listing category
 tdtp_subtotal text default text 'Cheques and Cash' not null,   -- subtotalcategory
 tdtp_desc text,                                                -- Tender type description - eg drawer
 tdtp_entity text,                                              -- eg bank
 tdtp_location text,                                            -- eg branch location
 --
 -- standard headers
 --
 tdtp__sequence serial not null,                              -- Row-ID
 tdtp__timestamp timestamp default now() not null,        -- Timestamp
 tdtp__user_entry text default CURRENT_USER not null,         -- Entered by
 tdtp__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(tdtp_code)
 );
 
 grant select,insert,update on tdtp to public;
 grant select,insert,update on tdtp_tdtp__sequence_seq to public;

insert into tdtp(tdtp_code) values('-');

select create_metadata('tdtp');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('tdtp','$Id:');
