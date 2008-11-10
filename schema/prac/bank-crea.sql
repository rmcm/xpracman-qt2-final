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
  
create table bank                       -- Creditor for invoices/payments
(
 bank_code text not null,        -- Creditor Code
 bank_name text,                 -- Name
 bank_bsb text,                  -- BSB
 bank_account text,              -- Account number/code
 bank_address text,              -- Street
 bank_suburb text,               -- Suburb 
 bank_state text,                -- State
 bank_postcode text,             -- Postcode
 bank_phone text,                -- Phone
 bank_bank text,                 -- Bank
 bank_branch text,               -- Branch
 --
 -- standard headers
 --
 bank__sequence serial not null,                              -- Row-ID
 bank__timestamp timestamp default now() not null,        -- Timestamp
 bank__user_entry text default CURRENT_USER not null,         -- Entered by
 bank__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(bank_code)
 );

grant select,insert,update on bank to public;
grant select,insert,update on bank_bank__sequence_seq to public;


insert into bank(bank_code) values('-');


select create_metadata('bank');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('bank','$Id:');
