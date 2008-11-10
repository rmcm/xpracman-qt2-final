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
-- copied from svpv
drop view svrv;
create view svrv                -- Services Performed Report (View)
 as select 

 date(svpf_date_service)                as svrv_date_service,
 svpf_date_service                      as svrv_ts_service,
 (case when svpf_serv_code ~* '/i$'
  then substring(svpf_serv_code from 1 for length(svpf_serv_code) - 2) || '*'
  else svpf_serv_code
  end)                                  as svrv_serv_code,
 svpf_percentage                        as svrv_percentage,
 svpf_desc                              as svrv_desc,
 svpf_amount                            as svrv_amount,
 svpf_gst_amount                        as svrv_gst_amount,
 svpf_amount+svpf_gst_amount            as svrv_total_amount,
 svpf_patn__sequence                    as svrv_patn__sequence,
  --
 -- patn
 --
 coalesce(patn_fsnam, '')
 ||' '
 ||coalesce(patn_psnam, '')             as svrv_patn_name,
 patn_address                           as svrv_patn_address,
 coalesce(patn_suburb, '')
 ||' '
 ||coalesce(patn_postcode , '')         as svrv_patn_suburb,
 patn_state                             as svrv_patn_state,
 patn_postcode                          as svrv_patn_postcode,
 short_date(patn_dob)                   as svrv_patn_dob,
 patn_hlfd_code                         as svrv_patn_hlfd_code,
 patn_ins_level                         as svrv_patn_ins_level,
 patn_healthnumb                        as svrv_patn_healthnumb,
 patn_medicare                          as svrv_patn_medicare,
 patn_healthcard                        as svrv_patn_healthcard,
 patn_patf_code                         as svrv_patn_patf_code,
 patn_claim_number                      as svrv_patn_claim_number,
 date(patn_accident_date)               as svrv_patn_accident_date,
  --
 -- standard headers
 --
 svpf__sequence                         as svrv__sequence,              -- Row-ID
 svpf__timestamp                        as svrv__timestamp,             -- Timestamp
 svpf__user_entry                       as svrv__user_entry,            -- Entered by
 svpf__status                           as svrv__status                 -- Record status
 --
-- 
 from svpf, patn
 where svpf_patn__sequence = patn__sequence
;

grant select on svrv to public;

-- update metadata
select create_metadata('svrv');
update mtcl set mtcl_title = 'Service Report', mtcl_order_by = 'order by svrv_ts_service'  where mtcl_name = 'svrv';
update mtat set mtat_input_method = 'RO' where mtat_class_name = 'svrv';
update mtat set mtat_view_order = 1, mtat_displen =  8, mtat_title = 'Item' where mtat_name = 'svrv_serv_code';
update mtat set mtat_view_order = 2, mtat_displen = 10, mtat_title = 'Date' where mtat_name = 'svrv_date_service';
update mtat set mtat_view_order = 3, mtat_displen = 12, mtat_title = 'Patient' where mtat_name = 'svrv_patn_name';
update mtat set mtat_view_order = 4, mtat_displen = 12, mtat_title = 'Suburb' where mtat_name = 'svrv_patn_suburb';
update mtat set mtat_view_order = 5, mtat_displen = 10, mtat_title = 'DOB' where mtat_name = 'svrv_patn_dob';
update mtat set mtat_view_order = 6, mtat_displen = 15, mtat_title = 'Description' where mtat_name = 'svrv_desc';
update mtat set mtat_view_order = 7, mtat_displen = 10, mtat_title = 'Amount' where mtat_name = 'svrv_amount';
update mtat set mtat_view_order = 8, mtat_displen =  6, mtat_title = 'GST' where mtat_name = 'svrv_gst_amount';
update mtat set mtat_view_order = 9, mtat_displen = 10, mtat_title = 'Total' where mtat_name = 'svrv_total_amount';


-- update history;
--insert into mttb(mttb_name, mttb_rcs_header) values('svrv','$Id$');


