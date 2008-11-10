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

drop view gsti;
create view gsti                       -- GST Invoicing View
 as select
 --
 -- invc
 --
 invc__sequence                           as gsti_invc__sequence,
 short_date(invc_date_printed)            as gsti_day_date,
 quarter_date(invc_date_printed)          as gsti_quarter_date,
 invc_dbtr_code                           as gsti_invc_dbtr_code,
 invc_bank_code                           as gsti_invc_bank_code,
 invc_prov_code                           as gsti_invc_prov_code,
 invc_patn__sequence                      as gsti_invc_patn__sequence,
 invc_empl_code                           as gsti_invc_empl_code,
 invc_feet_code                           as gsti_invc_feet_code,
 invc_rfdr_code                           as gsti_invc_rfdr_code,
 invc_rfdr_date                           as gsti_invc_rfdr_date,
 invc_rfdr_period                         as gsti_invc_rfdr_period,
 invc_date_created                        as gsti_invc_date_created,
 invc_date_printed                        as gsti_invc_date_printed,
 invc_date_reprint                        as gsti_invc_date_reprint,
 invc_amount                              as gsti_invc_amount,
 invc_paid_amount                         as gsti_invc_paid_amount,
 invc_gst_amount                          as gsti_invc_gst_amount,
 invc_paid_gst_amount                     as gsti_invc_paid_gst_amount,
 --
 -- patn
 --
 patn_flno                                as gsti_patn_flno,
 patn_psnam                               as gsti_patn_psnam,
 patn_fsnam                               as gsti_patn_fsnam,
 patn_title                               as gsti_patn_title,
 short_date(patn_dob)                     as gsti_patn_dob,
 patn_address                             as gsti_patn_address,
 patn_suburb                              as gsti_patn_suburb,
 patn_state                               as gsti_patn_state,
 patn_postcode                            as gsti_patn_postcode,
 patn_phone                               as gsti_patn_phone,
 --
 -- Debtor
 --
 patn_dbtr_code                           as gsti_dbtr_code,
 dbtr_name                                as gsti_dbtr_name,
 dbtr_address                             as gsti_dbtr_address,
 dbtr_suburb                              as gsti_dbtr_suburb,
 dbtr_postcode                            as gsti_dbtr_postcode,
 dbtr_state                               as gsti_dbtr_state,
 dbtr_phone                               as gsti_dbtr_phone,

 --
 -- standard headers
 --
 patn__sequence                           as gsti__sequence,
 patn__timestamp                          as gsti__timestamp,
 patn__user_entry                         as gsti__user_entry,
 patn__status                             as gsti__status

from  ( (invc JOIN patn ON (patn.patn__sequence = invc.invc_patn__sequence) ) 
          JOIN dbtr ON (dbtr.dbtr_code      = invc.invc_dbtr_code) )

where invc.invc_gst_amount != 0;

grant select on gsti to public;

select create_metadata('gsti');
update mtcl set mtcl_title = 'GST Invoicing (View)' where mtcl_name = 'gsti';

-- update history;
\set mttb_name '\'gsti\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

