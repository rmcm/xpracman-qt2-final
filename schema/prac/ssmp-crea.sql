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

drop view ssmp;
create view ssmp                -- Service Summary by service code
 as select 

 ssms_month                     as ssmp_month,
 sum(ssms_amount)               as ssmp_amount,
 sum(ssms_gst_amount)           as ssmp_gst_amount,
 sum(ssms_total_amount)         as ssmp_total_amount,
 count(*)                       as ssmp_count,
 --
 -- standard headers
 --
 max(ssms__sequence)            as ssmp__sequence,              -- Row-ID
 now()                          as ssmp__timestamp,             -- Timestamp
 CURRENT_USER                   as ssmp__user_entry,            -- Entered by
 text 'N'                       as ssmp__status                 -- Record status
 --
 from ssms
 group by ssms_month
;

grant select on ssmp to public;

select create_metadata('ssmp');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('ssmp','$Id:');
