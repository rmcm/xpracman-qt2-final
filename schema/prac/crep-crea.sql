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
 

drop view crep;
create view crep                -- Credits from payments (View)
 as select 

 invc_dbtr_code                         as crep_dbtr_code,
 invc_bank_code                         as crep_bank_code,
 invc_prov_code                         as crep_prov_code,
 invc_patn__sequence                    as crep_patn__sequence,
 invc_empl_code                         as crep_empl_code,
 invc_feet_code                         as crep_feet_code,
 invc_rfdr_code                         as crep_rfdr_code,
 invc_rfdr_date                         as crep_rfdr_date,
 invc_rfdr_period                       as crep_rfdr_period,
 invc_date_created                      as crep_date_created,
 invc_date_printed                      as crep_date_printed,
 invc_date_reprint                      as crep_date_reprint,
 invc_amount                            as crep_amount,
 invc_paid_amount                       as crep_paid_amount,
 invc_gst_amount                        as crep_gst_amount,
 invc_paid_gst_amount                   as crep_paid_gst_amount,

 cred_paym__sequence                    as crep_paym__sequence,
 cred_invc__sequence                    as crep_invc__sequence,
 cred_amount                            as crep_cred_amount,
 cred_gst_amount                        as crep_cred_gst_amount,
 invc_balance(cred_invc__sequence)      as crep_invc_balance,
 --
 -- patn
 --
 patn_patient_name(invc_patn__sequence) as crep_patient_name,
 --
 -- standard headers
 --
 cred__sequence                         as crep__sequence,              -- Row-ID
 cred__timestamp                        as crep__timestamp,             -- Timestamp
 cred__user_entry                       as crep__user_entry,            -- Entered by
 cred__status                           as crep__status                 -- Record status
 --
 from (cred JOIN invc ON (cred.cred_invc__sequence = invc.invc__sequence) )
       JOIN patn ON (invc.invc_patn__sequence = patn.patn__sequence)
;

grant select,insert,update on crep to public;

-- and now - an update rule

drop rule rl_crep_update;
create rule rl_crep_update as on update
       to crep
       do instead
       (
         -- IMPORTANT!!!!
         -- update credit first (updates invc via trigger),
         update cred
         set    cred_invc__sequence  = new.crep_invc__sequence,
                cred_paym__sequence  = new.crep_paym__sequence,
                cred_amount          = new.crep_cred_amount,
                cred_gst_amount      = new.crep_cred_gst_amount
         where  cred__sequence       = new.crep__sequence;
       );

drop rule rl_crep_delete;
create rule rl_crep_delete as on delete
        to crep
        do instead
        (
          delete from cred
          where  cred__sequence       = old.crep__sequence;
        );

select create_metadata('crep');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('crep','$Id:');
