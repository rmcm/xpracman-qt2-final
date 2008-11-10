--  -*- sql -*-
--  
--    $Id$

--  Copyright 2001 X=X Computer Software Trust
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

create or replace view ptst                -- invoices - for use on debtor statements.
 as select 

 -- dbtr

 dbtr_name                                                      as ptst_dbtr_name,
 date(now())                                                    as ptst_date,

 -- balances

 patn_total_balance(patn__sequence,0)                           as ptst_total_balance,
 patn_period_balance(patn__sequence,0,0)                        as ptst_balance_0,
 patn_period_balance(patn__sequence,30,0)                       as ptst_balance_30,
 patn_period_balance(patn__sequence,60,0)                       as ptst_balance_60,
 patn_period_balance(patn__sequence,90,0)                       as ptst_balance_90,
 patn_period_balance(patn__sequence,120,0)                      as ptst_balance_120,

 -- invc
 invc_feet_code                                                 as ptst_feet_code,
 invc__sequence                                                 as ptst_invc__sequence,
 invc_amount+invc_gst_amount                                    as ptst_amount,
 invc_amount+invc_gst_amount
        - invc_balance_then(invc__sequence,0)
                                                                as ptst_paid,
 invc_balance_then(invc__sequence,0)
                                                                as ptst_balance,
 invc_age_period(invc__sequence, 0)
                                                                as ptst_period,
 -- cannot use coalesce here - as date() only applies to non-null - ie date()
 -- will not return ''
 (case when invc_date_printed is null then
    NULL::date
  else date(invc_date_printed) end)                             as ptst_date_printed,
 (case when invc_date_reprint is null then
    NULL::date
  else date(invc_date_reprint) end)                             as ptst_date_reprint,

 --
 -- patn
 --
 patn_patient_title_name(patn__sequence)                        as ptst_patn_name,
 patn_address                                                   as ptst_patn_address,
 coalesce(patn_suburb, '')
 ||' '
 ||coalesce(patn_postcode , '')                                 as ptst_patn_suburb,
 patn_state                                                     as ptst_patn_state,
 patn_postcode                                                  as ptst_patn_postcode,
 short_date(patn_dob)                                           as ptst_patn_dob,
 patn_hlfd_code                                                 as ptst_patn_hlfd_code,
 patn_ins_level                                                 as ptst_patn_ins_level,
 patn_healthnumb                                                as ptst_patn_healthnumb,
 patn_medicare                                                  as ptst_patn_medicare,
 patn_healthcard                                                as ptst_patn_healthcard,
 patn_patf_code                                                 as ptst_patn_patf_code,
 patn__sequence                                                 as ptst_patn__sequence,

-- Credits
 cred_summary(invc__sequence, 5)                                as ptst_cred_summary,
 --
 -- standard headers
 --
 invc__sequence                                                 as ptst__sequence,   -- Row-ID
 invc__timestamp                                                as ptst__timestamp,  -- Timestamp
 invc__user_entry                                               as ptst__user_entry, -- Entered by
 invc__status                                                   as ptst__status      -- Record status
 --
 from  (patn LEFT JOIN invc ON (invc.invc_patn__sequence = patn.patn__sequence))
         LEFT JOIN dbtr ON (invc.invc_dbtr_code = dbtr.dbtr_code)
 where patn_patf_code != 'HOLD'
 and   invc_date_printed is not null
-- and   invc_fix_totals(invc__sequence) != 0.0

;

grant select on ptst to public;

select create_metadata('ptst');

-- update history;
\set mttb_name '\'ptst\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
