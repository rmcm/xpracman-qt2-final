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

drop view agdi;
create view agdi                -- Aged debt view
 as select 
 invc_dbtr_code                                                    as agdi_dbtr_code,
 dbtr_name                                                         as agdi_dbtr_name,
 invc_patient_name(invc__sequence)                                 as agdi_patient,
 invc_feet_code                                                    as agdi_feet_code,
 invc__sequence                                                    as agdi_invc__sequence,
 invc_amount+invc_gst_amount-invc_paid_amount-invc_paid_gst_amount as agdi_amount,
 invc_age_period(invc__sequence, 0)                                as agdi_period,
 (case when invc_date_printed is null then
    NULL::date
  else date(invc_date_printed) end)                                as agdi_date_printed,
 (case when invc_date_reprint is null then
    NULL::date
  else date(invc_date_reprint) end)                                as agdi_date_reprint,
 --
 -- standard headers
 --
 invc__sequence                                                    as agdi__sequence,   -- Row-ID
 invc__timestamp                                                   as agdi__timestamp,  -- Timestamp
 invc__user_entry                                                  as agdi__user_entry, -- Entered by
 invc__status                                                      as agdi__status      -- Record status
 --
 from  invc,dbtr
 where invc_amount+invc_gst_amount-invc_paid_amount-invc_paid_gst_amount!=0.00::numeric(12,2)
 and   dbtr_code = invc_dbtr_code
 and   invc_date_printed is not null
;

grant select on agdi to public;

select create_metadata('agdi');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('agdi','$Id:');
