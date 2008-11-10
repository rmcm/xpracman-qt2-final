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
  
create table paym                       -- Payments Received
(
 paym_date_entry timestamp default now() not null,           -- Date received
 paym_user_entry text default CURRENT_USER not null,              -- Received by
 paym_site_entry text,                                            -- Reception site
 paym_amount numeric(12,2) default numeric(12,2) '0.00' not null, -- Amount
 paym_tdtp_code text default text '-' not null,                   -- Tender type
 paym_drawer text,                                                -- Drawer
 paym_bank text,                                                  -- Bank
 paym_branch text,                                                -- Branch
 paym_bkdp__sequence integer default 0 not null,                  -- Deposit-batch code
 --
 -- standard headers
 --
 paym__sequence serial not null,                         -- Row-ID
 paym__timestamp timestamp default now() not null,   -- Timestamp
 paym__user_entry text default CURRENT_USER not null,    -- Entered by
 paym__status char default text 'N' not null,            -- Record status
 --
 -- constraints
 --
 primary key (paym__sequence)
 );


create index ix_paym_1 on paym(paym_bkdp__sequence);
create index ix_paym_2 on paym(paym_date_entry);

grant select,insert,update on paym to public;
-- this has to be granted to all the view CREV to function properly
grant select,insert,update on paym_paym__sequence_seq to public;

select create_metadata('paym');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('paym','$Id:');
