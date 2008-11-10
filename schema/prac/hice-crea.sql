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



create table hice                       -- HIC-Online Input data elements
(
 hice_name text,                                        -- data element name
 hice_input char(1),                                    -- input element (Y/N)
 hice_output char(1),                                   -- output element (Y/N)
 hice_session char(1),                                  -- session element (Y/N)
 hice_transmission char(1),                             -- transmission element (Y/N)
 hice_length text,                                      -- data element length
 hice_type text,                                        -- data element type
 hice_desc text,                                        -- descriptive text

 --
 -- standard headers
 --
 hice__sequence serial not null,                              -- Row-ID
 hice__timestamp timestamp default now() not null,       -- Timestamp
 hice__user_entry text default CURRENT_USER not null,         -- Entered by
 hice__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(hice__sequence)
 );

grant select,insert,update on hice to public;
grant select,insert,update on hice_hice__sequence_seq to public;

select create_metadata('hice');
update mtcl set mtcl_title = 'HIC Data Elements', mtcl_userlabel = '<b>%1:</b><br>%2,hice_name,hice_desc', 
        mtcl_order_by = 'order by hice_name' where mtcl_name = 'hice';
update mtat set mtat_displen = 20, mtat_title = 'Name' where mtat_name = 'hice_name';
update mtat set mtat_displen = 5, mtat_title = 'Input' where mtat_name = 'hice_input';
update mtat set mtat_displen = 5, mtat_title = 'Output' where mtat_name = 'hice_output';
update mtat set mtat_displen = 5, mtat_title = 'Session' where mtat_name = 'hice_session';
update mtat set mtat_displen = 5, mtat_title = 'Transmission' where mtat_name = 'hice_transmission';
update mtat set mtat_displen = 5, mtat_title = 'Length' where mtat_name = 'hice_length';
update mtat set mtat_displen = 5, mtat_title = 'Type' where mtat_name = 'hice_type';
update mtat set mtat_displen = 30, mtat_title = 'Description' where mtat_name = 'hice_desc';
update mtat set mtat_input_method = 'RO' where mtat_name ~* '^hice__.*';

-- update history;
\set mttb_name '\'hice\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

