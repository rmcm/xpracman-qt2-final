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



create table hpca                       -- HIC Patient Claim Archive
(
  hpca_hpcl__sequence integer,                  -- hpcl__sequence
  hpca_activity_state text,                     -- state of claim
  hpca_transaction_id text,                     -- HIC-assigned claim id
  hpca_message text,                            -- HIC claim message (xml)
 
 --
 -- standard headers
 --
 hpca__sequence serial not null,                        -- Row-ID
 hpca__timestamp timestamp default now() not null,      -- Timestamp
 hpca__user_entry text default CURRENT_USER not null,   -- Entered by
 hpca__status char default text 'N' not null,           -- Record status
 --
 -- constraints
 --
 primary key(hpca__sequence)
 );

grant select,insert,update on hpca to public;
grant select,insert,update on hpca_hpca__sequence_seq to public;

select create_metadata('hpca');
update mtcl set mtcl_title = 'HIC Patient Claim Archive' where mtcl_name = 'hpca';

update mtat set mtat_input_method = 'RO' where mtat_class_name = 'hpca';

update mtat set mtat_title = 'Transaction ID', mtat_displen = 15 where mtat_name = 'hpca_transactionid';

-- update history;
\set mttb_name '\'hpca\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

