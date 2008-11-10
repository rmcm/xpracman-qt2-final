--  -*- sql -*-
--
--     $Id$

--   Copyright 2001 X=X Computer Software Trust
--    		  Kangaroo Ground Australia 3097


--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public Licensepublished by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
 
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
 
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
--  Report problems and direct all questions to:
 
--      Rex McMaster, rmcm@compsoft.com.au
 

 
--  Revision Log
  


drop view surv;
create view surv                       -- Surgical summary
 as select
 --
 -- patn
 --  
 patn_psnam                               as surv_psnam,
 patn_fsnam                               as surv_fsnam,
 short_date(patn_dob)                     as surv_dob,
 --
 -- surg
 --
 surg_symptoms                            as surv_symptoms,
 surg_class                               as surv_class,
 surg_diag_codes                          as surv_diag_codes,
 surg_diagnosis                           as surv_diagnosis,
 surg_treatment                           as surv_treatment,
 surg_oprn_codes                          as surv_oprn_codes,
 surg_operation                           as surv_operation,
 surg_oprn_date                           as surv_oprn_date,
 surg_oprn_place                          as surv_oprn_place,
 surg_photo_code                          as surv_photo_code,
 --                                       
 -- standard headers                      
 --                                       
 surg__sequence                           as surv__sequence,
 surg__timestamp                          as surv__timestamp,
 surg__user_entry                         as surv__user_entry,
 surg__status                             as surv__status

from surg LEFT JOIN patn ON (surg.surg_patn__sequence = patn.patn__sequence);

grant select,insert,update on surv to public;

drop rule rl_surv_update;
create rule rl_surv_update as on update
       to surv
       do instead
       (
         -- 
         update surg
         set    surg_symptoms           = new.surv_symptoms,
                surg_class              = new.surv_class,
                surg_diag_codes         = new.surv_diag_codes,
                surg_diagnosis          = new.surv_diagnosis,
                surg_treatment          = new.surv_treatment,
                surg_oprn_codes         = new.surv_oprn_codes,
                surg_operation          = new.surv_operation,
                surg_oprn_date          = new.surv_oprn_date,
                surg_oprn_place         = new.surv_oprn_place,
                surg_photo_code         = new.surv_photo_code
         where  surg__sequence          = new.surv__sequence;
       );

drop rule rl_surv_delete;
create rule rl_surv_delete as on delete
       to surv
       do instead
       (
         delete from surg
         where  surg__sequence       = old.surv__sequence;
       );

select create_metadata('surv');
update mtat set mtat_input_method = 'RO' where mtat_name ~ '^surv_*';
update mtat set mtat_input_method = 'SYSTEM' where mtat_name ~ '^surv__*';
update mtat set mtat_input_method = 'RO', mtat_displen = 10 where mtat_name = 'surv__timestamp';
update mtat set mtat_input_method = '10', mtat_displen = 15 where mtat_name in (
        'surv_psnam',
        'surv_fsnam',
        'surv_dob',
        'surv_symptoms',
        'surv_class',
        'surv_diag_codes',
        'surv_diagnosis',
        'surv_treatment',
        'surv_oprn_codes',
        'surv_operation',
        'surv_oprn_date',
        'surv_oprn_place',
        'surv_photo_code');
update mtat set mtat_title = 'Surname', mtat_view_order = 2 where mtat_name = 'surv_psnam';
update mtat set mtat_title = 'Firstname', mtat_view_order = 4 where mtat_name = 'surv_fsnam';
update mtat set mtat_title = 'DOB', mtat_view_order = 6 where mtat_name = 'surv_symptoms';
update mtat set mtat_title = 'Symptoms', mtat_view_order = 10 where mtat_name = 'surv_symptoms';
update mtat set mtat_title = 'Class', mtat_view_order = 20 where mtat_name = 'surv_class';
update mtat set mtat_title = 'Diagnosis Codes', mtat_view_order = 30 where mtat_name = 'surv_diag_codes';
update mtat set mtat_title = 'Diagnosis', mtat_view_order = 40 where mtat_name = 'surv_diagnosis';
update mtat set mtat_title = 'Treatment', mtat_view_order = 50 where mtat_name = 'surv_treatment';
update mtat set mtat_title = 'Operation Codes', mtat_view_order = 60 where mtat_name = 'surv_oprn_codes';
update mtat set mtat_title = 'Operation', mtat_view_order = 70 where mtat_name = 'surv_operation';
update mtat set mtat_title = 'Op Date', mtat_view_order = 80 where mtat_name = 'surv_oprn_date';
update mtat set mtat_title = 'Op Place', mtat_view_order = 90 where mtat_name = 'surv_oprn_place';
update mtat set mtat_title = 'Photo', mtat_view_order = 100 where mtat_name = 'surv_photo_code';

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('surv','$Id:');
