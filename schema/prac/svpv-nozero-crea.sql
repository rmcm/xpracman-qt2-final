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

drop view svpv;
create view svpv                -- Services Performed (View)
 as select 

 date(svpf_date_service)                as svpv_date_service,
 svpf_date_service                      as svpv_ts_service,
 (case when svpf_serv_code ~* '/i$'
  then substring(svpf_serv_code from 1 for length(svpf_serv_code) - 2) || '*'
  else svpf_serv_code
  end)                                  as svpv_serv_code,
 svpf_percentage                        as svpv_percentage,
 svpf_desc                              as svpv_desc,
 svpf_amount                            as svpv_amount,
 svpf_gst_amount                        as svpv_gst_amount,
 svpf_amount+svpf_gst_amount            as svpv_total_amount,
 svpf_patn__sequence                    as svpv_patn__sequence,
 --
 -- invc
 --
 invc__sequence                         as svpv_invc__sequence,
 invc_date_printed                      as svpv_invc_date_printed,
 invc_date_reprint                      as svpv_invc_date_reprint,
 invc_amount                            as svpv_invc_amount,
 invc_gst_amount                        as svpv_invc_gst_amount,
 invc_paid_amount+invc_paid_gst_amount  as svpv_invc_credits,
 cred_summary(invc__sequence,3)         as svpv_cred_summary,
 invc_amount+invc_gst_amount
 -invc_paid_amount-invc_paid_gst_amount as svpv_invc_balance,
 (case when invc_date_printed is null
  then now()::date  else date(invc_date_printed)
  end)                                  as svpv_date_printed,
 (case when invc_date_printed is null
  then null::date else now()::date
  end)                                  as svpv_date_reprint,
  (case when invc_rfdr_date is null
  then null::date else date(invc_rfdr_date)
  end)                                  as svpv_rfdr_date,
 (case when invc_rfdr_period = '99'
       then 'Indefinite'::text
  when invc_rfdr_period is null or invc_rfdr_period = '0'
       then ''::text
  else invc_rfdr_period || ' Months'
   end)                                 as svpv_rfdr_period,
  (case when invc_amount+invc_gst_amount
                 -invc_paid_amount-invc_paid_gst_amount = 0
   then '' else 'PLEASE SETTLE ACCOUNT WITHIN 14 DAYS. ' ||
        'A RECEIPT WILL THEN BE ISSUED FOR MEDICARE/HEALTH FUND'
   end)                                 as svpv_zero_balance,
 --
 -- patn
 --
 coalesce(patn_fsnam, '')
 ||' '
 ||coalesce(patn_psnam, '')             as svpv_patn_name,
 patn_address                           as svpv_patn_address,
 coalesce(patn_suburb, '')
 ||' '
 ||coalesce(patn_postcode , '')         as svpv_patn_suburb,
 patn_state                             as svpv_patn_state,
 patn_postcode                          as svpv_patn_postcode,
 short_date(patn_dob)                   as svpv_patn_dob,
 patn_hlfd_code                         as svpv_patn_hlfd_code,
 patn_ins_level                         as svpv_patn_ins_level,
 patn_healthnumb                        as svpv_patn_healthnumb,
 patn_medicare                          as svpv_patn_medicare,
 patn_healthcard                        as svpv_patn_healthcard,
 patn_patf_code                         as svpv_patn_patf_code,
 --
 -- dbtr
 --
 dbtr_address(dbtr_code)                as svpv_dbtr_full_address,
 dbtr_name                              as svpv_dbtr_name,
 dbtr_address                           as svpv_dbtr_address,
 coalesce(dbtr_suburb,'')
 ||' '
 ||coalesce(dbtr_postcode,'')           as svpv_dbtr_suburb,
 dbtr_state                             as svpv_dbtr_state,
 --
 -- rfdr
 --
 rfdr_code                              as svpv_rfdr_code,
 rfdr_name                              as svpv_rfdr_name,
 rfdr_street                            as svpv_rfdr_street,
 rfdr_suburb                            as svpv_rfdr_suburb,
 rfdr_postcode                          as svpv_rfdr_postcode,
 rfdr_state                             as svpv_rfdr_state,
 rfdr_provider                          as svpv_rfdr_provider,
 rfdr_phone                             as svpv_rfdr_phone,
 rfdr_salutation                        as svpv_rfdr_salutation,
 rfdr_index                             as svpv_rfdr_index,

 --
 -- empl
 --
 empl_code                              as svpv_empl_code,
 empl_name                              as svpv_empl_name,
 empl_address                           as svpv_empl_address,
 empl_suburb                            as svpv_empl_suburb,
 empl_postcode                          as svpv_empl_postcode,
 empl_state                             as svpv_empl_state,
 --
 -- feet
 --
 feet_code                              as svpv_feet_code,
 feet_desc                              as svpv_feet_desc,
 --
 -- prov
 --
 prov_code                              as svpv_prov_code,
 prov_provider_num                      as svpv_prov_provider_num,
 prov_name                              as svpv_prov_name,
 prov_address                           as svpv_prov_address,
 prov_suburb                            as svpv_prov_suburb,
 prov_state                             as svpv_prov_state,
 prov_postcode                          as svpv_prov_postcode,
 prov_salutation                        as svpv_prov_salutation,
 prov_phone                             as svpv_prov_phone,
 prov_bank_code                         as svpv_prov_bank_code,

 --
 -- standard headers
 --
 svpf__sequence                         as svpv__sequence,              -- Row-ID
 svpf__timestamp                        as svpv__timestamp,             -- Timestamp
 svpf__user_entry                       as svpv__user_entry,            -- Entered by
 svpf__status                           as svpv__status                 -- Record status
 --

-- from (((((((svpf LEFT JOIN invc ON (svpf.svpf_invc__sequence = invc.invc__sequence))
--                  LEFT JOIN patn ON (invc.invc_patn__sequence = patn.patn__sequence))
--                  LEFT JOIN dbtr ON (invc.invc_dbtr_code = dbtr.dbtr_code))
--                  LEFT JOIN rfdr ON (invc.invc_rfdr_code = rfdr.rfdr_code))
--                  LEFT JOIN empl ON (invc.invc_empl_code = empl.empl_code))
--                  LEFT JOIN feet ON (invc.invc_feet_code = feet.feet_code))
--                  LEFT JOIN prov ON (invc.invc_prov_code = prov.prov_code))
-- 
 from svpf, invc, patn, dbtr, rfdr, empl, feet, prov
 where svpf_invc__sequence = invc__sequence
 and   invc_dbtr_code = dbtr_code
 and   invc_patn__sequence = patn__sequence
 and   patn_patf_code != 'HOLD'
 and   invc_rfdr_code = rfdr_code
 and   invc_feet_code = feet_code
 and   invc_prov_code = prov_code
 and   invc_empl_code = empl_code
-- order by svpf_invc__sequence desc, svpf_date_service asc, svpf_amount desc
;

grant select on svpv to public;

-- update metadata
select create_metadata('svpv');

-- update history;
--insert into mttb(mttb_name, mttb_rcs_header) values('svpv','$Id$');


