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

create or replace view evnv               -- Patient Demographics View
 as select
 --
 -- Events
 --
 evnt_patn__sequence                      as evnv_patn__sequence,
 evnt_prov_code                           as evnv_prov_code,
 evnt_rfdr_code                           as evnv_rfdr_code,
 evnt_locn_code                           as evnv_locn_code,
 evnt_aptp_code                           as evnv_aptp_code,
 evnt_apst_code                           as evnv_apst_code,
 evnt_starttime                           as evnv_starttime,
 evnt_duration                            as evnv_duration,
 evnt_desc                                as evnv_desc,
 evnt_note_1                              as evnv_note_1,
 evnt_note_2                              as evnv_note_2,
 evnt_note_3                              as evnv_note_3,
 evnt_note_4                              as evnv_note_4,
 evnt_note_5                              as evnv_note_5,
 evnt_note_6                              as evnv_note_6,
 evnt_note_7                              as evnv_note_7,
 evnt_note_8                              as evnv_note_8,
 evnt_note_9                              as evnv_note_9,
 to_char(evnt_starttime,'HH:MI am')       as evnv_ev_time,
 to_char(evnt_starttime,'Day DD-MM-YYYY') as evnv_ev_date_str,
 date(evnt_starttime)                     as evnv_ev_date,
 extract(hour from evnt_duration)*60
   + extract(minute from evnt_duration)   as evnv_ev_minutes,
 --
 -- Patient
 --
 patn_flno                                as evnv_patn_flno,
 coalesce(patn_psnam,'') || ', ' || coalesce(patn_fsnam,'')
                                          as evnv_patn_desc,
 coalesce(patn_psnam,'') || ', ' || coalesce(patn_fsnam,'') || '<br>Ph:' || coalesce(patn_phone,'')
   || '<br>Dob:' || coalesce(short_date(patn_dob),'') || '<br>' || coalesce(evnt_desc, '')
                                          as evnv_patn_desc_comp,
 patn_psnam                               as evnv_patn_psnam,
 patn_fsnam                               as evnv_patn_fsnam,
 patn_title                               as evnv_patn_title,
 short_date(patn_dob)                     as evnv_patn_dob,
 patn_address                             as evnv_patn_address,
 patn_suburb                              as evnv_patn_suburb,
 patn_state                               as evnv_patn_state,
 patn_postcode                            as evnv_patn_postcode,
 coalesce(patn_address,'')
 || ', ' || coalesce(patn_suburb,'')
 || ' ' || coalesce(patn_postcode,'')     as evnv_patn_full_address,
 patn_phone                               as evnv_patn_phone,
 patn_phone_1                             as evnv_patn_phone_1,
 patn_phone_2                             as evnv_patn_phone_2,
 patn_ref_date                            as evnv_patn_ref_date,
 patn_ref_period                          as evnv_patn_ref_period,
 referral_expires(patn_ref_date, patn_ref_period, patn__sequence)
                                          as evnv_patn_referral_expires,
 date_trunc('min',patn_last_visit)        as evnv_patn_last_visit,
 date_trunc('min', patn_last_service)     as evnv_patn_last_service,
 patn_amount_outstanding                  as evnv_patn_amount_outstanding,
 patn_fax_number                          as evnv_patn_fax_number,
 patn_email_address                       as evnv_patn_email_address,
 --
 -- Location
 --
 locn_desc                                as evnv_locn_desc,
 locn_fax_number                          as evnv_locn_fax_number,
 locn_email_address                       as evnv_locn_email_address,
 --
 -- Appointment Type
 --
 aptp_desc                                as evnv_aptp_desc,
 --
 -- Appointment progress
 --
 apst_desc                                as evnv_apst_desc,
 --
 -- Provider
 --
 prov_provider_num                        as evnv_prov_provider_num,
 prov_name                                as evnv_prov_name,
 prov_address                             as evnv_prov_address,
 prov_suburb                              as evnv_prov_suburb,
 prov_state                               as evnv_prov_state,
 prov_postcode                            as evnv_prov_postcode,
 prov_salutation                          as evnv_prov_salutation,
 prov_phone                               as evnv_prov_phone,
 prov_bank_code                           as evnv_prov_bank_code,
 prov_fax_number                          as evnv_prov_fax_number,
 prov_email_address                       as evnv_prov_email_address,
 --
 -- Referrer
 -- 
 rfdr_name                                as evnv_rfdr_name,
 rfdr_street                              as evnv_rfdr_street,
 rfdr_suburb                              as evnv_rfdr_suburb,
 rfdr_postcode                            as evnv_rfdr_postcode,
 rfdr_state                               as evnv_rfdr_state,
 rfdr_provider                            as evnv_rfdr_provider,
 rfdr_phone                               as evnv_rfdr_phone,
 rfdr_salutation                          as evnv_rfdr_salutation,
 rfdr_index                               as evnv_rfdr_index,
 rfdr_fax_number                          as evnv_rfdr_fax_number,
 rfdr_email_address                       as evnv_rfdr_email_address,
 --
 -- Rest of patn
 -- 
 patn_feet_code                           as evnv_patn_feet_code,
 patn_hlfd_code                           as evnv_patn_hlfd_code,
 patn_dbtr_code                           as evnv_patn_dbtr_code,
 hlfd_desc                                as evnv_patn_hlfd_desc,
 patn_ins_level                           as evnv_patn_ins_level,
 patn_healthnumb                          as evnv_patn_healthnumb,
 patn_medicare                            as evnv_patn_medicare,
 patn_healthcard                          as evnv_patn_healthcard,
 patn_country                             as evnv_patn_country,
 patn_aboriginality                       as evnv_patn_aboriginality,
 patn_sex                                 as evnv_patn_sex,
 patn_marital                             as evnv_patn_marital,
 patn_accl_code                           as evnv_patn_accl_code,
 patn_accommodation                       as evnv_patn_accommodation,
 patn_care                                as evnv_patn_care,
 patn_patf_code                           as evnv_patn_patf_code,
 --
 -- Health Fund
 --
 hlfd_group                               as evnv_hlfd_group,
 hlfd_desc                                as evnv_hlfd_desc,
 hlfd_fax_number                          as evnv_hlfd_fax_number,
 hlfd_email_address                       as evnv_hlfd_email_address,
 --
 -- Debtor
 --
 dbtr_name                                as evnv_dbtr_name,
 dbtr_fax_number                          as evnv_dbtr_fax_number,
 dbtr_email_address                       as evnv_dbtr_email_address,
 --
 -- Local variations
 --

 --
 -- standard headers
 --
 evnt__sequence                           as evnv__sequence,
 evnt__timestamp                          as evnv__timestamp,
 evnt__user_entry                         as evnv__user_entry,
 evnt__status                             as evnv__status

