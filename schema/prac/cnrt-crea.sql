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
  
create table cnrt                       -- Services performed
(
 cnrt_patn__sequence integer default 0 not null,                            -- Patient
 cnrt_amount numeric(12,2) default numeric(12,2) '0.0' not null,            -- Total Contract Amount
 ctrt_period interval default '3 months' not null,                          -- Contract duration
 cnrt_count integer default 3 not null,                                     -- Number of installments
 cnrt_start_date timestamp default now() not null,                     -- Start date
 cnrt_serv_code text default text '-' not null,                             -- Service code
 cnrt_first_installment numeric(12,2) default numeric(12,2) '0.0' not null, -- First Installment amount
 cnrt_other_installment numeric(12,2) default numeric(12,2) '0.0' not null, -- Other Installment amount
 cnrt_last_date timestamp,                                                  -- last installment posted date
 cnrt_balance numeric(12,2) default numeric(12,2) '0.0' not null,           -- Remaining balance on contract
 cnrt_end_date timestamp,                                                   -- End date
 --
 -- standard headers
 --
 cnrt__sequence serial not null,                              -- Row-ID
 cnrt__timestamp timestamp default now() not null,       -- Timestamp
 cnrt__user_entry text default CURRENT_USER not null,         -- Entered by
 cnrt__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key (cnrt__sequence)
 );


create index ix_cnrt_patn on cnrt(cnrt_patn__sequence);
create index ix_cnrt_serv on cnrt(cnrt_serv_code);

grant select,insert,update on cnrt to public;
grant select,insert,update on cnrt_cnrt__sequence_seq to public; 

select create_metadata('cnrt');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('cnrt','$Id$');
