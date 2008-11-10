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

create table feeb                               -- Fees for services
(               
 feeb_serv_code text not null default text '-',           -- Service code
 feeb_feet_code text not null default text '-',           -- Fee level
 feeb_amount numeric(12,2) default numeric(12,2) '0.00',  -- Amount
 --
 -- standard headers
 --
 feeb__sequence serial not null,                                   -- Row-ID
 feeb__timestamp timestamp default now() not null,             -- Timestamp
 feeb__user_entry text default CURRENT_USER not null,              -- Entered by
 feeb__status char default text 'N' not null,                     -- Record status
 --
 -- constraints
 --
 primary key (feeb__sequence)
 );


create unique index ix_feeb_1 on feeb(feeb_serv_code,feeb_feet_code);

grant select,insert,update on feeb to public;
grant select,insert,update on feeb_feeb__sequence_seq to public;

select create_metadata('feeb');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('feeb','$Id:');
