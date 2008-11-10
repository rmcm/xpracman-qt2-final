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

drop view ssmc;
create view ssmc                -- Service Summary by service code
 as select 

 ssms_month                     as ssmc_month,
 ssms_serv_code                 as ssmc_serv_code,
 sum(ssms_amount)               as ssmc_amount,
 sum(ssms_gst_amount)           as ssmc_gst_amount,
 sum(ssms_total_amount)         as ssmc_total_amount,
 count(*)                       as ssmc_count,
 --
 -- standard headers
 --
 max(ssms__sequence)            as ssmc__sequence,              -- Row-ID
 now()                          as ssmc__timestamp,             -- Timestamp
 CURRENT_USER                   as ssmc__user_entry,            -- Entered by
 text 'N'                       as ssmc__status                 -- Record status
 --
 from ssms
 group by ssms_month,ssms_serv_code
;

grant select on ssmc to public;

select create_metadata('ssmc');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('ssmc','$Id:');
