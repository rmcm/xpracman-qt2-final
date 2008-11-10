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
  
create table pyas                        -- Payment assignments to services -- 4. Payments -- 10000
(
 pyas_paym__sequence integer not null,                                 -- Payment serial number
 pyas_svpf__sequence integer not null,                                 -- Service serial number
 pyas_amount numeric(12,2) default numeric(12,2) '0.00' not null,      -- Amount assigned
 --
 -- standard headers
 --
 pyas__sequence serial not null,                        -- Row-ID
 pyas__timestamp timestamp default now() not null,  -- Timestamp
 pyas__user_entry text default CURRENT_USER not null,   -- Entered by
 pyas__status char default text 'N' not null,           -- Record status
 --
 -- constraints
 --
 primary key(pyas__sequence)
 );

create index ix_pyas_svpf on pyas(pyas_svpf__sequence);
create index ix_pyas_paym on pyas(pyas_paym__sequence);
grant select,insert,update on pyas to public;
grant select,insert,update on pyas_pyas__sequence_seq to public;

select create_metadata('pyas');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('pyas','$Id:');
