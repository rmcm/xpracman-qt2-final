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

drop view invv;

create view invv                -- Services Performed (View)
 as select 

 --
 -- invc
 --
 invc__sequence                         as invv_invc__sequence,
 short_date(invc_date_printed)          as invv_invc_date_printed,
 short_date(invc_date_reprint)          as invv_invc_date_reprint,
 invc_amount                            as invv_invc_amount,
 invc_gst_amount                        as invv_invc_gst_amount,
 invc_paid_amount+invc_paid_gst_amount  as invv_invc_credits,
 cred_summary(invc__sequence,3)         as invv_cred_summary,
 invc_amount+invc_gst_amount
 -invc_paid_amount-invc_paid_gst_amount as invv_invc_balance,
 short_date(case when invc_date_printed is null
  then now()::date  else date(invc_date_printed)
  end)                                  as invv_date_printed,
 short_date(case when invc_date_printed is null
  then null::date else now()::date
  end)                                  as invv_date_reprint,
  short_date(case when invc_rfdr_date is null
  then null::date else date(invc_rfdr_date)
  end)                                  as invv_rfdr_date,
 (case when invc_rfdr_period = '99'
       then 'Indefinite'::text
  when invc_rfdr_period is null or invc_rfdr_period = '0'
       then ''::text
  else invc_rfdr_period || ' Months'
   end)                                 as invv_rfdr_period,
 hlfd_desc                              as invv_hlfd_desc,
 invc_ins_level                         as invv_invc_ins_level,
 invc_healthnumb                        as invv_invc_healthnumb,
 invc_healthcard                        as invv_invc_healthcard,
 invc_medicare                          as invv_invc_medicare,
 invc_claim_number                      as invv_invc_claim_number,
 short_date(invc_accident_date)         as invv_invc_accident_date,
 invc_reference_1                       as invv_invc_reference_1,
 invc_reference_2                       as invv_invc_reference_2,
 invc_reference_3                       as invv_invc_reference_3,

 --
 -- patn
 --
 patn__sequence                         as invv_patn__sequence,
 coalesce(patn_fsnam, '')
 ||' '
 ||coalesce(patn_psnam, '')             as invv_patn_name,
 patn_address                           as invv_patn_address,
 coalesce(patn_suburb, '')
 ||' '
 ||coalesce(patn_postcode , '')         as invv_patn_suburb,
 patn_state                             as invv_patn_state,
 patn_postcode                          as invv_patn_postcode,
 short_date(patn_dob)                   as invv_patn_dob,
 patn_hlfd_code                         as invv_patn_hlfd_code,
 patn_ins_level                         as invv_patn_ins_level,
 patn_healthnumb                        as invv_patn_healthnumb,
 patn_medicare                          as invv_patn_medicare,
 patn_healthcard                        as invv_patn_healthcard,
 patn_patf_code                         as invv_patn_patf_code,
 patn_claim_number                      as invv_patn_claim_number,
 short_date(patn_accident_date)         as invv_patn_accident_date,
 --
 -- dbtr
 --
 dbtr_address(dbtr_code)                as invv_dbtr_full_address,
 dbtr_name                              as invv_dbtr_name,
 dbtr_address                           as invv_dbtr_address,
 coalesce(dbtr_suburb,'')
 ||' '
 ||coalesce(dbtr_postcode,'')           as invv_dbtr_suburb,
 dbtr_state                             as invv_dbtr_state,
 --
 -- rfdr
 --
 rfdr_code                              as invv_rfdr_code,
 rfdr_name                              as invv_rfdr_name,
 rfdr_street                            as invv_rfdr_street,
 rfdr_suburb                            as invv_rfdr_suburb,
 rfdr_postcode                          as invv_rfdr_postcode,
 rfdr_state                             as invv_rfdr_state,
 (case when rfdr_provider is null or length(rfdr_provider) < 1 then
    coalesce(rfdr_street,'') || ',' 
    || coalesce(rfdr_suburb, '') || ' '
    || coalesce(rfdr_postcode, '') || ' '
    || coalesce(rfdr_state, '')
  else
    rfdr_provider
  end)                                  as invv_rfdr_provider,
 rfdr_phone                             as invv_rfdr_phone,
 rfdr_salutation                        as invv_rfdr_salutation,
 rfdr_index                             as invv_rfdr_index,

 --
 -- empl
 --
 empl_code                              as invv_empl_code,
 empl_name                              as invv_empl_name,
 empl_address                           as invv_empl_address,
 empl_suburb                            as invv_empl_suburb,
 empl_postcode                          as invv_empl_postcode,
 empl_state                             as invv_empl_state,
 --
 -- feet
 --
 feet_code                              as invv_feet_code,
 feet_desc                              as invv_feet_desc,
 --
 -- prov
 --
 prov_code                              as invv_prov_code,
 prov_provider_num                      as invv_prov_provider_num,
 prov_name                              as invv_prov_name,
 prov_address                           as invv_prov_address,
 prov_suburb                            as invv_prov_suburb,
 prov_state                             as invv_prov_state,
 prov_postcode                          as invv_prov_postcode,
 prov_salutation                        as invv_prov_salutation,
 prov_phone                             as invv_prov_phone,
 prov_bank_code                         as invv_prov_bank_code,

 --
 -- standard headers
 --
 invc__sequence                         as invv__sequence,              -- Row-ID
 invc__timestamp                        as invv__timestamp,             -- Timestamp
 invc__user_entry                       as invv__user_entry,            -- Entered by
 invc__status                           as invv__status                 -- Record status
 --

 from (((((((invc LEFT JOIN patn ON (invc.invc_patn__sequence = patn.patn__sequence))
                  LEFT JOIN dbtr ON (invc.invc_dbtr_code = dbtr.dbtr_code))
                  LEFT JOIN rfdr ON (invc.invc_rfdr_code = rfdr.rfdr_code))
                  LEFT JOIN empl ON (invc.invc_empl_code = empl.empl_code))
                  LEFT JOIN feet ON (invc.invc_feet_code = feet.feet_code))
                  LEFT JOIN prov ON (invc.invc_prov_code = prov.prov_code))
                  LEFT JOIN hlfd ON (invc.invc_hlfd_code = hlfd.hlfd_code))

;

grant select on invv to public;

-- update metadata
select create_metadata('invv');

-- update history;
\set mttb_name '\'invv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;


