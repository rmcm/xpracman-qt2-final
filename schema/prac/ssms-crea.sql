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

drop view ssms;
create view ssms                -- Aged debt view
 as select 

 svpf_serv_code                         as ssms_serv_code,
 svpf_amount+svpf_gst_amount            as ssms_total_amount,
 svpf_amount                            as ssms_amount,
 svpf_gst_amount                        as ssms_gst_amount,
 to_char(svpf_date_service,'YYYY-MM')   as ssms_month,
 svpf_date_service                      as ssms_date_service,
 --
 -- standard headers
 --
 svpf__sequence                         as ssms__sequence,              -- Row-ID
 now()                                  as ssms__timestamp,             -- Timestamp
 CURRENT_USER                           as ssms__user_entry,            -- Entered by
 text 'N'                               as ssms__status                 -- Record status
 --
 from svpf
-- where svpf_amount+svpf_gst_amount!=0.00::numeric(12,2)
;

grant select on ssms to public;

select create_metadata('ssms');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('ssms','$Id:');
