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

drop view pmsp;
create view pmsp                -- Service Summary by service code
 as select 

 pmsc_month                                     as pmsp_month,
 sum(pmsc_amount)                               as pmsp_amount,
 sum(pmsc_gst_amount)                           as pmsp_gst_amount,
 sum(pmsc_total_amount)                         as pmsp_total_amount,
 count(*)                                       as pmsp_count,
 --
 -- standard headers
 --
 max(pmsc__sequence)                            as pmsp__sequence,              -- Row-ID
 now()                                          as pmsp__timestamp,             -- Timestamp
 CURRENT_USER                                   as pmsp__user_entry,            -- Entered by
 text 'N'                                       as pmsp__status                 -- Record status
 --
 from pmsc
 group by pmsc_month
;

grant select on pmsp to public;

select create_metadata('pmsp');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('pmsp','$Id:');
