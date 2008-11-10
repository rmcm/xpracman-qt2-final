--  -*- sql -*-
--
--    $Id$

--  Copyright 2001 X=X Computer Software Trust
--                Kangaroo Ground Australia 3097


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

drop view dbst;
create view dbst                -- invoices - for use on debtor statements.
 as select
 invc_dbtr_code                                                    as dbst_dbtr_code,
 dbtr_address(invc_dbtr_code)                                      as dbst_full_address,
 dbtr_name                                                         as dbst_name,
 dbtr_address                                                      as dbst_address,     -- Street
 coalesce(dbtr_suburb, '')
 ||' '
 ||coalesce(dbtr_postcode , '')                                    as dbst_suburb,      -- Suburb
 dbtr_postcode                                                     as dbst_postcode,    -- Postcode
 coalesce(dbtr_state,'')                                           as dbst_state,       -- State
 dbtr_phone                                                        as dbst_phone,       -- Phone
 invc_patient_name(invc__sequence)                                 as dbst_patient,     -- patient name
 dbtr_patient_key(dbtr_code)                                       as dbst_patient_key, -- sort key
 dbtr_group                                                        as dbst_group,
 dbtr_last_statement                                               as dbst_last_statement,
 dbtr_first_statement                                              as dbst_first_statement,
 dbtr_last_invc_printed(invc_dbtr_code)                            as dbst_last_invoice,
 dbtr_delay_statement                                              as dbst_delay_statement,
 date(now() - (conf_statement_age|| ' days')::interval)            as dbst_date,
 -- balances
 dbtr_amount_outstanding                                           as dbst_amount_outstanding,
 dbtr_total_balance(invc_dbtr_code)                                as dbst_total_balance,
 dbtr_period_balance(dbtr_code,0,conf_statement_age)               as dbst_balance_0,
 dbtr_period_balance(dbtr_code,30,conf_statement_age)              as dbst_balance_30,
 dbtr_period_balance(dbtr_code,60,conf_statement_age)              as dbst_balance_60,
 dbtr_period_balance(dbtr_code,90,conf_statement_age)              as dbst_balance_90,
 dbtr_period_balance(dbtr_code,120,conf_statement_age)             as dbst_balance_120,
 -- invc
 invc_feet_code                                                    as dbst_feet_code,
 invc__sequence                                                    as dbst_invc__sequence,
 invc_amount+invc_gst_amount                                       as dbst_amount,
 invc_amount+invc_gst_amount
        - invc_balance_then(invc__sequence,conf_statement_age)
                                                                   as dbst_paid,
 invc_balance_then(invc__sequence,conf_statement_age)
                                                                   as dbst_balance,
 invc_age_period(invc__sequence, conf_statement_age)
                                                                   as dbst_period,
 -- cannot use coalesce here - as date() only applies to non-null - ie date()
 -- will not return ''
 (case when invc_date_printed is null then
    NULL::date
  else date(invc_date_printed) end)                                as dbst_date_printed,
 (case when invc_date_reprint is null then
    NULL::date
  else date(invc_date_reprint) end)                                as dbst_date_reprint,

 (case when invc_feet_code ~ 'TAC|WC' then
    invc_claim_number
  else case when invc_feet_code ~ 'VA' then
    invc_healthcard
  else '' end end)                                                 as dbst_reference,

 --
 -- patn
 --
 coalesce(patn_fsnam, '')
 ||' '
 ||coalesce(patn_psnam, '')                                        as dbst_patn_name,
 patn_address                                                      as dbst_patn_address,
 coalesce(patn_suburb, '')
 ||' '
 ||coalesce(patn_postcode , '')                                    as dbst_patn_suburb,
 patn_state                                                        as dbst_patn_state,
 patn_postcode                                                     as dbst_patn_postcode,
 short_date(patn_dob)                                              as dbst_patn_dob,
 patn_hlfd_code                                                    as dbst_patn_hlfd_code,
 patn_ins_level                                                    as dbst_patn_ins_level,
 patn_healthnumb                                                   as dbst_patn_healthnumb,
 patn_medicare                                                     as dbst_patn_medicare,
 patn_healthcard                                                   as dbst_patn_healthcard,
 patn_patf_code                                                    as dbst_patn_patf_code,
 patn__sequence                                                    as dbst_patn__sequence,

 --
 -- standard headers
 --
 invc__sequence                                                    as dbst__sequence,   -- Row-ID
 invc__timestamp                                                   as dbst__timestamp,  -- Timestamp
 invc__user_entry                                                  as dbst__user_entry, -- Entered by
 invc__status                                                      as dbst__status      -- Record status
 --
 from  invc,dbtr,patn,
        (select conf_integer('dbst_statement_age') as conf_statement_age,
                now() - (conf_integer('dbst_statement_age') || ' days')::interval as conf_statement_date) as conf
 where dbtr_code = invc_dbtr_code
 and   patn_patf_code != 'HOLD'
 and   invc_date_printed is not null
 and   invc_date_printed < conf_statement_date
-- and   invc_balance_then(invc__sequence,conf_statement_age) is not null
 and   coalesce(invc_balance_then(invc__sequence,conf_statement_age), 0.0) != 0.0
 and   invc_patn__sequence = patn__sequence
;

grant select on dbst to public;

select create_metadata('dbst');

-- update history;
\set mttb_name '\'dbst\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
