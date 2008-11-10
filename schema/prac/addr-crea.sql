--     $Id$
-- 
--   Copyright 2003 X=X Computer Software Trust
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



create table addr                       -- General Address Book
(
  addr_name text DEFAULT 'New Record',                          -- Entry Name
  addr_contact text,                                            -- Contact name
  addr_organisation text,                                       -- Organisation
  addr_work_phone text,                                         -- Work - Phone
  addr_work_fax text,                                           -- Work - Fax
  addr_mobile_phone text,                                       -- Mobile phone
  addr_work_email text,                                         -- Work - email
  addr_work_www text,                                           -- Work - WWW
  addr_work_im text,                                            -- Work - Instant Messsaging
  addr_work_street text,                                        -- Work - street
  addr_work_suburb text,                                        -- Work - suburb
  addr_work_postcode text,                                      -- Work - postcode
  addr_home_email text,                                         -- Home - email
  addr_home_www text,                                           -- Home - WWW
  addr_home_im text,                                            -- Home - Instant Messsaging
  addr_home_street text,                                        -- Home - street
  addr_home_suburb text,                                        -- Home - suburb
  addr_home_postcode text,                                      -- Home - postcode
  addr_home_phone text,                                         -- Home - Phone
  addr_home_fax text,                                           -- Home - Fax
  addr_comments text,                                           -- Comments.
  --
  -- standard headers
  --
  addr__sequence serial not null,                               -- Row-ID
  addr__timestamp timestamp default now() not null,             -- Timestamp
  addr__user_entry text default CURRENT_USER not null,          -- Entered by
  addr__status char default text 'N' not null,                  -- Record status
  --
  -- constraints
  --
  primary key(addr__sequence)
 );

grant select,insert,update on addr to public;
grant select,insert,update on addr_addr__sequence_seq to public;

delete from mtcl where mtcl_name = 'addr';
select create_metadata('addr');
update mtcl set mtcl_title = 'Address Book' where mtcl_name = 'addr';

update mtat set mtat_displen = 12, mtat_title = 'Name' where mtat_name = 'addr_name';
update mtat set mtat_displen = 12, mtat_title = 'Contact' where mtat_name = 'addr_contact';
update mtat set mtat_displen = 12, mtat_title = 'Work Organisation' where mtat_name = 'addr_organisation';
update mtat set mtat_displen = 12, mtat_title = 'Work Phone' where mtat_name = 'addr_work_phone';
update mtat set mtat_displen = 12, mtat_title = 'Work Fax' where mtat_name = 'addr_work_fax';
update mtat set mtat_displen = 12, mtat_title = 'Mobile Phone' where mtat_name = 'addr_mobile_phone';
update mtat set mtat_displen = 12, mtat_title = 'Work Email' where mtat_name = 'addr_work_email';
update mtat set mtat_displen = 12, mtat_title = 'Work WWW' where mtat_name = 'addr_work_www';
update mtat set mtat_displen = 12, mtat_title = 'Work IM' where mtat_name = 'addr_work_im';
update mtat set mtat_displen = 12, mtat_title = 'Work Street' where mtat_name = 'addr_work_street';
update mtat set mtat_displen = 12, mtat_title = 'Work Suburb' where mtat_name = 'addr_work_suburb';
update mtat set mtat_displen = 12, mtat_title = 'Work Postcode' where mtat_name = 'addr_work_postcode';
update mtat set mtat_displen = 12, mtat_title = 'Home Email' where mtat_name = 'addr_home_email';
update mtat set mtat_displen = 12, mtat_title = 'Home WWW' where mtat_name = 'addr_home_www';
update mtat set mtat_displen = 12, mtat_title = 'Home IM' where mtat_name = 'addr_home_im';
update mtat set mtat_displen = 12, mtat_title = 'Home Street' where mtat_name = 'addr_home_street';
update mtat set mtat_displen = 12, mtat_title = 'Home Suburb' where mtat_name = 'addr_home_suburb';
update mtat set mtat_displen = 12, mtat_title = 'Home Postcode' where mtat_name = 'addr_home_postcode';
update mtat set mtat_displen = 12, mtat_title = 'Home Phone' where mtat_name = 'addr_home_phone';
update mtat set mtat_displen = 12, mtat_title = 'Home Fax' where mtat_name = 'addr_home_fax';
update mtat set mtat_displen = 12, mtat_title = 'Comments' where mtat_name = 'addr_comments';


-- update history;

\set mttb_name '\'addr\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;