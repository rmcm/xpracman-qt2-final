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

create table apst                       -- appointment status
(
 apst_code text not null,               -- Code
 apst_desc text,                        -- Description
 apst_colour text default '#0000FF',    -- colour attribute
 --
 -- standard headers
 --
 apst__sequence serial not null,                              -- Row-ID
 apst__timestamp timestamp default now() not null,        -- Timestamp
 apst__user_entry text default CURRENT_USER not null,         -- Entered by
 apst__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(apst_code)
 );

grant select,insert,update on apst to public;
grant select,insert,update on apst_apst__sequence_seq to public;
insert into apst(apst_code, apst_desc, apst_colour) values('-', 'Default', '#FF0000');

select create_metadata('apst');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('apst','$Id$');

