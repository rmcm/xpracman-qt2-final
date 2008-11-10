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

drop view evwl;
create view evwl                       -- Patient Demographics View
 as select
 --
 -- Events
 --
 evnt_prov_code                           as evwl_prov_code,
 evnt_aptp_code                           as evwl_aptp_code,
 evnt_apst_code                           as evwl_apst_code,
 evnt_desc                                as evwl_desc,
 to_char(evnt_starttime,'HH:MI am')       as evwl_ev_time,
 to_char(evnt_starttime,'DD-MM-YYYY')     as evwl_ev_date,
 extract(hour from evnt_duration)*60
   + extract(minute from evnt_duration)   as evwl_ev_minutes,
 --
 -- Patient
 --
 patn_flno                                as evwl_patn_flno,
 patn_psnam||', '||patn_fsnam             as evwl_patn_desc,
 patn_patf_code                           as evwl_patf_code,
 short_date(patn_dob)                     as evwl_patn_dob,
 patn_last_visit                          as evwl_patn_last_visit,
 patn_amount_outstanding                  as evwl_patn_amount_outstanding,
 patn__sequence                           as evwl_patn__sequence,
 --
 -- Referrer
 --
 rfdr_name                                as evwl_rfdr_name,

 -- hidden, used for sorting
 evnt_starttime                           as evwl_starttime,
 --
 -- standard headers
 --
 evnt__sequence                           as evwl__sequence,
 evnt__timestamp                          as evwl__timestamp,
 evnt__user_entry                         as evwl__user_entry,
 evnt__status                             as evwl__status

from ((evnt LEFT JOIN patn ON (evnt.evnt_patn__sequence = patn.patn__sequence))
                LEFT JOIN rfdr ON (evnt.evnt_rfdr_code = rfdr.rfdr_code));

grant select,insert,update on evwl to public;

-- and now - an update rule - should be here in crea file, as is dropped when view is dropped.
-- (creating view should be last procedure - ie after functions)
-- The insert is handled by evwl_insert_row() in evwl-functions.sql

drop rule rl_evwl_update;
create rule rl_evwl_update as on update
       to evwl
       do instead
       (
         update evnt
         set    evnt_apst_code       = new.evwl_apst_code,
                evnt_desc            = new.evwl_desc
         where  evnt__sequence       = new.evwl__sequence;
       );

select create_metadata('evwl');
update mtcl set mtcl_title = 'Waiting List', mtcl_order_by = 'order by evwl_starttime' where mtcl_name = 'evwl';
update mtat set mtat_view_order = 010, mtat_browse_order = 005, mtat_title = 'Prov', mtat_displen = 5, mtat_input_method = '' where mtat_name = 'evwl_prov_code';
update mtat set mtat_view_order = 010, mtat_browse_order = 010, mtat_title = 'Type', mtat_displen = 5, mtat_input_method = '' where mtat_name = 'evwl_aptp_code';
update mtat set mtat_view_order = 020, mtat_browse_order = 020, mtat_title = 'Status', mtat_displen = 5, mtat_input_method = '' where mtat_name = 'evwl_apst_code';
update mtat set mtat_view_order = 020, mtat_browse_order = 020, mtat_title = 'Flag', mtat_displen = 10, mtat_input_method = 'SYSTEM' where mtat_name = 'evwl_patf_code';
update mtat set mtat_view_order = 030, mtat_browse_order = 030, mtat_title = 'Notes', mtat_displen = 30, mtat_input_method = '' where mtat_name = 'evwl_desc';
update mtat set mtat_view_order = 040, mtat_browse_order = 040, mtat_title = 'Time', mtat_displen = 5, mtat_input_method = '' where mtat_name = 'evwl_ev_time';
update mtat set mtat_view_order = 050, mtat_browse_order = 050, mtat_title = 'Date', mtat_displen = 12, mtat_input_method = '' where mtat_name = 'evwl_ev_date';
update mtat set mtat_view_order = 060, mtat_browse_order = 060, mtat_title = 'Duration', mtat_displen = 5, mtat_input_method = '' where mtat_name = 'evwl_ev_minutes';
update mtat set mtat_view_order = 070, mtat_browse_order = 070, mtat_title = 'UR', mtat_displen = 12, mtat_input_method = 'SYSTEM' where mtat_name = 'evwl_patn_flno';
update mtat set mtat_view_order = 080, mtat_browse_order = 080, mtat_title = 'ID', mtat_displen = 12, mtat_input_method = 'RO' where mtat_name = 'evwl_patn__sequence';
update mtat set mtat_view_order = 090, mtat_browse_order = 090, mtat_title = 'Name', mtat_displen = 20, mtat_input_method = 'RO' where mtat_name = 'evwl_patn_desc';
update mtat set mtat_view_order = 110, mtat_browse_order = 110, mtat_title = 'DOB', mtat_displen = 12, mtat_input_method = 'RO' where mtat_name = 'evwl_patn_dob';
update mtat set mtat_view_order = 120, mtat_browse_order = 120, mtat_title = 'Last Visit', mtat_displen = 12, mtat_input_method = 'SYSTEM' where mtat_name = 'evwl_patn_last_visit';
update mtat set mtat_view_order = 130, mtat_browse_order = 130, mtat_title = 'Owing', mtat_displen = 10, mtat_input_method = 'SYSTEM' where mtat_name = 'evwl_patn_amount_outstanding';
update mtat set mtat_view_order = 140, mtat_browse_order = 140, mtat_title = 'Referrer', mtat_displen = 5, mtat_input_method = 'SYSTEM' where mtat_name = 'evwl_rfdr_name';
update mtat set mtat_view_order = 150, mtat_browse_order = 150, mtat_title = 'Time', mtat_displen = 30, mtat_input_method = 'SYSTEM' where mtat_name = 'evwl_starttime';
update mtat set mtat_view_order = 160, mtat_browse_order = 160, mtat_title = 'ID', mtat_displen = 10, mtat_input_method = 'SYSTEM' where mtat_name = 'evwl__sequence';
update mtat set mtat_view_order = 170, mtat_browse_order = 170, mtat_title = 'Created', mtat_displen = 20, mtat_input_method = 'SYSTEM' where mtat_name = 'evwl__timestamp';
update mtat set mtat_view_order = 180, mtat_browse_order = 180, mtat_title = 'Creator', mtat_displen = 10, mtat_input_method = 'SYSTEM' where mtat_name = 'evwl__user_entry';
update mtat set mtat_view_order = 190, mtat_browse_order = 190, mtat_title = 'Internal', mtat_displen = 1, mtat_input_method = 'SYSTEM' where mtat_name = 'evwl__status';


-- update history;
\set mttb_name '\'evwl\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;


