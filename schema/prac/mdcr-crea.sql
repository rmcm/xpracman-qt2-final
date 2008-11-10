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

create table mdcr                       -- Medicare Credits
(
 mdcr_paym__sequence integer not null,                                 -- Payment serial number
 mdcr_svpf__sequence integer not null,                                 -- Service serial number
 mdcr_amount numeric(12,2) default numeric(12,2) '0.00' not null,      -- Amount credited
 --
 -- standard headers
 --
 mdcr__sequence serial not null,                              -- Row-ID 
 mdcr__timestamp timestamp default now() not null,        -- Timestamp
 mdcr__user_entry text default CURRENT_USER not null,         -- Entered by
 mdcr__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key (mdcr__sequence)
 );

grant select,insert,update on mdcr to public;
grant select,insert,update on mdcr_mdcr__sequence_seq to public;


select create_metadata('mdcr');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('mdcr','$Id:');
