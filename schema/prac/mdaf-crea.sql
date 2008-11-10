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
  
create table mdaf                                       -- Medicare Assignment Forms
	(               
  mdaf_patn__sequence integer,                          -- Patient file number
  mdaf_prov_code text,                                  -- Service provider code
  mdaf_rfdr_code text,                                  -- Referrer code
  mdaf_locn_id text,                                    -- Location ID
  mdaf_voucher text,                                    -- Voucher code, external
  mdaf_benefit_authorised text default 'N',             -- Benefit assignment authorised
  mdaf_date_printed timestamp,                          -- Date printed
  mdaf_mdbt__sequence integer,                          -- Batch serial number
 --
 -- standard headers
 --
  mdaf__sequence serial,                                -- Row-ID
  mdaf__timestamp timestamp default now(),              -- Timestamp
  mdaf__user_entry text default CURRENT_USER,           -- Entered by
  mdaf__status char default text 'N',                   -- Record status
 --
 -- constraints
 --
 primary key (mdaf__sequence)
);



grant select,insert,update on mdaf to public;
grant select,insert,update on mdaf_mdaf__sequence_seq to public;

select create_metadata('mdaf');
update mtcl set mtcl_title = 'Medicare Voucher', mtcl_order_by = 'order by mdaf__timestamp desc' where mtcl_name = 'mdaf';
update mtat set mtat_input_method = 'RO' where mtat_class_name = 'mdaf';
update mtat set mtat_input_method = '10' where mtat_name = 'mdaf_voucher';
update mtat set mtat_title = 'Patient ID', mtat_displen = 15 where mtat_name = 'mdaf_patn__sequence';
update mtat set mtat_title = 'Provider', mtat_displen = 15  where mtat_name = 'mdaf_prov_code';
update mtat set mtat_title = 'Referrer', mtat_displen = 15 where mtat_name = 'mdaf_rfdr_code';
update mtat set mtat_title = 'Claimed', mtat_displen = 20 where mtat_name = 'mdaf_date_printed';
update mtat set mtat_title = 'Claim ID', mtat_displen = 10  where mtat_name = 'mdaf_mdbt__sequence';
update mtat set mtat_title = 'Authorised', mtat_displen = 5  where mtat_name = 'mdaf_benefit_authorised';

-- update history;
\set mttb_name '\'mdaf\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
