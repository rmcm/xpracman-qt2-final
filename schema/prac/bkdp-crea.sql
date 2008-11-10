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
  
--  Initial version

create table bkdp                       -- Bank Deposit Reports
(
 bkdp_date_created timestamp default now(),               -- Date created
 bkdp_date_printed timestamp,                                  -- Date Printed
 bkdp_user_printed text default CURRENT_USER,                 -- Printed by user
 bkdp_bank_code text default text '-' not null,               -- Creditor
 bkdp_desc text default text 'New Batch',                     -- Description for batch
 bkdp_amount numeric(12,2) default numeric(12,2) '0.00',      -- Batch total
 --
 -- standard headers
 --
 bkdp__sequence serial not null,                              -- Row-ID
 bkdp__timestamp timestamp default now() not null,        -- Timestamp
 bkdp__user_entry text default CURRENT_USER not null,         -- Entered by
 bkdp__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key (bkdp__sequence)
 );

grant select,insert,update on bkdp to public;
grant select,insert,update on bkdp_bkdp__sequence_seq to public;

select create_metadata('bkdp');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('bkdp','$Id:');
