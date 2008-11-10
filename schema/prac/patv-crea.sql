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

drop view patv;
create or replace view patv               -- Patient Demographics View
 as select
 patn_flno                                as patv_flno,
 patn_psnam                               as patv_psnam,
 patn_fsnam                               as patv_fsnam,
 patn_title                               as patv_title,
 short_date(patn_dob)                     as patv_dob,
 patn_address                             as patv_address,
 patn_suburb                              as patv_suburb,
 patn_state                               as patv_state,
 patn_postcode                            as patv_postcode,
 patn_phone                               as patv_phone,
 patn_phone_1                             as patv_phone_1,
 patn_phone_2                             as patv_phone_2,
 patn_amount_outstanding                  as patv_amount_outstanding,
 patn_last_visit                          as patv_last_visit,
 patn_last_service                        as patv_last_service,
 patn_claim_number                        as patv_claim_number,
 patn_accident_date                       as patv_accident_date,
 --
 -- health fund
 --
 patn_hlfd_code                           as patv_hlfd_code,
 hlfd_group                               as patv_hlfd_group,
 hlfd_desc                                as patv_hlfd_desc,
 patn_ins_level                           as patv_ins_level,
 patn_healthnumb                          as patv_healthnumb,
 --
 -- Feet type
 --
 patn_feet_code                           as patv_feet_code,
 feet_desc                                as patv_feet_desc,

 patn_medicare                            as patv_medicare,
 patn_healthcard                          as patv_healthcard,
 --
 -- Debtor
 --
 patn_dbtr_code                           as patv_dbtr_code,
 dbtr_name                                as patv_dbtr_name,
 dbtr_address                             as patv_dbtr_address,
 dbtr_suburb                              as patv_dbtr_suburb,
 dbtr_postcode                            as patv_dbtr_postcode,
 dbtr_state                               as patv_dbtr_state,
 dbtr_phone                               as patv_dbtr_phone,
 --
 -- Employer
 --
 patn_empl_code                           as patv_empl_code,
 empl_name                                as patv_empl_name,
 empl_address                             as patv_empl_address,
 empl_suburb                              as patv_empl_suburb,
 empl_postcode                            as patv_empl_postcode,
 empl_state                               as patv_empl_state,
 empl_phone                               as patv_empl_phone,
 empl_notes                               as patv_empl_notes,
 --
 -- referrer.
 -- 
 patn_rfdr_code                           as patv_rfdr_code,
 rfdr_name                                as patv_rfdr_name,
 rfdr_street                              as patv_rfdr_street,
 rfdr_street_1                            as patv_rfdr_st_1,
 rfdr_street_2                            as patv_rfdr_st_2,
 rfdr_suburb                              as patv_rfdr_suburb,
 rfdr_postcode                            as patv_rfdr_postcode,
 rfdr_state                               as patv_rfdr_state,
 rfdr_provider                            as patv_rfdr_provider,
 rfdr_phone                               as patv_rfdr_phone,
 rfdr_salutation                          as patv_rfdr_salutation,
 rfdr_index                               as patv_rfdr_index,
 short_date(patn_ref_date)                as patv_ref_date,
 patn_ref_period                          as patv_ref_period,
 --
 -- Provider
 --
 patn_prov_code                           as patv_prov_code,
 prov_provider_num                        as patv_prov_provider_num,
 prov_name                                as patv_prov_name,
 prov_address                             as patv_prov_address,
 prov_suburb                              as patv_prov_suburb,
 prov_state                               as patv_prov_state,
 prov_postcode                            as patv_prov_postcode,
 prov_salutation                          as patv_prov_salutation,
 prov_phone                               as patv_prov_phone,
 prov_bank_code                           as patv_prov_bank_code,

 patn_country                             as patv_country,
 patn_aboriginality                       as patv_aboriginality,
 patn_sex                                 as patv_sex,
 patn_marital                             as patv_marital,
 patn_accl_code                           as patv_accl_code,
 patn_accommodation                       as patv_accommodation,
 patn_care                                as patv_care,
 --
 -- standard headers
 --
 patn__sequence                           as patv__sequence,
 patn__timestamp                          as patv__timestamp,
 patn__user_entry                         as patv__user_entry,
 patn__status                             as patv__status

from ((((((patn LEFT JOIN rfdr ON (patn.patn_rfdr_code = rfdr.rfdr_code))
                LEFT JOIN hlfd ON (patn.patn_hlfd_code = hlfd.hlfd_code))
                LEFT JOIN feet ON (patn.patn_feet_code = feet.feet_code))
                LEFT JOIN dbtr ON (patn.patn_dbtr_code = dbtr.dbtr_code))
                LEFT JOIN empl ON (patn.patn_empl_code = empl.empl_code))
                LEFT JOIN prov ON (patn.patn_prov_code = prov.prov_code));

grant select on patv to public;

select create_metadata('patv');

-- update history;
\set mttb_name '\'patv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

