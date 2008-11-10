--     $Id$
-- 
--   Copyright 2005 X=X Computer Software Trust
--    		  Kangaroo Ground Australia 3097
-- 
-- 
--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License published by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
--  
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--  
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
--  
--  Report problems and direct all questions to:
--  
--      Rex McMaster, rmcm@compsoft.com.au
--  
-- 



create table svmc                       -- Medicare Claim Results against service
(
 svmc_svpf__sequence integer,                           -- foreign key to svpf table
 svmc_rsn_code text,                                    -- RSN code
 svmc_benefit_amount numeric(12,2) DEFAULT 0.0,         -- benefit
 --
 -- standard headers
 --
 svmc__sequence serial not null,                              -- Row-ID
 svmc__timestamp timestamp default now() not null,       -- Timestamp
 svmc__user_entry text default CURRENT_USER not null,         -- Entered by
 svmc__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(svmc__sequence),

 constraint svmc_fk_svpf
            foreign key (svmc_svpf__sequence)
            references svpf
            on update cascade
            on delete restrict
            initially deferred
 );

create unique index ix_svnc_svpf on svmc(svmc_svpf__sequence);

grant select,insert,update on svmc to public;
grant select,insert,update on svmc_svmc__sequence_seq to public;

select create_metadata('svmc');
update mtcl set mtcl_title = 'Medicare Claim Results against service' where mtcl_name = 'svmc';

-- update history;
\set mttb_name '\'svmc\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

