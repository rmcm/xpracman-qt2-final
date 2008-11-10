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
 

drop view bkdv;
create view bkdv                -- Services Performed (View)
 as select
 paym_bkdp__sequence                    as bkdv_bkdp__sequence, 
 to_char(paym_date_entry,'DD-MM-YYYY HH24:MI')                        as bkdv_date_entry,
 paym_user_entry                        as bkdv_user_entry,
 paym_site_entry                        as bkdv_site_entry,
 paym_amount                            as bkdv_paym_amount,
 paym_tdtp_code                         as bkdv_tdtp_code,
 tdtp_list                              as bkdv_tdtp_list,
 tdtp_subtotal                          as bkdv_tdtp_subtotal,
 tdtp_desc                              as bkdv_tdtp_desc,
 tdtp_entity                            as bkdv_tdtp_entity,
 tdtp_location                          as bkdv_tdtp_location,
 paym_drawer                            as bkdv_drawer,
 paym_bank                              as bkdv_bank,
 paym_branch                            as bkdv_branch,
 paym__sequence                         as bkdv_paym__sequence,
 cred_invc__sequence                    as bkdv_invc__sequence,
 cred_amount                            as bkdv_cred_amount,
 cred_gst_amount                        as bkdv_cred_gst_amount,
 invc_patn__sequence                    as bkdv_patn__sequence,
 patn_psnam                             as bkdv_patn_psnam,
 patn_fsnam                             as bkdv_patn_fsnam,
 invc_dbtr_code                         as bkdv_dbtr_code,
 dbtr_name                              as bkdv_dbtr_name,
 bank_code                              as bkdv_bank_code,
 bank_name                              as bkdv_bank_name,
 bank_address                           as bkdv_bank_address,
 bank_suburb                            as bkdv_bank_suburb,
 bank_state                             as bkdv_bank_state,
 bank_postcode                          as bkdv_bank_postcode,
 bank_phone                             as bkdv_bank_phone,
 bank_bank                              as bkdv_bank_bank,
 bank_branch                            as bkdv_bank_branch,
 bank_account                           as bkdv_bank_account,

 --
 -- standard headers
 --
 cred__sequence                         as bkdv__sequence,              -- Row-ID
 cred__timestamp                        as bkdv__timestamp,             -- Timestamp
 cred__user_entry                       as bkdv__user_entry,            -- Entered by
 cred__status                           as bkdv__status                 -- Record status
 --

FROM ((((((paym LEFT JOIN tdtp ON (tdtp.tdtp_code = paym.paym_tdtp_code))
        LEFT JOIN cred ON (cred.cred_paym__sequence = paym.paym__sequence))
        LEFT JOIN invc ON (invc.invc__sequence = cred.cred_invc__sequence))
        LEFT JOIN patn ON (patn.patn__sequence = invc.invc_patn__sequence))
        LEFT JOIN dbtr ON (dbtr.dbtr_code = invc.invc_dbtr_code))
        LEFT JOIN bank ON (bank.bank_code = invc.invc_bank_code))

;

grant select on bkdv to public;


select create_metadata('bkdv');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('bkdv','$Id:');
