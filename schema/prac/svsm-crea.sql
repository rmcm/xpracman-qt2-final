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
  
create table svsm                       -- Summary of Services performed
(
 svsm_date_start timestamp default now() not null,                 -- Period start date
 svsm_date_end timestamp default now() not null,                   -- Period end date
 svsm_type text default '-' not null,                                   -- Aggregate type
 svsm_prov_code text default text '-' not null,                         -- Provider Code
 svsm_prov_name text,                                                   -- Provider Name
 svsm_count integer default 0 not null,                                 -- Count
 svsm_serv_code text default text '-' not null,                         -- Service code
 svsm_desc text,                                                        -- Description
 svsm_amount numeric(12,2) default numeric(12,2) '0.0' not null,        -- Total Amount
 svsm_gst_amount numeric(12,2) default numeric(12,2) '0.0' not null,    -- Total GST amount

 --
 -- standard headers
 --
 svsm__sequence serial not null,                              -- Row-ID
 svsm__timestamp timestamp default now() not null,       -- Timestamp
 svsm__user_entry text default CURRENT_USER not null,         -- Entered by
 svsm__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key (svsm__sequence),
 constraint svsm_fk_prov
            foreign key(svsm_prov_code)
            references prov
            on update cascade
            on delete restrict
            initially deferred,
 constraint svsm_fk_serv
            foreign key (svsm_serv_code) references serv
            on update cascade
            on delete restrict
            initially deferred

 );


grant select,insert,update,delete on svsm to public;
grant select,insert,update,delete on svsm_svsm__sequence_seq to public; 

select create_metadata('svsm');
update mtcl set mtcl_title = 'Service Summary' where mtcl_name = 'svsm';

-- update history;
\set mttb_name '\'svsm\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

