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
  
create table svpf                       -- Services performed
(
 svpf_date_service timestamp default now() not null,               -- Date of service
 svpf_serv_code text default text '-' not null,                         -- Service code
 svpf_percentage integer default 100 not null,                     -- Fee percentage
 svpf_desc text,                                                        -- Description
 svpf_amount numeric(12,2) default numeric(12,2) '0.0' not null,        -- Amount
 svpf_gst_amount numeric(12,2) default numeric(12,2) '0.0' not null,    -- GST amount
 svpf_invc__sequence integer default 0 not null,                        -- Invoice number
 svpf_mdaf__sequence integer default 0 not null,                        -- Medicare form number
 svpf_patn__sequence integer default 0 not null,                        -- Patient
 --
 -- standard headers
 --
 svpf__sequence serial not null,                              -- Row-ID
 svpf__timestamp timestamp default now() not null,       -- Timestamp
 svpf__user_entry text default CURRENT_USER not null,         -- Entered by
 svpf__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key (svpf__sequence)
 );


create index ix_svpf_patn on svpf(svpf_patn__sequence);
create index ix_svpf_invc on svpf(svpf_invc__sequence);
create index ix_svpf_serv on svpf(svpf_serv_code);

grant select,insert,update on svpf to public;
grant select,insert,update on svpf_svpf__sequence_seq to public; 

select create_metadata('svpf');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('svpf','$Id:');
