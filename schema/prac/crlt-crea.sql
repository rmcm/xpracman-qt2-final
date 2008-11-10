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

drop view crlt;
create view crlt                --  Credits (View)
 as select 

 --
 -- cred
 -- 
 date(cred__timestamp)                  as crlt_date_credit,
 cred_paym__sequence                    as crlt_paym__sequence,
 cred_amount                            as crlt_cred_amount,
 cred_gst_amount                        as crlt_cred_gst_amount,
 cred_amount+cred_gst_amount            as crlt_total_amount,
 cred_notes                             as crlt_notes,
 --
 -- paym
 -- 
 date(paym_date_entry)                  as crlt_date_payment,
 paym_date_entry                        as crlt_ts_payment,
 paym_user_entry                        as crlt_user_entry,
 paym_site_entry                        as crlt_site_entry,
 paym_amount                            as crlt_paym_amount,
 paym_tdtp_code                         as crlt_tdtp_code,
 paym_drawer                            as crlt_drawer,
 paym_bank                              as crlt_bank,
 paym_branch                            as crlt_branch,
 paym_bkdp__sequence                    as crlt_bkdp__sequence,
 --
 -- invc
 --
 invc__sequence                         as crlt_invc__sequence,
 invc_date_printed                      as crlt_invc_date_printed,
 invc_date_reprint                      as crlt_invc_date_reprint,
 invc_amount                            as crlt_invc_amount,
 invc_gst_amount                        as crlt_invc_gst_amount,
 invc_paid_amount+invc_paid_gst_amount  as crlt_invc_credits,
 cred_summary(invc__sequence,3)         as crlt_cred_summary,
 invc_amount+invc_gst_amount
 -invc_paid_amount-invc_paid_gst_amount as crlt_invc_balance,
 (case when invc_date_printed is null
  then now()::date  else date(invc_date_printed)
  end)                                  as crlt_date_printed,
 (case when invc_date_printed is null
  then null::date else now()::date
  end)                                  as crlt_date_reprint,
  (case when invc_rfdr_date is null
  then null::date else date(invc_rfdr_date)
  end)                                  as crlt_rfdr_date,
 (case when invc_rfdr_period = '99'
       then 'Indefinite'::text
  when invc_rfdr_period is null or invc_rfdr_period = '0'
       then ''::text
  else invc_rfdr_period || ' Months'
   end)                                 as crlt_rfdr_period,
 --
 -- patn
 --
 coalesce(patn_fsnam, '')
 ||' '
 ||coalesce(patn_psnam, '')             as crlt_patn_name,
 patn_address                           as crlt_patn_address,
 coalesce(patn_suburb, '')
 ||' '
 ||coalesce(patn_postcode , '')         as crlt_patn_suburb,
 patn_state                             as crlt_patn_state,
 patn_postcode                          as crlt_patn_postcode,
 short_date(patn_dob)                   as crlt_patn_dob,
 patn_hlfd_code                         as crlt_patn_hlfd_code,
 patn_ins_level                         as crlt_patn_ins_level,
 patn_healthnumb                        as crlt_patn_healthnumb,
 patn_medicare                          as crlt_patn_medicare,
 patn_healthcard                        as crlt_patn_healthcard,
 patn_patf_code                         as crlt_patn_patf_code,
 --
 -- dbtr
 --
 dbtr_address(dbtr_code)                as crlt_dbtr_full_address,
 dbtr_name                              as crlt_dbtr_name,
 dbtr_address                           as crlt_dbtr_address,
 coalesce(dbtr_suburb,'')
 ||' '
 ||coalesce(dbtr_postcode,'')           as crlt_dbtr_suburb,
 dbtr_state                             as crlt_dbtr_state,
 --
 -- rfdr
 --
 rfdr_code                              as crlt_rfdr_code,
 rfdr_name                              as crlt_rfdr_name,
 rfdr_street                            as crlt_rfdr_street,
 rfdr_suburb                            as crlt_rfdr_suburb,
 rfdr_postcode                          as crlt_rfdr_postcode,
 rfdr_state                             as crlt_rfdr_state,
 rfdr_provider                          as crlt_rfdr_provider,
 rfdr_phone                             as crlt_rfdr_phone,
 rfdr_salutation                        as crlt_rfdr_salutation,
 rfdr_index                             as crlt_rfdr_index,

 --
 -- empl
 --
 empl_code                              as crlt_empl_code,
 empl_name                              as crlt_empl_name,
 empl_address                           as crlt_empl_address,
 empl_suburb                            as crlt_empl_suburb,
 empl_postcode                          as crlt_empl_postcode,
 empl_state                             as crlt_empl_state,
 --
 -- feet
 --
 feet_code                              as crlt_feet_code,
 feet_desc                              as crlt_feet_desc,
 --
 -- prov
 --
 prov_code                              as crlt_prov_code,
 prov_provider_num                      as crlt_prov_provider_num,
 prov_name                              as crlt_prov_name,
 prov_address                           as crlt_prov_address,
 prov_suburb                            as crlt_prov_suburb,
 prov_state                             as crlt_prov_state,
 prov_postcode                          as crlt_prov_postcode,
 prov_salutation                        as crlt_prov_salutation,
 prov_phone                             as crlt_prov_phone,
 prov_bank_code                         as crlt_prov_bank_code,

 --
 -- standard headers
 --
 cred__sequence                         as crlt__sequence,              -- Row-ID
 cred__timestamp                        as crlt__timestamp,             -- Timestamp
 cred__user_entry                       as crlt__user_entry,            -- Entered by
 cred__status                           as crlt__status                 -- Record status
 --

-- 
 from cred, paym, invc, patn, dbtr, rfdr, empl, feet, prov
 where cred_invc__sequence = invc__sequence
 and   cred_paym__sequence = paym__sequence
 and   invc_dbtr_code = dbtr_code
 and   invc_patn__sequence = patn__sequence
 and   invc_rfdr_code = rfdr_code
 and   invc_feet_code = feet_code
 and   invc_prov_code = prov_code
 and   invc_empl_code = empl_code

;

grant select on crlt to public;

-- update metadata
select create_metadata('crlt');

-- update history;
\set mttb_name '\'crlt\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;


