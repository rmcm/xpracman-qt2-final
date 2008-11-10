-- -*- sql -*-
 
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
  
--  

create table cash                                          -- Cash Assignments
(
 cash_paym__sequence integer default 0 not null,           -- Payment ID
 cash_bank_code text default text '-' not null,            -- Creditor
 cash_date timestamp,                                      -- Date of assignment
 cash_amount numeric(12,2) default numeric(12,2) '0.0',    -- Amount
 cash_text text,                                           -- Annotation
 --
 -- standard headers
 --
 cash__sequence serial not null,                           -- Row-ID
 cash__timestamp timestamp default now() not null,    -- Timestamp
 cash__user_entry text default CURRENT_USER not null,      -- Entered by
 cash__status char default text 'N' not null,              -- Record status
 --
 -- constraints
 --
 primary key(cash__sequence)
 );

grant select,insert,update on cash to public;
grant select,insert,update on cash_cash__sequence_seq to public;

select create_metadata('cash');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('cash','$Id:');
