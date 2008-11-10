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

drop view notv;
create view notv                       -- Patient Demographics View
 as select
 --
 -- Events
 --
 note_patn__sequence                      as notv_patn__sequence,
 to_char(note__timestamp,'Day DD-MM-YYYY HH:MI am') as notv_time,
 note_desc                                as notv_desc,
 --
 -- Patient
 --
 patn_flno                                as notv_patn_flno,
 patn_psnam||', '||patn_fsnam             as notv_patn_desc,
 patn_psnam                               as notv_patn_psnam,
 patn_fsnam                               as notv_patn_fsnam,
 patn_title                               as notv_patn_title,
 short_date(patn_dob)                     as notv_patn_dob,
 patn_address                             as notv_patn_address,
 patn_suburb                              as notv_patn_suburb,
 patn_state                               as notv_patn_state,
 patn_postcode                            as notv_patn_postcode,
 patn_phone                               as notv_patn_phone,
 --
 -- Rest of patn
 -- 
 patn_country                             as notv_patn_country,
 patn_aboriginality                       as notv_patn_aboriginality,
 patn_sex                                 as notv_patn_sex,
 patn_marital                             as notv_patn_marital,
 patn_accl_code                           as notv_patn_accl_code,
 patn_accommodation                       as notv_patn_accommodation,
 patn_care                                as notv_patn_care,
 --
 -- standard headers
 --
 note__sequence                           as notv__sequence,
 note__timestamp                          as notv__timestamp,
 note__user_entry                         as notv__user_entry,
 note__status                             as notv__status

from note LEFT JOIN patn ON (note.note_patn__sequence = patn.patn__sequence);

grant select,insert,update on notv to public;

-- and now - an update rule - should be here in crea file, as is dropped when view is dropped.
-- (creating view should be last procedure - ie after functions)
-- The insert is handled by notv_insert_row() in notv-functions.sql

drop rule rl_notv_update;
create rule rl_notv_update as on update
       to notv
       do instead
       (
         update note
         set    note_patn__sequence  = new.notv_patn__sequence,
                note_desc            = new.notv_desc
         where  note__sequence       = new.notv__sequence;
       );

drop rule rl_notv_delete;
create rule rl_notv_delete as on delete
       to notv
       do instead
       (
         delete from note
         where  note__sequence       = old.notv__sequence;
       );

drop rule rl_notv_insert;
create rule rl_notv_insert as on insert
       to notv
       do instead
       (
         select notv_insert_row();
       );

select create_metadata('notv');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('notv','$Id$');

