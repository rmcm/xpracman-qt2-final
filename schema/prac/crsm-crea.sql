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
  
create table crsm                       -- Summary of Credits
(
 crsm_date_start timestamp default now() not null,                 -- Period start date
 crsm_date_end timestamp default now() not null,                   -- Period end date
 crsm_type text default '-' not null,                                   -- Aggregate type
 crsm_prov_code text default text '-' not null,                         -- Provider Code
 crsm_prov_name text,                                                   -- Provider Name
 crsm_count integer default 0 not null,                                 -- Count
 crsm_tdtp_code text default text '-' not null,                         -- Tender code
 crsm_desc text,                                                        -- Description
 crsm_amount numeric(12,2) default numeric(12,2) '0.0' not null,        -- Total Amount
 crsm_gst_amount numeric(12,2) default numeric(12,2) '0.0' not null,    -- Total GST amount

 --
 -- standard headers
 --
 crsm__sequence serial not null,                              -- Row-ID
 crsm__timestamp timestamp default now() not null,       -- Timestamp
 crsm__user_entry text default CURRENT_USER not null,         -- Entered by
 crsm__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key (crsm__sequence),
 constraint crsm_fk_prov
            foreign key(crsm_prov_code)
            references prov
            on update cascade
            on delete restrict
            initially deferred,
 constraint crsm_fk_tdtp
            foreign key (crsm_tdtp_code) references tdtp
            on update cascade
            on delete restrict
            initially deferred

 );


grant select,insert,update,delete on crsm to public;
grant select,insert,update,delete on crsm_crsm__sequence_seq to public; 

select create_metadata('crsm');
update mtcl set mtcl_title = 'Credit Summary' where mtcl_name = 'crsm';

-- update history;
\set mttb_name '\'crsm\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
