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

create or replace function svpv_form(text, text)
returns text
as 'DECLARE
    arg_patf alias for $1;
    arg_feet alias for $2;
    x_form_code form.form_code%TYPE;

    BEGIN

      -- look for patf match first
      if (length(arg_patf) > 0) then
        select  form_code
        into    x_form_code
        from    form
        where   form_code = ''invoice-svpv_'' || arg_patf;
      end if;

      if (length(x_form_code) > 0) then
        return x_form_code;
      end if;

      -- look for feet match next
      if (length(arg_feet) > 0) then
        select  form_code
        into    x_form_code
        from    form
        where   form_code = ''invoice-svpv_'' || arg_feet;
      end if;

      if (length(x_form_code) > 0) then
        return x_form_code;
      end if;

      -- default
      return ''invoice-svpv'';
    END;'
  LANGUAGE 'plpgsql';

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
 (case when invc_date_printed is null then
        patn_total_debtor_balance(patn__sequence,0,invc_dbtr_code)
        + invc_amount+invc_gst_amount-invc_paid_amount-invc_paid_gst_amount
  else
         patn_total_debtor_balance(patn__sequence,0,invc_dbtr_code)
  end)                                  as svpv_patn_dbtr_balance,
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
 hlfd_desc                              as svpv_hlfd_desc,
 invc_ins_level                         as svpv_invc_ins_level,
 invc_healthnumb                        as svpv_invc_healthnumb,
 invc_healthcard                        as svpv_invc_healthcard,
 invc_medicare                          as svpv_invc_medicare,
 invc_claim_number                      as svpv_invc_claim_number,
 date(invc_accident_date)               as svpv_invc_accident_date,
 invc_reference_1                       as svpv_invc_reference_1,
 invc_reference_2                       as svpv_invc_reference_2,
 invc_reference_3                       as svpv_invc_reference_3,
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
 patn_claim_number                      as svpv_patn_claim_number,
 date(patn_accident_date)               as svpv_patn_accident_date,
 patn_fax_number                        as svpv_patn_fax_number,
 patn_email_address                     as svpv_patn_email_address,
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
 dbtr_fax_number                        as svpv_dbtr_fax_number,
 dbtr_email_address                     as svpv_dbtr_email_address,
 --
 -- rfdr
 --
 rfdr_code                              as svpv_rfdr_code,
 rfdr_name                              as svpv_rfdr_name,
 rfdr_street                            as svpv_rfdr_street,
 rfdr_suburb                            as svpv_rfdr_suburb,
 rfdr_postcode                          as svpv_rfdr_postcode,
 rfdr_state                             as svpv_rfdr_state,
 (case when rfdr_provider is null or length(rfdr_provider) < 1 then
    coalesce(rfdr_street,'') || ',' 
    || coalesce(rfdr_suburb, '') || ' '
    || coalesce(rfdr_postcode, '') || ' '
    || coalesce(rfdr_state, '')
  else
    rfdr_provider
  end)                                  as svpv_rfdr_provider,
 rfdr_phone                             as svpv_rfdr_phone,
 rfdr_salutation                        as svpv_rfdr_salutation,
 rfdr_index                             as svpv_rfdr_index,
 rfdr_fax_number                        as svpv_rfdr_fax_number,
 rfdr_email_address                     as svpv_rfdr_email_address,

 --
 -- empl
 --
 empl_code                              as svpv_empl_code,
 empl_name                              as svpv_empl_name,
 empl_address                           as svpv_empl_address,
 empl_suburb                            as svpv_empl_suburb,
 empl_postcode                          as svpv_empl_postcode,
 empl_state                             as svpv_empl_state,
 empl_fax_number                        as svpv_empl_fax_number,
 empl_email_address                     as svpv_empl_email_address,
 --
 -- feet
 --
 feet_code                              as svpv_feet_code,
 feet_desc                              as svpv_feet_desc,
 --
 -- hlfd
 --
 hlfd_fax_number                        as svpv_hlfd_fax_number,
 hlfd_email_address                     as svpv_hlfd_email_address,

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
 prov_fax_number                        as svpv_prov_fax_number,
 prov_email_address                     as svpv_prov_email_address,

 --
 -- Form
 -- 
 svpv_form(patn_patf_code,feet_code)    as svpv_form_code,
 --
 -- standard headers
 --
 svpf__sequence                         as svpv__sequence,              -- Row-ID
 svpf__timestamp                        as svpv__timestamp,             -- Timestamp
 svpf__user_entry                       as svpv__user_entry,            -- Entered by
 svpf__status                           as svpv__status                 -- Record status
 --

 from ((((((((svpf LEFT JOIN invc ON (svpf.svpf_invc__sequence = invc.invc__sequence))
                   LEFT JOIN patn ON (invc.invc_patn__sequence = patn.patn__sequence))
                   LEFT JOIN dbtr ON (invc.invc_dbtr_code = dbtr.dbtr_code))
                   LEFT JOIN rfdr ON (invc.invc_rfdr_code = rfdr.rfdr_code))
                   LEFT JOIN empl ON (invc.invc_empl_code = empl.empl_code))
                   LEFT JOIN feet ON (invc.invc_feet_code = feet.feet_code))
                   LEFT JOIN prov ON (invc.invc_prov_code = prov.prov_code))
                   LEFT JOIN hlfd ON (invc.invc_hlfd_code = hlfd.hlfd_code))

;

grant select on svpv to public;

-- update metadata
select create_metadata('svpv');
update mtcl set mtcl_order_by = 'order by svpv_form_code, svpv_invc__sequence desc, svpv__timestamp asc, svpv_amount desc' where mtcl_name = 'svpv';

-- update history;
\set mttb_name '\'svpv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;


