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

drop view agdt;
create view agdt                -- Aged debt view
 as select 

 agdi_feet_code                 as agdt_feet_code,
 sum(agdi_amount)               as agdt_amount,
 count(*)                       as agdt_count,
 --
 -- standard headers
 --
 max(agdi__sequence)            as agdt__sequence,              -- Row-ID
 now()                          as agdt__timestamp,             -- Timestamp
 CURRENT_USER                   as agdt__user_entry,            -- Entered by
 text 'N'                       as agdt__status                 -- Record status
 --
 from agdi
 group by agdi_feet_code
;

grant select on agdt to public;

select create_metadata('agdt');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('agdt','$Id:');
