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
 

drop view eftv;
create view eftv                -- Services Performed (View)
 as select
 eftr__sequence                                         as eftv_eftr__sequence,
 paym_bkdp__sequence                                    as eftv_bkdp__sequence, 
 to_char(paym_date_entry,'DD-MM-YYYY HH24:MI')          as eftv_date_entry,
 paym_user_entry                                        as eftv_user_entry,
 paym_site_entry                                        as eftv_site_entry,
 paym_amount                                            as eftv_paym_amount,
 paym_tdtp_code                                         as eftv_tdtp_code,
 tdtp_list                                              as eftv_tdtp_list,
 tdtp_subtotal                                          as eftv_tdtp_subtotal,
 tdtp_desc                                              as eftv_tdtp_desc,
 tdtp_entity                                            as eftv_tdtp_entity,
 tdtp_location                                          as eftv_tdtp_location,
 paym_drawer                                            as eftv_drawer,
 paym_bank                                              as eftv_bank,
 paym_branch                                            as eftv_branch,
 paym__sequence                                         as eftv_paym__sequence,
 cred_invc__sequence                                    as eftv_invc__sequence,
 cred_amount + cred_gst_amount                          as eftv_cred_total_amount,
 cred_amount                                            as eftv_cred_amount,
 cred_gst_amount                                        as eftv_cred_gst_amount,
 invc_patn__sequence                                    as eftv_patn__sequence,
 patn_psnam                                             as eftv_patn_psnam,
 patn_fsnam                                             as eftv_patn_fsnam,
 invc_dbtr_code                                         as eftv_dbtr_code,
 dbtr_name                                              as eftv_dbtr_name,
 bank_code                                              as eftv_bank_code,
 bank_name                                              as eftv_bank_name,
 bank_address                                           as eftv_bank_address,
 bank_suburb                                            as eftv_bank_suburb,
 bank_state                                             as eftv_bank_state,
 bank_postcode                                          as eftv_bank_postcode,
 bank_phone                                             as eftv_bank_phone,
 bank_bank                                              as eftv_bank_bank,
 bank_branch                                            as eftv_bank_branch,
 bank_account                                           as eftv_bank_account,

 --
 -- standard headers
 --
 cred__sequence                         as eftv__sequence,              -- Row-ID
 cred__timestamp                        as eftv__timestamp,             -- Timestamp
 cred__user_entry                       as eftv__user_entry,            -- Entered by
 cred__status                           as eftv__status                 -- Record status
 --
 from  eftr,paym,tdtp,bank,cred,invc,dbtr,patn
 where paym__sequence between eftr_first__sequence and eftr_last__sequence
 and   tdtp_list ~* 'directed'
 and   tdtp_code = paym_tdtp_code
 and   cred_paym__sequence = paym__sequence
 and   invc__sequence = cred_invc__sequence
 and   patn__sequence = invc_patn__sequence
 and   dbtr_code = invc_dbtr_code
 and   bank_code = invc_bank_code
 order by eftv_paym__sequence, eftv__sequence
;

grant select on eftv to public;
select create_metadata('eftv');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('eftv','$Id$');
