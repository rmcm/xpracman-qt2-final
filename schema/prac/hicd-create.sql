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



drop table hicd;
drop sequence hicd_hicd__sequence_seq;
create table hicd                       -- HIC Demographics - clients
(
 hicd_patn__sequence integer,           -- client number
 hicd_location_id text,                 -- locate ID CMP#####
 --
 -- standard headers
 --
 hicd__sequence serial not null,                              -- Row-ID
 hicd__timestamp timestamp default now() not null,       -- Timestamp
 hicd__user_entry text default CURRENT_USER not null,         -- Entered by
 hicd__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(hicd__sequence),
constraint hicd_fk_patn
          foreign key (hicd_patn__sequence) references patn
          on update cascade
          on delete cascade
          initially deferred
 );

grant select,insert,update on hicd to public;
grant select,insert,update on hicd_hicd__sequence_seq to public;

select create_metadata('hicd');
update mtcl set mtcl_title = 'HIC Demographics' where mtcl_name = 'hicd';
select create_rl_links('patn');
select create_fk_links('hicd');

-- update history;
\set mttb_name '\'hicd\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

