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
  

create table cred                       -- Credits to Invoices
(
 cred_paym__sequence integer not null,                             -- Payment Serial Number
 cred_invc__sequence integer not null,                             -- Invoice Serial Number
 cred_amount numeric(12,2) default numeric(12,2) '0.00',           -- Credit Amount
 cred_gst_amount numeric(12,2) default numeric(12,2) '0.00',       -- Credit GST Amount
 cred_notes text,                                                  -- Description (notes)
 --
 -- standard headers
 --
 cred__sequence serial not null,                                  -- Row-ID
 cred__timestamp timestamp default now() not null,            -- Timestamp
 cred__user_entry text default CURRENT_USER not null,             -- Entered by
 cred__status char default text 'N' not null,                     -- Record status
 --
 -- constraints
 --
 primary key (cred__sequence)
 );

create index ix_cred_invc on cred(cred_invc__sequence);
create index ix_cred_paym on cred(cred_paym__sequence);

grant select,insert,update on cred to public;
-- this has to be granted to all the view CREV to function properly
grant select,insert,update on cred_cred__sequence_seq to public;

select create_metadata('cred');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('cred','$Id:');
