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

drop view agdp;
create view agdp                -- Aged debt view
 as select 

 agdi_period                    as agdp_period,
 sum(agdi_amount)               as agdp_amount,
 count(*)                       as agdp_count,
 --
 -- standard headers
 --
 max(agdi__sequence)            as agdp__sequence,              -- Row-ID
 now()                          as agdp__timestamp,             -- Timestamp
 CURRENT_USER                   as agdp__user_entry,            -- Entered by
 text 'N'                       as agdp__status                 -- Record status
 --
 from agdi
 group by agdi_period
;

grant select on agdp to public;

select create_metadata('agdp');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('agdp','$Id:');
