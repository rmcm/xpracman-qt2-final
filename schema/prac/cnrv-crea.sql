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
  
drop view cnrv;
create view cnrv                       -- Contracts - view
 as select
 cnrt_patn__sequence                                            as cnrv_patn__sequence,
 patn_patient_name(cnrt_patn__sequence)                         as cnrv_patient_name,
 cnrt_amount                                                    as cnrv_amount,
 ctrt_period                                                    as ctrv_period,
 cnrt_count                                                     as cnrv_count,
 short_date(cnrt_start_date)                                    as cnrv_start_date,
 cnrt_serv_code                                                 as cnrv_serv_code,
 cnrt_first_installment                                         as cnrv_first_installment,
 cnrt_other_installment                                         as cnrv_other_installment,
 short_date(cnrt_last_date)                                     as cnrv_last_date,
 cnrt_balance                                                   as cnrv_balance,
 short_date(cnrt_end_date)                                      as cnrv_end_date,
 -- 
 -- Standard headers
 -- 
 cnrt__sequence                                                 as cnrv__sequence,
 cnrt__timestamp                                                as cnrv__timestamp,
 cnrt__user_entry                                               as cnrv__user_entry,
 cnrt__status                                                   as cnrv__status
 --
 -- constraints
 --

 from cnrt;

grant select,insert,update on cnrv to public;


-- and now - an update rule - chould be in crea file, as is dropped when view is dropped.
-- (creating view should be last procedure - ie after functions)

drop rule rl_cnrv_update;
create rule rl_cnrv_update as on update
       to cnrv
       do instead
       (
         update cnrt
         set    cnrt_patn__sequence     = new.cnrv_patn__sequence,
                cnrt_amount             = new.cnrv_amount,
                ctrt_period             = new.ctrv_period,
                cnrt_count              = new.cnrv_count,
                cnrt_start_date         = new.cnrv_start_date,
                cnrt_serv_code          = new.cnrv_serv_code,
                cnrt_first_installment  = new.cnrv_first_installment,
                cnrt_other_installment  = new.cnrv_other_installment,
                cnrt_last_date          = new.cnrv_last_date,
                cnrt_balance            = new.cnrv_balance,
                cnrt_end_date           = new.cnrv_end_date,
                cnrt__sequence          = new.cnrv__sequence,
                cnrt__timestamp         = new.cnrv__timestamp,
                cnrt__user_entry        = new.cnrv__user_entry,
                cnrt__status            = new.cnrv__status
         where  cnrt__sequence = new.cnrv__sequence;
       );

drop rule rl_cnrv_delete;
create rule rl_cnrv_delete as on delete
       to cnrv
       do instead
       (
         delete from cnrt
         where  cnrt__sequence       = old.cnrv__sequence;
       );

select create_metadata('cnrv');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('cnrv','$Id:');
