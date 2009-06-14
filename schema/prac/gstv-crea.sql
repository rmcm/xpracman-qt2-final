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

drop view gstv;
create view gstv                       -- GST Payments View
 as select
 --
 -- cred
 --
 cred_paym__sequence                      as gstv_cred_paym__sequence,
 cred_invc__sequence                      as gstv_cred_invc__sequence,
 cred_amount                              as gstv_cred_amount,
 cred_amount + cred_gst_amount            as gstv_cred_total_amount,
 cred_gst_amount                          as gstv_cred_gst_amount,
 --
 -- paym
 --
 paym_date_entry                          as gstv_paym_date_entry,
 paym_user_entry                          as gstv_paym_user_entry,
 paym_site_entry                          as gstv_paym_site_entry,
 paym_amount                              as gstv_paym_amount,
 paym_tdtp_code                           as gstv_paym_tdtp_code,
 paym_drawer                              as gstv_paym_drawer,
 paym_bank                                as gstv_paym_bank,
 paym_branch                              as gstv_paym_branch,
 paym_bkdp__sequence                      as gstv_paym_bkdp__sequence,
 --
 -- bkdp
 --
 short_date(bkdp_date_created)            as gstv_day_date,
 quarter_date(bkdp_date_created)          as gstv_quarter_date,
 bkdp_date_created                        as gstv_bkdp_date_created,
 bkdp_date_printed                        as gstv_bkdp_date_printed,
 bkdp_user_printed                        as gstv_bkdp_user_printed,
 bkdp_bank_code                           as gstv_bkdp_bank_code,
 bkdp_desc                                as gstv_bkdp_desc,
 bkdp_amount                              as gstv_bkdp_amount,
 --
 -- invc
 --
 invc_dbtr_code                           as gstv_invc_dbtr_code,
 invc_bank_code                           as gstv_invc_bank_code,
 invc_prov_code                           as gstv_invc_prov_code,
 invc_patn__sequence                      as gstv_invc_patn__sequence,
 invc_empl_code                           as gstv_invc_empl_code,
 invc_feet_code                           as gstv_invc_feet_code,
 invc_rfdr_code                           as gstv_invc_rfdr_code,
 invc_rfdr_date                           as gstv_invc_rfdr_date,
 invc_rfdr_period                         as gstv_invc_rfdr_period,
 invc_date_created                        as gstv_invc_date_created,
 invc_date_printed                        as gstv_invc_date_printed,
 invc_date_reprint                        as gstv_invc_date_reprint,
 invc_amount                              as gstv_invc_amount,
 invc_paid_amount                         as gstv_invc_paid_amount,
 invc_gst_amount                          as gstv_invc_gst_amount,
 invc_paid_gst_amount                     as gstv_invc_paid_gst_amount,
 --
 -- patn
 --
 patn_flno                                as gstv_patn_flno,
 patn_psnam                               as gstv_patn_psnam,
 patn_fsnam                               as gstv_patn_fsnam,
 patn_title                               as gstv_patn_title,
 short_date(patn_dob)                     as gstv_patn_dob,
 patn_address                             as gstv_patn_address,
 patn_suburb                              as gstv_patn_suburb,
 patn_state                               as gstv_patn_state,
 patn_postcode                            as gstv_patn_postcode,
 patn_phone                               as gstv_patn_phone,
 --
 -- Debtor
 --
 patn_dbtr_code                           as gstv_dbtr_code,
 dbtr_name                                as gstv_dbtr_name,
 dbtr_address                             as gstv_dbtr_address,
 dbtr_suburb                              as gstv_dbtr_suburb,
 dbtr_postcode                            as gstv_dbtr_postcode,
 dbtr_state                               as gstv_dbtr_state,
 dbtr_phone                               as gstv_dbtr_phone,

 --
 -- standard headers
 --
 patn__sequence                           as gstv__sequence,
 patn__timestamp                          as gstv__timestamp,
 patn__user_entry                         as gstv__user_entry,
 patn__status                             as gstv__status

from cred,paym,bkdp,invc,patn,dbtr
where cred.cred_gst_amount != 0
and   paym.paym__sequence = cred.cred_paym__sequence
and   bkdp.bkdp__sequence = paym.paym_bkdp__sequence
and   invc.invc__sequence = cred.cred_invc__sequence
and   patn.patn__sequence = invc.invc_patn__sequence
and   dbtr.dbtr_code      = invc.invc_dbtr_code
and   paym_tdtp_code not in ('W', 'R');

grant select on gstv to public;

select create_metadata('gstv');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('gstv','$Id$');