from ((((((((evnt LEFT JOIN patn ON (evnt.evnt_patn__sequence = patn.patn__sequence))
                  LEFT JOIN prov ON (evnt.evnt_prov_code = prov.prov_code))
                  LEFT JOIN rfdr ON (evnt.evnt_rfdr_code = rfdr.rfdr_code))
                  LEFT JOIN locn ON (evnt.evnt_locn_code = locn.locn_code))
                  LEFT JOIN aptp ON (evnt.evnt_aptp_code = aptp.aptp_code))
                  LEFT JOIN apst ON (evnt.evnt_apst_code = apst.apst_code))
                  LEFT JOIN dbtr ON (patn.patn_dbtr_code = dbtr.dbtr_code))
                  LEFT JOIN hlfd ON (patn.patn_hlfd_code = hlfd.hlfd_code));

grant select,insert,update on evnv to public;

-- and now - an update rule - should be here in crea file, as is dropped when view is dropped.
-- (creating view should be last procedure - ie after functions)
-- The insert is handled by evnv_insert_row() in evnv-functions.sql

create or replace rule rl_evnv_update as on update
       to evnv
       do instead
       (
         update evnt
         set    evnt_patn__sequence  = new.evnv_patn__sequence,
                evnt_prov_code       = new.evnv_prov_code,
                evnt_rfdr_code       = new.evnv_rfdr_code,
                evnt_locn_code       = new.evnv_locn_code,
                evnt_aptp_code       = new.evnv_aptp_code,
                evnt_apst_code       = new.evnv_apst_code,
                evnt_starttime       = new.evnv_starttime,
                evnt_duration        = new.evnv_duration,
                evnt_desc            = new.evnv_desc,
                evnt_note_1          = new.evnv_note_1,
                evnt_note_2          = new.evnv_note_2,
                evnt_note_3          = new.evnv_note_3,
                evnt_note_4          = new.evnv_note_4,
                evnt_note_5          = new.evnv_note_5,
                evnt_note_6          = new.evnv_note_6,
                evnt_note_7          = new.evnv_note_7,
                evnt_note_8          = new.evnv_note_8,
                evnt_note_9          = new.evnv_note_9
         where  evnt__sequence       = new.evnv__sequence;
         update patn
         set    patn_psnam           = new.evnv_patn_psnam,
                patn_fsnam           = new.evnv_patn_fsnam,
                patn_title           = new.evnv_patn_title,
                patn_dob             = new.evnv_patn_dob::timestamp,
                patn_address         = new.evnv_patn_address,
                patn_suburb          = new.evnv_patn_suburb,
                patn_postcode        = new.evnv_patn_postcode,
                patn_phone           = new.evnv_patn_phone,
                patn_phone_1         = new.evnv_patn_phone_1,
                patn_phone_2         = new.evnv_patn_phone_2,
                patn_rfdr_code       = new.evnv_rfdr_code,
                patn_ref_date        = new.evnv_patn_ref_date,
                patn_ref_period      = new.evnv_patn_ref_period,
                patn_flno            = new.evnv_patn_flno,
                patn_feet_code       = new.evnv_patn_feet_code,
                patn_hlfd_code       = new.evnv_patn_hlfd_code
         where  patn__sequence       = new.evnv_patn__sequence
         and    new.evnv_patn__sequence > 0;
       );

create or replace rule rl_evnv_delete as on delete
       to evnv
       do instead
       (
         delete from evnt
         where  evnt__sequence       = old.evnv__sequence;
       );

create or replace rule rl_evnv_insert as on insert
       to evnv
       do instead
       (
         select evnv_insert_row();
       );

select create_metadata('evnv');

grant select, insert, update, delete on evnv to public;



-- update history;

\set mttb_name '\'evnv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
