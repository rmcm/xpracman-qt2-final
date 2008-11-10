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

drop view svlt;
create view svlt                -- Services Reporting (View)
 as select 

 date(svpf_date_service)                as svlt_date_service,
 svpf_date_service                      as svlt_ts_service,
 (case when svpf_serv_code ~* '/i$'
  then substring(svpf_serv_code from 1 for length(svpf_serv_code) - 2) || '*'
  else svpf_serv_code
  end)                                  as svlt_serv_code,
 svpf_percentage                        as svlt_percentage,
 svpf_desc                              as svlt_desc,
 svpf_amount                            as svlt_amount,
 svpf_gst_amount                        as svlt_gst_amount,
 svpf_amount+svpf_gst_amount            as svlt_total_amount,
 svpf_patn__sequence                    as svlt_patn__sequence,
 --
 -- invc
 --
 invc__sequence                         as svlt_invc__sequence,
 invc_date_printed                      as svlt_invc_date_printed,
 invc_date_reprint                      as svlt_invc_date_reprint,
 invc_amount                            as svlt_invc_amount,
 invc_gst_amount                        as svlt_invc_gst_amount,
 invc_paid_amount+invc_paid_gst_amount  as svlt_invc_credits,
 cred_summary(invc__sequence,3)         as svlt_cred_summary,
 invc_amount+invc_gst_amount
 -invc_paid_amount-invc_paid_gst_amount as svlt_invc_balance,
 (case when invc_date_printed is null
  then now()::date  else date(invc_date_printed)
  end)                                  as svlt_date_printed,
 (case when invc_date_printed is null
  then null::date else now()::date
  end)                                  as svlt_date_reprint,
  (case when invc_rfdr_date is null
  then null::date else date(invc_rfdr_date)
  end)                                  as svlt_rfdr_date,
 (case when invc_rfdr_period = '99'
       then 'Indefinite'::text
  when invc_rfdr_period is null or invc_rfdr_period = '0'
       then ''::text
  else invc_rfdr_period || ' Months'
   end)                                 as svlt_rfdr_period,
 --
 -- patn
 --
 coalesce(patn_fsnam, '')
 ||' '
 ||coalesce(patn_psnam, '')             as svlt_patn_name,
 patn_address                           as svlt_patn_address,
 coalesce(patn_suburb, '')
 ||' '
 ||coalesce(patn_postcode , '')         as svlt_patn_suburb,
 patn_state                             as svlt_patn_state,
 patn_postcode                          as svlt_patn_postcode,
 short_date(patn_dob)                   as svlt_patn_dob,
 patn_hlfd_code                         as svlt_patn_hlfd_code,
 patn_ins_level                         as svlt_patn_ins_level,
 patn_healthnumb                        as svlt_patn_healthnumb,
 patn_medicare                          as svlt_patn_medicare,
 patn_healthcard                        as svlt_patn_healthcard,
 patn_patf_code                         as svlt_patn_patf_code,
 --
 -- dbtr
 --
 dbtr_address(dbtr_code)                as svlt_dbtr_full_address,
 dbtr_name                              as svlt_dbtr_name,
 dbtr_address                           as svlt_dbtr_address,
 coalesce(dbtr_suburb,'')
 ||' '
 ||coalesce(dbtr_postcode,'')           as svlt_dbtr_suburb,
 dbtr_state                             as svlt_dbtr_state,
 --
 -- rfdr
 --
 rfdr_code                              as svlt_rfdr_code,
 rfdr_name                              as svlt_rfdr_name,
 rfdr_street                            as svlt_rfdr_street,
 rfdr_suburb                            as svlt_rfdr_suburb,
 rfdr_postcode                          as svlt_rfdr_postcode,
 rfdr_state                             as svlt_rfdr_state,
 rfdr_provider                          as svlt_rfdr_provider,
 rfdr_phone                             as svlt_rfdr_phone,
 rfdr_salutation                        as svlt_rfdr_salutation,
 rfdr_index                             as svlt_rfdr_index,

 --
 -- empl
 --
 empl_code                              as svlt_empl_code,
 empl_name                              as svlt_empl_name,
 empl_address                           as svlt_empl_address,
 empl_suburb                            as svlt_empl_suburb,
 empl_postcode                          as svlt_empl_postcode,
 empl_state                             as svlt_empl_state,
 --
 -- feet
 --
 feet_code                              as svlt_feet_code,
 feet_desc                              as svlt_feet_desc,
 --
 -- prov
 --
 prov_code                              as svlt_prov_code,
 prov_provider_num                      as svlt_prov_provider_num,
 prov_name                              as svlt_prov_name,
 prov_address                           as svlt_prov_address,
 prov_suburb                            as svlt_prov_suburb,
 prov_state                             as svlt_prov_state,
 prov_postcode                          as svlt_prov_postcode,
 prov_salutation                        as svlt_prov_salutation,
 prov_phone                             as svlt_prov_phone,
 prov_bank_code                         as svlt_prov_bank_code,

 --
 -- standard headers
 --
 svpf__sequence                         as svlt__sequence,              -- Row-ID
 svpf__timestamp                        as svlt__timestamp,             -- Timestamp
 svpf__user_entry                       as svlt__user_entry,            -- Entered by
 svpf__status                           as svlt__status                 -- Record status
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
 and   invc_rfdr_code = rfdr_code
 and   invc_feet_code = feet_code
 and   invc_prov_code = prov_code
 and   invc_empl_code = empl_code
-- order by svpf_invc__sequence desc, svpf_date_service asc, svpf_amount desc
;

grant select,insert,update,delete on svlt to public;

-- update metadata
select create_metadata('svlt');
update mtcl set mtcl_title = 'Services Reporting (View)' where mtcl_name = 'svlt';

-- update history;
\set mttb_name '\'svlt\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

